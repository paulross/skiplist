/**
 * @file
 *
 * Project: skiplist
 *
 * Created by Paul Ross on 04/10/2016.
 *
 * Copyright (c) 2016-2023 Paul Ross. All rights reserved.
 *
 * @code
 * MIT License
 *
 * Copyright (c) 2016-2023 Paul Ross
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @endcode
 */

#ifndef __skiplist__cmpPyObject__
#define __skiplist__cmpPyObject__

#include "Python.h"
#include <functional>

/**
 * Python 3.8 changed how lambda refcounts were managed and our tests with
 * lambdas were segfaulting.
 *
 * The tests with normal functions worked fine.
 * This was because we were not:
 *
 * - Incrementing the reference count in the ctor.
 * - Decrementing the reference count in the dtor (we hadn't one).
 * - Adjusting the reference counts correctly in the default copy constructor.
 * - Implementing the default copy constructor that manages reference counts.
 *
 * We must of got away with it up to now as previous versions of Python treated
 * lambdas differently.
 */
#define SKIPLIST_TRACE_CMPPYOBJECT_REF_COUNTS 0

/**
 * @brief A wrapper around a CPython comparison function.
 */
class cmpPyObject {
public:
    /**
     * Constructor.
     *
     * @param cmp The CPython comparison function.
     */
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
    // Compare two objects.
    bool operator()(PyObject *a, PyObject *b) const;
    /// Destructor.
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
    /// Copying needs to manage the refcount of the cmp_func.
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
    /** Not implemented. */
    cmpPyObject &operator=(const cmpPyObject &rhs);
private:
    /**
     * Make a copy managing the reference count.
     *
     * @param rhs
     */
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
    /// The CPython comparison function.
    PyObject *cmp_func;
};

#endif /* defined(__skiplist__cmpPyObject__) */
