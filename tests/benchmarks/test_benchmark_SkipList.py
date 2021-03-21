"""
Typical usage:
$ caffeinate pytest --benchmark-sort=fullname tests/benchmarks/ --runslow --benchmark-autosave
"""
import functools
import sys
import typing

import pytest

import orderedstructs

# Python 3
int_type = int
# Python 2
if sys.version_info[0] == 2:
    int_type = long
    # int_type_str = 'long'


class SimpleOrdered(object):
    """Simple Python class."""
    def __init__(self, value):
        self._value = value

    def __eq__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value == other._value

    def __lt__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value < other._value


@functools.total_ordering
class TotalOrdered(object):
    """Python class with total_ordering."""
    def __init__(self, value):
        self._value = value

    def __eq__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value == other._value

    def __lt__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value < other._value


def _setup_skiplist(typ: typing.Type, n: int) -> orderedstructs.SkipList:
    """Returns a skiplist of a particular type of length n."""
    sl = orderedstructs.SkipList(typ)
    for v in range(n):
        sl.insert(typ(v))
    return sl


def _setup_skiplist_int_as_object(n: int) -> orderedstructs.SkipList:
    """Returns a skiplist of a particular type of length n."""
    sl = orderedstructs.SkipList(object)
    for v in range(n):
        sl.insert(v)
    return sl


def _setup_skiplist_with_class(cls: typing.Type, n: int) -> orderedstructs.SkipList:
    """Returns a skiplist with a particular class of length n."""
    sl = orderedstructs.SkipList(object)
    for v in range(n):
        sl.insert(cls(v))
    return sl


# Make formatted strings so that the test name sorts nicely.
SKIPLIST_LENGTHS_FLOATS = tuple(f'{2 ** i:8d}' for i in range(1, 24, 2))
SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS = tuple(f'{2**i:8d}' for i in range(1, 18, 2))


def _test_float_has(skip_list: orderedstructs.SkipList, value: float) -> bool:
    return skip_list.has(value)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_has_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_has, skip_list, 0.0)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_has_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_has, skip_list, int_len / 2.0)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_has_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_has, skip_list, float(int_len - 1))
    assert result
    assert skip_list.size() == int_len


def _test_float_at(skip_list: orderedstructs.SkipList, index: int) -> float:
    return skip_list.at(index)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_at_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_at, skip_list, 0)
    assert result == 0.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_at_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_at, skip_list, int_len // 2)
    assert result == int_len / 2.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_at_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_at, skip_list, int_len - 1)
    assert result == int_len - 1.0
    assert skip_list.size() == int_len


def _test_float_insert_remove(skip_list: orderedstructs.SkipList, value: float) -> None:
    skip_list.insert(value)
    skip_list.remove(value)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, 0.0)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, int_len / 2.0)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, float(int_len))
    assert result is None
    assert skip_list.size() == int_len


def _test_float_insert_at_remove(skip_list: orderedstructs.SkipList, value: float, index: int) -> float:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_at_remove_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_at_remove, skip_list, 0.0, 0)
    assert result == 0.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_at_remove_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_at_remove, skip_list, int_len / 2.0, int_len // 2)
    assert result == int_len / 2.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_at_remove_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_at_remove, skip_list, float(int_len), int_len)
    assert result == int_len
    assert skip_list.size() == int_len



def _test_simple_ordered_insert_at_remove(skip_list: orderedstructs.SkipList, value: SimpleOrdered, index: int) -> SimpleOrdered:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_insert_at_remove_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_insert_at_remove, skip_list, SimpleOrdered(0), 0)
    assert result == SimpleOrdered(0)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_insert_at_remove_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_insert_at_remove, skip_list, SimpleOrdered(int_len / 2), int_len // 2)
    assert result == SimpleOrdered(int_len / 2)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_insert_at_remove_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_insert_at_remove, skip_list, SimpleOrdered(int_len - 1), int_len - 1)
    assert result == SimpleOrdered(int_len - 1)


def _test_total_ordered_insert_at_remove(skip_list: orderedstructs.SkipList, value: TotalOrdered, index: int) -> TotalOrdered:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_insert_at_remove_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_insert_at_remove, skip_list, TotalOrdered(0), 0)
    assert result == TotalOrdered(0)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_insert_at_remove_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_insert_at_remove, skip_list, TotalOrdered(int_len / 2), int_len // 2)
    assert result == TotalOrdered(int_len / 2)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_insert_at_remove_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_insert_at_remove, skip_list, TotalOrdered(int_len - 1), int_len - 1)
    assert result == TotalOrdered(int_len - 1)
