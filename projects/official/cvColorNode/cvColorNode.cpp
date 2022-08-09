//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
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
// Produces the dependency graph node "cvColor".
// 
// This example provides an example of how to color the CVs of a NURBS surface
// by drawing points on top of them with a custom MPxDrawOverride (cvColorDrawOverride). 
// This node implements a locator that is intended to be made a sibling of a NURBS surface 
// shape and sits under the same transform. If the "local" space output attribute of the 
// shape is connected to the "inputSurface" attribute of the locator node, then the 
// cvColorDrawOverride will draw colored points at each CV location. The current algorithm 
// used by cvColorDrawOverride will color the CVs in one of the four different colors 
// based on their XY location:
// 
// x < 0 && y < 0: Red x < 0 && y >= 0: Cyan x >= 0 && y < 0: Blue x >= 0 && y >= 0: Yellow
//
// To use this plug-in:
//	Load the plug-in and then execute the command "source cvColorNode" to define the MEL command "attachColorNode".
//	This command iterates across the selected objects and attaches a "cvColor" node, as described above, to each
//	NURBS surface it encounters. Moving the objects or its CVs after the node is attached will cause the colors
//	of the CVs to change. The "pointSize" attribute of the node controls the size of the point that is drawn.
//	If the "drawingEnabled" attribute is set to false, it will disable the display of the colored points.  
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxLocatorNode.h> 
#include <maya/MPlug.h>
#include <maya/MMatrix.h>
#include <maya/MPointArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MDrawRegistry.h>
#include <maya/MColorArray.h>


class cvColor : public MPxLocatorNode
{
public:
					cvColor() {}
					~cvColor() override {}

    MStatus   		compute( const MPlug& plug, MDataBlock& data ) override;

	bool            isBounded() const override;

	static  void *  creator();
	static  MStatus initialize();

	static	MTypeId	id;
	static	MObject	drawingEnabled;
	static	MObject	pointSize;
	static  MObject inputSurface;
	static	MObject	cvLocations;
};


//
// Custom MUserData to cache the data used by cvColorDrawOverride.
//
class cvColorDrawOverrideData : public MUserData
{
public:

	cvColorDrawOverrideData() : MUserData(false) {}
	~cvColorDrawOverrideData() override{}

	bool        enableDrawing = true;
	MPointArray pointArray;
	MMatrix     worldSpace;
	float       pointSize = 4.0f;
};


//
// Implementation of custom MPxDrawOverride to draw the control vertices of a NURBS in the Viewport 2.0.
//
class  cvColorDrawOverride : public  MHWRender::MPxDrawOverride
{
public:
	static MPxDrawOverride* Creator(const MObject& obj)
	{
		return new cvColorDrawOverride(obj);
	}

	~cvColorDrawOverride() override{}

	MHWRender::DrawAPI supportedDrawAPIs() const override{ return MHWRender::kAllDevices; }

	bool isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override{ return false; }

	MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData) override;

	// Return true so addUIDrawables() will be called by the Viewport 2.0. 
	bool hasUIDrawables() const override{ return true; }

	void addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) override;

private:
	cvColorDrawOverride(const MObject& obj);
};



//
// cvColor implementation
//

MTypeId cvColor::id( 0x80006 );
MObject cvColor::inputSurface;
MObject cvColor::cvLocations;
MObject cvColor::pointSize;
MObject cvColor::drawingEnabled;


