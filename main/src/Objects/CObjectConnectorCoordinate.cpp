/** ***********************************************************************************************
* @brief        CObjectConnectorCoordinate implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-14 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorCoordinate.h"


//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectConnectorCoordinate::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, Index itemIndex, bool velocityLevel) const
{
	if (parameters.activeConnector)
	{
		if (!velocityLevel && !parameters.velocityLevel)
		{
			//this is the usual way: the marker measures the according value (could be a velocity)
			algebraicEquations.SetNumberOfItems(1);
			Real offset = parameters.offset;
			if (parameters.offsetUserFunction)
			{
				EvaluateUserFunctionOffset(offset, cSystemData->GetMainSystemBacklink(), t, itemIndex);
			}


			algebraicEquations[0] = markerData.GetMarkerData(1).vectorValue[0] * parameters.factorValue1 - markerData.GetMarkerData(0).vectorValue[0] - offset;
		}
		else
		{
			CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
				"CObjectConnectorCoordinate::ComputeAlgebraicEquations: marker does not provide velocityLevel information");

			algebraicEquations.SetNumberOfItems(1);
			algebraicEquations[0] = markerData.GetMarkerData(1).vectorValue_t[0] * parameters.factorValue1 - markerData.GetMarkerData(0).vectorValue_t[0]; //this is the index-reduced equation: does not have offset!!!
			
			if (parameters.offsetUserFunction_t)
			{
				Real offset=0;
				EvaluateUserFunctionOffset_t(offset, cSystemData->GetMainSystemBacklink(), t, itemIndex);
				algebraicEquations[0] -= offset;
			}
			else if (parameters.velocityLevel) { algebraicEquations[0] -= parameters.offset; }
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation lambda = 0, means that the current values need to be copied
	}

}

void CObjectConnectorCoordinate::ComputeJacobianAE(ResizableMatrix& jacobian_ODE2, ResizableMatrix& jacobian_ODE2_t, ResizableMatrix& jacobian_ODE1, 
	ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData, Real t, Index itemIndex) const
{
	if (parameters.activeConnector)
	{
		ResizableMatrix* usedJac;
		if (parameters.velocityLevel) //in this case, always the jacobian_ODE2_t must be used
		{
			usedJac = &jacobian_ODE2_t;
			jacobian_ODE2.SetNumberOfRowsAndColumns(0, 0); //for safety - needed?
		}
		else
		{
			usedJac = &jacobian_ODE2;
			jacobian_ODE2_t.SetNumberOfRowsAndColumns(0, 0); //for safety - needed?
		}

		usedJac->SetNumberOfRowsAndColumns(1, markerData.GetMarkerData(0).jacobian.NumberOfColumns()
			+ markerData.GetMarkerData(1).jacobian.NumberOfColumns());

		usedJac->SetSubmatrix(markerData.GetMarkerData(0).jacobian, 0, 0, -1.);
		usedJac->SetSubmatrix(markerData.GetMarkerData(1).jacobian, 0,
			markerData.GetMarkerData(0).jacobian.NumberOfColumns(), parameters.factorValue1);

	}
	else
	{
		jacobian_AE.SetNumberOfRowsAndColumns(1, 1);
		jacobian_AE(0, 0) = 1; //represents derivative of algebraic equation 'lambdaCoordinate = 0'
	}

}

JacobianType::Type CObjectConnectorCoordinate::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		if (parameters.velocityLevel)
		{
			return (JacobianType::Type)(JacobianType::AE_ODE2_t + JacobianType::AE_ODE2_t_function);
		}
		else
		{
			return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function);
		}
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}


//! provide according output variable in "value"
void CObjectConnectorCoordinate::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	Real relPos = markerData.GetMarkerData(1).vectorValue[0] - markerData.GetMarkerData(0).vectorValue[0];
	Real relVel = markerData.GetMarkerData(1).vectorValue_t[0] - markerData.GetMarkerData(0).vectorValue_t[0]; //this is the index-reduced equation: does not have offset!!!

	switch (variableType)
	{
	case OutputVariableType::Displacement: value = Vector({ relPos }); break;
	case OutputVariableType::Velocity: value = Vector({ relVel }); break;
	case OutputVariableType::ConstraintEquation: 
	{
		Real t = GetCSystemData()->GetCData().GetCurrent().GetTime();
		bool velocityLevel = false;
		Vector algebraicEquations;
		ComputeAlgebraicEquations(algebraicEquations, markerData, t, velocityLevel);
		value.CopyFrom(algebraicEquations); 
		break;
	}
	case OutputVariableType::Force: value.CopyFrom(markerData.GetLagrangeMultipliers()); break;
	default:
		SysError("CObjectConnectorCoordinate::GetOutputVariableConnector failed"); //error should not occur, because types are checked!
	}

}



