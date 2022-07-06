#include "maya_python_c_ext_py_hello_world.h"
#include "maya_python_c_ext_py_hello_world.cpp"

static const char MAYA_PYTHON_C_EXT_DOCSTRING[] = "An example Python C extension that makes use of Maya functionality.";


// NOTE: (sonictk) This declares the available methods for the module
static PyMethodDef mayaPythonCExtMethods[] = {
	{"hello_world_maya", pyHelloWorldMaya, METH_VARARGS, HELLO_WORLD_MAYA_DOCSTRING},
	{NULL, NULL, 0, NULL}	// NOTE: (sonictk) Sentinel value for Python
};

// NOTE: (sonictk) Disable name mangling for Python distutils since it really
// wants C-style compilation of exporting symbols. Also, since it hardcodes the
// name of the symbol that it exports, we have to use the snake-case naming
// for the entry point.
extern "C" PyMODINIT_FUNC initpy_hello()
{
	PyObject *module = Py_InitModule3("py_hello",
									  mayaPythonCExtMethods,
									  MAYA_PYTHON_C_EXT_DOCSTRING);
	if (module == NULL) {
		return;
	}
}
