
//  Node.h
//  SkipList
//
//  Created by Paul Ross on 03/12/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//

#ifndef SkipList_Node_h
#define SkipList_Node_h

#include "IntegrityEnums.h"

#if __cplusplus < 201103L
#define nullptr NULL
#endif

/**************************** Node *********************************/
template <typename T>
class Node {
public:
    Node(const T &value);
    // Const methods
    //
    // Returns the node value
    const T &value() const { return _value; }
    // Returns true if the value is present in the skip list from this node onwards.
    bool has(const T &value) const;
    // Returns the value at the index in the skip list from this node onwards.
    // Will return nullptr is not found.
    const Node<T> *at(size_t idx) const;
    // Number of linked lists that this node engages in, minimum 1.
    size_t height() const { return _nodeRefs.height(); }
    // Return the pointer to the next node at level 0
    const Node<T> *next() const;
    // Return the width at given level.
    size_t width(size_t level) const;
    // Return the node pointer at given level, only used for HeadNode
    // integrity checks.
    const Node<T> *pNode(size_t level) const;
    
    // Non-const methods
    // Get a reference to the node references
    SwappableNodeRefStack<T> &nodeRefs() { return _nodeRefs; }
    const SwappableNodeRefStack<T> &nodeRefs() const { return _nodeRefs; }
    // Insert a node
    Node<T> *insert(const T &value);
    // Remove a node
    Node<T> *remove(size_t call_level, const T &value);
    // An estimate of the number of bytes used by this node
    size_t size_of() const;
    
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    void dotFile(std::ostream &os, size_t suffix = 0) const;
    void writeNode(std::ostream &os, size_t suffix = 0) const;
#endif // INCLUDE_METHODS_THAT_USE_STREAMS
    
    // Integrity checks, returns non-zero on failure
    IntegrityCheck lacksIntegrity(size_t headnode_height) const;
    IntegrityCheck lacksIntegrityRefsInSet(const std::set<const Node<T>*> &nodeSet) const;
    
protected:
    Node<T> *_adjRemoveRefs(size_t level, Node<T> *pNode);
    
protected:
    T _value;
    SwappableNodeRefStack<T> _nodeRefs;
    
private:
    // Prevent cctor and operator=
    Node(const Node &that);
    Node &operator=(const Node &that) const;
};

template <typename T>
Node<T>::Node(const T &value) : _value(value) {
    do {
        _nodeRefs.push_back(this, _nodeRefs.height() ? 0 : 1);
    } while (tossCoin());
}

template <typename T>
bool Node<T>::has(const T &value) const {
    assert(_nodeRefs.height());
    assert(value == value); // value can not be NaN for example
    if (value > _value) {
        for (size_t l = _nodeRefs.height(); l-- > 0;) {
            if (_nodeRefs[l].pNode && _nodeRefs[l].pNode->has(value)) {
                return true;
            }
        }
        return false;
    }
    // false if value smaller
    return value == _value;
}

/* Return a pointer to the n'th node.
 * Start (or continue) from the highest level, drop down a level if not
 * found.
 * Return nullptr if not found at level 0.
 */
template <typename T>
const Node<T> *Node<T>::at(size_t idx) const {
    assert(_nodeRefs.height());
    if (idx == 0) {
        return this;
    }
    for (size_t l = _nodeRefs.height(); l-- > 0;) {
        if (_nodeRefs[l].pNode && _nodeRefs[l].width <= idx) {
            return _nodeRefs[l].pNode->at(idx - _nodeRefs[l].width);
        }
    }
    return nullptr;
}

template <typename T>
const Node<T> *Node<T>::next() const {
    assert(_nodeRefs.height());
    return _nodeRefs[0].pNode;
}

template <typename T>
size_t Node<T>::width(size_t level) const {
    assert(level < _nodeRefs.height());
    return _nodeRefs[level].width;
}

template <typename T>
const Node<T> *Node<T>::pNode(size_t level) const {
    assert(level < _nodeRefs.height());
    return _nodeRefs[level].pNode;
}

