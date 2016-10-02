//
//  cSkipList.cpp
//  SkipList
//
//  Created by Paul Ross on 20/11/2015.
//  Copyright (c) 2015 AHL. All rights reserved.
//
#include <Python.h>
#include "structmember.h"

#define INCLUDE_METHODS_THAT_USE_STREAMS // We want the DOT output.
#include "SkipList.h"


#include <string>
#include <iostream>

#include "OrderedStructs.h"
#include "cOrderedStructs.h"

typedef struct {
    PyObject_HEAD
    enum KeyDataType _data_type;
    union {
        /* NULL/non-NULL pointer, not to be de-referrenced. */
        ManAHL::SkipList::HeadNode<void>             *pSl_void;
        ManAHL::SkipList::HeadNode<TYPE_TYPE_LONG>   *pSl_long;
        ManAHL::SkipList::HeadNode<TYPE_TYPE_DOUBLE> *pSl_double;
        ManAHL::SkipList::HeadNode<TYPE_TYPE_BYTES>  *pSl_bytes;
    };
} SkipList;

static PyObject *
SkipList_new(PyTypeObject *type, PyObject */* args */, PyObject */* kwargs */) {
    SkipList *self = NULL;

    self = (SkipList *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_data_type = TYPE_ZERO;
        self->pSl_void = NULL;
    }
    return (PyObject *)self;
}

