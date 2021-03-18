//
//  cOderedStructs.cpp
//  skiplist
//
//  Created by Paul Ross on 30/09/2016.
//  Copyright (c) 2016 Paul Ross. All rights reserved.
//

#include <Python.h>

#include <limits.h>

#include "SkipList.h"
#include "cOrderedStructs.h"
#include "cSkipList.h"

static char _toss_coin_docs[] = \
"Toss a coin and return True/False."
" This calls OrderedStructs::SkipList::tossCoin().";

static PyObject *
_toss_coin(PyObject */* mod */)
{
    return PyBool_FromLong(OrderedStructs::SkipList::tossCoin());
}

static char _seed_rand_docs[] = \
"Seed the random number generator."
" This calls OrderedStructs::SkipList::seedRand().";

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
    OrderedStructs::SkipList::seedRand((unsigned) PyLong_AsLong(arg));
#else
    if (! PyLong_Check(arg) && ! PyInt_Check(arg)) {
        PyErr_Format(PyExc_TypeError,
                     "Type must be integer or long type not \"%s\" type",
                     arg->ob_type->tp_name);
        return NULL;
    }
    if (PyLong_Check(arg)) {
        // TODO: Check for overflow
        OrderedStructs::SkipList::seedRand((unsigned) PyLong_AsLong(arg));
    } else if (PyInt_Check(arg)) {
        // TODO: Check for overflow
        OrderedStructs::SkipList::seedRand((unsigned) PyInt_AS_LONG(arg));
    }
#endif
    Py_RETURN_NONE;
}

static PyObject *
_long_min_value(PyObject */* mod */)
{
    return PyLong_FromLongLong(LLONG_MIN);
}

static PyObject *
_long_max_value(PyObject */* mod */)
{
    return PyLong_FromLongLong(LLONG_MAX);
}

static PyMethodDef orderedstructsmodule_methods[] = {
    {"toss_coin", (PyCFunction)_toss_coin, METH_NOARGS, _toss_coin_docs },
    {"seed_rand", (PyCFunction)_seed_rand, METH_O, _seed_rand_docs},
    {"min_long", (PyCFunction)_long_min_value, METH_NOARGS,
        "Minimum value I can handle for an integer."},
    {"max_long", (PyCFunction)_long_max_value, METH_NOARGS,
        "Maximum value I can handle for an integer."},
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char _c_skip_list_docs[] =
"orderedstructs is an interface between Python and a C++ skip list implementation. It contains:"
"\nSkipList - An implementation of a skip list for float/long/bytes or objects."
"\nseed_rand(int) - Seed the random number generator."
"\ntoss_coin() - Toss a coin using the random number generator and return True/False.";

struct module_state {
    PyObject *error;
};

/* History:
 * 0.1.0 Initial release.
 * 0.2.0 Module now orderedstructs.
 * 0.3.0 Allows storing of PyObject* and rich comparison.
 * 0.3.1 Support Python 3.7, 3.8 and 3.9.
 * 0.3.2 Fix lambda segfault issues with Python 3.8 and 3.9.
 */
static const char *MODULE_VERSION = "0.3.2";

static const char* MODULE_BUILD_DOCS = "Initial standard build";

/** Start of module initialisation. */
#if PY_MAJOR_VERSION == 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
static struct module_state _state;
#define GETSTATE(m) (&_state)
#endif

#if PY_MAJOR_VERSION >= 3

static int orderedstructs_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int orderedstructs_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef orderedstructs_moduledef = {
    PyModuleDef_HEAD_INIT,
    ORDERED_STRUCTS_MODULE_NAME,
    _c_skip_list_docs,
    sizeof(struct module_state),
    orderedstructsmodule_methods,
    NULL,
    orderedstructs_traverse,
    orderedstructs_clear,
    NULL
};

PyMODINIT_FUNC
PyInit_orderedstructs(void)

#else // ! PY_MAJOR_VERSION >= 3

PyMODINIT_FUNC
initorderedstructs(void)
#endif // ! PY_MAJOR_VERSION >= 3
{
    PyObject* module = NULL;
    struct module_state *st = NULL;
    PyObject *class_dict = NULL;
    PyObject *thread_safe = NULL;
    
#ifdef WITH_THREAD
// From Python 3.7 onwards Py_Initialise() sets up the threads.
// PyEval_ThreadsInitialised() is deprecated.
#if PY_MAJOR_VERSION < 3 || PY_MINOR_VERSION < 7
    if (! PyEval_ThreadsInitialized()) {
        PyEval_InitThreads();
    }
#endif
#endif
#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&orderedstructs_moduledef);
#else
    module = Py_InitModule3(ORDERED_STRUCTS_MODULE_NAME,
                            orderedstructsmodule_methods,
                            _c_skip_list_docs);
#endif
    if (module == NULL) {
        goto except;
    }
    // Prepare every type in the modules, else segfaults!
    // SkipListType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&SkipListType) < 0) {
        goto except;
    }
    Py_INCREF(&SkipListType);
    if (PyModule_AddObject(module, "SkipList", (PyObject *)&SkipListType)) {
        goto except;
    }
    // Set read only class attribute with threading support.
    class_dict = SkipListType.tp_dict;
#ifdef WITH_THREAD
    thread_safe = Py_True;
#else
    thread_safe = Py_False;
#endif
    // PyDict_SetItemString will incref the value
    if (thread_safe == NULL
        || PyDict_SetItemString(class_dict, "thread_safe", thread_safe) < 0) {
        return NULL;
    }
    
    st = GETSTATE(module);
    if (st == NULL) {
        goto except;
    }
    st->error = PyErr_NewException((char*)ORDERED_STRUCTS_MODULE_NAME ".Error",
                                   NULL, NULL);
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