template <typename T>
Node<T> *Node<T>::insert(const T &value) {
    assert(_nodeRefs.height());
    assert(_nodeRefs.noNodePointerMatches(this));
    assert(! _nodeRefs.canSwap());
    assert(value == value); // NaN check for double

    if (value < _value) {
        return nullptr;
    }
    // Recursive search for where to put the node
    Node<T> *pNode = nullptr;
    size_t level = _nodeRefs.height();
    if (value >= _value) {
        for (level = _nodeRefs.height(); level-- > 0;) {
            if (_nodeRefs[level].pNode) {
                pNode = _nodeRefs[level].pNode->insert(value);
                if (pNode) {
                    break;
                }
            }
        }
    }
    if (! pNode && value >= _value) {
        // Insert new node here
        pNode = new Node<T>(value);
        level = 0;
    }
    assert(pNode); // Should never get here unless a NaN has slipped through
    // Adjust references by marching up and recursing back.
    SwappableNodeRefStack<T> &thatRefs = pNode->_nodeRefs;
    if (! thatRefs.canSwap()) {
        // Have an existing node or new node that is all swapped.
        // All I need to do is adjust my overshooting nodes and return
        // this for the caller to do the same.
        level = thatRefs.height();
        while (level < _nodeRefs.height()) {
            _nodeRefs[level].width += 1;
            ++level;
        }
        // The caller just has to increment its references that overshoot this
        assert(! _nodeRefs.canSwap());
        return this;
    }
    // March upwards
    if (level < thatRefs.swapLevel()) {
        assert(level == thatRefs.swapLevel() - 1);
        // This will happen when say a 3 high node, A, finds a 2 high
        // node, B, that creates a new 2+ high node. A will be at
        // level 1 and the new node will have swapLevel == 2 after
        // B has swapped.
        // Add the level to the accumulator at the next level
        thatRefs[thatRefs.swapLevel()].width += _nodeRefs[level].width;
        ++level;
    }
    size_t min_height = std::min(_nodeRefs.height(), thatRefs.height());
    while (level < min_height) {
        assert(thatRefs.canSwap());
        assert(level == thatRefs.swapLevel());
        assert(level < thatRefs.height());
        assert(_nodeRefs[level].width > 0);
        assert(thatRefs[level].width > 0);
        _nodeRefs[level].width -= thatRefs[level].width - 1;
        assert(_nodeRefs[level].width > 0);
        thatRefs.swap(_nodeRefs);
        if (thatRefs.canSwap()) {
            assert(thatRefs[thatRefs.swapLevel()].width == 0);
            thatRefs[thatRefs.swapLevel()].width = _nodeRefs[level].width;
        }
        ++level;
    }
    // Upwards march complete, now recurse back ('left').
    if (! thatRefs.canSwap()) {
        // All done with pNode locally.
        assert(level == thatRefs.height());
        assert(thatRefs.height() <= _nodeRefs.height());
        assert(level == thatRefs.swapLevel());
        // Adjust my overshooting nodes
        while (level < _nodeRefs.height()) {
            _nodeRefs[level].width += 1;
            ++level;
        }
        // The caller just has to increment its references that overshoot this
        assert(! _nodeRefs.canSwap());
        pNode = this;
    }
    return pNode;
}

template <typename T>
Node<T> *Node<T>::_adjRemoveRefs(size_t level, Node<T> *pNode) {
    assert(pNode);
    SwappableNodeRefStack<T> &thatRefs = pNode->_nodeRefs;
    
    assert(pNode != this);
    if (level < thatRefs.swapLevel()) {
        assert(level == thatRefs.swapLevel() - 1);
        ++level;
    }
    if (thatRefs.canSwap()) {
        assert(level == thatRefs.swapLevel());
        while (level < _nodeRefs.height() && thatRefs.canSwap()) {
            assert(level == thatRefs.swapLevel());
            // Compute the new width for the new node
            thatRefs[level].width += _nodeRefs[level].width - 1;
            thatRefs.swap(_nodeRefs);
            ++level;
        }
        if (! thatRefs.canSwap()) {
            // Confirm all pointers swapped back to 'this'.
            assert(thatRefs.allNodePointerMatch(pNode));
            delete pNode;
            pNode = this;
        } else {
            // More to do
            return pNode;
        }
    }
    // Decrement my widths as my refs are over the top of the missing pNode.
    while (level < _nodeRefs.height()) {
        _nodeRefs[level].width -= 1;
        ++level;
    }
    assert(! _nodeRefs.canSwap());
    return this;
}