static int
SkipList_init(SkipList *self, PyObject *args, PyObject *kwargs) {
    int ret_val = -1;
    PyObject *value_type = NULL;
    static char *kwlist[] = { (char *)"value_type", NULL};

    assert(self);
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "O:__init__",
                                      kwlist, &value_type)) {
        goto except;
    }
    assert(value_type);
    if (! PyType_CheckExact(value_type)) {
        PyErr_Format(PyExc_ValueError,
                     "Argument to __init__ must be type object not \"%s\"",
                     Py_TYPE(value_type)->tp_name);
        goto except;
    }
    if ((PyTypeObject *)value_type == &PyLong_Type) {
        self->_data_type = TYPE_LONG;
        self->pSl_long = new ManAHL::SkipList::HeadNode<TYPE_TYPE_LONG>();
    } else if ((PyTypeObject *)value_type == &PyFloat_Type) {
        self->_data_type = TYPE_DOUBLE;
        self->pSl_double = new ManAHL::SkipList::HeadNode<TYPE_TYPE_DOUBLE>();
    } else if ((PyTypeObject *)value_type == &PyBytes_Type) {
        self->_data_type = TYPE_BYTES;
        self->pSl_bytes = new ManAHL::SkipList::HeadNode<TYPE_TYPE_BYTES>();
    } else {
        PyErr_Format(PyExc_ValueError,
            "Argument to __init__ must be long, float or bytes, not \"%s\"",
                     ((PyTypeObject *)value_type)->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    assert(self);
    assert(self->pSl_void);
    ret_val = 0;
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(self);
    ret_val = -1;
finally:
    return ret_val;
}

static void
SkipList_dealloc(SkipList* self)
{
    if (self && self->pSl_void) {
        switch (self->_data_type) {
            case TYPE_LONG:
                delete self->pSl_long;
                break;
            case TYPE_DOUBLE:
                delete self->pSl_double;
                break;
            case TYPE_BYTES:
                delete self->pSl_bytes;
                break;
            default:
                PyErr_BadInternalCall();
                break;
        }
        Py_TYPE(self)->tp_free((PyObject*)self);
    }
}

static PyMemberDef SkipList_members[] = {
    {NULL, 0, 0, 0, NULL}  /* Sentinel */
};

static PyObject *
SkipList_has(SkipList* self, PyObject *arg)
{
    PyObject *ret_val = NULL;
    std::string str;
    
    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to has() must be long not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            ret_val = PyBool_FromLong(
                            self->pSl_long->has(PyLong_AsLongLong(arg))
                                      );
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to has() must be float not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            
            try {
                ret_val = PyBool_FromLong(
                    self->pSl_double->has(PyFloat_AS_DOUBLE(arg)));
            } catch (ManAHL::SkipList::FailedComparison &err) {
                /* This will happen if arg is a NaN. */
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_BYTES:
            if (! PyBytes_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to has() must be bytes not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            ret_val = PyBool_FromLong(self->pSl_bytes->has(
                                            bytes_as_std_string(arg)
                                                           ));
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    assert(! PyErr_Occurred());
    assert(ret_val);
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(ret_val);
finally:
    return ret_val;
}

/* Sets value to the size of the skiplist as a size_t.
 * Returns 0 on success, non-zero on failure. */
static size_t
_size(SkipList* self, Py_ssize_t &value) {
    int err_code = 0;

    ASSERT_TYPE_IN_RANGE;
    assert(self && self->pSl_void);
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            value = self->pSl_long->size();
            break;
        case TYPE_DOUBLE:
            value = self->pSl_double->size();
            break;
        case TYPE_BYTES:
            value = self->pSl_bytes->size();
            break;
        default:
            err_code = -1;
            break;
    }
    return err_code;
}

/* Checks an index against a size and sets an IndexError if out of range.
 * Returns 0 if index in range, non-zero otherwise.
 */
static int
_check_index_against_size(const char *prefix, long index, Py_ssize_t size) {
    if ((index < 0 && index + size < 0) || (index >= 0 && index >= size)) {
        if (index > 0) {
            PyErr_Format(PyExc_IndexError,
                         "%s %ld out of range 0 <= index < %ld",
                         prefix, index, size);
        } else {
            PyErr_Format(PyExc_IndexError,
                         "%s %ld out of range %ld < index <= -1",
                         prefix, index, -(size + 1));
        }
        return -1;
    }
    return 0;
}

static PyObject *
SkipList_at(SkipList *self, PyObject *arg)
{
    PyObject *ret_val = NULL;
    Py_ssize_t size;
    long long index = 0;

    assert(self && self->pSl_void);
    assert(! PyErr_Occurred());
    
#if PY_MAJOR_VERSION == 3
    if (! PyLong_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Index of skip list must be type \"long\" not type \"%s\"",
                     arg->ob_type->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    index = PyLong_AsLongLong(arg);
    // Check for failure
    if (index == -1 && PyErr_Occurred()) {
        goto except;
    }
#else
    if (! PyLong_Check(arg) && ! PyInt_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Index of skip list must be type \"long\" or \"int\" not type \"%s\"",
                     arg->ob_type->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    if (PyLong_Check(arg)) {
        index = PyLong_AsLongLong(arg);
        // Check for failure
        if (index == -1 && PyErr_Occurred()) {
            goto except;
        }
    } else if (PyInt_Check(arg)) {
        index = PyInt_AsLong(arg);
        // Check for failure
        if (index == -1 && PyErr_Occurred()) {
            goto except;
        }
    }
#endif
    if (_size(self, size)) {
        PyErr_BadInternalCall();
        goto except;
    }
    if (_check_index_against_size("Index", index, size)) {
        assert(PyErr_Occurred());
        goto except;
    }
    if (index < 0) {
        index += size;
    }
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromLongLong(self->pSl_long->at(index));
            break;
        case TYPE_DOUBLE:
            ret_val = PyFloat_FromDouble(self->pSl_double->at(index));
            break;
        case TYPE_BYTES:
            ret_val = std_string_as_bytes(self->pSl_bytes->at(index));
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    if (! ret_val) {
        goto except;
    }
    assert(! PyErr_Occurred());
    assert(ret_val);
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(ret_val);
finally:
    return ret_val;
}

static PyObject*
_at_sequence_long(SkipList *self, Py_ssize_t index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_LONG> dest;

    assert(self && self->pSl_void);
    assert(self->_data_type == TYPE_LONG);
    assert(! PyErr_Occurred());
    
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        self->pSl_long->at(index, count, dest);
    } catch (ManAHL::SkipList::IndexError &err) {
        PyErr_SetString(PyExc_IndexError, err.message().c_str());
        return NULL;
    }
    assert(dest.size() == count);
    for (Py_ssize_t i = 0; i < count; ++i) {
        PyTuple_SET_ITEM(ret_val, i, PyLong_FromLongLong(dest[i]));
    }
    return ret_val;
}

static PyObject*
_at_sequence_double(SkipList *self, Py_ssize_t index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_DOUBLE> dest;

    assert(self && self->pSl_void);
    assert(self->_data_type == TYPE_DOUBLE);
    assert(! PyErr_Occurred());
    
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        self->pSl_double->at(index, count, dest);
    } catch (ManAHL::SkipList::IndexError &err) {
        PyErr_SetString(PyExc_IndexError, err.message().c_str());
        return NULL;
    }
    assert(dest.size() == count);
    for (Py_ssize_t i = 0; i < count; ++i) {
        PyTuple_SET_ITEM(ret_val, i, PyFloat_FromDouble(dest[i]));
    }
    return ret_val;
}

static PyObject*
_at_sequence_bytes(SkipList *self, Py_ssize_t index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_BYTES> dest;

    assert(self && self->pSl_void);
    assert(self->_data_type == TYPE_BYTES);
    assert(! PyErr_Occurred());
    
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        self->pSl_bytes->at(index, count, dest);
    } catch (ManAHL::SkipList::IndexError &err) {
        PyErr_SetString(PyExc_IndexError, err.message().c_str());
        return NULL;
    }
    assert(dest.size() == count);
    for (Py_ssize_t i = 0; i < count; ++i) {
        PyTuple_SET_ITEM(ret_val, i, std_string_as_bytes(dest[i]));
    }
    return ret_val;
}


/* Returns a tuple of 'count' PyObjects starting at 'index'. */
static PyObject *
SkipList_at_sequence(SkipList *self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret = NULL;
    Py_ssize_t index;
    Py_ssize_t count;
    static char *kwlist[] = {
        (char *) "index", /* Integer, negative values are from the end. */
        (char *) "count", /* Integer, must be positive. */
        NULL
    };

    assert(self && self->pSl_void);
    assert(! PyErr_Occurred());
    
    /* Check input and compute real index. */
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "nn:at_seq",
                                      kwlist, &index, &count)) {
        assert(PyErr_Occurred());
        goto except;
    }
    if (count < 0) {
        PyErr_Format(PyExc_IndexError, "Count %zd must be positive", count);
        goto except;
    }
    Py_ssize_t size;
    if (_size(self, size)) {
        PyErr_BadInternalCall();
        goto except;
    }
    if (_check_index_against_size("Index", index, size)) {
        goto except;
    }
    if (index < 0) {
        index += size;
    }
    if ((count + index) > size) {
        PyErr_Format(PyExc_IndexError, "Count %zd from index %zd exceeds size %zd", count, index, size);
        goto except;
    }
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            ret = _at_sequence_long(self, index, count);
            if (! ret) {
                goto except;
            }
            break;
        case TYPE_DOUBLE:
            ret = _at_sequence_double(self, index, count);
            if (! ret) {
                goto except;
            }
            break;
        case TYPE_BYTES:
            ret = _at_sequence_bytes(self, index, count);
            if (! ret) {
                goto except;
            }
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    assert(! PyErr_Occurred());
    assert(ret);
    goto finally;
