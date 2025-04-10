"""
Not really a test, more of an exploration. Python 3.8+

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
import math
import multiprocessing
# Python 3.8+, need to be specific about importing this.
from multiprocessing import shared_memory
import pprint
import sys
import time
import typing

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


def _print(*args, **kwargs):
    pass
    # print(*args, **kwargs)


def rolling_median_of_column(read_array: np.ndarray, window_length: int, column_index: int,
                             write_array: np.ndarray) -> int:
    """Computes a rolling median of given column and writes out the results to the write array.
    Called by a child process."""
    assert read_array.ndim == 2
    assert write_array.ndim == 2
    assert read_array.shape == write_array.shape
    proc = psutil.Process()
    _print(
        f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d}'
        f' {write_array.shape} rolling_median_of_column()  START.'
    )
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
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} i == 0 before write.'
            )
        if i == 1024 ** 2 // 2:
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} i == 1024**2 // 2 before write.'
            )
        write_array[i, column_index] = median
        if i == 0:
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} i == 0 after write.'
            )
        if i == 1024 ** 2 // 2:
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} i == 1024**2 // 2 after write.'
            )
    _print(
        f'Child process {proc.pid} from parent {proc.ppid()}'
        f' RSS: {proc.memory_info().rss:,d} skiplist length {skip_list.size():,d}.'
    )
    _print(
        f'Child process {proc.pid} from parent {proc.ppid()}'
        f' RSS: {proc.memory_info().rss:,d} rolling_median_of_column()  DONE.'
    )
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
    _print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} START.')
    with recover_array_from_shared_memory_and_close(read_array_spec) as read_array:
        with recover_array_from_shared_memory_and_close(write_array_spec) as write_array:
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} rolling median calculation START.'
            )
            write_count = rolling_median_of_column(read_array, window_length, column_index, write_array)
            _print(
                f'Child process {proc.pid} from parent {proc.ppid()}'
                f' RSS: {proc.memory_info().rss:,d} rolling median calculation DONE.'
            )
    _print(f'Child process {proc.pid} from parent {proc.ppid()} RSS: {proc.memory_info().rss:,d} DONE.')
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
    """With the output SharedMemoryArraySpecification this creates a new numpy array and copies
    the shared memory into it."""
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
    logger.info(
        'compute_rolling_median_2d(): array shape %s window length %d with %d processes',
        read_array.shape, window_length, num_processes
    )
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
            logger.info(
                f'Parent {proc.pid} memory'
                f' RSS: {proc.memory_info().rss:,d} - before closing and unlinking read+write shm.'
            )
    logger.info(f'Parent {proc.pid} memory RSS: {proc.memory_info().rss:,d} DONE')
    logger.info(
        'compute_rolling_median_2d(): DONE array shape %s window length %d with %d processes',
        read_array.shape, window_length, num_processes
    )
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
    _print()
    for p in process_range:
        tim_start = time.perf_counter()
        result = compute_rolling_median_2d_mp(read_array, 21, p)
        tim_exec = time.perf_counter() - tim_start
        _print(f'Rows: {rows:8d} Columns: {columns:8d} Processes: {p:8d} Time: {tim_exec:8.3f}')
    assert 0


# s = 10 * 1024 ** 2 * 16
# f'{s:,d}'
# '167,772,160'
# f'{s*8:,d}'
# '1,342,177,280'
NUMPY_SIZE = 8 * 1024 ** 2 * 16
NUMPY_SIZE = 10 * 1024 ** 2 * 16


@pytest.mark.parametrize(
    'min_size, max_size, columns, process_range',
    (
            # (1024, NUMPY_SIZE, 16, 16),
            # (1024, NUMPY_SIZE, 1024, 16),
            (1024, NUMPY_SIZE, 16 * 1024, 16),
    )
)
def test_rm_2d_mp_time_b(min_size, max_size, columns, max_processes):
    rm_2d_mp_time_b(min_size, max_size, columns, max_processes)
    assert 0


def rm_2d_mp_time_b(row_range, columns, max_processes):
    """"""
    # NUM_POINTS = 8
    # rows = min_size // columns
    # rows_max = max_size // columns
    # rows_inc = 2**int(0.5 + (int(math.log2(rows_max)) - int(math.log2(rows))) / NUM_POINTS)
    # rows_inc = max(rows_inc, 2)
    print()
    print(
        f'Rows {row_range}'
        f' columns {columns}'
        f' max processes {max_processes}'
    )
    print(f'{"rows":8} {"columns":8} {"processes":8} {"tim_exec":8}')
    # results is {rows : {processes : time, ...}, ...}
    results = {}
    processes = set()
    tim_overall = time.perf_counter()
    for rows in row_range:
        read_array = np.random.random((rows, columns))
        p = 1
        while p <= max_processes:
            tim_start = time.perf_counter()
            _result = compute_rolling_median_2d_mp(read_array, 21, p)
            tim_exec = time.perf_counter() - tim_start
            print(f'{rows:8d} {columns:8d} {p:8d} {tim_exec:8.3f}')
            if rows not in results:
                results[rows] = {}
            results[rows][p] = tim_exec
            processes.add(p)
            p *= 2
        # rows *= rows_inc
        # if rows > 64:
        #     break
    print()
    # gnuplot dat output
    print(f'# Raw times columns={columns}')
    print(f'# {"Rows":6}', end='')
    for p in sorted(processes):
        s = f'p={p}'
        print(f' {s:>8}', end='')
    print()
    for row in sorted(results.keys()):
        print(f'{row:<8d}', end='')
        for p in sorted(results[row].keys()):
            print(f' {results[row][p]:8.3f}', end='')
        print()
    print()
    print(f'# Speedup factor over a single process. columns={columns}')
    print(f'# {"Rows":6}', end='')
    for p in processes:
        s = f'p={p}'
        print(f' {s:>8}', end='')
    print()
    for row in sorted(results.keys()):
        print(f'{row:<8d}', end='')
        for p in sorted(results[row].keys()):
            print(f' {results[row][1] / results[row][p]:8.3f}', end='')
        print()
    tim_exec_overall = time.perf_counter() - tim_overall
    print(f'Overall time: {tim_exec_overall:8.3f}')
    return results


def experiment() -> int:  # pragma: no cover
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
    COLUMNS = 16  # 1024 * 4 # 8
    ROLLING_MEDIAN_WINDOW_SIZE = 21
    logger.info(
        f'Configuration: rows {ROWS} columns {COLUMNS} window size {ROLLING_MEDIAN_WINDOW_SIZE}'
    )
    logger.info(f'CPU count {multiprocessing.cpu_count()}')
    logger.info(f'orderedstructs version {orderedstructs.__version__}')

    read_array = np.random.random((ROWS, COLUMNS))
    read_array_size = read_array.size
    timings: typing.Dict[int, float] = {}
    for p in range(1, 2):  # 17):
        logger.info(f' Processes {p} '.center(75, '-'))
        t_start = time.perf_counter()
        write_array = compute_rolling_median_2d_mp(
            read_array, window_length=ROLLING_MEDIAN_WINDOW_SIZE, num_processes=p,
        )
        t_elapsed = time.perf_counter() - t_start
        logger.info(f' DONE Processes {p} '.center(75, '-'))
        timings[p] = t_elapsed
    _print(f'Array shape {read_array.shape} size {read_array_size:,d}')
    time_1_cpu = timings[1]
    for p in sorted(timings.keys()):
        _print(
            f'compute_rolling_median():'
            f' Processes: {p:2d}'
            f' Time: {timings[p]:8.3f} (s)'
            f' Time x processes: {timings[p] * p:8.3f} (s)'
            f' rate {read_array_size / timings[p]:14,.0f} values/s'
            f' c.f. one CPU {time_1_cpu / timings[p]:8.3f}'
        )
    _print('Bye, bye!')
    return 0


def range_power(minimum, maximum, multiplier):
    value = minimum
    while value <= maximum:
        yield value
        value *= multiplier


def dump_results(results):
    # results is {columns : {rows : {processes : time, ...}, ...}, ...}
    # Make a gnuplot table
    print(f'TRACE: results:\n{pprint.pprint(results)}')
    rows = set()
    for columns in results:
        rows |= set(results[columns].keys())
    processes = set()
    for columns in results:
        for row in results[columns]:
            processes |= set(results[columns][row].keys())
    # Now arrange in a table
    for row_count in sorted(rows):
        for column_count in sorted(results.keys()):
            tim_unity = results[column_count][row_count][1]
            for process_count in sorted(processes):
                try:
                    tim = results[column_count][row_count][process_count]
                except KeyError:
                    tim = math.nan


def main() -> int:  # pragma: no cover
    # experiment()
    # pytest.main()
    print(f'NUMPY_SIZE: {NUMPY_SIZE:,d}')
    print(f'multiprocessing.cpu_count(): {multiprocessing.cpu_count()}')
    # results is {columns : {rows : {processes : time, ...}, ...}, ...}
    results = {}
    # Simple test:
    # results[16] = rm_2d_mp_time_b(range_power(128, 8388608 // 256, 4), 16, 16)
    results[16] = rm_2d_mp_time_b(range_power(128, 8388608, 4), 16, 16)
    results[1024] = rm_2d_mp_time_b(range_power(32, 131072, 4), 1024, 16)
    results[64 * 1024] = rm_2d_mp_time_b(range_power(32, 4096, 2), 64 * 1024, 16)
    # dump_results(results)
    return 0


if __name__ == '__main__':  # pragma: no cover
    sys.exit(main())

"""
(SkipList_3.10_rel) ➜  skiplist git:(develop) ✗
(SkipList_3.10_rel) ➜  skiplist git:(develop) ✗ caffeinate time python tests/unit/_test_rolling_median_shared_memory.py
NUMPY_SIZE: 167,772,160
multiprocessing.cpu_count(): 8

