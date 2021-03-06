import sys

import hypothesis
import hypothesis.strategies as hst
import pytest

import orderedstructs

from .compat_23 import int_type

@hypothesis.given(hst.lists(hst.integers(min_value=orderedstructs.min_long(),
                                         max_value=orderedstructs.max_long())))
def test_hypothesis_insert_integers(lst):
    sl = orderedstructs.SkipList(int_type)
    for v in lst:
        sl.insert(int_type(v))
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False)))
def test_hypothesis_insert_floats_no_nan(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_insert_floats_no_nan_with_infinity(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)

@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_insert_bytes(lst):
    sl = orderedstructs.SkipList(bytes)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)

@hypothesis.given(hst.lists(hst.integers(min_value=orderedstructs.min_long(),
                                         max_value=orderedstructs.max_long())))
def test_hypothesis_insert_remove_integers(lst):
    sl = orderedstructs.SkipList(int_type)
    for v in lst:
        sl.insert(int_type(v))
    for v in lst:
        sl.remove(int_type(v))
    assert sl.size() == 0
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False)))
def test_hypothesis_insert_remove_floats_no_nan(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    for v in lst:
        sl.remove(v)
    assert sl.size() == 0
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_insert_remove_floats_no_nan_with_infinity(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    for v in lst:
        sl.remove(v)
    assert sl.size() == 0

@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_insert_remove_bytes(lst):
    sl = orderedstructs.SkipList(bytes)
    for v in lst:
        sl.insert(v)
    for v in lst:
        sl.remove(v)
    assert sl.size() == 0

@hypothesis.given(hst.lists(hst.integers(min_value=orderedstructs.min_long(),
                                         max_value=orderedstructs.max_long())))
def test_hypothesis_index_integers(lst):
    sl = orderedstructs.SkipList(int_type)
    for v in lst:
        sl.insert(int_type(v))
    reference = sorted(lst)
    for v in lst:
        assert reference.index(v) == sl.index(int_type(v))
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_index_floats(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    reference = sorted(lst)
    for v in lst:
        assert reference.index(v) == sl.index(v)

@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_index_bytes(lst):
    sl = orderedstructs.SkipList(bytes)
    for v in lst:
        sl.insert(v)
    reference = sorted(lst)
    for v in lst:
        assert reference.index(v) == sl.index(v)

@hypothesis.given(hst.lists(hst.integers(min_value=orderedstructs.min_long(),
                                         max_value=orderedstructs.max_long())))
def test_hypothesis_index_matches_at_integers(lst):
    sl = orderedstructs.SkipList(int_type)
    for v in lst:
        sl.insert(int_type(v))
    for v in lst:
        assert sl.at(sl.index(int_type(v))) == v
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_index_matches_at_floats(lst):
    sl = orderedstructs.SkipList(float)
    for v in lst:
        sl.insert(v)
    for v in lst:
        assert sl.at(sl.index(v)) == v

@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_index_matches_at_bytes(lst):
    sl = orderedstructs.SkipList(bytes)
    for v in lst:
        sl.insert(v)
    for v in lst:
        assert sl.at(sl.index(v)) == v