except:
    Py_XDECREF(ret);
    ret = NULL;
finally:
    return ret;
}

static PyObject *
SkipList_index(SkipList* self, PyObject *arg)
{
    PyObject *ret_val = NULL;
    std::string str;
    
    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to index() must be long not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            try {
                ret_val = PyLong_FromSize_t(
                            self->pSl_long->index(PyLong_AsLongLong(arg))
                                            );
            } catch (ManAHL::SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                goto except;
            }
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to index() must be float not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            try {
                ret_val = PyLong_FromSize_t(
                        self->pSl_double->index(PyFloat_AS_DOUBLE(arg))
                                            );
            } catch (ManAHL::SkipList::FailedComparison &err) {
                /* This will happen if arg is a NaN. */
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                goto except;
            } catch (ManAHL::SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                goto except;
            }
            break;
        case TYPE_BYTES:
            if (! PyBytes_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Argument to index() must be bytes not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                goto except;
            }
            try {
                ret_val = PyLong_FromSize_t(
                        self->pSl_bytes->index(bytes_as_std_string(arg))
                                            );
            } catch (ManAHL::SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                goto except;
            }
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    assert(! PyErr_Occurred());
    assert(ret_val);
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(ret_val);
finally:
    return ret_val;
}

static PyObject *
SkipList_size(SkipList* self)
{
    PyObject *ret_val = NULL;

    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->pSl_long->size());
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->pSl_double->size());
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->pSl_bytes->size());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
SkipList_height(SkipList* self)
{
    PyObject *ret_val = NULL;

    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->pSl_long->height());
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->pSl_double->height());
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->pSl_bytes->height());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
SkipList_insert(SkipList *self, PyObject *arg)
{
    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be long not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            self->pSl_long->insert(PyLong_AsLongLong(arg));
            if (PyErr_Occurred()) {
                return NULL;
            }
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be float not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                self->pSl_double->insert(PyFloat_AS_DOUBLE(arg));
            } catch (ManAHL::SkipList::FailedComparison &err) {
                /* This will happen if arg is a NaN. */
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_BYTES:
            if (! PyBytes_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be bytes not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            self->pSl_bytes->insert(bytes_as_std_string(arg));
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    Py_RETURN_NONE;
}

static PyObject *
SkipList_remove(SkipList* self, PyObject *arg)
{
    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    TYPE_TYPE_LONG value;
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be long not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            value = PyLong_AsLongLong(arg);
            if (PyErr_Occurred()) {
                return NULL;
            }
            try {
                self->pSl_long->remove(value);
            } catch (ManAHL::SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be float not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                self->pSl_double->remove(PyFloat_AS_DOUBLE(arg));
            } catch (ManAHL::SkipList::ValueError &err) {
                // For whatever reason PyErr_Format does not support doubles
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_BYTES:
            if (! PyBytes_Check(arg)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be bytes not \"%s\" type",
                             Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                self->pSl_bytes->remove(bytes_as_std_string(arg));
            } catch (ManAHL::SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            if (PyErr_Occurred()) {
                return NULL;
            }
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    Py_RETURN_NONE;
}

#ifdef INCLUDE_METHODS_THAT_USE_STREAMS

static PyObject *
SkipList_dot_file(SkipList* self)
{
    std::stringstream ostr;

    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            self->pSl_long->dotFile(ostr);
            self->pSl_long->dotFileFinalise(ostr);
            break;
        case TYPE_DOUBLE:
            self->pSl_double->dotFile(ostr);
            self->pSl_double->dotFileFinalise(ostr);
            break;
        case TYPE_BYTES:
            self->pSl_bytes->dotFile(ostr);
            self->pSl_bytes->dotFileFinalise(ostr);
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return PyBytes_FromString(ostr.str().c_str());
}

#endif

static PyObject *
SkipList_lacks_integrity(SkipList* self)
{
    PyObject *ret_val = NULL;

    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->pSl_long->lacksIntegrity());
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->pSl_double->lacksIntegrity());
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->pSl_bytes->lacksIntegrity());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
SkipList_node_height(SkipList* self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret_val = NULL;
    int index;
    Py_ssize_t size;
    static char *kwlist[] = {
        (char *) "index", /* Integer, negative values are from the end. */
        NULL
    };
    
    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    if (! PyArg_ParseTupleAndKeywords(args, kwargs,
                                      "i:node_height", kwlist, &index)) {
        assert(PyErr_Occurred());
        goto except;
    }
    if (_size(self, size)) {
        PyErr_BadInternalCall();
        goto except;
    }
    if (_check_index_against_size("Index", index, size)) {
        goto except;
    }
    if (index < 0) {
        index += size;
    }
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->pSl_long->height(index));
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->pSl_double->height(index));
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->pSl_bytes->height(index));
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    assert(! PyErr_Occurred());
    assert(ret_val);
    goto finally;
