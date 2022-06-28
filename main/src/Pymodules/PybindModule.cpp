/** ***********************************************************************************************
* @brief		Implementation file for linking to python; includes several automatically generated header files inside of module (kind of hack)
* @details		Details:
				- main module linked to python (python interface)
*
* @author		Stefan Holzinger / Gerstmayr Johannes
* @date			2019-03-29 
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

//++++++++++++++++++++++++++
//for signal, catch CTRL-C in console:
//#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <signal.h>
//++++++++++++++++++++++++++

// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>       //interface to numpy
#include <pybind11/buffer_info.h> //passing reference to matrix to numpy
#include <pybind11/embed.h>       //scoped interpreter
//does not work globally: #include <pybind11/iostream.h> //used to redirect cout:  py::scoped_ostream_redirect output;
#include <pybind11/cast.h> //for arguments
#include <pybind11/functional.h> //for function handling ... otherwise gives a python error (no compilation error in C++ !)
namespace py = pybind11;
using namespace pybind11::literals; //brings in the '_a' literals; e.g. for short arguments definition

// includes needed for glfw test example
#define NOMINMAX //needs to be placed on top (before windows.h)! Otherwise std::min/max will cause error msg!
#include <cmath>

#include "Linalg/BasicLinalg.h"

// glfw testclass. This  includes the glfw test example
#include "Graphics/GlfwClient.h"

#include <iostream>
#include <ostream>
#include <fstream>

#include "System/versionCpp.h"

#include "Linalg/LinearSolver.h"

#include "Linalg/Geometry.h"

#include "Main/MainSystem.h"

#include "Pymodules/PybindUtilities.h"
#include "Pymodules/PyMatrixContainer.h"
#include "Pymodules/PyMatrixVector.h"
#include "Pymodules/PyGeneralContact.h"
#include "Autogenerated/MainObjectMassPoint.h"
#include "Autogenerated/MainNodePoint.h"

//#include "Solver/TimeIntegrationSolver.h"
#include "Main/SystemContainer.h"

#include "Main/MainSystemContainer.h"
#include "System/ItemIndices.h"

//included already in MainSolverBase:
//#include "Solver/CSolverImplicitSecondOrder.h" 
//#include "Solver/CSolverStatic.h"
//#include "Solver/CSolverExplicit.h"
#include "Solver/MainSolverBase.h"
#include "Autogenerated/MainSolver.h" //pybind access to solvers
#include "Autogenerated/SimulationSettings.h" 
#include "Autogenerated/PyStructuralElementsDataStructures.h"
#include "Autogenerated/BeamSectionGeometry.h"
#include "Autogenerated/DictionariesGetSet.h"


#include "Tests/UnitTestBase.h" //for unit tests

#include "Pymodules/PybindTests.h"


#ifdef __EXUDYN_RUNTIME_CHECKS__
extern Index array_new_counts;		//global counter of item allocations; is increased every time a new is called
extern Index array_delete_counts;	//global counter of item deallocations; is increased every time a delete is called
extern Index vector_new_counts;	//global counter of item allocations; is increased every time a new is called
extern Index vector_delete_counts; //global counter of item deallocations; is increased every time a delete is called
extern Index matrix_new_counts;	//global counter of item allocations; is increased every time a new is called
extern Index matrix_delete_counts; //global counter of item deallocations; is increased every time a delete is called
extern Index linkedDataVectorCast_counts; //global counter for unwanted type conversion from LinkedDataVector to Vector
#endif

#pragma message("==========================")
#ifdef use_AVX2
#pragma message("** compiled with AVX2 **")
#elif defined(use_AVX512)
#pragma message("** compiled with AVX512 **")
#else
#pragma message("** compiled without AVX **")
#endif

//check some platform / architecture or compiler specific things at which is compiled and define globally used flags:
#if defined(__EXUDYN__APPLEM1__)
#pragma message("*** compiled for MacOS (ARM M1) ***")
#elif defined(__EXUDYN__APPLE__)
#pragma message("*** compiled for MacOS (x86) ***")
#elif defined(__EXUDYN__WINDOWS__)
	#if defined(_WIN32) && !defined(_WIN64) //_WIN32 also defined in 64 bits mode!
	#pragma message("*** compiled for Windows _x86 (32bits) ***") //this works for VS2017
    #elif defined(_WIN64)
	#pragma message("*** compiled for Windows _x86 (64bits) ***") //this works for VS2017
	#else
	#pragma message("*** compiled for Windows _x86 ***")
	#endif
#elif defined(__EXUDYN__LINUX__ARM__)
#pragma message("*** compiled for linux / ARM CPU ***")
#elif defined(__EXUDYN__LINUX__x86__)
#pragma message("*** compiled for linux / x86 CPU ***")
#else
#pragma message("*** WARNING: NO KNOWN PLATFORM DETECTED!!! ***")
#endif

////test verified with numpy:
//Matrix3D M(3, 3, { 0.1,2,0.3,-0.2,2,0.7,0.1,0.11,-3 });
//pout << "M=" << M << "\n";
//pout << "Minv=" << M.GetInverse() << "\n";


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//some low level functions linked to exudyn
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! this function returns the Python version for which Exudyn is compiled (even micro version, which may be different from Python interpreter!)
//! returns e.g. "3.9.0"
STDstring GetExudynPythonVersionString()
{
	STDstring str = EXUstd::ToString(PY_MAJOR_VERSION) + '.' + EXUstd::ToString(PY_MINOR_VERSION) + '.' + EXUstd::ToString(PY_MICRO_VERSION);
	return str;
}

//! this function is available outside PybindModule.cpp and returns version + additional information
STDstring GetExudynBuildVersionString(bool addDetails)
{
	STDstring str = STDstring(EXUstd::exudynVersion);
#ifndef EXUDYN_RELEASE
	str += "(pre-release)";
#pragma message("====================================")
#pragma message("EXUDYN not compiled in release mode!")
#pragma message("====================================")
#endif
	if (addDetails)
	{
		str += "; Python" + GetExudynPythonVersionString();
		str += "; " + EXUstd::GetPlatformString();
	}
#ifdef __FAST_EXUDYN_LINALG
	if (addDetails)
	{
		str += "[FAST]"; //changed from "[NO RANGE CHECKS]"
	}
#pragma message("====================================")
#pragma message("EXUDYN using __FAST_EXUDYN_LINALG without range checks!")
#pragma message("====================================")
#endif
	return str;
}

//! retrieve current version as m.attr is not passed trough package
py::str PyGetVersionString(bool addDetails = false)
{
	return GetExudynBuildVersionString(addDetails);
}

//! Definition of Invalid Index; to be used in Python to check whether a function returned a valid index (e.g. AddObject(...))
Index GetInvalidIndex() { return EXUstd::InvalidIndex; }

//! set flag to write (true) or not write to console; default = true
void PySetWriteToConsole(bool flag) { outputBuffer.SetWriteToConsole(flag); }

//! set flag to write (true) or not write to console; default = false
void PySetWriteToFile(STDstring filename, bool flagWriteToFile, bool flagAppend)
{
	outputBuffer.SetWriteToFile(filename, flagWriteToFile, flagAppend);
} 

extern bool suppressWarnings; //!< global flag to suppress warnings
//! set flag to suppress (=true) or enable (=false) warnings
void PySuppressWarnings(bool flag)
{
	suppressWarnings = flag;
}

////redirect printing via exudyn, such that all output can be streamed to file ...
//void PyPrint(py::object pyObject)
//{
//	pout << py::str(pyObject);
//	//py::print(pyObject);
//}
//
//! print function with line feed
void PyPrint(py::args args)
{
	for (auto item: args)
	{
		pout << item << " ";
	}
	pout << "\n";
}
//! add some delay (in milliSeconds) to printing to console, in order to let Spyder process the output; default = 0
void PySetPrintDelayMilliSeconds(Index delayMilliSeconds)
{
	outputBuffer.SetDelayMilliSeconds(delayMilliSeconds);
}


//Print some (Debug) infos: linalg, threads, computational efficiency, etc.
py::list PythonInfoStat(bool writeOutput = true)
{
	py::list list;
#ifdef __EXUDYN_RUNTIME_CHECKS__
	if (writeOutput)
	{
		pout << "Linalg stats:\n";
		pout << "  array_new_counts:   " << array_new_counts << "\n";
		pout << "  array_delete_counts:" << array_delete_counts << "\n";

		pout << "  vector_new_counts:   " << vector_new_counts << "\n";
		pout << "  vector_delete_counts:" << vector_delete_counts << "\n";

		pout << "  matrix_new_counts:   " << matrix_new_counts << "\n";
		pout << "  matrix_delete_counts:" << matrix_delete_counts << "\n";

		pout << "  linkedDataVec_counts:" << linkedDataVectorCast_counts << "\n";
	}
	list.append(array_new_counts);
	list.append(array_delete_counts);
	list.append(vector_new_counts);
	list.append(vector_delete_counts);
	list.append(matrix_new_counts);
	list.append(matrix_delete_counts);
	list.append(linkedDataVectorCast_counts);
#else
	if (writeOutput)
	{
		pout << "Linalg stats deactivated (needs re-compile)\n";
	}
	list.append(0); //used by Static/DynamicSolver, therefore list needs to be created
	list.append(0);
	list.append(0);
	list.append(0);
	list.append(0);
	list.append(0);
	list.append(0);
#endif
	return list;


}

//! Set the precision for floating point numbers written to console; this is reset after a simulation is started by according simulation settings
void PySetOutputPrecision(Index precision)
{
	std::cout.precision(precision);
	pout.precision(precision);
}

extern bool linalgPrintUsePythonFormat; //!< true: use python format for output of vectors and matrices; false: use matlab format

//! true: use python format for output of vectors and matrices; false: use matlab format
void PySetLinalgOutputFormatPython(bool flagPythonFormat)
{
	linalgPrintUsePythonFormat = flagPythonFormat;
}

//+++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++
////DELETE at 2021-07-01
//extern bool correctOldImplicitSolver; //experimental flag, will be erased in future; default=false
//
////! true: use python format for output of vectors and matrices; false: use matlab format
//void PyCorrectOldImplicitSolver(bool flag)
//{
//	correctOldImplicitSolver = flag;
//}
//+++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++

//DELETE and put into exudyn.__sys()['currentRendererSelectionDict'] internal exchange for right-mouse-click
//py::dict internalSelectionDict;
//py::dict PyGetInternalSelectionDict()
//{
//	return internalSelectionDict;
//}
//
//void PySetInternalSelectionDict(py::dict dict)
//{
//	internalSelectionDict = dict;
//}
//
//! add a structure which can be written to from C++, but accessed from exudyn Python module via exudyn.__sys()

// write access to system variables dictionary inside exudyn module
void PyWriteToSysDictionary(const STDstring& key, py::object item)
{
	py::module exudynModule = py::module::import("exudyn");
	exudynModule.attr("sys")[key.c_str()] = item;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! start glfw renderer; return true if successful
bool PyStartOpenGLRenderer(bool verbose=true)
{
#ifdef USE_GLFW_GRAPHICS
	return glfwRenderer.SetupRenderer(verbose);
#else
	PyWarning("StartRenderer(): has no effect as GLFW_GRAPHICS is deactivated in your exudyn module (needs recompile or another version)");
	return false;
#endif
}

//! start glfw renderer; return true if successful
void PyStopOpenGLRenderer()
{
#ifdef USE_GLFW_GRAPHICS
	glfwRenderer.StopRenderer();
#else
	PyWarning("StopRenderer(): has no effect as GLFW_GRAPHICS is deactivated in your exudyn module (needs recompile or another version)");
#endif
}

//! run renderer idle for certain amount of time; use this for single-threaded, interactive animations
void PyDoRendererIdleTasks(Real waitSeconds)
{
#ifdef USE_GLFW_GRAPHICS
	glfwRenderer.DoRendererIdleTasks(waitSeconds);
#else
	PyWarning("DoRendererIdleTasks(): has no effect as GLFW_GRAPHICS is deactivated in your exudyn module (needs recompile or another version)");
#endif
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) 
{
	if (signum == SIGINT)
	{
		//std::cout << "Caught signal " << signum << "\n";
		//std::cout << "Process interrupted by user (CTRL+C)!\n";
		pout << "Process interrupted by user (CTRL+C)!\n";
		// Terminate program
		exit(signum);
	}
}

//enable CTRL-C in DOS windows
class MySignal 
{
public:
	MySignal()
	{
		// Register signal and signal handler
		signal(SIGINT, signal_callback_handler);
	}
};

MySignal registerSignal;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#ifdef __FAST_EXUDYN_LINALG
PYBIND11_MODULE(exudynCPPfast, m) {
	m.doc() = "EXUDYN binding Python<->C++\n This is the 'fast' version without range/memory/whatsoever checks and uses /fp:fast compiler options!\n -> usage:\nSC=exu.SystemContainer()\nmbs=SC.AddSystem()\n see theDoc.pdf for tutorials, interface description and further information"; // module docstring
#pragma message("***** pybind: building exudynCPPfast module *****")
//#pragma message("***** pybind: building exudynCPPfast module *****")
//#pragma message("***** pybind: building exudynCPPfast module *****")
//#pragma message("***** pybind: building exudynCPPfast module *****")
#else
PYBIND11_MODULE(exudynCPP, m) {
	m.doc() = "EXUDYN binding Python<->C++\n -> usage:\nSC=exu.SystemContainer()\nmbs=SC.AddSystem()\n see theDoc.pdf for tutorials, interface description and further information"; // module docstring
//#pragma message("***** pybind: building exudynCPP module *****")
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//variables linked to exudyn
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	py::dict exudynVariables; //!< global dictionary which can be used by the user to store local variables
	py::dict exudynSystemVariables; //!< global dictionary which is used by system functions to store local variables

	//put into pybindings.py: m.def("GetVersionString", &PyGetVersionString, "get version as string as a function which is passed trough exudyn package");

	//m.def("PrintLF", &PyPrintLF, "this allows printing via exudyn with similar syntax as in python print(...) except for keyword arguments: print('test=',42); allows to redirect all output to file given by SetWriteToFile(...)");
	
	//m.def("PyThrowTest", &PyThrowTest, "PyThrowTest");

	//m.def("GetVector", &GetVector, "GetVector");
	//m.def("GetMatrix", &GetMatrix, "GetMatrix");
	//m.def("SeeMatrix", &SeeMatrix, "SeeMatrix");
#ifndef EXUDYN_RELEASE
	m.def("Test", &PyTest, "internal test, do not use");
	//m.def("GetTestSD", &GetTestSD, "test user function");
#endif
	//m.def("SetTestFunction2", &PySetTestFunction2, "Set the test function");
	//m.def("EvaluateTestFunction", &PyEvaluateTestFunction, "Evaluate test function");

	//DELETE: m.def("GetInternalSelectionDict", &PyGetInternalSelectionDict, "internal function, do not use");

	//m.def("test2", &PyGetInternalSysDictionary, "access to internal dictionary; do not use nor modify");

	//moved here in order to be able to store current renderState in exudynSystemVariables
	//m.def("StopOpenGLRenderer", &GetVector, "GetVector");
	m.def("StopRenderer", [exudynSystemVariables]() {
		try
		{
			PyStopOpenGLRenderer();
	#ifdef USE_GLFW_GRAPHICS
			py::dict d = MainSystemContainer::RenderState2PyDict(glfwRenderer.GetRenderState());
			exudynSystemVariables["renderState"] = d;
			//pout << "stop renderer\n";
	#endif
		}
		catch (const EXUexception& ex)
		{
			SysError("EXUDYN raised internal error in StopOpenGLRenderer:\n" + STDstring(ex.what()) + "\n");
		}
		catch (...) //any other exception
		{
			SysError("Unexpected exception during StopOpenGLRenderer!\n");
		}

	}, "Stop the openGL renderer and write current renderState to exudyn.sys['renderState']");




#ifdef _MYDEBUG //only in debug mode!
	m.def("CreateTestSystem", &CreateTestSystem, "Create test system with systemNumber and optional arguments; will create the system depending on arg1 and arg2", py::arg("systemNumber")=0, py::arg("arg0") = 0, py::arg("arg1") = 0);
#endif

	py::enum_<AccessFunctionType>(m, "AccessFunctionType")
		.value("_None", AccessFunctionType::_None)
		.value("TranslationalVelocity_qt", AccessFunctionType::TranslationalVelocity_qt)
		.value("AngularVelocity_qt", AccessFunctionType::AngularVelocity_qt)
		.value("Coordinate_q", AccessFunctionType::Coordinate_q)
		.value("DisplacementLineIntegral_q", AccessFunctionType::DisplacementLineIntegral_q)
		.value("DisplacementSurfaceIntegral_q", AccessFunctionType::DisplacementSurfaceIntegral_q)
		.value("DisplacementVolumeIntegral_q", AccessFunctionType::DisplacementVolumeIntegral_q)
		.value("DisplacementMassIntegral_q", AccessFunctionType::DisplacementMassIntegral_q)
		.value("DisplacementSurfaceNormalIntegral_q", AccessFunctionType::DisplacementSurfaceNormalIntegral_q)
		//.value("Rotv1v2v3", AccessFunctionType::Rotv1v2v3_q)
		//.value("EndOfEnumList", AccessFunctionType::EndOfEnumList)
		.export_values();

	py::enum_<CObjectType>(m, "ObjectType")
		.value("_None", CObjectType::_None)
		.value("Ground", CObjectType::Ground)
		.value("Constraint", CObjectType::Constraint)
		.value("Connector", CObjectType::Connector)
		.value("Body", CObjectType::Body)
		.value("SingleNoded", CObjectType::SingleNoded)
		.value("MultiNoded", CObjectType::MultiNoded)
		.value("FiniteElement", CObjectType::FiniteElement)
		.value("SuperElement", CObjectType::SuperElement)
		.value("EndOfEnumList", CObjectType::EndOfEnumList)
		.export_values();


	//py::enum_<CNodeType>(m, "CNodeType")
	//	.value("_None", CNodeType::_None)
	//	.value("Point", CNodeType::Point)
	//	.value("RigidBody", CNodeType::RigidBody)
	//	.value("Temperature", CNodeType::Temperature)
	//	.value("General", CNodeType::General)
	//	.export_values();

		//py::enum_<Node::Type>(m, "NodeType")
		//.value("_None", Node::_None)
		//.value("Ground", Node::Ground)
		//.value("Position2D", Node::Position2D)
		//.value("Orientation2D", Node::Orientation2D)
		//.value("Point2DSlope1", Node::Point2DSlope1)
		//.value("Position", Node::Position)
		//.value("Orientation", Node::Orientation)
		//.value("RigidBody", Node::RigidBody)
		//.value("RotationEulerParameters", Node::RotationEulerParameters)
		//.value("RotationRxyz", Node::RotationRxyz)
		//.value("RotationRotationVector", Node::RotationRotationVector)
		//.value("RotationLieGroup", Node::RotationLieGroup)
		//.value("GenericODE2", Node::GenericODE2)
		//.value("GenericODE1", Node::GenericODE1)
		//.value("GenericAE", Node::GenericAE)
		//.value("GenericData", Node::GenericData)
		//.export_values();

	py::enum_<CNodeGroup>(m, "NodeGroup")
		.value("_None", CNodeGroup::_None)
		.value("ODE1variables", CNodeGroup::ODE1variables)
		.value("ODE2variables", CNodeGroup::ODE2variables)
		.value("AEvariables", CNodeGroup::AEvariables)
		.value("DataVariables", CNodeGroup::DataVariables)
		.export_values();

	//py::enum_<LinearSolverType>(m, "LinearSolverType")
	//	.value("_None", LinearSolverType::_None)
	//	.value("EXUdense", LinearSolverType::EXUdense)
	//	.value("EigenSparse", LinearSolverType::EigenSparse)
	//	.export_values();

	//dirty include: autogenerated pybind functions added here!
	#include "Autogenerated/Pybind_modules.h"

#ifdef PERFORM_UNIT_TESTS
	m.def("RunCppUnitTests", &RunUnitTests, "Run C++ unit tests and return int with 'number of fails' (0 if all tests passed); reportOnPass=True also outputs the passed tests; printOutput prints according output to console", 
		py::arg("reportOnPass") = false, py::arg("printOutput") = true);
#endif
	//++++++++++++++++++++++++++++++++++++++++

	#include "Autogenerated/pybind_manual_classes.h"

	////++++++++++++++++++++++++++++++++++++++++
	//DELETE:
	//py::class_<MainSystemBase>(m, "MainSystemBase", "MainSystemBase: equal to MainSystem")
	//	.def(py::init<>())
	//	;
	////used to avoid including MainSystem in many places (but very slow!)
	//py::implicitly_convertible<MainSystemBase, MainSystem>();

	//+++++++++++++++++++++++++++++++++++++++++++
	//item indices:
	py::class_<NodeIndex>(m, "NodeIndex", "NodeIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &NodeIndex::index)
		.def("GetTypeString", &NodeIndex::GetTypeString,
			"get type string for identification in Python")
		.def("GetIndex", &NodeIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &NodeIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const NodeIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__index__", [](const NodeIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index; consistent with Python 3.8+")
		//add operators:
		.def(py::self + int())
		.def(int() + py::self)
		.def(py::self - int())
		.def(int() - py::self)
		.def(int() * py::self)
		.def(py::self * int())
		.def(-py::self)
		//representation:
		.def("__repr__", [](const NodeIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<ObjectIndex>(m, "ObjectIndex", "ObjectIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &ObjectIndex::index)
		.def("GetTypeString", &ObjectIndex::GetTypeString,
			"get type string for identification in Python")
		.def("GetIndex", &ObjectIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &ObjectIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const ObjectIndex &item) {
			return item.GetIndex();
			}, "return the integer representation of the index")
		.def("__index__", [](const ObjectIndex &item) {
				return item.GetIndex();
			}, "return the integer representation of the index; consistent with Python 3.8+")
		//add operators:
		.def(py::self + int())
		.def(int() + py::self)
		.def(py::self - int())
		.def(int() - py::self)
		.def(int() * py::self)
		.def(py::self * int())
		.def(-py::self)
		//representation:
		.def("__repr__", [](const ObjectIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
			;

	py::class_<MarkerIndex>(m, "MarkerIndex", "MarkerIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &MarkerIndex::index)
		.def("GetTypeString", &MarkerIndex::GetTypeString,
			"get type string for identification in Python")
		.def("GetIndex", &MarkerIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &MarkerIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const MarkerIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__index__", [](const MarkerIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index; consistent with Python 3.8+")
			//add operators:
		.def(py::self + int())
		.def(int() + py::self)
		.def(py::self - int())
		.def(int() - py::self)
		.def(int() * py::self)
		.def(py::self * int())
		.def(-py::self)
		//representation:
		.def("__repr__", [](const MarkerIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<LoadIndex>(m, "LoadIndex", "LoadIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &LoadIndex::index)
		.def("GetTypeString", &LoadIndex::GetTypeString,
			"get type string for identification in Python")
		.def("GetIndex", &LoadIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &LoadIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const LoadIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__index__", [](const LoadIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index; consistent with Python 3.8+")
			//add operators:
		.def(py::self + int())
		.def(int() + py::self)
		.def(py::self - int())
		.def(int() - py::self)
		.def(int() * py::self)
		.def(py::self * int())
		.def(-py::self)
		//representation:
		.def("__repr__", [](const LoadIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<SensorIndex>(m, "SensorIndex", "SensorIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &SensorIndex::index)
		.def("GetTypeString", &SensorIndex::GetTypeString,
			"get type string for identification in Python")
		.def("GetIndex", &SensorIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &SensorIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const SensorIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__index__", [](const SensorIndex &item) {
			return item.GetIndex();
		}, "return the integer representation of the index; consistent with Python 3.8+")
		//add operators:
		.def(py::self + int())
		.def(int() + py::self)
		.def(py::self - int())
		.def(int() - py::self)
		.def(int() * py::self)
		.def(py::self * int())
		.def(-py::self)
		//representation:
		.def("__repr__", [](const SensorIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//NOT needed, should always convert to std::vector<NodeIndex>
	//py::class_<ArrayNodeIndex>(m, "ArrayNodeIndex", "ArrayNodeIndex: array of indices which may only be used for nodes")
	//	.def(py::init<>())
	//	.def(py::init<std::vector<Index>>())
	//	.def(py::init<std::vector<NodeIndex>>())
	//	//+++++++++++++++++++++++++++++++++++++++++++
	//	.def("GetTypeString", &ArrayNodeIndex::GetTypeString,
	//		"get type string for identification in Python")
	//	.def("GetArrayIndex", &ArrayNodeIndex::GetArrayIndex,
	//		"get index converted to index / int")
	//	.def("SetArrayIndex", &ArrayNodeIndex::SetArrayIndex,
	//		"set index with index / int")
	//	//.def("__list__", [](const ArrayNodeIndex &item) {
	//	//	return item.GetArrayIndex();
	//	//}, "return the integer representation of the index array")
	//	.def("__repr__", [](const ArrayNodeIndex &item) {
	//		return STDstring(EXUstd::ToString(ArrayIndex(item.GetArrayIndex())));
	//	}, "return the string representation of the index array, which can be, e.g., printed")
	//	;


	//+++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++
	//MAINSYSTEMCONTAINER
	py::class_<MainSystemContainer>(m, "SystemContainer", "SystemContainer: Contains a set of (multibody) systems")
		.def(py::init<>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//System functions:
		.def("AddSystem", &MainSystemContainer::AddMainSystem, "add a new computational system", py::return_value_policy::reference)

		.def_property("visualizationSettings", &MainSystemContainer::PyGetVisualizationSettings, &MainSystemContainer::PySetVisualizationSettings)//, py::return_value_policy::reference)

		.def("GetRenderState", &MainSystemContainer::PyGetRenderState, "Get dictionary with current render state (openGL zoom, modelview, etc.)")

		.def("SetRenderState", &MainSystemContainer::PySetRenderState, "Set current render state (openGL zoom, modelview, etc.) with given dictionary; usually, this dictionary has been obtained with GetRenderState")

		.def("WaitForRenderEngineStopFlag", &MainSystemContainer::WaitForRenderEngineStopFlag, "Wait for user to stop render engine (CTRL+Q)")

		.def("RenderEngineZoomAll", &MainSystemContainer::PyZoomAll, "Send zoom all signal, which will perform zoom all at next redraw request")
		
		.def("RedrawAndSaveImage", &MainSystemContainer::RedrawAndSaveImage, "Redraw openGL scene and save image (command waits until process is finished)")

		.def("AttachToRenderEngine", &MainSystemContainer::AttachToRenderEngine, "Links the SystemContainer to the render engine, such that the changes in the graphics structure drawn upon updates, etc.; done automatically on creation of SystemContainer; return False, if no renderer exists (e.g., compiled without GLFW) or cannot be linked (if other SystemContainer already linked)")

		.def("DetachFromRenderEngine", &MainSystemContainer::DetachFromRenderEngine, "Releases the SystemContainer from the render engine; return True if successfully released, False if no GLFW available or detaching failed")

		.def("GetCurrentMouseCoordinates", &MainSystemContainer::PyGetCurrentMouseCoordinates, "Get current mouse coordinates as list [x, y]; x and y being floats, as returned by GLFW, measured from top left corner of window; use GetCurrentMouseCoordinates(True) to obtain OpenGLcoordinates of projected plane", py::arg("useOpenGLcoordinates") = true)

		.def("Reset", &MainSystemContainer::Reset, "delete all systems and reset SystemContainer (including graphics)") 

		.def("NumberOfSystems", [](const MainSystemContainer& msc) {return msc.GetMainSystems().NumberOfItems(); }, "get number of MainSystems")

		.def("GetSystem", &MainSystemContainer::GetMainSystem, "Get main system i from system container", py::return_value_policy::reference) //added reference options as otherwise system is copied


		.def("__repr__", [](const MainSystemContainer &item) {
			STDstring str = "SystemContainer:\n";

			for (Index i = 0; i < item.GetMainSystems().NumberOfItems(); i++)
			{
				str += "System " + EXUstd::ToString(i) + " <systemData:\n";
				str += item.GetMainSystems()[i]->GetMainSystemData().PyInfoSummary() + ">\n";
			}
			return str;
		}, "return the representation of the systemContainer, which can be, e.g., printed")

		;

//does not work, use internal exudyn version:
//#ifdef VERSION_INFO
//		#pragma message( "Using exudyn version " VERSION_INFO )
//		m.attr("__version__") = VERSION_INFO;
//#else
	m.attr("__version__") = EXUstd::exudynVersion;
//#endif
}





