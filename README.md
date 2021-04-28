# SkipList

This project contains a SkipList implementation in C++ with Python bindings.

A SkipList behaves as an always sorted list with, typically, O(log(n)) cost for insertion, look-up and removal.
This makes it ideal for such operations as computing the rolling median of a large dataset.

See the full documentation on this project at [ReadTheDocs](http://skiplist.readthedocs.io/en/latest/index.html).

A SkipList is implemented as a singly linked list of ordered nodes where each node participates in a subset of, sparser, linked lists.
These additional 'sparse' linked lists provide rapid indexing and mutation of the underlying linked list.
It is a probabilistic data structure using a random function to determine how many 'sparse' linked lists any particular node participates in.
As such SkipList is an alternative to binary tree, Wikipedia has a introductory page on [SkipLists](<https://en.wikipedia.org/wiki/Skip_list>).

An advantage claimed for SkipLists are that the insert and remove logic is simpler (however I do not subscribe to this).
The drawbacks of a SkipList include its larger space requirements and its O(log(N)) lookup behaviour compared to other, more restricted and specialised, data structures that may have either faster runtime behaviour or lower space requirements or both.

This project contains a SkipList implementation in C++ with Python bindings with:

* No capacity restrictions apart from available memory.
* Works with any C++ type <T> that has meaningful comparison operators.
* The C++ SkipList can be compiled as thread safe.
* The Python SkipList is thread safe.
* The SkipList has exhaustive internal integrity checks.
* Python SkipLists can be long/float/bytes/object types, the latter can have user defined comparison functions.
* With Python 3.8+ SkipLists can be combined with the [multiprocessing.shared_memory](https://docs.python.org/3/library/multiprocessing.shared_memory.html#module-multiprocessing.shared_memory) module for concurrent operation on large arrays.
  For example [concurrent rolling medians](https://skiplist.readthedocs.io/en/latest/rolling_median.html#rolling-median-in-python-with-multiprocessing-shared-memory) which speed up near linearly with the number of cores.
* The implementation is extensively performance tested in C++ and Python.


There are a some novel features to this implementation:

* A SkipList is a probabilistic data structure but we have deterministic tests that work for any (sane) random number generator. See [Testing a Probabilistic Structure](http://skiplist.readthedocs.io/en/latest/test_notes.html#testing-a-probabilistic-structure)
* This SkipList can dynamically generate visualisations of its current internal state. See [Visualising a Skip List](http://skiplist.readthedocs.io/en/latest/visualisations.html#skiplist-visualisation-label)

# Credits

Originally written by Paul Ross with credits to: Wilfred Hughes (AHL), Luke Sewell (AHL) and Terry Tsantagoeds (AHL).


# Installation

## C++

This SkipList requires:

* A C++11 compiler.
* ``-I<skiplist>/src/cpp`` as an include path.
* ``<skiplist>/src/cpp/SkipList.cpp`` to be compiled/linked.
* The macro ``SKIPLIST_THREAD_SUPPORT`` set if you want a thread safe SkipList using C++ mutexes.

## Python

This SkipList version supports Python 3.6, 3.7, 3.8, 3.9 (and, probably, some earlier Python 3 versions).
Earlier versions supported Python 2.7, this version might still do that.

### From PyPi

    $ pip install orderedstructs

### From source

    $ git clone https://github.com/paulross/skiplist.git
    $ cd <skiplist>
    $ python setup.py install


# Testing


This SkipList has extensive tests for correctness and performance.

## C++

To run all the C++ functional and performance tests:

    $ cd <skiplist>/src/cpp
    $ make release
    $ ./SkipList_R.exe

To run the C++ functional tests with agressive internal integrity checks:

    $ cd <skiplist>/src/cpp
    $ make debug
    $ ./SkipList_D.exe

To run all the C++ functional and performance tests for a thread safe SkipList:

    $ cd <skiplist>/src/cpp
    $ make release CXXFLAGS=-DSKIPLIST_THREAD_SUPPORT
    $ ./SkipList_R.exe


## Python

Testing requires ``pytest`` and ``hypothesis``:

To run all the C++ functional and performance tests:

    $ cd <skiplist>
    $ py.test -vs tests/


# Examples

Here are some examples of using a SkipList in your code:

## C++


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

The C++ SkipList is thread safe when compiled with the macro ``SKIPLIST_THREAD_SUPPORT``, then a SkipList can then be shared across threads:

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


## Python

An example of using a SkipList of always ordered floats:

    import orderedstructs
    
    # Declare with a type. Supported types are long/float/bytes/object.
    sl = orderedstructs.SkipList(float)
    
    sl.insert(42.0)
    sl.insert(21.0)
    sl.insert(84.0)
    
    sl.has(42.0) # True
    sl.size()    # 3
    sl.at(1)     # 42.0

    sl.has(42.0) # True
    sl.size()    # 3
    sl.at(1)     # 42.0, raises IndexError if index out of range

    sl.remove(21.0); # raises ValueError if value not present
    
    sl.size()    # 2
    sl.at(1)     # 84.0

The Python SkipList can be used with user defined objects with a user defined sort order.
In this example the last name of the person takes precedence over the first name:

    import functools
    
    @functools.total_ordering
    class Person:
        """Simple example of ordering based on last name/first name."""
        def __init__(self, first_name, last_name):
            self.first_name = first_name
            self.last_name = last_name
    
        def __eq__(self, other):
            try:
                return self.last_name == other.last_name and self.first_name == other.first_name
            except AttributeError:
                return NotImplemented

        def __lt__(self, other):
            try:
                return self.last_name < other.last_name or self.first_name < other.first_name
            except AttributeError:
                return NotImplemented
    
        def __str__(self):
            return '{}, {}'.format(self.last_name, self.first_name)

    import orderedstructs
    
    sl = orderedstructs.SkipList(object)

    sl.insert(Person('Peter', 'Pan'))
    sl.insert(Person('Alan', 'Pan'))
    assert sl.size() == 2
    assert str(sl.at(0)) == 'Pan, Alan' 
    assert str(sl.at(1)) == 'Pan, Peter' 


The Python SkipList is thread safe when using any acceptable Python type even if that type has user defined comparison methods.
This uses Pythons mutex machinery which is independent of C++ mutexes.
