"""
Bench mark tests for Python rolling median with shared memory. Typical usage:
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
    'rows, columns, processes',
    (
            # (1024, 16, 1,),
            (1024 * 1024 * 8, 16, 1,),
            (1024 * 1024 * 8, 16, 2,),
            (1024 * 1024 * 8, 16, 4,),
            (1024 * 1024 * 8, 16, 8,),
            (1024 * 1024 * 8, 16, 16,),
            (1024 * 128, 1024, 1,),
            (1024 * 128, 1024, 2,),
            (1024 * 128, 1024, 4,),
            (1024 * 128, 1024, 8,),
            (1024 * 128, 1024, 16,),
            (1024 * 2, 64 * 1024, 1,),
            (1024 * 2, 64 * 1024, 2,),
            (1024 * 2, 64 * 1024, 4,),
            (1024 * 2, 64 * 1024, 8,),
            (1024 * 2, 64 * 1024, 16,),
    ),
)
def test_rolling_median_sh_mem(benchmark, rows, columns, processes):
    assert rows * columns == 128 * 1024
    read_array = np.random.random((rows, columns))
    benchmark(_test_rolling_median_float, read_array, processes)
