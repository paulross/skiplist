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

/* Roll our own RTTI */
enum SkipListDataType {
    TYPE_ZERO = 0,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_OVERFLOW
};

/* Define these as an attempt to reduce C&P errors in very similar code. */
typedef long        TYPE_TYPE_LONG;
typedef double      TYPE_TYPE_DOUBLE;
typedef std::string TYPE_TYPE_STRING;

#define ASSERT_TYPE_IN_RANGE assert(self->_data_type > TYPE_ZERO && self->_data_type < TYPE_OVERFLOW)

#define CAST_CALL_RETURN(fn_name, ...)                                                          \
ASSERT_TYPE_IN_RANGE;                                                                           \
switch (self->_data_type) {                                                                     \
    case TYPE_LONG:                                                                             \
        return static_cast<SkipList::HeadNode<long>*>(self->pSl)->fn_name(__VA_ARGS__);         \
        break;                                                                                  \
    case TYPE_DOUBLE:                                                                           \
        return static_cast<SkipList::HeadNode<double>*>(self->pSl)->fn_name(__VA_ARGS__);       \
        break;                                                                                  \
    case TYPE_STRING:                                                                           \
        return static_cast<SkipList::HeadNode<std::string>*>(self->pSl)->fn_name(__VA_ARGS__);  \
        break;                                                                                  \
    default:                                                                                    \
        PyErr_BadInternalCall();                                                                \
        break;                                                                                  \
}

typedef struct {
    PyObject_HEAD
    enum SkipListDataType _data_type;
    void *pSl;
} PySkipList;

static void
PySkipList_dealloc(PySkipList* self)
{
    std::cout << "PySkipList_dealloc " << self->_data_type << " " << self->pSl << std::endl;
    if (self && self->pSl) {
        switch (self->_data_type) {
            case TYPE_LONG:
                delete static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl);
                break;
            case TYPE_DOUBLE:
                delete static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl);
                break;
            case TYPE_STRING:
                delete static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl);
                break;
            default:
                PyErr_BadInternalCall();
                break;
        }
        Py_TYPE(self)->tp_free((PyObject*)self);
    }
}

static PyObject *
PySkipList_new(PyTypeObject *type, PyObject */* args */, PyObject */* kwargs */) {
    PySkipList *self = NULL;

    self = (PySkipList *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_data_type = TYPE_ZERO;
        self->pSl = NULL;
//        std::cout << "PySkipList_new " << self->_data_type << " " << self->pSl << std::endl;
    }
    std::cout << "PySkipList_new " << self << std::endl;
    return (PyObject *)self;
}

