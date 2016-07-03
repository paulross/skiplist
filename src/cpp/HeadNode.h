//
//  HeadNode.h
//  SkipList
//
//  Created by Paul Ross on 03/12/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//

#ifndef SkipList_HeadNode_h
#define SkipList_HeadNode_h

#include <vector>

#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
#include <sstream>
#endif // INCLUDE_METHODS_THAT_USE_STREAMS

#include "IntegrityEnums.h"

/************************** HeadNode *******************************
 * A HeadNode is a skip list. This is the single node leading to all
 * other nodes.
 *
 * Example:
 *  SkipList::HeadNode<double> sl;
 *  for (int i = 0; i < 100; ++i) sl.insert(i * 22.0 / 7.0);
 *  sl.size(); // 100
 *  sl.at(50); // Value of 50 pi
 *  sl.remove(sl.at(50)); // Remove 50 pi
 *
 */
template <typename T>
class HeadNode {
public:
    HeadNode() : _count(0) {}
    // Const methods
    //
    // Returns true if the value is present in the skip list.
    bool has(const T &value) const;
    // Returns the value at the index in the skip list.
    // Will throw a SkipList::IndexError if index out of range.
    const T &at(size_t index) const;
    // Find the value at index and write count values to dest.
    // Will throw a SkipList::IndexError if any index out of range.
    // This is useful for rolling median on even length lists where
    // the caller might want to implement the mean of two values.
    // Will throw a SkipList::IndexError if index or index + count is out of range.
    void at(size_t index, size_t count, std::vector<T> &dest) const;
    // Number of values in the skip list.
    size_t size() const { return _count; }

    // Non-const methods
    //
    // Insert a value.
    void insert(const T &value);
    // Remove a value. Will throw a SkipList::IndexError is value not present.
    void remove(const T &value);
    
    // Const methods that are mostly used for debugging and visualisation.
    //
    // Number of linked lists that are in the skip list.
    size_t height() const { return _nodeRefs.height(); }
    // Number of linked lists that the node at index has.
    // Will throw a SkipList::IndexError if idx out of range.
    size_t height(size_t idx) const;
    // The skip width of the node at index has.
    // May throw a SkipList::IndexError
    size_t width(size_t idx, size_t level) const;
    
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    void dotFile(std::ostream &os, size_t suffix = 0);
    void dotFileFinalise(std::ostream &os, size_t suffix);
#endif // INCLUDE_METHODS_THAT_USE_STREAMS
    
    // Returns non-zero if the integrity of this data structure is compromised
    // This is a thorough but expensive check!
    IntegrityCheck lacksIntegrity() const;
    // Estimate of the number of bytes used by the skip list
    size_t size_of() const;
    virtual ~HeadNode();
    
protected:
    void _adjRemoveRefs(size_t level, Node<T> *pNode);
    const Node<T> *_nodeAt(size_t idx) const;
    
protected:
    // Internal integrity checks
    IntegrityCheck _lacksIntegrityCyclicReferences() const;
    IntegrityCheck _lacksIntegrityWidthAccumulation() const;
    IntegrityCheck _lacksIntegrityNodeReferencesNotInList() const;

protected:
    // Number of nodes in the list
    size_t _count;
    // My node references, the size of this is the largest height in the list
    SwappableNodeRefStack<T> _nodeRefs;
    
private:
    // Prevent cctor and operator=
    HeadNode(const HeadNode &that);
    HeadNode &operator=(const HeadNode &that) const;
};

template <typename T>
bool HeadNode<T>::has(const T &value) const {
    for (size_t l = _nodeRefs.height(); l-- > 0;) {
        assert(_nodeRefs[l].pNode);
        if (_nodeRefs[l].pNode->has(value)) {
            return true;
        }
    }
    return false;
}

template <typename T>
const T &HeadNode<T>::at(size_t index) const {
    const Node<T> *pNode = _nodeAt(index);
    assert(pNode);
    return pNode->value();
}

