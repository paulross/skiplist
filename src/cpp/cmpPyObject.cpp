//
//  cmpPyObject.cpp
//  skiplist
//
//  Created by Paul Ross on 04/10/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <stdio.h>
#include <stdexcept>

#include "cmpPyObject.h"

bool cmpPyObject::operator()(PyObject *a, PyObject *b) const {
    // Try a cheap optimisation, PyObject_RichCompareBool does not do this for Py_LT
    if (a == b) {
        return false;
    }
    int result = PyObject_RichCompareBool(a, b, Py_LT);
//    fprintf(stdout, "Comparing %p (", a);
//    PyObject_Print(a, stdout, 0);
//    fprintf(stdout, ") with %p (", b);
//    PyObject_Print(b, stdout, 0);
//    fprintf(stdout, ") gives %d\n", result);
    if (result < 0) {
        // TODO: Make this more informative, when does this fail?
        throw std::invalid_argument("PyObject_RichCompareBool() returns -1");
    }
    return result != 0;
}
