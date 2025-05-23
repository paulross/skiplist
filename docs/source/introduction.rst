.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 3

.. _introduction-label:

======================================
Introduction
======================================

A SkipList behaves as a sorted list with, typically, O(log(n)) cost for insertion, look-up and removal.
This makes it ideal for such operations as computing the rolling median of a large dataset.

A SkipList is implemented as a singly linked list of ordered nodes where each node participates in a subset of, sparser,
linked lists.
These additional 'sparse' linked lists provide rapid indexing and mutation of the underlying linked list.
It is a probabilistic data structure using a random function to determine how many 'sparse' linked lists any particular
node participates in.
As such SkipList is an alternative to binary tree, Wikipedia has an introductory page on
`SkipLists <https://en.wikipedia.org/wiki/Skip_list>`_ .

An advantage claimed for SkipLists are that the insert and remove logic is simpler (however I do not subscribe to this).
The drawbacks of a SkipList include its larger space requirements and its O(log(N)) lookup behaviour compared to other,
more restricted and specialised, data structures that may have either faster runtime behaviour or lower space
requirements or both.

This project contains a SkipList implementation in C++, with Python bindings, which has these characteristics:

* No capacity restrictions apart from available memory.
* Works with any C++ type <T> that has meaningful comparison operators.
* The C++ SkipList can be compiled as thread safe.
* The Python SkipList is thread safe.
* The SkipList has exhaustive internal integrity checks.
* Python SkipLists can be long/float/bytes/object types, the latter can have user defined comparison functions.
* With Python 3.8+ SkipLists can be combined with the `multiprocessing.shared_memory <https://docs.python.org/3/library/multiprocessing.shared_memory.html#module-multiprocessing.shared_memory>`_ module for concurrent operation on large arrays.
  For example see :ref:`rolling-median-mp-shared-memory-label` which speeds up a rolling median near linearly with the number of cores.
* This implementation is extensively performance tested in C++ and Python, see :ref:`performance-label`.

There are a some novel features to this implementation:

* A SkipList is a probabilistic data structure but we have deterministic tests that work for any (sane) random number generator, see: :ref:`testing_a_probabilistic_structure`
* This SkipList can dynamically generate visualisations of its current internal state, see: :ref:`skiplist-visualisation-label`

================
Project
================

This project is hosted in a number of places:

* Source (the source of truth): `GitHub <https://github.com/paulross/skiplist/>`_
* Builds on `PyPi <https://pypi.org/project/orderedstructs/>`_
* Documentation: `Read The Docs <https://skiplist.readthedocs.io/en/latest/>`_

============
Credits
============

Originally written by Paul Ross with credits to: Wilfred Hughes (AHL), Luke Sewell (AHL) and Terry Tsantagoeds (AHL).

======================================
Installation
======================================

--------------------------------------
C++
--------------------------------------

This SkipList requires:

* A C++17 compiler.
* ``-I<skiplist>/src/cpp`` as an include path.
* ``<skiplist>/src/cpp/SkipList.cpp`` to be compiled/linked.
* The macro ``SKIPLIST_THREAD_SUPPORT`` set if you want a thread safe SkipList

--------------------------------------
Python
--------------------------------------

This SkipList version supports Python 3.8, 3.9, 3.10, 3.11, 3.12, 3.13 (and, probably, some earlier Python 3 versions)
[older versions, now abandoned, supported Python 2.7].

From PyPi
^^^^^^^^^^^^^^^^^^^^^^

The project is on `PyPi <https://pypi.org/project/orderedstructs/>`_

.. code-block:: sh

    $ pip install orderedstructs

Development
^^^^^^^^^^^^^^^^^^^^^^

Assuming you are in a virtual environment:

.. code-block:: sh

    $ cd <skiplist>
    $ pip install -r requirements.txt
    $ python setup.py develop

If you are on a platform that uses GCC you will need to set the environment variable ``USING_GCC`` before invoking
``setup.py`` (this just fixes some warnings) thus:

.. code-block:: sh

    $ USING_GCC=1 python setup.py develop

======================================
Examples
======================================

Here are some examples of using a SkipList in your code:

--------------------------------------
C++
--------------------------------------

.. code-block:: cpp

    #include "SkipList.h"
        
    // Declare with any type that has sane comparison.
    OrderedStructs::SkipList::HeadNode<double> sl;
    
    sl.insert(42.0);
    sl.insert(21.0);
    sl.insert(84.0);
    
    sl.has(42.0) // true
    sl.size()    // 3
    sl.at(1)     // 42.0, throws OrderedStructs::SkipList::IndexError if index out of range

    sl.remove(21.0); // throws OrderedStructs::SkipList::ValueError if value not present
    
    sl.size()    // 2
    sl.at(1)     // 84.0

