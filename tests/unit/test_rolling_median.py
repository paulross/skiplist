
# import numpy as np
import math

import orderedstructs
import pytest
import typing


def lists_are_equal(a: typing.List[float], b: typing.List[float]) -> bool:
    """Return True if all members of the two lists are equal. NaNs are treated as equal."""
    if len(a) != len(b):
        return False
    for v_a, v_b in zip(a, b):
        if math.isnan(v_a) or math.isnan(v_b):
            # Both NaN or both  non-NaN
            if math.isnan(v_a) ^ math.isnan(v_b):
                return False
        else:
            if v_a != v_b:
                return False
    return True


@pytest.mark.parametrize(
    'a, b, expected',
    (
        (
            [1, 2, 3], [1, 2, 3], True,
        ),
        (
            [1, 2, 3], [1, 2, ], False,
        ),
        (
            [1, 2, 3], [1, 2, 4], False,
        ),
        (
            [math.nan, ], [math.nan, ], True,
        ),
    )
)
def test_lists_are_equal(a, b, expected):
    result = lists_are_equal(a, b)
    assert result == expected


def rolling_median_no_nan(vector: typing.List[float], window_length: int) -> typing.List[float]:
    """Computes a rolling median of a vector of floats and returns the results. NaNs will throw an exception."""
    skip_list = orderedstructs.SkipList(float)
    ret: typing.List[float] = []
    for i in range(len(vector)):
        value = vector[i]
        skip_list.insert(float(value))
        if i >= window_length:
            median = skip_list.at(window_length // 2)
            skip_list.remove(vector[i - window_length])
        else:
            median = math.nan
        ret.append(median)
    return ret


@pytest.mark.parametrize(
    'input_vector, window_length, expected',
    (
        (
            [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0],
            5,
            [math.nan, math.nan, math.nan, math.nan, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, ],
        ),
    )
)
def test_simple_array(input_vector, window_length, expected):
    result = rolling_median_no_nan(input_vector, window_length)
    assert lists_are_equal(result, expected)


@pytest.mark.parametrize(
    'input_vector, window_length, expected_error',
    (
        (
            [0.0, 1.0, math.nan, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0],
            5,
            'Can not insert a NaN with error "Can not work with something that does not compare equal to itself."',
        ),
    )
)
def test_simple_array_raises_nan(input_vector, window_length, expected_error):
    with pytest.raises(ValueError) as err:
        rolling_median_no_nan(input_vector, window_length)
    assert err.value.args[0] == expected_error


def rolling_median_with_nan(vector: typing.List[float], window_length: int) -> typing.List[float]:
    """Computes a rolling median of a vector of floats and returns the results. NaNs will be consumed."""
    skip_list = orderedstructs.SkipList(float)
    ret: typing.List[float] = []
    for i in range(len(vector)):
        value = vector[i]
        if math.isnan(value):
            median = math.nan
        else:
            skip_list.insert(float(value))
            if i >= window_length:
                median = skip_list.at(window_length // 2)
                remove_value = vector[i - window_length]
                if not math.isnan(remove_value):
                    skip_list.remove(remove_value)
            else:
                median = math.nan
        ret.append(median)
    return ret


@pytest.mark.parametrize(
    'input_vector, window_length, expected',
    (
        (
            [0.0, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            5,
            [math.nan, math.nan, math.nan, math.nan, math.nan, 3.0, 4.0, math.nan, 4.0, 5.0, ],
        ),
        (
            [0.0, math.nan, 2.0, math.nan, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            5,
            [math.nan, math.nan, math.nan, math.nan, math.nan, 4.0, 5.0, math.nan, 5.0, 5.0, ],
        ),
    )
)
def test_simple_array_works_with_nan(input_vector, window_length, expected):
    result = rolling_median_with_nan(input_vector, window_length)
    print(result)
    assert lists_are_equal(result, expected)


def forward_fill(vector: typing.List[float]) -> None:
    """Forward fills NaNs in-place."""
    previous_value = math.nan
    for i in range(len(vector)):
        value = vector[i]
        if math.isnan(value):
            vector[i] = previous_value
        if not math.isnan(value):
            previous_value = value


@pytest.mark.parametrize(
    'input_vector, expected',
    (
        (
            [0.0, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            [0.0, 0.0, 2.0, 3.0, 4.0, 5.0, 6.0, 6.0, 8.0, 9.0],
        ),
        (
            [math.nan, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            [math.nan, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, 6.0, 8.0, 9.0],
        ),
    )
)
def test_forward_fill(input_vector, expected):
    forward_fill(input_vector)
    print(input_vector)
    assert lists_are_equal(input_vector, expected)


def rolling_median_with_nan_forward_fill(vector: typing.List[float], window_length: int) -> typing.List[float]:
    """Computes a rolling median of a vector of floats and returns the results. NaNs will be forward filled."""
    forward_fill(vector)
    return rolling_median_no_nan(vector, window_length)


@pytest.mark.parametrize(
    'input_vector, window_length, expected',
    (
        (
            [0.0, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            5,
            [math.nan, math.nan, math.nan, math.nan, math.nan, 2.0, 3.0, 4.0, 5.0, 6.0],
        ),
        (
            [0.0, math.nan, 2.0, math.nan, 4.0, 5.0, 6.0, math.nan, 8.0, 9.0],
            5,
            [math.nan, math.nan, math.nan, math.nan, math.nan, 2.0, 2.0, 4.0, 5.0, 6.0, ],
        ),
    )
)
def test_simple_array_works_with_nan_ffill(input_vector, window_length, expected):
    result = rolling_median_with_nan_forward_fill(input_vector, window_length)
    print(result)
    assert lists_are_equal(result, expected)
