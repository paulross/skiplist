//
//  OrderedStructs.cpp
//  skiplist
//
//  Created by Paul Ross on 30/09/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include "OrderedStructs.h"

/* Returns a std::string from the contents of a bytes object.
 * This includes any '\0' characters in the content.
 */
std::string bytes_as_std_string(PyObject *arg) {
    assert(PyBytes_Check(arg));
    return std::string(PyBytes_AS_STRING(arg), PyBytes_GET_SIZE(arg));
}

/* Returns a new PyBytesObject from a std::string.
 * This includes any '\0' characters in the content.
 */
PyObject *std_string_as_bytes(const std::string &str) {
    return PyBytes_FromStringAndSize(str.c_str(), str.size());
}

