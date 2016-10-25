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

class cmpPyObject {
public:
    cmpPyObject(PyObject *cmp=NULL) : cmp_func(cmp) {}
    bool operator()(PyObject *a, PyObject *b) const;
private:
    PyObject *cmp_func;
};

#endif /* defined(__skiplist__cmpPyObject__) */
