import itertools

import pytest

import orderedstructs
from . import seed_tree

from .compat_23 import int_type, math_nan, example_int_value, example_int_seq

def test_module():
    assert hasattr(orderedstructs, '__version__')
    assert hasattr(orderedstructs, '__build_time__')
    assert hasattr(orderedstructs, '__build_type__')
    assert orderedstructs.__build_type__ in ('debug', 'release')
    assert hasattr(orderedstructs, '__build_target__')
    print('Module attributes and values:')
    attrs = ('__version__',) + tuple([a for a in dir(orderedstructs) if a.startswith('__build')])
    for attr_name in attrs:
        print('{:16}:'.format(attr_name), getattr(orderedstructs, attr_name))

@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_ctor(typ):
    assert orderedstructs.SkipList(typ) is not None

def test_ctor_raises_no_type():
    with pytest.raises(TypeError) as err:
        orderedstructs.SkipList()
    assert err.value.args[0] == "Required argument 'value_type' (pos 1) not found"

def test_ctor_raises_not_a_type():
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList('')
    assert err.value.args[0] == \
        'Argument to __init__ must be a type object not "str"'

@pytest.mark.parametrize('typ, msg', [
                                 (complex, 'complex'),
                                 (tuple, 'tuple'),
                                 (list, 'list'),
                                 (dict, 'dict'),
                                 ])