static int
PySkipList_init(PySkipList *self, PyObject *args, PyObject *kwargs) {
    int ret_val = -1;
    PyObject *value_type = NULL;
    static char *kwlist[] = { "value_type", NULL};

    std::cout << "PySkipList_init " << self->_data_type << " " << self->pSl << std::endl;
    assert(self);
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "O:__init__", kwlist, &value_type)) {
        std::cout << "PySkipList_init: can not parse arguments " << std::endl;
        goto except;
    }
    assert(value_type);
    if (! PyType_CheckExact(value_type)) {
        PyErr_Format(PyExc_ValueError,
                     "Argument to __init__ must be type object not \"%s\"",
                     Py_TYPE(value_type)->tp_name);
    }
    if ((PyTypeObject *)value_type == &PyLong_Type) {
        self->_data_type = TYPE_LONG;
        self->pSl = static_cast<void *>(new SkipList::HeadNode<TYPE_TYPE_LONG>());
    } else if ((PyTypeObject *)value_type == &PyFloat_Type) {
        self->_data_type = TYPE_DOUBLE;
        self->pSl = static_cast<void *>(new SkipList::HeadNode<TYPE_TYPE_DOUBLE>());
    } else if ((PyTypeObject *)value_type == &PyString_Type) {
        self->_data_type = TYPE_STRING;
        self->pSl = static_cast<void *>(new SkipList::HeadNode<TYPE_TYPE_STRING>());
    } else {
        PyErr_Format(PyExc_ValueError,
                     "Argument to __init__ must be long, float or str, not \"%s\"",
                     Py_TYPE(value_type)->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    assert(self);
    assert(self->pSl);
    ret_val = 0;
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(self);
    ret_val = -1;
finally:
    return ret_val;
}

static PyMemberDef PySkipList_members[] = {
    {NULL, 0, 0, 0, NULL}  /* Sentinel */
};

static PyObject *
PySkipList_has(PySkipList* self, PyObject *arg)
{
    PyObject *ret_val = NULL;
    ASSERT_TYPE_IN_RANGE;
    std::string str;
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Argument to has() must be long not \"%s\" type", Py_TYPE(arg)->tp_name);
                goto except;
            }
            ret_val = PyBool_FromLong(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->has(PyLong_AsLong(arg)));
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Argument to has() must be float not \"%s\" type", Py_TYPE(arg)->tp_name);
                goto except;
            }
            ret_val = PyBool_FromLong(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->has(PyFloat_AS_DOUBLE(arg)));
            break;
        case TYPE_STRING:
            if (! PyString_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Argument to has() must be str not \"%s\" type", Py_TYPE(arg)->tp_name);
                goto except;
            }
            str = std::string(PyString_AS_STRING(arg));
            ret_val = PyBool_FromLong(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->has(str));
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
_size(PySkipList* self, Py_ssize_t &value) {
    int err_code = 0;
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            value = static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->size();
            break;
        case TYPE_DOUBLE:
            value = static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->size();
            break;
        case TYPE_STRING:
            value = static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->size();
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
            PyErr_Format(PyExc_IndexError, "%s %ld out of range 0 <= index < %ld", prefix, index, size);
        } else {
            PyErr_Format(PyExc_IndexError, "%s %ld out of range %ld < index <= -1", prefix, index, -(size + 1));
        }
        return -1;
    }
    return 0;
}

static PyObject *
PySkipList_at(PySkipList *self, PyObject *arg)
{
    PyObject *ret_val = NULL;
    Py_ssize_t size;
    long index = 0;

    if (! PyLong_Check(arg) && ! PyInt_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Type must be integer or long type not \"%s\" type",
                     arg->ob_type->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    if (PyLong_Check(arg)) {
        index = PyLong_AsLong(arg);
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
            ret_val = PyLong_FromLong(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->at(index));
            break;
        case TYPE_DOUBLE:
            ret_val = PyFloat_FromDouble(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->at(index));
            break;
        case TYPE_STRING:
            ret_val = PyString_FromString(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->at(index).c_str());
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
_at_sequence_long(PySkipList *self, int index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_LONG> dest;

    assert(self->_data_type == TYPE_LONG);
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->at(index, count, dest);
    } catch (SkipList::IndexError &err) {
        PyErr_SetString(PyExc_IndexError, err.message().c_str());
        return NULL;
    }
    assert(dest.size() == count);
    for (Py_ssize_t i = 0; i < count; ++i) {
        PyTuple_SET_ITEM(ret_val, i, PyLong_FromLong(dest[i]));
    }
    return ret_val;
}

static PyObject*
_at_sequence_double(PySkipList *self, int index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_DOUBLE> dest;

    assert(self->_data_type == TYPE_DOUBLE);
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->at(index, count, dest);
    } catch (SkipList::IndexError &err) {
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
_at_sequence_string(PySkipList *self, int index, Py_ssize_t count) {
    PyObject *ret_val = NULL;
    std::vector<TYPE_TYPE_STRING> dest;

    assert(self->_data_type == TYPE_STRING);
    ret_val = PyTuple_New(count);
    if (! ret_val) {
        PyErr_SetString(PyExc_MemoryError, "Could not create tuple.");
        return NULL;
    }
    try {
        static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->at(index, count, dest);
    } catch (SkipList::IndexError &err) {
        PyErr_SetString(PyExc_IndexError, err.message().c_str());
        return NULL;
    }
    assert(dest.size() == count);
    for (Py_ssize_t i = 0; i < count; ++i) {
        PyTuple_SET_ITEM(ret_val, i, PyString_FromString(dest[i].c_str()));
    }
    return ret_val;
}


/* Returns a tuple of 'count' PyObjects starting at 'index'. */
static PyObject *
PySkipList_at_sequence(PySkipList *self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret = NULL;
    Py_ssize_t index;
    Py_ssize_t count;
    static char *kwlist[] = {
        (char *) "index", /* Integer, negative values are from the end. */
        (char *) "count", /* Integer, must be positive. */
        NULL
    };

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
        case TYPE_STRING:
            ret = _at_sequence_string(self, index, count);
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
PySkipList_size(PySkipList* self)
{
    PyObject *ret_val = NULL;
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->size());
            break;
        case TYPE_DOUBLE:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->size());
            break;
        case TYPE_STRING:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->size());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
PySkipList_height(PySkipList* self)
{
    PyObject *ret_val = NULL;
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->height());
            break;
        case TYPE_DOUBLE:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->height());
            break;
        case TYPE_STRING:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->height());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
