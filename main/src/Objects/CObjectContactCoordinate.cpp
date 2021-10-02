/** ***********************************************************************************************
* @brief        Implementation of CObjectContactCoordinate
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CNodeGenericData.h"
#include "Autogenerated/CObjectContactCoordinate.h"


//! compute gap for given configuration (current, start of step, ...); gap <= 0 means contact, gap > 0 is no contact
Real CObjectContactCoordinate::ComputeGap(const MarkerDataStructure& markerData) const
{
	return (markerData.GetMarkerData(1).vectorValue[0] - markerData.GetMarkerData(0).vectorValue[0] - parameters.offset);
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
//  MODEL: f
void CObjectContactCoordinate::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData, Index objectNumber) const
{
	CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
		"CObjectContactCoordinate::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

	//gap>0: no contact, gap<0: contact
	//Real gap = (markerData.GetMarkerData(1).value - markerData.GetMarkerData(0).value - parameters.offset);
	Real gap = ComputeGap(markerData);

	//velocity in dynamic computation:
	Real gap_t = (markerData.GetMarkerData(1).vectorValue_t[0] - markerData.GetMarkerData(0).vectorValue_t[0]);

	//if (gap_t != 0.) { pout << "error: gap_t=" << gap_t << "\n"; }

	//decision upon contact is not gap, but the dataVariable ==> "active set strategy", needed for Newton solver to converge
	Real hasContact = 0; //1 for contact, 0 else
	if (GetCNode(0)->GetCurrentCoordinate(0) <= 0) { hasContact = 1; }; //this is the contact state: 1=contact/use contact force, 0=no contact

	//as gap is negative in case of contact, the force needs to act in opposite direction
	Real fContact = hasContact*(gap * parameters.contactStiffness + gap_t * parameters.contactDamping);

	////link separate vectors to result (ode2Lhs) vector
	//ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
	//ode2Lhs.SetAll(0.);

	Vector1D fVec({ fContact });
	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).jacobian.NumberOfColumns() + markerData.GetMarkerData(1).jacobian.NumberOfColumns());
	ode2Lhs.SetAll(0.);

	//now link ode2Lhs Vector to partial result using the two jacobians
	if (markerData.GetMarkerData(1).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
	{
		LinkedDataVector ldv1(ode2Lhs, markerData.GetMarkerData(0).jacobian.NumberOfColumns(), markerData.GetMarkerData(1).jacobian.NumberOfColumns());

		//positive force on marker1
		EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).jacobian, fVec, ldv1);
	}

	if (markerData.GetMarkerData(0).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
	{
		LinkedDataVector ldv0(ode2Lhs, 0, markerData.GetMarkerData(0).jacobian.NumberOfColumns());

		fVec *= -1; //negative force on marker0
		EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).jacobian, fVec, ldv0);
	}

}


//! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
OutputVariableType CObjectContactCoordinate::GetOutputVariableTypes() const
{
	return OutputVariableType::Distance;
}

//! provide according output variable in "value"
void CObjectContactCoordinate::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	SysError("CObjectContactCoordinate::GetOutputVariableConnector not implemented");
}


//! function called after Newton method; returns a residual error (force); 
//! done for two different computation states in order to estimate the correct time of contact
Real CObjectContactCoordinate::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, Index itemIndex, PostNewtonFlags::Type& flags, Real& recommendedStepSize)
{
	//return force-type error in case of contact: in case that the assumed contact state has been wrong, 
	//  the contact force (also negative) is returned as measure of the error
	Real discontinuousError = 0;
	flags = PostNewtonFlags::_None;

	Real startofStepState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::StartOfStep)[0];	//state0
	Real& currentState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current)[0];			//state1

	
	//for adaptive step: Real startOfStepGap = ComputeGap(markerDataStartOfStep);	//gap0 ==> not computed!
	Real currentGap = ComputeGap(markerDataCurrent);			//gap1

	//possible situations (state: C=contact, N=no contact); k=parameters.contactStiffness:
	//  assumption (because of convergence): gap0 and state0 are always consistent
	//state0	state1	gap1	action		error	
	//N			N		>0		no			0		
	//N			N		<=0		state1=C	|gap1*k|
	//N			C		>		state1=N	|gap1*k|
	//N			C		<=0		no			0
	//C			N		>0		no			0
	//C			N		<=0		state1=C	|gap1*k|
	//C			C		>		state1=N	|gap1*k|
	//C			C		<=		no			0

	//delete: Real previousState = currentState;
	Real vGap = markerDataCurrent.GetMarkerData(1).vectorValue_t[0] - markerDataCurrent.GetMarkerData(0).vectorValue_t[0];

	if ((currentGap > 0 && startofStepState <= 0) || (currentGap <= 0 && startofStepState > 0)) //action: state1=currentGapState, error = |currentGap*k|
	{
		discontinuousError = fabs(currentGap * parameters.contactStiffness);
		if (vGap != 0) { recommendedStepSize = fabs(startofStepState / vGap); } //in fact it is (0-startofStepState) which is the part to go in this steps!

		flags = PostNewtonFlags::UpdateJacobian;
	}
	//pout << "m1=" << markerDataCurrent.GetMarkerData(1).vectorValue[0] << ", m0=" << markerDataCurrent.GetMarkerData(0).vectorValue[0] << ", o=" << parameters.offset << ", ";
	//pout << "curGap=" << currentGap << ", startOfStep=" << startofStepState << ", currentState=" << currentState << ", ";
	//pout << "vGap   =" << vGap << ", ";
	//pout << "recStep=" << recommendedStepSize << "\n";


	currentState = currentGap;

	//pout << "PNS: currentGap=" << currentGap << ", previousState=" << previousState << ", currentState=" << currentState << ", error=" << discontinuousError << "\n";
	return discontinuousError;
}

//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
void CObjectContactCoordinate::PostDiscontinuousIterationStep() 
{

}

