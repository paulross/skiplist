"""
Bench mark tests for Python rolling median. Typical usage:
$ caffeinate pytest --benchmark-sort=fullname tests/benchmarks/test_benchmark_SkipList_floats.py --runslow --benchmark-autosave --benchmark-histogram
"""
import typing

import numpy as np

import pytest

import orderedstructs

# Make formatted strings so that the test name sorts nicely.
# Typ. ('      1', ... '  524288',)
ROLLING_MEDIAN_WINDOW_WIDTHS = tuple(f'{2 ** i:8d}' for i in range(0, 20, 1))


def _test_rolling_median(vector: np.array, result: np.array, window_length: int):
    skip_list = orderedstructs.SkipList(float)
    for i in range(len(vector)):
        value = vector[i]
        skip_list.insert(value)
        if i >= window_length - 1:
            median = skip_list.at(window_length // 2)
            skip_list.remove(vector[i - window_length + 1])
        else:
            median = np.nan
        result[i] = median


@pytest.mark.slow
@pytest.mark.parametrize('window_length', ROLLING_MEDIAN_WINDOW_WIDTHS)
def test_rolling_median_by_window_length(benchmark, window_length):
    length = 1000 ** 2
    vector = np.random.random(length)
    result = np.empty_like(vector)
    benchmark(_test_rolling_median, vector, result, int(window_length))
