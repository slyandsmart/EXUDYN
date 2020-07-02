// AUTO:  ++++++++++++++++++++++
// AUTO:  pybind11 manual module includes; generated by Johannes Gerstmayr
// AUTO:  last modified = 2020-06-24
// AUTO:  ++++++++++++++++++++++

//        pybinding to enum classes:
  py::enum_<OutputVariableType>(m, "OutputVariableType")
		.value("_None", OutputVariableType::_None)    //no value; used, e.g., to select no output variable in contour plot
		.value("Distance", OutputVariableType::Distance)    //e.g., measure distance in spring damper connector
		.value("Position", OutputVariableType::Position)    //measure 3D position, e.g., of node or body
		.value("Displacement", OutputVariableType::Displacement)    //measure displacement; usually difference between current position and reference position
		.value("Velocity", OutputVariableType::Velocity)    //measure (translational) velocity of node or object
		.value("Acceleration", OutputVariableType::Acceleration)    //measure (translational) acceleration of node or object
		.value("RotationMatrix", OutputVariableType::RotationMatrix)    //measure rotation matrix of rigid body node or object
		.value("AngularVelocity", OutputVariableType::AngularVelocity)    //measure angular velocity of node or object
		.value("AngularVelocityLocal", OutputVariableType::AngularVelocityLocal)    //measure local (body-fixed) angular velocity of node or object
		.value("AngularAcceleration", OutputVariableType::AngularAcceleration)    //measure angular acceleration of node or object
		.value("Rotation", OutputVariableType::Rotation)    //measure, e.g., scalar rotation of 2D body, Euler angles of a 3D object or rotation within a joint
		.value("Coordinates", OutputVariableType::Coordinates)    //measure the coordinates of a node or object; coordinates usually just contain displacements, but not the position values
		.value("Coordinates_t", OutputVariableType::Coordinates_t)    //measure the time derivative of coordinates (= velocity coordinates) of a node or object
		.value("SlidingCoordinate", OutputVariableType::SlidingCoordinate)    //measure sliding coordinate in sliding joint
		.value("Director1", OutputVariableType::Director1)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 1 or x-direction
		.value("Director2", OutputVariableType::Director2)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 2 or y-direction
		.value("Director3", OutputVariableType::Director3)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 3 or z-direction
		.value("Force", OutputVariableType::Force)    //measure force, e.g., in joint or beam (resultant force)
		.value("Torque", OutputVariableType::Torque)    //measure torque, e.g., in joint or beam (resultant couple/moment)
		.value("Strain", OutputVariableType::Strain)    //measure strain, e.g., axial strain in beam
		.value("Stress", OutputVariableType::Stress)    //measure stress, e.g., axial stress in beam
		.value("Curvature", OutputVariableType::Curvature)    //measure curvature; may be scalar or vectorial: twist and curvature
		.value("DisplacementLocal", OutputVariableType::DisplacementLocal)    //measure local displacement, e.g. in local joint coordinates
		.value("VelocityLocal", OutputVariableType::VelocityLocal)    //measure local (translational) velocity, , e.g. in local joint coordinates
		.value("ForceLocal", OutputVariableType::ForceLocal)    //measure local force, e.g., in joint or beam (resultant force)
		.value("TorqueLocal", OutputVariableType::TorqueLocal)    //measure local torque, e.g., in joint or beam (resultant couple/moment)
		.value("ConstraintEquation", OutputVariableType::ConstraintEquation)    //evaluates constraint equation (=current deviation or drift of constraint equation)
		.value("EndOfEnumList", OutputVariableType::EndOfEnumList)    //this marks the end of the list, usually not important to the user
		.export_values();

  py::enum_<ConfigurationType>(m, "ConfigurationType")
		.value("_None", ConfigurationType::_None)    //no configuration; usually not valid, but may be used, e.g., if no configurationType is required
		.value("Initial", ConfigurationType::Initial)    //initial configuration prior to static or dynamic solver; is computed during mbs.Assemble() or AssembleInitializeSystemCoordinates()
		.value("Current", ConfigurationType::Current)    //current configuration during and at the end of the computation of a step (static or dynamic)
		.value("Reference", ConfigurationType::Reference)    //configuration used to define deformable bodies (reference configuration for finite elements) or joints (configuration for which some joints are defined)
		.value("StartOfStep", ConfigurationType::StartOfStep)    //during computation, this refers to the solution at the start of the step = end of last step, to which the solver falls back if convergence fails
		.value("Visualization", ConfigurationType::Visualization)    //this is a state completely de-coupled from computation, used for visualization
		.value("EndOfEnumList", ConfigurationType::EndOfEnumList)    //this marks the end of the list, usually not important to the user
		.export_values();

  py::enum_<LinearSolverType>(m, "LinearSolverType")
		.value("_None", LinearSolverType::_None)    //no value; used, e.g., if no solver is selected
		.value("EXUdense", LinearSolverType::EXUdense)    //use dense matrices and according solvers for densly populated matrices (usually the CPU time grows cubically with the number of unknowns)
		.value("EigenSparse", LinearSolverType::EigenSparse)    //use sparse matrices and according solvers; additional overhead for very small systems; specifically, memory allocation is performed during a factorization process
		.export_values();


        m.def("Go", &PythonGo, "Creates a SystemContainer SC and a main system mbs");
        m.def("InfoStat", &PythonInfoStat, "Print some global (debug) information: linear algebra, memory allocation, threads, computational efficiency, etc.");
        m.def("StartRenderer", &PyStartOpenGLRenderer, "Start OpenGL rendering engine (in separate thread); use verbose=True to output information during OpenGL window creation", py::arg("verbose") = false);
        m.def("StopRenderer", &PyStopOpenGLRenderer, "Stop OpenGL rendering engine");
        m.def("SetOutputPrecision", &PySetOutputPrecision, "Set the precision (integer) for floating point numbers written to console (reset when simulation is started!)", py::arg("numberOfDigits"));
        m.def("SetLinalgOutputFormatPython", &PySetLinalgOutputFormatPython, "true: use python format for output of vectors and matrices; false: use matlab format", py::arg("flagPythonFormat"));
        m.def("InvalidIndex", &GetInvalidIndex, "This function provides the invalid index, which depends on the kind of 32-bit, 64-bit signed or unsigned integer; e.g. node index or item index in list");
        m.def("SetWriteToConsole", &PySetWriteToConsole, "set flag to write (true) or not write to console; default = true", py::arg("flag"));
        m.def("SetWriteToFile", &PySetWriteToFile, "set flag to write (true) or not write to console; default value of flagWriteToFile = false; flagAppend appends output to file, if set true; in order to finalize the file, write exu.SetWriteToFile('', False) to close the output file", py::arg("filename"), py::arg("flagWriteToFile") = true, py::arg("flagAppend") = false);
        m.def("SetPrintDelayMilliSeconds", &PySetPrintDelayMilliSeconds, "add some delay (in milliSeconds) to printing to console, in order to let Spyder process the output; default = 0", py::arg("delayMilliSeconds"));
        m.def("Print", &PyPrint, "this allows printing via exudyn with similar syntax as in python print(args) except for keyword arguments: print('test=',42); allows to redirect all output to file given by SetWriteToFile(...); does not output in case that SetWriteToConsole is set to false");
        m.attr("variables") = exudynVariables;
        m.attr("sys") = exudynSystemVariables;

    py::class_<MainSystem>(m, "MainSystem")
        .def(py::init<>())
        .def("Assemble", &MainSystem::Assemble, "assemble items (nodes, bodies, markers, loads, ...); Calls CheckSystemIntegrity(...), AssembleCoordinates(), AssembleLTGLists(), and AssembleInitializeSystemCoordinates()")
        .def("AssembleCoordinates", &MainSystem::AssembleCoordinates, "assemble coordinates: assign computational coordinates to nodes and constraints (algebraic variables)")
        .def("AssembleLTGLists", &MainSystem::AssembleLTGLists, "build local-to-global (ltg) coordinate lists for objects (used to build global ODE2RHS, MassMatrix, etc. vectors and matrices)")
        .def("AssembleInitializeSystemCoordinates", &MainSystem::AssembleInitializeSystemCoordinates, "initialize all system-wide coordinates based on initial values given in nodes")
        .def("Reset", &MainSystem::Reset, "reset all lists of items (nodes, bodies, markers, loads, ...) and temporary vectors; deallocate memory")
        .def("WaitForUserToContinue", &MainSystem::WaitForUserToContinue, "interrupt further computation until user input --> 'pause' function")
        .def("SendRedrawSignal", &MainSystem::SendRedrawSignal, "this function is used to send a signal to the renderer that the scene shall be redrawn because the visualization state has been updated")
        .def("GetRenderEngineStopFlag", &MainSystem::GetRenderEngineStopFlag, "get the current stop simulation flag; true=user wants to stop simulation")
        .def("SetRenderEngineStopFlag", &MainSystem::SetRenderEngineStopFlag, "set the current stop simulation flag; set to false, in order to continue a previously user-interrupted simulation")
        .def("SetPreStepUserFunction", &MainSystem::PySetPreStepUserFunction, "Sets a user function PreStepUserFunction(mbs, t) executed at beginning of every computation step; in normal case return True; return False to stop simulation after current step")
        .def("__repr__", [](const MainSystem &ms) {
            return "<systemData: \n" + ms.GetMainSystemData().PyInfoSummary() + "\nmainSystem:\n  variables = " + EXUstd::ToString(ms.variables) + "\n  sys = " + EXUstd::ToString(ms.systemVariables) + "\n>\n"; }, "return the representation of the system, which can be, e.g., printed")
        .def_property("systemIsConsistent", &MainSystem::GetFlagSystemIsConsistent, &MainSystem::SetFlagSystemIsConsistent)
        .def_property("interactiveMode", &MainSystem::GetInteractiveMode, &MainSystem::SetInteractiveMode)
        .def_readwrite("variables", &MainSystem::variables, py::return_value_policy::reference)
        .def_readwrite("sys", &MainSystem::systemVariables, py::return_value_policy::reference)
        .def_property("solverSignalJacobianUpdate", &MainSystem::GetFlagSolverSignalJacobianUpdate, &MainSystem::SetFlagSolverSignalJacobianUpdate)
        .def_readwrite("systemData", &MainSystem::mainSystemData, py::return_value_policy::reference)

