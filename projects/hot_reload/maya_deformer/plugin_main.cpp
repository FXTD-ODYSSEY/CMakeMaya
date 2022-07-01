#include "plugin_main.h"
#include "deformer_platform.h"

// NOTE assign memory
MString kPluginLogicLibraryPath;
DeformerLogicLibrary kLogicLibrary;

const char *kAUTHOR = "Me, the author";
const char *kVERSION = "1.0.0";
const char *kREQUIRED_API_VERSION = "Any";

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, kAUTHOR, kVERSION, kREQUIRED_API_VERSION);

	MString pluginPath = plugin.loadPath();
	const char *pluginPathC = pluginPath.asChar();
	const sizet lenPluginPath = strlen(pluginPathC);
	char OSPluginPath[kMaxPathLen];
	strncpy(OSPluginPath, pluginPathC, lenPluginPath + 1);
	int replaced = convertPathSeparatorsToOSNative(OSPluginPath);
	if (replaced < 0)
	{
		return MStatus::kFailure;
	}
	if (strlen(OSPluginPath) <= 0)
	{
		return MStatus::kFailure;
	}

	kPluginLogicLibraryPath = getDeformerLogicLibraryPath(OSPluginPath);

	status = plugin.registerNode(kHotReloadableDeformerName,
								 kHotReloadableDeformerID,
								 &HotReloadableDeformer::creator,
								 &HotReloadableDeformer::initialize,
								 MPxNode::kGeometryFilter);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status;

	if (kLogicLibrary.isValid && kLogicLibrary.handle)
	{
		unloadDeformerLogicDLL(kLogicLibrary);
	}

	status = plugin.deregisterNode(kHotReloadableDeformerID);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

