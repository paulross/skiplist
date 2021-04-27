"""
Example output::

2021-04-23 10:49:39,638 - mp_rolling_median_three.py#206  -  7114 - (MainThread) - INFO     - CPU count 8
...
Array shape (1048576, 16) size 16,777,216
compute_rolling_median(): Processes:  1 Time:   20.844 (s) Time x processes:   20.844 (s) rate        804,911 values/s c.f. one CPU    1.000
compute_rolling_median(): Processes:  2 Time:   11.466 (s) Time x processes:   22.931 (s) rate      1,463,268 values/s c.f. one CPU    1.818
compute_rolling_median(): Processes:  3 Time:   10.274 (s) Time x processes:   30.822 (s) rate      1,633,002 values/s c.f. one CPU    2.029
compute_rolling_median(): Processes:  4 Time:    9.347 (s) Time x processes:   37.389 (s) rate      1,794,905 values/s c.f. one CPU    2.230
compute_rolling_median(): Processes:  5 Time:    7.745 (s) Time x processes:   38.725 (s) rate      2,166,184 values/s c.f. one CPU    2.691
compute_rolling_median(): Processes:  6 Time:    6.701 (s) Time x processes:   40.203 (s) rate      2,503,853 values/s c.f. one CPU    3.111
compute_rolling_median(): Processes:  7 Time:    7.351 (s) Time x processes:   51.458 (s) rate      2,282,270 values/s c.f. one CPU    2.835
compute_rolling_median(): Processes:  8 Time:    6.642 (s) Time x processes:   53.133 (s) rate      2,526,056 values/s c.f. one CPU    3.138
compute_rolling_median(): Processes:  9 Time:    6.640 (s) Time x processes:   59.759 (s) rate      2,526,730 values/s c.f. one CPU    3.139
compute_rolling_median(): Processes: 10 Time:    6.739 (s) Time x processes:   67.395 (s) rate      2,489,388 values/s c.f. one CPU    3.093
compute_rolling_median(): Processes: 11 Time:    6.829 (s) Time x processes:   75.118 (s) rate      2,456,794 values/s c.f. one CPU    3.052
compute_rolling_median(): Processes: 12 Time:    6.823 (s) Time x processes:   81.873 (s) rate      2,459,004 values/s c.f. one CPU    3.055
compute_rolling_median(): Processes: 13 Time:    7.140 (s) Time x processes:   92.818 (s) rate      2,349,808 values/s c.f. one CPU    2.919
compute_rolling_median(): Processes: 14 Time:    7.427 (s) Time x processes:  103.976 (s) rate      2,258,993 values/s c.f. one CPU    2.807
compute_rolling_median(): Processes: 15 Time:    7.914 (s) Time x processes:  118.716 (s) rate      2,119,826 values/s c.f. one CPU    2.634
compute_rolling_median(): Processes: 16 Time:    6.794 (s) Time x processes:  108.706 (s) rate      2,469,368 values/s c.f. one CPU    3.068
Bye, bye!
Process finished with exit code 0
"""
import contextlib
import io
import logging
import multiprocessing
import sys
import time
import typing
from multiprocessing import shared_memory

import numpy as np

import orderedstructs
import psutil
import pytest

logger = logging.getLogger(__file__)


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


def rolling_median_of_column(read_array: np.ndarray, window_length: int, column_index: int,
                             write_array: np.ndarray) -> int:
    """Computes a rolling median of given column and writes out the results to the write array.
    Called by a child process."""
    assert read_array.ndim == 2
    assert write_array.ndim == 2
    assert read_array.shape == write_array.shape
    proc = psutil.Process()
    print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} {write_array.shape} rolling_median_of_column()  START.')
    skip_list = orderedstructs.SkipList(float)
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
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} i == 0 before write.')
        if i == 1024**2 // 2:
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} i == 1024**2 // 2 before write.')
        write_array[i, column_index] = median
        if i == 0:
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} i == 0 after write.')
        if i == 1024**2 // 2:
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} i == 1024**2 // 2 after write.')
    print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} skiplist length {skip_list.size():,d}.')
    print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} rolling_median_of_column()  DONE.')
    return write_count


class SharedMemoryArraySpecification(typing.NamedTuple):
    shape: typing.Tuple[int, ...]
    dtype: np.dtype
    nbytes: int
    shm: shared_memory.SharedMemory

    @property
    def name(self) -> str:
        return self.shm.name

    def __str__(self):
        return (
            f'<SharedMemoryArraySpecification shape {self.shape} dtype {self.dtype} name "{self.name}"'
            f' nbytes {self.nbytes} buffer id 0x{id(self.shm)}>'
        )

    def close(self) -> None:
        self.shm.close()

    def close_and_unlink(self) -> None:
        self.close()
        self.shm.unlink()


