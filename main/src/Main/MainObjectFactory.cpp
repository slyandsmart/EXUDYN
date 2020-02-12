/** ***********************************************************************************************
* @brief		ObjectFactory implementation
* @details		Details:
				- creation of objects in Exudyn
*
* @author		Gerstmayr Johannes
* @date			2019-04-19 (generated)
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


#include "Main/MainSystem.h"

#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectMassPoint.h"
#include "Autogenerated/MainObjectMassPoint2D.h"
#include "Autogenerated/MainObjectRigidBody.h"
#include "Autogenerated/MainObjectRigidBody2D.h"
#include "Autogenerated/MainObjectGround.h"

#include "Autogenerated/MainObjectANCFCable2D.h"
#include "Autogenerated/MainObjectALEANCFCable2D.h"

#include "Autogenerated/MainObjectConnectorSpringDamper.h"
#include "Autogenerated/MainObjectConnectorCartesianSpringDamper.h"
#include "Autogenerated/MainObjectConnectorRigidBodySpringDamper.h"
#include "Autogenerated/MainObjectConnectorCoordinateSpringDamper.h"
#include "Autogenerated/MainObjectConnectorDistance.h"
#include "Autogenerated/MainObjectConnectorCoordinate.h"
#include "Autogenerated/MainObjectContactCoordinate.h"
#include "Autogenerated/MainObjectContactCircleCable2D.h"
#include "Autogenerated/MainObjectContactFrictionCircleCable2D.h"
#include "Autogenerated/MainObjectJointGeneric.h"
#include "Autogenerated/MainObjectJointRevolute2D.h"
#include "Autogenerated/MainObjectJointPrismatic2D.h"
#include "Autogenerated/MainObjectJointSliding2D.h"
#include "Autogenerated/MainObjectJointALEMoving2D.h"

#include "Autogenerated/MainNodePoint.h"
#include "Autogenerated/MainNodePointGround.h"
#include "Autogenerated/MainNodePoint2D.h"
#include "Autogenerated/MainNodeRigidBodyEP.h"
#include "Autogenerated/MainNodeRigidBodyRxyz.h"
#include "Autogenerated/MainNodeRigidBodyRotVecLG.h"
#include "Autogenerated/MainNodeRigidBody2D.h"
#include "Autogenerated/MainNodePoint2DSlope1.h"
#include "Autogenerated/MainNodeGenericODE2.h"
#include "Autogenerated/MainNodeGenericData.h"

#include "Autogenerated/MainMarkerBodyPosition.h"
#include "Autogenerated/MainMarkerBodyRigid.h"
#include "Autogenerated/MainMarkerBodyMass.h"
#include "Autogenerated/MainMarkerNodePosition.h"
#include "Autogenerated/MainMarkerNodeRigid.h"
#include "Autogenerated/MainMarkerNodeCoordinate.h"
#include "Autogenerated/MainMarkerBodyCable2DShape.h"
#include "Autogenerated/MainMarkerBodyCable2DCoordinates.h"

#include "Autogenerated/MainLoadForceVector.h"
#include "Autogenerated/MainLoadMassProportional.h"
#include "Autogenerated/MainLoadTorqueVector.h"
#include "Autogenerated/MainLoadCoordinate.h"

#include "Autogenerated/MainSensorNode.h"
#include "Autogenerated/MainSensorBody.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  NODE
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



//! Create a specific node with nodeType; returns node=Null if no success
MainNode* MainObjectFactory::CreateMainNode(MainSystem& mainSystem, STDstring nodeType)
{
	MainNode* node = NULL;
	if (nodeType == "Point")
	{
		CNodePoint* cNode = new CNodePoint();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodePoint(); //new main node
		((MainNodePoint*)node)->SetCNodePoint(cNode);
		VisualizationNodePoint* vNode = new VisualizationNodePoint();				//visualization item
		((MainNodePoint*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "PointGround")
	{
		CNodePointGround* cNode = new CNodePointGround();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodePointGround(); //new main node
		((MainNodePointGround*)node)->SetCNodePointGround(cNode);
		VisualizationNodePointGround* vNode = new VisualizationNodePointGround();				//visualization item
		((MainNodePointGround*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "Point2D")
	{
		CNodePoint2D* cNode = new CNodePoint2D();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodePoint2D(); //new main node
		((MainNodePoint2D*)node)->SetCNodePoint2D(cNode);
		VisualizationNodePoint2D* vNode = new VisualizationNodePoint2D();				//visualization item
		((MainNodePoint2D*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "RigidBodyEP")
	{
		CNodeRigidBodyEP* cNode = new CNodeRigidBodyEP();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeRigidBodyEP(); //new main node
		((MainNodeRigidBodyEP*)node)->SetCNodeRigidBodyEP(cNode);
		VisualizationNodeRigidBodyEP* vNode = new VisualizationNodeRigidBodyEP();				//visualization item
		((MainNodeRigidBodyEP*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "RigidBodyRxyz")
	{
		CNodeRigidBodyRxyz* cNode = new CNodeRigidBodyRxyz();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeRigidBodyRxyz(); //new main node
		((MainNodeRigidBodyRxyz*)node)->SetCNodeRigidBodyRxyz(cNode);
		VisualizationNodeRigidBodyRxyz* vNode = new VisualizationNodeRigidBodyRxyz();				//visualization item
		((MainNodeRigidBodyRxyz*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "RigidBodyRotVecLG")
	{
		CNodeRigidBodyRotVecLG* cNode = new CNodeRigidBodyRotVecLG();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeRigidBodyRotVecLG(); //new main node
		((MainNodeRigidBodyRotVecLG*)node)->SetCNodeRigidBodyRotVecLG(cNode);
		VisualizationNodeRigidBodyRotVecLG* vNode = new VisualizationNodeRigidBodyRotVecLG();				//visualization item
		((MainNodeRigidBodyRotVecLG*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "RigidBody2D")
	{
		CNodeRigidBody2D* cNode = new CNodeRigidBody2D();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeRigidBody2D(); //new main node
		((MainNodeRigidBody2D*)node)->SetCNodeRigidBody2D(cNode);
		VisualizationNodeRigidBody2D* vNode = new VisualizationNodeRigidBody2D();				//visualization item
		((MainNodeRigidBody2D*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "Point2DSlope1")
	{
		CNodePoint2DSlope1* cNode = new CNodePoint2DSlope1();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodePoint2DSlope1(); //new main node
		((MainNodePoint2DSlope1*)node)->SetCNodePoint2DSlope1(cNode);
		VisualizationNodePoint2DSlope1* vNode = new VisualizationNodePoint2DSlope1();				//visualization item
		((MainNodePoint2DSlope1*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "GenericODE2")
	{
		CNodeGenericODE2* cNode = new CNodeGenericODE2();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeGenericODE2(); //new main node
		((MainNodeGenericODE2*)node)->SetCNodeGenericODE2(cNode);
		VisualizationNodeGenericODE2* vNode = new VisualizationNodeGenericODE2();				//visualization item
		((MainNodeGenericODE2*)node)->SetVisualizationNode(vNode);
	}
	if (nodeType == "GenericData")
	{
		CNodeGenericData* cNode = new CNodeGenericData();										//computational item
		cNode->GetCData() = &(mainSystem.GetCSystem()->GetSystemData().GetCData()); //add CData reference to CNode
		node = new MainNodeGenericData(); //new main node
		((MainNodeGenericData*)node)->SetCNodeGenericData(cNode);
		VisualizationNodeGenericData* vNode = new VisualizationNodeGenericData();				//visualization item
		((MainNodeGenericData*)node)->SetVisualizationNode(vNode);
	}
	//else if (nodeType == "NodeRigid") { .. }
	return node;
}

//! check whether dictionary has valid name (return true), or generate a name for certain item (node, object, marker, ...);
//! the generated name is baseItem+string(currentNumber); in case of inconsistencies, errorFound is set to true
bool MainObjectFactory::DictHasValidName(const MainSystem& mainSystem, const py::dict& d, Index currentNumber, const STDstring& baseItem, bool& errorFound)
{
	Index baseItemLength = baseItem.size();
	bool hasName = false; //set to true, if name is already o.k.

	if (d.contains("name"))
	{
				
		if (EPyUtils::DictItemIsValidString(d, "name"))
		{
			STDstring nameString = py::cast<std::string>(d["name"]); //! read out dictionary and cast to C++ type

			if (nameString.size() != 0) //empty string '' shall not raise error, but generate name==>this is the default value for python interface classes
			{
				hasName = true;
				//check if name conflicts with item enumeration (having the according name + a digit)
				if (nameString.size() > baseItemLength && nameString.substr(0, baseItemLength) == baseItem && isdigit(nameString[baseItemLength]))
				{
					STDstring s = baseItem; s[0] = toupper(s[0]);	s = "Add" + s; //generate string for error messages, e.g. "AddNode"
					PyError(s + ", arg:dict['name'] may not have the format '" + baseItem + "[0-9][...]'");
					errorFound = true;
				}

				//check if name exists:
				for (auto item : mainSystem.GetMainSystemData().GetMainNodes())
				{
					if (item->GetName() == nameString)
					{
						STDstring s = baseItem; s[0] = toupper(s[0]);	s = "Add" + s; //generate string for error messages, e.g. "AddNode"
						PyError(s + ", arg:dict['name'] '" + nameString + "' already exists; item not added");
						errorFound = true;
						break;
					}
				}
			}
		}
		else
		{
			STDstring s = baseItem; s[0] = toupper(s[0]);	s = "Add" + s; //generate string for error messages, e.g. "AddNode"
			PyError(s + ", arg:dict['name'] must be a valid string");
			errorFound = true;
		}
	}

	return hasName && !errorFound;
}

Index MainObjectFactory::AddMainNode(MainSystem& mainSystem, py::dict d)
{
	bool errorFound = false;

	STDstring baseItem = "node";
	Index currentNumber = mainSystem.GetMainSystemData().GetMainNodes().NumberOfItems();

	if (!DictHasValidName(mainSystem, d, currentNumber, baseItem, errorFound))
	{
		if (!errorFound) {d["name"] = baseItem + EXUstd::Num2String(currentNumber); }
	}

	if (!errorFound)
	{
		if (d.contains("nodeType"))
		{
			py::object nameItem = d["nodeType"]; //this is necessary to make isinstance work
			if (py::isinstance<py::str>(nameItem))
			{
				STDstring nameString = py::cast<std::string>(nameItem); //!  read out dictionary and cast to C++ type

				MainNode* mainNode = CreateMainNode(mainSystem, nameString);

				if (mainNode != NULL)
				{
					mainNode->SetWithDictionary(d);
					mainSystem.GetCSystem()->GetSystemData().GetCNodes().Append(mainNode->GetCNode());
					mainSystem.GetVisualizationSystem().GetVisualizationSystemData().GetVisualizationNodes().Append(mainNode->GetVisualizationNode());
					return mainSystem.GetMainSystemData().GetMainNodes().Append(mainNode);
				}
				else
				{
					PyError(STDstring("AddNode, arg:dict['nodeType'] contains invalid node type '") + nameString + "'");
				}

			}
			else
			{
				PyError("AddNode, arg:dict['nodeType'] must be of string type");
			}
		}
		else
		{
			PyError("AddNode, arg:dict must contain item 'nodeType'");
		}
	}
	return EXUstd::InvalidIndex;


}





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  OBJECT
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Create a specific object with objectType; returns object=Null if no success
MainObject* MainObjectFactory::CreateMainObject(MainSystem& mainSystem, STDstring objectType)
{
	MainObject* object = NULL;
	if (objectType == "MassPoint")
	{
		CObjectMassPoint* cObject = new CObjectMassPoint();							
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectMassPoint(); //new main object
		((MainObjectMassPoint*)object)->SetCObjectMassPoint(cObject);
		VisualizationObjectMassPoint* vObject = new VisualizationObjectMassPoint();				//visualization item
		((MainObjectMassPoint*)object)->SetVisualizationObject(vObject);
	}
	if (objectType == "MassPoint2D")
	{
		CObjectMassPoint2D* cObject = new CObjectMassPoint2D();							
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectMassPoint2D(); //new main object
		((MainObjectMassPoint2D*)object)->SetCObjectMassPoint2D(cObject);
		VisualizationObjectMassPoint2D* vObject = new VisualizationObjectMassPoint2D();				//visualization item
		((MainObjectMassPoint2D*)object)->SetVisualizationObject(vObject);
	}
	if (objectType == "RigidBody")
	{
		CObjectRigidBody* cObject = new CObjectRigidBody();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectRigidBody(); //new main object
		((MainObjectRigidBody*)object)->SetCObjectRigidBody(cObject);
		VisualizationObjectRigidBody* vObject = new VisualizationObjectRigidBody();				//visualization item
		((MainObjectRigidBody*)object)->SetVisualizationObject(vObject);
	}
	if (objectType == "RigidBody2D")
	{
		CObjectRigidBody2D* cObject = new CObjectRigidBody2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectRigidBody2D(); //new main object
		((MainObjectRigidBody2D*)object)->SetCObjectRigidBody2D(cObject);
		VisualizationObjectRigidBody2D* vObject = new VisualizationObjectRigidBody2D();				//visualization item
		((MainObjectRigidBody2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "Ground")
	{
		CObjectGround* cObject = new CObjectGround();								
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectGround(); //new main object
		((MainObjectGround*)object)->SetCObjectGround(cObject);
		VisualizationObjectGround* vObject = new VisualizationObjectGround();				//visualization item
		((MainObjectGround*)object)->SetVisualizationObject(vObject);
	}
	//+++++++++++++++++++++++++++ FINITE ELEMENTS ++++++++++++++++++++
	if (objectType == "ANCFCable2D")
	{

		CObjectANCFCable2D* cObject = new CObjectANCFCable2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectANCFCable2D(); //new main object
		((MainObjectANCFCable2D*)object)->SetCObjectANCFCable2D(cObject);
		VisualizationObjectANCFCable2D* vObject = new VisualizationObjectANCFCable2D();				//visualization item
		((MainObjectANCFCable2D*)object)->SetVisualizationObject(vObject);
	}
	if (objectType == "ALEANCFCable2D")
	{

		CObjectALEANCFCable2D* cObject = new CObjectALEANCFCable2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectALEANCFCable2D(); //new main object
		((MainObjectALEANCFCable2D*)object)->SetCObjectALEANCFCable2D(cObject);
		VisualizationObjectALEANCFCable2D* vObject = new VisualizationObjectALEANCFCable2D();				//visualization item
		((MainObjectALEANCFCable2D*)object)->SetVisualizationObject(vObject);
	}
	//+++++++++++++++++++++++++++ CONNECTORS +++++++++++++++++++++++++
	else if (objectType == "ConnectorSpringDamper")
	{
		CObjectConnectorSpringDamper* cObject = new CObjectConnectorSpringDamper();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorSpringDamper(); //new main object
		((MainObjectConnectorSpringDamper*)object)->SetCObjectConnectorSpringDamper(cObject);
		VisualizationObjectConnectorSpringDamper* vObject = new VisualizationObjectConnectorSpringDamper();				//visualization item
		((MainObjectConnectorSpringDamper*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ConnectorCartesianSpringDamper")
	{
		CObjectConnectorCartesianSpringDamper* cObject = new CObjectConnectorCartesianSpringDamper();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorCartesianSpringDamper(); //new main object
		((MainObjectConnectorCartesianSpringDamper*)object)->SetCObjectConnectorCartesianSpringDamper(cObject);
		VisualizationObjectConnectorCartesianSpringDamper* vObject = new VisualizationObjectConnectorCartesianSpringDamper();				//visualization item
		((MainObjectConnectorCartesianSpringDamper*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ConnectorRigidBodySpringDamper")
	{
		CObjectConnectorRigidBodySpringDamper* cObject = new CObjectConnectorRigidBodySpringDamper();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorRigidBodySpringDamper(); //new main object
		((MainObjectConnectorRigidBodySpringDamper*)object)->SetCObjectConnectorRigidBodySpringDamper(cObject);
		VisualizationObjectConnectorRigidBodySpringDamper* vObject = new VisualizationObjectConnectorRigidBodySpringDamper();				//visualization item
		((MainObjectConnectorRigidBodySpringDamper*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ConnectorCoordinateSpringDamper")
	{
		CObjectConnectorCoordinateSpringDamper* cObject = new CObjectConnectorCoordinateSpringDamper();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorCoordinateSpringDamper(); //new main object
		((MainObjectConnectorCoordinateSpringDamper*)object)->SetCObjectConnectorCoordinateSpringDamper(cObject);
		VisualizationObjectConnectorCoordinateSpringDamper* vObject = new VisualizationObjectConnectorCoordinateSpringDamper();				//visualization item
		((MainObjectConnectorCoordinateSpringDamper*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ConnectorDistance")
	{
		CObjectConnectorDistance* cObject = new CObjectConnectorDistance();							
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorDistance(); //new main object
		((MainObjectConnectorDistance*)object)->SetCObjectConnectorDistance(cObject);
		VisualizationObjectConnectorDistance* vObject = new VisualizationObjectConnectorDistance();				//visualization item
		((MainObjectConnectorDistance*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ConnectorCoordinate")
	{
		CObjectConnectorCoordinate* cObject = new CObjectConnectorCoordinate();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectConnectorCoordinate(); //new main object
		((MainObjectConnectorCoordinate*)object)->SetCObjectConnectorCoordinate(cObject);
		VisualizationObjectConnectorCoordinate* vObject = new VisualizationObjectConnectorCoordinate();				//visualization item
		((MainObjectConnectorCoordinate*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ContactCoordinate")
	{
		CObjectContactCoordinate* cObject = new CObjectContactCoordinate();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectContactCoordinate(); //new main object
		((MainObjectContactCoordinate*)object)->SetCObjectContactCoordinate(cObject);
		VisualizationObjectContactCoordinate* vObject = new VisualizationObjectContactCoordinate();				//visualization item
		((MainObjectContactCoordinate*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ContactCircleCable2D")
	{
		CObjectContactCircleCable2D* cObject = new CObjectContactCircleCable2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectContactCircleCable2D(); //new main object
		((MainObjectContactCircleCable2D*)object)->SetCObjectContactCircleCable2D(cObject);
		VisualizationObjectContactCircleCable2D* vObject = new VisualizationObjectContactCircleCable2D();				//visualization item
		((MainObjectContactCircleCable2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "ContactFrictionCircleCable2D")
	{
		CObjectContactFrictionCircleCable2D* cObject = new CObjectContactFrictionCircleCable2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectContactFrictionCircleCable2D(); //new main object
		((MainObjectContactFrictionCircleCable2D*)object)->SetCObjectContactFrictionCircleCable2D(cObject);
		VisualizationObjectContactFrictionCircleCable2D* vObject = new VisualizationObjectContactFrictionCircleCable2D();				//visualization item
		((MainObjectContactFrictionCircleCable2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "JointGeneric")
	{
		CObjectJointGeneric* cObject = new CObjectJointGeneric();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectJointGeneric(); //new main object
		((MainObjectJointGeneric*)object)->SetCObjectJointGeneric(cObject);
		VisualizationObjectJointGeneric* vObject = new VisualizationObjectJointGeneric();				//visualization item
		((MainObjectJointGeneric*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "JointRevolute2D")
	{
	CObjectJointRevolute2D* cObject = new CObjectJointRevolute2D();
	cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
	object = new MainObjectJointRevolute2D(); //new main object
	((MainObjectJointRevolute2D*)object)->SetCObjectJointRevolute2D(cObject);
	VisualizationObjectJointRevolute2D* vObject = new VisualizationObjectJointRevolute2D();				//visualization item
	((MainObjectJointRevolute2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "JointPrismatic2D")
	{
		CObjectJointPrismatic2D* cObject = new CObjectJointPrismatic2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectJointPrismatic2D(); //new main object
		((MainObjectJointPrismatic2D*)object)->SetCObjectJointPrismatic2D(cObject);
		VisualizationObjectJointPrismatic2D* vObject = new VisualizationObjectJointPrismatic2D();				//visualization item
		((MainObjectJointPrismatic2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "JointSliding2D")
	{
		CObjectJointSliding2D* cObject = new CObjectJointSliding2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectJointSliding2D(); //new main object
		((MainObjectJointSliding2D*)object)->SetCObjectJointSliding2D(cObject);
		VisualizationObjectJointSliding2D* vObject = new VisualizationObjectJointSliding2D();				//visualization item
		((MainObjectJointSliding2D*)object)->SetVisualizationObject(vObject);
	}
	else if (objectType == "JointALEMoving2D")
	{
		CObjectJointALEMoving2D* cObject = new CObjectJointALEMoving2D();
		cObject->SetCSystemData(&(mainSystem.GetCSystem()->GetSystemData()));		//add CSystemData* to objects (workaround, will be removed)
		object = new MainObjectJointALEMoving2D(); //new main object
		((MainObjectJointALEMoving2D*)object)->SetCObjectJointALEMoving2D(cObject);
		VisualizationObjectJointALEMoving2D* vObject = new VisualizationObjectJointALEMoving2D();				//visualization item
		((MainObjectJointALEMoving2D*)object)->SetVisualizationObject(vObject);
	}
	
	return object;
}

Index MainObjectFactory::AddMainObject(MainSystem& mainSystem, py::dict d)
{
	bool errorFound = false;

	//add default name, if it does not exist ...
	STDstring baseItem = "object";
	Index currentNumber = mainSystem.GetMainSystemData().GetMainObjects().NumberOfItems();

	if (!DictHasValidName(mainSystem, d, currentNumber, baseItem, errorFound))
	{
		if (!errorFound) { d["name"] = baseItem + EXUstd::Num2String(currentNumber); }
	}

	if (!errorFound)
	{
		if (d.contains("objectType"))
		{
			py::object nameItem = d["objectType"]; //this is necessary to make isinstance work
			if (py::isinstance<py::str>(nameItem))
			{
				STDstring nameString = py::cast<std::string>(nameItem); //!  read out dictionary and cast to C++ type

				MainObject* mainObject = CreateMainObject(mainSystem, nameString);

				if (mainObject != NULL)
				{
					mainObject->SetWithDictionary(d);
					mainSystem.GetCSystem()->GetSystemData().GetCObjects().Append(mainObject->GetCObject());
					mainSystem.GetVisualizationSystem().GetVisualizationSystemData().GetVisualizationObjects().Append(mainObject->GetVisualizationObject());
					return mainSystem.GetMainSystemData().GetMainObjects().Append(mainObject);
				}
				else
				{
					PyError(STDstring("AddObject, arg:dict['objectType'] contains invalid object type '") + nameString + "'");
				}

			}
			else
			{
				PyError("AddObject, arg:dict['objectType'] must be of string type");
			}
		}
		else
		{
			PyError("AddObject, arg:dict must contain item 'objectType'");
		}
	}
	return EXUstd::InvalidIndex;

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  MARKER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Create a specific marker with markerType; returns marker=Null if no success
MainMarker* MainObjectFactory::CreateMainMarker(MainSystem& mainSystem, STDstring markerType)
{
	MainMarker* marker = NULL;
	if (markerType == "BodyPosition")
	{
		CMarkerBodyPosition* cMarker = new CMarkerBodyPosition();							//new point marker
		marker = new MainMarkerBodyPosition(); //new main marker
		((MainMarkerBodyPosition*)marker)->SetCMarkerBodyPosition(cMarker);
		VisualizationMarkerBodyPosition* vMarker = new VisualizationMarkerBodyPosition();				//visualization item
		((MainMarkerBodyPosition*)marker)->SetVisualizationMarker(vMarker);
	}
	if (markerType == "BodyRigid")
	{
		CMarkerBodyRigid* cMarker = new CMarkerBodyRigid();							//new point marker
		marker = new MainMarkerBodyRigid(); //new main marker
		((MainMarkerBodyRigid*)marker)->SetCMarkerBodyRigid(cMarker);
		VisualizationMarkerBodyRigid* vMarker = new VisualizationMarkerBodyRigid();				//visualization item
		((MainMarkerBodyRigid*)marker)->SetVisualizationMarker(vMarker);
	}
	if (markerType == "BodyMass")
	{
		CMarkerBodyMass* cMarker = new CMarkerBodyMass();							//new point marker
		marker = new MainMarkerBodyMass(); //new main marker
		((MainMarkerBodyMass*)marker)->SetCMarkerBodyMass(cMarker);
		VisualizationMarkerBodyMass* vMarker = new VisualizationMarkerBodyMass();				//visualization item
		((MainMarkerBodyMass*)marker)->SetVisualizationMarker(vMarker);
	}
	if (markerType == "BodyCable2DShape")
	{
		CMarkerBodyCable2DShape* cMarker = new CMarkerBodyCable2DShape();							//new point marker
		marker = new MainMarkerBodyCable2DShape(); //new main marker
		((MainMarkerBodyCable2DShape*)marker)->SetCMarkerBodyCable2DShape(cMarker);
		VisualizationMarkerBodyCable2DShape* vMarker = new VisualizationMarkerBodyCable2DShape();				//visualization item
		((MainMarkerBodyCable2DShape*)marker)->SetVisualizationMarker(vMarker);
	}
	if (markerType == "BodyCable2DCoordinates")
	{
		CMarkerBodyCable2DCoordinates* cMarker = new CMarkerBodyCable2DCoordinates();							//new point marker
		marker = new MainMarkerBodyCable2DCoordinates(); //new main marker
		((MainMarkerBodyCable2DCoordinates*)marker)->SetCMarkerBodyCable2DCoordinates(cMarker);
		VisualizationMarkerBodyCable2DCoordinates* vMarker = new VisualizationMarkerBodyCable2DCoordinates();				//visualization item
		((MainMarkerBodyCable2DCoordinates*)marker)->SetVisualizationMarker(vMarker);
	}
	else if (markerType == "NodePosition")
	{
		CMarkerNodePosition* cMarker = new CMarkerNodePosition();							//new point marker
		marker = new MainMarkerNodePosition(); //new main marker
		((MainMarkerNodePosition*)marker)->SetCMarkerNodePosition(cMarker);
		VisualizationMarkerNodePosition* vMarker = new VisualizationMarkerNodePosition();				//visualization item
		((MainMarkerNodePosition*)marker)->SetVisualizationMarker(vMarker);
	}
	else if (markerType == "NodeRigid")
	{
		CMarkerNodeRigid* cMarker = new CMarkerNodeRigid();							//new point marker
		marker = new MainMarkerNodeRigid(); //new main marker
		((MainMarkerNodeRigid*)marker)->SetCMarkerNodeRigid(cMarker);
		VisualizationMarkerNodeRigid* vMarker = new VisualizationMarkerNodeRigid();				//visualization item
		((MainMarkerNodeRigid*)marker)->SetVisualizationMarker(vMarker);
	}
	else if (markerType == "NodeCoordinate")
	{
		CMarkerNodeCoordinate* cMarker = new CMarkerNodeCoordinate();							//new point marker
		marker = new MainMarkerNodeCoordinate(); //new main marker
		((MainMarkerNodeCoordinate*)marker)->SetCMarkerNodeCoordinate(cMarker);
		VisualizationMarkerNodeCoordinate* vMarker = new VisualizationMarkerNodeCoordinate();				//visualization item
		((MainMarkerNodeCoordinate*)marker)->SetVisualizationMarker(vMarker);
	}
	//else if (markerType == "MarkerRigid") { .. }
	return marker;
}

Index MainObjectFactory::AddMainMarker(MainSystem& mainSystem, py::dict d)
{
	bool errorFound = false;

	//add default name, if it does not exist ...
	STDstring baseItem = "marker";
	Index currentNumber = mainSystem.GetMainSystemData().GetMainMarkers().NumberOfItems();

	if (!DictHasValidName(mainSystem, d, currentNumber, baseItem, errorFound))
	{
		if (!errorFound) { d["name"] = baseItem + EXUstd::Num2String(currentNumber); }
	}

	if (!errorFound)
	{
		if (d.contains("markerType"))
		{
			py::object nameItem = d["markerType"]; //this is necessary to make isinstance work
			if (py::isinstance<py::str>(nameItem))
			{
				STDstring nameString = py::cast<std::string>(nameItem); //!  read out dictionary and cast to C++ type

				MainMarker* mainMarker = CreateMainMarker(mainSystem, nameString);

				if (mainMarker != NULL)
				{
					mainMarker->SetWithDictionary(d);
					mainSystem.GetCSystem()->GetSystemData().GetCMarkers().Append(mainMarker->GetCMarker());
					mainSystem.GetVisualizationSystem().GetVisualizationSystemData().GetVisualizationMarkers().Append(mainMarker->GetVisualizationMarker());
					return mainSystem.GetMainSystemData().GetMainMarkers().Append(mainMarker);
				}
				else
				{
					PyError(STDstring("AddMarker, arg:dict['markerType'] contains invalid marker type '") + nameString + "'");
				}

			}
			else
			{
				PyError("AddMarker, arg:dict['markerType'] must be of string type");
			}
		}
		else
		{
			PyError("AddMarker, arg:dict must contain item 'markerType'");
		}
	}
	return EXUstd::InvalidIndex;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  LOAD
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Create a specific load with loadType; returns load=Null if no success
MainLoad* MainObjectFactory::CreateMainLoad(MainSystem& mainSystem, STDstring loadType)
{
	MainLoad* load = NULL;
	if (loadType == "ForceVector")
	{
		CLoadForceVector* cLoad = new CLoadForceVector();							//new point load
		load = new MainLoadForceVector(); //new main load
		((MainLoadForceVector*)load)->SetCLoadForceVector(cLoad);
		VisualizationLoadForceVector* vLoad = new VisualizationLoadForceVector();				//visualization item
		((MainLoadForceVector*)load)->SetVisualizationLoadForceVector(vLoad);
	}
	else if (loadType == "MassProportional")
	{
		CLoadMassProportional* cLoad = new CLoadMassProportional();							//new point load
		load = new MainLoadMassProportional(); //new main load
		((MainLoadMassProportional*)load)->SetCLoadMassProportional(cLoad);
		VisualizationLoadMassProportional* vLoad = new VisualizationLoadMassProportional();				//visualization item
		((MainLoadMassProportional*)load)->SetVisualizationLoadMassProportional(vLoad);
	}
	else if (loadType == "TorqueVector")
	{
		CLoadTorqueVector* cLoad = new CLoadTorqueVector();							//new point load
		load = new MainLoadTorqueVector(); //new main load
		((MainLoadTorqueVector*)load)->SetCLoadTorqueVector(cLoad);
		VisualizationLoadTorqueVector* vLoad = new VisualizationLoadTorqueVector();				//visualization item
		((MainLoadTorqueVector*)load)->SetVisualizationLoadTorqueVector(vLoad);
	}
	else if (loadType == "Coordinate")
	{
		CLoadCoordinate* cLoad = new CLoadCoordinate();							//new point load
		load = new MainLoadCoordinate(); //new main load
		((MainLoadCoordinate*)load)->SetCLoadCoordinate(cLoad);
		VisualizationLoadCoordinate* vLoad = new VisualizationLoadCoordinate();				//visualization item
		((MainLoadCoordinate*)load)->SetVisualizationLoadCoordinate(vLoad);
	}
	//else if (loadType == "LoadRigid") { .. }
	return load;
}


Index MainObjectFactory::AddMainLoad(MainSystem& mainSystem, py::dict d)
{
	bool errorFound = false;

	//add default name, if it does not exist ...
	STDstring baseItem = "load";
	Index currentNumber = mainSystem.GetMainSystemData().GetMainLoads().NumberOfItems();

	if (!DictHasValidName(mainSystem, d, currentNumber, baseItem, errorFound))
	{
		if (!errorFound) { d["name"] = baseItem + EXUstd::Num2String(currentNumber); }
	}

	if (!errorFound)
	{
		if (d.contains("loadType"))
		{
			py::object nameItem = d["loadType"]; //this is necessary to make isinstance work
			if (py::isinstance<py::str>(nameItem))
			{
				STDstring nameString = py::cast<std::string>(nameItem); //!  read out dictionary and cast to C++ type

				MainLoad* mainLoad = CreateMainLoad(mainSystem, nameString);

				if (mainLoad != NULL)
				{
					mainLoad->SetWithDictionary(d);
					mainSystem.GetCSystem()->GetSystemData().GetCLoads().Append(mainLoad->GetCLoad());
					mainSystem.GetVisualizationSystem().GetVisualizationSystemData().GetVisualizationLoads().Append(mainLoad->GetVisualizationLoad());
					return mainSystem.GetMainSystemData().GetMainLoads().Append(mainLoad);
				}
				else
				{
					PyError(STDstring("AddLoad, arg:dict['loadType'] contains invalid load type '") + nameString + "'");
				}

			}
			else
			{
				PyError("AddLoad, arg:dict['loadType'] must be of string type");
			}
		}
		else
		{
			PyError("AddLoad, arg:dict must contain item 'loadType'");
		}
	}
	return EXUstd::InvalidIndex;

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  SENSOR
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Create a specific sensor with sensorType; returns sensor=Null if no success
MainSensor* MainObjectFactory::CreateMainSensor(MainSystem& mainSystem, STDstring sensorType)
{
	MainSensor* sensor = NULL;
	if (sensorType == "Node")
	{
		CSensorNode* cSensor = new CSensorNode();							//new point sensor
		sensor = new MainSensorNode(); //new main sensor
		((MainSensorNode*)sensor)->SetCSensorNode(cSensor);
		VisualizationSensorNode* vSensor = new VisualizationSensorNode();				//visualization item
		((MainSensorNode*)sensor)->SetVisualizationSensorNode(vSensor);
	}
	else if (sensorType == "Body")
	{
		CSensorBody* cSensor = new CSensorBody();							//new point sensor
		sensor = new MainSensorBody(); //new main sensor
		((MainSensorBody*)sensor)->SetCSensorBody(cSensor);
		VisualizationSensorBody* vSensor = new VisualizationSensorBody();				//visualization item
		((MainSensorBody*)sensor)->SetVisualizationSensorBody(vSensor);
	}
	return sensor;
}


Index MainObjectFactory::AddMainSensor(MainSystem& mainSystem, py::dict d)
{
	bool errorFound = false;

	//add default name, if it does not exist ...
	STDstring baseItem = "sensor";
	Index currentNumber = mainSystem.GetMainSystemData().GetMainSensors().NumberOfItems();

	if (!DictHasValidName(mainSystem, d, currentNumber, baseItem, errorFound))
	{
		if (!errorFound) { d["name"] = baseItem + EXUstd::Num2String(currentNumber); }
	}

	if (!errorFound)
	{
		if (d.contains("sensorType"))
		{
			py::object nameItem = d["sensorType"]; //this is necessary to make isinstance work
			if (py::isinstance<py::str>(nameItem))
			{
				STDstring nameString = py::cast<std::string>(nameItem); //!  read out dictionary and cast to C++ type

				MainSensor* mainSensor = CreateMainSensor(mainSystem, nameString);

				if (mainSensor != NULL)
				{
					mainSensor->SetWithDictionary(d);
					mainSystem.GetCSystem()->GetSystemData().GetCSensors().Append(mainSensor->GetCSensor());
					mainSystem.GetVisualizationSystem().GetVisualizationSystemData().GetVisualizationSensors().Append(mainSensor->GetVisualizationSensor());
					return mainSystem.GetMainSystemData().GetMainSensors().Append(mainSensor);
				}
				else
				{
					PyError(STDstring("AddSensor, arg:dict['sensorType'] contains invalid sensor type '") + nameString + "'");
				}

			}
			else
			{
				PyError("AddSensor, arg:dict['sensorType'] must be of string type");
			}
		}
		else
		{
			PyError("AddSensor, arg:dict must contain item 'sensorType'");
		}
	}
	return EXUstd::InvalidIndex;

}

