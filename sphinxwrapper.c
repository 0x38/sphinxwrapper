/*
 * sphinxwrapper.c
 *
 *  Created on 8 Jul. 2017
 *      Author: Dane Finlay
 */

#include "pyutil.h"
#include "audio.h"
#include "pypocketsphinx.h"

#if PY_MAJOR_VERSION >= 3
struct module_state {};
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#endif

static PyMethodDef sphinxwrapper_methods[] = {
    {NULL, NULL, 0, NULL} // Sentinel signifying the end of definitions
};

#if PY_MAJOR_VERSION >= 3
// Python 3 extensions define modules similar to the way custom types are defined
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "sphinxwrapper",             /* m_name */
    NULL,                        /* m_doc */
    sizeof(struct module_state), /* m_size */
    sphinxwrapper_methods,       /* m_methods */
    NULL,                        /* m_slots */
    NULL,                        /* m_traverse */
    NULL,                        /* m_clear */
    NULL                         /* m_free */
};

PyMODINIT_FUNC
PyInit_sphinxwrapper(void) {
#else
PyMODINIT_FUNC
initsphinxwrapper(void) {
#endif
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("sphinxwrapper", sphinxwrapper_methods);
#endif
    // Set up the 'PocketSphinx' type and anything else it needs
    // Return appropriately for the Python version if there's an error
    if (initpocketsphinx(module) == NULL)
        INITERROR;

    // Set up the audio related types
    if (initaudio(module) == NULL)
        INITERROR;

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

int
main(int argc, char *argv[]) {
#if PY_MAJOR_VERSION >= 3
    /* Pass argv[0] to the Python interpreter */
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
#else
    char *program = argv[0];
#endif

    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    PyEval_InitThreads();

    /* Add a static module */
#if PY_MAJOR_VERSION >= 3
    PyInit_sphinxwrapper();
#else
    initsphinxwrapper();
#endif
}

