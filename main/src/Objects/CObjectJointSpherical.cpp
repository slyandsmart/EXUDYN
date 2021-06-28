/** ***********************************************************************************************
* @brief        CObjectJointSpherical implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectJointSpherical.h"
//#include "Utilities/RigidBodyMath.h"

//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectJointSpherical::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, Index itemIndex, bool velocityLevel) const
{
	if (parameters.activeConnector)
	{

		algebraicEquations.SetNumberOfItems(nConstraints);
		LinkedDataVector lambda = markerData.GetLagrangeMultipliers();

		Vector3D cEqu; //constraint equations: relative position or velocities

		if (!velocityLevel)
		{

			//use difference of positions in local coordinates
			cEqu = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); 
		}
		else
		{
			CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
				"CObjectJointSpherical::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

			cEqu = (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);
		}

		//++++++++++++++++++++++++++++++++
		//add constrained axes equations:
		for (Index i = 0; i < 3; i++)
		{
			if (parameters.constrainedAxes[i] == 1)
			{
				algebraicEquations[i] = cEqu[i]; //these are the local constraints
			}
		}
		//add equations for unconstrained axes:
		for (Index i = 0; i < nConstraints; i++)
		{
			if (parameters.constrainedAxes[i] == 0)
			{
				algebraicEquations[i] = lambda[i];
			}
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [lambda0,lambda1]^T = [0,0]^T, means that the current values need to be copied
	}
}


void CObjectJointSpherical::ComputeJacobianAE(ResizableMatrix& jacobian_ODE2, ResizableMatrix& jacobian_ODE2_t, ResizableMatrix& jacobian_ODE1, 
	ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData, Real t, Index itemIndex) const
{
	if (parameters.activeConnector)
	{
		//for free axes, set jacobian_AE diagonal term to 1
		jacobian_AE.SetScalarMatrix(nConstraints, 0.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
		for (Index i = 0; i < nConstraints; i++)
		{
			if (parameters.constrainedAxes[i] == 0) { jacobian_AE(i,i) = 1.; }
		}

		//markerData contains already the correct jacobians ==> transformed to constraint jacobian
		Index nColumnsJac0 = markerData.GetMarkerData(0).positionJacobian.NumberOfColumns();
		Index nColumnsJac1 = markerData.GetMarkerData(1).positionJacobian.NumberOfColumns();
		jacobian_ODE2.SetNumberOfRowsAndColumns(nConstraints, nColumnsJac0 + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
		if (!(parameters.constrainedAxes[0] == 1 && parameters.constrainedAxes[1] == 1 && parameters.constrainedAxes[2] == 1))
		{
			jacobian_ODE2.SetAll(0);
		}

		//global equations
		//vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //local equations (marker0-fixed)
		for (Index i = 0; i < nColumnsJac0; i++)
		{
			if (parameters.constrainedAxes[0]) { jacobian_ODE2(0, i) = -markerData.GetMarkerData(0).positionJacobian(0, i); } //negative sign in marker0 because of eq: (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position)
			if (parameters.constrainedAxes[1]) { jacobian_ODE2(1, i) = -markerData.GetMarkerData(0).positionJacobian(1, i); }
			if (parameters.constrainedAxes[2]) { jacobian_ODE2(2, i) = -markerData.GetMarkerData(0).positionJacobian(2, i); }
		}
		for (Index i = 0; i < nColumnsJac1; i++)
		{
			if (parameters.constrainedAxes[0]) { jacobian_ODE2(0, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(0, i); }
			if (parameters.constrainedAxes[1]) { jacobian_ODE2(1, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(1, i); }
			if (parameters.constrainedAxes[2]) { jacobian_ODE2(2, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(2, i); }
		}
	}
	else
	{
		jacobian_AE.SetScalarMatrix(nConstraints, 1.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
	}
}

JacobianType::Type CObjectJointSpherical::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function + JacobianType::AE_AE + JacobianType::AE_AE_function); //all AE_AE terms needed, because of free axes
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}


//! provide according output variable in "value"
void CObjectJointSpherical::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(markerData.GetMarkerData(0).position); break;
	case OutputVariableType::Velocity: value.CopyFrom(markerData.GetMarkerData(0).velocity); break;
	case OutputVariableType::Displacement: value.CopyFrom(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); break;
	case OutputVariableType::Force:
	{
		value = Vector({ GetCurrentAEcoordinate(0), GetCurrentAEcoordinate(1), GetCurrentAEcoordinate(2) });
		break;
	}
	default:
		SysError("CObjectJointSpherical::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}



