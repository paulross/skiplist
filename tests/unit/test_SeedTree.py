'''
Created on 21 Dec 2015

@author: paulross
'''
import itertools
import random
import time

import pytest

import SeedTree

def test_time_to_depth_dict():
    print('Time:')
    for depth in range(1, 10):
        t = time.clock()
        SeedTree.find_seeds_for_sequences(depth, SeedTree.RANDOM_SEED, SeedTree.FAIR_COIN)
        print('%4d %8.3f' % (depth, time.clock() - t))

def test_seed_tree_permutations_dict():
    depth = 8
    seed_dict = SeedTree.find_seeds_for_sequences(depth, SeedTree.RANDOM_SEED, SeedTree.FAIR_COIN)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([SeedTree.FAIR_COIN() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

def test_one_over_four_sided_coin_dict():
    rand_fn = lambda: random.randint(0, 4) == 0
    depth = 5
    seed_dict = SeedTree.find_seeds_for_sequences(depth, SeedTree.RANDOM_SEED, rand_fn)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([rand_fn() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses

def test_three_over_four_sided_coin():
    rand_fn = lambda: random.randint(0, 4) > 0
    depth = 5
    seed_dict = SeedTree.find_seeds_for_sequences(depth, SeedTree.RANDOM_SEED, rand_fn)
    results = []
    for seq in itertools.product(range(2), repeat=depth):
        random.seed(seed_dict[seq])
        random_tosses = tuple([rand_fn() for _i in range(depth)])
        results.append(seq == random_tosses)
        assert seq == random_tosses