template <typename T>
void HeadNode<T>::at(size_t index, size_t count, std::vector<T> &dest) const {
    dest.clear();
    const Node<T> *pNode = _nodeAt(index);
    // _nodeAt will (should) throw an IndexError so this assert should always be true
    assert(pNode);
    while (count) {
        if (! pNode) {
            _throw_exceeds_size(_count);
        }
        dest.push_back(pNode->value());
        pNode = pNode->next();
        --count;
    }
}

template <typename T>
size_t HeadNode<T>::height(size_t idx) const {
    const Node<T> *pNode = _nodeAt(idx);
    assert(pNode);
    return pNode->height();
}

template <typename T>
size_t HeadNode<T>::width(size_t idx, size_t level) const {
    const Node<T> *pNode = _nodeAt(idx);
    assert(pNode);
    if (level >= pNode->height()) {
        _throw_exceeds_size(pNode->height());
//        throw IndexError("level out of range.");
    }
    return pNode->nodeRefs()[level].width;
}

template <typename T>
const Node<T> *HeadNode<T>::_nodeAt(size_t idx) const {
    if (idx < _count) {
        for (size_t l = _nodeRefs.height(); l-- > 0;) {
            if (_nodeRefs[l].pNode && _nodeRefs[l].width <= idx + 1) {
                size_t new_index = idx + 1 - _nodeRefs[l].width;
                const Node<T> *pNode = _nodeRefs[l].pNode->at(new_index);
                if (pNode) {
                    return pNode;
                }
            }
        }
    }
    assert(idx >= _count);
    _throw_exceeds_size(_count);
    // Should not get here as _throw_exceeds_size() will always throw.
    return NULL;
}

template <typename T>
void HeadNode<T>::insert(const T &value) {
    Node<T> *pNode = nullptr;
    size_t level = _nodeRefs.height();
    while (level-- > 0) {
        assert(_nodeRefs[level].pNode);
        pNode = _nodeRefs[level].pNode->insert(value);
        if (pNode) {
            break;
        }
    }
    if (! pNode) {
        pNode = new Node<T>(value);
        level = 0;
    }
    assert(pNode);
    SwappableNodeRefStack<T> &thatRefs = pNode->nodeRefs();
    if (thatRefs.canSwap()) {
        
        // Expand this to that
        while (_nodeRefs.height() < thatRefs.height()) {
            _nodeRefs.push_back(nullptr, _count + 1);
        }
        if (level < thatRefs.swapLevel()) {
            // Happens when we were originally, say 3 high (max height of any
            // previously seen node). Then a node is created
            // say 5 high. In that case this will be at level 2 and
            // thatRefs.swapLevel() will be 3
            assert(level + 1 == thatRefs.swapLevel());
            thatRefs[thatRefs.swapLevel()].width += _nodeRefs[level].width;
            ++level;
        }
        // Now swap
        while (level < _nodeRefs.height() && thatRefs.canSwap()) {
            assert(thatRefs.canSwap());
            assert(level == thatRefs.swapLevel());
            _nodeRefs[level].width -= thatRefs[level].width - 1;
            thatRefs.swap(_nodeRefs);
            if (thatRefs.canSwap()) {
                assert(thatRefs[thatRefs.swapLevel()].width == 0);
                thatRefs[thatRefs.swapLevel()].width = _nodeRefs[level].width;
            }
            ++level;
        }
        // Check all references swapped
        assert(! thatRefs.canSwap());
        // Check that all 'this' pointers created on construction have been moved
        assert(thatRefs.noNodePointerMatches(pNode));
    }
    if (level < thatRefs.swapLevel()) {
        // Happens when we are, say 5 high then a node is created
        // and consumed by the next node say 3 high. In that case this will be
        // at level 2 and thatRefs.swapLevel() will be 3
        assert(level + 1 == thatRefs.swapLevel());
        ++level;
    }
    // Increment my widths as my references are now going over the top of
    // pNode.
    while (level < _nodeRefs.height() && level >= thatRefs.height()) {
        _nodeRefs[level++].width += 1;
    }
    ++_count;
}