MStatus cvColor::compute( const MPlug& plug, MDataBlock& data )
{ 
	MStatus stat;

	// cout << "cvColor::compute\n";

	if ( plug == cvLocations ) {
		MDataHandle inputData = data.inputValue ( inputSurface, &stat );
		if (!stat) {
			stat.perror("cvColor::compute get inputSurface");
			return stat;
		}

		MObject surf = inputData.asNurbsSurface();
		MFnNurbsSurface surfFn (surf, &stat);
		if (!stat) {
			stat.perror("cvColor::compute surface creator");
			return stat;
		}

		MDataHandle outputData = data.outputValue ( cvLocations, &stat );
		if (!stat) {
			stat.perror("cvColor::compute get cvLocations");
			return stat;
		}
		
		MObject cvs = outputData.data();
		MFnPointArrayData cvData(cvs, &stat);
		if (!stat) {
			stat.perror("cvColor::compute point array data creator");
			return stat;
		}

		MPointArray cvArray;
		stat = surfFn.getCVs( cvArray, MSpace::kObject);
		if (!stat) {
			stat.perror("cvColor::compute getCVs");
			return stat;
		}

		stat = cvData.set ( cvArray );
		if (!stat) {
			stat.perror("cvColor::compute setCVs");
			return stat;
		}

		outputData.set ( cvs );

		stat = data.setClean ( plug );
		if (!stat) {
			stat.perror("cvColor::compute setClean");
			return stat;
		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

bool cvColor::isBounded() const
{ 
	return false;
}

void* cvColor::creator()
{
	return new cvColor();
}

MStatus cvColor::initialize()
{ 
	MStatus				stat;
	MFnNumericAttribute	numericAttr;
	MFnTypedAttribute	typedAttr;

	drawingEnabled = numericAttr.create( "drawingEnabled", "en",
									MFnNumericData::kBoolean, 1, &stat );
	if (!stat) {
		stat.perror("create drawingEnabled attribute");
		return stat;
	}

	pointSize = numericAttr.create( "pointSize", "ps",
									MFnNumericData::kFloat, 4.0, &stat );
	if (!stat) {
		stat.perror("create pointSize attribute");
		return stat;
	}
	
	inputSurface = typedAttr.create( "inputSurface", "is",
							 MFnNurbsSurfaceData::kNurbsSurface, MObject::kNullObj, &stat);
	if (!stat) {
		stat.perror("create inputSurface attribute");
		return stat;
	}

	cvLocations = typedAttr.create( "cvLocations", "cv",
							 MFnPointArrayData::kPointArray, MObject::kNullObj, &stat);
	if (!stat) {
		stat.perror("create cvLocations attribute");
		return stat;
	}

	MPointArray			defaultPoints;
	MFnPointArrayData	defaultArray;
	MObject				defaultAttr;

	defaultPoints.clear(); // Empty array
	defaultAttr = defaultArray.create (defaultPoints);
	stat = typedAttr.setDefault(defaultAttr);
	if (!stat) {
		stat.perror("could not create default output attribute");
		return stat;
	}

	stat = addAttribute (drawingEnabled);
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute (pointSize);
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute (inputSurface);
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute (cvLocations);
		if (!stat) { stat.perror("addAttribute"); return stat;}

	stat = attributeAffects( inputSurface, cvLocations );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( drawingEnabled, cvLocations );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( pointSize, cvLocations );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	
	return MS::kSuccess;
}




//
// cvColorDrawOverride implementation
//


cvColorDrawOverride::cvColorDrawOverride(const MObject& obj)
	: MHWRender::MPxDrawOverride(obj, nullptr, false/*isAlwaysDirty*/)
{

}



MUserData* cvColorDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MFrameContext& frameContext, MUserData* oldData)
{
	// Called by Maya whenever the object is dirty and needs to update for draw.
	// Any data needed from the Maya dependency graph must be retrieved and cached
	// in this stage. It is invalid to pull data from the Maya dependency graph in
	// the draw callback method and Maya may become unstable if that is attempted.

	
	// Retrieve data cache (create if does not exist)
	cvColorDrawOverrideData* cvColorData = dynamic_cast<cvColorDrawOverrideData*>(oldData);
	if(!cvColorData)
	{
		cvColorData = new cvColorDrawOverrideData();
	}
	
	MObject locatorNode = objPath.node();

	//
	// Get the drawingEnabled attribute value
	//
	MPlug drawingEnabledPlug(locatorNode, cvColor::drawingEnabled);
	MStatus stat = drawingEnabledPlug.getValue(cvColorData->enableDrawing);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get drawingEnabled failed !");
	}


	//
	// Get the pointSize attribute value
	//
	MPlug pointSizePlug(locatorNode, cvColor::pointSize);
	stat = pointSizePlug.getValue(cvColorData->pointSize);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get pointSize failed !");
	}

	//
	// Get the cvLocations attribute value
	//
	MPlug cvLocationsPlug(locatorNode, cvColor::cvLocations);
	MObject cvLocationsObject;
	stat = cvLocationsPlug.getValue(cvLocationsObject);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw  get cvObject failed !");
	}

	MFnPointArrayData cvData(cvLocationsObject, &stat);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get point array data failed !");
	}

	cvColorData->pointArray = cvData.array(&stat);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw  get point array failed !");
	}

	//
	// Extract the 'worldMatrix' attribute that is inherited from 'dagNode'
	//
	MFnDependencyNode fnLocatorNode(locatorNode);
	MObject worldSpaceAttribute = fnLocatorNode.attribute("worldMatrix");
	MPlug matrixPlug(locatorNode, worldSpaceAttribute);

	//
	// 'worldMatrix' is an array so we must specify which element the plug
	// refers to
	matrixPlug = matrixPlug.elementByLogicalIndex(0);

	//
	// Get the value of the 'worldMatrix' attribute
	//
	MObject matObject;
	stat = matrixPlug.getValue(matObject);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get matObject failed !");
	}

	MFnMatrixData matrixData(matObject, &stat);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get world matrix data failed !");
	}

	cvColorData->worldSpace = matrixData.matrix(&stat);
	if (!stat) {
		stat.perror("cvColorDrawOverride::prepareForDraw get world matrix failed !");
	}

	return cvColorData;
}



void cvColorDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
	cvColorDrawOverrideData* cvColorData = (cvColorDrawOverrideData*)data;
	if (!cvColorData)
		return;

	if (!cvColorData->enableDrawing)
		return;

	//
	// Build the color array
	// Each vertex color is based on the vertex world position
	//
	const MColor red(1.0, 0.0f, 0.0f);
	const MColor cyan(0.0, 1.0f, 1.0f);
	const MColor blue(0.0, 0.0f, 1.0f);
	const MColor yellow(1.0, 1.0f, 0.0f);

	MColorArray colorArray;
	colorArray.setLength(cvColorData->pointArray.length());
	for(unsigned int i = 0; i < cvColorData->pointArray.length(); i++)
	{
		MPoint point = cvColorData->pointArray[i];
		
		point *= cvColorData->worldSpace;

		if (point.x < 0 && point.y < 0)
		{
			colorArray[i] = red;
		}
		else if (point.x < 0 && point.y >= 0)
		{
			colorArray[i] = cyan;
		}
		else if (point.x >= 0 && point.y < 0)
		{
			colorArray[i] = blue;
		}
		else if (point.x >= 0 && point.y >= 0)
		{
			colorArray[i] = yellow;
		}
	}



	drawManager.beginDrawable();

	drawManager.setPointSize(cvColorData->pointSize);
	
	drawManager.mesh(MHWRender::MUIDrawManager::Primitive::kPoints, cvColorData->pointArray, nullptr/*normals*/, &colorArray, nullptr/*indices*/, nullptr/*textCoords*/);

	drawManager.endDrawable();
}







namespace 
{
	const MString gDrawDbClassification("drawdb/geometry/cvColorDrawOverride");
	const MString gDrawRegistrantId("cvColorPlugin");
}


MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "cvColor", cvColor::id, 
						 &cvColor::creator, &cvColor::initialize,
						 MPxNode::kLocatorNode, &gDrawDbClassification);
	if (!status) {
		status.perror("registerNode");
		return status;
	}


	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
		gDrawDbClassification,
		gDrawRegistrantId,
		cvColorDrawOverride::Creator);
	if (!status) {
		status.perror("registerDrawOverrideCreator");
		return status;
	}


	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( cvColor::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(gDrawDbClassification, gDrawRegistrantId);
	if (!status) {
		status.perror("deregisterDrawOverrideCreator");
		return status;
	}

	return status;
}
