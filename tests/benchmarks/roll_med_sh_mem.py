"""
Rolling median with shared memory.
"""
import contextlib
import dataclasses
import io
import logging
import multiprocessing
import sys
import time
# Python 3.8+, need to be specific about importing this.
from multiprocessing import shared_memory
import typing

import numpy as np
import orderedstructs
import psutil

logger = logging.getLogger(__file__)


class RangePower:
    """Returns a range of values where minimum <= value <= maximum and value is
    multiplied by the multiplier at each iteration.
    Mainly used for exploration and testing.

    For example::

        rp = RangePower(8, 128, 4)
        assert list(rp) == [8, 32, 128]
    """

    def __init__(self, minimum: int, maximum: int, multiplier: int):
        if multiplier == 1:
            raise ValueError('Multiplier of 1 will make no progress.')
        self.minimum = minimum
        self.maximum = maximum
        self.multiplier = multiplier
        self.current = minimum

    def __str__(self) -> str:
        return (
            f'RangePower(minimum={self.minimum}, maximum={self.maximum},'
            f' multiplier={self.multiplier}, current={self.current})'
        )

    def __iter__(self):
        return self

    def __next__(self):
        if self.current > self.maximum:
            raise StopIteration
        else:
            temp = self.current
            self.current *= self.multiplier
            return temp


def np_info(a: np.ndarray) -> str:
    """Emulates np.info() but as a sting rather than to stdout.

    Example::

        class:  ndarray
        shape:  (1024, 4096)
        strides:  (32768, 8)
        itemsize:  8
        aligned:  True
        contiguous:  True
        fortran:  False
        data pointer: 0x10dd6b000
        byteorder:  little
        byteswap:  False
        type: float64
    """
    with io.StringIO() as file:
        np.info(a, output=file)
        return file.getvalue().rstrip()


def np_data_pointer(a: np.ndarray) -> int:
    """The address of the actual data. 'data pointer' in np.info()."""
    return a.__array_interface__["data"][0]


def np_data_read_only(a: np.ndarray) -> bool:
    """The data is read only."""
    return a.__array_interface__["data"][1]


def logger_debug_with_psutil(message_suffix: str) -> None:
    """Write process information to logger.debug"""
    if logger.getEffectiveLevel() == logging.DEBUG:
        proc = psutil.Process()
        logger.debug(
            f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d}'
            f' {message_suffix}'
        )


