//
//  cOderedStructs.cpp
//  skiplist
//
//  Created by Paul Ross on 30/09/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include <Python.h>

#include "SkipList.h"

#include "cSkipList.h"
#include "cStdMap.h"

static char _toss_coin_docs[] = \
"Toss a coin and return True/False."
" This calls ManAHL::SkipList::tossCoin().";

static PyObject *
_toss_coin(PyObject */* mod */)
{
    return PyBool_FromLong(ManAHL::SkipList::tossCoin());
}

static char _seed_rand_docs[] = \
"Seed the random number generator."
" This calls ManAHL::SkipList::seedRand().";

static PyObject *
_seed_rand(PyObject */* mod */, PyObject *arg)
{
#if PY_MAJOR_VERSION == 3
    if (! PyLong_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Type must be long type not \"%s\" type",
                     arg->ob_type->tp_name);
        return NULL;
    }
    ManAHL::SkipList::seedRand((unsigned) PyLong_AsLong(arg));
#else
    if (! PyLong_Check(arg) && ! PyInt_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Type must be integer or long type not \"%s\" type",
                     arg->ob_type->tp_name);
        return NULL;
    }
    if (PyLong_Check(arg)) {
        // TODO: Check for overflow
        ManAHL::SkipList::seedRand((unsigned) PyLong_AsLong(arg));
    } else if (PyInt_Check(arg)) {
        // TODO: Check for overflow
        ManAHL::SkipList::seedRand((unsigned) PyInt_AS_LONG(arg));
    }
#endif
    Py_RETURN_NONE;
}

static PyObject *
_long_min_value(PyObject */* mod */)
{
    return PyLong_FromLongLong(LONG_LONG_MIN);
}

static PyObject *
_long_max_value(PyObject */* mod */)
{
    return PyLong_FromLongLong(LONG_LONG_MAX);
}

static PyMethodDef cSkipListmodule_methods[] = {
    {"toss_coin", (PyCFunction)_toss_coin, METH_NOARGS, _toss_coin_docs },
    {"seed_rand", (PyCFunction)_seed_rand, METH_O, _seed_rand_docs},
    {"min_long", (PyCFunction)_long_min_value, METH_NOARGS,
        "Minimum value I can handle for an integer."},
    {"max_long", (PyCFunction)_long_max_value, METH_NOARGS,
        "Maximum value I can handle for an integer."},
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char _c_skip_list_docs[] =
"cSkipList is an interface between Python and a C++ skip list implementation. It contains:"
"\nPySkipList - An implementation of a skip list for floats."
"\nseed_rand(int) - Seed the random number generator."
"\ntoss_coin() - Toss a coin using the random number generator and return True/False.";

struct module_state {
    PyObject *error;
};

static const char *MODULE_VERSION = "0.1.0";

static const char* MODULE_BUILD_DOCS = "Initial standard build";

/** Start of module initialisation. */
#if PY_MAJOR_VERSION == 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
static struct module_state _state;
#define GETSTATE(m) (&_state)
#endif

#if PY_MAJOR_VERSION >= 3

static int cSkipList_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int cSkipList_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef cSkipList_moduledef = {
    PyModuleDef_HEAD_INIT,
    "cSkipList",
    _c_skip_list_docs,
    sizeof(struct module_state),
    cSkipListmodule_methods,
    NULL,
    cSkipList_traverse,
    cSkipList_clear,
    NULL
};

PyMODINIT_FUNC
PyInit_cSkipList(void)

#else // ! PY_MAJOR_VERSION >= 3

PyMODINIT_FUNC
initcSkipList(void)
#endif // ! PY_MAJOR_VERSION >= 3
{
    PyObject* module = NULL;
    struct module_state *st = NULL;
    
    // Prepare every type in the moduls, else segfaults!
    PySkipListType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PySkipListType) < 0) {
        goto except;
    }
    PyStdMapType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyStdMapType) < 0) {
        goto except;
    }
#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&cSkipList_moduledef);
#else
    module = Py_InitModule3("cSkipList",
                            cSkipListmodule_methods,
                            _c_skip_list_docs);
#endif
    if (module == NULL) {
        goto except;
    }
    Py_INCREF(&PySkipListType);
    if (PyModule_AddObject(module, "PySkipList", (PyObject *)&PySkipListType)) {
        goto except;
    }
    Py_INCREF(&PyStdMapType);
    if (PyModule_AddObject(module, "PyStdMap", (PyObject *)&PyStdMapType)) {
        goto except;
    }
    st = GETSTATE(module);
    if (st == NULL) {
        goto except;
    }
    st->error = PyErr_NewException("cSkipList.Error", NULL, NULL);
    if (st->error == NULL) {
        goto except;
    }
    /* Adds version and build information to the module. */
    if (PyModule_AddStringConstant(module, "__version__", MODULE_VERSION)) {
        goto except;
    }
    if (PyModule_AddStringConstant(module, "__build_time__",
                                   __DATE__ " " __TIME__)) {
        goto except;
    }
#ifdef DEBUG
    if (PyModule_AddStringConstant(module, "__build_type__", "debug")) {
        goto except;
    }
#else
    if (PyModule_AddStringConstant(module, "__build_type__", "release")) {
        goto except;
    }
#endif
    if (PyModule_AddStringConstant(module, "__build_target__", PY_VERSION)) {
        goto except;
    }
    if (PyModule_AddStringConstant(module, "__build_docs__",
                                   MODULE_BUILD_DOCS)) {
        goto except;
    }
    goto finally;
except:
    Py_XDECREF(module);
    module = NULL;
finally:
#if PY_MAJOR_VERSION >= 3
    return module;
#else
    return;
#endif
}
