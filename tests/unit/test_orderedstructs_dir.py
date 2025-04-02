import sys

import pytest

import orderedstructs


def test_orderedstructs_dir():
    assert dir(orderedstructs) == ['SkipList',
                                   '__build_docs__',
                                   '__build_target__',
                                   '__build_time__',
                                   '__build_type__',
                                   '__doc__',
                                   '__file__',
                                   '__loader__',
                                   '__name__',
                                   '__package__',
                                   '__spec__',
                                   '__version__',
                                   'max_long',
                                   'min_long',
                                   'seed_rand',
                                   'toss_coin']


@pytest.mark.parametrize(
    'attribute',
    (
            '__build_docs__',
            '__build_target__',
            '__build_time__',
            '__build_type__',
    ),
)
def test_orderedstructs_hasattr(attribute):
    assert hasattr(orderedstructs, attribute)


@pytest.mark.parametrize(
    'attribute, expected',
    (
            ('__build_docs__', 'Standard build.'),
            # ('__build_target__', f'{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}'),
            ('__build_target__', sys.version.split()[0]),
            ('__build_type__', 'release'),
    )
)
def test_orderedstructs_getattr(attribute, expected):
    assert hasattr(orderedstructs, attribute)
    assert getattr(orderedstructs, attribute) == expected


@pytest.mark.skipif(not (sys.version_info.minor < 11), reason='Python < 3.11')
def test_orderedstructs_skiplist_dir():
    assert dir(orderedstructs.SkipList) == ['__class__',
                                            '__delattr__',
                                            '__dir__',
                                            '__doc__',
                                            '__eq__',
                                            '__format__',
                                            '__ge__',
                                            '__getattribute__',
                                            '__gt__',
                                            '__hash__',
                                            '__init__',
                                            '__init_subclass__',
                                            '__le__',
                                            '__len__',
                                            '__lt__',
                                            '__ne__',
                                            '__new__',
                                            '__reduce__',
                                            '__reduce_ex__',
                                            '__repr__',
                                            '__setattr__',
                                            '__sizeof__',
                                            '__str__',
                                            '__subclasshook__',
                                            'at',
                                            'at_seq',
                                            'dot_file',
                                            'has',
                                            'height',
                                            'index',
                                            'insert',
                                            'lacks_integrity',
                                            'node_height',
                                            'node_width',
                                            'remove',
                                            'size',
                                            'size_of',
                                            'thread_safe']


@pytest.mark.skipif(not (sys.version_info.minor >= 11), reason='Python 3.11+')
def test_orderedstructs_skiplist_dir():
    assert dir(orderedstructs.SkipList) == ['__class__',
                                            '__delattr__',
                                            '__dir__',
                                            '__doc__',
                                            '__eq__',
                                            '__format__',
                                            '__ge__',
                                            '__getattribute__',
                                            '__getstate__',
                                            '__gt__',
                                            '__hash__',
                                            '__init__',
                                            '__init_subclass__',
                                            '__le__',
                                            '__len__',
                                            '__lt__',
                                            '__ne__',
                                            '__new__',
                                            '__reduce__',
                                            '__reduce_ex__',
                                            '__repr__',
                                            '__setattr__',
                                            '__sizeof__',
                                            '__str__',
                                            '__subclasshook__',
                                            'at',
                                            'at_seq',
                                            'dot_file',
                                            'has',
                                            'height',
                                            'index',
                                            'insert',
                                            'lacks_integrity',
                                            'node_height',
                                            'node_width',
                                            'remove',
                                            'size',
                                            'size_of',
                                            'thread_safe']