//        NODES:
        .def("AddNode", &MainSystem::AddMainNodePyClass, "add a node with nodeDefinition from Python node class; returns (global) node number of newly added node", py::arg("pyObject"))
        .def("GetNodeNumber", &MainSystem::PyGetNodeNumber, "get node's number by name (string)", py::arg("nodeName"))
        .def("GetNode", &MainSystem::PyGetNode, "get node's dictionary by index", py::arg("nodeNumber"))
        .def("ModifyNode", &MainSystem::PyModifyNode, "modify node's dictionary by index", py::arg("nodeNumber"), py::arg("nodeDict"))
        .def("GetNodeDefaults", &MainSystem::PyGetNodeDefaults, "get node's default values for a certain nodeType as (dictionary)", py::arg("typeName"))
        .def("GetNodeOutput", &MainSystem::PyGetNodeOutputVariable, "get the ouput of the node specified with the OutputVariableType; default configuration = 'current'; output may be scalar or array (e.g. displacement vector)", py::arg("nodeNumber"), py::arg("variableType"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetNodeODE2Index", &MainSystem::PyGetNodeODE2Index, "get index in the global ODE2 coordinate vector for the first node coordinate of the specified node", py::arg("nodeNumber"))
        .def("GetNodeParameter", &MainSystem::PyGetNodeParameter, "get nodes's parameter from nodeNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("nodeNumber"), py::arg("parameterName"))
        .def("SetNodeParameter", &MainSystem::PySetNodeParameter, "set parameter 'parameterName' of node with nodeNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("nodeNumber"), py::arg("parameterName"), py::arg("value"))

//        OBJECTS:
        .def("AddObject", &MainSystem::AddMainObjectPyClass, "add a object with objectDefinition from Python object class; returns (global) object number of newly added object", py::arg("pyObject"))
        .def("GetObjectNumber", &MainSystem::PyGetObjectNumber, "get object's number by name (string)", py::arg("objectName"))
        .def("GetObject", &MainSystem::PyGetObject, "get object's dictionary by index", py::arg("objectNumber"))
        .def("ModifyObject", &MainSystem::PyModifyObject, "modify object's dictionary by index", py::arg("objectNumber"), py::arg("objectDict"))
        .def("GetObjectDefaults", &MainSystem::PyGetObjectDefaults, "get object's default values for a certain objectType as (dictionary)", py::arg("typeName"))
        .def("GetObjectOutput", &MainSystem::PyGetObjectOutputVariable, "get object's current output variable from objectNumber and OutputVariableType; can only be computed for exu.ConfigurationType.Current configuration!", py::arg("objectNumber"), py::arg("variableType"))
        .def("GetObjectOutputBody", &MainSystem::PyGetObjectOutputVariableBody, "get body's output variable from objectNumber and OutputVariableType", py::arg("objectNumber"), py::arg("variableType"), py::arg("localPosition"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetObjectOutputSuperElement", &MainSystem::PyGetObjectOutputVariableSuperElement, "get output variable from mesh node number of object with type SuperElement (GenericODE2, FFRF, FFRFreduced - CMS) with specific OutputVariableType; the meshNodeNumber is the object's local node number, not the global node number!", py::arg("objectNumber"), py::arg("variableType"), py::arg("meshNodeNumber"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetObjectParameter", &MainSystem::PyGetObjectParameter, "get objects's parameter from objectNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("objectNumber"), py::arg("parameterName"))
        .def("SetObjectParameter", &MainSystem::PySetObjectParameter, "set parameter 'parameterName' of object with objectNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("objectNumber"), py::arg("parameterName"), py::arg("value"))

//        MARKER:
        .def("AddMarker", &MainSystem::AddMainMarkerPyClass, "add a marker with markerDefinition from Python marker class; returns (global) marker number of newly added marker", py::arg("pyObject"))
        .def("GetMarkerNumber", &MainSystem::PyGetMarkerNumber, "get marker's number by name (string)", py::arg("markerName"))
        .def("GetMarker", &MainSystem::PyGetMarker, "get marker's dictionary by index", py::arg("markerNumber"))
        .def("ModifyMarker", &MainSystem::PyModifyMarker, "modify marker's dictionary by index", py::arg("markerNumber"), py::arg("markerDict"))
        .def("GetMarkerDefaults", &MainSystem::PyGetMarkerDefaults, "get marker's default values for a certain markerType as (dictionary)", py::arg("typeName"))
        .def("GetMarkerParameter", &MainSystem::PyGetMarkerParameter, "get markers's parameter from markerNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("markerNumber"), py::arg("parameterName"))
        .def("SetMarkerParameter", &MainSystem::PySetMarkerParameter, "set parameter 'parameterName' of marker with markerNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("markerNumber"), py::arg("parameterName"), py::arg("value"))

//        LOADS:
        .def("AddLoad", &MainSystem::AddMainLoadPyClass, "add a load with loadDefinition from Python load class; returns (global) load number of newly added load", py::arg("pyObject"))
        .def("GetLoadNumber", &MainSystem::PyGetLoadNumber, "get load's number by name (string)", py::arg("loadName"))
        .def("GetLoad", &MainSystem::PyGetLoad, "get load's dictionary by index", py::arg("loadNumber"))
        .def("ModifyLoad", &MainSystem::PyModifyLoad, "modify load's dictionary by index", py::arg("loadNumber"), py::arg("loadDict"))
        .def("GetLoadDefaults", &MainSystem::PyGetLoadDefaults, "get load's default values for a certain loadType as (dictionary)", py::arg("typeName"))
        .def("GetLoadValues", &MainSystem::PyGetLoadValues, "Get current load values, specifically if user-defined loads are used; can be scalar or vector-valued return value", py::arg("loadNumber"))
        .def("GetLoadParameter", &MainSystem::PyGetLoadParameter, "get loads's parameter from loadNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("loadNumber"), py::arg("parameterName"))
        .def("SetLoadParameter", &MainSystem::PySetLoadParameter, "set parameter 'parameterName' of load with loadNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("loadNumber"), py::arg("parameterName"), py::arg("value"))

//        SENSORS:
        .def("AddSensor", &MainSystem::AddMainSensorPyClass, "add a sensor with sensor definition from Python sensor class; returns (global) sensor number of newly added sensor", py::arg("pyObject"))
        .def("GetSensorNumber", &MainSystem::PyGetSensorNumber, "get sensor's number by name (string)", py::arg("sensorName"))
        .def("GetSensor", &MainSystem::PyGetSensor, "get sensor's dictionary by index", py::arg("sensorNumber"))
        .def("ModifySensor", &MainSystem::PyModifySensor, "modify sensor's dictionary by index", py::arg("sensorNumber"), py::arg("sensorDict"))
        .def("GetSensorDefaults", &MainSystem::PyGetSensorDefaults, "get sensor's default values for a certain sensorType as (dictionary)", py::arg("typeName"))
        .def("GetSensorValues", &MainSystem::PyGetSensorValues, "get sensors's values for configuration; can be a scalar or vector-valued return value!", py::arg("sensorNumber"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetSensorParameter", &MainSystem::PyGetSensorParameter, "get sensors's parameter from sensorNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("sensorNumber"), py::arg("parameterName"))
        .def("SetSensorParameter", &MainSystem::PySetSensorParameter, "set parameter 'parameterName' of sensor with sensorNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("sensorNumber"), py::arg("parameterName"), py::arg("value"))
        ; // end of MainSystem pybind definitions


    py::class_<MainSystemData>(m, "SystemData")
        .def(py::init<>())

//        General functions:
        .def("NumberOfLoads", [](const MainSystemData& msd) {return msd.GetMainLoads().NumberOfItems(); }, "return number of loads in system")
        .def("NumberOfMarkers", [](const MainSystemData& msd) {return msd.GetMainMarkers().NumberOfItems(); }, "return number of markers in system")
        .def("NumberOfNodes", [](const MainSystemData& msd) {return msd.GetMainNodes().NumberOfItems(); }, "return number of nodes in system")
        .def("NumberOfObjects", [](const MainSystemData& msd) {return msd.GetMainObjects().NumberOfItems(); }, "return number of objects in system")
        .def("GetTime", &MainSystemData::PyGetStateTime, "get configuration dependent time.", py::arg("configurationType") = ConfigurationType::Current)
        .def("SetTime", &MainSystemData::PySetStateTime, "set configuration dependent time; use this access with care, e.g. in user-defined solvers.", py::arg("newTime"), py::arg("configurationType") = ConfigurationType::Current)
        .def("GetCurrentTime", &MainSystemData::PyGetCurrentTime, "DEPRICATED; get current (simulation) time; time is updated in time integration solvers and in static solver; use this function e.g. during simulation to define time-dependent loads")
        .def("SetVisualizationTime", &MainSystemData::PySetVisualizationTime, "DEPRICATED; set time for render window (visualization)")
        .def("Info", [](const MainSystemData& msd) {pout << msd.PyInfoDetailed(); }, "print detailed system information for every item; for short information use print(mbs)")

//        Coordinate access:
        .def("GetODE2Coordinates", &MainSystemData::GetODE2Coords, "get ODE2 system coordinates (displacements) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE2Coordinates", &MainSystemData::SetODE2Coords, "set ODE2 system coordinates (displacements) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetODE2Coordinates_t", &MainSystemData::GetODE2Coords_t, "get ODE2 system coordinates (velocities) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE2Coordinates_t", &MainSystemData::SetODE2Coords_t, "set ODE2 system coordinates (velocities) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetODE1Coordinates", &MainSystemData::GetODE1Coords, "get ODE1 system coordinates (displacements) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE1Coordinates", &MainSystemData::SetODE1Coords, "set ODE1 system coordinates (displacements) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetAECoordinates", &MainSystemData::GetAECoords, "get algebraic equations (AE) system coordinates for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetAECoordinates", &MainSystemData::SetAECoords, "set algebraic equations (AE) system coordinates for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetDataCoordinates", &MainSystemData::GetDataCoords, "get system data coordinates for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetDataCoordinates", &MainSystemData::SetDataCoords, "set system data coordinates for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetSystemState", &MainSystemData::PyGetSystemState, "get system state for given configuration (default: exu.Configuration.Current); state vectors do not include the non-state derivatives ODE1_t and ODE2_tt and the time; function is copying data - not highly efficient; format of pyList: [ODE2Coords, ODE2Coords_t, ODE1Coords, AEcoords, dataCoords]", py::arg("configuration") = ConfigurationType::Current)
        .def("SetSystemState", &MainSystemData::PySetSystemState, "set system data coordinates for given configuration (default: exu.Configuration.Current); invalid list of vectors / vector size may lead to system crash; write access to state vectors (but not the non-state derivatives ODE1_t and ODE2_tt and the time); function is copying data - not highly efficient; format of pyList: [ODE2Coords, ODE2Coords_t, ODE1Coords, AEcoords, dataCoords]", py::arg("systemStateList"), py::arg("configuration") = ConfigurationType::Current)

//        LTG readout functions:
        .def("GetObjectLTGODE2", &MainSystemData::PyGetObjectLocalToGlobalODE2, "get local-to-global coordinate mapping (list of global coordinate indices) for ODE2 coordinates; only available after Assemble()", py::arg("objectNumber"))
        .def("GetObjectLTGODE1", &MainSystemData::PyGetObjectLocalToGlobalODE1, "get local-to-global coordinate mapping (list of global coordinate indices) for ODE1 coordinates; only available after Assemble()", py::arg("objectNumber"))
        .def("GetObjectLTGAE", &MainSystemData::PyGetObjectLocalToGlobalAE, "get local-to-global coordinate mapping (list of global coordinate indices) for algebraic equations (AE) coordinates; only available after Assemble()", py::arg("objectNumber"))
        .def("GetObjectLTGData", &MainSystemData::PyGetObjectLocalToGlobalData, "get local-to-global coordinate mapping (list of global coordinate indices) for data coordinates; only available after Assemble()", py::arg("objectNumber"))
        ; // end of SystemData pybind definitions

