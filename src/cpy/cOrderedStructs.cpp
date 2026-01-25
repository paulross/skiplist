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

static char toss_coin_docs[] = \
"Toss a coin and return True/False."
" This calls OrderedStructs::SkipList::tossCoin().";

/**
 * Virtual toss of a coin.
 */
static PyObject *
toss_coin(PyObject */* mod */) {
    return PyBool_FromLong(OrderedStructs::SkipList::tossCoin());
}

static char seed_rand_docs[] = \
"Seed the random number generator."
" This calls OrderedStructs::SkipList::seedRand().";

/**
 * Seed the random number generator.
 */
static PyObject *
seed_rand(PyObject */* mod */, PyObject * arg) {
#if PY_MAJOR_VERSION == 3
    if (!PyLong_Check(arg)) {
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
long_min_value(PyObject */* mod */) {
    return PyLong_FromLongLong(LLONG_MIN);
}

static PyObject *
long_max_value(PyObject */* mod */) {
    return PyLong_FromLongLong(LLONG_MAX);
}

/**
 * CPython orderedstructs package methods.
 */
static PyMethodDef orderedstructsmodule_methods[] = {
        {"toss_coin", (PyCFunction) toss_coin,      METH_NOARGS, toss_coin_docs},
        {"seed_rand", (PyCFunction) seed_rand,      METH_O,      seed_rand_docs},
        {"min_long",  (PyCFunction) long_min_value, METH_NOARGS,
                                                                 "Minimum value I can handle for an integer."},
        {"max_long",  (PyCFunction) long_max_value, METH_NOARGS,
                                                                 "Maximum value I can handle for an integer."},
        {NULL, NULL, 0, NULL}  /* Sentinel */
};

static char c_skip_list_docs[] =
        "orderedstructs is an interface between Python and a C++ skip list implementation. It contains:"
        "\nSkipList - An implementation of a skip list for float/long/bytes or objects."
        "\nseed_rand(int) - Seed the random number generator."
        "\ntoss_coin() - Toss a coin using the random number generator and return True/False.";

/** @brief Wrapper of module state for Python 2 and 3. */
struct module_state {
    PyObject *error;
};

/* History:
 * See HISTORY.md at the project root.
 */
static const char *MODULE_VERSION = "0.3.18rc0";

static const char *MODULE_BUILD_DOCS = "Standard build.";

/** Start of module initialisation. */
#if PY_MAJOR_VERSION == 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
static struct module_state _state;
#define GETSTATE(m) (&_state)
#endif

#if PY_MAJOR_VERSION >= 3

static int orderedstructs_traverse(PyObject * m, visitproc
visit,
void *arg
) {
Py_VISIT(GETSTATE(m)->error);
return 0;
}

static int orderedstructs_clear(PyObject * m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef orderedstructs_moduledef = {
        PyModuleDef_HEAD_INIT,
        ORDERED_STRUCTS_MODULE_NAME,
        c_skip_list_docs,
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
    PyObject * module = NULL;
    PyObject * class_dict = NULL;
    PyObject * thread_safe = NULL;

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
                            c_skip_list_docs);
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
    if (PyModule_AddObject(module, "SkipList", (PyObject *) &SkipListType)) {
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
