.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 3

***********************************************
Skip List API
***********************************************

.. _skiplist-cpp-api-label:

===============================================
C++ API
===============================================

-------------------------
Example in C++
-------------------------

.. code-block:: cpp

    #include "SkipList.h"
    
    using namespace ManAHL::SkipList;
    
    HeadNode<double> sl;
    
    sl.insert(42.0);
    sl.insert(21.0);
    sl.insert(84.0);
    
    sl.has(42.0) // true
    sl.size()    // 3
    sl.at(1)     // 42.0, throws IndexError if index out of range

    sl.remove(21.0); // throws ValueError if value not present
    
    sl.size()    // 2
    sl.at(1)     // 84.0

--------------------
Constructors
--------------------

There is only one constructor to a ``HeadNode`` and that takes no arguments.

----------------------
``has(T &val) const;``
----------------------

Declaration: ``bool has(const T &value) const;``

This returns ``true`` or ``false`` if the skip list has the value val. This is O(log(n)) for well formed skip lists.

----------------------------
``at(size_t index) const;``
----------------------------

Declaration: ``const T &at(size_t index) const;``

This returns the value of type ``T`` at the given index. This will throw an ``IndexError`` if the index is >= size of the skip list. This is O(log(n)) for well formed skip lists.

----------------------------------------------------------------
``at(size_t index, size_t count, std::vector<T> &dest) const;``
----------------------------------------------------------------

Declaration: ``void at(size_t index, size_t count, std::vector<T> &dest) const;``

This loads a vector of type ``T`` with ``count`` values starting at the given index. This will throw an ``IndexError`` if the index + count is >= size of the skip list. This is O(count * log(n)) for well formed skip lists.

------------------
``size() const``
------------------

Declaration: ``size_t size() const;``

Returns the number of items in the skip list.

------------------
``insert(T &val)``
------------------

Declaration: ``void insert(const T &value);``

Inserts a copy of ``value`` such that the previous value, if present, is <= ``value`` and the next value is > ``value``.

Finding the place to insert a node follows the has(T &val) algorithm to find the place in the skip list to create a new node.
Inserts of duplicate values are made after any existing duplicate values.
All nodes are inserted at level 0 even if the insertion point can be seen at a higher level.
The search for an insertion location creates a recursion stack that, when unwound, updates the traversed nodes {width, Node<T>*} data.
Once an insert position is found a Node is created whose height is determined by repeatedly tossing a virtual coin until 'tails' is found.
This node initially has all node references to be to itself (this), and the widths set to 1 for level 0 and 0 for the remaining levels, they will be used to sum the widths at one level down.
On recursion ('left') each node adds its width to the new node at the level above the current level.
On moving up a level the current node swaps its width and node pointer with the new node at that new level.

------------------
``remove(T &val)``
------------------

Declaration: ``void remove(const T &value);``

Removes the value from the skip list. This will throw an ``ValueError`` if the value is not present.

If there are duplicate values the last one is removed first, this is for symmetry with insert().
Essentially this is the same as insert() but once the node is found the insert() updating algorithm is reversed and the node deleted.


-------------------------------------
Specialised APIs
-------------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^
``height() const``
^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t height() const;``

Returns the number of linked lists that make up the skip list. Minimum 1.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``height(size_t idx) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t height(size_t idx) const;``

Returns the number of linked lists that a particular node at index ``idx`` participates in. Will throw a IndexError if idx is >= size of the skip list. Minimum 1.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``width(size_t idx, size_t level) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t width(size_t idx, size_t level) const;``

Returns the number of nodes a node reference skips for the node at ``idx`` and the level at ``level``. Will throw a IndexError if idx is >= size of the skip list or ``level`` >= the node height. Minimum 1.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``dotFile(std::ostream &os) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``void dotFile(std::ostream &os) const;``

Writes a representation of the current state of the skip is as a fragment of a DOT file to the stream ``os`` as a ``subgraph``.

Internally the ``HeadNode`` keeps a count of how many times this has been called. The first time this function is called writes the preamble of DOT file as well as the subgraph.

Writing to ``os`` before or between ``dotFile()`` calls may produce undefined DOT format files.

Here is an example of using this method:

.. code-block:: cpp

    #include "SkipList.h"
    
    ManAHL::SkipList::HeadNode<double> sl;
    sl.insert(42.0);
    sl.insert(84.0);
    sl.insert(21.0);
    sl.insert(100.0);
    sl.insert(12.0);
    sl.dotFile(std::cout);
    sl.dotFileFinalise(std::cout);

There are examples of code that performs this in *src/cpp/main.cpp*

There are examples of the result in :ref:`skiplist-visualisation-label`

This method requires the macro ``INCLUDE_METHODS_THAT_USE_STREAMS`` to be defined.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``dotFileFinalise(std::ostream &os) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``void dotFileFinalise(std::ostream &os) const;``

Writing to ``os`` after ``dotFileFinalise()`` produces may produce undefined DOT format files.

This method requires the macro ``INCLUDE_METHODS_THAT_USE_STREAMS`` to be defined.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``lacksIntegrity() const;``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``IntegrityCheck lacksIntegrity() const;``

Returns non-zero if the integrity of this data structure is compromised.
The return values are defined in *src/cpp/IntegrityEnums.h*
This is a thorough but expensive check!

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``size_of() const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t size_of() const;``

Estimate of the number of bytes used by the skip list. This will not include any dynamically allocated memory such as used by ``std::string``.

.. _skiplist-python-api-label:

===============================================
Python API
===============================================

-------------------------
Example in Python
-------------------------

.. code-block:: python

    import cSkipList
    
    sl = cSkipList.PySkipList(float)
    
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


