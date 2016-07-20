.. highlight:: python
    :linenothreshold: 10

.. highlight:: c
    :linenothreshold: 10

.. toctree::
    :maxdepth: 3

***********************************************
Design and Implementation of the C++ Skip List
***********************************************

.. _skiplist-design-implementation-label:

===============================================
Design of the C++ Skip List
===============================================

A skip list is a a singly linked list with additional, coarser, linked lists. These additional lists allow rapid location, insertion and removal of nodes. Values in a skip list are maintained in order at all times.

Skip lists are alternatives to balanced trees for operations such as a rolling median.
The disadvantages of skip lists are:

- Less space efficient than balanced trees, see: :ref:`performance-space-complexity-label`.
- Performance is similar to balanced trees except finding the mid-point which is O(log(N)) for a skip list compared with O(1) for a balanced tree.

The advantages claimed for skip lists are:
    - The insert() and remove() logic is simpler (I do not subscribe to this).

This skip list design has the coarser lists implemented as optional additional links between the nodes themselves.
The illustration below shows a well formed skip list with a head node (``HED``) linked to the ordered nodes ``A`` to ``H``::

    | 5 E |------------------------------------->| 4 0 |---------------------------->| NULL |
    | 1 A |->| 2 C |---------->| 2 E |---------->| 2 G |---------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |


Each node has a stack of values that consist of a 'width' and a reference to another node (or NULL).
At the lowest level is a singly linked list and all widths are 1. They are shown in the diagram as a number for the width and the node name for the reference (0 for NULL). The 'widths' at each node/level specify how many level 0 nodes the node reference skips over. These widths are used to rapidly index into the skip list starting from the highest level and working down.

At level 1 the links are (ideally) to every other node, at level 2 the links are (ideally) to every fourth node  and at level 3 the links are (ideally) to every eighth node and so on.

--------------------------------------
Search Algorithms
--------------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Presence
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This search returns true/false as to whether a value is present. The search is recursive starting at the coarsest list dropping down a level when the search overshoots. Take for example searching for the presence of ``D``. The search starts as ``HED[2]`` this leads to ``E[2]`` and ``E > D`` so that search is rejected. The search then tries ``HED[2] -> HED[1]`` which leads to ``A[1] -> C[1] -> E[1]``, again that search is rejected as ``E > D``. The search finally succeeds following the path ``HED[2] -> HED[1] -> A[1] -> C[1] -> C[0] -> D[0]`` so returns true.

It is not necessary for the search to reach level 0 if it can be terminated earlier. For example the search for ``C`` would be achieved by ``HED[2] -> HED[1] -> A[1] -> C[1]``.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Index
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Searching for a particular index value is accomplished in a similar way using the widths to calculate the position in the list. For example to find the 6th node (``F``) the search starts at ``HED[2]`` and moves to ``E[2]``. Any further move to the right is not possible because 5 + 4 > 6. Dropping down to ``E[1]`` no further progress can be made to the right as 5 + 2 > 6. Dropping down to ``E[0]`` finds node ``F`` as 5 + 1 == 6.

It is not necessary for the search to reach level 0 if it can be terminated earlier. For example the search for the fifth member would be achieved by ``HED[2] -> E[2]``.

--------------------------------------
Insertion Algorithm
--------------------------------------

To understand how the skip list is maintained, consider insertion. Before inserting node ``E`` the skip list would look like this::
 
    | 1 A |->| 2 C |---------->| 3 G |------------------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  F  |  |  G  |  |  H  |

Inserting ``E`` means:

* Finding where ``E`` should be inserted (after ``D``).
* Creating node ``E`` with a random height (heads/heads/tails so 3 high).
* Updating ``D`` to refer to ``E`` at level 0.
* Updating ``C`` to refer to ``E`` at level 1 and decreasing ``C``'s width to 2, increasing ``E`` width at level 1 to 2.
* Expanding ``HED`` to level 2 with a reference to ``E`` and a width of 5.
* Updating ``E`` with a reference to NULL and a width of 4.

