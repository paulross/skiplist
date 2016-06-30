import sys
import pytest
import copy

import numpy as np
import ahl.timeseries as ats

from ahl.numpy.fast import rollingmedian

SUPPORTED_FAST_DTYPES = [
    np.dtype(np.longdouble),
    np.dtype(np.double),
    np.dtype(np.float32),
]

def _test_rollingmedian_nonerror(data, kwargs, expected):
    res1 = rollingmedian(data, **kwargs)
    assert ats.allclose(res1, expected)
    kwargs_order = ["n", "axis", "dtype", "out", "skip_nans", "minobs"]
    defaults = [Exception, 0, None, None, True, None]
    args = []
    for k, d in zip(kwargs_order, defaults):
        if kwargs:
            arg = kwargs.pop(k, d)
            if arg == Exception:
                raise ValueError('Must pass in a window size "n" for the test')
            args.append(arg)
    res2 = rollingmedian(data, *args)
    assert ats.allclose(res2, expected)
    res3 = rollingmedian(data * -1, *args)
    assert ats.allclose(res3, expected * -1)


@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_len_zero_ok(dtype):
    data = np.array([], dtype=dtype)
    res = rollingmedian(data, n=-10)
    assert ats.allclose(res, np.array([], dtype=dtype))

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_window_size_zero_errors(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    with pytest.raises(ValueError):
        rollingmedian(data, n=0)
    with pytest.raises(ValueError):
        rollingmedian(data, 0)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_no_window_size_errors(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    with pytest.raises(TypeError):
        rollingmedian(data)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_minobs_zero_errors(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    with pytest.raises(ValueError):
        _test_rollingmedian_nonerror(data, dict(n=2, minobs=0), None)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_minobs_greater_than_window_size_errors(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    with pytest.raises(ValueError):
        _test_rollingmedian_nonerror(data, dict(n=2, minobs=3), None)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_minobs_negative_errors(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    with pytest.raises(ValueError):
        _test_rollingmedian_nonerror(data, dict(n=-2, minobs=-2), None)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_minobs_unspecified_defaults_to_abs_window_size(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    expected = np.array([np.nan, np.nan, 2., 3.], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3), expected)
    expected_2 = np.array([2., 3., np.nan, np.nan], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=3), expected_2)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_window_size_negative(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    expected = np.array([np.nan, 1.5, 2.5, 3.5], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-2), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_window_size_positive(dtype):
    data = np.array([1., 2., 3., 4.], dtype=dtype)
    expected = np.array([1.5, 2.5, 3.5, np.nan], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=2), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_skipnans_true_minobs_unspecified(dtype):
    data = np.array([1., np.nan, np.nan, 2., 3., np.nan, 4.], dtype=dtype)
    expected = np.array([np.nan, np.nan, np.nan, 1.5, 2.5, np.nan, 3.5], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-2, skip_nans=True), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_skipnans_false_minobs_unspecified(dtype):
    data = np.array([1., np.nan, np.nan, 2., 3., np.nan, 4.], dtype=dtype)
    expected = np.array([np.nan, np.nan, np.nan, np.nan, 2.5, np.nan, np.nan], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-2, skip_nans=False), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_skipnans_true_with_minobs(dtype):
    data = np.array([1., np.nan, np.nan, 2., 3., np.nan, 4.], dtype=dtype)
    expected = np.array([1., np.nan, np.nan, 1.5, 2.0, np.nan, 3.0], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3, skip_nans=True, minobs=1), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_skipnans_false_with_minobs(dtype):
    data = np.array([0., 1.0, np.nan, 2., 3., np.nan, 4., np.nan, 5.], dtype=dtype)
    expected = np.array([np.nan, 0.5, 0.5, 1.5, 2.5, 2.5, 3.5, np.nan, 4.5], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3, skip_nans=False, minobs=2), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_1d_window_size_overflow_int_errors(dtype):
    data = np.array([0., 1.0, np.nan, 2., 3., np.nan, 4., np.nan, 5.], dtype=dtype)
    with pytest.raises(OverflowError):
        rollingmedian(data, n=sys.maxint + 1)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_2d_axis_not_specified_defaults_to_zero(dtype):
    data = np.array([[0., 1., 2., 3., 4.], [1., 2., 3., 4., 5], [2., 3., 4., 5., 6.],
                     [3., 4., 5., 6., 7.]], dtype=dtype)
    expected = np.array([[0., 1., 2., 3., 4.], [0.5, 1.5, 2.5, 3.5, 4.5], [1., 2., 3., 4., 5],
                         [2., 3., 4., 5., 6.]], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3, minobs=1), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_2d_axis_0(dtype):
    data = np.array([[0., 1., 2., 3., 4.], [1., 2., 3., 4., 5], [2., 3., 4., 5., 6.],
                     [3., 4., 5., 6., 7.]], dtype=dtype)
    expected = np.array([[0., 1., 2., 3., 4.], [0.5, 1.5, 2.5, 3.5, 4.5], [1., 2., 3., 4., 5],
                         [2., 3., 4., 5., 6.]], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3, axis=0, minobs=1), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_2d_axis_1(dtype):
    data = np.array([[0., 1., 2., 3., 4.], [1., 2., 3., 4., 5], [2., 3., 4., 5., 6.],
                     [3., 4., 5., 6., 7.]], dtype=dtype)
    expected = np.array([[0., 0.5, 1., 2., 3.], [1., 1.5, 2., 3., 4.], [2., 2.5, 3., 4., 5.],
                          [3., 3.5, 4., 5., 6.]], dtype=dtype)
    _test_rollingmedian_nonerror(data, dict(n=-3, axis=1, minobs=1), expected)

@pytest.mark.parametrize('dtype', SUPPORTED_FAST_DTYPES)
def test_rollingmedian_doesnt_alter_inputs(dtype):
    data = np.array([[0., 1., 2., 3., 4.], [1., 2., np.nan, 4., 5], [2., 3., 4., 5., 6.],
                      [3., 4., 5., 6., 7.], [np.nan] * 5], dtype=dtype)
    expected_original_data = copy.deepcopy(data)
    rollingmedian(data, n=-3, axis=1, minobs=1)
    assert ats.allclose(data, expected_original_data)

def test_rollingmedian_arguments_array_fails():
    """Tests poor first argument."""
    with pytest.raises(TypeError):
        res = rollingmedian(n=-10)
    with pytest.raises(TypeError):
        res = rollingmedian(1, n=-10)
    with pytest.raises(ValueError):
        res = rollingmedian(np.ndarray([]), n=-10)

def test_rollingmedian_arguments_window_size_fails():
    """Tests poor second argument."""
    with pytest.raises(TypeError):
        res = rollingmedian(np.ndarray((4, 3)))
    with pytest.raises(TypeError):
        res = rollingmedian(np.ndarray((4, 3)), n='1')
    with pytest.raises(ValueError):
        res = rollingmedian(np.ndarray((4, 3)), n=0)

def test_rollingmedian_arguments_axis_fails():
    with pytest.raises(TypeError):
        res = rollingmedian(np.ndarray((4, 3)), 2, '0')
    with pytest.raises(TypeError):
        res = rollingmedian(np.ndarray((4, 3)), 2, axis='0')
    with pytest.raises(TypeError):
        res = rollingmedian(np.ndarray((4, 3)), n=-2, axis='0')
    with pytest.raises(ValueError) as err:
        res = rollingmedian(np.ndarray((4, 3)), n=-2, axis=2)
    assert err.value.message == 'fast_rollingmedian() axis(=2) out of bounds for rank=2'
    with pytest.raises(ValueError) as err:
        res = rollingmedian(np.ndarray((4, 3)), n=-2, axis=-3)
    assert err.value.message == 'fast_rollingmedian() axis(=-3) out of bounds for rank=2'

def test_rollingmedian_arguments_dtype_fails():
    with pytest.raises(NotImplementedError) as err:
        res = rollingmedian(np.ndarray((4, 3)), 2, dtype=np.float)
    assert err.value.message == 'dtype must be absent or None (dtype change not supported).'

def test_rollingmedian_arguments_out_fails():
    with pytest.raises(NotImplementedError) as err:
        res = rollingmedian(np.ndarray((4, 3)), 2, out=np.ndarray((4, 3)))
    assert err.value.message == 'Writing to output array is not supported.'

def test_rollingmedian_arguments_minobs_fails():
    with pytest.raises(TypeError) as err:
        res = rollingmedian(np.ndarray((4, 3)), 2, minobs='1')
    assert err.value.message == 'fast_rollingmedian() minobs, if present, must be None or an integer, not a "str" type.'
    with pytest.raises(ValueError) as err:
        res = rollingmedian(np.ndarray((4, 3)), n=-2, minobs=0)
    assert err.value.message == 'Minobs of 0 must be > 0 and <= lookback of 2.'
    with pytest.raises(ValueError) as err:
        res = rollingmedian(np.ndarray((4, 3)), n=-2, minobs=-1)
    assert err.value.message == 'Minobs of -1 must be > 0 and <= lookback of 2.'
    with pytest.raises(ValueError) as err:
        res = rollingmedian(np.ndarray((4, 3)), n=-2, minobs=3)
    assert err.value.message == 'Minobs of 3 must be > 0 and <= lookback of 2.'


