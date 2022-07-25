#include "testDeformer.h"

#include <maya/MFnPlugin.h>
#include <Python.h>
#include <maya/MGlobal.h>
#include <stdio.h>

#if PY_MAJOR_VERSION == 2
extern "C" PyMODINIT_FUNC initpy_deformer()
#elif PY_MAJOR_VERSION == 3
extern "C" PyMODINIT_FUNC PyInit_py_deformer()
#endif
{
	// NOTE: make sure matrixNodes loaded.
	MGlobal::executePythonCommand("from maya import cmds\nif not cmds.pluginInfo('matrixNodes',q=1,l=1):cmds.loadPlugin('matrixNodes.mll')");
	MObject obj = MFnPlugin::findPlugin("matrixNodes");
	MFnPlugin fnPlugin(obj, "Any", "1.0", "Any");
	fnPlugin.registerNode("myDeformer", MyDeformer::id, MyDeformer::creator, MyDeformer::initialize, MPxNode::kDeformerNode);
	MGlobal::displayInfo("register myDeformer node");
#if PY_MAJOR_VERSION == 3
	return NULL;
#endif
}
