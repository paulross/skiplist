'''
Created on 21 Dec 2015

@author: paulross
'''
import itertools
import random

import hypothesis
import hypothesis.strategies as hst

from . import seed_tree

def test_time_to_depth_dict():
    for depth in range(1, 10):
        seed_tree.find_seeds_for_sequences(depth,
                                           seed_tree.RANDOM_SEED,
                                           seed_tree.FAIR_COIN)

def test_seed_tree_permutations_dict():
    depth = 8
    seed_dict = seed_tree.find_seeds_for_sequences(depth,
                                                   seed_tree.RANDOM_SEED,
                                                   seed_tree.FAIR_COIN)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([seed_tree.FAIR_COIN() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

def test_one_over_four_sided_coin_dict():
    rand_fn = lambda: random.randint(0, 4) == 0
    depth = 5
    seed_dict = seed_tree.find_seeds_for_sequences(depth,
                                                   seed_tree.RANDOM_SEED,
                                                   rand_fn)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([rand_fn() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

def test_three_over_four_sided_coin():
    rand_fn = lambda: random.randint(0, 4) > 0
    depth = 5
    seed_dict = seed_tree.find_seeds_for_sequences(depth,
                                                   seed_tree.RANDOM_SEED,
                                                   rand_fn)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([rand_fn() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

HYPOTHESIS_STD_DEPTH = 8


@hypothesis.settings(deadline=None)
@hypothesis.given(hst.lists(hst.lists(hst.booleans(),
                                      min_size=1,
                                      max_size=HYPOTHESIS_STD_DEPTH)))
def test_hypothesis_has(lst):
    seed_dict = seed_tree.find_seeds_for_sequences(HYPOTHESIS_STD_DEPTH,
                                                   seed_tree.RANDOM_SEED,
                                                   seed_tree.FAIR_COIN)
    for seq in lst:
        assert tuple(seq) in seed_dict


@hypothesis.settings(deadline=None)
@hypothesis.given(hst.lists(hst.lists(hst.booleans(),
                                      min_size=1,
                                      max_size=HYPOTHESIS_STD_DEPTH)))
def test_hypothesis_rand_generates_correctly(lst):
    seed_dict = seed_tree.find_seeds_for_sequences(HYPOTHESIS_STD_DEPTH,
                                                   seed_tree.RANDOM_SEED,
                                                   seed_tree.FAIR_COIN)
    for seq in lst:
        seq = tuple(seq)
        random.seed(seed_dict[seq])
        random_tosses = tuple([seed_tree.FAIR_COIN() for _i in range(len(seq))])
        assert seq == random_tosses