/*
 * Adjust references for removal of the node.
 */
template <typename T>
void HeadNode<T>::_adjRemoveRefs(size_t level, Node<T> *pNode) {
    assert(pNode);
    SwappableNodeRefStack<T> &thatRefs = pNode->nodeRefs();
    
    // Swap all remaining levels
    // This assertion checks that if swapping can take place we must be at the
    // same level.
    assert(! thatRefs.canSwap() || level == thatRefs.swapLevel());
    while (level < _nodeRefs.height() && thatRefs.canSwap()) {
        assert(level == thatRefs.swapLevel());
        // Compute the new width for the new node
        thatRefs[level].width += _nodeRefs[level].width - 1;
        thatRefs.swap(_nodeRefs);
        ++level;
        if (! thatRefs.canSwap()) {
            delete pNode;
            pNode = nullptr;
            break;
        }
    }
    // Check either:
    // - either: node deleted here
    // - or: all references swapped (it is an existing node and the removed
    //  node has already been deleted).
    assert(pNode == nullptr || ! thatRefs.canSwap());
    // Decrement my widths as my references are now going over the top of
    // pNode.
    while (level < _nodeRefs.height()) {
        _nodeRefs[level++].width -= 1;
    }
    // Decrement my stack while top has a NULL pointer.
    while (_nodeRefs.height() && ! _nodeRefs[_nodeRefs.height() - 1].pNode) {
        _nodeRefs.pop_back();
    }
}

template <typename T>
void HeadNode<T>::remove(const T &value) {
    Node<T> *pNode = nullptr;
    size_t level;
    for (level = _nodeRefs.height(); level-- > 0;) {
        assert(_nodeRefs[level].pNode);
        pNode = _nodeRefs[level].pNode->remove(level, value);
        if (pNode) {
            break;
        }
    }
    if (! pNode) {
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
        std::ostringstream oss;
        oss << "Value " << value << " not found.";
        std::string err_msg = oss.str();
#else
        std::string err_msg = "Value not found.";
#endif
        throw ValueError(err_msg);
    }
    // Take swap level as some swaps will have been dealt with by the remove() above.
    _adjRemoveRefs(pNode->nodeRefs().swapLevel(), pNode);
    --_count;
}

/* This tests that at every level >= 0 the sequence of node pointers
 * at that level does not contain a cyclic reference.
 */
template <typename T>
IntegrityCheck HeadNode<T>::_lacksIntegrityCyclicReferences() const {
    assert(height());
    // Check for cyclic references at each level
    for (size_t level = 0; level < _nodeRefs.height(); ++level) {
        Node<T> *p1 = _nodeRefs[level].pNode;
        Node<T> *p2 = _nodeRefs[level].pNode;
        while (p1 && p2) {
            p1 = p1->nodeRefs()[level].pNode;
            if (p2->nodeRefs()[level].pNode) {
                p2 = p2->nodeRefs()[level].pNode->nodeRefs()[level].pNode;
            } else {
                p2 = nullptr;
            }
            if (p1 && p2 && p1 == p2) {
                return HEADNODE_DETECTS_CYCLIC_REFERENCE;
            }
        }
    }
    return INTEGRITY_SUCCESS;
}

/* This tests that at every level > 0 the node to node width is the same
 * as the accumulated node to node widths at level - 1.
 */
