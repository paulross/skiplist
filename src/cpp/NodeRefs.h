//
//  NodeRefs.h
//  SkipList
//
//  Created by Paul Ross on 03/12/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//

#ifndef SkipList_NodeRefs_h
#define SkipList_NodeRefs_h

#include "IntegrityEnums.h"

// Forward reference
template <typename T>
class Node;

template <typename T>
struct NodeRef {
    Node<T> *pNode;
    size_t width;
};

/******************** SwappableNodeRefStack **********************
 * Class that represents a stack of references to other nodes.
 * Each reference is a NodeRef so a pointer to a Node and a width.
 * This just does simple bookkeeping on this stack.
 */
template <typename T>
class SwappableNodeRefStack {
public:
    SwappableNodeRefStack() : _swapLevel(0) { }
    
    // Const methods
    // -------------
    // Subscript read/write
    const NodeRef<T> &operator[](size_t level) const;
    NodeRef<T> &operator[](size_t level);
    // Number of nodes referenced
    size_t height() const {
        return _nodes.size();
    }
    // The current swap level
    size_t swapLevel() const { return _swapLevel; }
    bool canSwap() const { return _swapLevel < height(); }
    // Returns true if there is no record of p in my data that
    // could lead to circular references
    bool noNodePointerMatches(const Node<T> *p) const;
    // Returns true if all pointers in my data are equal to p.
    bool allNodePointerMatch(const Node<T> *p) const;
    
    // Non-const methods
    // -----------------
    // Add a new reference
    void push_back(Node<T> *p, size_t w) {
    	struct NodeRef<T> val = {p, w};
        _nodes.push_back(val);
    }
    // Remove top reference
    void pop_back() {
        _nodes.pop_back();
    }
    // Swap reference at current swap level with another SwappableNodeRefStack
    void swap(SwappableNodeRefStack<T> &val);
    // Reset the swap level (for example before starting a remove).
    void resetSwapLevel() { _swapLevel = 0; }

    IntegrityCheck lacksIntegrity() const;
    
    // Returns the estimate of the memory usage of an instance
    size_t size_of() const;
    
protected:
    std::vector<struct NodeRef<T> > _nodes;
    size_t _swapLevel;
    
private:
    // Prevent cctor and operator=
    SwappableNodeRefStack(const SwappableNodeRefStack &that);
    SwappableNodeRefStack &operator=(const SwappableNodeRefStack &that) const;
};

template <typename T>
const NodeRef<T> &SwappableNodeRefStack<T>::operator[](size_t level) const {
    // NOTE: No bounds checking on vector::operator[], so this assert will do
    assert(level < _nodes.size());
    return _nodes[level];
}

template <typename T>
NodeRef<T> &SwappableNodeRefStack<T>::operator[](size_t level) {
    // NOTE: No bounds checking on vector::operator[], so this assert will do
    assert(level < _nodes.size());
    return _nodes[level];
}

// Whether all node references are swapped.
// Should be true after an insert.
template <typename T>
bool SwappableNodeRefStack<T>::noNodePointerMatches(const Node<T> *p) const {
    for (size_t level = height(); level-- > 0;) {
        if (p == _nodes[level].pNode) {
            return false;
        }
    }
    return true;
}

// Returns true if all pointers in my data are equal to p.
// Should be true after an remove.
template <typename T>
bool SwappableNodeRefStack<T>::allNodePointerMatch(const Node<T> *p) const {
    for (size_t level = height(); level-- > 0;) {
        if (p != _nodes[level].pNode) {
            return false;
        }
    }
    return true;
}

// Swap references with another SwappableNodeRefStack at the current swap level.
template <typename T>
void SwappableNodeRefStack<T>::swap(SwappableNodeRefStack<T> &val) {
    NodeRef<T> temp = val[_swapLevel];
    val[_swapLevel] = _nodes[_swapLevel];
    _nodes[_swapLevel] = temp;
    ++_swapLevel;
}

/* This checks the internal consistency of the object. It returns
 * INTEGRITY_SUCCESS [0] if successful,
 * non-zero on error. The tests are:
 *
 * - Widths must all be >= 1
 * - Widths muts be weakly increasing with level
 */
template <typename T>
IntegrityCheck SwappableNodeRefStack<T>::lacksIntegrity() const {
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

// Returns the estimate of the memory usage of an instance
template <typename T>
size_t SwappableNodeRefStack<T>::size_of() const {
    return sizeof(*this) \
        + sizeof(std::vector<struct NodeRef<T> >) \
        + _nodes.capacity() * sizeof(struct NodeRef<T>) \
        + sizeof(size_t) /* _swapLevel */;
}

/********************* END: SwappableNodeRefStack ****************************/

#endif // SkipList_NodeRefs_h
