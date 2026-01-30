"""
Bench mark tests for Python rolling median. Typical usage:
$ caffeinate pytest tests/benchmarks/test_benchmark_SkipList_rolling_median.py --runslow --benchmark-sort=fullname --benchmark-autosave --benchmark-histogram
"""
import random
import string
import typing

import numpy as np

import pytest

import orderedstructs

# Make formatted strings so that the test name sorts nicely.
# Typ. ('      1', ... '  524288',)
ROLLING_MEDIAN_FLOAT_WINDOW_WIDTHS = tuple(f'{2 ** i:8d}' for i in range(0, 20, 1))


def _test_rolling_median_float(vector: np.array, result: np.array, window_length: int):
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
@pytest.mark.parametrize('window_length', ROLLING_MEDIAN_FLOAT_WINDOW_WIDTHS)
def test_rolling_median_float_by_window_length(benchmark, window_length):
    length = 1000 ** 2
    vector = np.random.random(length)
    result = np.empty_like(vector)
    benchmark(_test_rolling_median_float, vector, result, int(window_length))


# Source - https://stackoverflow.com/a
# Posted by Ignacio Vazquez-Abrams, modified by community. See post 'Timeline' for change history
# Retrieved 2026-01-28, License - CC BY-SA 4.0
# Modified to use all printable characters.
def random_bytes(length: int) -> bytes:
    return ''.join(random.choices(string.printable, k=length)).encode('ascii')


# Make formatted strings so that the test name sorts nicely.
# Typ. ('      1', ... '  65536',)
# Align with perf_roll_med_vector_style_even_win_length_string()
ROLLING_MEDIAN_STR_WINDOW_WIDTHS = tuple(f'{2 ** i:8d}' for i in range(0, 17, 1))


def _test_rolling_median_bytes(vector: typing.List[str], window_length: int):
    skip_list = orderedstructs.SkipList(bytes)
    result = []
    for i in range(len(vector)):
        value = vector[i]
        skip_list.insert(value)
        if i >= window_length - 1:
            median = skip_list.at(window_length // 2)
            result.append(median)
            skip_list.remove(vector[i - window_length + 1])


@pytest.mark.slow
@pytest.mark.parametrize('window_length', ROLLING_MEDIAN_STR_WINDOW_WIDTHS)
def test_rolling_median_bytes_by_window_length(benchmark, window_length):
    # Align with perf_roll_med_vector_style_even_win_length_string()
    length = 100000
    vector = []
    for i in range(length):
        vector.append(random_bytes(1024))
    benchmark(_test_rolling_median_bytes, vector, int(window_length))
