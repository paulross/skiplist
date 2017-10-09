"""Some specific PyObject* tests"""
import logging
import sys
import threading
import time

import pytest

import orderedstructs

from .SkipList_common import TotalOrdered, OrderedLt

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

# def insert_and_remove_TotalOrdered_verbose(sl, value, count, verbose):
#     logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
#     lacks_integrity = sl.lacks_integrity()
#     assert lacks_integrity == 0, 'lacks_integrity is {}'.format(lacks_integrity)
#     to_value = TotalOrdered(value)
#     for _i in range(count):
#         if verbose:
#             logging.debug('Inserting')
#             sl.insert(to_value)
#             logging.debug('Inserting - DONE')
#             logging.debug('Checking integrity [A]')
#             lacks_integrity = sl.lacks_integrity()
#             logging.debug('Checking integrity [A] - DONE')
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
#             logging.debug('Removing')
#             removed_value = sl.remove(to_value)
#             logging.debug('Removing - DONE')
#             assert id(to_value) == id(removed_value)
#             logging.debug('Checking integrity [B]')
#             lacks_integrity = sl.lacks_integrity()
#             logging.debug('Checking integrity [B] - DONE')
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
#         else:
#             sl.insert(to_value)
#             lacks_integrity = sl.lacks_integrity()
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.format(lacks_integrity)
#             removed_value = sl.remove(to_value)
#             assert id(to_value) == id(removed_value)
#             lacks_integrity = sl.lacks_integrity()
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.format(lacks_integrity)
#     logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def insert_and_remove_TotalOrdered(sl, value, count):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    to_value = TotalOrdered(value)
    for _i in range(count):
        assert sl.lacks_integrity() == 0
        sl.insert(to_value)
        assert sl.lacks_integrity() == 0
        removed_value = sl.remove(to_value)
        assert id(to_value) == id(removed_value)
        assert sl.lacks_integrity() == 0
    logging.debug('  Ending: {}()'.format(sys._getframe().f_code.co_name))

def test_insert_and_remove():
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    sl.insert(TotalOrdered(1000))
    NUM_INSERTS = 10000
    threads = [
        threading.Thread(
            name='insert_remove[{:6d}]'.format((i + 2) * 1000),
            target=insert_and_remove_TotalOrdered,
            args=(sl, (i + 2) * 1000, NUM_INSERTS)
        ) for i in range(16)
    ]
    for thread in threads:
        thread.start()
    logging.debug('Waiting for worker threads')
    main_thread = threading.currentThread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))

def insert_has_remove_TotalOrdered(sl, value, count):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    to_value = TotalOrdered(value)
    for _i in range(count):
        assert sl.lacks_integrity() == 0
        sl.insert(to_value)
        assert sl.lacks_integrity() == 0
        assert sl.has(to_value)
        removed_value = sl.remove(to_value)
        assert id(to_value) == id(removed_value)
        assert not sl.has(to_value)
        assert sl.lacks_integrity() == 0
    logging.debug('  Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def test_insert_has_remove():
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    sl.insert(TotalOrdered(1000))
    NUM_INSERTS = 10000
    threads = [
        threading.Thread(
            name='insert_remove[{:6d}]'.format((i + 2) * 1000),
            target=insert_has_remove_TotalOrdered,
            args=(sl, (i + 2) * 1000, NUM_INSERTS)
        ) for i in range(16)
    ]
    for thread in threads:
        thread.start()
    logging.debug('Waiting for worker threads')
    main_thread = threading.currentThread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))

def insert_has_remove_int(sl, value, count):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    for _i in range(count):
        assert sl.lacks_integrity() == 0
        sl.insert(value)
        assert sl.lacks_integrity() == 0
        assert sl.has(value)
        removed_value = sl.remove(value)
        assert id(value) == id(removed_value)
        assert not sl.has(value)
        assert sl.lacks_integrity() == 0
    logging.debug('  Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def test_insert_has_remove_int():
    """Tests multi-threaded insert()/has()/remove() of integers converted to
    native C++ objects."""
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    # Load SkipList with one value
    sl.insert(1000)
    NUM_INSERTS = 10000
    threads = [
        threading.Thread(
            name='insert_remove[{:6d}]'.format((i + 2) * 1000),
            target=insert_has_remove_int,
            args=(sl, (i + 2) * 1000, NUM_INSERTS)
        ) for i in range(16)
    ]
    for thread in threads:
        thread.start()
    logging.debug('Waiting for worker threads')
    main_thread = threading.currentThread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))

def insert_has_remove_float(sl, value, count):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    for _i in range(count):
        assert sl.lacks_integrity() == 0
        sl.insert(value)
        assert sl.lacks_integrity() == 0
        assert sl.has(value)
        removed_value = sl.remove(value)
        assert id(value) == id(removed_value)
        assert not sl.has(value)
        assert sl.lacks_integrity() == 0
    logging.debug('  Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def test_insert_has_remove_float():
    """Tests multi-threaded insert()/has()/remove() of floats converted to
    native C++ objects."""
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    # Load SkipList with one value
    sl.insert(1000.0)
    NUM_INSERTS = 10000
    threads = [
        threading.Thread(
            name='insert_remove[{:6d}]'.format((i + 2) * 1000),
            target=insert_has_remove_float,
            args=(sl, float((i + 2) * 1000), NUM_INSERTS)
        ) for i in range(16)
    ]
    for thread in threads:
        thread.start()
    logging.debug('Waiting for worker threads')
    main_thread = threading.currentThread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))

if __name__ == '__main__':
    test_insert_and_remove()
    test_insert_has_remove()
