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

A skip list behaves as a maintained sorted list with O(log(n)) cost for insertion, look-up and removal. This makes it ideal for such operations as computing the rolling median of a large dataset. A skip list is an alternative to binary tree.

A skip list is implemented as a singly linked list of ordered nodes where each node participates in a subset of, sparser, linked lists. These additional 'sparse' linked lists provide rapid indexing and mutation of the underlying linked list.

A skip list is a probabilistic data structure using a random function to determine how many 'sparse' linked lists any particular node participates in.

The advantages claimed for skip lists are that the insert and remove logic is simpler (however I do not subscribe to this).

The drawbacks of a skip list include its larger space requirements and its O(log(N)) lookup behaviour compared to other, more restricted and specialised, data structures that may have either faster runtime behaviour or lower space requirements or both.

Wikipedia has a introductory page on `skip lists <https://en.wikipedia.org/wiki/Skip_list>`_ .


This project contains a Skip List implementation in C++ with Python bindings that has the following characteristics:

* No capacity restrictions apart from available memory.
* Works with any C++ type <T> that has meaningful comparison operators.
* This implementation is heavily performance tested.
* Functional tests at both the C++ and Python level.
* Deterministic tests of a probabilistic data structure.
* The C++ skip list has exhaustive internal integrity checks.
* The skip list can generate DOT files that provide a visualisation of its internal state.
* There is extensive documentation. 

Originally written by Paul Ross with credits to AHL engineers: Wilfred Hughes, Luke Sewell and Terry Tsantagoeds.
