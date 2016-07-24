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

Here is a reasonable attempt at doing that with the arguments:

* ``src`` - a pointer to an array of data of type ``T``.
* ``count`` - a count of the number of elements in that array.
* ``win_length`` - a 'window' size. The median is computed over this number of values.
* ``dest`` - a destination array for the result. This is assumed to be able to hold ``count - win_length`` values.

.. code-block:: c

    template <typename T>
    void rolling_median(const T *src, size_t count, size_t win_length, T *dest) {
        SkipList::HeadNode<T> sl;
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



---------------------------------
When the Window Length is Even
---------------------------------

