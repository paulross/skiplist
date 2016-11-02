"""Some specific PyObject* tests"""
import logging
import sys
import threading
import time

import pytest

import orderedstructs

from SkipList_common import TotalOrdered, OrderedLt

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

def insert_and_remove_TotalOrdered(sl, value, count, verbose):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    lacks_integrity = sl.lacks_integrity()
    assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
    to_value = TotalOrdered(value)
    for _i in range(count):
        if verbose:
            logging.debug('Inserting')
            sl.insert(to_value)
            logging.debug('Inserting - DONE')
            logging.debug('Checking integrity [A]')
            lacks_integrity = sl.lacks_integrity()
            logging.debug('Checking integrity [A] - DONE')
            assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
            logging.debug('Removing')
            removed_value = sl.remove(to_value)
            logging.debug('Removing - DONE')
            assert id(to_value) == id(removed_value)
            logging.debug('Checking integrity [B]')
            lacks_integrity = sl.lacks_integrity()
            logging.debug('Checking integrity [B] - DONE')
            assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
        else:
            sl.insert(to_value)
#             lacks_integrity = sl.lacks_integrity()
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
            removed_value = sl.remove(to_value)
            assert id(to_value) == id(removed_value)
#             lacks_integrity = sl.lacks_integrity()
#             assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def test_one():
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    sl.insert(TotalOrdered(1000))
    NUM_INSERTS = 100000
    threads = [
        threading.Thread(
            name='insert_remove[{:d}]'.format((i + 2) * 1000),
            target=insert_and_remove_TotalOrdered,
            args=(sl, (i + 2) * 1000, NUM_INSERTS, False)) for i in range(2)]
    for thread in threads:
        thread.start()
    logging.debug('Waiting for worker threads')
    main_thread = threading.currentThread()
    for t in threading.enumerate():
        if t is not main_thread:
            t.join()
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))

if __name__ == '__main__':
    test_one()
