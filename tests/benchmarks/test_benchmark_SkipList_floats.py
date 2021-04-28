"""
iar == 'insert_at_remove'

Bench mark tests for Python floats. Typical usage:
$ caffeinate pytest --benchmark-sort=fullname tests/benchmarks/test_benchmark_SkipList_floats.py --runslow --benchmark-autosave --benchmark-histogram
"""
import typing

import pytest

import orderedstructs


# The iar (insert, at, remove) tests are more representative.
ONLY_BENCHMARK_INSERT_AT_REMOVE = True

def _setup_skiplist(typ: typing.Type, n: int) -> orderedstructs.SkipList:
    """Returns a skiplist of a particular type of length n."""
    sl = orderedstructs.SkipList(typ)
    for v in range(n):
        sl.insert(typ(v))
    return sl


# Make formatted strings so that the test name sorts nicely.
# Typ. ('     512', '    2048', '    8192', '   32768', '  131072', '  524288', ' 2097152', ' 8388608')
SKIPLIST_LENGTHS_FLOATS = tuple(f'{2 ** i:8d}' for i in range(9, 24, 2))


def _test_float_has(skip_list: orderedstructs.SkipList, value: float) -> bool:
    return skip_list.has(value)


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_has_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_has, skip_list, 0.0)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_has_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_has, skip_list, int_len / 2.0)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
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


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_at_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_at, skip_list, 0)
    assert result == 0.0
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_at_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_at, skip_list, int_len // 2)
    assert result == int_len / 2.0
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
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


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, 0.0)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, int_len / 2.0)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_insert_remove_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_insert_remove, skip_list, float(int_len))
    assert result is None
    assert skip_list.size() == int_len


def _test_float_iar(skip_list: orderedstructs.SkipList, value: float, index: int) -> float:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_iar_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_iar, skip_list, 0.0, 0)
    assert result == 0.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_iar_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_iar, skip_list, int_len / 2.0, int_len // 2)
    assert result == int_len / 2.0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_FLOATS)
def test_float_iar_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(float, int_len)
    result = benchmark(_test_float_iar, skip_list, float(int_len), int_len)
    assert result == int_len
    assert skip_list.size() == int_len
