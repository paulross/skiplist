'''
Created on 15 Jul 2016

@author: paulross
'''

import hypothesis
import hypothesis.strategies as hst
# import pytest

import cSkipList

BYTES_LONG_LONG = 8

# def min_value():
#     return -(1 << ((8 * BYTES_LONG_LONG) - 1))
# 
# def max_value():
#     return (1 << ((8 * BYTES_LONG_LONG) - 1)) - 1

@hypothesis.given(hst.lists(hst.integers(min_value=cSkipList.min_long(),
                                         max_value=cSkipList.max_long())))
def test_hypothesis_integers(lst):
    sl = cSkipList.PySkipList(int)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False)))
def test_hypothesis_floats_no_nan(lst):
    sl = cSkipList.PySkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_floats_no_nan_with_infinity(lst):
    sl = cSkipList.PySkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)

@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_bytes(lst):
    sl = cSkipList.PySkipList(bytes)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
