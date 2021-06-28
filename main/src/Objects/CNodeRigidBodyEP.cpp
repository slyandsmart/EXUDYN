/** ***********************************************************************************************
* @brief		Implementation for NodeRigidBodyEP
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
#include "Autogenerated/CNodeRigidBodyEP.h"
#include "Utilities/RigidBodyMath.h"


// Compute vector to of 4 Euler Parameters from reference and configuration coordinates
ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> CNodeRigidBodyEP::GetRotationParameters(ConfigurationType configuration) const
{
	LinkedDataVector refCoordinates = GetReferenceCoordinateVector();
	
	ConstSizeVector<maxRotationCoordinates> ep({ refCoordinates[nDisplacementCoordinates + 0], //these are the reference values
		refCoordinates[nDisplacementCoordinates + 1],
		refCoordinates[nDisplacementCoordinates + 2],
		refCoordinates[nDisplacementCoordinates + 3] }); //reference values for Euler Parameters

	if (configuration != ConfigurationType::Reference)
	{
		LinkedDataVector coordinates = GetCoordinateVector(configuration);
		ep[0] += coordinates[nDisplacementCoordinates + 0];
		ep[1] += coordinates[nDisplacementCoordinates + 1];
		ep[2] += coordinates[nDisplacementCoordinates + 2];
		ep[3] += coordinates[nDisplacementCoordinates + 3];
	}

	return ep;
}

// Link vector to time derivative of 4 Euler Parameters in given configuration
LinkedDataVector CNodeRigidBodyEP::GetRotationParameters_t(ConfigurationType configuration) const
{
	return LinkedDataVector(GetCoordinateVector_t(configuration), nDisplacementCoordinates, nRotationCoordinates);
	//return ConstSizeVector<CNodeRigidBody::maxRotationCoordinates>({
	//	ep_t[nDisplacementCoordinates + 0], 
	//	ep_t[nDisplacementCoordinates + 1],
	//	ep_t[nDisplacementCoordinates + 2],
	//	ep_t[nDisplacementCoordinates + 3] }); 
}

//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
void CNodeRigidBodyEP::GetG(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::EP2G(GetRotationParameters(configuration));
}

//! Compute local G matrix for given configuration
void CNodeRigidBodyEP::GetGlocal(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::EP2Glocal(GetRotationParameters(configuration));
}

//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
void CNodeRigidBodyEP::GetG_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::EP_t2G_t(GetRotationParameters_t(configuration));
}

//! Compute local G matrix for given configuration
void CNodeRigidBodyEP::GetGlocal_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::EP_t2Glocal_t(GetRotationParameters_t(configuration));
}


Vector3D CNodeRigidBodyEP::GetPosition(ConfigurationType configuration) const
{
	//compute reference coordinate vector (this is the position/orientation of the node without displacements)
	LinkedDataVector pRef3D = GetReferenceCoordinateVector();
	Vector3D pRef({ pRef3D[0], pRef3D[1], pRef3D[2] });
	if (configuration == ConfigurationType::Reference) { return pRef; }

	//compute displacement vector (this is the displacement/rotation superimposed to reference configuration)
	LinkedDataVector u3D = GetCoordinateVector(configuration);
	Vector3D displacement({ u3D[0], u3D[1], u3D[2] });
	
	return pRef + displacement; //valid, because position and displacement is still additive!
}

Vector3D CNodeRigidBodyEP::GetVelocity(ConfigurationType configuration) const
{
	LinkedDataVector u3D_t = GetCoordinateVector_t(configuration);
	return Vector3D({ u3D_t[0], u3D_t[1], u3D_t[2] });
}

Vector3D CNodeRigidBodyEP::GetAcceleration(ConfigurationType configuration) const
{
	LinkedDataVector u3D_tt = GetCoordinateVector_tt(configuration);
	return Vector3D({ u3D_tt[0], u3D_tt[1], u3D_tt[2] });
}

Matrix3D CNodeRigidBodyEP::GetRotationMatrix(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> ep(GetRotationParameters(configuration));
	return RigidBodyMath::EP2RotationMatrix(ep);
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyEP::GetAngularVelocity(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> ep(GetRotationParameters(configuration));
	LinkedDataVector ep_t(GetRotationParameters_t(configuration));
	//ConstSizeMatrix<nDim3D * nRotationCoordinates> G = RigidBodyMath::EP2G(ep);

	Vector3D omega;
	EXUmath::MultMatrixVector(RigidBodyMath::EP2G(ep), ep_t, omega);

	return omega;
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyEP::GetAngularAcceleration(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> ep(GetRotationParameters(configuration));
	//LinkedDataVector ep_t(GetRotationParameters_t(configuration));
	LinkedDataVector ep_tt(GetCoordinateVector_tt(configuration),nDisplacementCoordinates,nRotationCoordinates);

	//Vector3D omega;
	//EXUmath::MultMatrixVector(RigidBodyMath::EP2G(ep), ep_t, omega);
	Vector3D alpha;
	EXUmath::MultMatrixVector(RigidBodyMath::EP2G(ep), ep_tt, alpha); //EP2G_t not needed for Euler parameters as EP2G_t*ep_t=0!

	return alpha;
}

//! return configuration dependent local (=body fixed) angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyEP::GetAngularVelocityLocal(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> ep(GetRotationParameters(configuration));
	LinkedDataVector ep_t(GetRotationParameters_t(configuration));
	//ConstSizeMatrix<nDim3D * nRotationCoordinates> Glocal = RigidBodyMath::EP2Glocal(ep);

	Vector3D omegaLocal;
	EXUmath::MultMatrixVector(RigidBodyMath::EP2Glocal(ep), ep_t, omegaLocal);

	return omegaLocal;
}

//! provide position jacobian of node; derivative of 3D Position with respect to 7 coordinates ux,uy,uz,ep0,...,ep3
void CNodeRigidBodyEP::GetPositionJacobian(Matrix& value) const
{
	value.SetNumberOfRowsAndColumns(3, nDisplacementCoordinates + nRotationCoordinates);
	value.SetAll(0.);
	value(0, 0) = 1.;
	value(1, 1) = 1.;
	value(2, 2) = 1.;
}

//! provide "rotation" jacobian \f$\Jm_R\f$ of node; derivative of 3D angular velocity vector with respect to all velocity coordinates ("G-matrix"); action of torque \f$\mv\f$: \f$\Qm_m = \Jm_R^T \mv\f$
void CNodeRigidBodyEP::GetRotationJacobian(Matrix& value) const
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

//! provide according output variable in "value"
void CNodeRigidBodyEP::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	static_assert(maxRotationCoordinates == RigidBodyMath::maxRotCoordinates); //add this code to raise compiler error, if max. number of rotation coordiantes changes
	static_assert(nDim3D == 3); //add this code to raise compiler error, if nDim3D changes

	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(configuration)); break;
	case OutputVariableType::Displacement: value.CopyFrom(GetPosition(configuration) - GetPosition(ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(configuration)); break;
	case OutputVariableType::Acceleration: value.CopyFrom(GetAcceleration(configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocity(configuration)); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(GetAngularVelocityLocal(configuration)); break;
	case OutputVariableType::AngularAcceleration: value.CopyFrom(GetAngularAcceleration(configuration)); break;
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
		if (IsConfigurationInitialCurrentReferenceVisualization(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Reference + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyEP::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_t:
	{
		if (IsConfigurationInitialCurrentVisualization(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector_t(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyEP::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_tt:
	{
		if (IsConfigurationInitialCurrentVisualization(configuration))
		{
			value = GetCoordinateVector_tt(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyEP::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodeRigidBodyEP::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

void CNodeRigidBodyEP::ComputeAlgebraicEquations(Vector& algebraicEquations, bool useIndex2) const
{
	algebraicEquations.SetNumberOfItems(1);
	if (!useIndex2)
	{
		//position level constraint:

		ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> ep = GetRotationParameters();
		algebraicEquations[0] = ep * ep - 1.;
	}
	else
	{
		//velocity level constraint:
		ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> ep = GetRotationParameters();
		LinkedDataVector ep_t = GetRotationParameters_t();

		algebraicEquations[0] = 2. * (ep * ep_t);
	}
}

//! Compute jacobians of algebraic equations part of rigid body w.r.t. ODE2
void CNodeRigidBodyEP::ComputeJacobianAE(ResizableMatrix& jacobian_ODE2, ResizableMatrix& jacobian_ODE2_t,
	ResizableMatrix& jacobian_ODE1, ResizableMatrix& jacobian_AE) const
{
	jacobian_ODE2.SetNumberOfRowsAndColumns(GetNumberOfAECoordinates(), GetNumberOfODE2Coordinates());
	jacobian_ODE2_t.SetNumberOfRowsAndColumns(0, 0); //for safety!
	jacobian_ODE1.SetNumberOfRowsAndColumns(0, 0); //for safety!
	jacobian_AE.SetNumberOfRowsAndColumns(0, 0);//for safety!

	ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> ep = GetRotationParameters();

	//jacobian = [0 0 0 2*ep0 2*ep1 2*ep2 2*ep3]
	for (Index i = 0; i < nDisplacementCoordinates; i++) { jacobian_ODE2(0, i) = 0.; }
	for (Index i = 0; i < GetNumberOfRotationCoordinates(); i++)
	{
		jacobian_ODE2(0, 3 + i) = 2.*ep[i];
	}
	//pout << "jacRB=" << jacobian_ODE2 << "\n";
}


