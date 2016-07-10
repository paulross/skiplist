.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 3

.. _performance-label:

**************************************
Skip List Performance
**************************************

.. toctree::
   :maxdepth: 2

====================================
Running the C++ Performance Tests
====================================

The time perfromance tests are run as follows:

.. code-block:: sh

    $ cd src/cpp
    $ make release
    $ ./SkipList_R.exe
                     test_very_simple_insert(): PASS
                                                        ...
                     test_roll_med_even_mean(): PASS
                   perf_single_insert_remove(): 451.554 (ms) rate x.xe+06 /s
                                                        ...
                perf_roll_med_odd_index_wins(): vectors length:  1000000 window width: 524288 time: x.x (ms)
                                perf_size_of(): size_of(       1):      216 bytes ratio:      216 /sizeof(T):       x
                                                        ...
    Final result: PASS
    Exec time: x.x (s)
    Bye, bye!


====================================
Time Performance
====================================

The performance test mostly work on a skip list of type ``double`` that has 1 million values. Test on a couple of modern 64 bit OS's [Linux, Mac OS X] show that the cost of skip list operations is typically as follows.

-------------------------------------------------
Mutating operations: ``insert()``, ``remove()``
-------------------------------------------------

These operations on a skip list containing 1 million doubles is typically 450 ns (2.2 million operations per second).

-------------------------------------------------
Indexing operations: ``at()``, ``has()``
-------------------------------------------------

These operations on a skip list containing 1 million doubles is typically 220 ns (4.6 million operations per second).


-------------------------------------------------
Detailed Performance
-------------------------------------------------

The performance test function names all start with ``perf_...`` and are as follows. The skip list type is ``<double>``. In the table below 1M means mega, i.e. 2**20 or 1024*1024 or 1048576:

.. =================================== ================================================================
.. Test Name                           Description
.. =================================== ================================================================
.. ``perf_single_insert_remove()``     With an empty skip list add one item and remove it.
.. ``perf_large_skiplist_ins_only()``  Starting with an empty skiplist append 1 million values.
.. ``perf_large_skiplist_ins_rem()``   Starting with an empty skiplist append 1 million values then
..                                     remove the first (lowest) value until the skip list is empty.
.. ``perf_single_ins_rem_middle()``    With a skiplist of 1 million values insert the
..                                     middle value (i.e. 500,000.0) and remove it. 
.. ``perf_single_at_middle()``         With a skiplist of 1 million values find the middle value.
.. ``perf_single_has_middle()``        With a skiplist of 1 million values test for the middle value.
.. ``perf_single_ins_at_rem_middle()`` With a skiplist of 1 million values call ``insert(v)``,
..                                     ``at(500000)`` and ``remove(v)`` where ``v`` corresponds to
..                                     the middle value. This simulates the actions of a rolling
..                                     median.
.. ``perf_median_sliding_window()``    Simulate a rolling median of 100 values. Create an
..                                     initially empty skip list. For each of 10,000 random values
..                                     insert the value into the skip list. For indecies > 100 extract
..                                     the middle value from the skip list as the median then remove
..                                     the i-100 value from the skip list.
.. ``perf_1m_median_values()``         Simulate a rolling median of 101 values. Similar to
..                                     ``perf_median_sliding_window()`` but uses 1 million values.
.. ``perf_1m_medians_1000_vectors()``  Simulate a rolling median of 101 values. Similar to
..                                     ``perf_1m_median_values()`` but uses 1000 values repeated
..                                     1000 times.
.. ``perf_simulate_real_use()``        Simulate a rolling median of 200 values. Similar to
..                                     ``perf_1m_medians_1000_vectors()`` but uses 8000 values repeated
..                                     8000 times i.e. the rolling median of 8000x8000 array.
.. ``perf_at_in_one_million()``        For 1M values call ``at(i)`` where i ranges from 2**1 to 2**19.
..                                     This explores the time complexity of ``at()``.
.. ``perf_has_in_one_million()``       For 1M values call ``has(i)`` where i ranges from 2**1 to 2**19.
..                                     This explores the time complexity of ``has()``.
.. ``perf_roll_med_odd_index()``       Tests the time cost of ``ManAHL::RollingMedian::odd_index`` for
..                                     1 million values and a window size of 101.
.. ``perf_roll_med_odd_index_wins()``  As ``perf_roll_med_odd_index()`` but explores various window
..                                     sizes from 1 to 524288.
.. ``perf_size_of()``                  Explores the space usage of a skip list of various lengths from
..                                     0 to 1M.
.. =================================== ================================================================

