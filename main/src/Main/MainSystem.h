/** ***********************************************************************************************
* @class        MainSystem
* @brief		MainSystem and ObjectFactory
* @details		Details:
				- handling of CSystem
				- initialization
				- pybind11 interface
				- object factory
*
* @author		Gerstmayr Johannes
* @date			2018-05-17 (generated)
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
#ifndef MAINSYSTEM__H
#define MAINSYSTEM__H

#include "Main/MainSystemData.h"
#include "System/ItemIndices.h"
#include "Main/MainObjectFactory.h"
//#include "Graphics/VisualizationSystemData.h"
#include "Graphics/VisualizationSystemContainer.h"

//#include "Pymodules/PyGeneralContact.h"

#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)

//#include "Utilities/BasicFunctions.h"

class MainSystemContainer;
class PyGeneralContact;

//!Interface to a CSystem, used in Python
// This class mirrors all functionality accessible in Python
// Additionally, the MainSystem includes the object factory
class MainSystem: public MainSystemBase
{
public: //! objects made public, because Python functions are anyway mostly Autogenerated ==> keep interface simple
	CSystem* cSystem;			//!< link to CSystem object; cSystem must always be initialized correctly!
	VisualizationSystem visualizationSystem; //!< link to graphics representation of system

	//add specific Python lists here:
	MainSystemData mainSystemData;
	MainObjectFactory mainObjectFactory;
	py::dict variables;			//!< dictionary which is used to store local variables for models
	py::dict systemVariables;	//!< dictionary which is used to store system variables (e.g. for solvers, etc.)

private:
	bool interactiveMode;		//!< if this is true, every AddItem(...), ModifyItem(...), etc. causes Assemble() to be called; this guarantees that the system is always consistent to be drawn
	MainSystemContainer* mainSystemContainerBacklink; //!< backlink used to avoid needing SC and mbs in functions -> mbs is sufficient for all purposes!
	Index mainSystemIndex;		//!< index of mainSystem (mbs) in SystemContainer; used e.g. in graphics functions, but may also accessed via Python; -1 indicates that it is not initialized

public:
	//MainSystem() {};
	//! destructor for correct deletion of derived classes
	virtual ~MainSystem() 
	{
		delete cSystem;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   MEMBER VARIABLE ACCESS
	CSystem* GetCSystem() { return cSystem; }
	const CSystem* GetCSystem() const { return cSystem; }

	MainSystemData& GetMainSystemData() { return mainSystemData; }
	const MainSystemData& GetMainSystemData() const { return mainSystemData; }

	void SetMainSystemContainer(MainSystemContainer* mainSystemContainerBacklinkInit) { mainSystemContainerBacklink = mainSystemContainerBacklinkInit; }

	//! backlink to MainSystemContainer
	MainSystemContainer& GetMainSystemContainer();
	//! backlink to MainSystemContainer; add const as it cannot be distinguished by pybind11
	const MainSystemContainer& GetMainSystemContainerConst() const;

	//! signal CSystem and VisualizationSystem that there are new items; system becomes invalid; 
	//! visualization system needs to update maxSceneSize and center
	void SystemHasChanged();

	//DELETE: void Test() const { int i = 1; };

	VisualizationSystem& GetVisualizationSystem() { return visualizationSystem; }
	const VisualizationSystem& GetVisualizationSystem() const { return visualizationSystem; }

	MainObjectFactory& GetMainObjectFactory() { return mainObjectFactory; }

	//create a new general contact and add to system
	PyGeneralContact& AddGeneralContact();
	//obtain read/write access to general contact
	PyGeneralContact& GetGeneralContact(Index generalContactNumber);
	//delete general contact, resort indices
	void DeleteGeneralContact(Index generalContactNumber);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   SYSTEM FUNCTIONS
	void Reset();
	bool GetFlagSystemIsConsistent() const { return GetCSystem()->IsSystemConsistent(); }
	void SetFlagSystemIsConsistent(bool flag) { GetCSystem()->SetSystemIsConsistent(flag); }

	Index GetMainSystemIndex() const { return mainSystemIndex; }
	void SetMainSystemIndex(Index mainSystemIndexInit) { mainSystemIndex = mainSystemIndexInit; }

	bool GetFlagSolverSignalJacobianUpdate() const { return GetCSystem()->GetSolverData().signalJacobianUpdate; }
	void SetFlagSolverSignalJacobianUpdate(bool flag) { GetCSystem()->GetSolverData().signalJacobianUpdate = flag; }

	bool GetInteractiveMode() const { return interactiveMode; }
	void SetInteractiveMode(bool flag) { interactiveMode = flag; }
	void InteractiveModeActions(); //!<if interAciveMode == true: causes Assemble() to be called; this guarantees that the system is always consistent to be drawn
	//! raise exception if configuration is not reference configuration; used for several MainSystem functions called from Python
	void RaiseIfConfigurationNotReference(const char* functionName, ConfigurationType configuration) const;
	void RaiseIfNotConsistent(const char* functionName) const;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   VISUALIZATION FUNCTIONS

	//! set rendering true/false
	void ActivateRendering(bool flag);

	//! this function links to the VisualizationSystem (which has link to a render engine), such that the changes in the graphics structure drawn upon updates, etc.
	//!  This function is called on creation of a main system and automatically links to VisualizationSystem, not directly to Renderer
	bool LinkToVisualizationSystem();

	//! for future, unregister mbs from renderer; but currently not possible
	bool UnlinkVisualizationSystem();

	//! this function is used to only send a signal that the scene shall be redrawn because the visualization state has been updated
	void SendRedrawSignal() { GetCSystem()->GetPostProcessData()->SendRedrawSignal(); };

	//! interrupt further computation until user input --> 'pause' function
	void WaitForUserToContinue() { GetCSystem()->GetPostProcessData()->WaitForUserToContinue(); }

	//! return the render engine stop flag (e.g. in order to interrupt animation or postprocessing)
	bool GetRenderEngineStopFlag() const { return GetCSystem()->GetPostProcessData()->stopSimulation; }

	//! return the render engine stop flag (e.g. in order to interrupt animation or postprocessing)
	void SetRenderEngineStopFlag(bool stopFlag) 
	{ 
		GetCSystem()->GetPostProcessData()->stopSimulation = stopFlag; 
		if (!stopFlag)
		{
			GetCSystem()->GetPostProcessData()->forceQuitSimulation = stopFlag; //this allows to proceed after a render engine has been stopped
		}
	}

	////! this function waits for the stop flag in the render engine;
	//bool WaitForRenderEngineStopFlag();

	////! access to settings via pybind, not knowing visualization system:
	//const VisualizationSettings& PyGetVisualizationSettings() const { return visualizationSystem.settings; }
	//void PySetVisualizationSettings(const VisualizationSettings& visualizationSettings) { visualizationSystem.settings = visualizationSettings; }


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   COMPUTATIONAL FUNCTIONS
	void Assemble() 
	{ 
		cSystem->Assemble(*this); 
		//visualizationSystem.InitializeView(); //uses reference coordinates to obtain the scene center and its size
	}
	void AssembleCoordinates() { cSystem->AssembleCoordinates(*this); }
	void AssembleLTGLists() { cSystem->AssembleLTGLists(*this); }
	void AssembleInitializeSystemCoordinates() { cSystem->AssembleInitializeSystemCoordinates(*this); }

	//! initialize some system data, e.g., generalContact objects (searchTree, etc.)
	void AssembleSystemInitialize() { cSystem->AssembleSystemInitialize(*this); }

	//! set user function to be called by solvers at beginning of step (static or dynamic step)
	void PySetPreStepUserFunction(const py::object& value);
	//! set user function to be called immediately after Newton (after an update of the solution has been computed, but before discontinuous iteration)
	void PySetPostNewtonUserFunction(const py::object& value);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   NODES
	//! this is the hook to the object factory, handling all kinds of objects, nodes, ...
	Index AddMainNode(py::dict d);
	//! Add a MainNode with a python class
	NodeIndex AddMainNodePyClass(py::object pyObject);
	//! get node's dictionary by name; does not throw a error message
	NodeIndex PyGetNodeNumber(STDstring name);
	//! hook to read node's dictionary
	py::dict PyGetNode(const py::object& itemIndex);
	////! get node's dictionary by name
	//py::dict PyGetNodeByName(STDstring name);
	//! modify node's dictionary
	void PyModifyNode(const py::object& itemIndex, py::dict nodeDict);
	//! get node's with type 'name' default values, which helps for manual writing of python input
	py::dict PyGetNodeDefaults(STDstring typeName);

	//! GetOutputVariable with type and return value; copies values==>slow!; can be scalar or vector-valued! maps to CNode GetOutputVariable(...)
	virtual py::object PyGetNodeOutputVariable(const py::object& itemIndex, OutputVariableType variableType, ConfigurationType configuration = ConfigurationType::Current) const;

	//! get index in global ODE2 coordinate vector for first node coordinate
	virtual Index PyGetNodeODE2Index(const py::object& itemIndex) const;

	//! get index in global ODE1 coordinate vector for first node coordinate
	virtual Index PyGetNodeODE1Index(const py::object& itemIndex) const;

	//! get index in global AE coordinate vector for first node coordinate
	virtual Index PyGetNodeAEIndex(const py::object& itemIndex) const;

	////! call pybind node function, possibly with arguments
	//py::object PyCallNodeFunction(Index nodeNumber, STDstring functionName, py::dict args);

	//! Get (read) parameter 'parameterName' of 'objectNumber' via pybind / pyhton interface instead of obtaining the whole dictionary with GetDictionary
	virtual py::object PyGetNodeParameter(const py::object& itemIndex, const STDstring& parameterName) const;
	//! Set (write) parameter 'parameterName' of 'itemNumber' to 'value' via pybind / pyhton interface instead of writing the whole dictionary with SetWithDictionary(...)
	virtual void PySetNodeParameter(const py::object& itemIndex, const STDstring& parameterName, const py::object& value);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   OBJECTS
	//! this is the hook to the object factory, handling all kinds of objects, nodes, ...
	Index AddMainObject(py::dict d);
	//! Add a MainObject with a python class
	ObjectIndex AddMainObjectPyClass(py::object pyObject);
	//! get object's dictionary by name; does not throw a error message
	ObjectIndex PyGetObjectNumber(STDstring itemName);
	//! hook to read object's dictionary
	py::dict PyGetObject(const py::object& itemIndex, bool addGraphicsData = false);
	////! get object's dictionary by name
	//py::dict PyGetObjectByName(STDstring itemName);
	//! modify object's dictionary
	void PyModifyObject(const py::object& itemIndex, py::dict d);
	//! get object's with type 'typeName' default values, which helps for manual writing of python input
	py::dict PyGetObjectDefaults(STDstring typeName);

	////! call pybind object function, possibly with arguments
	//py::object PyCallObjectFunction(Index itemNumber, STDstring functionName, py::dict args);
	//! Get specific output variable with variable type; as this will involve MarkerDataStructure for constraints, this call may be slower than other calls
	py::object PyGetObjectOutputVariable(const py::object& itemIndex, OutputVariableType variableType, ConfigurationType configuration=ConfigurationType::Current) const;
	//! Get specific output variable with variable type; ONLY for bodies;
	py::object PyGetObjectOutputVariableBody(const py::object& itemIndex, OutputVariableType variableType, 
		const std::vector<Real>& localPosition, ConfigurationType configuration = ConfigurationType::Current) const;
	//! get output variable from mesh node number of object with type SuperElement (GenericODE2, FFRF, FFRFreduced - CMS) with specific OutputVariableType
	py::object PyGetObjectOutputVariableSuperElement(const py::object& itemIndex, OutputVariableType variableType, Index meshNodeNumber, ConfigurationType configuration) const;

	//! Get (read) parameter 'parameterName' of 'objectNumber' via pybind / pyhton interface instead of obtaining the whole dictionary with GetDictionary
	virtual py::object PyGetObjectParameter(const py::object& itemIndex, const STDstring& parameterName) const;
	//! Set (write) parameter 'parameterName' of 'objectNumber' to 'value' via pybind / pyhton interface instead of writing the whole dictionary with SetWithDictionary(...)
	virtual void PySetObjectParameter(const py::object& itemIndex, const STDstring& parameterName, const py::object& value);



	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   MARKER
	//! this is the hook to the object factory, handling all kinds of objects, nodes, ...
	Index AddMainMarker(py::dict d);
	//! Add a MainMarker with a python class
	MarkerIndex AddMainMarkerPyClass(py::object pyObject);
	//! get marker's dictionary by name; does not throw a error message
	MarkerIndex PyGetMarkerNumber(STDstring itemName);
	//! hook to read marker's dictionary
	py::dict PyGetMarker(const py::object& itemIndex);
	////! get marker's dictionary by name
	//py::dict PyGetMarkerByName(STDstring itemName);
	//! modify marker's dictionary
	void PyModifyMarker(const py::object& itemIndex, py::dict d);
	//! get marker's default values, which helps for manual writing of python input
	py::dict PyGetMarkerDefaults(STDstring typeName);

	//! Get (read) parameter 'parameterName' of 'itemNumber' via pybind / pyhton interface instead of obtaining the whole dictionary with GetDictionary
	virtual py::object PyGetMarkerParameter(const py::object& itemIndex, const STDstring& parameterName) const;
	//! Set (write) parameter 'parameterName' of 'itemNumber' to 'value' via pybind / pyhton interface instead of writing the whole dictionary with SetWithDictionary(...)
	virtual void PySetMarkerParameter(const py::object& itemIndex, const STDstring& parameterName, const py::object& value);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   LOAD
	//! this is the hook to the object factory, handling all kinds of objects, nodes, ...
	Index AddMainLoad(py::dict d);
	//! Add a MainLoad with a python class
	LoadIndex AddMainLoadPyClass(py::object pyObject);
	//! get load's dictionary by name; does not throw a error message
	LoadIndex PyGetLoadNumber(STDstring itemName);
	//! hook to read load's dictionary
	py::dict PyGetLoad(const py::object& itemIndex);
	////! get load's dictionary by name
	//py::dict PyGetLoadByName(STDstring itemName);
	//! modify load's dictionary
	void PyModifyLoad(const py::object& itemIndex, py::dict d);
	//! get load's default values, which helps for manual writing of python input
	py::dict PyGetLoadDefaults(STDstring typeName);

	//! Get current load values, specifically if user-defined loads are used
	virtual py::object PyGetLoadValues(const py::object& itemIndex) const;

	//! Get (read) parameter 'parameterName' of 'itemNumber' via pybind / pyhton interface instead of obtaining the whole dictionary with GetDictionary
	virtual py::object PyGetLoadParameter(const py::object& itemIndex, const STDstring& parameterName) const;
	//! Set (write) parameter 'parameterName' of 'itemNumber' to 'value' via pybind / pyhton interface instead of writing the whole dictionary with SetWithDictionary(...)
	virtual void PySetLoadParameter(const py::object& itemIndex, const STDstring& parameterName, const py::object& value);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//   SENSOR
	//! this is the hook to the object factory, handling all kinds of objects, nodes, ...
	Index AddMainSensor(py::dict d);
	//! Add a MainSensor with a python class
	SensorIndex AddMainSensorPyClass(py::object pyObject);
	//! get sensor's dictionary by name; does not throw a error message
	SensorIndex PyGetSensorNumber(STDstring itemName);
	//! hook to read sensor's dictionary
	py::dict PyGetSensor(const py::object& itemIndex);
	////! get sensor's dictionary by name
	//py::dict PyGetSensorByName(STDstring itemName);
	//! modify sensor's dictionary
	void PyModifySensor(const py::object& itemIndex, py::dict d);
	//! get sensor's default values, which helps for manual writing of python input
	py::dict PyGetSensorDefaults(STDstring typeName);
	//! get sensor's values
	py::object PyGetSensorValues(const py::object& itemIndex, ConfigurationType configuration = ConfigurationType::Current);
	//! get sensor's values
	py::array_t<Real> PyGetSensorStoredData(const py::object& itemIndex);

	//! Get (read) parameter 'parameterName' of 'itemNumber' via pybind / pyhton interface instead of obtaining the whole dictionary with GetDictionary
	virtual py::object PyGetSensorParameter(const py::object& itemIndex, const STDstring& parameterName) const;
	//! Set (write) parameter 'parameterName' of 'itemNumber' to 'value' via pybind / pyhton interface instead of writing the whole dictionary with SetWithDictionary(...)
	virtual void PySetSensorParameter(const py::object& itemIndex, const STDstring& parameterName, const py::object& value);


};

#endif