The C++ SkipList is thread safe when compiled with the macro ``SKIPLIST_THREAD_SUPPORT``, then a SkipList can then be
shared across threads, such as:

.. code-block:: cpp

    #include <thread>
    #include <vector>
    
    #include "SkipList.h"

    void do_something(OrderedStructs::SkipList::HeadNode<double> *pSkipList) {
        // Insert/remove items into *pSkipList
        // Read items inserted by other threads.
    }

    OrderedStructs::SkipList::HeadNode<double> sl;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < thread_count; ++i) {
        threads.push_back(std::thread(do_something, &sl));
    }
    for (auto &t: threads) {
        t.join();
    }
    // The SkipList now contains the totality of the thread actions.

See ``src/cpp/test/test_concurrent.cpp`` for more examples.

--------------------------------------
Python
--------------------------------------

An example of using a SkipList of always ordered floats:

.. code-block:: python

    import orderedstructs

    # Declare with a type. Supported types are long/float/bytes/object.
    sl = orderedstructs.SkipList(float)

    sl.has(42.0) # True
    sl.size()    # 3
    sl.at(1)     # 42.0, raises IndexError if index out of range

    sl.remove(21.0) # raises ValueError if value not present

    sl.size()    # 2
    sl.at(1)     # 84.0

The Python SkipList can be used with user defined objects with a user defined sort order.
In this example the last name of the person takes precedence over the first name:

.. code-block:: python

    import functools

    import orderedstructs

    @functools.total_ordering
    class Person:
        """Simple example of ordering based on last name/first name."""
        def __init__(self, first_name, last_name):
            self.first_name = first_name
            self.last_name = last_name
    
        def __eq__(self, other):
            try:
                return self.last_name == other.last_name \
                    and self.first_name == other.first_name
            except AttributeError:
                return NotImplemented

        def __lt__(self, other):
            try:
                return self.last_name < other.last_name \
                    or self.first_name < other.first_name
            except AttributeError:
                return NotImplemented
    
        def __str__(self):
            return '{}, {}'.format(self.last_name, self.first_name)

    sl = orderedstructs.SkipList(object)

    sl.insert(Person('Peter', 'Pan'))
    sl.insert(Person('Alan', 'Pan'))
    assert sl.size() == 2
    assert str(sl.at(0)) == 'Pan, Alan' 
    assert str(sl.at(1)) == 'Pan, Peter' 

======================================
Testing
======================================

This SkipList has extensive tests for correctness and performance.

--------------------------------------
C++
--------------------------------------

Using the Makefile
^^^^^^^^^^^^^^^^^^^^

To run all the C++ functional and performance tests:

.. code-block:: sh

    $ cd <skiplist>/src/cpp
    $ make release
    $ ./SkipList_R.exe

To run the C++ functional tests with agressive internal integrity checks but excluding the performance checks:

.. code-block:: sh

    $ cd <skiplist>/src/cpp
    $ make debug
    $ ./SkipList_D.exe

To run all the C++ functional and performance tests for a thread safe SkipList:

.. code-block:: sh

    $ cd <skiplist>/src/cpp
    $ make release CXXFLAGS=-DSKIPLIST_THREAD_SUPPORT
    $ ./SkipList_R.exe

Using CMake
^^^^^^^^^^^^^^^^^^^^

To build the binary:

.. code-block:: sh

    $ cd <skiplist>
    $ cmake --build cmake-build-release --target SkipList -- -j 6

And to run the tests:

.. code-block:: sh

    $ cd <skiplist>
    $ ./cmake-build-release/SkipList

This takes 60s or so.

--------------------------------------
Python
--------------------------------------

Testing requires ``pytest`` and ``hypothesis``:

To run basic tests:

.. code-block:: sh

    $ cd <skiplist>
    $ pytest tests/

This takes typically 30s.

To run more extensive (slow) tests and generate benchmarks:

.. code-block:: sh

    $ cd <skiplist>
    $ pytest tests/ --runslow --benchmark-sort=name \
      --benchmark-autosave --benchmark-histogram

--------------------------------------
Both C++ and Python
--------------------------------------

Running all tests (C++ and Python) and building all currently supported Python versions is done with a build script:

.. code-block:: sh

    $ cd <skiplist>
    $ ./build_all.sh

This takes 5 to 10 minutes per Python version.

.. note:: Naming conventions

    The project on PyPi is called ``orderedstructs`` as it was originally intended as a repository for multiple ordered
    structures.
    Because of changing requirements the sole ordered structure in this project currently is a Skip List.
    Thus ``SkipList`` and ``orderedstructs`` are used (sort of) interchangeably.