Rows <generator object range_power at 0x1107c1700> columns 16 max processes 16
rows     columns  processes tim_exec
     128       16        1    0.361
     128       16        2    0.453
     128       16        4    0.560
     128       16        8    0.885
     128       16       16    1.428
     512       16        1    0.366
     512       16        2    0.404
     512       16        4    0.507
     512       16        8    0.783
     512       16       16    1.470
    2048       16        1    0.360
    2048       16        2    0.438
    2048       16        4    0.559
    2048       16        8    0.752
    2048       16       16    1.517
    8192       16        1    0.509
    8192       16        2    0.514
    8192       16        4    0.585
    8192       16        8    0.929
    8192       16       16    1.696
   32768       16        1    0.943
   32768       16        2    0.736
   32768       16        4    0.717
   32768       16        8    0.994
   32768       16       16    1.785
  131072       16        1    2.654
  131072       16        2    1.629
  131072       16        4    1.284
  131072       16        8    1.405
  131072       16       16    2.188
  524288       16        1   10.122
  524288       16        2    5.497
  524288       16        4    3.429
  524288       16        8    3.113
  524288       16       16    3.876
 2097152       16        1   39.786
 2097152       16        2   20.230
 2097152       16        4   12.021
 2097152       16        8    9.821
 2097152       16       16   10.712
 8388608       16        1  161.122
 8388608       16        2   80.830
 8388608       16        4   50.502
 8388608       16        8   39.111
 8388608       16       16   39.548

