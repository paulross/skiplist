"""Some specific PyObject* tests"""
import logging
import threading
import time

import pytest

import orderedstructs

from .SkipList_common import TotalOrdered, OrderedLt

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )
def test_one():
    sl = orderedstructs.SkipList(object)
    obj_insert = TotalOrdered(0)
    sl.insert(obj_insert)
    obj_remove = sl.remove(obj_insert)
    assert id(obj_remove) == id(obj_insert)

# def worker():
#     logging.debug('Starting')
#     time.sleep(2)
#     logging.debug('Exiting')
# 
# def my_service():
#     logging.debug('Starting')
#     time.sleep(3)
#     logging.debug('Exiting')
# 
# t = threading.Thread(name='my_service', target=my_service)
# w = threading.Thread(name='worker', target=worker)
# w2 = threading.Thread(target=worker) # use default name
# 
# w.start()
# w2.start()
# t.start()