def test_ctor_raises_wrong_type(typ, msg):
    with pytest.raises(ValueError) as err:
        orderedstructs.SkipList(typ)
    assert err.value.args[0] == \
        'Argument to __init__ must be long, float, bytes or object, not "%s"' % msg

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_insert(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    assert sl.insert(value) is None
    assert sl.lacks_integrity() == 0
    
@pytest.mark.parametrize('typ,value',
                         [
                            (int_type, 8.0),
                            (int_type, '8'),
                            (float, 8),
                            (float, '8.0'),
                            (bytes, u'unicode'),
                        ])
def test_single_insert_raises(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    with pytest.raises(TypeError):
        sl.insert(value)
    assert sl.lacks_integrity() == 0

#------- Some specialised insert tests for particular types ----------
def test_single_insert_long_min_long():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    sl.insert(orderedstructs.min_long())
    assert sl.size() == 1
    assert sl.lacks_integrity() == 0

def test_single_insert_long_max_long():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    sl.insert(orderedstructs.max_long())
    assert sl.size() == 1
    assert sl.lacks_integrity() == 0

def test_single_insert_long_underflow_raises():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.insert(orderedstructs.min_long() - 1)
    assert sl.lacks_integrity() == 0

def test_single_insert_long_overflow_raises():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.insert(orderedstructs.max_long() + 1)
    assert sl.lacks_integrity() == 0

def test_single_remove_long_underflow_raises():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.remove(orderedstructs.min_long() - 1)
    assert sl.lacks_integrity() == 0

def test_single_remove_long_overflow_raises():
    sl = orderedstructs.SkipList(int_type)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.remove(orderedstructs.max_long() + 1)
    assert sl.lacks_integrity() == 0

def test_single_insert_float_NaN_raises():
    sl = orderedstructs.SkipList(float)
    assert sl.lacks_integrity() == 0
    with pytest.raises(ValueError) as err:
        sl.insert(math_nan)
    assert err.value.args[0] == \
        'Can not work with something that does not compare equal to itself.'
    assert sl.lacks_integrity() == 0

def test_has_float_NaN_raises():
    sl = orderedstructs.SkipList(float)
    assert sl.lacks_integrity() == 0
    with pytest.raises(ValueError) as err:
        sl.has(math_nan)
    assert err.value.args[0] == \
        'Can not work with something that does not compare equal to itself.'
    assert sl.lacks_integrity() == 0

#------- END: Some specialised insert tests for particular types ----------

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_has(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.has(value)
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_at(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.at(0) == value
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_at_fails(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    with pytest.raises(IndexError) as err:
        sl.at(1)
    assert err.value.args[0] == 'Index 1 out of range 0 <= index < 1'
    with pytest.raises(IndexError) as err:
        sl.at(-2)
    assert err.value.args[0] == 'Index -2 out of range -2 < index <= -1'

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_insert_different_and_size(typ, value):
    sl = orderedstructs.SkipList(typ)
    for i in range(16):
        assert sl.lacks_integrity() == 0
        assert sl.size() == i
        sl.insert(value * i)
        assert sl.has(value * i)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1
    
@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_insert_same_and_size(typ, value):
    sl = orderedstructs.SkipList(typ)
    for i in range(16):
        assert sl.lacks_integrity() == 0
        assert sl.size() == i
        sl.insert(value)
        assert sl.has(value)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_remove(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.size() == 0
    assert sl.lacks_integrity() == 0
    assert sl.insert(value) is None
    assert sl.size() == 1
    assert sl.remove(value) is None
    assert sl.lacks_integrity() == 0
    assert sl.size() == 0

@pytest.mark.parametrize('typ,value',
                         [
#                           (int_type, example_int_value),
#                           (float, 8.0),
#                           (bytes, b'abc'),
#                           (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
#                           (object, 'abc'),
                          ])
def test_remove(typ, value):
    sl = orderedstructs.SkipList(typ)
    for i in range(16):
        assert sl.size() == i
        assert sl.lacks_integrity() == 0
        print('Inserting {}'.format(repr(value * i)))
        assert sl.insert(value * i) is None
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1
    for i in range(16):
        assert sl.size() == 16 - i
        assert sl.lacks_integrity() == 0
        print('Removing  {}'.format(repr(value * i)))
        assert sl.remove(value * i) is None
        assert sl.lacks_integrity() == 0
        assert sl.size() == 16 - i - 1
    assert sl.lacks_integrity() == 0
    assert sl.size() == 0

@pytest.mark.parametrize('typ,value',
                         [(int_type, example_int_value),
                          (float, 8.0),
                          (bytes, b'abc'),
                          (object, example_int_value),
                          (object, 8.0),
                          (object, b'abc'),
                          (object, 'abc'),
                          ])
def test_single_remove_fails(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.size() == 0
    assert sl.lacks_integrity() == 0
    assert sl.insert(value) is None
    assert sl.size() == 1
    bogus_value = value * 2
    with pytest.raises(ValueError) as err:
        sl.remove(bogus_value)
    assert err.value.args[0].startswith('Value ')
    assert err.value.args[0].endswith(' not found.')
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ, seq',
                         [
                            (int_type, example_int_seq),
                            (float, (1.0, 2.0, 4.0, 8.0)),
                            (bytes, (b'abc', b'def', b'ghi', b'jkl')),
                            (object, example_int_seq),
                            (object, (1.0, 2.0, 4.0, 8.0)),
                            (object, (b'abc', b'def', b'ghi', b'jkl')),
                            (object, ('abc', 'def', 'ghi', 'jkl')),
                          ])
def test_at_seq(typ, seq):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    for value in seq:
        sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.at_seq(0, 0) == tuple()
    assert sl.at_seq(0, 2) == seq[:2]
    assert sl.at_seq(2, 2) == seq[2:4]

@pytest.mark.parametrize('typ', [int_type, float, bytes, object])
def test_at_seq_empty_fails(typ):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    with pytest.raises(IndexError) as err:
        sl.at_seq(0, 0)
    assert err.value.args[0] == 'Index 0 out of range -1 < index <= -1'
    with pytest.raises(IndexError) as err:
        sl.at_seq(-1, 0)
    assert err.value.args[0] == 'Index -1 out of range -1 < index <= -1'

@pytest.mark.parametrize('typ, seq',
                         [
                            (int_type, example_int_seq),
                            (float, (1.0, 2.0, 4.0, 8.0)),
                            (bytes, (b'abc', b'def', b'ghi', b'jkl')),
                            (object, example_int_seq),
                            (object, (1.0, 2.0, 4.0, 8.0)),
                            (object, (b'abc', b'def', b'ghi', b'jkl')),
                            (object, ('abc', 'def', 'ghi', 'jkl')),
                          ])
def test_at_seq_raises_types(typ, seq):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    for value in seq:
        sl.insert(value)
    assert sl.lacks_integrity() == 0
    with pytest.raises(TypeError):
        sl.at_seq('0', 2)
    with pytest.raises(TypeError):
        sl.at_seq(0, '2')
    with pytest.raises(IndexError):
        sl.at_seq(0, -2)
    with pytest.raises(IndexError):
        sl.at_seq(0, 5)
    with pytest.raises(IndexError):
        sl.at_seq(1, 4)
    with pytest.raises(IndexError):
        sl.at_seq(2, 3)
    with pytest.raises(IndexError):
        sl.at_seq(3, 2)
    with pytest.raises(IndexError):
        sl.at_seq(4, 1)
    with pytest.raises(IndexError):
        sl.at_seq(5, 0)

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_index(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.index(value) == 0
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_index_raises_out_of_range(typ, value):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    with pytest.raises(ValueError) as err:
        assert sl.index(value) == 0
    assert err.value.args[0].startswith('Value ')
    assert err.value.args[0].endswith(' not found.')
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ, seq',
                         [(int_type, example_int_seq),
                          (float, (1.0, 2.0, 4.0, 8.0)),
                          (bytes, (b'abc', b'def', b'ghi', b'jkl'))])
def test_index_sequence(typ, seq):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    for value in seq:
        sl.insert(value)
    assert sl.lacks_integrity() == 0
    reference = sorted(seq)
    for v in seq:
        assert reference.index(v) == sl.index(v)

@pytest.mark.parametrize('typ, seq',
                         [(int_type, reversed(example_int_seq)),
                          (float, reversed((1.0, 2.0, 4.0, 8.0))),
                          (bytes, reversed((b'abc', b'def', b'ghi', b'jkl')))])
def test_index_sequence_reversed(typ, seq):
    sl = orderedstructs.SkipList(typ)
    assert sl.lacks_integrity() == 0
    for value in seq:
        sl.insert(value)
    assert sl.lacks_integrity() == 0
    reference = sorted(seq)
    for v in seq:
        assert reference.index(v) == sl.index(v)

#----------- Test using SkipList for rolling median. -------
# def test_rolling_median():
#     sl = orderedstructs.SkipList()
#     assert sl.lacks_integrity() == 0
#     win_len = 7
#     data_len = 1024
#     result = []
#     for i in range(data_len):
#         assert sl.lacks_integrity() == 0
#         sl.insert(float(i))
#         assert sl.lacks_integrity() == 0
#         if i >= win_len:
#             result.append(sl.at(win_len // 2))
#             assert sl.lacks_integrity() == 0
#             sl.remove(float(i - win_len))
#             assert sl.lacks_integrity() == 0
#     assert result == [float(v) for v in
#                       range(win_len // 2, data_len - win_len // 2 - 1)]

#--------- END: Test using SkipList for rolling median. ----

#----------------- Test probabilistic results. -----------
SEEDTREE_DEPTH = 10
SEED_DICT = seed_tree.find_seeds_for_sequences(SEEDTREE_DEPTH,
                                               orderedstructs.seed_rand,
                                               orderedstructs.toss_coin)

def test_seed_tree_permutations():
    """Test the seed_tree using the orderedstructs random number generator which is
    actually std::rand() and std::srand() for seeding."""
    results = []
    for seq in itertools.product(range(2), repeat=SEEDTREE_DEPTH):
        orderedstructs.seed_rand(SEED_DICT[seq])
        random_tosses = tuple([orderedstructs.toss_coin() for _i in range(SEEDTREE_DEPTH)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_node_height_raises(typ):
    sl = orderedstructs.SkipList(typ)
    with pytest.raises(TypeError):
        sl.node_height('123')
    with pytest.raises(TypeError):
        sl.node_height(1, 2)

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_one(typ, value):
    """Set up the random number generator so that each node is height 1."""
    num = 3
    seq = (0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == 1
        assert sl.lacks_integrity() == 0
        
@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_two(typ, value):
    """Set up the random number generator so that each node is height 2."""
    num = 3
    seq = (1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == 2
        assert sl.lacks_integrity() == 0
        
@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_three(typ, value):
    """Set up the random number generator so that each node is height 3."""
    num = 2
    seq = (1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == 3
        assert sl.lacks_integrity() == 0
        
@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_four(typ, value):
    """Set up the random number generator so that each node is height 4."""
    num = 2
    seq = (1, 1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == 4
        assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_levels_one_two_three_four(typ, value):
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4."""
    num_nodes = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == i + 1
        assert sl.lacks_integrity() == 0
        
@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_levels_one_two_three_four_node_width(typ, value):
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4."""
    num_nodes = 4
    seq = (0,
           1, 0,
           1, 1, 0,
           1, 1, 1, 0,
    )
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    expected = {
        0 : (1,),
        1 : (1, 1),
        2 : (1, 1, 1),
        3 : (1, 1, 1, 1),
    }
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == len(expected[i])
        for level in range(sl.node_height(i)):
            assert sl.node_width(i, level) == expected[i][level]
        assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_node_width(typ, value):
    """See: skiplist/doc/dot/doc_insert_remove.svg to node 3"""
    num_nodes = 4
    seq = (1, 1, 0,  # Node value 0
           1, 0,  # Node value 1
           1, 1, 0,  # Node value 2
           0,  # Node value 3
    )
    assert len(seq) <= SEEDTREE_DEPTH
    orderedstructs.seed_rand(SEED_DICT[seq])
    sl = orderedstructs.SkipList(typ)
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        sl.insert(value * i)
        assert sl.lacks_integrity() == 0
    # Now test
    expected = {
        0 : (1, 1, 2),
        1 : (1, 1),
        2 : (1, 2, 2),
        3 : (1,),
    }
    for i in range(num_nodes):
        assert sl.lacks_integrity() == 0
        assert sl.node_height(i) == len(expected[i])
        for level in range(sl.node_height(i)):
            assert sl.node_width(i, level) == expected[i][level]
        assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ', [int_type, float, bytes])
def test_node_width_raises(typ):
    sl = orderedstructs.SkipList(typ)
    with pytest.raises(TypeError):
        sl.node_width(1)
    with pytest.raises(TypeError):
        sl.node_width('1', 2)
    with pytest.raises(TypeError):
        sl.node_width(1, '2')

@pytest.mark.parametrize('typ, value',
                         [(int_type, example_int_value), (float, 8.0), (bytes, b'abc')])
def test_insert_levels_one_two_three_four_permutations(typ, value):
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4.
    The values to be inserted are all permutations of range(4)."""
    num = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    for values in itertools.permutations(range(num)):
        orderedstructs.seed_rand(SEED_DICT[seq])
        sl = orderedstructs.SkipList(typ)
        for i in values:
            assert sl.lacks_integrity() == 0
            sl.insert(value * i)
            assert sl.lacks_integrity() == 0
        # Now test
        for i in range(num):
            assert sl.lacks_integrity() == 0
            assert sl.has(value * i)
            assert sl.at(i) == value * i
            assert sl.lacks_integrity() == 0

#----------------- END: Test probabilistic results. ----------- 

if __name__ == '__main__':
    test_remove()