PySkipList_insert(PySkipList* self, PyObject *arg)
{
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be long not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->insert(PyLong_AsLong(arg));
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be float not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->insert(PyFloat_AS_DOUBLE(arg));
            break;
        case TYPE_STRING:
            if (! PyString_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be str not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->insert(PyString_AsString(arg));
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    Py_RETURN_NONE;
}

static PyObject *
PySkipList_remove(PySkipList* self, PyObject *arg)
{
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be long not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->remove(PyLong_AsLong(arg));
            } catch (SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be float not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->remove(PyFloat_AS_DOUBLE(arg));
            } catch (SkipList::ValueError &err) {
                // For whatever reason PyErr_Format does not support doubles
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        case TYPE_STRING:
            if (! PyString_Check(arg)) {
                PyErr_Format(PyExc_TypeError, "Type must be str not \"%s\" type", Py_TYPE(arg)->tp_name);
                return NULL;
            }
            try {
                static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->remove(PyString_AsString(arg));
            } catch (SkipList::ValueError &err) {
                PyErr_SetString(PyExc_ValueError, err.message().c_str());
                return NULL;
            }
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    Py_RETURN_NONE;
}

static PyObject *
PySkipList_dot_file(PySkipList* self)
{
    std::stringstream ostr;

    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->dotFile(ostr);
            static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->dotFileFinalise(ostr);
            break;
        case TYPE_DOUBLE:
            static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->dotFile(ostr);
            static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->dotFileFinalise(ostr);
            break;
        case TYPE_STRING:
            static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->dotFile(ostr);
            static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->dotFileFinalise(ostr);
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return PyBytes_FromString(ostr.str().c_str());
}

static PyObject *
PySkipList_lacks_integrity(PySkipList* self)
{
    PyObject *ret_val = NULL;
    ASSERT_TYPE_IN_RANGE;
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->lacksIntegrity());
            break;
        case TYPE_DOUBLE:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->lacksIntegrity());
            break;
        case TYPE_STRING:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->lacksIntegrity());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
PySkipList_node_height(PySkipList* self, PyObject *args, PyObject *kwargs)
{
    PyObject *ret_val = NULL;
    int index;
    Py_ssize_t size;
    static char *kwlist[] = {
        (char *) "index", /* Integer, negative values are from the end. */
        NULL
    };
    
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "i:node_height", kwlist, &index)) {
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
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->height(index));
            break;
        case TYPE_DOUBLE:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->height(index));
            break;
        case TYPE_STRING:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->height(index));
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
    Py_XDECREF(ret_val);
    ret_val = NULL;
finally:
    return ret_val;
}

static PyObject *
PySkipList_node_width(PySkipList *self, PyObject *args, PyObject *kwargs)
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
            height = static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->height(index);
            break;
        case TYPE_DOUBLE:
            height = static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->height(index);
            break;
        case TYPE_STRING:
            height = static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->height(index);
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
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_LONG>*>(self->pSl)->width(index, level));
            break;
        case TYPE_DOUBLE:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_DOUBLE>*>(self->pSl)->width(index, level));
            break;
        case TYPE_STRING:
            ret_val = PyInt_FromSsize_t(static_cast<SkipList::HeadNode<TYPE_TYPE_STRING>*>(self->pSl)->width(index, level));
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


