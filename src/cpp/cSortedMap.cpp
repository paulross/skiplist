//
//  cSortedMap.cpp
//  skiplist
//
//  Created by Paul Ross on 30/09/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//
#include <Python.h>
#include "structmember.h"

#include <map>
#include <string>

#include "OrderedStructs.h"
#include "cOrderedStructs.h"

typedef struct {
    PyObject_HEAD
    enum KeyDataType _data_type;
    union {
        /* NULL/non-NULL pointer, not to be de-referrenced. */
        std::map<void, PyObject*>             *_pKeyVoid;
        std::map<TYPE_TYPE_LONG, PyObject*>   *_pKeyLong;
        std::map<TYPE_TYPE_DOUBLE, PyObject*> *_pKeyDouble;
        std::map<TYPE_TYPE_BYTES, PyObject*>  *_pKeyBytes;
    };
} SortedMap;

static PyObject *
SortedMap_new(PyTypeObject *type, PyObject */* args */, PyObject */* kwargs */) {
    SortedMap *self = NULL;
    
    self = (SortedMap *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->_data_type = TYPE_ZERO;
        self->_pKeyVoid = NULL;
    }
    return (PyObject *)self;
}

static int
SortedMap_init(SortedMap *self, PyObject *args, PyObject *kwargs) {
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
        self->_pKeyLong = new std::map<TYPE_TYPE_LONG, PyObject*>();
    } else if ((PyTypeObject *)value_type == &PyFloat_Type) {
        self->_data_type = TYPE_DOUBLE;
        self->_pKeyDouble = new std::map<TYPE_TYPE_DOUBLE, PyObject*>();
    } else if ((PyTypeObject *)value_type == &PyBytes_Type) {
        self->_data_type = TYPE_BYTES;
        self->_pKeyBytes = new std::map<TYPE_TYPE_BYTES, PyObject*>();
    } else {
        PyErr_Format(PyExc_ValueError,
                     "Argument to __init__ must be long, float or bytes, not \"%s\"",
                     ((PyTypeObject *)value_type)->tp_name);
        goto except;
    }
    assert(! PyErr_Occurred());
    assert(self);
    assert(self->_pKeyVoid);
    ret_val = 0;
    goto finally;
except:
    assert(PyErr_Occurred());
    Py_XDECREF(self);
    ret_val = -1;
finally:
    return ret_val;
}

template<typename T>
static void
_decref_all_values(std::map<T, PyObject*> *p) {
    for (auto iter = p->begin(); iter != p->end(); ++iter) {
        Py_DECREF(iter->second);
    }
}
// Template instatiations.
template void
_decref_all_values<TYPE_TYPE_LONG>(std::map<TYPE_TYPE_LONG, PyObject*> *p);
template void
_decref_all_values<TYPE_TYPE_DOUBLE>(std::map<TYPE_TYPE_DOUBLE, PyObject*> *p);
template void
_decref_all_values<TYPE_TYPE_BYTES>(std::map<TYPE_TYPE_BYTES, PyObject*> *p);

static void
SortedMap_dealloc(SortedMap *self)
{
    if (self && self->_pKeyVoid) {
        switch (self->_data_type) {
            case TYPE_LONG:
                _decref_all_values(self->_pKeyLong);
                delete self->_pKeyLong;
                break;
            case TYPE_DOUBLE:
                _decref_all_values(self->_pKeyDouble);
                delete self->_pKeyDouble;
                break;
            case TYPE_BYTES:
                _decref_all_values(self->_pKeyBytes);
                delete self->_pKeyBytes;
                break;
            default:
                PyErr_BadInternalCall();
                break;
        }
        Py_TYPE(self)->tp_free((PyObject*)self);
    }
}

static PyObject *
SortedMap_size(SortedMap *self)
{
    PyObject *ret_val = NULL;
    
    assert(self && self->_pKeyVoid);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->_pKeyLong->size());
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->_pKeyDouble->size());
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->_pKeyBytes->size());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
SortedMap_max_size(SortedMap *self)
{
    PyObject *ret_val = NULL;
    
    assert(self && self->_pKeyVoid);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    switch (self->_data_type) {
        case TYPE_LONG:
            ret_val = PyLong_FromSsize_t(self->_pKeyLong->max_size());
            break;
        case TYPE_DOUBLE:
            ret_val = PyLong_FromSsize_t(self->_pKeyDouble->max_size());
            break;
        case TYPE_BYTES:
            ret_val = PyLong_FromSsize_t(self->_pKeyBytes->max_size());
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    return ret_val;
}

static PyObject *
SortedMap_insert(SortedMap *self, PyObject *args, PyObject *kwargs)
{
    assert(self && self->_pKeyVoid);
    ASSERT_TYPE_IN_RANGE;
    assert(! PyErr_Occurred());
    
    PyObject *key = NULL;
    PyObject *val = NULL;
    static char *kwlist[] = {
        (char *) "key", /* Integer, negative values are from the end. */
        (char *) "value", /* Integer, negative values are from the end. */
        NULL
    };
    if (! PyArg_ParseTupleAndKeywords(args, kwargs, "OO:insert",
                                      kwlist, &key, &val)) {
        assert(PyErr_Occurred());
        return NULL;
    }
    
    switch (self->_data_type) {
        case TYPE_LONG:
            if (! PyLong_Check(key)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be long not \"%s\" type",
                             Py_TYPE(key)->tp_name);
                return NULL;
            }
            assert(0);
//            self->pSl_long->insert(PyLong_AsLongLong(arg));
            if (PyErr_Occurred()) {
                return NULL;
            }
            break;
        case TYPE_DOUBLE:
            if (! PyFloat_Check(key)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be float not \"%s\" type",
                             Py_TYPE(key)->tp_name);
                return NULL;
            }
            assert(0);
//            try {
//                self->pSl_double->insert(PyFloat_AS_DOUBLE(arg));
//            } catch (ManAHL::SkipList::FailedComparison &err) {
//                /* This will happen if arg is a NaN. */
//                PyErr_SetString(PyExc_ValueError, err.message().c_str());
//                return NULL;
//            }
            break;
        case TYPE_BYTES:
            if (! PyBytes_Check(key)) {
                PyErr_Format(PyExc_TypeError,
                             "Type must be bytes not \"%s\" type",
                             Py_TYPE(key)->tp_name);
                return NULL;
            }
            assert(0);
//            self->pSl_bytes->insert(bytes_as_std_string(arg));
            break;
        default:
            PyErr_BadInternalCall();
            break;
    }
    Py_RETURN_NONE;
}

static PyMethodDef SortedMap_methods[] = {
    {"size", (PyCFunction)SortedMap_size, METH_NOARGS,
        "Return the number of elements in the map."
    },
    {"max_size", (PyCFunction)SortedMap_max_size, METH_NOARGS,
        "Return returns the maximum possible number of elements in the map."
    },
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static PyMemberDef SortedMap_members[] = {
    {NULL, 0, 0, 0, NULL}  /* Sentinel */
};

static char _py_std_map_docs[] =
"SortedMap - An implementation of a sorted map for floats, long and bytes.";

PyTypeObject SortedMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    ORDERED_STRUCTS_MODULE_NAME ".SortedMap",      /* tp_name */
    sizeof(SortedMap),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)SortedMap_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    /* TODO: ? implement:
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
    _py_std_map_docs,           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    SortedMap_methods,             /* tp_methods */
    SortedMap_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)SortedMap_init,      /* tp_init */
    0,                         /* tp_alloc */
    SortedMap_new,                 /* tp_new */
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