# Raw times columns=16
# Rows        p=1      p=2      p=4      p=8     p=16
128         0.361    0.453    0.560    0.885    1.428
512         0.366    0.404    0.507    0.783    1.470
2048        0.360    0.438    0.559    0.752    1.517
8192        0.509    0.514    0.585    0.929    1.696
32768       0.943    0.736    0.717    0.994    1.785
131072      2.654    1.629    1.284    1.405    2.188
524288     10.122    5.497    3.429    3.113    3.876
2097152    39.786   20.230   12.021    9.821   10.712
8388608   161.122   80.830   50.502   39.111   39.548

# Speedup factor over a single process. columns=16
# Rows        p=1      p=2      p=4      p=8     p=16
128         1.000    0.797    0.645    0.408    0.253
512         1.000    0.906    0.722    0.467    0.249
2048        1.000    0.823    0.644    0.479    0.237
8192        1.000    0.990    0.870    0.548    0.300
32768       1.000    1.282    1.316    0.949    0.528
131072      1.000    1.630    2.068    1.889    1.213
524288      1.000    1.841    2.952    3.252    2.611
2097152     1.000    1.967    3.310    4.051    3.714
8388608     1.000    1.993    3.190    4.120    4.074
Overall time:  520.728

Rows <generator object range_power at 0x1107c1700> columns 1024 max processes 16
rows     columns  processes tim_exec
      32     1024        1    0.624
      32     1024        2    0.596
      32     1024        4    0.688
      32     1024        8    0.999
      32     1024       16    1.694
     128     1024        1    0.978
     128     1024        2    0.942
     128     1024        4    0.996
     128     1024        8    1.239
     128     1024       16    2.186
     512     1024        1    2.149
     512     1024        2    1.236
     512     1024        4    1.032
     512     1024        8    1.392
     512     1024       16    2.014
    2048     1024        1    6.414
    2048     1024        2    3.653
    2048     1024        4    2.656
    2048     1024        8    2.932
    2048     1024       16    3.680
    8192     1024        1   26.529
    8192     1024        2   13.660
    8192     1024        4    8.239
    8192     1024        8    8.583
    8192     1024       16    9.813
   32768     1024        1  104.275
   32768     1024        2   55.218
   32768     1024        4   31.499
   32768     1024        8   36.608
   32768     1024       16   41.076
  131072     1024        1  442.605
  131072     1024        2  212.931
  131072     1024        4  135.094
  131072     1024        8  138.338
  131072     1024       16  162.729

