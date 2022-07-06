//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Produces the dependency graph node "sine".
//
// This plug-in is a simpler version of circleNode.
// It takes a single input value and outputs the sine of this multiplied by 10.
// It is a simple example of creating a procedural animation.
// Below are a simple set of commands to attach this node to a sphere:
//
//	sphere -n sphere;
//	createNode sine -n sine1;
//	connectAttr sine1.output sphere.tx;
//	connectAttr time1.outTime sine1.input;
//
// After this is done and as time changes, the sphere will roughly move along the
// X axis in a periodic manner (Roughly because the input is normally 1 through 48
// and the compute method of the sine node just takes the sine of the input,
// which must be radians).
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

class sine : public MPxNode
{
public:
	sine();
	~sine() override;

	MStatus compute(const MPlug &plug, MDataBlock &data) override;

	static void *creator();
	static MStatus initialize();

public:
	static MObject input;  // The input value.
	static MObject output; // The output value.
	static MTypeId id;
};

MTypeId sine::id(0x80012);
MObject sine::input;
MObject sine::output;

sine::sine() {}
sine::~sine() {}

MStatus sine::compute(const MPlug &plug, MDataBlock &data)
{

	MStatus returnStatus;

	if (plug == output)
	{
		MDataHandle inputData = data.inputValue(input, &returnStatus);

		if (returnStatus != MS::kSuccess)
			cerr << "ERROR getting data" << endl;
		else
		{
			float result = sinf(inputData.asFloat()) * 10.0f;
			MDataHandle outputHandle = data.outputValue(sine::output);
			outputHandle.set(result);
			data.setClean(plug);
		}
	}
	else
	{
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void *sine::creator()
{
	return new sine();
}

MStatus sine::initialize()
{
	MFnNumericAttribute nAttr;
	MStatus stat;

	input = nAttr.create("input", "in", MFnNumericData::kFloat, 0.0);
	nAttr.setStorable(true);

	output = nAttr.create("output", "out", MFnNumericData::kFloat, 0.0);
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	stat = addAttribute(input);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}
	stat = addAttribute(output);
	if (!stat)
	{
		stat.perror("addAttribute");
		return stat;
	}

	stat = attributeAffects(input, output);
	if (!stat)
	{
		stat.perror("attributeAffects");
		return stat;
	}

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode("sine", sine::id, sine::creator,
								 sine::initialize);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(sine::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
