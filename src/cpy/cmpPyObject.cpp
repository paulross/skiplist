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

#include <assert.h>
#include <sstream>
#include <stdexcept>

#include "cmpPyObject.h"

/**
 * Compare two CPython objects.
 *
 * This calls PyObject_CallFunctionObjArgs() if the comparison function is non-NULL.
 *
 * This calls PyObject_RichCompareBool() if the comparison function is NULL.
 *
 * @param a
 * @param b
 * @return true if the objects compare equal.
 */
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
