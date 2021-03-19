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

Rolling Median in C++
-----------------------------------------

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


Even Window Length
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The above code assumes that if the window length is even that the median is at ``(window length - 1) / 2``. A more plausible median for even sized window lengths is the mean of ``(window length - 1) / 2`` and ``window length / 2``. This requires that the mean of two types is meaningful which it will not be for strings.


Rolling Median in Python
----------------------------------------

Here is an example of computing a rolling median of a ``numpy`` 1D array:

.. code-block:: python

    import numpy as np

    import orderedstructs

    def simple_python_rolling_median(vector: np.ndarray,
                                     window_length: int) -> np.ndarray:
        """Computes a rolling median of a numpy vector returning a new numpy
        vector of the same length.
        NaNs in the input are not handled but a ValueError will be raised."""
        if vector.ndim != 1:
            raise ValueError(
                f'vector must be one dimensional not shape {vector.shape}'
            )
        skip_list = orderedstructs.SkipList(float)
        ret = np.empty_like(vector)
        for i in range(len(vector)):
            value = vector[i]
            skip_list.insert(value)
            if i >= window_length - 1:
                # // 4 for lower quartile
                # * 3 // 4 for upper quartile etc.
                median = skip_list.at(window_length // 2)
                skip_list.remove(vector[i - window_length + 1])
            else:
                median = np.nan
            ret[i] = median
        return ret

This can be called thus:


.. code-block:: python

    np_array = np.arange(10.0)
    print('Original:', np_array)
    result = simple_python_rolling_median(np_array, 3)
    print('  Result:', result)

And the result will be:

.. code-block:: text

    Original: [0. 1. 2. 3. 4. 5. 6. 7. 8. 9.]
      Result: [nan nan  1.  2.  3.  4.  5.  6.  7.  8.]


.. _rolling-median-mp-shared-memory-label:

Rolling Median in Python with ``multiprocessing.shared_memory``
----------------------------------------------------------------

An exiting development in Python 3.8+ is `multiprocessing.shared_memory <https://docs.python.org/3/library/multiprocessing.shared_memory.html#module-multiprocessing.shared_memory>`_
This allows a parent process to share memory with its child processes.

In this example we are going to compute a rolling median on a 2D numpy array where each child process works on a single column of the same array and writes the result to a shared output array.
There will be two shared memory areas; a read one with the input data and a write one with the result from all the child processes
There will be two corresponding numpy arrays the input that we are given and the output numpy array that we create.

The only copying going on here is the initial copy of the input array into shared memory and then the final copy, when all child processes have completed of *that* shared memory to a single numpy array.

Pictorially:

.. code-block:: text

    Parent                                        Children
    ======                                        ========
    Copies the array to the input SharedMemory
    Creates the output SharedMemory
    Launches n child processes...
    \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
                                                  Work on part of the input SharedMemory
                                                  Write to the output SharedMemory
                                                  ...
    /<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<</
    When all child processes complete...
    Copies output SharedMemory to a new numpy array
    Releases both SharedMemory resources.


First let's write some code that wraps the low level ``multiprocessing.shared_memory.SharedMemory`` class that can be used by the parent process.
This is a dataclass that records essential information about the array and includes the ``SharedMemory`` object itself.
We will call it an ``ArraySpecification``, it is pretty simple, just a named tuple:

.. code-block:: python

    import multiprocessing
    import typing
    from multiprocessing import shared_memory

    import numpy as np

    import orderedstructs

    # NOTE: This code used by the parent process.

    class ArraySpecification(typing.NamedTuple):
        """Class that represents a numpy array and shared memory."""
        shape: typing.Tuple[int, ...]
        dtype: np.dtype
        nbytes: int
        shm: shared_memory.SharedMemory

        @property
        def name(self) -> str:
            """The name (identifier) of the shared memory block."""
            return self.shm.name

        def __str__(self):
            return (
                f'<ArraySpecification
                f' shape {self.shape} dtype {self.dtype} name "{self.name}"
                f' nbytes {self.nbytes}'
                f' buffer id 0x{id(self.shm)}>'
            )

        def close_and_unlink(self) -> None:
            """Free up the shared memory object when done."""
            self.shm.close()
            self.shm.unlink()

Now create a function that takes a numpy array, creates the ``SharedMemory`` and returns an ``ArraySpecification``:

.. code-block:: python

    # NOTE: This code used by the parent process.

    def shared_memory_and_array_spec(arr: np.ndarray) -> ArraySpecification:
        """Given a numpy array create a SharedMemory object and encapsulate it in an
        ArraySpecification."""
        shm = shared_memory.SharedMemory(create=True, size=arr.nbytes)
        array_spec = ArraySpecification(arr.shape, arr.dtype, arr.nbytes, shm)
        return array_spec


Now a function that, given an (input) array and an array specification copies the array data into the shared memory:

.. code-block:: python

    # NOTE: This code used by the parent process.

    def copy_array_to_shared_memory(array: np.ndarray,
                                    array_spec: ArraySpecification) -> None:
        """Copy an array to the shared memory of an ArraySpecification."""
        array_view = np.ndarray(
            array_spec.shape, dtype=array_spec.dtype, buffer=array_spec.shm.buf
        )
        array_view[:] = array[:]

This is used as in this code fragment given ``read_array``:

.. code-block:: python

    # NOTE: This code used by the parent process.

    read_array_spec = shared_memory_and_array_spec(read_array)
    copy_array_to_shared_memory(read_array, read_array_spec)

We also need a separate shared memory area for the output array:

.. code-block:: python

    # NOTE: This code used by the parent process.

    write_array_spec = shared_memory_and_array_spec(read_array)

Now we create a function that computes a rolling median that the child processes use.
It takes the two ``ArraySpecification``'s from the Parent process and some details such as rolling median window length and the array column index.
This function is going to invoked as a child process:


.. code-block:: python

    # NOTE: This code used by the child processes.

    def compute_rolling_median_2d_from_index(read_array_spec: ArraySpecification,
                                             window_length: int,
                                             column_index: int,
                                             write_array_spec: ArraySpecification) -> None:
        """Computes a rolling median of the 2D read array and window length and writes it
        to the 2D write array."""
        # First recall the shared memory
        read_array_shm = shared_memory.SharedMemory(name=read_array_spec.name)
        # Create a numpy view on that memory
        read_array = np.ndarray(
            read_array_spec.shape, read_array_spec.dtype, buffer=read_array_shm.buf
        )
        assert read_array.ndim == 2, 'Sanity check'
        # And the same for the write array.
        write_array_shm = shared_memory.SharedMemory(name=write_array_spec.name)
        write_array = np.ndarray(
            write_array_spec.shape, write_array_spec.dtype, buffer=write_array_shm.buf
        )
        assert write_array.ndim == 2, 'Sanity check'
        assert read_array.shape == write_array.shape, 'Sanity check'

        # Now let's get into the rolling median
        skip_list = orderedstructs.SkipList(float)
        for i in range(len(read_array)):
            value = read_array[i, column_index]
            skip_list.insert(value)
            if i >= window_length - 1:
                median = skip_list.at(window_length // 2)
                skip_list.remove(vector[i - window_length + 1])
            else:
                median = np.nan
            write_array[i, column_index] = median
        # Done, so finish with our view on the shared memory.
        read_array_shm.close()
        write_array_shm.close()

Before we put this all together we need to write something that will copy the *output* shared memory to a numpy array that we can use:

.. code-block:: python

    # NOTE: This code used by the parent process.

    def copy_shared_memory_to_new_array(array_spec: ArraySpecification) -> np.ndarray:
        temp_write = np.ndarray(
            array_spec.shape, dtype=array_spec.dtype, buffer=array_spec.shm.buf
        )
        new_array = np.empty(array_spec.shape, dtype=array_spec.dtype)
        new_array[:] = temp_write[:]
        return new_array

This would be called thus from the ``write_array_spec`` we created above when needed like this:

.. code-block:: python

    # NOTE: This code used by the parent process.

    write_array = copy_shared_memory_to_new_array(write_array_spec)

Now use all of this in the parent process:

.. code-block:: python

    # NOTE: This code used by the parent process.

    def compute_rolling_median_2d_mp(read_array: np.ndarray,
                                     window_length: int,
                                     num_processes: int) -> np.ndarray:
        """Compute a rolling median on a 2D array using multiprocessing."""
        # Create the read shared memory.
        read_array_spec = shared_memory_and_array_spec(read_array)
        # Copy the input numpy array into the read shared memory.
        copy_array_to_shared_memory(read_array, read_array_spec)
        # Create the write shared memory with an identical size to the read shared memory.
        write_array_spec = shared_memory_and_array_spec(read_array)
        # Create the multiprocessing pool, limit to number of columns if necessary.
        mp_pool = multiprocessing.Pool(processes=max(num_processes, read_array.shape[1]))
        # Create the tasks, one per column.
        # An alternative design would give a range of columns to each sub-process.
        tasks = [
            (read_array_spec, window_length, i, write_array_spec)
                for i in range(read_array.shape[1])
        ]
        # apply_async() with compute_rolling_median_2d_from_index
        pool_apply = [
            mp_pool.apply_async(compute_rolling_median_2d_from_index, t) for t in tasks
        ]
        # Run all the sub-processes
        results = [r.get() for r in pool_apply]
        # Now take the write shared memory and copy it into a numpy array.
        write_array = copy_shared_memory_to_new_array(write_array_spec)
        # Delete the read/write shared memory.
        read_array_spec.close_and_unlink()
        write_array_spec.close_and_unlink()
        # Return the result.
        return write_array

Now run it on an example array:

.. code-block:: python

    # NOTE: This code used by the parent process.

    # Create an array with a million rows and 16 columns.
    read_array = np.random.random((1024 * 1024, 16))
    print(f'Array shape {read_array.shape} size {read_array_size:,d}')
    # From 1 to 16 child processes.
    for p in range(1, 17):
        t_start = time.perf_counter()
        write_array = compute_rolling_median_2d_mp(
            read_array, window_length=21, num_processes=p
        )
        t_elapsed = time.perf_counter() - t_start
        print(
            f'compute_rolling_median():'
            f' Processes: {p:2d}'
            f' Time: {t_elapsed:8.3f} (s)'
            f' rate {read_array.size / t_elapsed:12,.0f} values/s'
        )


And the output will be something like this on a MacBook Pro, 2.7 GHz Intel Core i7 4 cores with hyper-threading:

.. code-block:: text

    Array shape (1048576, 16) size 16,777,216
    compute_rolling_median(): Processes:  1 Time:   21.731 (s) rate      772,044 values/s
    compute_rolling_median(): Processes:  2 Time:   11.986 (s) rate    1,399,776 values/s
    compute_rolling_median(): Processes:  3 Time:    9.511 (s) rate    1,763,997 values/s
    compute_rolling_median(): Processes:  4 Time:    7.612 (s) rate    2,204,077 values/s
    compute_rolling_median(): Processes:  5 Time:    8.187 (s) rate    2,049,138 values/s
    compute_rolling_median(): Processes:  6 Time:    7.109 (s) rate    2,359,920 values/s
    compute_rolling_median(): Processes:  7 Time:    7.863 (s) rate    2,133,823 values/s
    compute_rolling_median(): Processes:  8 Time:    7.050 (s) rate    2,379,835 values/s
    compute_rolling_median(): Processes:  9 Time:    6.944 (s) rate    2,415,980 values/s
    compute_rolling_median(): Processes: 10 Time:    7.142 (s) rate    2,349,237 values/s
    compute_rolling_median(): Processes: 11 Time:    7.760 (s) rate    2,161,943 values/s
    compute_rolling_median(): Processes: 12 Time:    7.153 (s) rate    2,345,611 values/s
    compute_rolling_median(): Processes: 13 Time:    7.521 (s) rate    2,230,636 values/s
    compute_rolling_median(): Processes: 14 Time:    7.766 (s) rate    2,160,363 values/s
    compute_rolling_median(): Processes: 15 Time:    8.655 (s) rate    1,938,537 values/s
    compute_rolling_median(): Processes: 16 Time:    7.176 (s) rate    2,337,818 values/s

This shows that speedups of around 3x are possible with 4 cores.

.. note::

    This is simplified code.
    It be written more effectively with context managers that could automatically close and unlink the shared memory.
