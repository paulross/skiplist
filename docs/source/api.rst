.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 2

***********************************************
Skip List API
***********************************************

.. _skiplist-cpp-api-label:

===============================================
C++ API
===============================================

This describes the C++ API to a skip list with some algorithmic details of their implementation. Here is a visualisation of a skip list::

    | 5 E |------------------------------------->| 4 0 |---------------------------->| NULL |
    | 1 A |->| 2 C |---------->| 2 E |---------->| 2 G |---------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |

In these descriptions:

* 'right' and 'left' is used to mean move to a higher/lower ordinal node.
* 'up' and 'down' means to move to a coarser/finer grained list, 'top' is the highest, 'bottom' is the level 0.

-------------------------
Example in C++
-------------------------

.. code-block:: cpp

    #include "SkipList.h"
        
    SkipList::HeadNode<double> sl;
    
    sl.insert(42.0);
    sl.insert(21.0);
    sl.insert(84.0);
    
    sl.has(42.0) // true
    sl.size()    // 3
    sl.at(1)     // 42.0, throws SkipList::IndexError if index out of range

    sl.remove(21.0); // throws SkipList::ValueError if value not present
    
    sl.size()    // 2
    sl.at(1)     // 84.0

--------------------
Constructors
--------------------

There is only one constructor to a ``HeadNode`` and that takes no arguments.

---------------------------------------
``HeadNode::insert(const T &val)``
---------------------------------------

Declaration: ``void HeadNode::insert(const T &value);``

Inserts a copy of ``value`` such that the previous value, if present, is <= ``value`` and the next value, if present, is > ``value``.

^^^^^^^^^^^^^^^^^
Algorithm
^^^^^^^^^^^^^^^^^

Finding the place to insert a node first follows the ``has(T &val)`` algorithm to find the place in the skip list to create a new node.
Inserts of duplicate values are made after any existing duplicate values.
All nodes are inserted at level 0 even if the insertion point can be seen at a higher level.
The search for an insertion location creates a recursion stack that, when unwound, updates the traversed nodes ``{width, Node<T>*}`` data.

Once an insert position is found a Node is created whose height is determined by repeatedly tossing a virtual coin until 'tails' is found.
This node initially has all node references to be to itself (this), and the widths set to 1 for level 0 and 0 for the remaining levels, they will be used to sum the widths at one level down.
On recursion ('left') each node adds its width to the new node at the level above the current level.
On moving up a level the current node swaps its width and node pointer with the new node at that new level.

-------------------------------------------
``HeadNode::remove(const T &val)``
-------------------------------------------

Declaration: ``void HeadNode::remove(const T &value);``

Removes the value from the skip list. This will throw an ``ValueError`` if the value is not present.

If there are duplicate values the last one is removed first, this is for symmetry with ``insert()``.
Essentially this is the same as ``insert()`` but once the node is found the ``insert()`` updating algorithm is reversed and the node deleted.

------------------------------------------
``HeadNode::has(const T &val) const;``
------------------------------------------

Declaration: ``bool HeadNode::has(const T &value) const;``

This returns ``true`` or ``false`` if the skip list has the value val. This is O(log(n)) for well formed skip lists.

^^^^^^^^^^^^^^^^^
Algorithm
^^^^^^^^^^^^^^^^^

Starting at the highest possible level search rightwards until a larger value is encountered, then drop down. At level 0 return true if the Node value is the supplied value.

-------------------------------------
``HeadNode::at(size_t index) const;``
-------------------------------------

Declaration: ``const T& HeadNode::at(size_t index) const;``

This returns the value of type ``T`` at the given index. This will throw an ``IndexError`` if the index is >= size of the skip list. This is O(log(n)) for well formed skip lists.

^^^^^^^^^^^^^^^^^
Algorithm
^^^^^^^^^^^^^^^^^

The algorithm is similar to ``has(T &val)`` but the search moves rightwards if the width is less than the index and decrementing the index by the width.
If progress can not be made to the right, drop down a level.
If the index is 0 return the node value.

