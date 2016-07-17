import itertools

import pytest

import cSkipList
import SeedTree

@pytest.mark.parametrize('typ', [int, float, bytes])
def test_ctor(typ):
    assert cSkipList.PySkipList(typ) is not None

@pytest.mark.parametrize('typ,value',
                         [(int, 8), (float, 8.0), (bytes, b'abc')])
def test_single_insert(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacksIntegrity() == 0
    assert sl.insert(value) is None
    assert sl.lacksIntegrity() == 0
    
@pytest.mark.parametrize('typ,value',
                         [
                            (int, 8.0),
                            (float, 8),
                            (bytes, 'abc')
                        ])
def test_single_insert_raises(typ, value):
    sl = cSkipList.PySkipList(typ)
    assert sl.lacksIntegrity() == 0
    with pytest.raises(TypeError):
        sl.insert(value)
    assert sl.lacksIntegrity() == 0

def test_single_insert_raises_str():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    with pytest.raises(TypeError):
        sl.insert('8')
    assert sl.lacksIntegrity() == 0

def test_single_has():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(8.0)
    assert sl.lacksIntegrity() == 0
    assert sl.has(8.0)
    assert sl.lacksIntegrity() == 0

def test_single_at():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(8.0)
    assert sl.lacksIntegrity() == 0
    assert sl.at(0) == 8.0
    assert sl.lacksIntegrity() == 0

def test_single_at_fails():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(8.0)
    assert sl.lacksIntegrity() == 0
    with pytest.raises(IndexError) as err:
        sl.at(1)
    assert err.value.message == 'Index 1 out of range 0 <= index < 1'
    with pytest.raises(IndexError) as err:
        sl.at(-2)
    assert err.value.message == 'Index -2 out of range -2 < index <= -1'

def test_single_insert_different_and_size():
    sl = cSkipList.PySkipList()
    for i in range(16):
        assert sl.lacksIntegrity() == 0
        assert sl.size() == i
        sl.insert(float(i))
        assert sl.has(float(i))
        assert sl.lacksIntegrity() == 0
        assert sl.size() == i + 1
    
def test_single_insert_same_and_size():
    sl = cSkipList.PySkipList()
    for i in range(16):
        assert sl.lacksIntegrity() == 0
        assert sl.size() == i
        sl.insert(8.0)
        assert sl.lacksIntegrity() == 0
        assert sl.size() == i + 1

def test_single_remove():
    sl = cSkipList.PySkipList()
    assert sl.size() == 0
    assert sl.lacksIntegrity() == 0
    assert sl.insert(8.0) is None
    assert sl.size() == 1
    assert sl.remove(8.0) is None
    assert sl.lacksIntegrity() == 0
    assert sl.size() == 0

def test_remove():
    sl = cSkipList.PySkipList()
    for i in range(16):
        assert sl.size() == i
        assert sl.lacksIntegrity() == 0
        assert sl.insert(float(i)) is None
        assert sl.size() == i + 1
    for i in range(16):
        assert sl.size() == 16 - i
        assert sl.remove(float(i)) is None
        assert sl.size() == 16 - i - 1
    assert sl.lacksIntegrity() == 0
    assert sl.size() == 0

def test_single_remove():
    sl = cSkipList.PySkipList()
    assert sl.size() == 0
    assert sl.lacksIntegrity() == 0
    assert sl.insert(8.0) is None
    assert sl.size() == 1
    with pytest.raises(ValueError) as err:
        sl.remove(16.0)
    assert err.value.message == 'Value 16 not found.'
    assert sl.lacksIntegrity() == 0

def test_at_seq():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(1.0)
    sl.insert(2.0)
    sl.insert(4.0)
    sl.insert(16.0)
    assert sl.lacksIntegrity() == 0
    assert sl.at_seq(0, 0) == tuple()
    assert sl.at_seq(0, 2) == (1.0, 2.0)

def test_at_seq_empty():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(1.0)
    sl.insert(2.0)
    sl.insert(4.0)
    sl.insert(16.0)
    assert sl.lacksIntegrity() == 0
    assert sl.at_seq(0, 0) == tuple()

def test_at_seq_empty_fails():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    with pytest.raises(IndexError) as err:
        sl.at_seq(0, 0)
    assert err.value.message == 'Index 0 out of range -1 < index <= -1'
    with pytest.raises(IndexError) as err:
        sl.at_seq(-1, 0)
    assert err.value.message == 'Index -1 out of range -1 < index <= -1'

def test_at_seq_raises_types():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    sl.insert(1.0)
    sl.insert(2.0)
    sl.insert(4.0)
    sl.insert(16.0)
    assert sl.lacksIntegrity() == 0
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
def test_rolling_median():
    sl = cSkipList.PySkipList()
    assert sl.lacksIntegrity() == 0
    win_len = 7
    data_len = 1024
    result = []
    for i in range(data_len):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
        if i >= win_len:
            result.append(sl.at(win_len // 2))
            assert sl.lacksIntegrity() == 0
            sl.remove(float(i - win_len))
            assert sl.lacksIntegrity() == 0
    assert result == [float(v) for v in
                      range(win_len // 2, data_len - win_len // 2 - 1)]

#--------- END: Test using SkipList for rolling median. ----

#----------------- Test probabilistic results. -----------
SEEDTREE_DEPTH = 10
# SEEDTREE = SeedTree.SeedTree.create(SEEDTREE_DEPTH,
#                                     cSkipList.seed_rand,
#                                     cSkipList.toss_coin)

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

def test_node_height_raises():
    sl = cSkipList.PySkipList()
    with pytest.raises(TypeError):
        sl.nodeHeight('123')
    with pytest.raises(TypeError):
        sl.nodeHeight(1, 2)

def test_insert_all_level_one():
    """Set up the random number generator so that each node is height 1."""
    num = 3
    seq = (0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == 1
        assert sl.lacksIntegrity() == 0
        
def test_insert_all_level_two():
    """Set up the random number generator so that each node is height 2."""
    num = 3
    seq = (1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == 2
        assert sl.lacksIntegrity() == 0
        
def test_insert_all_level_three():
    """Set up the random number generator so that each node is height 3."""
    num = 2
    seq = (1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == 3
        assert sl.lacksIntegrity() == 0
        
def test_insert_all_level_four():
    """Set up the random number generator so that each node is height 4."""
    num = 2
    seq = (1, 1, 1, 0,) * num
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    for i in range(num):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == 4
        assert sl.lacksIntegrity() == 0
        

def test_insert_levels_one_two_three_four():
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4."""
    num_nodes = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == i + 1
        assert sl.lacksIntegrity() == 0
        
def test_insert_levels_one_two_three_four_nodeWidth():
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
    sl = cSkipList.PySkipList()
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    expected = {
        0 : (1,),
        1 : (1, 1),
        2 : (1, 1, 1),
        3 : (1, 1, 1, 1),
    }
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == len(expected[i])
        for level in range(sl.nodeHeight(i)):
            assert sl.nodeWidth(i, level) == expected[i][level]
        assert sl.lacksIntegrity() == 0

def test_insert_nodeWidth():
    """See: skiplist/doc/dot/doc_insert_remove.svg to node 3"""
    num_nodes = 4
    seq = (1, 1, 0,  # Node value 0
           1, 0,  # Node value 1
           1, 1, 0,  # Node value 2
           0,  # Node value 3
    )
    assert len(seq) <= SEEDTREE_DEPTH
    cSkipList.seed_rand(SEED_DICT[seq])
    sl = cSkipList.PySkipList()
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        sl.insert(float(i))
        assert sl.lacksIntegrity() == 0
    # Now test
    expected = {
        0 : (1, 1, 2),
        1 : (1, 1),
        2 : (1, 2, 2),
        3 : (1,),
    }
    for i in range(num_nodes):
        assert sl.lacksIntegrity() == 0
        assert sl.nodeHeight(i) == len(expected[i])
        for level in range(sl.nodeHeight(i)):
            assert sl.nodeWidth(i, level) == expected[i][level]
        assert sl.lacksIntegrity() == 0

def test_node_width_raises():
    sl = cSkipList.PySkipList()
    with pytest.raises(TypeError):
        sl.nodeWidth(1)
    with pytest.raises(TypeError):
        sl.nodeWidth('1', 2)
    with pytest.raises(TypeError):
        sl.nodeWidth(1, '2')

def test_insert_levels_one_two_three_four_permutations():
    """Set up the random number generator so that the first node is inserted
    with height=1, second node inserted with height=2, third node inserted
    with height=3 and fourth node inserted with height=4.
    The values to be inserted are all permutations of range(4)."""
    num = 4
    seq = (0, 1, 0, 1, 1, 0, 1, 1, 1, 0)
    assert len(seq) <= SEEDTREE_DEPTH
    for values in itertools.permutations(range(num)):
        cSkipList.seed_rand(SEED_DICT[seq])
        sl = cSkipList.PySkipList()
        for i in values:
            assert sl.lacksIntegrity() == 0
            sl.insert(float(i))
            assert sl.lacksIntegrity() == 0
        # Now test
        for i in range(num):
            assert sl.lacksIntegrity() == 0
            assert sl.has(float(i))
            assert sl.at(i) == i
            assert sl.lacksIntegrity() == 0

#----------------- END: Test probabilistic results. ----------- 
