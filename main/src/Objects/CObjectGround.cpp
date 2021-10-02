/** ***********************************************************************************************
* @brief        CObjectGround implementation
*
* @author       Gerstmayr Johannes
* @date         2019-04-29 (generated)
* @date         2019-04-29 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectGround.h"

////for CallFunction(...)
//#include "Main/MainSystem.h"
//#include "Pymodules/PybindUtilities.h"
//#include "Autogenerated/MainObjectGround.h"

//! Computational function: compute mass matrix
void CObjectGround::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	//Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	//massMatrix.SetNumberOfRowsAndColumns(0, 0);

	massMatrixC.SetUseDenseMatrix(false); //this is sufficient to signal that sparse mode is used

}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectGround::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	ode2Lhs.SetNumberOfItems(0);
}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectGround::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::AngularVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in "value" according to object access
void CObjectGround::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
		value.SetNumberOfRowsAndColumns(0, 0); // no action induced from ground joint!
		break;
	case AccessFunctionType::AngularVelocity_qt:
		value.SetNumberOfRowsAndColumns(0, 0); // no action induced from ground joint!
		break;
	case AccessFunctionType::DisplacementMassIntegral_q:
		value.SetNumberOfRowsAndColumns(0, 0); // no action induced from ground joint!
		break;
	default:
		SysError("CObjectGround:GetAccessFunctionBody illegal accessType");
	}
}

//! provide according output variable in "value"
void CObjectGround::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value, Index objectNumber) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetDisplacement(localPosition, configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocity(localPosition, configuration)); break;
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = GetRotationMatrix(localPosition, configuration);
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	default:
		SysError("CObjectGround::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//!  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectGround::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return localPosition + parameters.referencePosition; //always the reference position!
}







////! call a certain function of object (autogenerated in future!)
//py::object MainObjectGround::CallFunction(STDstring functionName, py::dict args) const
//{
//	//these calls should be automated by python script ...
//	if (functionName == "GetTypeName")
//	{
//		return py::str(GetTypeName());
//	}
//	else if (functionName == "GetType")
//	{
//		return py::cast(GetCObjectBody()->GetType());
//	}
//	else if (functionName == "GetOutputVariableTypes")
//	{
//		return py::cast(GetCObject()->GetOutputVariableTypes());
//	}
//	else if (functionName == "GetPosition")
//	{
//		Vector3D locPos;
//		STDstring configStr;
//		ConfigurationType configType = ConfigurationType::_None;
//		EPyUtils::SetVector3DSafely(args, "localPosition", locPos);
//		EPyUtils::SetStringSafely(args, "configuration", configStr);
//		if (configStr == "Current") { configType = ConfigurationType::Current; }
//		else if (configStr == "Initial") { configType = ConfigurationType::Initial; }
//		else if (configStr == "Reference") { configType = ConfigurationType::Reference; }
//
//		Vector3D v(GetCObjectBody()->GetPosition(locPos, configType));
//		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
//	}
//	else if (functionName == "GetNumberOfNodes")
//	{
//		return py::int_(GetCObjectBody()->GetNumberOfNodes());
//	}
//	else if (functionName == "GetODE2Size")
//	{
//		return py::int_(GetCObject()->GetODE2Size());
//	}
//	else if (functionName == "GetNumberOfCoordinates")
//	{
//		return py::int_(GetCObject()->GetNumberOfCoordinates());
//	}
//	PyError(STDstring("MainObjectGround::CallFunction called with invalid functionName '" + functionName + "'"));
//	return py::int_(EXUstd::InvalidIndex);
//}
