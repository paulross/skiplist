"""
iar == 'insert_at_remove'

Bench mark tests for Python objects. Typical usage:
$ caffeinate pytest --benchmark-sort=fullname tests/benchmarks/test_benchmark_SkipList_objects.py --runslow --benchmark-autosave --benchmark-histogram
"""
import functools
import typing

import pytest

import orderedstructs

# Make formatted strings so that the test name sorts nicely.
SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS = tuple(f'{2**i:8d}' for i in range(5, 20, 2))


SKIP_TOTAL_ORDERING = True


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


def _setup_skiplist_with_class(cls: typing.Type, n: int) -> orderedstructs.SkipList:
    """Returns a skiplist with a particular class of length n."""
    sl = orderedstructs.SkipList(object)
    for v in range(n):
        sl.insert(cls(v))
    return sl


def _test_simple_ordered_iar(skip_list: orderedstructs.SkipList,
                                          value: SimpleOrdered, index: int) -> SimpleOrdered:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_iar_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_iar, skip_list, SimpleOrdered(0), 0)
    assert result == SimpleOrdered(0)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_iar_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_iar, skip_list, SimpleOrdered(int_len / 2), int_len // 2)
    assert result == SimpleOrdered(int_len / 2)


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_simple_iar_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(SimpleOrdered, int_len)
    result = benchmark(_test_simple_ordered_iar, skip_list, SimpleOrdered(int_len - 1), int_len - 1)
    assert result == SimpleOrdered(int_len - 1)


def _test_total_ordered_iar(skip_list: orderedstructs.SkipList,
                                         value: TotalOrdered, index: int) -> TotalOrdered:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.skipif(SKIP_TOTAL_ORDERING, reason='This does not seem to add much.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_iar_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_iar, skip_list, TotalOrdered(0), 0)
    assert result == TotalOrdered(0)


@pytest.mark.skipif(SKIP_TOTAL_ORDERING, reason='This does not seem to add much.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_iar_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_iar, skip_list, TotalOrdered(int_len / 2), int_len // 2)
    assert result == TotalOrdered(int_len / 2)


@pytest.mark.skipif(SKIP_TOTAL_ORDERING, reason='This does not seem to add much.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FOR_PYTHON_SKIPLISTS)
def test_object_total_iar_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist_with_class(TotalOrdered, int_len)
    result = benchmark(_test_total_ordered_iar, skip_list, TotalOrdered(int_len - 1), int_len - 1)
    assert result == TotalOrdered(int_len - 1)