# Raw times columns=1024
# Rows        p=1      p=2      p=4      p=8     p=16
32          0.624    0.596    0.688    0.999    1.694
128         0.978    0.942    0.996    1.239    2.186
512         2.149    1.236    1.032    1.392    2.014
2048        6.414    3.653    2.656    2.932    3.680
8192       26.529   13.660    8.239    8.583    9.813
32768     104.275   55.218   31.499   36.608   41.076
131072    442.605  212.931  135.094  138.338  162.729

# Speedup factor over a single process. columns=1024
# Rows        p=1      p=2      p=4      p=8     p=16
32          1.000    1.048    0.907    0.625    0.368
128         1.000    1.038    0.981    0.789    0.447
512         1.000    1.739    2.081    1.543    1.067
2048        1.000    1.756    2.415    2.188    1.743
8192        1.000    1.942    3.220    3.091    2.703
32768       1.000    1.888    3.310    2.848    2.539
131072      1.000    2.079    3.276    3.199    2.720
Overall time: 1467.364

Rows <generator object range_power at 0x1107c1700> columns 65536 max processes 16
rows     columns  processes tim_exec
      32    65536        1   21.930
      32    65536        2   12.227
      32    65536        4    7.962
      32    65536        8    8.206
      32    65536       16   10.410
      64    65536        1   30.561
      64    65536        2   16.497
      64    65536        4   10.312
      64    65536        8   10.217
      64    65536       16   12.060
     128    65536        1   47.422
     128    65536        2   25.378
     128    65536        4   15.631
     128    65536        8   16.004
     128    65536       16   17.241
     256    65536        1   79.608
     256    65536        2   43.358
     256    65536        4   26.144
     256    65536        8   25.120
     256    65536       16   27.194
     512    65536        1  147.709
     512    65536        2   79.879
     512    65536        4   48.939
     512    65536        8   47.238
     512    65536       16   49.634
    1024    65536        1  300.651
    1024    65536        2  184.718
    1024    65536        4  115.465
    1024    65536        8   99.105
    1024    65536       16   99.699
    2048    65536        1  617.459
    2048    65536        2  342.996
    2048    65536        4  211.434
    2048    65536        8  203.465
    2048    65536       16  200.316
    4096    65536        1 1424.802
    4096    65536        2  833.940
    4096    65536        4  553.641
    4096    65536        8  475.938
    4096    65536       16  498.710

# Raw times columns=65536
# Rows        p=1      p=2      p=4      p=8     p=16
32         21.930   12.227    7.962    8.206   10.410
64         30.561   16.497   10.312   10.217   12.060
128        47.422   25.378   15.631   16.004   17.241
256        79.608   43.358   26.144   25.120   27.194
512       147.709   79.879   48.939   47.238   49.634
1024      300.651  184.718  115.465   99.105   99.699
2048      617.459  342.996  211.434  203.465  200.316
4096     1424.802  833.940  553.641  475.938  498.710

# Speedup factor over a single process. columns=65536
# Rows        p=1      p=2      p=4      p=8     p=16
32          1.000    1.793    2.754    2.672    2.107
64          1.000    1.853    2.964    2.991    2.534
128         1.000    1.869    3.034    2.963    2.750
256         1.000    1.836    3.045    3.169    2.927
512         1.000    1.849    3.018    3.127    2.976
1024        1.000    1.628    2.604    3.034    3.016
2048        1.000    1.800    2.920    3.035    3.082
4096        1.000    1.709    2.574    2.994    2.857
Overall time: 7004.139
     8993.03 real     10594.95 user     17169.47 sys
(SkipList_3.10_rel) ➜  skiplist git:(develop) ✗
"""