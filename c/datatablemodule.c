#include <Python.h>
#include "py_datatable.h"
#include "py_datawindow.h"
#include "py_rowmapping.h"
#include "dtutils.h"



//------------------------------------------------------------------------------
// Module definition
//------------------------------------------------------------------------------

static PyMethodDef DatatableModuleMethods[] = {
    {"select_row_slice", (PyCFunction)RowMappingPy_from_slice, METH_VARARGS,
        "Row selector constructed from a slice of rows"},
    {"select_row_indices", (PyCFunction)RowMappingPy_from_array, METH_VARARGS,
        "Row selector constructed from a list of row indices"},
    {"select_with_filter", (PyCFunction)RowMappingPy_from_filter, METH_VARARGS,
        "Row selector constructed using a filter function"},
    {"datatable_from_list", (PyCFunction)dt_DataTable_fromlist, METH_VARARGS,
        "Create Datatable from a list"},

    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static PyModuleDef datatablemodule = {
    PyModuleDef_HEAD_INIT,
    "_datatable",  /* name of the module */
    "module doc",  /* module documentation */
    -1,            /* size of per-interpreter state of the module, or -1
                      if the module keeps state in global variables */
    DatatableModuleMethods
};

/* Called when Python program imports the module */
PyMODINIT_FUNC
PyInit__datatable(void) {
    PyObject *m;

    // Sanity checks
    assert(sizeof(char) == sizeof(unsigned char));

    ColType_size[DT_AUTO] = 0;
    ColType_size[DT_DOUBLE] = sizeof(double);
    ColType_size[DT_LONG] = sizeof(long);
    ColType_size[DT_BOOL] = sizeof(char);
    ColType_size[DT_STRING] = sizeof(char*);
    ColType_size[DT_OBJECT] = sizeof(PyObject*);

    Py_int0 = PyLong_FromLong(0);
    Py_int1 = PyLong_FromLong(1);

    DataTable_PyType.tp_new = PyType_GenericNew;
    DataWindow_PyType.tp_new = PyType_GenericNew;
    RowMapping_PyType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&DataTable_PyType) < 0 ||
        PyType_Ready(&DataWindow_PyType) < 0 ||
        PyType_Ready(&RowMapping_PyType) < 0)
        return NULL;

    m = PyModule_Create(&datatablemodule);
    if (m == NULL)
        return NULL;

    init_py_datatable();

    Py_INCREF(&DataTable_PyType);
    Py_INCREF(&DataWindow_PyType);
    Py_INCREF(&RowMapping_PyType);
    PyModule_AddObject(m, "DataTable", (PyObject*) &DataTable_PyType);
    PyModule_AddObject(m, "DataWindow", (PyObject*) &DataWindow_PyType);
    PyModule_AddObject(m, "RowMapping", (PyObject*) &RowMapping_PyType);
    return m;
}
