"""
iar == insert_at_remove

Bench mark tests for Python ints. Typical usage:
$ caffeinate pytest --benchmark-sort=fullname tests/benchmarks/test_benchmark_SkipList_ints.py --runslow --benchmark-autosave --benchmark-histogram
"""
import sys
import typing

import pytest

import orderedstructs

ONLY_BENCHMARK_INSERT_AT_REMOVE = True

# Python 3
int_type = int
# Python 2
if sys.version_info[0] == 2:
    int_type = long
    # int_type_str = 'long'


def _setup_skiplist_int_as_object(n: int) -> orderedstructs.SkipList:
    """Returns a skiplist of a particular type of length n."""
    sl = orderedstructs.SkipList(object)
    for v in range(n):
        sl.insert(v)
    return sl


def _setup_skiplist(typ: typing.Type, n: int) -> orderedstructs.SkipList:
    """Returns a skiplist of a particular type of length n."""
    sl = orderedstructs.SkipList(typ)
    for v in range(n):
        sl.insert(typ(v))
    return sl


# Make formatted strings so that the test name sorts nicely.
# Typ. ('     512', '    2048', '    8192', '   32768', '  131072', '  524288', ' 2097152', ' 8388608')
SKIPLIST_LENGTHS_INTS = tuple(f'{2 ** i:8d}' for i in range(9, 24, 2))


def _test_int_has(skip_list: orderedstructs.SkipList, value: int) -> bool:
    return skip_list.has(value)


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_has_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_has, skip_list, 0)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_has_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_has, skip_list, int_len // 2)
    assert result
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_has_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_has, skip_list, int_len - 1)
    assert result
    assert skip_list.size() == int_len


def _test_int_at(skip_list: orderedstructs.SkipList, index: int) -> int:
    return skip_list.at(index)


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_at_begin(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_at, skip_list, 0)
    assert result == 0
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_at_mid(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_at, skip_list, int_len // 2)
    assert result == int_len // 2
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_at_end(benchmark, length):
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_at, skip_list, int_len - 1)
    assert result == int_len - 1
    assert skip_list.size() == int_len


def _test_int_insert_remove(skip_list: orderedstructs.SkipList, value: int) -> None:
    skip_list.insert(value)
    skip_list.remove(value)


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_insert_remove_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_insert_remove, skip_list, 0)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_insert_remove_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_insert_remove, skip_list, int_len // 2)
    assert result is None
    assert skip_list.size() == int_len


@pytest.mark.skipif(ONLY_BENCHMARK_INSERT_AT_REMOVE,
                    reason='The iar (insert, at, remove) tests are more representative.')
@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_insert_remove_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_insert_remove, skip_list, int_len)
    assert result is None
    assert skip_list.size() == int_len


def _test_int_iar(skip_list: orderedstructs.SkipList, value: int, index: int) -> int:
    """Insert/at/remove which simulates a rolling median."""
    skip_list.insert(value)
    result = skip_list.at(index)
    skip_list.remove(value)
    return result


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_iar_begin(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_iar, skip_list, 0, 0)
    assert result == 0
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_iar_mid(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_iar, skip_list, int_len // 2, int_len // 2)
    assert result == int_len // 2
    assert skip_list.size() == int_len


@pytest.mark.slow
@pytest.mark.parametrize('length', SKIPLIST_LENGTHS_INTS)
def test_int_iar_end(benchmark, length):
    """SkipList.at() of mid value."""
    int_len = int(length)
    skip_list = _setup_skiplist(int, int_len)
    result = benchmark(_test_int_iar, skip_list, int_len, int_len)
    assert result == int_len
    assert skip_list.size() == int_len
