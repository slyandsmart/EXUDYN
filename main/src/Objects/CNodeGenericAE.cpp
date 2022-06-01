/** ***********************************************************************************************
* @brief		Implementation for CNodeGenericAE
*
* @author		Gerstmayr Johannes
* @date			2019-05-02 (generated)
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
#include "Autogenerated/CNodeGenericAE.h"


//! provide according output variable in "value"
void CNodeGenericAE::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Coordinates:
	{
		if (IsConfigurationInitialCurrentReferenceVisualization(configuration))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodeGenericAE::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodeGenericAE::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

