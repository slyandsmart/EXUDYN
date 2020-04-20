/** ***********************************************************************************************
* @class	    CObjectBody
* @brief		
* @details		Details:
 				- base class for computational body
*
* @author		Gerstmayr Johannes
* @date			2018-05-17 (generated)
* @pre			...
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: missing
* 				
*
* *** Example code ***
*
************************************************************************************************ */
#pragma once

#include "Utilities/ReleaseAssert.h"
#include <initializer_list>
#include "Utilities/BasicDefinitions.h" //defines Real
#include "Utilities/ResizableArray.h" 

#include "Main/OutputVariable.h" 
#include "System/CObject.h" 

class CObjectBodyParameters //will be Autogenerated, in accordance with Python-access
{
};

class CNode;

class CObjectBody: public CObject 
{
protected:
	//ResizableArray<Index> nodes; //done in every object; body have single or several nodes

public:
    ////! get an exact clone of *this, must be implemented in all derived classes! Necessary for better handling in ObjectContainer
    virtual CObjectBody* GetClone() const { return new CObjectBody(*this); }

    virtual CObjectType GetType() const override { return CObjectType::Body; }

	virtual void Print(std::ostream& os) const;

	//local to global coordinates are available in CSystemData
	virtual void GetODE2LocalToGlobalCoordinates(ArrayIndex& ltg) const;

	//!compute local coordinate index (within body coordinates) for a certain local node number
	//!this is inefficient for objects with many nodes and needs to be reimplemented, e.g., in ObjectGenericODE2
	virtual Index GetLocalODE2CoordinateIndexPerNode(Index localNode) const
	{
		Index nn = GetNumberOfNodes();
		Index localCoordinate = 0;
		for (Index i = 0; i < nn; i++)
		{
			if (localNode == i) { return localCoordinate; }
			localCoordinate += GetCNode(i)->GetNumberOfODE2Coordinates();
		}
		CHECKandTHROWstring("CObjectBody::GetLocalCoordinateIndexPerNode: invalid localNode number");
		return 0;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ACCESS FUNCTIONS
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    virtual void GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const { CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetAccessFunctionBody"); }
    virtual void GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, 
									   ConfigurationType configuration, Vector& value) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetOutputVariableBody");
	}

    //basic access function position, which are available in every body; used for connectors
    virtual Vector3D GetPosition(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetPosition");
        return Vector3D({ 0., 0., 0. });
    }

	//basic access function velocity, which are available in every body; used for connectors
	virtual Vector3D GetVelocity(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetVelocity");
		return Vector3D({ 0., 0., 0. });
	}

	//basic access function displacement, which are available in every body; used for connectors
	virtual Vector3D GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetDisplacement");
		return Vector3D({ 0., 0., 0. });
    }

	//! basic access function for rotation of node; returns always a 3D Vector; for rigid bodies, the argument localPosition has no effect
	virtual Matrix3D GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetDisplacement");
		return Matrix3D();
	}

	//! basic access function for angular velocity of node; returns always a 3D Vector; for rigid bodies, the argument localPosition has no effect
	virtual Vector3D GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetDisplacement");
		return Vector3D();
	}

	//! basic access function for angular velocity of node; returns always a 3D Vector; for rigid bodies, the argument localPosition has no effect
	virtual Vector3D GetAngularVelocityLocal(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const {
		CHECKandTHROWstring("ERROR: illegal call to CObjectBody::GetDisplacement");
		return Vector3D();
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Computation FUNCTIONS

	//! compute object massmatrix to massMatrix ==> only possible for bodies!!!
	virtual void ComputeMassMatrix(Matrix& massMatrix) const { CHECKandTHROWstring("ERROR: illegal call to CObject::ComputeMassMatrix"); }

};
