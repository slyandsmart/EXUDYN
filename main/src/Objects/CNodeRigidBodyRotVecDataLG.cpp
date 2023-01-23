/** ***********************************************************************************************
* @brief		Implementation for NodeRigidBodyRotVecDataLG
*
* @author		Gerstmayr Johannes
* @date			2019-10-19 (generated)
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

#include "Main/CSystemData.h"
#include "Autogenerated/CNodeRigidBodyRotVecDataLG.h"
#include "Linalg/RigidBodyMath.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//autodiff for jacobian of rotationJacobian
#include "Utilities/AutomaticDifferentiation.h"

typedef EXUmath::AutoDiff<3, Real> DReal3;


// Notes SH:
// In the Newton method, TSO3 must be applied to the Jacobian matrix of the constraints as well as to the tangent stiffness matrix K, see Br�ls2010 (On the use of Lie group Time Integrators in Multibody Dynamics)
// The formula for TSO3 and TSO3^-1 is implemented in RigidBodyMath.h under the namespace EXUlie.

//! compute (G^T*v) for rotation vector (Glocal = I, G = RotationMatrix)
SlimVectorBase<DReal3, 3> RotationVectorGTvData(const ConstSizeVectorBase<DReal3, 3>& rot, const SlimVectorBase<Real, 3>& v)
	//const SlimVectorBase<DReal3, 3>& v)
{
	DReal3 angle = rot.GetL2Norm();
	//Real angle = sqrt(EXUstd::Square(rot[0].Value()) + EXUstd::Square(rot[1].Value()) + EXUstd::Square(rot[2].Value()));
	ConstSizeMatrixBase<DReal3, 9> mat;
	mat.SetScalarMatrix(3, 1.);

	if (angle.Value() == 0.) {
		return mat*v; //mat.T * v //could be more efficient by just returning zero vector, but rarely occuring ...
	}
	else
	{
		ConstSizeMatrixBase<DReal3, 9> vTilde(3, 3, {    0.,-rot[2], rot[1],
													 rot[2],      0,-rot[0],
													-rot[1], rot[0],      0 });
		DReal3 sAngle = sin(angle);
		mat += (sAngle / angle)*vTilde;

		DReal3 sAngle2 = sin(0.5*angle);
		mat += (2. * sAngle2 * sAngle2 / (angle * angle))*vTilde*vTilde;

		return v*mat; //mat.T * v
	}
}

//! compute d(G^T*v)/dq for rotation vector (Glocal = I, G = RotationMatrix)
//! using autodiff
Matrix3D CNodeRigidBodyRotVecDataLG::RotationVectorGTv_q(const CSVector4D& rotParameters, const Vector3D& v3D) // Maybe better RotationVectorGTvData_q() than RotationVectorGTvData()?
{
	ConstSizeVectorBase<DReal3, 3> rot;
	for (Index i = 0; i < 3; i++)
	{
		rot[i] = rotParameters[i];
		rot[i].DValue(i) = 1;

		//v[i] = v3D[i]; //derivatives are 0!
	}
	SlimVectorBase<DReal3, 3> diff = RotationVectorGTvData(rot, v3D);
	//SH: SlimVectorBase<DReal3, 3> diff = RotationVectorGTvData(EXUlie::CompositionRotationVector(vec0, Omega), v3D);

	Matrix3D jac(3,3);

	//now copy autodifferentiated result:
	for (Index i = 0; i < 3; i++)
	{
		for (Index j = 0; j < 3; j++)
		{
			jac(i, j) = diff[i].DValue(j);
		}
	}
	return jac;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! provide nodal values efficiently for rigid body computation
void CNodeRigidBodyRotVecDataLG::CollectCurrentNodeData1(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& Glocal, Vector3D& angularVelocityLocal) const
{
	//CHECKandTHROWstring("CNodeRigidBody::CollectCurrentNodeData1(...): not implemented");
	GetGlocal(Glocal);
	angularVelocityLocal = GetAngularVelocityLocal();
}

void CNodeRigidBodyRotVecDataLG::CollectCurrentNodeMarkerData(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& Glocal, ConstSizeMatrix<maxRotationCoordinates * nDim3D>& G,
	Vector3D& pos, Vector3D& vel, Matrix3D& A, Vector3D& angularVelocityLocal) const
{
	GetGlocal(Glocal);
	GetG(G);
	pos = GetPosition();
	vel = GetVelocity();
	A = GetRotationMatrix();
	angularVelocityLocal = GetAngularVelocityLocal();
}


// Compute vector to of 3 rotation parameters from reference and configuration coordinates
ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> CNodeRigidBodyRotVecDataLG::GetRotationParameters(ConfigurationType configuration) const
{
	LinkedDataVector coordinates = GetCoordinateVector(configuration);
	ConstSizeVector<maxRotationCoordinates> rot({ coordinates[nDisplacementCoordinates + 0], //these are the reference values
		coordinates[nDisplacementCoordinates + 1],
		coordinates[nDisplacementCoordinates + 2] });

	return rot;
}

// Link vector to time derivative of 3 rotation parameters in given configuration
LinkedDataVector CNodeRigidBodyRotVecDataLG::GetRotationParameters_t(ConfigurationType configuration) const
{
	return LinkedDataVector(GetCoordinateVector_t(configuration), nDisplacementCoordinates, nRotationCoordinates);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
void CNodeRigidBodyRotVecDataLG::GetG(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix.CopyFrom(GetRotationMatrix(configuration));
}

//! Compute local G matrix for given configuration
void CNodeRigidBodyRotVecDataLG::GetGlocal(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix.CopyFrom(EXUmath::unitMatrix3D); //velocity coordinates are identical to local angular velocity vector
}

//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
//not used so far! keep?
void CNodeRigidBodyRotVecDataLG::GetG_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	//G_t = A_t = A * skew(omegaBar)
	Matrix3D mat = RigidBodyMath::Vector2SkewMatrix(GetAngularVelocityLocal(configuration));
	mat = GetRotationMatrix(configuration) * mat;

	matrix.CopyFrom(mat);
}

//! Compute time derivative of local G matrix for given configuration
void CNodeRigidBodyRotVecDataLG::GetGlocal_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	//matrix.CopyFrom(Matrix3D(3, 3, 0.)); //zero matrix
	matrix.SetNumberOfRowsAndColumns(nRotationCoordinates, nRotationCoordinates);
	matrix.SetAll(0.); //zero matrix
}

//! compute d(G^T*v)/dq for rotation vector; needed for jacobians
void CNodeRigidBodyRotVecDataLG::GetGTv_q(const Vector3D& v, ConstSizeMatrix<maxRotationCoordinates * maxRotationCoordinates>& matrix,
	ConfigurationType configuration) const
{
	//matrix = CNodeRigidBodyRotVecDataLG::RotationVectorGTv_q(GetRotationParameters(configuration), v);
	matrix.CopyFrom(CNodeRigidBodyRotVecDataLG::RotationVectorGTv_q(GetRotationParameters(configuration), v)); //SH: Data Variable needed? --> See RotationVectorGTv_q()
}

//! compute d(G^T*v)/dq for rotation vector; needed for jacobians
void CNodeRigidBodyRotVecDataLG::GetGlocalTv_q(const Vector3D& v, ConstSizeMatrix<maxRotationCoordinates * maxRotationCoordinates>& matrix,
	ConfigurationType configuration) const
{
	matrix.SetNumberOfRowsAndColumns(nRotationCoordinates, nRotationCoordinates);
	matrix.SetAll(0.); //zero matrix
	//this leads to problems when adding: matrix.SetNumberOfRowsAndColumns(0,0); //indicates that this matrix is zero
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Vector3D CNodeRigidBodyRotVecDataLG::GetPosition(ConfigurationType configuration) const
{
	//compute reference coordinate vector (this is the position/orientation of the node without displacements)
	if (configuration != ConfigurationType::Reference) 
	{ 
		LinkedDataVector pRef3D = GetReferenceCoordinateVector();
		return Vector3D({ pRef3D[0], pRef3D[1], pRef3D[2] });
	}
	else
	{
		//compute displacement vector (this is the displacement/rotation superimposed to reference configuration)
		LinkedDataVector p3D = GetCoordinateVector(configuration); //calls according function based on data coordinates
		return Vector3D({ p3D[0], p3D[1], p3D[2] });
	}
}

Vector3D CNodeRigidBodyRotVecDataLG::GetVelocity(ConfigurationType configuration) const
{
	LinkedDataVector u3D_t = GetCoordinateVector_t(configuration);
	return Vector3D({ u3D_t[0], u3D_t[1], u3D_t[2] });
}

Matrix3D CNodeRigidBodyRotVecDataLG::GetRotationMatrix(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> rot(GetRotationParameters(configuration));
	return RigidBodyMath::RotationVector2RotationMatrix(rot);
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyRotVecDataLG::GetAngularVelocity(ConfigurationType configuration) const
{
	Vector3D omega;
	omega.CopyFrom(GetRotationParameters_t(configuration));
	omega = GetRotationMatrix(configuration) * omega;

	return omega;
}

//! return configuration dependent local (=body fixed) angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyRotVecDataLG::GetAngularVelocityLocal(ConfigurationType configuration) const
{
	Vector3D omegaLocal;
	omegaLocal.CopyFrom(GetRotationParameters_t(configuration));
	return omegaLocal;
}

//! AUTO:  provide position jacobian of node; derivative of 3D Position with respect to all coordinates
void CNodeRigidBodyRotVecDataLG::GetPositionJacobian(Matrix& value) const
{
	value.SetNumberOfRowsAndColumns(3, nDisplacementCoordinates + nRotationCoordinates);
	value.SetAll(0.);
	value(0, 0) = 1.;
	value(1, 1) = 1.;
	value(2, 2) = 1.;
}

//! AUTO:  provide 'rotation' jacobian \f$\Jm_R\f$ of node; derivative of 3D angular velocity vector with respect to all coordinates (zero-matrix + 'G-matrix'); action of torque \f$\mv\f$: \f$\Qm_m = \Jm_R^T \mv\f$
void CNodeRigidBodyRotVecDataLG::GetRotationJacobian(Matrix& value) const
{
	value.SetNumberOfRowsAndColumns(3, nDisplacementCoordinates + nRotationCoordinates);
	value.SetAll(0.);

	ConstSizeMatrix<3 * maxRotationCoordinates> G;
	GetG(G);

	for (Index i = 0; i < 3; i++) //dimensionality
	{
		for (Index j = 0; j < nRotationCoordinates; j++)
		{
			value(i, j + nDisplacementCoordinates) = G(i, j);
		}
	}
}

//! provide derivative w.r.t. coordinates of rotation Jacobian times vector; for current configuration
void CNodeRigidBodyRotVecDataLG::GetRotationJacobianTTimesVector_q(const Vector3D& vector, Matrix& jacobian_q) const
{
	//d(rot)/dq = [I_{3x3}, G]
	//d(rot^T*v)/dq = [0_{3x3}, 0_{3x3}]
	//                [0_{rx3}, GTv_rot]
	jacobian_q.SetNumberOfRowsAndColumns(nDisplacementCoordinates + nRotationCoordinates, nDisplacementCoordinates + nRotationCoordinates);
	jacobian_q.SetAll(0.);
	jacobian_q.SetSubmatrix(RotationVectorGTv_q(GetRotationParameters(), vector), //SH: Data Variable needed? --> See RotationVectorGTv_q() 
		nDisplacementCoordinates, nDisplacementCoordinates, 1.);
}

//! provide according output variable in "value"
void CNodeRigidBodyRotVecDataLG::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(configuration)); break;
	case OutputVariableType::Displacement: value.CopyFrom(GetPosition(configuration) - GetPosition(ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocity(configuration)); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(GetAngularVelocityLocal(configuration)); break;
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = GetRotationMatrix(configuration);
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::Rotation: {
		Matrix3D rotMat = GetRotationMatrix(configuration);
		Vector3D rot = RigidBodyMath::RotationMatrix2RotXYZ(rotMat);
		value.SetVector(3, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::Coordinates:
	{
		if (IsValidConfiguration(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Reference + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyRotVecDataLG::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_t:
	{
		if (IsValidConfigurationButNotReference(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector_t(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyRotVecDataLG::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodeRigidBodyRotVecDataLG::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

//! composition operation in R3xSO(3) with rotation vector as rotation parameters
void CNodeRigidBodyRotVecDataLG::CompositionRule(const LinkedDataVector& currentPosition, const LinkedDataVector& currentOrientation,
	const Vector6D& incrementalMotion,
	LinkedDataVector& newPosition, LinkedDataVector& newOrientation) const
{
	// incremental position/rotation
	LinkedDataVector incrementalPosition(incrementalMotion, 0, nDisplacementCoordinates);
	LinkedDataVector incrementalRotation(incrementalMotion, nDisplacementCoordinates, nRotationCoordinates);

	// position update: x0 + incPosVec
	newPosition = currentPosition; 
	newPosition += incrementalPosition;

	// rotation update (composition operaton for roitation vectors): rotVec0 o incRotVec 
	newOrientation = EXUlie::CompositionRotationVector((Vector3D)currentOrientation, (Vector3D)incrementalRotation);

	//no improvements for small increments:
	//Matrix3D A0 = RigidBodyMath::RotationVector2RotationMatrix(currentOrientation);
	//Matrix3D Ainc = RigidBodyMath::RotationVector2RotationMatrix(incrementalRotation);
	//newOrientation = RigidBodyMath::SkewMatrix2Vector(EXUlie::LogSO3(A0*Ainc));
}


#ifdef CONFIGSPACE_SE3_ROTVEC
//! composition operation in SE(3) with rotation vector as rotation parameters
//void CNodeRigidBodyRotVecDataLG::CompositionRule(const LinkedDataVector& currentPosition, const LinkedDataVector& currentOrientation, const Vector6D& incrementalMotion,
//	const LinkedDataVector& newPosition, const LinkedDataVector& newOrientation) const
void CNodeRigidBodyRotVecDataLG::CompositionRule(const LinkedDataVector& currentPosition, const LinkedDataVector& currentOrientation, const Vector6D& incrementalMotion,
	LinkedDataVector& newPosition, LinkedDataVector& newOrientation) const
{
	// incremental position/rotation
	Vector3D incrementalPosition = { incrementalMotion[0], incrementalMotion[1], incrementalMotion[2] };
	Vector3D incrementalRotation = { incrementalMotion[3], incrementalMotion[4], incrementalMotion[5] };

	// for testing with EOM in R3xSO(3)
	incrementalPosition = EXUlie::ExpSO3((Vector3D)currentOrientation).GetTransposed()*incrementalPosition;

	// position update: x = x0 + deltaX = x0 + R0*TexpSO3^T(incRotVec)*incPocVec; R0=expSO3(rotVec0)
	//newPosition.SetAll(0.); // needed?
	newPosition = EXUlie::ExpSO3((Vector3D)currentOrientation)*EXUlie::TExpSO3(incrementalRotation).GetTransposed()*incrementalPosition; // R0*TexpSO3^T(incRotVec)*incPocVec = deltaX
	newPosition += currentPosition; // deltaX + x0

	// rotation update (composition operaton for roitation vectors): rotVec0 o incRotVec 
	//newOrientation.SetAll(0.); // needed?
	newOrientation = (LinkedDataVector)EXUlie::CompositionRotationVector((Vector3D)currentOrientation, incrementalRotation);
}
#endif // CONFIGSPACE_SE3_ROTVEC

