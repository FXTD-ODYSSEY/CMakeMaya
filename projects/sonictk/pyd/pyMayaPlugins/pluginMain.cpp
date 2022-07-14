#include "pythonModule.h"
#include <maya/MFnPlugin.h>

const char *kAUTHOR = "TimmyLiang";
const char *kVERSION = "1.0.0";
const char *kREQUIRED_API_VERSION = "Any";

PyObject *module = NULL;

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
		// TODO not importable
		static PyModuleDef hello_module = {
			PyModuleDef_HEAD_INIT,
			"mll_py",					 // Module name to use with Python import statements
			MAYA_PYTHON_C_EXT_DOCSTRING, // Module description
			0,
			mayaPythonCExtMethods // Structure that defines the methods of the module
		};

		module = PyModule_Create(&hello_module);
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