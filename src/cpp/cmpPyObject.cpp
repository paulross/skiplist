//
//  cmpPyObject.cpp
//  skiplist
//
//  Created by Paul Ross on 04/10/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <stdexcept>
#include <sstream>

#include "cmpPyObject.h"

bool cmpPyObject::operator()(PyObject *a, PyObject *b) const {
    // Try a cheap optimisation since PyObject_RichCompareBool does not
    // do this for Py_LT
    if (a == b) {
        return false;
    }
    int result = PyObject_RichCompareBool(a, b, Py_LT);
    if (result < 0) {
        std::stringstream ostr;
        ostr << "Skip list: Can not compare \"" << Py_TYPE(a)->tp_name;
        ostr << "\" object with \"" << Py_TYPE(b)->tp_name << "\" object.";
        throw std::invalid_argument(ostr.str());
    }
    return result != 0;
}
