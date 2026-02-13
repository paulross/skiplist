"""
Benchmark tests for Python rolling median with shared memory. Typical usage:

pytest tests/benchmarks/test_benchmark_SkipList_rolling_median_sh_mem.py --runslow --runveryslow --benchmark-sort=name --benchmark-autosave --benchmark-histogram -v
"""
import numpy as np

import pytest

from tests.benchmarks import roll_med_sh_mem


def _test_rolling_median_float(read_array: np.array, processes: int) -> np.array:
    window_length = 21
    write_array = roll_med_sh_mem.compute_rolling_median_2d_mp(
        read_array, window_length=window_length, num_processes=processes,
    )
    return write_array


@pytest.mark.veryslow
@pytest.mark.parametrize(
    'processes',
    ( 1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_8388608_16(benchmark, processes):
    rows = 1024 * 1024 * 8
    columns = 16
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes, rounds=5)


@pytest.mark.veryslow
@pytest.mark.parametrize(
    'processes',
    ( 1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_131072_1024(benchmark, processes):
    rows = 1024 * 128
    columns = 1024
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes, rounds=5)


@pytest.mark.veryslow
@pytest.mark.parametrize(
    'processes',
    ( 1, 2, 4, 8, 16,)
    # ( 4,)
)
def test_rolling_median_sh_mem_2048_65536(benchmark, processes):
    rows = 1024 * 2
    columns = 65536
    # 134217728 values, 1073741824 bytes (1GB)
    assert rows * columns == 2048 * 65536
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes, rounds=5)
