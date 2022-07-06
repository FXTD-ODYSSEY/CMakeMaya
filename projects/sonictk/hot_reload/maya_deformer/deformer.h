#ifndef DEFORMER_H
#define DEFORMER_H

#include <maya/MPxDeformerNode.h>
#include <maya/MTypeId.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>	
#include <maya/MItGeometry.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>

// We'll use these to help us identify the node later on
static const MTypeId kHotReloadableDeformerID = 0x0008002E;
static const char *kHotReloadableDeformerName = "hotReloadableDeformer";

// Remember, in C++, a struct is the same thing as a class, except you type fewer
// access specifiers!
// struct HotReloadableDeformer : MPxGeometryFilter
// {
//     static void *creator();

//     static MStatus initialize();

//     MStatus deform(MDataBlock &block,
//                    MItGeometry &iterator,
//                    const MMatrix &matrix,
//                    unsigned int multiIndex);
// }


class HotReloadableDeformer : public MPxGeometryFilter
{
public:
	static void* creator();
	void postConstructor();
	static MStatus initialize();

	//deformer main function
	virtual MStatus deform(MDataBlock &block,
                   MItGeometry &iterator,
                   const MMatrix &matrix,
                   unsigned int multiIndex);
};


#endif