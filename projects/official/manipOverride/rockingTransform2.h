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
#include <maya/MMatrix.h>
#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>


//
// Define declarations
//

#define kRockingTransformNodeID 0x8110B
#define kRockingTransformMatrixID 0x8110C

#define ReturnOnError(status)			   \
		if (MS::kSuccess != status) {	   \
			return status;				  \
		}

//
// Class declarations -- matrix and transform
//

class rockingTransformMatrix : public MPxTransformationMatrix
{
	// A really simple implementation of MPxTransformationMatrix.
	// The methods include:
	// - Two accessor methods for getting and setting the 
	// rock
	// - The virtual asMatrix() method which passes the matrix 
	// back to Maya when the command "xform -q -ws -m" is invoked

	public:
		rockingTransformMatrix();
		static MPxTransformationMatrix *creator();
		
		MMatrix asMatrix() const override;
		MMatrix	asMatrix(double percent) const override;
		MMatrix	asRotateMatrix() const override;
		
		// Degrees
		double	getRockInX() const;
		void	setRockInX( double rock );
		
		static	MTypeId	id;
	protected:		
		typedef MPxTransformationMatrix ParentClass;
		// Degrees
		double rockXValue;
};

class rockingTransformNode : public MPxTransform 
{
	// A really simple custom transform.
	public:
		rockingTransformNode();
		~rockingTransformNode() override;

		MPxTransformationMatrix *createTransformationMatrix() override;
					
		void postConstructor() override;

		MStatus validateAndSetValue(const MPlug&, const MDataHandle&) override;
					
		void  resetTransformation (MPxTransformationMatrix *) override;
		void  resetTransformation (const MMatrix &) override;
					
		// Utility for getting the related rock matrix pointer
		rockingTransformMatrix *getRockingTransformMatrix();
				
		const char* className();
		static	void * 	creator();
		static  MStatus	initialize();
		
		static	MTypeId	id;
	protected:
		// Degrees
		static	MObject aRockInX;
		double rockXValue;
		typedef MPxTransform ParentClass;
};

class DegreeRadianConverter
{
	public:
		double degreesToRadians( double degrees );
		double radiansToDegrees( double radians );
};
