.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 2

.. _skiplist-rolling-median-label:

===============================================
Computing a Rolling Median
===============================================

A powerful use case for a skip list is in the computation of a rolling fraction, for example a rolling median.

Here is a reasonable C++ attempt at doing that with the arguments:

* ``data`` - A vector of data of type ``T``.
* ``win_length`` - a 'window' size. The median is computed over this number of values.
* ``result`` - a destination vector for the result. This will end up with ``count - win_length`` values.

Rolling median code using a skip list might look like this, error checking is omitted:

.. code-block:: cpp

    #include "SkipList.h"
    
    template <typename T>
    void rolling_median(const std::vector<T> data,
                        size_t win_length,
                        std::vector<T> &result) {
        
        OrderedStructs::SkipList::HeadNode<T> sl;

        result.clear();
        for (size_t i = 0; i < data.size(); ++i) {
            sl.insert(data[i]);
            if (i  >= win_length) {
                result.push_back(sl.at(win_length / 2));
                sl.remove(data[i - win_length]);
            }
        }
    }

If you are working with C arrays (such as Numpy arrays) then this C'ish approach might be better, again error checking omitted:

.. code-block:: c

    #include "SkipList.h"
    
    template <typename T>
    void rolling_median(const T *src, size_t count, size_t win_length, T *dest) {
        
        OrderedStructs::SkipList::HeadNode<T> sl;
        const T *tail = src;

        for (size_t i = 0; i < count; ++i) {
            sl.insert(*src);
            if (i + 1 >= win_length) {
                *dest = sl.at(win_length / 2);
                ++dest;
                sl.remove(*tail);
                ++tail;
            }
            ++src;
        }
    }

Multidimensional Numpy arrays have a stride value which is omitted in the above code but is simple to add. See *RollingMedian.h* and *test/test_rolling_median.cpp* for further examples.

Rolling percentiles require a argument that says what fraction of the window the required value lies. Again, this is easy to add.

---------------------------------
Even Window Length
---------------------------------

The above code assumes that if the window length is even that the median is at ``(window length - 1) / 2``. A more plausible median for even sized window lengths is the mean of ``(window length - 1) / 2`` and ``window length / 2``. This requires that the mean of two types is meaningful which it will not be for strings.