#ifndef MAYA_PYTHON_C_EXT_PY_MOD_COMMON_H
#define MAYA_PYTHON_C_EXT_PY_MOD_COMMON_H

#include <Python.h>
#include <maya/MGlobal.h>

static const char HELLO_WORLD_MAYA_DOCSTRING[] = "Says hello world!";

static PyObject *pyHelloWorldMaya(PyObject *module, PyObject *args)
{
	const char *inputString;
	if (!PyArg_ParseTuple(args, "s", &inputString)) {
		return NULL;
	}

	PyGILState_STATE pyGILState = PyGILState_Ensure();

	MGlobal::displayInfo(inputString);

	PyObject *result = Py_BuildValue("s", inputString);

	PyGILState_Release(pyGILState);

	return result;
}


static const char MAYA_PYTHON_C_EXT_DOCSTRING[] = "An example Python C extension that makes use of Maya functionality.";

// NOTE: (sonictk) This declares the available methods for the module
static PyMethodDef mayaPythonCExtMethods[] = {
	{"hello_world_maya", pyHelloWorldMaya, METH_VARARGS, HELLO_WORLD_MAYA_DOCSTRING},
	{NULL, NULL, 0, NULL}	// NOTE: (sonictk) Sentinel value for Python
};


#endif /* MAYA_PYTHON_C_EXT_PY_MOD_COMMON_H */
