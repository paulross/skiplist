import itertools

import pytest

import orderedstructs
from . import seed_tree

from .compat_23 import int_type, math_nan, example_int_value, example_int_seq

@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_ctor(typ):
    assert orderedstructs.SortedMap(typ) is not None

def test_ctor_raises_no_type():
    with pytest.raises(TypeError) as err:
        orderedstructs.SortedMap()
    assert err.value.args[0] == "Required argument 'value_type' (pos 1) not found"

def test_ctor_raises_not_a_type():
    with pytest.raises(ValueError) as err:
        orderedstructs.SortedMap('')
    assert err.value.args[0] == \
        'Argument to __init__ must be type object not "str"'

@pytest.mark.parametrize('typ, msg', [
                                 (complex, 'complex'),
                                 (tuple, 'tuple'),
                                 (list, 'list'),
                                 (dict, 'dict'),
                                 ])
def test_ctor_raises_wrong_type(typ, msg):
    with pytest.raises(ValueError) as err:
        orderedstructs.SortedMap(typ)
    assert err.value.args[0] == \
        'Argument to __init__ must be long, float or bytes, not "%s"' % msg

@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_size_zero(typ):
    sm = orderedstructs.SortedMap(typ)
    assert sm.size() == 0
    
@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_max_size_greater_than_zero(typ):
    sm = orderedstructs.SortedMap(typ)
    max_size = sm.max_size()
    assert max_size.bit_length() > 32
    
# @pytest.mark.parametrize('typ,value',
#                          [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
# def test_single_insert(typ, value):
#     sm = orderedstructs.SortedMap(typ)
#     assert sm.insert(value) is None
#     assert sm.size() == 1