except:
    Py_XDECREF(ret_val);
    ret_val = NULL;
finally:
    return ret_val;
}

static PyObject *
SkipList_node_width(SkipList *self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret_val = NULL;
    int index;
    int level;
    Py_ssize_t size;
    static char *kwlist[] = {
        (char *) "index", /* Integer, negative values are from the end. */
        (char *) "level", /* Integer, negative values are from the end. */
        NULL
    };

    assert(self && self->pSl_void);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "ii:node_width",
                                      kwlist, &index, &level)) {
        assert(PyErr_Occurred());
        goto except;
    }
    if (_size(self, size)) {
        PyErr_BadInternalCall();
        goto except;
    }
    if (_check_index_against_size("Index", index, size)) {
        goto except;
    }
    if (index < 0) {
        index += size;
    }
    Py_ssize_t height;
    /* Check level in limits. */
    switch (self->_data_type) {
        case TYPE_LONG:
            height = self->pSl_long->height(index);
            break;
        case TYPE_DOUBLE:
            height = self->pSl_double->height(index);
            break;
        case TYPE_BYTES:
            height = self->pSl_bytes->height(index);
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    if (_check_index_against_size("Level", level, height)) {
        goto except;
    }
    /* Get width. */
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->pSl_long->width(index, level));
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->pSl_double->width(index, level));
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->pSl_bytes->width(index, level));
            break;
        default:
            PyErr_BadInternalCall();
            goto except;
            break;
    }
    assert(! PyErr_Occurred());
    assert(ret_val);
    goto finally;