@contextlib.contextmanager
def recover_array_from_shared_memory_and_close(array_spec: SharedMemoryArraySpecification) -> np.ndarray:
    array_shm = shared_memory.SharedMemory(name=array_spec.name)
    array_view = np.ndarray(array_spec.shape, array_spec.dtype, buffer=array_shm.buf)
    try:
        yield array_view
    finally:
        array_shm.close()


def compute_rolling_median_2d_from_index(read_array_spec: SharedMemoryArraySpecification, window_length: int,
                                         column_index: int,
                                         write_array_spec: SharedMemoryArraySpecification) -> int:
    """Computes a rolling median of the 2D read array and window length and writes it to the 2D write array.
    This is invoked as a child process."""
    proc = psutil.Process()
    print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} START.')
    with recover_array_from_shared_memory_and_close(read_array_spec) as read_array:
        with recover_array_from_shared_memory_and_close(write_array_spec) as write_array:
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} rolling median calculation START.')
            write_count = rolling_median_of_column(read_array, window_length, column_index, write_array)
            print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} rolling median calculation DONE.')
    print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} DONE.')
    return write_count


@contextlib.contextmanager
def create_read_shared_memory_array_spec_close_unlink(arr: np.ndarray) -> SharedMemoryArraySpecification:
    """Context manager that creates a Shared Memory instance and copies the numpy array into it.
    The Shared Memory instance is closed and unlinked on exit."""
    shm = shared_memory.SharedMemory(create=True, size=arr.nbytes)
    array_spec = SharedMemoryArraySpecification(arr.shape, arr.dtype, arr.nbytes, shm)
    logger.info('Created shared memory %s ', array_spec)
    try:
        # Copy the numpy array into shared memory.
        array_view = np.ndarray(array_spec.shape, dtype=array_spec.dtype, buffer=array_spec.shm.buf)
        array_view[:] = arr[:]
        yield array_spec
    finally:
        array_spec.close_and_unlink()


@contextlib.contextmanager
def create_write_shared_memory_array_spec_close_unlink(arr: np.ndarray) -> SharedMemoryArraySpecification:
    """Context manager that creates a Shared Memory instance for writing to a numpy array.
    The numpy array can be recovered with copy_shared_memory_into_new_numpy_array().
    The Shared Memory instance is closed and unlinked on exit."""
    shm = shared_memory.SharedMemory(create=True, size=arr.nbytes)
    array_spec = SharedMemoryArraySpecification(arr.shape, arr.dtype, arr.nbytes, shm)
    logger.info('Created shared memory %s ', array_spec)
    try:
        yield array_spec
    finally:
        array_spec.close_and_unlink()


def copy_shared_memory_into_new_numpy_array(write_array_spec: SharedMemoryArraySpecification) -> np.ndarray:
    """With the output SharedMemoryArraySpecification this creates a new numpy array and copies the shared memory into it."""
    temp_write = np.ndarray(write_array_spec.shape, dtype=write_array_spec.dtype, buffer=write_array_spec.shm.buf)
    write_array = np.empty(write_array_spec.shape, dtype=write_array_spec.dtype)
    write_array[:] = temp_write[:]
    return write_array


def compute_rolling_median_2d_mp(read_array: np.ndarray, window_length: int, num_processes: int) -> np.ndarray:
    """Compute a rolling median of a numpy 2D array using multiprocessing and shared memory.
    This is run as the parent process."""
    if read_array.ndim != 2:
        raise ValueError(f'Array  must be 2D not {read_array.shape}')
    # Limit number of processes if the number of columns is small.
    if read_array.shape[1] < num_processes:
        num_processes = read_array.shape[1]
    logger.info('compute_rolling_median_2d(): array shape %s window length %d with %d processes', read_array.shape,
                window_length, num_processes)
    proc = psutil.Process()
    logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} START')
    logger.info(f'read_array data @ 0x{np_data_pointer(read_array):x}')
    with create_read_shared_memory_array_spec_close_unlink(read_array) as read_array_spec:
        with create_write_shared_memory_array_spec_close_unlink(read_array) as write_array_spec:
            logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} - after creating read+write shm')
            mp_pool = multiprocessing.Pool(processes=num_processes)
            tasks = []
            for column_index in range(read_array.shape[1]):
                tasks.append((read_array_spec, window_length, column_index, write_array_spec))
            pool_apply = [mp_pool.apply_async(compute_rolling_median_2d_from_index, t) for t in tasks]
            results = [r.get() for r in pool_apply]
            logger.debug(f'compute_rolling_median_2d(): Results: {results}')
            write_array = copy_shared_memory_into_new_numpy_array(write_array_spec)
            logger.info(f'write_array data @ 0x{np_data_pointer(write_array):x}')
            logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} - before closing and unlinking read+write shm.')
    logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} DONE')
    logger.info('compute_rolling_median_2d(): DONE array shape %s window length %d with %d processes', read_array.shape,
                window_length, num_processes)
    return write_array