=================================== =========================================== =========== ========
Test Name                           Measure                                     Time/value  Rate
=================================== =========================================== =========== ========
``perf_single_insert_remove()``     With an empty skip list                     240 ns      4.1 M/s
                                    add one item and remove it.
``perf_large_skiplist_ins_only()``  Starting with an empty skiplist append 1    740 ns      1.3 M/s
                                    million values.
``perf_large_skiplist_ins_rem()``   Starting with an empty skiplist append 1    900 ns      1.1 M/s
                                    million values then remove the first
                                    (lowest) value until the skip list is
                                    empty.
``perf_single_ins_rem_middle()``    With a skiplist of 1 million values insert  1200 ns     0.85 M/s
                                    the middle value (i.e. 500,000.0) and
                                    remove it. 
``perf_single_at_middle()``         With a skiplist of 1 million values find    220 ns      4.6 M/s
                                    the middle value.
``perf_single_has_middle()``        With a skiplist of 1 million values test    210 ns      4.8 M/s
                                    for the middle value.
``perf_single_ins_at_rem_middle()`` With a skiplist of 1 million values call    1400 ns     0.7 M/s
                                    ``insert(v)``, ``at(500000)`` and
                                    ``remove(v)`` where ``v`` corresponds to
                                    the middle value. This simulates the
                                    actions of a rolling median.
``perf_median_sliding_window()``    Simulate a rolling median of 100 values.    800 ns      1.3 M/s
                                    Create an initially empty skip list. For
                                    each of 10,000 random values
                                    insert the value into the skip list. For
                                    indicies > 100 extract the middle value
                                    from the skip list as the median then
                                    remove the i-100 value from the skip list.
``perf_1m_median_values()``         Simulate a rolling median of 101 values.    720 ns      1.4 M/s
                                    Similar to
                                    ``perf_median_sliding_window()`` but uses
                                    1 million values.
``perf_1m_medians_1000_vectors()``  Simulate a rolling median of 101 values.    690 ns      1.4 M/s
                                    Similar to ``perf_1m_median_values()``
                                    but uses 1000 values repeated 1000 times.
``perf_simulate_real_use()``        Simulate a rolling median of 200 values.    760 ns      1.3 M/s
                                    Similar to
                                    ``perf_1m_medians_1000_vectors()`` but
                                    uses 8000 values repeated 8000 times
                                    i.e. the rolling median of 8000x8000
                                    array.
``perf_roll_med_odd_index()``       Tests the time cost of                      830 ns      1.2 M/s
                                    ``ManAHL::RollingMedian::odd_index``
                                    for 1 million values and a window size of
                                    101.
=================================== =========================================== =========== ========

---------------------------------
Time Complexity
---------------------------------

There are a number of tests that check the execution time of operations with varying sizes of skip lists. The expectation is that the time complexity is O(log(n)).


=================================== ================================================================
Test Name                           Description
=================================== ================================================================
``perf_at_in_one_million()``        For 1M values call ``at(i)`` where i ranges from 2**1 to 2**19.
                                    This explores the time complexity of ``at()``.
``perf_has_in_one_million()``       For 1M values call ``has(i)`` where i ranges from 2**1 to 2**19.
                                    This explores the time complexity of ``has()``.
``perf_roll_med_odd_index_wins()``  As ``perf_roll_med_odd_index()`` but explores various window
                                    sizes from 1 to 524288.
=================================== ================================================================


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``perf_at_in_one_million()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``perf_has_in_one_million()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``perf_roll_med_odd_index_wins()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^





====================================
Space Performance
====================================

=================================== ================================================================
Test Name                           Description
=================================== ================================================================
``perf_size_of()``                  Explores the space usage of a skip list of various lengths from
                                    0 to 1M.
=================================== ================================================================


====================================
Effect of a Biased Coin
====================================



p()	perf_single_at_middle()	perf_single_has_middle()	perf_single_ins_at_rem_middle()	perf_size_of()
0.0625	481.0	729.0	2917.7	7.14
0.125	490.9	669.0	2851.8	7.31
0.25	238.5	338.1	1591.0	7.78
0.5	94.8	179.5	1182.0	9.56
0.75	114.7	301.8	1617.7	15.21
0.875	197.1	653.4	2748.4	26.76

index = [0.0625, 0.125, 0.25, 0.5, 0.75, 0.875,]
data = {
'at()' : [481.0, 490.9, 238.5, 94.8, 114.7, 197.1],
'has()' : [729.0, 669.0, 338.1, 179.5, 301.8, 653.4],
'insert(), at(), remove()' : [2917.7, 2851.8, 1591.0, 1182.0, 1617.7, 2748.4],
'size_of()' : [7.14, 7.31, 7.78, 9.56, 15.21, 26.76],
}
pdf = pd.DataFrame(data, index)
