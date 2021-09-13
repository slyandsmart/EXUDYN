// AUTO:  ++++++++++++++++++++++
// AUTO:  pybind11 manual module includes; generated by Johannes Gerstmayr
// AUTO:  last modified = 2021-08-26
// AUTO:  ++++++++++++++++++++++

//        pybinding to enum classes:
  py::enum_<OutputVariableType>(m, "OutputVariableType")
		.value("_None", OutputVariableType::_None)    //no value; used, e.g., to select no output variable in contour plot
		.value("Distance", OutputVariableType::Distance)    //e.g., measure distance in spring damper connector
		.value("Position", OutputVariableType::Position)    //measure 3D position, e.g., of node or body
		.value("Displacement", OutputVariableType::Displacement)    //measure displacement; usually difference between current position and reference position
		.value("DisplacementLocal", OutputVariableType::DisplacementLocal)    //measure local displacement, e.g. in local joint coordinates
		.value("Velocity", OutputVariableType::Velocity)    //measure (translational) velocity of node or object
		.value("VelocityLocal", OutputVariableType::VelocityLocal)    //measure local (translational) velocity, e.g. in local body or joint coordinates
		.value("Acceleration", OutputVariableType::Acceleration)    //measure (translational) acceleration of node or object
		.value("RotationMatrix", OutputVariableType::RotationMatrix)    //measure rotation matrix of rigid body node or object
		.value("Rotation", OutputVariableType::Rotation)    //measure, e.g., scalar rotation of 2D body, Euler angles of a 3D object or rotation within a joint
		.value("AngularVelocity", OutputVariableType::AngularVelocity)    //measure angular velocity of node or object
		.value("AngularVelocityLocal", OutputVariableType::AngularVelocityLocal)    //measure local (body-fixed) angular velocity of node or object
		.value("AngularAcceleration", OutputVariableType::AngularAcceleration)    //measure angular acceleration of node or object
		.value("Coordinates", OutputVariableType::Coordinates)    //measure the coordinates of a node or object; coordinates usually just contain displacements, but not the position values
		.value("Coordinates_t", OutputVariableType::Coordinates_t)    //measure the time derivative of coordinates (= velocity coordinates) of a node or object
		.value("Coordinates_tt", OutputVariableType::Coordinates_tt)    //measure the second time derivative of coordinates (= acceleration coordinates) of a node or object
		.value("SlidingCoordinate", OutputVariableType::SlidingCoordinate)    //measure sliding coordinate in sliding joint
		.value("Director1", OutputVariableType::Director1)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 1 or x-direction
		.value("Director2", OutputVariableType::Director2)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 2 or y-direction
		.value("Director3", OutputVariableType::Director3)    //measure a director (e.g. of a rigid body frame), or a slope vector in local 3 or z-direction
		.value("Force", OutputVariableType::Force)    //measure global force, e.g., in joint or beam (resultant force), or generalized forces; see description of according object
		.value("ForceLocal", OutputVariableType::ForceLocal)    //measure local force, e.g., in joint or beam (resultant force)
		.value("Torque", OutputVariableType::Torque)    //measure torque, e.g., in joint or beam (resultant couple/moment)
		.value("TorqueLocal", OutputVariableType::TorqueLocal)    //measure local torque, e.g., in joint or beam (resultant couple/moment)
		.value("StrainLocal", OutputVariableType::StrainLocal)    //measure local strain, e.g., axial strain in cross section frame of beam or Green-Lagrange strain
		.value("StressLocal", OutputVariableType::StressLocal)    //measure local stress, e.g., axial stress in cross section frame of beam or Second Piola-Kirchoff stress; choosing component==-1 will result in the computation of the Mises stress
		.value("CurvatureLocal", OutputVariableType::CurvatureLocal)    //measure local curvature; may be scalar or vectorial: twist and curvature of beam in cross section frame
		.value("ConstraintEquation", OutputVariableType::ConstraintEquation)    //evaluates constraint equation (=current deviation or drift of constraint equation)
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

  py::enum_<ItemType>(m, "ItemType")
		.value("_None", ItemType::_None)    //item has no type
		.value("Node", ItemType::Node)    //item or index is of type Node
		.value("Object", ItemType::Object)    //item or index is of type Object
		.value("Marker", ItemType::Marker)    //item or index is of type Marker
		.value("Load", ItemType::Load)    //item or index is of type Load
		.value("Sensor", ItemType::Sensor)    //item or index is of type Sensor
		.export_values();

  py::enum_<Node::Type>(m, "NodeType")
		.value("_None", Node::_None)    //node has no type
		.value("Ground", Node::Ground)    //ground node
		.value("Position2D", Node::Position2D)    //2D position node 
		.value("Orientation2D", Node::Orientation2D)    //node with 2D rotation
		.value("Point2DSlope1", Node::Point2DSlope1)    //2D node with 1 slope vector
		.value("Position", Node::Position)    //3D position node
		.value("Orientation", Node::Orientation)    //3D orientation node
		.value("RigidBody", Node::RigidBody)    //node that can be used for rigid bodies
		.value("RotationEulerParameters", Node::RotationEulerParameters)    //node with 3D orientations that are modelled with Euler parameters (unit quaternions)
		.value("RotationRxyz", Node::RotationRxyz)    //node with 3D orientations that are modelled with Tait-Bryan angles
		.value("RotationRotationVector", Node::RotationRotationVector)    //node with 3D orientations that are modelled with the rotation vector
		.value("RotationLieGroup", Node::RotationLieGroup)    //node intended to be solved with Lie group methods
		.value("GenericODE2", Node::GenericODE2)    //node with general ODE2 variables
		.value("GenericODE1", Node::GenericODE1)    //node with general ODE1 variables
		.value("GenericAE", Node::GenericAE)    //node with general algebraic variables
		.value("GenericData", Node::GenericData)    //node with general data variables
		.export_values();

  py::enum_<DynamicSolverType>(m, "DynamicSolverType")
		.value("GeneralizedAlpha", DynamicSolverType::GeneralizedAlpha)    //an implicit solver for index 3 problems; allows to set variables also for Newmark and trapezoidal implicit index 2 solvers
		.value("TrapezoidalIndex2", DynamicSolverType::TrapezoidalIndex2)    //an implicit solver for index 3 problems with index2 reduction; uses generalized alpha solver with settings for Newmark with index2 reduction
		.value("ExplicitEuler", DynamicSolverType::ExplicitEuler)    //an explicit 1st order solver (generally not compatible with constraints)
		.value("ExplicitMidpoint", DynamicSolverType::ExplicitMidpoint)    //an explicit 2nd order solver (generally not compatible with constraints)
		.value("RK33", DynamicSolverType::RK33)    //an explicit 3 stage 3rd order Runge-Kutta method, aka "Heun"; (generally not compatible with constraints)
		.value("RK44", DynamicSolverType::RK44)    //an explicit 4 stage 4th order Runge-Kutta method, aka "classical Runge Kutta" (generally not compatible with constraints), compatible with Lie group integration and elimination of CoordinateConstraints
		.value("RK67", DynamicSolverType::RK67)    //an explicit 7 stage 6th order Runge-Kutta method, see 'On Runge-Kutta Processes of High Order', J. C. Butcher, J. Austr Math Soc 4, (1964); can be used for very accurate (reference) solutions, but without step size control!
		.value("ODE23", DynamicSolverType::ODE23)    //an explicit Runge Kutta method with automatic step size selection with 3rd order of accuracy and 2nd order error estimation, see Bogacki and Shampine, 1989; also known as ODE23 in MATLAB
		.value("DOPRI5", DynamicSolverType::DOPRI5)    //an explicit Runge Kutta method with automatic step size selection with 5th order of accuracy and 4th order error estimation, see  Dormand and Prince, 'A Family of Embedded Runge-Kutta Formulae.', J. Comp. Appl. Math. 6, 1980
		.value("DVERK6", DynamicSolverType::DVERK6)    //[NOT IMPLEMENTED YET] an explicit Runge Kutta solver of 6th order with 5th order error estimation; includes adaptive step selection
		.export_values();

  py::enum_<KeyCode>(m, "KeyCode")
		.value("SPACE", KeyCode::SPACE)    //space key
		.value("ENTER", KeyCode::ENTER)    //enter (return) key
		.value("TAB", KeyCode::TAB)    //
		.value("BACKSPACE", KeyCode::BACKSPACE)    //
		.value("RIGHT", KeyCode::RIGHT)    //cursor right
		.value("LEFT", KeyCode::LEFT)    //cursor left
		.value("DOWN", KeyCode::DOWN)    //cursor down
		.value("UP", KeyCode::UP)    //cursor up
		.value("F1", KeyCode::F1)    //function key F1
		.value("F2", KeyCode::F2)    //function key F2
		.value("F3", KeyCode::F3)    //function key F3
		.value("F4", KeyCode::F4)    //function key F4
		.value("F5", KeyCode::F5)    //function key F5
		.value("F6", KeyCode::F6)    //function key F6
		.value("F7", KeyCode::F7)    //function key F7
		.value("F8", KeyCode::F8)    //function key F8
		.value("F9", KeyCode::F9)    //function key F9
		.value("F10", KeyCode::F10)    //function key F10
		.export_values();

  py::enum_<LinearSolverType>(m, "LinearSolverType")
		.value("_None", LinearSolverType::_None)    //no value; used, e.g., if no solver is selected
		.value("EXUdense", LinearSolverType::EXUdense)    //use dense matrices and according solvers for densly populated matrices (usually the CPU time grows cubically with the number of unknowns)
		.value("EigenSparse", LinearSolverType::EigenSparse)    //use sparse matrices and according solvers; additional overhead for very small systems; specifically, memory allocation is performed during a factorization process
		.export_values();


        m.def("GetVersionString", &PyGetVersionString, "Get Exudyn module version as string");
        m.def("StartRenderer", &PyStartOpenGLRenderer, "Start OpenGL rendering engine (in separate thread); use verbose=True to output information during OpenGL window creation; some of the information will only be seen in windows command (powershell) windows or linux shell, but not inside iPython of Spyder", py::arg("verbose") = false);
        m.def("DoRendererIdleTasks", &PyDoRendererIdleTasks, "Call this function in order to interact with Renderer window; use waitSeconds in order to run this idle tasks while animating a model (e.g. waitSeconds=0.04), use waitSeconds=0 without waiting, or use waitSeconds=-1 to wait until window is closed", py::arg("waitSeconds") = 0);
        m.def("SetOutputPrecision", &PySetOutputPrecision, "Set the precision (integer) for floating point numbers written to console (reset when simulation is started!)", py::arg("numberOfDigits"));
        m.def("SetLinalgOutputFormatPython", &PySetLinalgOutputFormatPython, "true: use python format for output of vectors and matrices; false: use matlab format", py::arg("flagPythonFormat"));
        m.def("SetWriteToConsole", &PySetWriteToConsole, "set flag to write (true) or not write to console; default = true", py::arg("flag"));
        m.def("SetWriteToFile", &PySetWriteToFile, "set flag to write (true) or not write to console; default value of flagWriteToFile = false; flagAppend appends output to file, if set true; in order to finalize the file, write exu.SetWriteToFile('', False) to close the output file", py::arg("filename"), py::arg("flagWriteToFile") = true, py::arg("flagAppend") = false);
        m.def("SetPrintDelayMilliSeconds", &PySetPrintDelayMilliSeconds, "add some delay (in milliSeconds) to printing to console, in order to let Spyder process the output; default = 0", py::arg("delayMilliSeconds"));
        m.def("Print", &PyPrint, "this allows printing via exudyn with similar syntax as in python print(args) except for keyword arguments: print('test=',42); allows to redirect all output to file given by SetWriteToFile(...); does not output in case that SetWriteToConsole is set to false");
        m.def("InfoStat", &PythonInfoStat, "Retrieve list of global information on memory allocation and other counts as list:[array_new_counts, array_delete_counts, vector_new_counts, vector_delete_counts, matrix_new_counts, matrix_delete_counts, linkedDataVectorCast_counts]; May be extended in future; if writeOutput==True, it additionally prints the statistics; counts for new vectors and matrices should not depend on numberOfSteps, except for some objects such as ObjectGenericODE2 and for (sensor) output to files; Not available if code is compiled with __FAST_EXUDYN_LINALG flag", py::arg("writeOutput") = true);
        m.def("Go", &PythonGo, "Creates a SystemContainer SC and a main system mbs");
        m.def("InvalidIndex", &GetInvalidIndex, "This function provides the invalid index, which may depend on the kind of 32-bit, 64-bit signed or unsigned integer; e.g. node index or item index in list; currently, the InvalidIndex() gives -1, but it may be changed in future versions, therefore you should use this function");
        m.attr("variables") = exudynVariables;
        m.attr("sys") = exudynSystemVariables;

    py::class_<MainSystem>(m, "MainSystem")
        .def(py::init<>())
        .def("Assemble", &MainSystem::Assemble, "assemble items (nodes, bodies, markers, loads, ...); Calls CheckSystemIntegrity(...), AssembleCoordinates(), AssembleLTGLists(), and AssembleInitializeSystemCoordinates()")
        .def("AssembleCoordinates", &MainSystem::AssembleCoordinates, "assemble coordinates: assign computational coordinates to nodes and constraints (algebraic variables)")
        .def("AssembleLTGLists", &MainSystem::AssembleLTGLists, "build local-to-global (ltg) coordinate lists for objects (used to build global ODE2RHS, MassMatrix, etc. vectors and matrices) and store special object lists (body, connector, constraint, ...)")
        .def("AssembleInitializeSystemCoordinates", &MainSystem::AssembleInitializeSystemCoordinates, "initialize all system-wide coordinates based on initial values given in nodes")
        .def("Reset", &MainSystem::Reset, "reset all lists of items (nodes, bodies, markers, loads, ...) and temporary vectors; deallocate memory")
        .def("GetSystemContainer", &MainSystem::GetMainSystemContainer, "return the systemContainer where the mainSystem (mbs) was created")
        .def("WaitForUserToContinue", &MainSystem::WaitForUserToContinue, "interrupt further computation until user input --> 'pause' function")
        .def("SendRedrawSignal", &MainSystem::SendRedrawSignal, "this function is used to send a signal to the renderer that the scene shall be redrawn because the visualization state has been updated")
        .def("GetRenderEngineStopFlag", &MainSystem::GetRenderEngineStopFlag, "get the current stop simulation flag; true=user wants to stop simulation")
        .def("SetRenderEngineStopFlag", &MainSystem::SetRenderEngineStopFlag, "set the current stop simulation flag; set to false, in order to continue a previously user-interrupted simulation")
        .def("ActivateRendering", &MainSystem::ActivateRendering, "activate (flag=True) or deactivate (flag=False) rendering for this system", py::arg("flag") = true)
        .def("SetPreStepUserFunction", &MainSystem::PySetPreStepUserFunction, "Sets a user function PreStepUserFunction(mbs, t) executed at beginning of every computation step; in normal case return True; return False to stop simulation after current step")
        .def("SetPostNewtonUserFunction", &MainSystem::PySetPostNewtonUserFunction, "Sets a user function PostNewtonUserFunction(mbs, t) executed after successful Newton iteration in implicit or static solvers and after step update of explicit solvers, but BEFORE PostNewton functions are called by the solver; function returns list [discontinuousError, recommendedStepSize], containing a error of the PostNewtonStep, which is compared to [solver].discontinuous.iterationTolerance. The recommendedStepSize shall be negative, if no recommendation is given, 0 in order to enforce minimum step size or a specific value to which the current step size will be reduced and the step will be repeated; use this function, e.g., to reduce step size after impact or change of data variables")
        .def("__repr__", [](const MainSystem &ms) {
            return "<systemData: \n" + ms.GetMainSystemData().PyInfoSummary() + "\nFor details see mbs.systemData, mbs.sys and mbs.variables\n>\n"; }, "return the representation of the system, which can be, e.g., printed")
        .def_property("systemIsConsistent", &MainSystem::GetFlagSystemIsConsistent, &MainSystem::SetFlagSystemIsConsistent)
        .def_property("interactiveMode", &MainSystem::GetInteractiveMode, &MainSystem::SetInteractiveMode)
        .def_readwrite("variables", &MainSystem::variables, py::return_value_policy::reference)
        .def_readwrite("sys", &MainSystem::systemVariables, py::return_value_policy::reference)
        .def_property("solverSignalJacobianUpdate", &MainSystem::GetFlagSolverSignalJacobianUpdate, &MainSystem::SetFlagSolverSignalJacobianUpdate)
        .def_readwrite("systemData", &MainSystem::mainSystemData, py::return_value_policy::reference)

