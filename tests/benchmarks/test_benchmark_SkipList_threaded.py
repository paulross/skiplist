"""Some specific PyObject* tests"""
import logging
import sys
import threading

import pytest

import orderedstructs

logging.basicConfig(level=logging.DEBUG,
                    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
                    )

logger = logging.getLogger(__file__)
logger.setLevel(10)


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


def insert_has_remove_float_minimal(sl, value, count):
    # logger.info('Starting: {}()'.format(sys._getframe().f_code.co_name))
    # print('Starting: {}() {}'.format(sys._getframe().f_code.co_name, threading.current_thread()))
    # t_s = 10 / (1000 * 1000)
    for _i in range(count):
        sl.insert(value)
        sl.has(value)
        sl.remove(value)
        # Sleep time is, on average: count * 0.5 / (1000 * 1000) so count * 0.5 us
        # time.sleep(random.random() / (1000 * 1000))
        # Sleep time is, on average: count * 10 us
        # Example: 10ms
        # time.sleep(t_s)
    # print(
    #     '  Ending: {}() {} Sleep: {}'.format(
    #         sys._getframe().f_code.co_name, threading.current_thread(), t_s * count
    #     )
    # )
    # logger.info('  Ending: {}()'.format(sys._getframe().f_code.co_name))


def benchmark_insert_has_remove_float_threaded(skip_list, insert_value, num_inserts, num_threads):
    if num_threads > 0:
        threads = [
            threading.Thread(
                name='insert_has_remove[{:3d}]'.format(i),
                target=insert_has_remove_float_minimal,
                args=(skip_list, insert_value, num_inserts)
            ) for i in range(num_threads)
        ]
        for thread in threads:
            thread.start()
        # logging.debug('Waiting for worker threads')
        main_thread = threading.current_thread()
        for t in threading.enumerate():
            if t is not main_thread:
                t.join()
    else:
        insert_has_remove_float_minimal(skip_list, insert_value, num_inserts)


@pytest.mark.slow
@pytest.mark.parametrize(
    'num_threads',
    (
            0,
            1,
            2,
            4,
            8,
            16,
            32,
            64,
            128,
    ),
    # ids=[
    #     'Len_1e6_Threads_000',
    #     'Len_1e6_Threads_001',
    #     'Len_1e6_Threads_002',
    #     'Len_1e6_Threads_004',
    #     'Len_1e6_Threads_008',
    #     'Len_1e6_Threads_016',
    #     'Len_1e6_Threads_032',
    #     'Len_1e6_Threads_064',
    #     'Len_1e6_Threads_128',
    # ]
)
def test_ihr_float_threaded(benchmark, num_threads):
    """Tests multithreaded insert()/has()/remove() of floats converted to
    native C++ objects."""
    # logging.debug('Starting: {}()'.format(sys._getframe().f_code.co_name))
    sl = orderedstructs.SkipList(float)
    length = 1000 * 1000
    # Load SkipList with length values
    for i in range(length):
        sl.insert(float(i))
    insert_value = float(length / 2.0)
    # Scale the number of repeat inserts to the number of threads.
    if num_threads == 0:
        num_inserts = 1024 * 8
    else:
        num_inserts = 1024 * 8 // num_threads
    benchmark(benchmark_insert_has_remove_float_threaded, sl, insert_value, num_inserts, num_threads)
    # logging.debug('Ending: {}()'.format(sys._getframe().f_code.co_name))
