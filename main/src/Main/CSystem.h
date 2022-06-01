/** ***********************************************************************************************
* @class        CSystem
* @brief		
* @details		Details:
 				- a computational system, containing data and functions to be solved with a solver
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
#ifndef CSYSTEM__H
#define CSYSTEM__H

#include <atomic>
#include "Main/CSystemData.h"
#include "Autogenerated/SimulationSettings.h" 
#include "Linalg/LinearSolver.h" //for GeneralMatrixEXUdense
#include "Linalg/MatrixContainer.h" //for ComputeMassMatrix, etc.

#include <functional> //! AUTO: needed for std::function

#include "Graphics/PostProcessData.h"
#include "Pymodules/PythonUserFunctions.h"
#include "Main/TemporaryComputationData.h"
#include "System/CContact.h"

class System;



//! @brief data which is updated during different computation tasks, e.g. load factor, load steps, time steps, solver accuracy, ...
class SolverData
{
public:
	Real loadFactor;			//!< load factor, mainly used for static computation; should be set to 1, otherwise all loads are reduced by this factor
	bool signalJacobianUpdate;  //!< false: default, do nothing; true: jacobian (of static computation or time integration) shall be updated, e.g. due to system change - if constraints are turned off, ...; this flag is regularly reset within a jacobian computation

	bool doPostNewtonIteration; //!< default=true; set false for explicit solvers to deactivate Post Newton step e.g. for contact, which directly compute contact conditions

	SolverData()
	{
		Reset();
	}

	//! reset all data for a new (static/dynamic) computation
	void Reset()
	{
		loadFactor = 1;
		signalJacobianUpdate = false;
		doPostNewtonIteration = true;
	}
};

//call to pointer to member function (std::invoke) did not work
////memberfunction pointers follow idea of https://isocpp.org/wiki/faq/pointers-to-members
//typedef  void (CSystem::*CSystemDifferentiableFunction)(TemporaryComputationData& temp, Vector& systemODE2Rhs);

class CSystem
{
protected:
    CSystemData cSystemData;			//!< contains all system-relevant data; Nodes, Objects, etc. 
	PostProcessData postProcessData;	//!< data needed for post-processing
	SolverData solverData;				//!< data updated by specific solvers 
	PythonUserFunctions pythonUserFunctions; //!< user functions and MainSystem
	//Index numberOfThreads;				//!< multithreading information set by solver

	bool systemIsConsistent;				//!< variable is set after check of system consistency ==> in order to draw or compute system; usually set after Assemble()
	ResizableArray<GeneralContact*> generalContacts;	//!< array of general contacts, that are not objects

public:
	~CSystem() 
	{ 
		ResetGeneralContacts(); 
	} //added for correct deletion of derived classes

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ACCESS FUNCTIONS for member variables
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	CSystemData& GetSystemData() { return cSystemData; }
	const CSystemData& GetSystemData() const { return cSystemData; }

	//! return pointer to data in order to be linked to visualization class
	PostProcessData* GetPostProcessData() { return &postProcessData; }
	//! return pointer to data in order to be linked to visualization class
	const PostProcessData* GetPostProcessData() const { return &postProcessData; }

	//! write access to encapsulated Python user functions (e.g., preStepUserFunction)
	PythonUserFunctions& GetPythonUserFunctions() { return pythonUserFunctions; }
	//! read access to encapsulated Python user functions (e.g., preStepUserFunction)
	const PythonUserFunctions& GetPythonUserFunctions() const { return pythonUserFunctions; }

	//! write access to general contacts
	ResizableArray<GeneralContact*>& GetGeneralContacts() { return generalContacts; }
	//! read access to general contacts
	const ResizableArray<GeneralContact*>& GetGeneralContacts() const { return generalContacts; }

	//! reset general contacts
	void ResetGeneralContacts()
	{
		for (GeneralContact* item : generalContacts)
		{
			delete item; //calls Reset() first
		}
		generalContacts.Flush();
	}

	//! this function is used to copy the current state to the visualization state and to send a signal that the PostProcessData has been updated
	void UpdatePostProcessData(bool recordImage = false);

	SolverData& GetSolverData() { return solverData; }
	const SolverData& GetSolverData() const { return solverData; }
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// CSystem management functions
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! return true, if system is assembled, initialized and ready to be computed; special draw functions can be applied (visualization state exists)
	bool IsSystemConsistent() const { return systemIsConsistent; }

	//! Set true, if system is assembled, initialized and ready to be computed; special draw functions can be applied (visualization state exists)
	void SetSystemIsConsistent(bool flag) 
	{ 
		systemIsConsistent = flag; 
		cSystemData.GetCData().SetSystemIsConsistent(flag);
		if (flag == false) { postProcessData.postProcessDataReady = flag; }	//do not draw system anymore
	}

	//merged with SetSystemIsConsistent //! Function called e.g. by AddNode/Object/..., ModifyNode/Object/... to signal that the system has changed and consistency is not guaranteed
	void SystemHasChanged() 
	{
		SetSystemIsConsistent(false);						//do not compute or access nodal dof lists, etc.
	}
	
	// reset system; everything done in MainSystem
	void Initialize() 
	{
		SetSystemIsConsistent(false);
		//numberOfThreads = 1; //per default, if not otherwise set by solver

		//postProcessData.postProcessDataReady = false;
		//postProcessData.simulationFinished = false;

		//postProcessData.updateCounter = 1; //done synchronized with visualizationSystem.graphicsData.visualizationCounter
	}

//	Index GetNumberOfThreads() const
//	{
//#ifdef USE_NGSOLVE_TASKMANAGER
//		return ngstd::TaskManager::GetNumThreads();
//#else
//		return 1;
//#endif
//	}

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // CSystem computational functions
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//void Test() { pout << "CSystem test function called\n"; }

	//! Check if all references are valid (body->node number, marker->body/nodenumber, load->marker, etc.) for assemble;
	//  Function is called before main assemble functions
	bool CheckSystemIntegrity(const MainSystem& mainSystem);

	//! Prepare a newly created System of nodes, objects, loads, ... for computation
	void Assemble(const MainSystem& mainSystem);

	//! assign coordinate numbers to nodes (?and prepare LinkedDataVectors (of coordinates) for nodes)
	void AssembleCoordinates(const MainSystem& mainSystem);

	//! build ltg-coordinate lists for object with 'objectIndex' and set indices in ltg lists
	void AssembleObjectLTGLists(Index objectIndex, ArrayIndex& ltgListODE2, ArrayIndex& ltgListODE1, 
		ArrayIndex& ltgListAE, ArrayIndex& ltgListData);

	//! build ltg-coordinate lists for objects (used to build global ODE2LHS, MassMatrix, etc. vectors and matrices)
	void AssembleLTGLists(const MainSystem& mainSystem);

	//! initialize some system data, e.g., generalContact objects (searchTree, etc.)
	void AssembleSystemInitialize(const MainSystem& mainSystem);

	//! precompute item lists (special lists for constraints, connectors, etc.)
	void PreComputeItemLists();

	////! NEEDED? prepare LinkedDataVectors for objects
	//void AssembleObjects();

	//! Use initial values of nodes to compute system-wide initial coordinate vectors
	void AssembleInitializeSystemCoordinates(const MainSystem& mainSystem);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // CSystem computation functions
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //! compute system massmatrix and ADD to given massMatrix; matrix needs to have according size; set entries to zero beforehand, if only the mass matrix is required
	void ComputeMassMatrix(TemporaryComputationData& temp, GeneralMatrix& massMatrix);
	//! run through all bodies and check if has constant mass matrix; used for solver
	bool HasConstantMassMatrix();
	//void ComputeMassMatrixOLD(TemporaryComputationData& temp, Matrix& massMatrix);

	//! compute left-hand-side (LHS) of second order ordinary differential equations (ODE) for every object (used in numerical differentiation and in LHS computation); return true, if object has localODE2Lhs, false otherwise
	bool ComputeObjectODE2LHS(TemporaryComputationData& temp, CObject* object, Vector& localODE2Lhs, Index objectNumber);
		
	//! compute right-hand-side (RHS) of first order ordinary differential equations (ODE) for every object (used in numerical differentiation and in LHS computation); return true, if object has localODE1Rhs, false otherwise
	bool ComputeObjectODE1RHS(TemporaryComputationData& temp, CObject* object, Vector& localODE1Lhs, Index objectNumber);

	//! compute system right-hand-side (RHS) of second order ordinary differential equations (ODE) to 'systemODE2Rhs' for ODE2 part
	void ComputeSystemODE2RHS(TemporaryComputationDataArray& tempArray, Vector& systemODE2Rhs);

	//! compute system right-hand-side (RHS) of first order ordinary differential equations (ODE) to 'systemODE1Rhs' for ODE1 part
	void ComputeSystemODE1RHS(TemporaryComputationData& temp, Vector& systemODE1Rhs);

	//! compute system right-hand-side (RHS) due to loads and add them to 'ode2rhs' for ODE2 part
	void ComputeODE2Loads(TemporaryComputationDataArray& tempArray, Vector& systemODE2Rhs);

	//! compute part of load for 'ode2rhs' or to sparsevector; if fillSparseVector, values are added to temp.sparseVector; otherwise, filled directly into systemODE2Rhs
	void ComputeODE2SingleLoad(Index loadIndex, TemporaryComputationData& temp, Real currentTime, Vector& systemODE2Rhs, bool fillSparseVector);

	//! compute system right-hand-side (RHS) due to loads and add them to 'ode2rhs' for ODE2 part
	void ComputeODE1Loads(TemporaryComputationData& temp, Vector& systemODE1Rhs);

	//! add the projected action of Lagrange multipliers (reaction forces) to the ODE2 coordinates and add it to the ode2ReactionForces residual:
	//! ode2ReactionForces += C_{q2}^T * \lambda
	void ComputeODE2ProjectedReactionForces(TemporaryComputationData& temp, const Vector& reactionForces, Vector& ode2ReactionForces);

	//! compute numerically the derivative of (C_{q2} * v), v being an arbitrary vector
	//! jacobianCqV = scalarFactor*d/dq2(C_{q2} * v)
	void ComputeConstraintJacobianDerivative(TemporaryComputationData& temp, const NumericalDifferentiationSettings& numDiff, Vector& f0, Vector& f1, 
		const Vector& v, GeneralMatrix& jacobianCqV, Real scalarFactor = 1., Index rowOffset = 0, Index columnOffset = 0);

	//! compute (C_{q2} * v), v being an arbitrary vector
	void ComputeConstraintJacobianTimesVector(TemporaryComputationData& temp, const Vector& v, Vector& result);

	//! PostNewtonStep: do this for every object (connector), which has a PostNewtonStep ->discontinuous iteration e.g. to resolve contact, friction or plasticity; returns an error (residual)
	//! recommended step size \f$h_{recom}\f$ after PostNewton(...): \f$h_{recom} < 0\f$: no recommendation, \f$h_{recom}==0\f$: use minimum step size, \f$h_{recom}>0\f$: use specific step size, if no smaller size requested by other reason
	Real PostNewtonStep(TemporaryComputationDataArray& tempArray, Real& recommendedStepSize);

	//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
	void PostDiscontinuousIterationStep();

	//! compute system right-hand-side (RHS) of algebraic equations (AE) to vector 'AERhs'
	void ComputeAlgebraicEquations(TemporaryComputationData& temp, Vector& algebraicEquations, bool velocityLevel = false);

	//directly called via cSystemData
	////! compute MarkerDataStructure for a given connector (using its markers); used in ComputeSystemODE2RHS, GetOutputVariableConnector, etc.
	//void ComputeMarkerDataStructure(const CObjectConnector* connector, bool computeJacobian, MarkerDataStructure& markerDataStructure) const
	//{ cSystemData.ComputeMarkerDataStructure(connector, computeJacobian, markerDataStructure); }

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Jacobians
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! compute numerical differentiation of ODE2RHS w.r.t. ODE2 and ODE2_t quantities; 
	//! multiply (before added to jacobianGM) ODE2 with factorODE2 and ODE2_t with factorODE2_t
	//! the jacobian is ADDed to jacobianGM, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	void JacobianODE2RHS(TemporaryComputationDataArray& tempArray, const NumericalDifferentiationSettings& numDiff,
		GeneralMatrix& jacobianGM, Real factorODE2 = 1., Real factorODE2_t = 0.);

	////! compute numerical differentiation of ODE2RHS; result is a jacobian;  multiply the added entries with scalarFactor
	////! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	//void NumericalJacobianODE2RHS(TemporaryComputationData& temp, const NumericalDifferentiationSettings& numDiff,
	//	Vector& f0, Vector& f1, GeneralMatrix& jacobianGM, Real scalarFactor = 1.); // ResizableMatrix& jacobian);

	////! compute numerical differentiation of ODE2RHS with respect to velocity coordinates; result is a jacobian; multiply the added entries with scalarFactor
	////! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	//void NumericalJacobianODE2RHS_t(TemporaryComputationData& temp, const NumericalDifferentiationSettings& numDiff,
	//	Vector& f0, Vector& f1, GeneralMatrix& jacobianGM, Real scalarFactor = 1.);

	//! compute numerical differentiation of ODE1RHS; result is a jacobian;  multiply the added entries with scalarFactor
	//! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	void NumericalJacobianODE1RHS(TemporaryComputationDataArray& tempArray, const NumericalDifferentiationSettings& numDiff,
		Vector& f0, Vector& f1, GeneralMatrix& jacobianGM, Real factorODE1 = 1., Real factorODE2 = 0., Real factorODE2_t = 0.);

																					//! numerical computation of constraint jacobian with respect to ODE2 and ODE1 (fillIntoSystemMatrix=true: also w.r.t. AE) coordinates
	//! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	//! factorODE2 is used to scale the ODE2-part of the jacobian (to avoid postmultiplication); 
	//! velocityLevel = velocityLevel constraints are used, if available; 
	//! always true: fillIntoSystemMatrix=true: fill in g_q_ODE2, g_q_ODE2^T AND g_q_AE into system matrix at according positions
	//! DEPRECATED: fillIntoSystemMatrix=false: fill in g_q_ODE2 into jacobian matrix at (0,0)
	template<class TGeneralMatrix>
	void NumericalJacobianAE(TemporaryComputationData& temp, const NumericalDifferentiationSettings& numDiff,
		Vector& f0, Vector& f1, TGeneralMatrix& jacobianGM, Real factorAE_ODE2, Real factorAE_ODE2_t, 
		bool velocityLevel = false, Real factorODE2_AE = 1., Real factorAE_AE = 1.);// , bool fillIntoSystemMatrix = false); //ResizableMatrix& jacobian

	////! compute numerical differentiation of AE with respect to ODE2 velocity coordinates; if fillIntoSystemMatrix==true, the jacobian is filled directly into the system matrix; result is a jacobian; THIS FUNCTION IS ONLY FOR COMPARISON (SLOW!!!)
	//void NumericalJacobianAE_ODE2_t(const NumericalDifferentiation& numDiff,
	//	TemporaryComputationData& temp, Vector& f0, Vector& f1, ResizableMatrix& jacobian, Real factor, bool velocityLevel = false);


	//! compute object-wise jacobian of ODE1RHS w.r.t. ODE1 coordinates
	//! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	//! The factor 'factor_ODE1' is used to scale the jacobian
	void JacobianODE1RHS(TemporaryComputationData& temp, const NumericalDifferentiationSettings& newton, Real factorODE1,
		ResizableMatrix& jacobian_ODE1) {}; //used in future!

	//!compute per-object jacobians for object j, providing TemporaryComputationData;
	//! the jacobian computed in according temp structure
	void ComputeObjectJacobianAE(Index j, TemporaryComputationData& temp,
		bool& objectUsesVelocityLevel, bool& flagAE_ODE2filled, bool& flagAE_ODE2_tFilled, bool& flagAE_ODE1filled, bool& flagAE_AEfilled);

	//implemented direcly in JacobianAE
	////!compute per-node jacobians for node j, providing TemporaryComputationData;
	////! the jacobian computed in according temp structure
	//void ComputeNodeJacobianAE(Index j, TemporaryComputationData& temp,
	//	bool& nodeUsesVelocityLevel, bool& flagAE_ODE2filled, bool& flagAE_ODE2_tFilled, bool& flagAE_ODE1filled, bool& flagAE_AEfilled);

	//! compute constraint jacobian of AE with respect to ODE2 (fillIntoSystemMatrix=true: also w.r.t. ODE1 and AE) coordinates
	//! the jacobian is ADDed to the given matrix, which needs to have according size; set entries to zero beforehand in order to obtain only the jacobian
	//! newton.useNumericalDifferentiation: if true, numerical differentiation is used throughout; otherwise: analytical/function jacobian when available
	//! factorAE_ODE2 is used to scale the ODE2-part of the jacobian, depending on index2 or index3 formulation (to avoid later multiplication at system level); 
	//! factorAE_ODE2_t is used to scale the ODE2_t-part of the jacobian for velocity level constraints (to avoid later multiplication at system level); 
	//! velocityLevel = velocityLevel constraints are used, if available; 
	//! ALWAYS TRUE: fillIntoSystemMatrix=true: fill in g_q_ODE2, g_q_ODE2^T AND g_q_AE into system matrix at according positions
	//! DEPRECATED: fillIntoSystemMatrix=false: fill in g_q_ODE2 into jacobian matrix at (0,0)
	void JacobianAE(TemporaryComputationData& temp, const NewtonSettings& newton, GeneralMatrix& jacobianGM,
		Real factorAE_ODE2, Real factorAE_ODE2_t, bool velocityLevel = false, Real factorODE2_AE = 1., Real factorAE_AE=1.);// , bool fillIntoSystemMatrix = false);
	//template<class TGeneralMatrix>
	//void JacobianAE(TemporaryComputationData& temp, const Newton& newton, TGeneralMatrix& jacobianGM,
	//	Real factorAE_ODE2, Real factorAE_ODE2_t, bool velocityLevel = false, bool fillIntoSystemMatrix = false);


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Various functions
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


};


#endif
