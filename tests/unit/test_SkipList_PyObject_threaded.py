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

def insert_and_remove_TotalOrdered(sl, count):
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    lacks_integrity = sl.lacks_integrity()
    assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
    for _i in range(count):
#         logging.debug('Inserting')
        sl.insert(TotalOrdered(1))
#         logging.debug('Inserting - DONE')
#         logging.debug('Checking integrity [0]')
        lacks_integrity = sl.lacks_integrity()
#         logging.debug('Checking integrity [0] - DONE')
        assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
#         logging.debug('Removing')
        sl.remove(TotalOrdered(1))
#         logging.debug('Removing - DONE')
#         logging.debug('Checking integrity [1]')
        lacks_integrity = sl.lacks_integrity()
#         logging.debug('Checking integrity [1] - DONE')
        assert lacks_integrity == 0, 'lacks_integrity is {}'.lacks_integrity
    logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))
 
def test_one():
    logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(object)
    sl.insert(TotalOrdered(0))
    NUM_INSERTS = 100000
    threads = [
        threading.Thread(
            name='insert_remove[0]',
            target=insert_and_remove_TotalOrdered,
            args=(sl, NUM_INSERTS)),
        threading.Thread(
            name='insert_remove[1]',
            target=insert_and_remove_TotalOrdered,
            args=(sl, NUM_INSERTS)),
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
    test_one()
