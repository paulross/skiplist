//
//  NodeRefs.h
//  SkipList
//
//  Created by Paul Ross on 03/12/2015.
//  Copyright (c) 2017 Paul Ross. All rights reserved.
//

#ifndef SkipList_NodeRefs_h
#define SkipList_NodeRefs_h

#include "IntegrityEnums.h"

// Forward reference
template <typename T, typename _Compare>
class Node;

template <typename T, typename _Compare=std::less<T> >
struct NodeRef {
    Node<T, _Compare> *pNode;
    size_t width;
};

/******************** SwappableNodeRefStack **********************
 * Class that represents a stack of references to other nodes.
 * Each reference is a NodeRef so a pointer to a Node and a width.
 * This just does simple bookkeeping on this stack.
 */
template <typename T, typename _Compare>
class SwappableNodeRefStack {
public:
    SwappableNodeRefStack() : _swapLevel(0) { }
    
    // Const methods
    // -------------
    // Subscript read/write
    const NodeRef<T, _Compare> &operator[](size_t level) const;
    NodeRef<T, _Compare> &operator[](size_t level);
    // Number of nodes referenced
    size_t height() const {
        return _nodes.size();
    }
    // The current swap level
    size_t swapLevel() const { return _swapLevel; }
    bool canSwap() const { return _swapLevel < height(); }
    // Returns true if there is no record of p in my data that
    // could lead to circular references
    bool noNodePointerMatches(const Node<T, _Compare> *p) const;
    // Returns true if all pointers in my data are equal to p.
    bool allNodePointerMatch(const Node<T, _Compare> *p) const;
    
    // Non-const methods
    // -----------------
    // Add a new reference
    void push_back(Node<T, _Compare> *p, size_t w) {
    	struct NodeRef<T, _Compare> val = {p, w};
        _nodes.push_back(val);
    }
    // Remove top reference
    void pop_back() {
        _nodes.pop_back();
    }
    // Swap reference at current swap level with another SwappableNodeRefStack
    void swap(SwappableNodeRefStack<T, _Compare> &val);
    // Reset the swap level (for example before starting a remove).
    void resetSwapLevel() { _swapLevel = 0; }
    // Increment the swap level. This is used when removing nodes where
    // the parent node can record to what level it has made its adjustments
    // so the grand parent knows where to start.
    // For this reason the _swapLevel can easily be >= _nodes.size().
    void incSwapLevel() { ++_swapLevel; }

    IntegrityCheck lacksIntegrity() const;
    
    // Returns an estimate of the memory usage of an instance
    size_t size_of() const;
    
protected:
    std::vector<struct NodeRef<T, _Compare> > _nodes;
    size_t _swapLevel;
    
private:
    // Prevent cctor and operator=
    SwappableNodeRefStack(const SwappableNodeRefStack &that);
    SwappableNodeRefStack &operator=(const SwappableNodeRefStack &that) const;
};

template <typename T, typename _Compare>
const NodeRef<T, _Compare> &SwappableNodeRefStack<T, _Compare>::operator[](size_t level) const {
    // NOTE: No bounds checking on vector::operator[], so this assert will do
    assert(level < _nodes.size());
    return _nodes[level];
}

template <typename T, typename _Compare>
NodeRef<T, _Compare> &SwappableNodeRefStack<T, _Compare>::operator[](size_t level) {
    // NOTE: No bounds checking on vector::operator[], so this assert will do
    assert(level < _nodes.size());
    return _nodes[level];
}

// Whether all node references are swapped.
// Should be true after an insert.
template <typename T, typename _Compare>
bool SwappableNodeRefStack<T, _Compare>::noNodePointerMatches(const Node<T, _Compare> *p) const {
    for (size_t level = height(); level-- > 0;) {
        if (p == _nodes[level].pNode) {
            return false;
        }
    }
    return true;
}

// Returns true if all pointers in my data are equal to p.
// Should be true after an remove.
template <typename T, typename _Compare>
bool SwappableNodeRefStack<T, _Compare>::allNodePointerMatch(const Node<T, _Compare> *p) const {
    for (size_t level = height(); level-- > 0;) {
        if (p != _nodes[level].pNode) {
            return false;
        }
    }
    return true;
}

// Swap references with another SwappableNodeRefStack at the current swap level.
template <typename T, typename _Compare>
void SwappableNodeRefStack<T, _Compare>::swap(SwappableNodeRefStack<T, _Compare> &val) {
    assert(_swapLevel < height());
    NodeRef<T, _Compare> temp = val[_swapLevel];
    val[_swapLevel] = _nodes[_swapLevel];
    _nodes[_swapLevel] = temp;
    ++_swapLevel;
}

/* This checks the internal consistency of the object. It returns
 * INTEGRITY_SUCCESS [0] if successful or non-zero on error.
 * The tests are:
 *
 * - Widths must all be >= 1
 * - Widths must be weakly increasing with increasing level
 */
template <typename T, typename _Compare>
IntegrityCheck SwappableNodeRefStack<T, _Compare>::lacksIntegrity() const {
    if (height()) {
        if (_nodes[0].width != 1) {
            return NODEREFS_WIDTH_ZERO_NOT_UNITY;
        }
        for (size_t level = 1; level < height(); ++level) {
            if (_nodes[level].width < _nodes[level - 1].width) {
                return NODEREFS_WIDTH_DECREASING;
            }
        }
    }
    return INTEGRITY_SUCCESS;
}

// Returns an estimate of the memory usage of an instance
template <typename T, typename _Compare>
size_t SwappableNodeRefStack<T, _Compare>::size_of() const {
    return sizeof(*this) + _nodes.capacity() * sizeof(struct NodeRef<T>);
}

/********************* END: SwappableNodeRefStack ****************************/

#endif // SkipList_NodeRefs_h