After inserting node E the skip list looks like this::

    | 5 E |------------------------------------->| 4 0 |---------------------------->| NULL |
    | 1 A |->| 2 C |---------->| 2 E |---------->| 2 G |---------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Insertion Position
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The first two operations are done by a recursive search.
This creates the call chain that passes through the nodes ``HED[1] -> A[1] -> C[1] -> C[0] -> D[0]``. Thus ``E`` will be created at level 0 and inserted after ``D``.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Node Creation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Node E is created with a stack containing a single pointer to the next node F.
Then a virtual coin is tossed, for each 'head' an extra NULL reference is added to the stack.
If a 'tail' is thrown the stack is complete.
In the example above when creating ``E`` we have encountered tosses of 'head', 'head', 'tail'.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Recursive Unwinding
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The remaining operations are done as recursion unwinds.
D[0] and C[0] update E[1] with their cumulative width (2).
C[1] adds 1 to width (a new node is inserted) then subtracts E[1].
Then C[1]/E[1] are swapped so that the pointers and widths are correct.
And so on until HED is reached, in this case a new level is added and HED[2] swapped with E[2].

A more detailed description of the algorithms used for each method is in the :ref:`skiplist-cpp-api-label`

--------------------------------------
Removal Algorithm
--------------------------------------

This is essentially the insertion algorithm in reverse. Before removing node E the skip list looks like this::

    | 5 E |------------------------------------->| 4 0 |---------------------------->| NULL |
    | 1 A |->| 2 C |---------->| 2 E |---------->| 2 G |---------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |

After removing ``E`` the skip list looks like this::
 
    | 1 A |->| 2 C |---------->| 3 G |------------------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  F  |  |  G  |  |  H  |

To remove ``E`` a recursive search for ``E[0]`` is made. Starting at the top (level = 2) of the head node ``HED[2]`` then ``E[2]`` is found immediately however the level is not 0 so the search is discarded. A search through ``HED[2] -> HED[1]`` follows the path ``HED[2] -> HED[1] -> A[1] -> C[1]`` to ``E[1]``, again this search is discarded as the level is not 0.

The final search succeeds by setting up the call chain ``HED[2] -> HED[1] -> A[1] -> C[1] -> C[0] -> D[0] -> E[0]``.

As the recursion unwinds node that data at each level of ``E`` is used to swap node pointers and adjust the widths of node C. Finally ``HED`` can drop one level before deleting ``E``.

=====================================
Implementation Notes
=====================================

This skip list is implemented in C++ with templates so works for any type ``<T>`` that can be compared deterministically.

Node membership of the additional lists is done in a probabilistic manner. This is achieved at node creation time by tossing a virtual coin.
These lists are not explicit, they are implied by the references between Nodes at a particular level.

'Level' is an ``size_t`` type that specifies the linked list, level 0 is the linked list to every node.
The list at level 1 links (ideally) to every other node.
The list at level 2 links (ideally) to every fourth node and so on.
In general the list at level n links (ideally) to every 2**n node.

.. warning::

    This implementation is not thread safe.

--------------------------------------
Memory Management
--------------------------------------

Memory management is pretty simple in a skip list. Essentially it is no more complicated than a singly linked list and every node is created and deleted at level 0. The duplicate pointers at higher levels can be ignored.

===========
Code Layout
===========
There are three template classes defined in their own .h files and these are all included into the *SkipList.h* file.

The classes are:

=============================== ====================================================================
Class                           Description
=============================== ====================================================================
``SwappableNodeRefStack<T>``    A simple bookkeeping class that has a vector of
                                ``[{skip_width, Node<T>*}, ...]``
                                This vector can be expanded or contracted at will. Both HeadNode
                                and Node classes have one of these to manage their references.
                                This is defined in *NodeRefs.h*
``Node<T>``                     This represents a single value in the skip list. The height of a
                                Node is determined at construction by tossing a virtual coin, this
                                determines how many coarser lists this node participates in.
                                A Node has a ``SwappableNodeRefStack`` object and a value of
                                type ``T``.
                                This is defined in *Node.h*
``HeadNode<T>``                 There is one of these per skip list and this provides the API to
                                the entire skip list. The height of the HeadNode expands and
                                contracts as required when Nodes are inserted or removed
                                (it is the height of the highest Node).
                                A HeadNode has a ``SwappableNodeRefStack`` object and an
                                independently maintained count of the number of Node objects in the
                                skip list.
                                This is defined in *HeadNode.h*
=============================== ====================================================================

A ``Node`` and ``HeadNode`` have specialised methods such as ``has()``, ``at()``, ``insert()``, ``remove()`` that traverse the skip list recursively. These are described in the :ref:`skiplist-cpp-api-label`

