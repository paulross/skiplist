"""Tests some example code in the documentation.

This example is in the introduction.
"""

import pytest


def test_A():
    import orderedstructs

    # Declare with a type. Supported types are long/float/bytes/object.
    sl = orderedstructs.SkipList(float)

    sl.insert(42.0)
    sl.insert(21.0)
    sl.insert(84.0)

    sl.has(42.0)  # True
    assert sl.has(42.0)
    sl.size()  # 3
    assert sl.size() == 3
    sl.at(1)  # 42.0, raises IndexError if index out of range
    assert sl.at(1) == 42.0

    with pytest.raises(IndexError) as err:
        sl.at(10)

    sl.remove(21.0)  # raises ValueError if value not present
    with pytest.raises(ValueError) as err:
        sl.remove(21.0)  # raises ValueError if value not present

    sl.size()  # 2
    assert sl.size() == 2
    sl.at(1)  # 84.0
    assert sl.at(1) == 84.0


def test_B():
    import functools

    import orderedstructs

    @functools.total_ordering
    class Person:
        """Simple example of ordering based on last name/first name."""

        def __init__(self, first_name, last_name):
            self.first_name = first_name
            self.last_name = last_name

        def __eq__(self, other):
            try:
                return self.last_name == other.last_name and self.first_name == other.first_name
            except AttributeError:
                return NotImplemented

        def __lt__(self, other):
            try:
                return self.last_name < other.last_name or self.first_name < other.first_name
            except AttributeError:
                return NotImplemented

        def __str__(self):
            return '{}, {}'.format(self.last_name, self.first_name)

    sl = orderedstructs.SkipList(object)

    sl.insert(Person('Peter', 'Pan'))
    sl.insert(Person('Alan', 'Pan'))
    assert sl.size() == 2
    assert str(sl.at(0)) == 'Pan, Alan'
    assert str(sl.at(1)) == 'Pan, Peter'
