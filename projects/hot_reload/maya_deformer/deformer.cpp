#include "deformer.h"
#include "deformer_platform.h"

MString kPluginLogicLibraryPath;
DeformerLogicLibrary kLogicLibrary;

void HotReloadableDeformer::postConstructor()
{
	MGlobal::displayInfo("postConstructor kPluginLogicLibraryPath: "+kPluginLogicLibraryPath);
    // printf("postConstructor %d\n",&kPluginLogicLibraryPath);
    LibraryStatus result = loadDeformerLogicDLL(kLogicLibrary);
    if (result != LibraryStatus_Success)
    {
        MGlobal::displayError("Failed to load shared library!");
        return;
    }

    return;
}

void *HotReloadableDeformer::creator()
{
    return new HotReloadableDeformer;
}

MStatus HotReloadableDeformer::initialize()
{
    MStatus result;
    attributeAffects(envelope, outputGeom);

    return result;
}

MStatus HotReloadableDeformer::deform(MDataBlock &block,
                                      MItGeometry &iter,
                                      const MMatrix &matrix,
                                      unsigned int multiIndex)
{
    LibraryStatus status;

    if (!kLogicLibrary.isValid) {
        unloadDeformerLogicDLL(kLogicLibrary);
        status = loadDeformerLogicDLL(kLogicLibrary);
        if (status != LibraryStatus_Success) {
	        MGlobal::displayInfo("load fail");
            return MStatus::kFailure;
        }
    }

    if (kPluginLogicLibraryPath.numChars() == 0) {
        MGlobal::displayInfo("path null");
        return MStatus::kFailure;
    }

    FileTime lastModified = getLastWriteTime(kPluginLogicLibraryPath.asChar());
    if (lastModified >= 0 && lastModified != kLogicLibrary.lastModified) {
        status = unloadDeformerLogicDLL(kLogicLibrary);
        if (status != LibraryStatus_Success) {
	        MGlobal::displayInfo("unload fail");
            // return MStatus::kFailure;
        }
        status = loadDeformerLogicDLL(kLogicLibrary);
        if (status != LibraryStatus_Success) {
	        MGlobal::displayInfo("load fail");
            return MStatus::kFailure;
        }
    }

    MStatus result;

    MDataHandle envelopeHandle = block.inputValue(envelope, &result);
    CHECK_MSTATUS_AND_RETURN_IT(result);

    float envelope = envelopeHandle.asFloat();

    for (; !iter.isDone(); iter.next())
    {

        MPoint curPtPosPt = iter.position();
        MPoint finalPosPt = (MPoint)kLogicLibrary.deformCB(MVector(curPtPosPt), envelope);

        iter.setPosition(finalPosPt);
    }

    return result;
}
