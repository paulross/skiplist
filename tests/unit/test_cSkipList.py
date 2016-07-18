import itertools
import math

import pytest

import cSkipList
import SeedTree

def test_module():
    assert hasattr(cSkipList, '__version__')
    assert hasattr(cSkipList, '__build_time__')
    assert hasattr(cSkipList, '__build_type__')
    assert cSkipList.__build_type__ in ('debug', 'release')
    assert hasattr(cSkipList, '__build_target__')

@pytest.mark.parametrize('typ', [int, float, bytes])
def test_ctor(typ):
    assert cSkipList.PySkipList(typ) is not None

def test_ctor_raises_no_type():
    with pytest.raises(TypeError) as err:
        cSkipList.PySkipList()
    assert err.value.args[0] == "Required argument 'value_type' (pos 1) not found"

def test_ctor_raises_not_a_type():
    with pytest.raises(ValueError) as err:
        cSkipList.PySkipList('')
    assert err.value.args[0] == \
        'Argument to __init__ must be long, float or bytes, not "str"'

@pytest.mark.parametrize('typ', [complex, tuple, list, dict, ])
def test_ctor_raises_wrong_type(typ):
    with pytest.raises(ValueError) as err:
        cSkipList.PySkipList(typ)
    print(err.value.args[0])
    assert err.value.args[0] == \
        'Argument to __init__ must be long, float or bytes, not "type"'

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_insert(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    assert sl.insert(value) is None
    assert sl.lacks_integrity() == 0
    
@pytest.mark.parametrize('typ,value',
                         [
                            (int, 8.0),
                            (int, '8'),
                            (float, 8),
                            (float, '8.0'),
                            (bytes, 'abc'),
                            (bytes, u'abc'),
                            (bytes, r'abc'),
                        ])
def test_single_insert_raises(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    with pytest.raises(TypeError):
        sl.insert(value)
    assert sl.lacks_integrity() == 0

#------- Some specialised insert tests for particular types ----------
def test_single_insert_long_min_long():
    sl = cSkipList.PySkipList(int)
    assert sl.lacks_integrity() == 0
    sl.insert(cSkipList.min_long())
    assert sl.size() == 1
    assert sl.lacks_integrity() == 0

def test_single_insert_long_max_long():
    sl = cSkipList.PySkipList(int)
    assert sl.lacks_integrity() == 0
    sl.insert(cSkipList.max_long())
    assert sl.size() == 1
    assert sl.lacks_integrity() == 0

def test_single_insert_long_underflow_raises():
    sl = cSkipList.PySkipList(int)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.insert(cSkipList.min_long() - 1)
    assert sl.lacks_integrity() == 0

def test_single_insert_long_overflow_raises():
    sl = cSkipList.PySkipList(int)
    assert sl.lacks_integrity() == 0
    with pytest.raises(OverflowError):
        sl.insert(cSkipList.max_long() + 1)
    assert sl.lacks_integrity() == 0

def test_single_insert_float_NaN_raises():
    sl = cSkipList.PySkipList(float)
    assert sl.lacks_integrity() == 0
    with pytest.raises(ValueError) as err:
        sl.insert(math.nan)
    assert err.value.args[0] == 'Can not insert NaN.'
    assert sl.lacks_integrity() == 0

#------- END: Some specialised insert tests for particular types ----------

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_has(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.has(value)
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_at(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.at(0) == value
    assert sl.lacks_integrity() == 0

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_at_fails(typ, value):
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 4), (float, 4.0), (bytes, b'a')])
def test_single_insert_different_and_size(typ, value):
    sl = cSkipList.PySkipList(typ)
    for i in range(16):
        assert sl.lacks_integrity() == 0
        assert sl.size() == i
        sl.insert(value * i)
        assert sl.has(value * i)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1
    
@pytest.mark.parametrize('typ,value',
                         [(int, 4), (float, 4.0), (bytes, b'a')])
def test_single_insert_same_and_size(typ, value):
    sl = cSkipList.PySkipList(typ)
    for i in range(16):
        assert sl.lacks_integrity() == 0
        assert sl.size() == i
        sl.insert(value)
        assert sl.has(value)
        assert sl.lacks_integrity() == 0
        assert sl.size() == i + 1

@pytest.mark.parametrize('typ,value',
                         [(int, 4), (float, 4.0), (bytes, b'a')])
def test_single_remove(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.size() == 0
    assert sl.lacks_integrity() == 0
    assert sl.insert(value) is None
    assert sl.size() == 1
    assert sl.remove(value) is None
    assert sl.lacks_integrity() == 0
    assert sl.size() == 0

@pytest.mark.parametrize('typ,value',
                         [(int, 4), (float, 4.0), (bytes, b'a')])
def test_remove(typ, value):
    sl = cSkipList.PySkipList(typ)
    for i in range(16):
        assert sl.size() == i
        assert sl.lacks_integrity() == 0
        assert sl.insert(value * i) is None
        assert sl.size() == i + 1
    for i in range(16):
        assert sl.size() == 16 - i
        assert sl.remove(value * i) is None
        assert sl.size() == 16 - i - 1
    assert sl.lacks_integrity() == 0
    assert sl.size() == 0

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_remove_fails(typ, value):
    sl = cSkipList.PySkipList(typ)
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
                         [(int, (1, 2, 4, 8,)),
                          (float, (1.0, 2.0, 4.0, 8.0)),
                          (bytes, (b'abc', b'def', b'ghi', b'jkl'))])
def test_at_seq(typ, seq):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    for value in seq:
        sl.insert(value)
    assert sl.lacks_integrity() == 0
    assert sl.at_seq(0, 0) == tuple()
    assert sl.at_seq(0, 2) == seq[:2]
    assert sl.at_seq(2, 2) == seq[2:4]

@pytest.mark.parametrize('typ', [int, float, bytes])
def test_at_seq_empty_fails(typ):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacks_integrity() == 0
    with pytest.raises(IndexError) as err:
        sl.at_seq(0, 0)
    assert err.value.args[0] == 'Index 0 out of range -1 < index <= -1'
    with pytest.raises(IndexError) as err:
        sl.at_seq(-1, 0)
    assert err.value.args[0] == 'Index -1 out of range -1 < index <= -1'

@pytest.mark.parametrize('typ, seq',
                         [(int, (1, 2, 4, 8,)),
                          (float, (1.0, 2.0, 4.0, 8.0)),
                          (bytes, (b'abc', b'def', b'ghi', b'jkl'))])
def test_at_seq_raises_types(typ, seq):
    sl = cSkipList.PySkipList(typ)
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

#----------- Test using SkipList for rolling median. -------
# def test_rolling_median():
#     sl = cSkipList.PySkipList()
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
SEED_DICT = SeedTree.find_seeds_for_sequences(SEEDTREE_DEPTH,
                                              cSkipList.seed_rand,
                                              cSkipList.toss_coin)

def test_seed_tree_permutations():
    """Test the SeedTree using the cSkipList random number generator which is
    actually std::rand() and std::srand() for seeding."""
    results = []
    for seq in itertools.product(range(2), repeat=SEEDTREE_DEPTH):
        cSkipList.seed_rand(SEED_DICT[seq])
        random_tosses = tuple([cSkipList.toss_coin() for _i in range(SEEDTREE_DEPTH)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

@pytest.mark.parametrize('typ', [int, float, bytes])
def test_node_height_raises(typ):
    sl = cSkipList.PySkipList(typ)
    with pytest.raises(TypeError):
        sl.node_height('123')
    with pytest.raises(TypeError):
        sl.node_height(1, 2)

@pytest.mark.parametrize('typ, value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_one(typ, value):
    """Set up the random number generator so that each node is height 1."""
    num = 3
    seq = (0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_two(typ, value):
    """Set up the random number generator so that each node is height 2."""
    num = 3
    seq = (1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_three(typ, value):
    """Set up the random number generator so that each node is height 3."""
    num = 2
    seq = (1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_all_level_four(typ, value):
    """Set up the random number generator so that each node is height 4."""
    num = 2
    seq = (1, 1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_levels_one_two_three_four(typ, value):
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4."""
    num_nodes = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
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
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_node_width(typ, value):
    """See: skiplist/doc/dot/doc_insert_remove.svg to node 3"""
    num_nodes = 4
    seq = (1, 1, 0,  # Node value 0
           1, 0,  # Node value 1
           1, 1, 0,  # Node value 2
           0,  # Node value 3
    )
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList(typ)
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

@pytest.mark.parametrize('typ', [int, float, bytes])
def test_node_width_raises(typ):
    sl = cSkipList.PySkipList(typ)
    with pytest.raises(TypeError):
        sl.node_width(1)
    with pytest.raises(TypeError):
        sl.node_width('1', 2)
    with pytest.raises(TypeError):
        sl.node_width(1, '2')

@pytest.mark.parametrize('typ, value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_insert_levels_one_two_three_four_permutations(typ, value):
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4.
    The values to be inserted are all permutations of range(4)."""
    num = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    for values in itertools.permutations(range(num)):
        cSkipList.seed_rand(SEED_DICT[seq])
        sl = cSkipList.PySkipList(typ)
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
