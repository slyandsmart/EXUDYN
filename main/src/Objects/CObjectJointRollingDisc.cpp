/** ***********************************************************************************************
* @brief        CObjectJointRollingDisc implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectJointRollingDisc.h"
#include "Utilities/RigidBodyMath.h"

#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h> //accept numpy arrays: numpy array automatically converted to std::vector<Real,...> ==> accepts np.array([1,0,0]) and [1,0,0] as return value!

//for consistency checks:
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectJointRollingDisc.h"

//Vector3D vPlaneNormal = Vector3D({ 0,0,1 }); //plane normal direction; cannot be put into .h file class

bool MainObjectJointRollingDisc::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectJointRollingDisc* cObject = (CObjectJointRollingDisc*)GetCObject();

	if (cObject->GetParameters().discRadius <= 0)
	{
		errorString = "CObjectJointRollingDisc: discRadius must be > 0";
		return false;
	}
	return true;
}

//helper function for ComputeAlgebraicEquations and for GetOutputVariableConnector
//returns contact position and velocity
void ComputeContactPoint(const Vector3D& p1, const Matrix3D& A1, const Vector3D& v1, const Vector3D& omega1,
	const Vector3D& planeNormal, Real discRadius, Vector3D& pC, Vector3D& vC, Vector3D& w2, Vector3D& w3)
{
	Vector3D vAxisLocal({ 1,0,0 });
	//Vector3D pB({ p1[0], p1[1], 0 }); //disc center point projected to ground

	Vector3D w1 = A1 * vAxisLocal;
	w2 = w1.CrossProduct(planeNormal); //longitudinal direction; cross product has length != 1
	w2.Normalize();
	w3 = w1.CrossProduct(w2); //w1 and w2 are perpendicular ==> w3 already normalized

	//compute contact point position and velocity:
	pC = p1 + discRadius*w3;
	vC = v1 + omega1.CrossProduct(discRadius*w3);

}

//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectJointRollingDisc::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, bool velocityLevel) const
{
	if (parameters.activeConnector)
	{
		CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable, "CObjectJointRollingDisc::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

		algebraicEquations.SetNumberOfItems(nConstraints);
		LinkedDataVector lambda = markerData.GetLagrangeMultipliers();

		const Vector3D& p1 = markerData.GetMarkerData(1).position;
		const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
		const Vector3D& v1 = markerData.GetMarkerData(1).velocity;
		Vector3D omega1 = A1*markerData.GetMarkerData(1).angularVelocityLocal;
		const Vector3D& p0 = markerData.GetMarkerData(0).position; //use already z-position ..., but no transformation

		Vector3D constraintVec; //will contain constraint violation in the end

		Vector3D pC, vC, w2, w3; //all vectors in global coordinates; therefore, lambda is also in global coordinates for now
		ComputeContactPoint(p1, A1, v1, omega1, parameters.planeNormal, parameters.discRadius, pC, vC, w2, w3);
		//pout << "++++++++++++++\n";
		//pout << " p1=" << p1 << "\n";
		//pout << " v1=" << v1 << "\n";
		//pout << " pC=" << pC << "\n";
		//pout << " vC=" << vC << "\n";
		//pout << " w2=" << w2 << "\n";
		//pout << " w3=" << w3 << "\n";

		constraintVec = vC;
		//object always uses velocity level for all components! ==> otherwise, splitting of jacobian and jacobian_t would be necessary for CqT action!
		//if (!velocityLevel) //only for normal direction
		//{
		//	constraintVec[2] = pC[2] - p0[2];
		//}

		//++++++++++++++++++++++++++++++++
		//activate single constraints
		for (Index i = 0; i < nConstraints; i++)
		{
			if (parameters.constrainedAxes[i] == 1)
			{
				algebraicEquations[i] = constraintVec[i]; //these are the local constraints
			}
			else
			{
				algebraicEquations[i] = lambda[i]; //these are the local constraints
			}
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [lambda0,lambda1]^T = [0,0]^T, means that the current values need to be copied
	}
}


void CObjectJointRollingDisc::ComputeJacobianAE(ResizableMatrix& jacobian, ResizableMatrix& jacobian_t, ResizableMatrix& jacobian_AE, 
	const MarkerDataStructure& markerData, Real t) const
{
	if (parameters.activeConnector)
	{
		//for free axes, set jacobian_AE diagonal term to 1
		jacobian_AE.SetScalarMatrix(nConstraints, 0.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
		for (Index i = 0; i < nConstraints; i++)
		{
			if (parameters.constrainedAxes[i] == 0) { jacobian_AE(i,i) = 1.; }
		}

		//markerData contains already the correct jacobians ==> transformed to constraint jacobian
		Index nColumnsJac0 = markerData.GetMarkerData(0).positionJacobian.NumberOfColumns();
		Index nColumnsJac1 = markerData.GetMarkerData(1).positionJacobian.NumberOfColumns();
		//jacobian.SetNumberOfRowsAndColumns(nConstraints, nColumnsJac0 + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
		//jacobian.SetAll(0);
		jacobian_t.SetNumberOfRowsAndColumns(nConstraints, nColumnsJac0 + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());
		jacobian_t.SetAll(0);

		const Vector3D& p1 = markerData.GetMarkerData(1).position;
		const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
		const Vector3D& v1 = markerData.GetMarkerData(1).velocity;
		Vector3D omega1 = A1 * markerData.GetMarkerData(1).angularVelocityLocal;
		const Vector3D& p0 = markerData.GetMarkerData(0).position; //use already z-position ..., but no transformation

		Vector3D constraintVec; //will contain constraint violation in the end

		Vector3D pC, vC, w2, w3;
		ComputeContactPoint(p1, A1, v1, omega1, parameters.planeNormal, parameters.discRadius, pC, vC, w2, w3);
		Vector3D w3radius = parameters.discRadius * w3;

		//global equations
		for (Index i = 0; i < nColumnsJac0; i++)
		{
			//currently, no action on marker 0 body (must be computed via rigid body motion)
			jacobian_t(0, i) = 0; // -markerData.GetMarkerData(0).positionJacobian(0, i); //negative sign in marker0 because of eq: (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position)
			jacobian_t(1, i) = 0; // -markerData.GetMarkerData(0).positionJacobian(1, i);
			jacobian_t(2, i) = 0; // -markerData.GetMarkerData(0).positionJacobian(2, i);
		}
		for (Index i = 0; i < nColumnsJac1; i++)
		{
			Vector3D vJacPosI = markerData.GetMarkerData(1).positionJacobian.GetColumnVector<3>(i);
			Vector3D vJacRotI = markerData.GetMarkerData(1).rotationJacobian.GetColumnVector<3>(i);
			vJacPosI += vJacRotI.CrossProduct(w3radius);
			for (Index j = 0; j < nConstraints; j++)
			{
				if (parameters.constrainedAxes[j] == 1)
				{
					jacobian_t(j, i + nColumnsJac0) = vJacPosI[j];
				}
				else
				{
					jacobian_t(j, i + nColumnsJac0) = 0;
				}
			}
		}

		//pout << "jac=" << jacobian_t << "\n";

		jacobian.SetNumberOfRowsAndColumns(0, 0); //for safety? check that this cannot happen ...
	}
	else
	{
		jacobian_AE.SetScalarMatrix(nConstraints, 1.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
	}
}

JacobianType::Type CObjectJointRollingDisc::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		//has only derivatives w.r.t. velocities:
		return (JacobianType::Type)(JacobianType::AE_ODE2_t + JacobianType::AE_ODE2_t_function + JacobianType::AE_AE + JacobianType::AE_AE_function); //all AE_AE terms needed, because of free axes
		//return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function + JacobianType::AE_ODE2_t + JacobianType::AE_ODE2_t_function + JacobianType::AE_AE + JacobianType::AE_AE_function); //all AE_AE terms needed, because of free axes
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}

//! provide according output variable in "value"
void CObjectJointRollingDisc::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	LinkedDataVector lambda = markerData.GetLagrangeMultipliers();

	const Vector3D& p1 = markerData.GetMarkerData(1).position;
	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Vector3D& v1 = markerData.GetMarkerData(1).velocity;
	Vector3D omega1 = A1 * markerData.GetMarkerData(1).angularVelocityLocal;
	const Vector3D& p0 = markerData.GetMarkerData(0).position; //use already z-position ..., but no transformation

	Vector3D constraintVec; //will contain constraint violation in the end

	Vector3D pC, vC, w2, w3;
	ComputeContactPoint(p1, A1, v1, omega1, parameters.planeNormal, parameters.discRadius, pC, vC, w2, w3);
	Vector3D wLateral = parameters.planeNormal.CrossProduct(w2);

	Vector3D vCG; //velocity of the trail, in joint coordinates; computed by neglegting the rotation of the wheel for the contact point
	Real r = parameters.discRadius;

	//compute trail velocity: point projected from pC to wheel axis:
	//Real cosAlpha = p1[2] / r;
	//Real lP = r / cosAlpha; //=r^2/p1[2]
	////Real cosAlpha = -w3 * parameters.planeNormal; //alternative, without using z-coordinate
	//if (cosAlpha > 1) { cosAlpha = 1.; }
	//if (cosAlpha < -1) { cosAlpha = -1.; }
	//Real alpha = acos(cosAlpha);
	//Real axisOffsetX = r * tan(alpha); //this is the x-coordinate of the point on the disc axis exactly above trail point
	//Real axisOffsetX_t = r * (1 + tan(alpha)*tan(alpha))*(omega1*w2); //this is the point on the disc axis exactly above trail point

	//compute trail velocity: point projected from pC to wheel axis:
	//Real cosAlpha = p1[2] / r;
	//REWRITE IN TERMS OF SIN(...) !!! more stable
	Real cosAlpha = -w3 * parameters.planeNormal; //alternative, without using z-coordinate; more accurate because z-drift
	Real sign = 1.;
	if (w3*wLateral > 0) { sign = -1.; }
	//Real lP = r / cosAlpha; //=r^2/p1[2]
	if (cosAlpha*cosAlpha > 1) { cosAlpha = EXUstd::Sgn(cosAlpha); }
	Real axisOffsetX = sign*r * sqrt(1. - cosAlpha * cosAlpha) / cosAlpha;
	Real cosAlpha_t = v1[2] / r;
	Real axisOffsetX_t = 0;
	if (1. - cosAlpha * cosAlpha>1e-15 && fabs(cosAlpha_t) > 1e-8)
	{
		axisOffsetX_t = sign * r * cosAlpha_t / EXUstd::Square(cosAlpha) *(-1. / sqrt(1. - cosAlpha * cosAlpha) * cosAlpha * cosAlpha - sqrt(1. - cosAlpha * cosAlpha));
	}

	vCG = v1 - omega1.CrossProduct(A1*Vector3D({ axisOffsetX,0,0 })) - A1*Vector3D({ axisOffsetX_t,0,0 });
	vCG[2] = 0; //z-velocity

	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(pC); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(vCG); break;
	case OutputVariableType::ForceLocal:
	{
		Vector3D lambda3D({ -lambda[0], -lambda[1], -lambda[2]}); //negative sign, because of algebraic equations definition; lambda should be force acting on disc (marker1)
		Vector3D force({ lambda3D*wLateral, lambda3D*w2, lambda3D*parameters.planeNormal }); //project forces into local coordinates
		value.CopyFrom(force);
		break;
	}
	default:
		SysError("CObjectJointRollingDisc::GetOutputVariable failed"); //error should not occur, because types are checked!
	}

}



