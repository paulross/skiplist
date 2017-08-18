//
//  IntegrityEnums.h
//  SkipList
//
//  Created by Paul Ross on 11/12/2015.
//  Copyright (c) 2017 Paul Ross. All rights reserved.
//

#ifndef SkipList_IntegrityEnums_h
#define SkipList_IntegrityEnums_h

enum IntegrityCheck {
    INTEGRITY_SUCCESS = 0,
    // SwappableNodeRefStack integrity checks
    NODEREFS_WIDTH_ZERO_NOT_UNITY = 100,
    NODEREFS_WIDTH_DECREASING,
    // Node integrity checks
    NODE_HEIGHT_ZERO = 200,
    NODE_HEIGHT_EXCEEDS_HEADNODE,
    NODE_NON_NULL_AFTER_NULL,
    NODE_SELF_REFERENCE,
    NODE_REFERENCES_NOT_IN_GLOBAL_SET,
    // HeadNode integrity checks
    HEADNODE_CONTAINS_NULL = 300,
    HEADNODE_COUNT_MISMATCH,
    HEADNODE_LEVEL_WIDTHS_MISMATCH,
    HEADNODE_DETECTS_CYCLIC_REFERENCE,
    HEADNODE_DETECTS_OUT_OF_ORDER,
};

#endif
