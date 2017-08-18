"""Some specific PyObject* tests"""
import sys

import pytest

import orderedstructs

from .SkipList_common import TotalOrdered, OrderedLt, Person

def test_ordered_person():
    sl = orderedstructs.SkipList(object)
    sl.insert(Person('Peter', 'Pan'))
    sl.insert(Person('Alan', 'Pan'))
    assert sl.size() == 2
    assert str(sl.at(0)) == 'Pan, Alan' 
    assert str(sl.at(1)) == 'Pan, Peter' 

#---- id() tests -------
@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_insert_remove_id_is_same(cls):
    """Check that an insert followed by a remove refers to the same Python object."""
    sl = orderedstructs.SkipList(object)
    obj_insert = cls(0)
    sl.insert(obj_insert)
    obj_remove = sl.remove(obj_insert)
    assert id(obj_remove) == id(obj_insert)

@pytest.mark.parametrize('cls', [TotalOrdered, ])#OrderedLt])
def test_ordered_insert_remove_diff_id_is_same(cls):
    """Check that an insert followed by a remove of an equivalent object
    returns the originally inserted Python object."""
    sl = orderedstructs.SkipList(object)
    obj_insert = cls(0)
    sl.insert(obj_insert)
    # Remove a different one should return the one inserted
    obj_to_remove = cls(0)
    assert id(obj_to_remove) != id(obj_insert)
    assert obj_to_remove == obj_insert
    obj_remove = sl.remove(obj_to_remove)
    assert id(obj_remove) == id(obj_insert)
#---- END: id() tests -------

#---- Refcount tests -------
@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_refcount_insert(cls):
    """Reference count is incremented on insert."""
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_refcount_remove(cls):
    """Reference count is maintained on insert+remove."""
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    sl.remove(obj)
    assert sys.getrefcount(obj) == rc

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_refcount_after_del(cls):
    """Reference count is maintained after insert and then dealloc."""
    obj = cls(0)
    rc = sys.getrefcount(obj)
    sl = orderedstructs.SkipList(object)
    sl.insert(obj)
    del sl
    assert sys.getrefcount(obj) == rc

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_refcount_has(cls):
    """Reference count is not incremented on at()."""
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1
    assert sl.has(obj)
    assert sys.getrefcount(obj) == rc + 1

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_refcount_at(cls):
    """Reference count is incremented when at() is assigned."""
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    rc = sys.getrefcount(obj)
    sl.insert(obj)
    assert sys.getrefcount(obj) == rc + 1
    obj_at = sl.at(0)
    assert id(obj_at) == id(obj)
    assert sys.getrefcount(obj) == rc + 2

#---- END: Refcount tests -------

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_insert(cls):
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('cls', [TotalOrdered, ])#OrderedLt])
def test_ordered_insert_remove(cls):
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(i)
        removed_obj = sl.remove(obj)
        assert removed_obj == obj
        