static PyMethodDef PySkipList_methods[] = {
    {"has", (PyCFunction)PySkipList_has, METH_O,
        "Return True if the value is in the skip list, False otherwise."
    },
    {"at", (PyCFunction)PySkipList_at, METH_O,
        "Return the value at the given index."
    },
    {"at_seq", (PyCFunction)PySkipList_at_sequence, METH_VARARGS | METH_KEYWORDS,
        "Return the sequence of count values at the given index."
    },
    {"size", (PyCFunction)PySkipList_size, METH_NOARGS,
     "Return the number of elements in the skip list."
    },
    {"height", (PyCFunction)PySkipList_height, METH_NOARGS,
        "Return the height of the skip list head node."
    },
    {"insert", (PyCFunction)PySkipList_insert, METH_O,
        "Insert the value into the skip list."
    },
    {"remove", (PyCFunction)PySkipList_remove, METH_O,
        "Remove the value from the skip list."
    },
    {"dot_file", (PyCFunction)PySkipList_dot_file, METH_NOARGS,
        "Returns a bytes object suitable for Graphviz processing of the"
        "current state of the skip list."
        " Use open(<name>, 'wb').write(self.dotFile()) for Graphviz."
    },
    {"lacks_integrity", (PyCFunction)PySkipList_lacks_integrity, METH_NOARGS,
        "Returns non-zero is the skip list faulty, zero if OK."
    },
    {"node_height", (PyCFunction)PySkipList_node_height, METH_VARARGS | METH_KEYWORDS,
        "Return the height of node at the given index."
    },
    {"node_width", (PyCFunction)PySkipList_node_width, METH_VARARGS | METH_KEYWORDS,
        "Return the width of node at the given index and level."
    },
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char _py_skip_list_docs[] =
"PySkipList - An implementation of a skip list for floats.";

static PyTypeObject PySkipListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "cSkipList.PySkipList",      /* tp_name */
    sizeof(PySkipList),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)PySkipList_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    /* TODO: implement PySequenceMethods.sq_length, PySequenceMethods.sq_item  etc. */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    _py_skip_list_docs,           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    PySkipList_methods,             /* tp_methods */
    PySkipList_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PySkipList_init,      /* tp_init */
    0,                         /* tp_alloc */
    PySkipList_new,                 /* tp_new */
    0,                         /* tp_free */
    0,                         /* tp_is_gc */
    0,                         /* tp_bases */
    0,                         /* tp_mro */
    0,                         /* tp_cache */
    0,                         /* tp_subclasses */
    0,                         /* tp_weaklist */
    0,                         /* tp_del */
    0,                         /* tp_version_tag */
};


static char _toss_coin_docs[] =
"Toss a coin and return True/False.";

static PyObject *_toss_coin(PyObject */* mod */)
{
    return PyBool_FromLong(SkipList::tossCoin());
}

static char _seed_rand_docs[] =
"Seed the random number generator.";

static PyObject *_seed_rand(PyObject */* mod */, PyObject *arg)
{
    if (! PyLong_Check(arg) && ! PyInt_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Type must be integer or long type not \"%s\" type",
                     arg->ob_type->tp_name);
        return NULL;
    }
    if (PyLong_Check(arg)) {
        // TODO: Check for overflow
        SkipList::seedRand((unsigned) PyLong_AsLong(arg));
    } else if (PyInt_Check(arg)) {
        // TODO: Check for overflow
        SkipList::seedRand((unsigned) PyInt_AS_LONG(arg));
    }
    Py_RETURN_NONE;
}


static PyMethodDef cSkipListmodule_methods[] = {
    {"toss_coin", (PyCFunction)_toss_coin, METH_NOARGS, _toss_coin_docs },
    {"seed_rand", (PyCFunction)_seed_rand, METH_O, _seed_rand_docs},
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char _c_skip_list_docs[] =
    "cSkipList is an interface between Python and a C++ skip list implementation. It contains:"
    "\nPySkipList - An implementation of a skip list for floats."
    "\nseed_rand(int) - seed the random number generator."
    "\ntoss_coin() - Toss a coin using the random number generator and return True/False.";

PyMODINIT_FUNC
initcSkipList(void)
{
    PyObject* m = NULL;

    PySkipListType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PySkipListType) < 0) {
        return;
    }
    m = Py_InitModule3("cSkipList", cSkipListmodule_methods, _c_skip_list_docs);
    if (m == NULL) {
        return;
    }
    Py_INCREF(&PySkipListType);
    if (PyModule_AddObject(m, "PySkipList", (PyObject *)&PySkipListType)) {
        return;
    }
}