//        NODES:
        .def("AddNode", &MainSystem::AddMainNodePyClass, "add a node with nodeDefinition from Python node class; returns (global) node index (type NodeIndex) of newly added node; use int(nodeIndex) to convert to int, if needed (but not recommended in order not to mix up index types of nodes, objects, markers, ...)", py::arg("pyObject"))
        .def("GetNodeNumber", &MainSystem::PyGetNodeNumber, "get node's number by name (string)", py::arg("nodeName"))
        .def("GetNode", &MainSystem::PyGetNode, "get node's dictionary by node number (type NodeIndex)", py::arg("nodeNumber"))
        .def("ModifyNode", &MainSystem::PyModifyNode, "modify node's dictionary by node number (type NodeIndex)", py::arg("nodeNumber"), py::arg("nodeDict"))
        .def("GetNodeDefaults", &MainSystem::PyGetNodeDefaults, "get node's default values for a certain nodeType as (dictionary)", py::arg("typeName"))
        .def("GetNodeOutput", &MainSystem::PyGetNodeOutputVariable, "get the ouput of the node specified with the OutputVariableType; default configuration = 'current'; output may be scalar or array (e.g. displacement vector)", py::arg("nodeNumber"), py::arg("variableType"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetNodeODE2Index", &MainSystem::PyGetNodeODE2Index, "get index in the global ODE2 coordinate vector for the first node coordinate of the specified node", py::arg("nodeNumber"))
        .def("GetNodeParameter", &MainSystem::PyGetNodeParameter, "get nodes's parameter from node number (type NodeIndex) and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("nodeNumber"), py::arg("parameterName"))
        .def("SetNodeParameter", &MainSystem::PySetNodeParameter, "set parameter 'parameterName' of node with node number (type NodeIndex) to value; parameter names can be found for the specific items in the reference manual", py::arg("nodeNumber"), py::arg("parameterName"), py::arg("value"))

//        OBJECTS:
        .def("AddObject", &MainSystem::AddMainObjectPyClass, "add an object with objectDefinition from Python object class; returns (global) object number (type ObjectIndex) of newly added object", py::arg("pyObject"))
        .def("GetObjectNumber", &MainSystem::PyGetObjectNumber, "get object's number by name (string)", py::arg("objectName"))
        .def("GetObject", &MainSystem::PyGetObject, "get object's dictionary by object number (type ObjectIndex)", py::arg("objectNumber"))
        .def("ModifyObject", &MainSystem::PyModifyObject, "modify object's dictionary by object number (type ObjectIndex)", py::arg("objectNumber"), py::arg("objectDict"))
        .def("GetObjectDefaults", &MainSystem::PyGetObjectDefaults, "get object's default values for a certain objectType as (dictionary)", py::arg("typeName"))
        .def("GetObjectOutput", &MainSystem::PyGetObjectOutputVariable, "get object's current output variable from object number (type ObjectIndex) and OutputVariableType; can only be computed for exu.ConfigurationType.Current configuration!", py::arg("objectNumber"), py::arg("variableType"))
        .def("GetObjectOutputBody", &MainSystem::PyGetObjectOutputVariableBody, "get body's output variable from object number (type ObjectIndex) and OutputVariableType, using the localPosition $\pLocB$ similar to MarkerBody and SensorBody", py::arg("objectNumber"), py::arg("variableType"), py::arg("localPosition"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetObjectOutputSuperElement", &MainSystem::PyGetObjectOutputVariableSuperElement, "get output variable from mesh node number of object with type SuperElement (GenericODE2, FFRF, FFRFreduced - CMS) with specific OutputVariableType; the meshNodeNumber is the object's local node number, not the global node number!", py::arg("objectNumber"), py::arg("variableType"), py::arg("meshNodeNumber"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetObjectParameter", &MainSystem::PyGetObjectParameter, "get objects's parameter from object number (type ObjectIndex) and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("objectNumber"), py::arg("parameterName"))
        .def("SetObjectParameter", &MainSystem::PySetObjectParameter, "set parameter 'parameterName' of object with object number (type ObjectIndex) to value; parameter names can be found for the specific items in the reference manual", py::arg("objectNumber"), py::arg("parameterName"), py::arg("value"))

//        MARKER:
        .def("AddMarker", &MainSystem::AddMainMarkerPyClass, "add a marker with markerDefinition from Python marker class; returns (global) marker number (type MarkerIndex) of newly added marker", py::arg("pyObject"))
        .def("GetMarkerNumber", &MainSystem::PyGetMarkerNumber, "get marker's number by name (string)", py::arg("markerName"))
        .def("GetMarker", &MainSystem::PyGetMarker, "get marker's dictionary by index", py::arg("markerNumber"))
        .def("ModifyMarker", &MainSystem::PyModifyMarker, "modify marker's dictionary by index", py::arg("markerNumber"), py::arg("markerDict"))
        .def("GetMarkerDefaults", &MainSystem::PyGetMarkerDefaults, "get marker's default values for a certain markerType as (dictionary)", py::arg("typeName"))
        .def("GetMarkerParameter", &MainSystem::PyGetMarkerParameter, "get markers's parameter from markerNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("markerNumber"), py::arg("parameterName"))
        .def("SetMarkerParameter", &MainSystem::PySetMarkerParameter, "set parameter 'parameterName' of marker with markerNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("markerNumber"), py::arg("parameterName"), py::arg("value"))

//        LOADS:
        .def("AddLoad", &MainSystem::AddMainLoadPyClass, "add a load with loadDefinition from Python load class; returns (global) load number (type LoadIndex) of newly added load", py::arg("pyObject"))
        .def("GetLoadNumber", &MainSystem::PyGetLoadNumber, "get load's number by name (string)", py::arg("loadName"))
        .def("GetLoad", &MainSystem::PyGetLoad, "get load's dictionary by index", py::arg("loadNumber"))
        .def("ModifyLoad", &MainSystem::PyModifyLoad, "modify load's dictionary by index", py::arg("loadNumber"), py::arg("loadDict"))
        .def("GetLoadDefaults", &MainSystem::PyGetLoadDefaults, "get load's default values for a certain loadType as (dictionary)", py::arg("typeName"))
        .def("GetLoadValues", &MainSystem::PyGetLoadValues, "Get current load values, specifically if user-defined loads are used; can be scalar or vector-valued return value", py::arg("loadNumber"))
        .def("GetLoadParameter", &MainSystem::PyGetLoadParameter, "get loads's parameter from loadNumber and parameterName; parameter names can be found for the specific items in the reference manual", py::arg("loadNumber"), py::arg("parameterName"))
        .def("SetLoadParameter", &MainSystem::PySetLoadParameter, "set parameter 'parameterName' of load with loadNumber to value; parameter names can be found for the specific items in the reference manual", py::arg("loadNumber"), py::arg("parameterName"), py::arg("value"))

//        SENSORS:
        .def("AddSensor", &MainSystem::AddMainSensorPyClass, "add a sensor with sensor definition from Python sensor class; returns (global) sensor number (type SensorIndex) of newly added sensor", py::arg("pyObject"))
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
        .def("NumberOfSensors", [](const MainSystemData& msd) {return msd.GetMainSensors().NumberOfItems(); }, "return number of sensors in system")
        .def("ODE2Size", &MainSystemData::PyODE2Size, "get size of ODE2 coordinate vector for given configuration (only works correctly after mbs.Assemble() )", py::arg("configurationType") = ConfigurationType::Current)
        .def("ODE1Size", &MainSystemData::PyODE1Size, "get size of ODE1 coordinate vector for given configuration (only works correctly after mbs.Assemble() )", py::arg("configurationType") = ConfigurationType::Current)
        .def("AEsize", &MainSystemData::PyAEsize, "get size of AE coordinate vector for given configuration (only works correctly after mbs.Assemble() )", py::arg("configurationType") = ConfigurationType::Current)
        .def("DataSize", &MainSystemData::PyDataSize, "get size of Data coordinate vector for given configuration (only works correctly after mbs.Assemble() )", py::arg("configurationType") = ConfigurationType::Current)
        .def("SystemSize", &MainSystemData::PySystemSize, "get size of System coordinate vector for given configuration (only works correctly after mbs.Assemble() )", py::arg("configurationType") = ConfigurationType::Current)
        .def("GetTime", &MainSystemData::PyGetStateTime, "get configuration dependent time.", py::arg("configurationType") = ConfigurationType::Current)
        .def("SetTime", &MainSystemData::PySetStateTime, "set configuration dependent time; use this access with care, e.g. in user-defined solvers.", py::arg("newTime"), py::arg("configurationType") = ConfigurationType::Current)
        .def("Info", [](const MainSystemData& msd) {pout << msd.PyInfoDetailed(); }, "print detailed system information for every item; for short information use print(mbs)")

//        Coordinate access:
        .def("GetODE2Coordinates", &MainSystemData::GetODE2Coords, "get ODE2 system coordinates (displacements) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE2Coordinates", &MainSystemData::SetODE2Coords, "set ODE2 system coordinates (displacements) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetODE2Coordinates_t", &MainSystemData::GetODE2Coords_t, "get ODE2 system coordinates (velocities) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE2Coordinates_t", &MainSystemData::SetODE2Coords_t, "set ODE2 system coordinates (velocities) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
        .def("GetODE2Coordinates_tt", &MainSystemData::GetODE2Coords_tt, "get ODE2 system coordinates (accelerations) for given configuration (default: exu.Configuration.Current)", py::arg("configuration") = ConfigurationType::Current)
        .def("SetODE2Coordinates_tt", &MainSystemData::SetODE2Coords_tt, "set ODE2 system coordinates (accelerations) for given configuration (default: exu.Configuration.Current); invalid vector size may lead to system crash!", py::arg("coordinates"), py::arg("configuration") = ConfigurationType::Current)
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