template <typename T>
IntegrityCheck HeadNode<T>::_lacksIntegrityWidthAccumulation() const {
    assert(height());
    for (size_t level = 1; level < _nodeRefs.height(); ++level) {
        const Node<T> *pl = _nodeRefs[level].pNode;
        const Node<T> *pl_1 = _nodeRefs[level - 1].pNode;
        assert(pl && pl_1); // No nulls allowed in HeadNode
        size_t wl = _nodeRefs[level].width;
        size_t wl_1 = _nodeRefs[level - 1].width;
        while (true) {
            while (pl != pl_1) {
                assert(pl_1); // Could only happen if a lower reference was NULL and the higher non-NULL.
                wl_1 += pl_1->width(level - 1);
                pl_1 = pl_1->pNode(level - 1);
            }
            if (wl != wl_1) {
                return HEADNODE_LEVEL_WIDTHS_MISMATCH;
            }
            if (pl == nullptr && pl_1 == nullptr) {
                break;
            }
            wl = pl->width(level);
            wl_1 = pl_1->width(level - 1);
            pl = pl->pNode(level);
            pl_1 = pl_1->pNode(level - 1);
        }
    }
    return INTEGRITY_SUCCESS;
}

template <typename T>
IntegrityCheck HeadNode<T>::_lacksIntegrityNodeReferencesNotInList() const {
    assert(height());

    IntegrityCheck result;
    std::set<const Node<T>*> nodeSet;
    const Node<T> *pNode = _nodeRefs[0].pNode;
    assert(pNode);
    
    // First gather all nodes, slightly awkward code here is so that
    // NULL is always included.
    nodeSet.insert(pNode);
    do {
        pNode = pNode->next();
        nodeSet.insert(pNode);
    } while (pNode);
    assert(nodeSet.size() == _count + 1); // All nodes plus NULL
    // Then test each node does not have pointers that are not in nodeSet
    pNode = _nodeRefs[0].pNode;
    while (pNode) {
        result = pNode->lacksIntegrityRefsInSet(nodeSet);
        if (result) {
            return result;
        }
        pNode = pNode->next();
    }
    
    return INTEGRITY_SUCCESS;
}


template <typename T>
IntegrityCheck HeadNode<T>::lacksIntegrity() const {
    if (_nodeRefs.height()) {
        IntegrityCheck result = _nodeRefs.lacksIntegrity();
        if (result) {
            return result;
        }
        if (! _nodeRefs.noNodePointerMatches(nullptr)) {
            return HEADNODE_CONTAINS_NULL;
        }
        // Check all nodes for integrity
        const Node<T> *pNode = _nodeRefs[0].pNode;
        while (pNode) {
            result = pNode->lacksIntegrity(height());
            if (result) {
                return result;
            }
            pNode = pNode->next();
        }
        // Check count against total number of nodes
        pNode = _nodeRefs[0].pNode;
        size_t total = 0;
        while (pNode) {
            total += pNode->nodeRefs()[0].width;
            pNode = pNode->next();
        }
        if (total != _count) {
            return HEADNODE_COUNT_MISMATCH;
        }
        result = _lacksIntegrityWidthAccumulation();
        if (result) {
            return result;
        }
        result = _lacksIntegrityCyclicReferences();
        if (result) {
            return result;
        }
        result = _lacksIntegrityNodeReferencesNotInList();
        if (result) {
            return result;
        }
    }
    return INTEGRITY_SUCCESS;
}

// Returns an estimate of the memory usage of an instance
template <typename T>
size_t HeadNode<T>::size_of() const {
    // sizeof(*this) includes the size of _nodeRefs but _nodeRefs.size_of()
    // includes sizeof(_nodeRefs) so we need to subtract to avoid double counting
    size_t ret_val = sizeof(*this) + _nodeRefs.size_of() - sizeof(_nodeRefs);
    if (_nodeRefs.height()) {
        const Node<T> *node = _nodeRefs[0].pNode;
        while (node) {
            ret_val += node->size_of();
            node = node->next();
        }
    }
    return ret_val;
}

