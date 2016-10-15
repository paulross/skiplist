import pytest

import orderedstructs

# from .compat_23 import int_type, math_nan, example_int_value, example_int_seq

def test_remove():
    test_values = [
#                           (int_type, example_int_value),
#                           (float, 8.0),
#                           (bytes, b'abc'),
#                           (object, example_int_value),
                          (object, 8.0),
                        (object, 'abc'),
#                           (object, b'abc'),
                          ]
    for typ, value in test_values:
        print('Starting {} {}'.format(typ, value))
        sl = orderedstructs.SkipList(typ)
        for i in range(16):
            assert sl.size() == i
            assert sl.lacks_integrity() == 0
            print('Inserting {}'.format(repr(value * i)), flush=True)
            assert sl.insert(value * i) is None
            assert sl.lacks_integrity() == 0
            assert sl.size() == i + 1
        for i in range(16):
            assert sl.size() == 16 - i
            assert sl.lacks_integrity() == 0
            print('Removing  {}'.format(repr(value * i)), flush=True)
            assert sl.remove(value * i) is None
            assert sl.lacks_integrity() == 0
            assert sl.size() == 16 - i - 1
        assert sl.lacks_integrity() == 0
        assert sl.size() == 0
        print('Done {} {}'.format(typ, value), flush=True)
    print('Done all', flush=True)

if __name__ == '__main__':
    test_remove()