def rolling_median_of_column(read_array: np.ndarray,
                             window_length: int, column_index: int,
                             write_array: np.ndarray) -> int:
    """Computes a rolling median of given column and writes out the
    results to the write array.

    This is called by a child process.

    This fills the initial column values with NaN where there is
    not enough data for a rolling median of window_length.

    Will raise:
    - A ValueError if the read/write arrays do not match
      size and shape.
    - A ValueError if the read/write arrays share the
      same data.
    - A TypeError if the read array data type is not
      equivalent to a Python float or int.
    """
    if read_array.ndim != 2:
        raise ValueError(f'Read array must have dimensions of 2 not {read_array.ndim}')
    if write_array.ndim != 2:
        raise ValueError(f'Write array must have dimensions of 2 not {write_array.ndim}')
    if read_array.shape != write_array.shape:
        raise ValueError(f'Read array shape {read_array.shape} != {write_array.shape}')
    if np_data_pointer(read_array) == np_data_pointer(write_array):
        raise ValueError(
            f'Read array data pointer {np_data_pointer(read_array)}'
            f' is identical to the write array data pointer'
        )
    if read_array.dtype == np.dtype(float):
        skip_list = orderedstructs.SkipList(float)
    elif read_array.dtype == np.dtype(float):
        skip_list = orderedstructs.SkipList(int)
    else:
        raise TypeError(
            f'Read array must have numpy type compatible with float or int, not {read_array.dtype}'
        )
    logger_debug_with_psutil(f'{write_array.shape} rolling_median_of_column()  START.')
    write_count = 0
    for i in range(len(read_array)):
        value = read_array[i, column_index]
        skip_list.insert(value)
        if i >= window_length:
            median = skip_list.at(window_length // 2)
            skip_list.remove(read_array[i - window_length, column_index])
            write_count += 1
        else:
            median = np.nan

        if i == 0:
            logger_debug_with_psutil('i == 0 before write.')
        if i == 1024 ** 2 // 2:
            logger_debug_with_psutil('i == 1024**2 // 2 before write.')

        write_array[i, column_index] = median

        if i == 0:
            logger_debug_with_psutil('i == 0 after write.')
        if i == 1024 ** 2 // 2:
            logger_debug_with_psutil('i == 1024**2 // 2 after write.')
    logger_debug_with_psutil(f'skiplist length {skip_list.size():,d}.')
    logger_debug_with_psutil(f'rolling_median_of_column()  DONE.')
    return write_count


@dataclasses.dataclass
class SharedMemoryArraySpecification:
    """A PoD class that contains the data needed for
    managing a shared_memory.SharedMemory.

    Typical usage, given ``arr: np.ndarray``::

        from multiprocessing import shared_memory

        shm = shared_memory.SharedMemory(create=True, size=arr.nbytes)
        array_spec = SharedMemoryArraySpecification(arr.shape, arr.dtype, arr.nbytes, shm)
    """
    shape: typing.Tuple[int, ...]
    dtype: np.dtype
    nbytes: int
    shm: shared_memory.SharedMemory

    @property
    def name(self) -> str:
        return self.shm.name

    def __str__(self):
        return (
            f'<SharedMemoryArraySpecification'
            f' shape {self.shape}'
            f' dtype {self.dtype}'
            f' name "{self.name}"'
            f' nbytes {self.nbytes}'
            f' buffer id 0x{id(self.shm)}>'
        )

    def close(self) -> None:
        """Close the file descriptor/handle to the shared memory
        from this instance."""
        self.shm.close()

    def close_and_unlink(self) -> None:
        """Delete the underlying shared memory block.
        This should be called only once per shared memory block
        regardless of the number of handles to it, even in other
        processes."""
        self.close()
        self.shm.unlink()


@contextlib.contextmanager
def create_shared_memory_array_spec_close_unlink(
        arr: np.ndarray,
        copy_array: bool,
        log_progress: bool = False,
) -> SharedMemoryArraySpecification:
    """Context manager that creates a Shared Memory instance and,
    optionally, copies the numpy array into it.
    The Shared Memory instance is closed and unlinked on exit."""
    shm = shared_memory.SharedMemory(create=True, size=arr.nbytes)
    array_spec = SharedMemoryArraySpecification(
        arr.shape, arr.dtype, arr.nbytes, shm
    )
    if log_progress:
        logger.info('Created shared memory %s ', array_spec)
    try:
        if copy_array:
            # Copy the numpy array into shared memory.
            array_view = np.ndarray(
                array_spec.shape,
                dtype=array_spec.dtype, buffer=array_spec.shm.buf
            )
            array_view[:] = arr[:]
        yield array_spec
    finally:
        if log_progress:
            logger.info(
                'Closing and unlinking shared memory %s ', array_spec
            )
        array_spec.close_and_unlink()


@contextlib.contextmanager
def recover_array_from_shared_memory_and_close(
        array_spec: SharedMemoryArraySpecification,
) -> np.ndarray:
    """A context manager as a wrapper around a SharedMemoryArraySpecification.
    This yields a view on the numpy input or output array and ensures that the
    shared memory is closed on __exit__."""
    array_shm = shared_memory.SharedMemory(name=array_spec.name)
    array_view = np.ndarray(
        array_spec.shape, array_spec.dtype, buffer=array_shm.buf
    )
    try:
        yield array_view
    finally:
        array_shm.close()


def compute_rolling_median_2d_from_index(
        read_array_spec: SharedMemoryArraySpecification,
        window_length: int, column_index: int,
        write_array_spec: SharedMemoryArraySpecification,
) -> int:
    """Computes a rolling median of the 2D read array and
    window length and writes it to the 2D write array.

    This function is passed to multiprocessing to be invoked
    by the child process.
    """
    logger_debug_with_psutil('compute_rolling_median_2d_from_index START.')
    with recover_array_from_shared_memory_and_close(read_array_spec) as read_array:
        with recover_array_from_shared_memory_and_close(write_array_spec) as write_array:
            logger_debug_with_psutil('compute_rolling_median_2d_from_index rolling median calculation START.')

            write_count = rolling_median_of_column(
                read_array, window_length, column_index, write_array
            )

            logger_debug_with_psutil('compute_rolling_median_2d_from_index rolling median calculation DONE.')
    logger_debug_with_psutil('compute_rolling_median_2d_from_index DONE.')
    return write_count


def copy_shared_memory_into_new_numpy_array(
        write_array_spec: SharedMemoryArraySpecification,
) -> np.ndarray:
    """With the output SharedMemoryArraySpecification
    this creates a new numpy array and copies the shared memory into it."""
    temp_write = np.ndarray(
        write_array_spec.shape,
        dtype=write_array_spec.dtype,
        buffer=write_array_spec.shm.buf
    )
    write_array = np.empty(
        write_array_spec.shape,
        dtype=write_array_spec.dtype,
    )
    write_array[:] = temp_write[:]
    return write_array


def compute_rolling_median_2d_mp(
        read_array: np.ndarray,
        window_length: int, num_processes: int,
        log_progress: bool = False,
) -> np.ndarray:
    """Compute a rolling median of a numpy 2D array using
    multiprocessing and shared memory.
    This is the top level call and is run within the parent process.

    This returns a np.ndarray of the same shape as the input with the
    rolling medians.
    Rows of this up to the window_length will be NaN.

    If log_progress is True then this will log info/debug results.
    """
    if read_array.ndim != 2:
        raise ValueError(f'Array  must be 2D not {read_array.shape}')
    # Limit number of processes if the number of columns is small.
    if read_array.shape[1] < num_processes:
        num_processes = read_array.shape[1]
    if log_progress:
        logger.info(
            'compute_rolling_median_2d(): array shape %s window length %d with %d processes',
            read_array.shape, window_length, num_processes
        )
        proc = psutil.Process()
        logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} START')
        logger.info(f'read_array data @ 0x{np_data_pointer(read_array):x}')
        logger.debug(f'read_array info\n{np_info(read_array)}')
    with create_shared_memory_array_spec_close_unlink(
            read_array, copy_array=True, log_progress=log_progress,
    ) as read_array_spec:
        with create_shared_memory_array_spec_close_unlink(
                read_array, copy_array=False, log_progress=log_progress,
        ) as write_array_spec:
            if log_progress:
                logger.info(
                    f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d}'
                    f' - after creating read+write shm'
                )

            # Create the tasks.
            tasks = []
            for column_index in range(read_array.shape[1]):
                tasks.append(
                    (read_array_spec, window_length, column_index, write_array_spec)
                )

            # Create the pool and apply the tasks.
            mp_pool = multiprocessing.Pool(processes=num_processes)
            pool_apply = [
                mp_pool.apply_async(compute_rolling_median_2d_from_index, t) for t in tasks
            ]
            _write_counts = [r.get() for r in pool_apply]
            write_array = copy_shared_memory_into_new_numpy_array(write_array_spec)

            if log_progress:
                logger.debug(f'compute_rolling_median_2d(): Results: {_write_counts}')
                logger.info(f'write_array data @ 0x{np_data_pointer(write_array):x}')
                logger.info(
                    f'Parent {proc.pid} memory'
                    f' RSS: {proc.memory_info().rss:,d} - before closing and unlinking read+write shm.'
                )
    if log_progress:
        logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} DONE')
        logger.info(
            'compute_rolling_median_2d(): DONE array shape %s window length %d with %d processes',
            read_array.shape, window_length, num_processes
        )
    return write_array


