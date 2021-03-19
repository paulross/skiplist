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


/* Python 3.8 changed how lambda refcounts were managed and our tests with
 * lambdas were segfaulting.
 * The tests with normal functions worked fine.
 * This was because we were not:
 * - Incrementing the reference count in the ctor.
 * - Deccrementing the reference count in the dtor (we hadn't one).
 * - Adjusting the reference counts correctly in the default copy constructor.
 * - Implementing the default copy constructor that manages reference counts.
 *
 * We must of got away worth it up to now as previous versions of Python treated
 * lambdas differently.
 */
#define SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS 0

class cmpPyObject {
public:
    cmpPyObject(PyObject *cmp=NULL) : cmp_func(cmp) {
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
        printf("ctor this: %p cmp_func %p", this, cmp_func);
        if (cmp_func) {
            printf(" ref count was=%ld", cmp_func->ob_refcnt);
            Py_XINCREF(cmp_func);
            int err = PyObject_Print(cmp_func, stdout, Py_PRINT_RAW);
            printf(" err=%d ref count now=%ld", err, cmp_func->ob_refcnt);
        } else {
            printf("NULL");
        }
        printf("\n");
#else
        Py_XINCREF(cmp_func);
#endif
    }
    bool operator()(PyObject *a, PyObject *b) const;
    ~cmpPyObject() {
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
        printf("dtor this: %p cmp_func %p", this, cmp_func);
        if (cmp_func) {
            printf(" ref count was=%ld", cmp_func->ob_refcnt);
            Py_XDECREF(cmp_func);
            int err = PyObject_Print(cmp_func, stdout, Py_PRINT_RAW);
            printf(" err=%d ref count now=%ld", err, cmp_func->ob_refcnt);
        } else {
            printf("NULL");
        }
        printf("\n");
#else
        Py_XDECREF(cmp_func);
#endif
    }
    // Copying needs to manage the refcount of the cmp_func.
    cmpPyObject(const cmpPyObject &rhs) {
        if (&rhs != this) {
            cmp_func = NULL;
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
            printf("cctor this: %p rhs %p cmp_func %p", this, &rhs, cmp_func);
#endif
            copy_cmpPyObject(rhs);
        }
    }
private:
    /* Not implemented. */
    cmpPyObject &operator=(const cmpPyObject &rhs);
private:
    void copy_cmpPyObject(const cmpPyObject &rhs) {
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
        if (cmp_func) {
            printf(" ref count was=%ld ", cmp_func->ob_refcnt);
            int err = PyObject_Print(cmp_func, stdout, Py_PRINT_RAW);
            Py_XDECREF(cmp_func);
            printf(" err=%d ref count now=%ld", err, cmp_func->ob_refcnt);
        } else {
            printf(" NULL");
        }
#else
        Py_XDECREF(cmp_func);
#endif
        cmp_func = rhs.cmp_func;
#if SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS
        if (cmp_func) {
            printf(" ref count was=%ld ", cmp_func->ob_refcnt);
            int err = PyObject_Print(cmp_func, stdout, Py_PRINT_RAW);
            Py_INCREF(cmp_func);
            printf(" err=%d ref count now=%ld", err, cmp_func->ob_refcnt);
        } else {
            printf(" NULL");
        }
        printf("\n");
#else
        Py_XINCREF(cmp_func);
#endif
    }
private:
    PyObject *cmp_func;
};

#endif /* defined(__skiplist__cmpPyObject__) */
