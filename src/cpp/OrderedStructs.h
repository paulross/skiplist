//
//  OrderedStructs.h
//  skiplist
//
//  This is just common C++/CPython code used by all ordered structures.
//  We only have the SkipList at the moment however.
//
//  Created by Paul Ross on 30/09/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#ifndef skiplist_OrderedStructs_h
#define skiplist_OrderedStructs_h

#include <Python.h>
#include <string>

/* Roll our own RTTI */
enum KeyDataType {
    TYPE_ZERO = 0,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_BYTES,
    TYPE_OBJECT,
    /* Insert new enumerated values above this comment. */
    TYPE_OVERFLOW
};

/* Define these as an attempt to reduce C&P errors in very similar code. */
typedef long long   TYPE_TYPE_LONG;
typedef double      TYPE_TYPE_DOUBLE;
/* Note: We use a std::string but store bytes in it.
 * So we have to be careful not to treat data as a NTS.
 */
typedef std::string TYPE_TYPE_BYTES;
typedef PyObject* TYPE_TYPE_OBJECT;

#define ASSERT_TYPE_IN_RANGE assert(self->_data_type > TYPE_ZERO && self->_data_type < TYPE_OVERFLOW)

/* Returns a std::string from the contents of a bytes object.
 * This includes any '\0' characters in the content.
 */
std::string bytes_as_std_string(PyObject *arg);
/* Returns a new PyBytesObject from a std::string.
 * This includes any '\0' characters in the content.
 */
PyObject *std_string_as_bytes(const std::string &str);


#endif
