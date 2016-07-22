import inspect
import sys
import timeit

import pytest

import cSkipList

if sys.version_info[0] == 3:
    int_type = int
elif sys.version_info[0] == 2:
    int_type = long    

def _setup_skiplist(typ, n):
    return '\n'.join([
                       'import cSkipList',
                       'sl = cSkipList.PySkipList({})'.format(typ),
                       'for v in range({}): sl.insert({}(v))'.format(n, typ)
                       ])

def _setup_integers(n):
    return _setup_skiplist('int', n)

def _setup_floats(n):
    return _setup_skiplist('float', n)

STD_SKIP_LIST_LENGTH = 1000 * 100
STD_TIMEIT_COUNT = 1000 * 1000
MAX_FUNCTION_NAME_LENGTH = 64

def test_at_integer():
    length = STD_SKIP_LIST_LENGTH
    t = timeit.Timer('sl.at({})'.format(length // 2), _setup_integers(length))
    num_timeits = STD_TIMEIT_COUNT
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')
    
def test_at_float():
    length = STD_SKIP_LIST_LENGTH
    t = timeit.Timer('sl.at({})'.format(length // 2), _setup_floats(length))
    num_timeits = STD_TIMEIT_COUNT
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')

def test_has_integer():
    length = STD_SKIP_LIST_LENGTH
    t = timeit.Timer('sl.has({})'.format(length // 2), _setup_integers(length))
    num_timeits = STD_TIMEIT_COUNT
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')
    
def test_has_float():
    length = STD_SKIP_LIST_LENGTH
    t = timeit.Timer('sl.has({})'.format(length / 2), _setup_floats(length))
    num_timeits = STD_TIMEIT_COUNT
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')

def test_insert_remove_mid_integer():
    length = STD_SKIP_LIST_LENGTH
    cmdS = [
        'sl.insert({})'.format(length // 2),
        'sl.remove({})'.format(length // 2),
    ]
    t = timeit.Timer('\n'.join(cmdS), _setup_integers(length))
    num_timeits = STD_TIMEIT_COUNT // 10
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')
    
def test_insert_remove_mid_float():
    length = STD_SKIP_LIST_LENGTH
    cmdS = [
        'sl.insert({})'.format(length / 2),
        'sl.remove({})'.format(length / 2),
    ]
    t = timeit.Timer('\n'.join(cmdS), _setup_floats(length))
    num_timeits = STD_TIMEIT_COUNT // 10
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')

def test_index_mid_int():
    length = STD_SKIP_LIST_LENGTH
    cmdS = [
        'sl.index({})'.format(int_type(length // 2)),
    ]
    t = timeit.Timer('\n'.join(cmdS), _setup_integers(length))
    num_timeits = STD_TIMEIT_COUNT // 10
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')
     
def test_index_mid_float():
    length = STD_SKIP_LIST_LENGTH
    cmdS = [
        'sl.index({})'.format(float(length // 2)),
    ]
    t = timeit.Timer('\n'.join(cmdS), _setup_floats(length))
    num_timeits = STD_TIMEIT_COUNT // 10
    tim = t.timeit(num_timeits)
    fn_name = inspect.getframeinfo(inspect.currentframe()).function
    print('{:>{width}s}'.format(fn_name, width=(MAX_FUNCTION_NAME_LENGTH - len(fn_name))),
          '{:6.0f} (ns) '.format(1e9 * tim / num_timeits), end='')
     