template <typename T>
Node<T> *Node<T>::remove(size_t call_level,
                         const T &value) {
    assert(_nodeRefs.height());
    assert(_nodeRefs.noNodePointerMatches(this));
    
    Node<T> *pNode = nullptr;
    if (value >= _value) {
        for (size_t level = call_level + 1; level-- > 0;) {
            if (_nodeRefs[level].pNode) {
                // Make progress to the right
                pNode = _nodeRefs[level].pNode->remove(level, value);
                if (pNode) {
                    return _adjRemoveRefs(level, pNode);
                }
            }
            // Make progress down
        }
    }
    if (! pNode) { // Base case
        // We only admit to being the node to remove if the caller is
        // approaching us from level 0. It is entirely likely that
        // the same (or an other) caller can see us at a higher level
        // but the recursion stack will not have been set up in the correct
        // step wise fashion so that the lower level references will
        // not be swapped.
        if (call_level == 0 and value == _value) {
            _nodeRefs.resetSwapLevel();
            return this;
        }
    }
    assert(pNode == nullptr);
    return nullptr;
}

/* This checks the internal concistency of a Node. It returns 0
 * if succesful, non-zero on error. The tests are:
 *
 * - Height must be >= 1
 * - Height must not exceed HeadNode height.
 * - NULL pointer must not have a non-NULL above them.
 * - Node pointers must not be self-referential.
 */
template <typename T>
IntegrityCheck Node<T>::lacksIntegrity(size_t headnode_height) const {
    IntegrityCheck result = _nodeRefs.lacksIntegrity();
    if (result) {
        return result;
    }
    if (_nodeRefs.height() == 0) {
        return NODE_HEIGHT_ZERO;
    }
    if (_nodeRefs.height() > headnode_height) {
        return NODE_HEIGHT_EXCEEDS_HEADNODE;
    }
    // Test: All nodes above a nullprt must be nullptr
    size_t level = 0;
    while (level < _nodeRefs.height()) {
        if (! _nodeRefs[level].pNode) {
            break;
        }
        ++level;
    }
    while (level < _nodeRefs.height()) {
        if (_nodeRefs[level].pNode) {
            return NODE_NON_NULL_AFTER_NULL;
        }
        ++level;
    }
    // No reference should be to self.
    if (! _nodeRefs.noNodePointerMatches(this)) {
        return NODE_SELF_REFERENCE;
    }
    return INTEGRITY_SUCCESS;
}

template <typename T>
IntegrityCheck Node<T>::lacksIntegrityRefsInSet(const std::set<const Node<T>*> &nodeSet) const {
    size_t level = 0;
    while (level < _nodeRefs.height()) {
        if (nodeSet.count(_nodeRefs[level].pNode) == 0) {
            return NODE_REFERENCES_NOT_IN_GLOBAL_SET;
        }
        ++level;
    }
    return INTEGRITY_SUCCESS;
}

// Returns an estimate of the memory usage of an instance
template <typename T>
size_t Node<T>::size_of() const {
    // sizeof(*this) includes the size of _nodeRefs but _nodeRefs.size_of()
    // includes sizeof(_nodeRefs) so we need to subtract to avoid double counting
    return sizeof(*this) + _nodeRefs.size_of() - sizeof(_nodeRefs);
}


#ifdef INCLUDE_METHODS_THAT_USE_STREAMS

template <typename T>
void Node<T>::writeNode(std::ostream &os, size_t suffix) const {
    os << "\"node";
    os << suffix;
    os << std::hex << this << std::dec << "\"";
}

template <typename T>
void Node<T>::dotFile(std::ostream &os, size_t suffix) const {
    assert(_nodeRefs.height());
    writeNode(os, suffix);
    os << " [" << std::endl;
    os << "label = \"";
    for (size_t level = _nodeRefs.height(); level-- > 0;) {
        os << " { <w" << level + 1 << "> " << _nodeRefs[level].width;
        os << " | <f" << level + 1 << "> ";
        os << std::hex << _nodeRefs[level].pNode << std::dec;
        os << " }";
        os << " |";
    }
    os << " <f0> " << _value << "\"" << std::endl;
    os << "shape = \"record\"" << std::endl;
    os << "];" << std::endl;
    // Now edges
    for (size_t level = 0; level < _nodeRefs.height(); ++level) {
        writeNode(os, suffix);
        os << ":f" << level + 1 << " -> ";
        _nodeRefs[level].pNode->writeNode(os, suffix);
        //        writeNode(os, suffix);
        //        os << ":f" << i + 1 << " [];" << std::endl;
        os << ":w" << level + 1 << " [];" << std::endl;
    }
    assert(_nodeRefs.height());
    if (_nodeRefs[0].pNode) {
        _nodeRefs[0].pNode->dotFile(os, suffix);
    }
}

#endif // INCLUDE_METHODS_THAT_USE_STREAMS

/************************** END: Node *******************************/


#endif