def main() -> int:
    """This provides illustrative code for computing a rolling median with shared memory."""
    logging.basicConfig(
        level=logging.INFO,
        format=(
            '%(asctime)s - %(filename)24s#%(lineno)-4d - %(process)5d'
            ' - (%(threadName)-10s) - %(levelname)-8s - %(message)s'
        ),
        stream=sys.stdout,
    )
    logger.setLevel(logging.INFO)
    logger.info(f'Process memory RSS: {psutil.Process().memory_info().rss:,d}')
    ROWS = 1024 * 1024  # 1024 * 1
    # ROWS = 1024
    COLUMNS = 16  # 1024 * 4 # 8
    ROLLING_MEDIAN_WINDOW_SIZE = 21
    logger.info(
        f'Configuration: rows {ROWS} columns {COLUMNS} window size {ROLLING_MEDIAN_WINDOW_SIZE}'
    )
    logger.info(f'CPU count {multiprocessing.cpu_count()}')
    logger.info(f'orderedstructs version {orderedstructs.__version__}')

    read_array = np.random.random((ROWS, COLUMNS))
    timings: typing.Dict[int, float] = {}
    for p in RangePower(1, 16, 2):
        logger.info(f' Processes {p} '.center(75, '-'))
        t_start = time.perf_counter()

        # This is the essential function call.
        _write_array = compute_rolling_median_2d_mp(
            read_array, window_length=ROLLING_MEDIAN_WINDOW_SIZE, num_processes=p,
        )

        t_elapsed = time.perf_counter() - t_start
        logger.info(f' DONE Processes {p} '.center(75, '-'))
        timings[p] = t_elapsed
    print(f'Array shape {read_array.shape} size {read_array.size:,d}')
    time_1_cpu = timings[1]
    for p in sorted(timings.keys()):
        print(
            f'compute_rolling_median():'
            f' Processes: {p:2d}'
            f' Time: {timings[p]:8.3f} (s)'
            f' Time x processes: {timings[p] * p:8.3f} (s)'
            f' rate {read_array.size / timings[p]:14,.0f} values/s'
            f' c.f. one CPU {time_1_cpu / timings[p]:8.3f}'
        )
    print('Bye, bye!')
    return 0


if __name__ == '__main__':
    exit(main())