except:
    Py_XDECREF(ret_val);
    ret_val = NULL;
finally:
    return ret_val;
}

static PyMethodDef SkipList_methods[] = {
    {"has", (PyCFunction)SkipList_has, METH_O,
        "Return True if the value is in the skip list, False otherwise."
    },
    {"at", (PyCFunction)SkipList_at, METH_O,
        "Return the value at the given index."
    },
    {"at_seq", (PyCFunction)SkipList_at_sequence, METH_VARARGS | METH_KEYWORDS,
        "Return the sequence of count values at the given index."
    },
    {"index", (PyCFunction)SkipList_index, METH_O,
        "Return the index of the given value, if found."
    },
    {"size", (PyCFunction)SkipList_size, METH_NOARGS,
     "Return the number of elements in the skip list."
    },
    {"height", (PyCFunction)SkipList_height, METH_NOARGS,
        "Return the height of the skip list head node."
    },
    {"insert", (PyCFunction)SkipList_insert, METH_O,
        "Insert the value into the skip list."
    },
    {"remove", (PyCFunction)SkipList_remove, METH_O,
        "Remove the value from the skip list."
    },
#ifdef INCLUDE_METHODS_THAT_USE_STREAMS
    {"dot_file", (PyCFunction)SkipList_dot_file, METH_NOARGS,
        "Returns a bytes object suitable for Graphviz processing of the"
        "current state of the skip list."
        " Use open(<name>, 'wb').write(self.dotFile()) for Graphviz."
    },
#endif
    {"lacks_integrity", (PyCFunction)SkipList_lacks_integrity, METH_NOARGS,
        "Returns non-zero is the skip list faulty, zero if OK."
    },
    {"node_height", (PyCFunction)SkipList_node_height, METH_VARARGS | METH_KEYWORDS,
        "Return the height of node at the given index."
    },
    {"node_width", (PyCFunction)SkipList_node_width, METH_VARARGS | METH_KEYWORDS,
        "Return the width of node at the given index and level."
    },
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char _py_skip_list_docs[] =
"SkipList - An implementation of a skip list for floats, longs and bytes.";

PyTypeObject SkipListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    ORDERED_STRUCTS_MODULE_NAME ".SkipList",      /* tp_name */
    sizeof(SkipList),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)SkipList_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    /* TODO: implement:
     * PySequenceMethods.sq_length,
     * PySequenceMethods.sq_item  etc. */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    _py_skip_list_docs,           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    SkipList_methods,             /* tp_methods */
    SkipList_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)SkipList_init,      /* tp_init */
    0,                         /* tp_alloc */
    SkipList_new,                 /* tp_new */
    0,                         /* tp_free */
    0,                         /* tp_is_gc */
    0,                         /* tp_bases */
    0,                         /* tp_mro */
    0,                         /* tp_cache */
    0,                         /* tp_subclasses */
    0,                         /* tp_weaklist */
    0,                         /* tp_del */
    0,                         /* tp_version_tag */
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 4
    0,                         /* tp_tp_finalize */
#endif
};


