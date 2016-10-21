"""Some specific PyObject* tests"""
import functools
import sys

import pytest

import orderedstructs

@functools.total_ordering
class Ordered(object):
    """A class that has Python total ordering on a Python object."""
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

#---- id() tests -------
def test_ordered_insert_remove_id_is_same():
    """Check that an insert followed by a remove refers to the same Python object."""
    sl = orderedstructs.SkipList(object)
    obj_insert = Ordered(0)
    sl.insert(obj_insert)
    obj_remove = sl.remove(obj_insert)
    assert id(obj_remove) == id(obj_insert)

def test_ordered_insert_remove_diff_id_is_same():
    """Check that an insert followed by a remove of an equivalent object
    returns the originally inserted Python object."""
    sl = orderedstructs.SkipList(object)
    obj_insert = Ordered(0)
    sl.insert(obj_insert)
    # Remove a different one should return the one inserted
    obj_to_remove = Ordered(0)
    assert id(obj_to_remove) != id(obj_insert)
    assert obj_to_remove == obj_insert
    obj_remove = sl.remove(obj_to_remove)
    assert id(obj_remove) == id(obj_insert)
#---- END: id() tests -------

#---- Refcount tests -------
def test_ordered_refcount_insert():
    """Reference count is incremented on insert."""
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1

def test_ordered_refcount_remove():
    """Reference count is maintained on insert+remove."""
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    sl.remove(obj)
    assert sys.getrefcount(obj) == rc

def test_ordered_refcount_after_del():
    """Reference count is maintained after insert and then dealloc."""
    obj = Ordered(0)
    rc = sys.getrefcount(obj)
    sl = orderedstructs.SkipList(object)
    sl.insert(obj)
    del sl
    assert sys.getrefcount(obj) == rc

def test_ordered_refcount_has():
    """Reference count is not incremented on at()."""
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1
    assert sl.has(obj)
    assert sys.getrefcount(obj) == rc + 1

def test_ordered_refcount_at():
    """Reference count is incremented when at() is assigned."""
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1
    obj_at = sl.at(0)
    assert id(obj_at) == id(obj)
    assert sys.getrefcount(obj) == rc + 2

#---- END: Refcount tests -------

def test_ordered_insert():
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = Ordered(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0

def test_ordered_insert_remove():
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = Ordered(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = Ordered(i)
        removed_obj = sl.remove(obj)
        assert removed_obj == obj
        
def test_ordered_size():
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = Ordered(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1

def test_ordered_has():
    """Insert an object and check the object is there."""
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    sl.insert(obj)
    assert sl.has(obj)

def test_ordered_has_same_value_diff_id():
    """Insert an object and check that an equivalent object is there."""
    sl = orderedstructs.SkipList(object)
    obj_a = Ordered(0)
    sl.insert(obj_a)
    assert sl.has(obj_a)
    obj_b = Ordered(0)
    assert id(obj_a) != id(obj_b)
    assert sl.has(obj_b)

def test_ordered_at():
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    sl.insert(obj)
    obj_at = sl.at(0)
    assert id(obj_at) == id(obj)

def test_ordered_index():
    sl = orderedstructs.SkipList(object)
    obj = Ordered(0)
    sl.insert(obj)
    idx = sl.index(obj)
    assert idx == 0
    obj_other = Ordered(0)
    assert id(obj_other) != id(obj)
    idx = sl.index(obj_other)
    assert idx == 0



@pytest.mark.parametrize('obj0, obj1, method', [
                                 (0, 0.0, 'insert'),
                                 (0.0, 0, 'insert'),
                                 (0, 0.0, 'index'),
                                 (0.0, 0, 'index'),
                                 (0, 0.0, 'remove'),
                                 (0.0, 0, 'remove'),
                                 (0, 0.0, 'has'),
                                 (0.0, 0, 'has'),
                                 ])
def test_mixed_types_succeeds(obj0, obj1, method):
    sl = orderedstructs.SkipList(object)
    sl.insert(obj0)
    getattr(sl, method)(obj1)

# NOTE: Can not compare int/float and complex
@pytest.mark.parametrize('obj0, obj1, method', [
                                 (Ordered(0), 0.0, 'insert'),
                                 (0.0, Ordered(0), 'insert'),
                                 (0, (1+2j), 'insert'),
                                 ((1+2j), 0, 'insert'),
                                 (0.0, (1+2j), 'insert'),
                                 ((1+2j), 0.0, 'insert'),
                                 (Ordered(0), 0.0, 'index'),
                                 (0.0, Ordered(0), 'index'),
                                 (0, (1+2j), 'index'),
                                 ((1+2j), 0, 'index'),
                                 (0.0, (1+2j), 'index'),
                                 ((1+2j), 0.0, 'index'),
                                 (Ordered(0), 0.0, 'remove'),
                                 (0.0, Ordered(0), 'remove'),
                                 (0, (1+2j), 'remove'),
                                 ((1+2j), 0, 'remove'),
                                 (0.0, (1+2j), 'remove'),
                                 ((1+2j), 0.0, 'remove'),
                                 (Ordered(0), 0.0, 'has'),
                                 (0.0, Ordered(0), 'has'),
                                 (0, (0+0j), 'has'),
                                 ((0+0j), 0, 'has'),
                                 (0.0, (0+0j), 'has'),
                                 ((0+0j), 0.0, 'has'),
                                 ])
def test_fails_with_no_comparison(obj0, obj1, method):
    sl = orderedstructs.SkipList(object)
    sl.insert(obj0)
    with pytest.raises(TypeError):
        getattr(sl, method)(obj1)