-------------------------------------------------------------------------
``HeadNode::at(size_t index, size_t count, std::vector<T> &dest) const;``
-------------------------------------------------------------------------

Declaration: ``void HeadNode::at(size_t index, size_t count, std::vector<T> &dest) const;``

This loads a vector of type ``T`` with ``count`` values starting at the given index. This will throw an ``IndexError`` if the index + count is >= size of the skip list. This is O(count * log(n)) for well formed skip lists.

^^^^^^^^^^^^^^^^^
Algorithm
^^^^^^^^^^^^^^^^^

The inital location follows the algorithm of ``at(size_t index) const;`` then sequential nodes are included.

----------------------------------------------
``HeadNode::index(const T &value) const;``
----------------------------------------------

Declaration: ``size_t HeadNode<T>::index(const T& value) const``

Returns the index of the first occurence of the value. This will throw a ``ValueError`` if not found. This will throw a ``FailedComparison`` if the value is not comparable. This is O(log(n)) for well formed skip lists.

``at(index(value))`` is always true if ``value`` is in the skip list. If there are no duplicate values ``index(at(i))`` is true for all indices.

------------------------------
``HeadNode::size() const``
------------------------------

Declaration: ``size_t HeadNode::size() const;``

Returns the number of items in the skip list.

-------------------------------------
Specialised APIs
-------------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::height() const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t HeadNode::height() const;``

Returns the number of linked lists that make up the skip list (minimum 1).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::height(size_t idx) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t HeadNode::height(size_t idx) const;``

Returns the number of linked lists that a particular node at index ``idx`` participates in. Will throw a IndexError if idx is >= size of the skip list (minimum 1).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::width(size_t idx, size_t level) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t HeadNode::width(size_t idx, size_t level) const;``

Returns the number of nodes a node reference skips for the node at ``idx`` and the level at ``level``. Will throw a IndexError if idx is >= size of the skip list or ``level`` >= the node height (minimum 1).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::dotFile(std::ostream &os) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``void HeadNode::dotFile(std::ostream &os) const;``

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
``HeadNode::dotFileFinalise(std::ostream &os) const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``void HeadNode::dotFileFinalise(std::ostream &os) const;``

Writing to ``os`` after ``dotFileFinalise()`` produces may produce undefined DOT format files.

This method requires the macro ``INCLUDE_METHODS_THAT_USE_STREAMS`` to be defined.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::lacksIntegrity() const;``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``IntegrityCheck HeadNode::lacksIntegrity() const;``

Returns non-zero if the integrity of this data structure is compromised.
The return values are defined in *src/cpp/IntegrityEnums.h*
This is a thorough but expensive check!

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``HeadNode::size_of() const``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Declaration: ``size_t HeadNode::size_of() const;``

Estimate of the number of bytes used by the skip list. This will not include any dynamically allocated memory such as used by ``std::string``.

.. _skiplist-python-api-label:

===============================================
Python API
===============================================

The Python API closely follows the C++ API.

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

-------------------------------------
Module ``cSkipList``
-------------------------------------

The module contains the following attributes:

=================================== ================================================================
Attribute                           Description
=================================== ================================================================
``cSkipList.__build_target__``      The Python version this module is built for.
                                    Example: ``'3.5.1'``
``cSkipList.__build_time__``        The date and time the module was built.
                                    Example: ``'Jul 14 2016 11:35:03'``
``cSkipList.__build_type__``        The build type, either ``'debug'`` or ``'release'``.
``cSkipList.__version__``           The version of the build. Example ``'0.1.0'``.
=================================== ================================================================

The module contains the following module level functions:

=================================== ================================================================
Function                            Description
=================================== ================================================================
``toss_coin()``                     Returns the result of a virtual coin toss.
``seed_rand(long)``                 Seeds the random number generator with a long integer.
``min_long()``                      The minimum storable value of a ``PySkipList(long)``.
``max_long()``                      The maximum storable value of a ``PySkipList(long)``.
=================================== ================================================================

