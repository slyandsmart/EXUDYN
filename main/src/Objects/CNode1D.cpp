/** ***********************************************************************************************
* @brief		Implementation for Node1D
*
* @author		Gerstmayr Johannes
* @date			2020-05-26 (generated)
* @pre			...
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: https://github.com/jgerstmayr/EXUDYN
* 				
*
* *** Example code ***
*
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CNode1D.h"

//! this is the local position, the nodal coordinate being the x-coordinate of a 3D-vector
Vector3D CNode1D::GetPosition(ConfigurationType configuration) const
{
	Vector3D pRef3D({ GetReferenceCoordinateVector()[0], 0., 0. });
	if (configuration == ConfigurationType::Reference)
	{
		return pRef3D;
	}

	pRef3D[0] += GetCoordinateVector(configuration)[0];
	return pRef3D;
}

//! this is the local velocity, the nodal coordinate being the x-coordinate of a 3D-vector
Vector3D CNode1D::GetVelocity(ConfigurationType configuration) const
{
    return Vector3D({ GetCoordinateVector_t(configuration)[0], 0., 0. });
}

//! this is the local acceleration, the nodal coordinate being the x-coordinate of a 3D-vector
Vector3D CNode1D::GetAcceleration(ConfigurationType configuration) const
{
    return Vector3D({ GetCoordinateVector_tt(configuration)[0], 0., 0. });
}

//! provide according output variable in "value"
void CNode1D::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	//case OutputVariableType::Position: value.CopyFrom(GetPosition(configuration)); break;
	//case OutputVariableType::Displacement: value.CopyFrom(GetPosition(configuration) - GetPosition(ConfigurationType::Reference)); break;
	//case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(configuration)); break;
	case OutputVariableType::Coordinates:
	{
		if (IsValidConfiguration(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Reference + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNode1D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_t:
	{
		if (IsValidConfigurationButNotReference(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector_t(configuration);
		}
		else
		{
			PyError("CNode1D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_tt:
	{
		if (IsValidConfigurationButNotReference(configuration)) 
		{
			value = GetCoordinateVector_tt(configuration);
		}
		else
		{
			PyError("CNode1D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNode1D::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