@pytest.mark.parametrize(
    'rows, columns, window_length, processes',
    (
            (1000, 10, 21, 1),
    )
)
def _test_compute_rolling_median_2d_mp(rows, columns, window_length, processes):
    read_array = np.random.random((rows, columns))
    tim_start = time.perf_counter()
    result = compute_rolling_median_2d_mp(read_array, window_length, processes)
    tim_exec = time.perf_counter() - tim_start
    assert result.shape == read_array.shape


@pytest.mark.parametrize(
    'rows, columns, process_range',
    (
            (100, 4, range(1, 5)),
            (1000, 4, range(1, 5)),
            (10000, 4, range(1, 5)),
            (100000, 4, range(1, 5)),
            (1000000, 4, range(1, 5)),
    )
)
def _test_rm_2d_mp_time(rows, columns, process_range):
    read_array = np.random.random((rows, columns))
    print()
    for p in process_range:
        tim_start = time.perf_counter()
        result = compute_rolling_median_2d_mp(read_array, 21, p)
        tim_exec = time.perf_counter() - tim_start
        print(f'Rows: {rows:8d} Columns: {columns:8d} Processes: {p:8d} Time: {tim_exec:8.3f}')
    assert 0


@pytest.mark.parametrize(
    'start_rows, end_rows, process_range',
    (
            (64, 1024 ** 2, range(1, 17)),
    )
)
def _test_rm_2d_mp_time_b(start_rows, end_rows, process_range):
    rows = start_rows
    columns = 16
    print()
    print(f'{"rows":8} {"columns":8} {"processes":8} {"tim_exec":8}')
    # results is {rows : {processes : time, ...}, ...}
    results = {}
    while rows <= end_rows:
        read_array = np.random.random((rows, columns))
        for p in process_range:
            tim_start = time.perf_counter()
            _result = compute_rolling_median_2d_mp(read_array, 21, p)
            tim_exec = time.perf_counter() - tim_start
            print(f'{rows:8d} {columns:8d} {p:8d} {tim_exec:8.3f}')
            if rows not in results:
                results[rows] = {}
            results[rows][p] = tim_exec
        rows *= 2
    # gnuplot dat output
    print()
    print('# Raw times')
    print(f'# {"Rows":6}', end='')
    for p in process_range:
        s = f'p={p}'
        print(f' {s:>8}', end='')
    print()
    for row in sorted(results.keys()):
        print(f'# {row:<8d}', end='')
        for p in sorted(results[row].keys()):
            print(f' {results[row][p]:8.3f}', end='')
        print()
    print()
    print('# Speedup factor over a single process.')
    print(f'# {"Rows":6}', end='')
    for p in process_range:
        s = f'p={p}'
        print(f' {s:>8}', end='')
    print()
    for row in sorted(results.keys()):
        print(f'{row:<8d}', end='')
        for p in sorted(results[row].keys()):
            print(f' {results[row][1] / results[row][p]:8.3f}', end='')
        print()
    assert 0


def main() -> int:  # pragma: no cover
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(filename)24s#%(lineno)-4d - %(process)5d - (%(threadName)-10s) - %(levelname)-8s - %(message)s',
                        stream=sys.stdout)
    logger.setLevel(logging.INFO)
    logger.info(f'Process memory RSS: {psutil.Process().memory_info().rss:,d}')
    ROWS = 1024 * 1024  # 1024 * 1
    COLUMNS = 16  # 1024 * 4 # 8
    ROLLING_MEDIAN_WINDOW_SIZE = 21
    logger.info(f'Configuration: rows {ROWS} columns {COLUMNS} window size {ROLLING_MEDIAN_WINDOW_SIZE}')
    logger.info(f'CPU count {multiprocessing.cpu_count()}')
    logger.info(f'orderedstructs version {orderedstructs.__version__}')

    read_array = np.random.random((ROWS, COLUMNS))
    read_array_size = read_array.size
    timings: typing.Dict[int, float] = {}
    for p in range(1, 2): # 17):
        logger.info(f' Processes {p} '.center(75, '-'))
        t_start = time.perf_counter()
        write_array = compute_rolling_median_2d_mp(read_array, window_length=ROLLING_MEDIAN_WINDOW_SIZE,
                                                   num_processes=p)
        t_elapsed = time.perf_counter() - t_start
        logger.info(f' DONE Processes {p} '.center(75, '-'))
        timings[p] = t_elapsed
    print(f'Array shape {read_array.shape} size {read_array_size:,d}')
    time_1_cpu = timings[1]
    for p in sorted(timings.keys()):
        print(
            f'compute_rolling_median():'
            f' Processes: {p:2d}'
            f' Time: {timings[p]:8.3f} (s)'
            f' Time x processes: {timings[p] * p:8.3f} (s)'
            f' rate {read_array_size / timings[p]:14,.0f} values/s'
            f' c.f. one CPU {time_1_cpu / timings[p]:8.3f}'
        )
    print('Bye, bye!')
    return 0


if __name__ == '__main__':  # pragma: no cover
    sys.exit(main())
