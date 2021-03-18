//
//  cmpPyObject.cpp
//  skiplist
//
//  Created by Paul Ross on 04/10/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <assert.h>
#include <sstream>
#include <stdexcept>

#include "cmpPyObject.h"

bool cmpPyObject::operator()(PyObject *a, PyObject *b) const {
    bool ret_val;
    long result;
    assert(a);
    assert(b);

    if (cmp_func) {
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
        printf("cmp_func: %p", this);
        PyObject_Print(cmp_func, stdout, Py_PRINT_RAW);
        printf(" ref count=%ld", cmp_func->ob_refcnt);
        printf("\n");
        printf("a: ");
        PyObject_Print(a, stdout, Py_PRINT_RAW);
        printf("\n");
        printf("b: ");
        PyObject_Print(b, stdout, Py_PRINT_RAW);
        printf("\n");
#endif
        PyObject *py_result = PyObject_CallFunctionObjArgs(cmp_func, a, b, NULL);
        if (! py_result) {
            throw std::invalid_argument("Skip list: Compare function fails.");
        }
        // result will be 0, 1 or -1 on failure
        result = PyObject_IsTrue(py_result);
        Py_DECREF(py_result);
    } else {
        // Try a cheap optimisation since PyObject_RichCompareBool does not
        // do this for Py_LT
        if (a == b) {
            return false;
        }
        result = PyObject_RichCompareBool(a, b, Py_LT);
    }
    if (result < 0) {
        std::stringstream ostr;
        ostr << "Skip list: Can not compare \"" << Py_TYPE(a)->tp_name;
        ostr << "\" object with \"" << Py_TYPE(b)->tp_name << "\" object.";
        throw std::invalid_argument(ostr.str());
    }
    ret_val = result != 0;
    return ret_val;
}
