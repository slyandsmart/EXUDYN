/** ***********************************************************************************************
* @brief        CObjectJointGeneric implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectJointGeneric.h"
#include "Utilities/RigidBodyMath.h"

#include <pybind11/stl.h> 
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h> //accept numpy arrays: numpy array automatically converted to std::vector<Real,...> ==> accepts np.array([1,0,0]) and [1,0,0] as return value!

//! find free roation axis for revolute joint
void CObjectJointGenericFreeRotAxis(const ArrayIndex& constrainedAxes, Index& freeAxis, Index& lockedAxis1, Index& lockedAxis2)
{
	if (constrainedAxes[3] == 0) { freeAxis = 0; lockedAxis1 = 1; lockedAxis2 = 2; }
	else if (constrainedAxes[4] == 0) { freeAxis = 1; lockedAxis1 = 0; lockedAxis2 = 2; }
	else if (constrainedAxes[5] == 0) { freeAxis = 2; lockedAxis1 = 0; lockedAxis2 = 1; }
	else
	{
		CHECKandTHROWstring("CObjectJointGeneric: CObjectJointGenericFreeRotAxis: illegal case");
	}
}

//! find locked roation axis for universal joint
void CObjectJointGenericLockedRotAxis(const ArrayIndex& constrainedAxes, Index& lockedAxis, Index& freeAxis1, Index& freeAxis2)
{
	if (constrainedAxes[3] == 1) { lockedAxis = 0; freeAxis1 = 1; freeAxis2 = 2; }
	else if (constrainedAxes[4] == 1) { lockedAxis = 1; freeAxis1 = 0; freeAxis2 = 2; }
	else if (constrainedAxes[5] == 1) { lockedAxis = 2; freeAxis1 = 0; freeAxis2 = 1; }
	else
	{
		CHECKandTHROWstring("CObjectJointGeneric: CObjectJointGenericLockedRotAxis: illegal case");
	}
}

//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectJointGeneric::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, Index itemIndex, bool velocityLevel) const
{
	if (parameters.activeConnector)
	{

		algebraicEquations.SetNumberOfItems(nConstraints);
		const LinkedDataVector& lambda = markerData.GetLagrangeMultipliers();

		const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
		const Matrix3D& A0off = parameters.rotationMarker0;
		Matrix3D A0all = (A0*A0off);

		if (!velocityLevel)
		{
			//consider offset function
			Vector6D userOffset(0.);
			if (parameters.offsetUserFunction) //from here on it becomes much slower ... (python)
			{
				EvaluateUserFunctionOffset(userOffset, cSystemData->GetMainSystemBacklink(), t, itemIndex);

				if (userOffset[3] != 0. || userOffset[4] != 0. || userOffset[5] != 0.)
				{
					Matrix3D A0offset = RigidBodyMath::RotXYZ2RotationMatrix(Vector3D({ userOffset[3], userOffset[4], userOffset[5] }));
					A0all = A0all*A0offset;
				}
			}

			//use difference of positions in local coordinates
			Vector3D vPos;
			if (parameters.constrainedAxes[0] == 1 && parameters.constrainedAxes[1] == 1 && parameters.constrainedAxes[2] == 1)
			{
				//global equations in case that all position coordinates are locked!
				vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); 
			}
			else
			{
				vPos = A0all.GetTransposed()*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //local equations (marker0-fixed)
			}


			//++++++++++++++++++++++++++++++++
			//translation constraints:
			for (Index i = 0; i < 3; i++)
			{
				if (parameters.constrainedAxes[i] == 1)
				{
					algebraicEquations[i] = vPos[i] - userOffset[i]; //these are the local constraints
				}
			}
			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			Index constrainedRotations = parameters.constrainedAxes[3] + parameters.constrainedAxes[4] + parameters.constrainedAxes[5];

			if (constrainedRotations > 0) //improve speed for spherical joint configuration
			{
				const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
				const Matrix3D& A1off = parameters.rotationMarker1;
				Matrix3D A1all = (A1*A1off);

				//three cases: 
				if (constrainedRotations == 3) //rigid joint (at least regarding rotations)
				{
					Vector3D vx0 = A0all.GetColumnVector<3>(0);
					//Vector3D vy0 = A0all.GetColumnVector<3>(1);
					Vector3D vz0 = A0all.GetColumnVector<3>(2);

					Vector3D vx1 = A1all.GetColumnVector<3>(0);
					Vector3D vy1 = A1all.GetColumnVector<3>(1);
					//Vector3D vz1 = A1all.GetColumnVector<3>(2);

					algebraicEquations[3] = vz0 * vy1;
					algebraicEquations[4] = vz0 * vx1;
					algebraicEquations[5] = vx0 * vy1;
				}
				else if (constrainedRotations == 2) //Revolute joint
				{
					Index freeAxis, lockedAxis1, lockedAxis2;
					CObjectJointGenericFreeRotAxis(parameters.constrainedAxes, freeAxis, lockedAxis1, lockedAxis2);


					Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
					Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
					Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

					//free axis leads to Lagrange multiplier; locked axes must be perpendicular to rotation axis of marker0 (vRot0)
					algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3];
					algebraicEquations[lockedAxis1 + 3] = vRot0 * vLocked1;
					algebraicEquations[lockedAxis2 + 3] = vRot0 * vLocked2;
				}
				else if (constrainedRotations == 1) //Universal joint
				{
					Index lockedAxis, freeAxis1, freeAxis2;
					CObjectJointGenericLockedRotAxis(parameters.constrainedAxes, lockedAxis, freeAxis1, freeAxis2);

					//free axes lead to Lagrange multipliers
					algebraicEquations[freeAxis1 + 3] = lambda[freeAxis1 + 3];
					algebraicEquations[freeAxis2 + 3] = lambda[freeAxis2 + 3];

					//Vector3D vRot0 = A0all.GetColumnVector<3>(lockedAxis);
					Vector3D vFree1 = A0all.GetColumnVector<3>(freeAxis1);
					Vector3D vFree2 = A1all.GetColumnVector<3>(freeAxis2);

					//there would be more possibilities!
					//here, the universal joint is chosen such that freeAxis1 is defined in marker0 and freeAxis2 is defined in marker1
					algebraicEquations[lockedAxis + 3] = vFree1 * vFree2;
				}
			}
			//add equations for unconstrained axes:
			for (Index i = 0; i < nConstraints; i++)
			{
				if (parameters.constrainedAxes[i] == 0) 
				{ 
					algebraicEquations[i] = lambda[i]; 
				}
			}

		}
		else //index2
		{
			//CHECKandTHROWstring("CObjectJointGeneric: velocity level not implemented");
			CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable, "CObjectJointGeneric::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

			if (parameters.offsetUserFunction_t) //from here on it becomes much slower ... (python)
			{
				PyError("ObjectJointGeneric: offsetUserFunction_t not implemented for velocity level constraints!");
			}
			
			Vector3D vVel;
			if (parameters.constrainedAxes[0] == 1 && parameters.constrainedAxes[1] == 1 && parameters.constrainedAxes[2] == 1)
			{
				//use difference of velocities in global coordinates
				vVel = (markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);
			}
			else
			{
				//use difference of velocities in local coordinates
				//the term A0all_t is not considered here as we assume that the positions are identical ...; may this cause problems in Newton or drift?
				vVel = A0all.GetTransposed()*(markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);
				//+ A0all_t.GetTransposed()*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); 
			}
			//++++++++++++++++++++++++++++++++
			//translation constraints:
			for (Index i = 0; i < 3; i++)
			{
				if (parameters.constrainedAxes[i] == 1)
				{
					algebraicEquations[i] = vVel[i]; //these are the local constraints
				}
			}

			//++++++++++++++++++++++++++++++++
			//rotation constraints:
			Index constrainedRotations = parameters.constrainedAxes[3] + parameters.constrainedAxes[4] + parameters.constrainedAxes[5];

			if (constrainedRotations > 0)
			{
				const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
				const Matrix3D& A1off = parameters.rotationMarker1;
				Matrix3D A1all = (A1*A1off);

				Matrix3D A0_t = markerData.GetMarkerData(0).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(0).angularVelocityLocal);
				Matrix3D A1_t = markerData.GetMarkerData(1).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(1).angularVelocityLocal);
				Matrix3D A0all_t = (A0_t*A0off);
				Matrix3D A1all_t = (A1_t*A1off);

				//three cases: 
				if (constrainedRotations == 3) //rigid joint (at least regarding rotations)
				{
					Vector3D vx0 = A0all.GetColumnVector<3>(0);
					Vector3D vy0 = A0all.GetColumnVector<3>(1);
					Vector3D vz0 = A0all.GetColumnVector<3>(2);

					Vector3D vx1 = A1all.GetColumnVector<3>(0);
					Vector3D vy1 = A1all.GetColumnVector<3>(1);
					Vector3D vz1 = A1all.GetColumnVector<3>(2);

					Vector3D vx0_t = A0all_t.GetColumnVector<3>(0);
					Vector3D vy0_t = A0all_t.GetColumnVector<3>(1);
					Vector3D vz0_t = A0all_t.GetColumnVector<3>(2);

					Vector3D vx1_t = A1all_t.GetColumnVector<3>(0);
					Vector3D vy1_t = A1all_t.GetColumnVector<3>(1);
					Vector3D vz1_t = A1all_t.GetColumnVector<3>(2);

					algebraicEquations[3] = vz0_t * vy1 + vz0 * vy1_t;
					algebraicEquations[4] = vz0_t * vx1 + vz0 * vx1_t;
					algebraicEquations[5] = vx0_t * vy1 + vx0 * vy1_t;

					////alternative approach:
					//Vector3D dOmega = A0all * markerData.GetMarkerData(0).angularVelocityLocal - A1all * markerData.GetMarkerData(1).angularVelocityLocal;
					//
					//algebraicEquations[3] = dOmega[0];
					//algebraicEquations[4] = dOmega[1];
					//algebraicEquations[5] = dOmega[2];
				}
				else if (constrainedRotations == 2) //Revolute joint
				{
					Index freeAxis, lockedAxis1, lockedAxis2;
					CObjectJointGenericFreeRotAxis(parameters.constrainedAxes, freeAxis, lockedAxis1, lockedAxis2);


					Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
					Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
					Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

					Vector3D vRot0_t = A0all_t.GetColumnVector<3>(freeAxis);
					Vector3D vLocked1_t = A1all_t.GetColumnVector<3>(lockedAxis1);
					Vector3D vLocked2_t = A1all_t.GetColumnVector<3>(lockedAxis2);

					//free axis leads to Lagrange multiplier; locked axes must be perpendicular to rotation axis of marker0 (vRot0)
					algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3];
					algebraicEquations[lockedAxis1 + 3] = vRot0_t * vLocked1 + vRot0 * vLocked1_t;
					algebraicEquations[lockedAxis2 + 3] = vRot0_t * vLocked2 + vRot0 * vLocked2_t;
				}
				else if (constrainedRotations == 1) //Universal joint
				{
					Index lockedAxis, freeAxis1, freeAxis2;
					CObjectJointGenericLockedRotAxis(parameters.constrainedAxes, lockedAxis, freeAxis1, freeAxis2);

					//free axes lead to Lagrange multipliers
					algebraicEquations[freeAxis1 + 3] = lambda[freeAxis1 + 3];
					algebraicEquations[freeAxis2 + 3] = lambda[freeAxis2 + 3];

					//Vector3D vRot0 = A0all.GetColumnVector<3>(lockedAxis);
					Vector3D vFree1 = A0all.GetColumnVector<3>(freeAxis1);
					Vector3D vFree2 = A1all.GetColumnVector<3>(freeAxis2);

					Vector3D vFree1_t = A0all_t.GetColumnVector<3>(freeAxis1);
					Vector3D vFree2_t = A1all_t.GetColumnVector<3>(freeAxis2);

					//there would be more possibilities!
					//here, the universal joint is chosen such that freeAxis1 is defined in marker0 and freeAxis2 is defined in marker1
					algebraicEquations[lockedAxis + 3] = vFree1_t * vFree2 + vFree1 * vFree2_t;
				}
			}
			//add equations for unconstrained translational axes:
			for (Index i = 0; i < 3; i++)
			{
				if (parameters.constrainedAxes[i] == 0) { algebraicEquations[i] = lambda[i]; }
			}


		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [lambda0,lambda1]^T = [0,0]^T, means that the current values need to be copied
	}
}


void CObjectJointGeneric::ComputeJacobianAE(ResizableMatrix& jacobian_ODE2, ResizableMatrix& jacobian_ODE2_t, ResizableMatrix& jacobian_ODE1, 
	ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData, Real t, Index itemIndex) const
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
		jacobian_ODE2.SetNumberOfRowsAndColumns(nConstraints, nColumnsJac0 + markerData.GetMarkerData(1).positionJacobian.NumberOfColumns());

		const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
		const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
		//const Matrix3D& A0off = EXUmath::unitMatrix3D; //parameters.rotationMarker0;
		//const Matrix3D& A1off = EXUmath::unitMatrix3D; //parameters.rotationMarker1;
		const Matrix3D& A0off = parameters.rotationMarker0;
		const Matrix3D& A1off = parameters.rotationMarker1;
		Matrix3D A0all = (A0*A0off);
		Matrix3D A1all = (A1*A1off);

		//include offsetUserFunction rotation
		if (parameters.offsetUserFunction) //from here on it becomes much slower ... (python)
		{
			Vector6D userOffset(0.);
			EvaluateUserFunctionOffset(userOffset, cSystemData->GetMainSystemBacklink(), t, itemIndex);

			if (userOffset[3] != 0. || userOffset[4] != 0. || userOffset[5] != 0.)
			{
				Matrix3D A0offset = RigidBodyMath::RotXYZ2RotationMatrix(Vector3D({ userOffset[3], userOffset[4], userOffset[5] }));
				A0all = A0all * A0offset;
			}
		}

		if (parameters.constrainedAxes[0] == 1 && parameters.constrainedAxes[1] == 1 && parameters.constrainedAxes[2] == 1)
		{
			//global equations
			const ResizableMatrix& posJac0 = markerData.GetMarkerData(0).positionJacobian;
			//vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //local equations (marker0-fixed)
			for (Index i = 0; i < nColumnsJac0; i++)
			{
				jacobian_ODE2(0, i) = -posJac0(0, i); //negative sign in marker0 because of eq: (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position)
				jacobian_ODE2(1, i) = -posJac0(1, i);
				jacobian_ODE2(2, i) = -posJac0(2, i);
			}
			const ResizableMatrix& posJac1 = markerData.GetMarkerData(1).positionJacobian;
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				jacobian_ODE2(0, i + nColumnsJac0) = posJac1(0, i);
				jacobian_ODE2(1, i + nColumnsJac0) = posJac1(1, i);
				jacobian_ODE2(2, i + nColumnsJac0) = posJac1(2, i);
			}
			//for (Index i = 0; i < nColumnsJac0; i++)
			//{
			//	jacobian_ODE2(0, i) = -markerData.GetMarkerData(0).positionJacobian(0, i); //negative sign in marker0 because of eq: (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position)
			//	jacobian_ODE2(1, i) = -markerData.GetMarkerData(0).positionJacobian(1, i);
			//	jacobian_ODE2(2, i) = -markerData.GetMarkerData(0).positionJacobian(2, i);
			//}
			//for (Index i = 0; i < nColumnsJac1; i++)
			//{
			//	jacobian_ODE2(0, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(0, i);
			//	jacobian_ODE2(1, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(1, i);
			//	jacobian_ODE2(2, i + nColumnsJac0) = markerData.GetMarkerData(1).positionJacobian(2, i);
			//}
		}
		else
		{
			Vector3D vPos = (markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position);
			Matrix3D A0allT = A0all.GetTransposed();
			Matrix3D vPosTilde = RigidBodyMath::Vector2SkewMatrix(vPos);

			//vPos = A0allT*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); //local equations (marker0-fixed)
			// A0allT_q0*vPos - A0allT * posJac0
			// A0allT * posJac1
			for (Index i = 0; i < nColumnsJac0; i++)
			{
				//A^T*A_t = omegaBarTilde, A_t*A^T=omegaTilde; A*A_t^T = -omegaTilde
				//(A*vBar)_theta = -A*uBarTilde*Gbar
				//A*A_t^T*u = -omegaTilde*u
				//A_t^T*u = -A^T*omegaTilde*u = A^T*uTilde*omega
				//(A^T*u)_theta = A^T*uTilde*G (positive sign!)
				Vector3D Gvec0 = markerData.GetMarkerData(0).rotationJacobian.GetColumnVector<3>(i);
				Vector3D jacPos0 = markerData.GetMarkerData(0).positionJacobian.GetColumnVector<3>(i);
				Vector3D v = A0allT * (vPosTilde*Gvec0 - jacPos0);  //A0allT*vPosTilde*Gvec0  - A0allT * jacPos0;
				if (parameters.constrainedAxes[0]) { jacobian_ODE2(0, i) = v[0]; }
				else { jacobian_ODE2(0, i) = 0; }
				if (parameters.constrainedAxes[1]) { jacobian_ODE2(1, i) = v[1]; }
				else { jacobian_ODE2(1, i) = 0; }
				if (parameters.constrainedAxes[2]) { jacobian_ODE2(2, i) = v[2]; }
				else { jacobian_ODE2(2, i) = 0; }
			}
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				Vector3D jacPos1 = markerData.GetMarkerData(1).positionJacobian.GetColumnVector<3>(i);
				Vector3D v = A0allT * jacPos1;
				if (parameters.constrainedAxes[0]) { jacobian_ODE2(0, i + nColumnsJac0) = v[0]; }
				else { jacobian_ODE2(0, i + nColumnsJac0) = 0; }
				if (parameters.constrainedAxes[1]) { jacobian_ODE2(1, i + nColumnsJac0) = v[1]; }
				else { jacobian_ODE2(1, i + nColumnsJac0) = 0; }
				if (parameters.constrainedAxes[2]) { jacobian_ODE2(2, i + nColumnsJac0) = v[2]; }
				else { jacobian_ODE2(2, i + nColumnsJac0) = 0; }
			}
		}

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		Index constraintRotations = parameters.constrainedAxes[3] + parameters.constrainedAxes[4] + parameters.constrainedAxes[5];
		if (constraintRotations == 3) //rigid joint (at least regarding rotations)
		{
			Vector3D vx0 = A0all.GetColumnVector<3>(0);
			//Vector3D vy0 = A0all.GetColumnVector<3>(1);
			Vector3D vz0 = A0all.GetColumnVector<3>(2);

			Vector3D vx1 = A1all.GetColumnVector<3>(0);
			Vector3D vy1 = A1all.GetColumnVector<3>(1);
			//Vector3D vz1 = A1all.GetColumnVector<3>(2);

			//algebraicEquations[3] = vz0 * vy1;
			//algebraicEquations[4] = vz0 * vx1;
			//algebraicEquations[5] = vx0 * vy1;

			for (Index i = 0; i < nColumnsJac0; i++)
			{
				Vector3D Gvec0 = -1.*markerData.GetMarkerData(0).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3, i) = vy1 * (RigidBodyMath::Vector2SkewMatrix(vz0) * Gvec0);
				jacobian_ODE2(4, i) = vx1 * (RigidBodyMath::Vector2SkewMatrix(vz0) * Gvec0);
				jacobian_ODE2(5, i) = vy1 * (RigidBodyMath::Vector2SkewMatrix(vx0) * Gvec0);
			}
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				Vector3D Gvec1 = -1.*markerData.GetMarkerData(1).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3, i + nColumnsJac0) = vz0 * (RigidBodyMath::Vector2SkewMatrix(vy1) * Gvec1);
				jacobian_ODE2(4, i + nColumnsJac0) = vz0 * (RigidBodyMath::Vector2SkewMatrix(vx1) * Gvec1);
				jacobian_ODE2(5, i + nColumnsJac0) = vx0 * (RigidBodyMath::Vector2SkewMatrix(vy1) * Gvec1);
			}
		}
		else if (constraintRotations == 2) //Revolute joint
		{
			Index freeAxis, lockedAxis1, lockedAxis2;
			CObjectJointGenericFreeRotAxis(parameters.constrainedAxes, freeAxis, lockedAxis1, lockedAxis2);

			//CHECKandTHROWstring("CObjectJointGeneric: revolute configuration (2 rotation locked) not implemented");

			Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
			Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
			Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

			//algebraicEquations[lockedAxis1 + 3] = vRot0 * vLocked1;
			//algebraicEquations[lockedAxis2 + 3] = vRot0 * vLocked2;

			//vRot0_t = omegaTilde * vRot0 = -vRot0tilde * omega ==> vRot0/dTheta = -vRot0tilde * G
			for (Index i = 0; i < nColumnsJac0; i++)
			{
				Vector3D Gvec0 = -1.*markerData.GetMarkerData(0).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3 + lockedAxis1, i) = vLocked1 * (RigidBodyMath::Vector2SkewMatrix(vRot0) * Gvec0);
				jacobian_ODE2(3 + lockedAxis2, i) = vLocked2 * (RigidBodyMath::Vector2SkewMatrix(vRot0) * Gvec0);
				jacobian_ODE2(3 + freeAxis, i) = 0;
			}
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				Vector3D Gvec1 = -1.*markerData.GetMarkerData(1).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3 + lockedAxis1, i + nColumnsJac0) = vRot0 * (RigidBodyMath::Vector2SkewMatrix(vLocked1) * Gvec1);
				jacobian_ODE2(3 + lockedAxis2, i + nColumnsJac0) = vRot0 * (RigidBodyMath::Vector2SkewMatrix(vLocked2) * Gvec1);
				jacobian_ODE2(3 + freeAxis, i + nColumnsJac0) = 0;
			}
			//Matrix3D A0_t = markerData.GetMarkerData(0).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(0).angularVelocityLocal);
			//Matrix3D A1_t = markerData.GetMarkerData(1).orientation * RigidBodyMath::Vector2SkewMatrix(markerData.GetMarkerData(1).angularVelocityLocal);
			//Vector3D vRot0 = A0all.GetColumnVector<3>(freeAxis);
			//Vector3D vLocked1 = A1all.GetColumnVector<3>(lockedAxis1);
			//Vector3D vLocked2 = A1all.GetColumnVector<3>(lockedAxis2);

			//Vector3D vRot0_t = A0all_t.GetColumnVector<3>(freeAxis);
			//Vector3D vLocked1_t = A1all_t.GetColumnVector<3>(lockedAxis1);
			//Vector3D vLocked2_t = A1all_t.GetColumnVector<3>(lockedAxis2);

			////free axis leads to Lagrange multiplier; locked axes must be perpendicular to rotation axis of marker0 (vRot0)
			//algebraicEquations[freeAxis + 3] = lambda[freeAxis + 3];
			//algebraicEquations[lockedAxis1 + 3] = vRot0_t * vLocked1 + vRot0 * vLocked1_t;
			//algebraicEquations[lockedAxis2 + 3] = vRot0_t * vLocked2 + vRot0 * vLocked2_t;

		}
		else if (constraintRotations == 1) //Universal joint
		{
			Index lockedAxis, freeAxis1, freeAxis2;
			CObjectJointGenericLockedRotAxis(parameters.constrainedAxes, lockedAxis, freeAxis1, freeAxis2);

			Vector3D vFree1 = A0all.GetColumnVector<3>(freeAxis1);
			Vector3D vFree2 = A1all.GetColumnVector<3>(freeAxis2);
			//algebraicEquations[lockedAxis + 3] = vFree1 * vFree2;

			for (Index i = 0; i < nColumnsJac0; i++)
			{
				Vector3D Gvec0 = -1.*markerData.GetMarkerData(0).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3 + lockedAxis, i) = vFree2 * (RigidBodyMath::Vector2SkewMatrix(vFree1) * Gvec0);
				jacobian_ODE2(3 + freeAxis1, i) = 0;
				jacobian_ODE2(3 + freeAxis2, i) = 0;
			}
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				Vector3D Gvec1 = -1.*markerData.GetMarkerData(1).rotationJacobian.GetColumnVector<3>(i);
				jacobian_ODE2(3 + lockedAxis, i + nColumnsJac0) = vFree1 * (RigidBodyMath::Vector2SkewMatrix(vFree2) * Gvec1);
				jacobian_ODE2(3 + freeAxis1, i + nColumnsJac0) = 0;
				jacobian_ODE2(3 + freeAxis2, i + nColumnsJac0) = 0;
			}
		}
		else //no rotations constrained
		{
			for (Index i = 0; i < nColumnsJac0; i++)
			{
				jacobian_ODE2(3, i) = 0;
				jacobian_ODE2(4, i) = 0;
				jacobian_ODE2(5, i) = 0;
			}
			for (Index i = 0; i < nColumnsJac1; i++)
			{
				jacobian_ODE2(3, i + nColumnsJac0) = 0;
				jacobian_ODE2(4, i + nColumnsJac0) = 0;
				jacobian_ODE2(5, i + nColumnsJac0) = 0;
			}
		}

		jacobian_ODE2_t.SetNumberOfRowsAndColumns(0, 0); //for safety? check that this cannot happen ...
		//jacobian_AE.SetNumberOfRowsAndColumns(0, 0);//for safety!
	}
	else
	{
		jacobian_AE.SetScalarMatrix(nConstraints, 1.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
	}
}

JacobianType::Type CObjectJointGeneric::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function + JacobianType::AE_AE + JacobianType::AE_AE_function); //all AE_AE terms needed, because of free axes
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}

//! provide according output variable in "value"
void CObjectJointGeneric::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Index itemIndex, Vector& value) const
{
	//const LinkedDataVector& lambda = markerData.GetLagrangeMultipliers();

	const Matrix3D& A0 = markerData.GetMarkerData(0).orientation;
	const Matrix3D& A0off = parameters.rotationMarker0;
	Matrix3D A0all = (A0*A0off);

	const Matrix3D& A1 = markerData.GetMarkerData(1).orientation;
	const Matrix3D& A1off = parameters.rotationMarker1;
	Matrix3D A1all = (A1*A1off);

	//use difference of positions in local joint J0 coordinates
	Vector3D vPosLocal = A0all.GetTransposed()*(markerData.GetMarkerData(1).position - markerData.GetMarkerData(0).position); 
	Vector3D vVelLocal = A0all.GetTransposed()*(markerData.GetMarkerData(1).velocity - markerData.GetMarkerData(0).velocity);

	Vector3D angVelLocal = A0all.GetTransposed()*(A1*markerData.GetMarkerData(1).angularVelocityLocal - A0 * markerData.GetMarkerData(0).angularVelocityLocal); //difference of global angular velocities, projected into J0

	//compute relative rotation, J0-fixed:
	Matrix3D relRot = A0all.GetTransposed() * A1all;
	Vector3D vLocRot = RigidBodyMath::RotationMatrix2RotXYZ(relRot);

	if (vLocRot[0] > EXUstd::pi) { vLocRot[0] -= 2.*EXUstd::pi; }
	if (vLocRot[1] > EXUstd::pi) { vLocRot[1] -= 2.*EXUstd::pi; }
	if (vLocRot[2] > EXUstd::pi) { vLocRot[2] -= 2.*EXUstd::pi; }

	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(markerData.GetMarkerData(0).position); break;
	case OutputVariableType::Velocity: value.CopyFrom(markerData.GetMarkerData(0).velocity); break;
	case OutputVariableType::DisplacementLocal: value.CopyFrom(vPosLocal); break;
	case OutputVariableType::Rotation: value.CopyFrom(vLocRot); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(vVelLocal); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(angVelLocal); break;
	case OutputVariableType::ForceLocal:
	{
		Vector3D force({ GetCurrentAEcoordinate(0), GetCurrentAEcoordinate(1), GetCurrentAEcoordinate(2) }); 
		if (parameters.constrainedAxes[0] == 1 && parameters.constrainedAxes[1] == 1 && parameters.constrainedAxes[2] == 1) //then force is global!
		{
			force = A0all.GetTransposed()*force;
		}
		value.CopyFrom(force);
		break;
	}
	case OutputVariableType::TorqueLocal:
	{
		Vector3D torque({ GetCurrentAEcoordinate(3), GetCurrentAEcoordinate(4), GetCurrentAEcoordinate(5) });
		value.CopyFrom(torque);
		break;
	}
	default:
		SysError("CObjectJointGeneric::GetOutputVariable failed"); //error should not occur, because types are checked!
	}

}



