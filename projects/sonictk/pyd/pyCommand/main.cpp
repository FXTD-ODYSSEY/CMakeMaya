#include "testCommand.h"

#include <maya/MFnPlugin.h>
#include <Python.h>
#include <maya/MGlobal.h>
#include <stdio.h>

//registering the deformer node in Maya system
MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin fnPlugin(obj, "Any", "1.0", "Any");

	status = fnPlugin.registerCommand("runTest", myTestCmd::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;


	MFnPlugin fnPlugin(obj);

	status = fnPlugin.deregisterCommand("runTest");
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}



static const char MAYA_PYTHON_C_EXT_DOCSTRING[] = "An example Python C extension that makes use of Maya functionality.";
static const char HELLO_WORLD_MAYA_DOCSTRING[] = "Says hello world!";

static PyObject *pyHelloWorldMaya(PyObject *module, PyObject *args)
{
	const char *inputString;
	if (!PyArg_ParseTuple(args, "s", &inputString))
	{
		return NULL;
	}

	PyGILState_STATE pyGILState = PyGILState_Ensure();

	MGlobal::displayInfo(inputString);

	PyObject *result = Py_BuildValue("s", inputString);

	PyGILState_Release(pyGILState);

	return result;
}

// NOTE: (sonictk) This declares the available methods for the module
static PyMethodDef mayaPythonCExtMethods[] = {
	{"hello_world_maya", pyHelloWorldMaya, METH_VARARGS, HELLO_WORLD_MAYA_DOCSTRING},
	{NULL, NULL, 0, NULL} // NOTE: (sonictk) Sentinel value for Python
};

// NOTE: (sonictk) Disable name mangling for Python distutils since it really
// wants C-style compilation of exporting symbols. Also, since it hardcodes the
// name of the symbol that it exports, we have to use the snake-case naming
// for the entry point.
#if PY_MAJOR_VERSION == 2
extern "C" PyMODINIT_FUNC initpy_command()
{	
	MGlobal::displayInfo("register pyCommand node");
	
	PyObject *module = Py_InitModule3("py_command",
									  mayaPythonCExtMethods,
									  MAYA_PYTHON_C_EXT_DOCSTRING);
}
#elif PY_MAJOR_VERSION == 3
extern "C" PyMODINIT_FUNC PyInit_py_command()
{
	static PyModuleDef hello_module = {
		PyModuleDef_HEAD_INIT,
		"py_command",					 // Module name to use with Python import statements
		MAYA_PYTHON_C_EXT_DOCSTRING, // Module description
		0,
		mayaPythonCExtMethods // Structure that defines the methods of the module
	};
	return PyModule_Create(&hello_module);
}
#endif
