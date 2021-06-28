/** ***********************************************************************************************
* @brief		Implementation for computational sensors (collective implementation file for GetSensorValues functions);
*               This file covers implementation parts of loads, specifically python bindings
*
* @author		Gerstmayr Johannes
* @date			2020-01-24 (generated)
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

//#include "Main/CSystemData.h"
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Utilities/RigidBodyMath.h"

#include "Autogenerated/CSensorNode.h"
#include "Autogenerated/CSensorObject.h"
#include "Autogenerated/CSensorBody.h"
#include "Autogenerated/CSensorSuperElement.h"
#include "Autogenerated/MainSensorMarker.h"
#include "Autogenerated/MainSensorLoad.h"
#include "Autogenerated/MainSensorUserFunction.h"

py::object MainSensor::GetSensorValues(const CSystemData& cSystemData, ConfigurationType configuration) const
{
	Vector value;
	GetCSensor()->GetSensorValues(cSystemData, value, configuration);

	//check if it is scalar or a vector-valued:
	if (value.NumberOfItems() == 1) { return py::float_(value[0]); }
	else { return py::array_t<Real>(value.NumberOfItems(), value.GetDataPointer()); }

}


//! main function to generate sensor output values
void CSensorNode::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	const CNode& cNode = cSystemData.GetCNode(parameters.nodeNumber);
	cNode.GetOutputVariable(parameters.outputVariableType, configuration, values);
}

//! main function to generate sensor output values
void CSensorObject::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	const CObject* cObject = cSystemData.GetCObjects()[parameters.objectNumber];
	if (((Index)cObject->GetType() & (Index)CObjectType::Connector) == 0)
	{
		//must be object ==> may leed to illegal call, if not implemented
		cObject->GetOutputVariable(parameters.outputVariableType, values);
	}
	else
	{
		const CObjectConnector* cConnector = (const CObjectConnector*)cObject;

		MarkerDataStructure markerDataStructure;
		const bool computeJacobian = false; //not needed for OutputVariables
		cSystemData.ComputeMarkerDataStructure(cConnector, computeJacobian, markerDataStructure);

		cConnector->GetOutputVariableConnector(parameters.outputVariableType, markerDataStructure, parameters.objectNumber, values);

	}
}

//! main function to generate sensor output values
void CSensorBody::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	const CObjectBody& cObject = cSystemData.GetCObjectBody(parameters.bodyNumber);
	cObject.GetOutputVariableBody(parameters.outputVariableType, parameters.localPosition, configuration, values, parameters.bodyNumber);
}

//! main function to generate sensor output values
void CSensorSuperElement::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	const CObjectSuperElement* cObject = (const CObjectSuperElement*)(cSystemData.GetCObjects()[parameters.bodyNumber]);
	cObject->GetOutputVariableSuperElement(parameters.outputVariableType, parameters.meshNodeNumber, configuration, values);
}


//! main function to generate sensor output values
void CSensorLoad::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	Real time = cSystemData.GetCData().GetCurrent().GetTime();
	const CLoad& cLoad = *(cSystemData.GetCLoads()[parameters.loadNumber]);

	if (cLoad.IsVector()) //vector
	{
		values.CopyFrom(cLoad.GetLoadVector(cSystemData.GetMainSystemBacklink(), time));
	}
	else //scalar
	{
		values.CopyFrom(Vector1D(cLoad.GetLoadValue(cSystemData.GetMainSystemBacklink(), time)));
	}
}

//! main function to generate sensor output values
void CSensorMarker::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	Real time = cSystemData.GetCData().GetCurrent().GetTime();

	const CMarker& cMarker = cSystemData.GetCMarker(parameters.markerNumber);
	bool computeJacobian = false;
	MarkerData markerData;
	cMarker.ComputeMarkerData(cSystemData, computeJacobian, markerData);
	if (configuration != ConfigurationType::Current)
	{
		SysError("SensorMarker: GetSensorValues: marker values are only available at current configuration!");
	}

	//at this place, the outputVariableType has been checked against the marker and data should be written into the according markerData entry
	switch (parameters.outputVariableType)
	{
	case OutputVariableType::Position: values.CopyFrom(markerData.position); break;
	case OutputVariableType::Velocity:
	{
		CHECKandTHROW(markerData.velocityAvailable, "SensorMarker: OutputVariableType::Velocity: velocityLevel not available for Marker");
		values.CopyFrom(markerData.velocity); break;
	}
	case OutputVariableType::Rotation: {
		Vector3D rot = RigidBodyMath::RotationMatrix2RotXYZ(markerData.orientation);
		values.CopyFrom(rot);
		break;
	}
	case OutputVariableType::AngularVelocityLocal: {
		CHECKandTHROW(markerData.velocityAvailable, "SensorMarker: OutputVariableType::AngularVelocityLocal: velocityLevel not available for Marker");
		values.CopyFrom(markerData.angularVelocityLocal); break;
		break;
	}
	default:
		SysError("SensorMarker: failed"); //error should not occur, because types are checked!
	}
}

//! main function to generate sensor output values
void CSensorUserFunction::GetSensorValues(const CSystemData& cSystemData, Vector& values, ConfigurationType configuration) const
{
	Real time = cSystemData.GetCData().Get(configuration).GetTime();
	
	EvaluateUserFunction(values, cSystemData.GetMainSystemBacklink(), time, configuration);
}



