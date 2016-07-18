'''
Created on 15 Jul 2016

@author: paulross
'''

HAS_HYPOTHESIS = False
try:
    import hypothesis
    import hypothesis.strategies as hst
    HAS_HYPOTHESIS = True
except ImportError:
    pass

import pytest

import cSkipList

@pytest.mark.skipif(not HAS_HYPOTHESIS, reason='Requires hypothesis')
@hypothesis.given(hst.lists(hst.integers(min_value=cSkipList.min_long(),
                                         max_value=cSkipList.max_long())))
def test_hypothesis_integers(lst):
    sl = cSkipList.PySkipList(int)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@pytest.mark.skipif(not HAS_HYPOTHESIS, reason='Requires hypothesis')
@hypothesis.given(hst.lists(hst.floats(allow_nan=False)))
def test_hypothesis_floats_no_nan(lst):
    sl = cSkipList.PySkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
    
@pytest.mark.skipif(not HAS_HYPOTHESIS, reason='Requires hypothesis')
@hypothesis.given(hst.lists(hst.floats(allow_nan=False, allow_infinity=True)))
def test_hypothesis_floats_no_nan_with_infinity(lst):
    sl = cSkipList.PySkipList(float)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)

@pytest.mark.skipif(not HAS_HYPOTHESIS, reason='Requires hypothesis')
@hypothesis.given(hst.lists(hst.binary()))
def test_hypothesis_bytes(lst):
    sl = cSkipList.PySkipList(bytes)
    for v in lst:
        sl.insert(v)
    result = [sl.at(i) for i in range(sl.size())]
    assert result == sorted(lst)
