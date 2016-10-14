//
//  cmpPyObject.h
//  skiplist
//
//  Created by Paul Ross on 04/10/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#ifndef __skiplist__cmpPyObject__
#define __skiplist__cmpPyObject__

#include "Python.h"
#include <functional>

struct cmpPyObject {
    bool operator()(PyObject *a, PyObject *b) const;
};

#endif /* defined(__skiplist__cmpPyObject__) */
