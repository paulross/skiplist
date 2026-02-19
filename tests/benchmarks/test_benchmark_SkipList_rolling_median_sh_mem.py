"""
Benchmark tests for Python rolling median with shared memory. Typical usage:

pytest tests/benchmarks/test_benchmark_SkipList_rolling_median_sh_mem.py --runslow --runveryslow --benchmark-sort=name --benchmark-autosave --benchmark-histogram -v
"""
import numpy as np

import pytest

from tests.benchmarks import roll_med_sh_mem


# Low level tests such as copying


def _create_shared_memory_array_spec_close_unlink(array: np.ndarray, copy_array: bool) -> None:
    with roll_med_sh_mem.create_shared_memory_array_spec_close_unlink(
            array, copy_array=copy_array, log_progress=False,
    ) as read_array_spec:
        pass


@pytest.mark.slow
@pytest.mark.parametrize(
    'rows, columns',
    (
            (1024 * 1024 // 16, 16,),
            (1024 * 1024 // 8, 16,),
            (1024 * 1024 // 4, 16,),
            (1024 * 1024 // 2, 16,),
            (1024 * 1024, 16,),
            (1024 * 1024 * 2, 16,),
            (1024 * 1024 * 4, 16,),
            (1024 * 1024 * 8, 16,),
            (1024 * 1024 * 16, 16,),
            (1024 * 1024 * 32, 16,),

            # (1024 * 128, 1024,),

            # (1024 * 2, 65536,),
    )
)
def test_sh_mem_create_copy(benchmark, rows, columns):
    array = np.random.random((rows, columns))
    benchmark(_create_shared_memory_array_spec_close_unlink, array, True)


@pytest.mark.slow
@pytest.mark.parametrize(
    'rows, columns',
    (
            (1024 * 1024 // 16, 16,),
            (1024 * 1024 // 8, 16,),
            (1024 * 1024 // 4, 16,),
            (1024 * 1024 // 2, 16,),
            (1024 * 1024, 16,),
            (1024 * 1024 * 2, 16,),
            (1024 * 1024 * 4, 16,),
            (1024 * 1024 * 8, 16,),
            (1024 * 1024 * 16, 16,),
            (1024 * 1024 * 32, 16,),

            # (1024 * 128, 1024,),

            # (1024 * 2, 65536,),
    )
)
def test_sh_mem_create_no_copy(benchmark, rows, columns):
    array = np.random.random((rows, columns))
    benchmark(_create_shared_memory_array_spec_close_unlink, array, False)


@pytest.mark.slow
@pytest.mark.parametrize(
    'rows, columns',
    (
            (1024 * 1024 // 16, 16,),
            (1024 * 1024 // 8, 16,),
            (1024 * 1024 // 4, 16,),
            (1024 * 1024 // 2, 16,),
            (1024 * 1024, 16,),
            (1024 * 1024 * 2, 16,),
            (1024 * 1024 * 4, 16,),
            (1024 * 1024 * 8, 16,),
            (1024 * 1024 * 16, 16,),
            (1024 * 1024 * 32, 16,),

            # (1024 * 128, 1024,),

            # (1024 * 2, 65536,),
    )
)
def test_copy_shared_memory_into_new_numpy_array(benchmark, rows, columns):
    array = np.random.random((rows, columns))
    with roll_med_sh_mem.create_shared_memory_array_spec_close_unlink(
            array, copy_array=True, log_progress=False,
    ) as read_array_spec:
        benchmark(roll_med_sh_mem.copy_shared_memory_into_new_numpy_array, read_array_spec)


def _test_rolling_median_float(read_array: np.array, processes: int) -> np.array:
    window_length = 21
    write_array = roll_med_sh_mem.compute_rolling_median_2d_mp(
        read_array, window_length=window_length, num_processes=processes,
    )
    return write_array


@pytest.mark.skip(reason="Very long running tests.")
@pytest.mark.parametrize(
    'processes',
    (1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_8388608_16(benchmark, processes):
    rows = 1024 * 1024 * 8
    columns = 16
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes)


@pytest.mark.skip(reason="Very long running tests.")
@pytest.mark.parametrize(
    'processes',
    (1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_131072_1024(benchmark, processes):
    rows = 1024 * 128
    columns = 1024
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes)


@pytest.mark.skip(reason="Very long running tests.")
@pytest.mark.parametrize(
    'processes',
    (1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_2048_65536(benchmark, processes):
    rows = 1024 * 2
    columns = 65536
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes)