@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_size(cls):
    sl = orderedstructs.SkipList(object)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_has(cls):
    """Insert an object and check the object is there."""
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    sl.insert(obj)
    assert sl.has(obj)

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_has_same_value_diff_id(cls):
    """Insert an object and check that an equivalent object is there."""
    sl = orderedstructs.SkipList(object)
    obj_a = cls(0)
    sl.insert(obj_a)
    assert sl.has(obj_a)
    obj_b = cls(0)
    assert id(obj_a) != id(obj_b)
    assert sl.has(obj_b)

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_at(cls):
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    sl.insert(obj)
    obj_at = sl.at(0)
    assert id(obj_at) == id(obj)

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_index(cls):
    sl = orderedstructs.SkipList(object)
    obj = cls(0)
    sl.insert(obj)
    idx = sl.index(obj)
    assert idx == 0
    obj_other = cls(0)
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
@pytest.mark.skipif(sys.version_info.major==2, reason='Requires Python 3')
@pytest.mark.parametrize('obj0, obj1, method', [
                                 (TotalOrdered(0), 0.0, 'insert'),
                                 (0.0, TotalOrdered(0), 'insert'),
                                 (OrderedLt(0), 0.0, 'insert'),
                                 (0.0, OrderedLt(0), 'insert'),
                                 (0, (1+2j), 'insert'),
                                 ((1+2j), 0, 'insert'),
                                 (0.0, (1+2j), 'insert'),
                                 ((1+2j), 0.0, 'insert'),
                                 (TotalOrdered(0), 0.0, 'index'),
                                 (0.0, TotalOrdered(0), 'index'),
                                 (OrderedLt(0), 0.0, 'index'),
                                 (0.0, OrderedLt(0), 'index'),
                                 (0, (1+2j), 'index'),
                                 ((1+2j), 0, 'index'),
                                 (0.0, (1+2j), 'index'),
                                 ((1+2j), 0.0, 'index'),
                                 (TotalOrdered(0), 0.0, 'remove'),
                                 (0.0, TotalOrdered(0), 'remove'),
                                 (OrderedLt(0), 0.0, 'remove'),
                                 (0.0, OrderedLt(0), 'remove'),
                                 (0, (1+2j), 'remove'),
                                 ((1+2j), 0, 'remove'),
                                 (0.0, (1+2j), 'remove'),
                                 ((1+2j), 0.0, 'remove'),
                                 (TotalOrdered(0), 0.0, 'has'),
                                 (0.0, TotalOrdered(0), 'has'),
                                 (OrderedLt(0), 0.0, 'has'),
                                 (0.0, OrderedLt(0), 'has'),
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


#==== Bespoke comparison function fails ====

#---- Bespoke non-object and comparison function fails ----

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_given_cmp_function_int_fails(cls):
    """Test of passing in a non-callable.
    This can be detected at instantiation time."""
    if sys.version_info.major == 2:
        int_type = long
    elif sys.version_info.major == 3:
        int_type = int
    else:
        assert 0, 'Unsupported Python version.'
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList(int_type, lambda x, y: x < y)
    assert err.value.args[0] == \
        'Can not specify comparison function with type "long".'

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_given_cmp_function_float_fails(cls):
    """Test of passing in a non-callable.
    This can be detected at instantiation time."""
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList(float, lambda x, y: x < y)
    assert err.value.args[0] == \
        'Can not specify comparison function with type "float".'

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_given_cmp_function_bytes_fails(cls):
    """Test of passing in a non-callable.
    This can be detected at instantiation time."""
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList(bytes, lambda x, y: x < y)
    assert err.value.args[0] == \
        'Can not specify comparison function with type "bytes".'

#---- END: Bespoke non-object and comparison function fails ----

#---- Bespoke comparison function wrong signature ----
@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_cmp_not_a_function(cls):
    """Test of passing in a non-callable.
    This can be detected at instantiation time."""
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList(object, 14)
    assert err.value.args[0] == \
        'Argument "cmp_func" to __init__ must be a callable object not an "int" object.'

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_cmp_function_lambda_too_few_arguments(cls):
    """Test of passing in a lambda with too few arguments.
    This can be not be detected at instantiation time,
    only when a comparison method is called."""
    sl = orderedstructs.SkipList(object, lambda x: x < 0)
    sl.insert(4.0)
    with pytest.raises(TypeError) as err:
        sl.insert(8.0)
    if sys.version_info.major == 2:
        exp = '<lambda>() takes exactly 1 argument (2 given)'
    elif sys.version_info.major == 3:
        exp = '<lambda>() takes 1 positional argument but 2 were given'
    else:
        assert 0, 'Unsupported Python version.'
    assert err.value.args[0] == exp

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_cmp_function_lambda_too_many_arguments(cls):
    """Test of passing in a lambda with too many arguments.
    This can be not be detected at instantiation time,
    only when a comparison method is called."""
    sl = orderedstructs.SkipList(object, lambda x, y, z: x + y + z < 0)
    sl.insert(4.0)
    with pytest.raises(TypeError) as err:
        sl.insert(8.0)
    if sys.version_info.major == 2:
        exp = '<lambda>() takes exactly 3 arguments (2 given)'
    elif sys.version_info.major == 3:
        exp = "<lambda>() missing 1 required positional argument: 'z'"
    else:
        assert 0, 'Unsupported Python version.'
    assert err.value.args[0] == exp

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_cmp_function_local_too_few_arguments(cls):
    """Test of passing in a callable with too few arguments.
    This can be not be detected at instantiation time,
    only when a comparison method is called."""
    def cmp(x):
        return x < 0
    sl = orderedstructs.SkipList(object, cmp)
    sl.insert(4.0)
    with pytest.raises(TypeError) as err:
        sl.insert(8.0)
    if sys.version_info.major == 2:
        exp = 'cmp() takes exactly 1 argument (2 given)'
    elif sys.version_info.major == 3:
        exp = 'cmp() takes 1 positional argument but 2 were given'
    else:
        assert 0, 'Unsupported Python version.'
    assert err.value.args[0] == exp

@pytest.mark.parametrize('cls', [TotalOrdered, OrderedLt])
def test_ordered_cmp_function_local_too_many_arguments(cls):
    """Test of passing in a callable with too many arguments.
    This can be not be detected at instantiation time,
    only when a comparison method is called."""
    def cmp(x, y, z):
        return x + y + z < 0
    sl = orderedstructs.SkipList(object, cmp)
    sl.insert(4.0)
    with pytest.raises(TypeError) as err:
        sl.insert(8.0)
    if sys.version_info.major == 2:
        exp = "cmp() takes exactly 3 arguments (2 given)"
    elif sys.version_info.major == 3:
        exp = "cmp() missing 1 required positional argument: 'z'"
    else:
        assert 0, 'Unsupported Python version.'
    assert err.value.args[0] == exp

#---- END: Bespoke comparison function wrong signature ----

#==== END: Bespoke comparison function fails ====

@pytest.mark.parametrize('cls', [TotalOrdered,])
def test_ordered_insert_remove_cmp_reversed(cls):
    sl = orderedstructs.SkipList(object, lambda x, y: y < x)
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(i)
        sl.insert(obj)
        assert sl.lacks_integrity() == 0
    for i in range(8):
        assert sl.lacks_integrity() == 0
        obj = cls(7 - i)
        removed_obj = sl.remove(obj)
        assert removed_obj == obj
        