template <typename T>
HeadNode<T>::~HeadNode() {
    if (_nodeRefs.height()) {
        // Traverse the lowest level list iteratively deleting as we go
        // Doing this recursivley could be expensive as we are at level 0.
        const Node<T> *node = _nodeRefs[0].pNode;
        const Node<T> *next;
        while (node) {
            next = node->next();
            delete node;
            --_count;
            node = next;
        }
    }
    assert(_count == 0);
}

#ifdef INCLUDE_METHODS_THAT_USE_STREAMS

template <typename T> void HeadNode<T>::dotFile(std::ostream &os, size_t suffix) {
    if (suffix == 0) {
        os << "digraph SkipList {" << std::endl;
        os << "label = \"SkipList.\"" << std::endl;
        os << "graph [rankdir = \"LR\"];" << std::endl;
        os << "node [fontsize = \"12\" shape = \"ellipse\"];" << std::endl;
        os << "edge [];" << std::endl;
        os << std::endl;
    }
    os << "subgraph cluster" << suffix << " {" << std::endl;
    os << "style=dashed" << std::endl;
    os << "label=\"Skip list iteration " << suffix << "\"" << std::endl;
    os << "\"HeadNode" << suffix;
    os << "\" [" << std::endl;
    os << "label = \"";
    // Write out the fields
    if (_nodeRefs.height()) {
        for (size_t i = 0; i < _nodeRefs.height(); ++i) {
            if (i > 0) {
                os << " | ";
            }
            os << "{ " << _nodeRefs[i].width << " | ";
            os << "<f" << i << "> ";
            os << std::hex << _nodeRefs[i].pNode << std::dec;
            os << "}";
        }
    } else {
        os << "Empty HeadNode";
    }
    os << "\"" << std::endl;
    os << "shape = \"record\"" << std::endl;
    os << "];" << std::endl;
    if (_nodeRefs.height()) {
        // NULL, the sentinal node
        os << "\"node";
        os << suffix;
        os << "0x0\" [label = \"";
        for (size_t level = 0; level < _nodeRefs.height(); ++level) {
            if (level) {
                os << " | ";
            }
            os << "<w" << level + 1 << "> NULL";
        }
        os << "\" shape = \"record\"];" << std::endl;
    }
    // Edges for head node
    for (size_t level = 0; level < _nodeRefs.height(); ++level) {
        os << "\"HeadNode";
        os << suffix;
        os << "\":f" << level << " -> ";
        _nodeRefs[level].pNode->writeNode(os, suffix);
        os << ":w" << level + 1 << " [];" << std::endl;
    }
    // Now all nodes via level 0, if non-empty
    if (_nodeRefs.height()) {
        Node<T> *pNode = this->_nodeRefs[0].pNode;
        pNode->dotFile(os, suffix);
    }
    // End: "subgraph cluster1 {"
    os << "}" << std::endl;
    os << std::endl;
}

template <typename T> void HeadNode<T>::dotFileFinalise(std::ostream &os,
                                                        size_t suffix) {
    if (suffix > 0) {
        // Link the nodes together with an invisible node.
        //    node0 [shape=record, label = "<f0> | <f1> | <f2> | <f3> | <f4> | <f5> | <f6> | <f7> | <f8> | ",
        //           style=invis,
        //           width=0.01];
        os << "node0 [shape=record, label = \"";
        for (size_t i = 0; i < suffix; ++i) {
            os << "<f" << i << "> | ";
        }
        os << "\", style=invis, width=0.01];" << std::endl;
        // Now:
        //    node0:f0 -> HeadNode [style=invis];
        //    node0:f1 -> HeadNode1 [style=invis];
        for (size_t i = 0; i < suffix; ++i) {
            os << "node0:f" << i << " -> HeadNode" << i;
            os << " [style=invis];" << std::endl;
        }
    }
    os << "}" << std::endl;
}

#endif // INCLUDE_METHODS_THAT_USE_STREAMS

/************************** END: HeadNode *******************************/

#endif // SkipList_HeadNode_h
