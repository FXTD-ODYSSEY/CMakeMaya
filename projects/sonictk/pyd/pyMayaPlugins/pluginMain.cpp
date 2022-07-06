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

		module = Py_InitModule3("mll_py",
								mayaPythonCExtMethods,
								MAYA_PYTHON_C_EXT_DOCSTRING);

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