---------------------------
Other Files of Significance
---------------------------
*SkipList.cpp* exposes the random number generator (``rand()``) and seeder (``srand()``) so that they can be accessed by CPython and others for deterministic testing.

*IntegrityEnums.h* has definitions of error codes that can be created by the skip list integrity checking functions.

*cSkipList.h*, *cSkipList.cpp* contains a CPython module with a SkipList implementation for a number of builtin Python types. This has the usual *setup.py* and *setup.cfg* files.

===========
Code Idioms
===========

---------------
Prevent Copying
---------------
Copying operations are (mostly) prohibited for performance reasons.
The only class that allows copying is ``struct NodeRef`` that contains fundamental types (int and pointer).
All other classes declare their copying operation private and unimplemented (rather than using C++11 delete) for compatibility with older compilers.

--------------------------------
Reverse Loop of ``unsigned int``
--------------------------------
In a lot of the code we have to count down from some value to 0
with a ``size_t`` (an ``unsigned int`` type), the idiom used is this:

.. code-block:: c

    for (size_t l = height(); l-- > 0;) {

The ``l-- > 0`` will test ``l`` against 0 then decrement it.
``l`` will thus start at the value ``height() - 1`` down to 0 then exit the loop.

.. note::

    If ``l`` is declared before the loop it will have the maximum value of a size_t on exit from this loop unless a break statement is encountered.

==========================
Worst Case Behaviour
==========================

It is possible that the skip list can get mal-formed with an unfortunate set of coin tosses (or a knowledgeable and adversarial user). In which case the search time complexity can become O(n) rather than O(log(n)). Here is a well-formed skip list::

    | 5 E |------------------------------------->| 4 0 |---------------------------->| NULL |
    | 1 A |->| 2 C |---------->| 2 E |---------->| 2 G |---------->| 2 0 |---------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |

The same frequency of coin tosses but in worst case order might produce this::

    | 5 E |--| 8 0 |---------------------------------------------------------------->| NULL |
    | 1 A |->| 1 B |--| 1 C |->| 1 D |->| 5 0 |------------------------------------->| NULL |
    | 1 A |->| 1 B |->| 1 C |->| 1 D |->| 1 E |->| 1 F |->| 1 G |->| 1 H |->| 1 0 |->| NULL |
    | HED |  |  A  |  |  B  |  |  C  |  |  D  |  |  E  |  |  F  |  |  G  |  |  H  |

Any search on the latter skip list rapidly degrades into a simple linear search.

There is no particular protection against this in this implementation of a skip list.

===================
Roads not Travelled
===================

Certain design and implementations were not explored, here they are and why.

------------------------
Key/Value Implementation
------------------------

Skip lists are commonly used for key/value dictionaries. Given things
like map<T> or unorderedmap<T> etc. in C++ I see no reason why a SkipList should be used
as an alternative.

-----------------
Adversarial Users
-----------------

If the user knows the behaviour of the random number generator it is possible that they can change the order of insertion to create a poor distribution of nodes which will make operations tend to O(N) rather than O(log(N)).

--------------------------------------------------
Optimisation: Re-index Nodes on Complete Traversal
--------------------------------------------------

To correct the worst case behaviour above it would be possible to re-index the skip list from time to time, for example when a full traversal is made. This is not included in this implementation.

----------------------------------------------
Optimisation: Reuse removed nodes
----------------------------------------------

If node construction is expensive then a ``remove()`` + ``insert()`` operation could re-use the removed node rather than deleting and creating a new one. This is not included in this implementation.

------------------
Reference Counting
------------------

Some time (and particularly space) improvement could be obtained by reference counting nodes so that duplicate values could be eliminated at some additional code/space cost.
Since the primary use case for this skip list is for computing the rolling median of doubles the chances of duplicates are slim and no advantage would be gained.
For int, long and string there is a higher probability of sameness so reference counting might be implemented in the future if these types become commonly used.

--------------------------------------------------------------------
Use an Array of ``{skip_width, Node<T>*}`` rather than a vector
--------------------------------------------------------------------

Less space would be used for each Node if the ``SwappableNodeRefStack`` used a dynamically allocated array of ``[{skip_width, Node<T>*}, ...]`` rather than a vector.