-------------------------------------
Class ``cSkipList.PySkipList``
-------------------------------------

--------------------
Constructor
--------------------

The constructor takes a Python type. The following are valid:

.. code-block:: python

    import cSkipList
    
    sl = cSkipList.PySkipList(int) # Python 3, for Python 2 use PySkipList(long)
    sl = cSkipList.PySkipList(float)
    sl = cSkipList.PySkipList(bytes) # In Python 2 PySkipList(str) also works

------------------------------------
``PySkipList.has(val)``
------------------------------------

This returns ``True`` or ``False`` if the skip list has the value val. Will raise a ``TypeError`` if ``val`` is not the same type as the skip list was constructed with. This is O(log(n)) for well formed skip lists.

-------------------------------------
``PySkipList.at(index)``
-------------------------------------

This returns the value at the given index which must be of type long. Negative values of the index are dealt with Pythonically. This will raise an ``IndexError`` if the index is >= size of the skip list. This is O(log(n)) for well formed skip lists.

-------------------------------------------------------------------------
``PySkipList.at_seq(index, count)``
-------------------------------------------------------------------------

This returns a tuple of ``count`` values starting at the given ``index`` which must be of type long. Negative values of the index are dealt with Pythonically. ``count`` must be positive. This tuple contains a copy of the data in the skip list. This will raise an ``IndexError`` if the ``index`` + ``count`` is >= size of the skip list. This is O(count * log(n)) for well formed skip lists.

----------------------------------------------
``PySkipList.index(value)``
----------------------------------------------

Returns the index of the first occurence of the value. This will throw a ``ValueError`` if not found or the value is not comparable. This is O(log(n)) for well formed skip lists.

------------------------------
``PySkipList.size()``
------------------------------

Returns the number of items in the skip list.

---------------------------------
``PySkipList.insert(value)``
---------------------------------

Inserts a copy of ``value`` such that the previous value, if present, is <= ``value`` and the next value, if present, is > ``value``. Will raise a ``TypeError`` if ``val`` is not the same type as the skip list was constructed with.

In the case of a ``PySkipList(long)`` if the value < ``min_long()`` or > ``max_long()`` an ``OverflowError`` will be raised.

------------------------------
``PySkipList.remove(value)``
------------------------------

Removes the value from the skip list. This will raise an ``ValueError`` if the value is not present. Will raise a ``TypeError`` if ``val`` is not the same type as the skip list was constructed with.

If there are duplicate values the last one is removed first, this is for symmetry with ``insert()``.
Essentially this is the same as ``insert()`` but once the node is found the ``insert()`` updating algorithm is reversed and the node deleted.

In the case of a ``PySkipList(long)`` if the value < ``min_long()`` or > ``max_long()`` an ``OverflowError`` will be raised.

-------------------------------------
Specialised APIs
-------------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``PySkipList.height()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the number of linked lists that make up the skip list (minimum 1).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``PySkipList.node_height(index)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the number of linked lists that a particular node at index ``index`` participates in (minimum 1). Will raise a ``IndexError`` if idx is >= size of the skip list.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``PySkipList.width(index, level)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the number of nodes a node reference skips for the node at ``index`` and the level at ``level``. Will raise an ``IndexError`` if ``index`` is >= size of the skip list or ``level`` >= the node height (minimum 1).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``PySkipList.dot_file()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns a representation of the current state of the skip is as a bytes object that represents a DOT file.

This method requires the macro ``INCLUDE_METHODS_THAT_USE_STREAMS`` to be defined.

There are examples of the result in :ref:`skiplist-visualisation-label`

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
``PySkipList.lacks_integrity()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns non-zero if the integrity of this data structure is compromised.
The return values are defined in *src/cpp/IntegrityEnums.h*
This is a thorough but expensive check!
