#include <Python.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

const char *kAUTHOR = "TimmyLiang";
const char *kVERSION = "1.0.0";
const char *kREQUIRED_API_VERSION = "Any";

static const char HELLO_WORLD_MAYA_DOCSTRING[] = "Says hello world!";
static const char MAYA_PYTHON_C_EXT_DOCSTRING[] = "An example Python C extension that makes use of Maya functionality.";

PyObject *module = NULL;

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



// NOTE: (sonictk) This declares the available methods for the module
static PyMethodDef mayaPythonCExtMethods[] = {
	{"hello_world_maya", pyHelloWorldMaya, METH_VARARGS, HELLO_WORLD_MAYA_DOCSTRING},
	{NULL, NULL, 0, NULL}	// NOTE: (sonictk) Sentinel value for Python
};



MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, kAUTHOR, kVERSION, kREQUIRED_API_VERSION);
	if (!Py_IsInitialized())
		Py_Initialize();

	if (Py_IsInitialized())
	{
		PyGILState_STATE pyGILState = PyGILState_Ensure();

#if PY_MAJOR_VERSION == 2
		module = Py_InitModule3("mll_py",
								mayaPythonCExtMethods,
								MAYA_PYTHON_C_EXT_DOCSTRING);
#elif PY_MAJOR_VERSION == 3
		// https://github.com/LinuxCNC/linuxcnc/issues/825
		static PyModuleDef hello_module = {
			PyModuleDef_HEAD_INIT,
			"mll_py",					 // Module name to use with Python import statements
			MAYA_PYTHON_C_EXT_DOCSTRING, // Module description
			0,
			mayaPythonCExtMethods // Structure that defines the methods of the module
		};

		module = PyModule_Create(&hello_module);
		PyObject *sys_modules = PyImport_GetModuleDict();
		PyDict_SetItemString(sys_modules, "mll_py", module);
#endif

		MGlobal::displayInfo("Registered Python bindings!");

		if (module == NULL)
		{
			return MStatus::kFailure;
		}
		Py_INCREF(module);

		PyGILState_Release(pyGILState);
	}

	return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	Py_DECREF(module);

	return status;
}
