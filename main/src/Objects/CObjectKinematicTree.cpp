/** ***********************************************************************************************
* @brief        CObjectKinematicTree implementation
*
* @author       Gerstmayr Johannes
* @date         2019-06-15 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"

#include "Main/CSystemData.h"
#include "Linalg/RigidBodyMath.h"
//#include "Linalg/KinematicsBasics.h" //included in CObjectKinematicTree.h

#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/CObjectKinematicTree.h"

//**delete
//#include <pybind11/pybind11.h>
//#include <pybind11/stl.h>
//#include <pybind11/stl_bind.h>
//#include <pybind11/operators.h>
//#include <pybind11/numpy.h>       //interface to numpy
//#include <pybind11/buffer_info.h> //passing reference to matrix to numpy
//#include <pybind11/embed.h>       //scoped interpreter
////does not work globally: #include <pybind11/iostream.h> //used to redirect cout:  py::scoped_ostream_redirect output;
////#include <pybind11/cast.h> //for arguments
//#include <pybind11/functional.h> //for function handling ... otherwise gives a python error (no compilation error in C++ !)

//typedef py::array_t<Real> NumpyMatrix; //declaration to avoid all pybind includes



//! Computational function: compute mass matrix
void CObjectKinematicTree::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	//massMatrixC.SetUseDenseMatrix(true); //uses sparse matrix with no entries.
	//Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	//Index nODE2 = NumberOfLinks();
	//massMatrix.SetNumberOfRowsAndColumns(nODE2, nODE2);
	//massMatrix.SetAll(0.);

	//LinkedDataVector q(GetCNode(0)->GetCurrentCoordinateVector());
	//LinkedDataVector q_t(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t());

	//LinkedDataVector qRef(GetCNode(0)->GetReferenceCoordinateVector());

	ComputeMassMatrixAndODE2LHS(&massMatrixC, &ltg, nullptr, objectNumber, true);

}

//! Computational function: compute right-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
//in fact, this is the LHS function!
void CObjectKinematicTree::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	//done in ComputeMassMatrixAndODE2LHS:
	//ode2Lhs.SetNumberOfItems(nODE2);
	//ode2Lhs.SetAll(0.);

	ComputeMassMatrixAndODE2LHS(nullptr, nullptr, &ode2Lhs, objectNumber, false);


}

//! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags
JacobianType::Type CObjectKinematicTree::GetAvailableJacobians() const
{
	return (JacobianType::Type)(JacobianType::ODE2_ODE2 + JacobianType::ODE2_ODE2_t);
}

//! provide according output variable in "value"
void CObjectKinematicTree::GetOutputVariable(OutputVariableType variableType, Vector& value, ConfigurationType configuration, Index objectNumber) const
{
	switch (variableType)
	{
	case OutputVariableType::Coordinates: ((CNodeODE2*)GetCNode(0))->GetODE2CoordinateVectorWithReference(value, configuration); break;
	case OutputVariableType::Coordinates_t: value.CopyFrom(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t());	break;
	case OutputVariableType::Coordinates_tt: 
	{
		value.CopyFrom(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_tt());
		break;
	}
	case OutputVariableType::Force:
	{
		CHECKandTHROW(configuration == ConfigurationType::Current, "ObjectKinematicTree::GetOutputVariable: OutputVariableType::Force can only be computed for Current configuration");
		ComputeODE2LHS(value, objectNumber);	break;
	}
	default:
		SysError("CObjectKinematicTree::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectKinematicTree::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	//this function is currently called, but maybe a way is found to avoid this!
	//CHECKandTHROWstring("CObjectKinematicTree::GetPosition: function not available");
	//return { 0,0,0 };
	return parameters.baseOffset; //used e.g. for Sensors!
	//return ((CNodeODE2*)GetCNode(0))->GetPosition(configuration) + localPosition;

}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectKinematicTree::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	CHECKandTHROWstring("CObjectKinematicTree::GetVelocity: function not available");
	return { 0,0,0 };
	//return ((CNodeODE2*)GetCNode(0))->GetVelocity(configuration);
}

//! return the (global) position of "localPosition" according to configuration type
Vector3D CObjectKinematicTree::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	CHECKandTHROWstring("CObjectKinematicTree::GetDisplacement: function not available");
	return { 0,0,0 };
	//return ((CNodeODE2*)GetCNode(0))->GetPosition(configuration) - ((CNodeODE2*)GetCNode(0))->GetPosition(ConfigurationType::Reference); //this also works for NodePointGround
}

//! return the local position of the center of mass, used for massProportionalLoad, which may NOT be appropriate for GenericODE2
Vector3D CObjectKinematicTree::GetLocalCenterOfMass() const 
{
	CHECKandTHROWstring("CObjectKinematicTree::GetLocalCenterOfMass: function not available");
	return Vector3D(0.);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++     KINEMATIC TREE MAIN FUNCTIONS     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! compute joint transformation T and motion subspace MS for jointType and joint value q
void CObjectKinematicTree::JointTransformMotionSubspace66(Joint::Type jointType, Real q, Transformation66& T, Vector6D& MS) const
{
	switch (jointType)
	{
	case Joint::RevoluteX:
	{
		T = RigidBodyMath::RotationTranslation2T66(RigidBodyMath::RotationMatrix1(-q), Vector3D({ 0.,0.,0. })); //negative sign because of inverse transformation needed!
		MS = Vector6D({ 1., 0., 0., 0., 0., 0. });
		break;
	}
	case Joint::RevoluteY:
	{
		T = RigidBodyMath::RotationTranslation2T66(RigidBodyMath::RotationMatrix2(-q), Vector3D({ 0.,0.,0. }));
		MS = Vector6D({ 0., 1., 0., 0., 0., 0. });
		break;
	}
	case Joint::RevoluteZ:
	{
		T = RigidBodyMath::RotationTranslation2T66(RigidBodyMath::RotationMatrix3(-q), Vector3D({ 0.,0.,0. }));
		MS = Vector6D({ 0., 0., 1., 0., 0., 0. });
		break;
	}
	case Joint::PrismaticX:
	{
		T = RigidBodyMath::RotationTranslation2T66(EXUmath::unitMatrix3D, Vector3D({ -q, 0., 0. })); //negative sign because of inverse transformation needed!
		MS = Vector6D({ 0., 0., 0., 1., 0., 0. });
		break;
	}
	case Joint::PrismaticY:
	{
		T = RigidBodyMath::RotationTranslation2T66(EXUmath::unitMatrix3D, Vector3D({ 0., -q, 0. }));
		MS = Vector6D({ 0., 0., 0., 0., 1., 0. });
		break;
	}
	case Joint::PrismaticZ:
	{
		T = RigidBodyMath::RotationTranslation2T66(EXUmath::unitMatrix3D, Vector3D({ 0., 0., -q }));
		MS = Vector6D({ 0., 0., 0., 0., 0., 1. });
		break;
	}
	default:
		SysError("CObjectKinematicTree::JointTransformMotionSubspace66 failed"); //error should not occur, because types are checked!
	}

}

//! compute negative 6D gravity to be used in Pluecker transforms
void CObjectKinematicTree::GetNegativeGravity6D(Vector6D& gravity6D) const
{
	gravity6D[0] = 0.; 
	gravity6D[1] = 0.; 
	gravity6D[2] = 0.;
	gravity6D[3] = -parameters.gravity[0];
	gravity6D[4] = -parameters.gravity[1];
	gravity6D[5] = -parameters.gravity[2];
}


//! compute link transformations as 6D vectors; velcoties and accelerations are absolute, while Xup is relative if computeAbsoluteTransformations==false
void CObjectKinematicTree::ComputeTreeTransformations(ConfigurationType configuration, bool computeVelocitiesAccelerations, 
	bool computeAbsoluteTransformations, Transformations66List& Xup, Vector6DList& V, Vector6DList& A) const
{
	Index n = NumberOfLinks();
	Xup.SetNumberOfItems(n);
	if (computeVelocitiesAccelerations)
	{
		V.SetNumberOfItems(n);
		A.SetNumberOfItems(n);
	}
	else
	{
		V.SetNumberOfItems(0);
		A.SetNumberOfItems(0);
	}
	//Vector6D negGravity6D;
	//GetNegativeGravity6D(negGravity6D);

	LinkedDataVector q(GetCNode(0)->GetCoordinateVector(configuration));
	LinkedDataVector q_t(((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration));
	LinkedDataVector q_tt(((CNodeODE2*)GetCNode(0))->GetCoordinateVector_tt(configuration));
	LinkedDataVector qRef(GetCNode(0)->GetReferenceCoordinateVector());

	Vector6D MS; //motion subspace
	Transformation66 XJ; //transformation from previous to this link
	Transformation66 baseTransform = RigidBodyMath::RotationTranslation2T66Inverse(EXUmath::unitMatrix3D,
		GetParameters().baseOffset);

	//iterate over all joints in kinematic tree
	for (Index i = 0; i < n; i++)
	{
		Real qTotal = q[i];
		if (configuration != ConfigurationType::Reference) { qTotal += qRef[i]; }
		JointTransformMotionSubspace66(parameters.jointTypes[i], qTotal, XJ, MS);
		Transformation66 XL=RigidBodyMath::RotationTranslation2T66Inverse(parameters.jointTransformations[i], parameters.jointOffsets[i]);
		Xup[i] = XJ * XL; 

		if (computeVelocitiesAccelerations)
		{
			A[i].SetAll(0.);
			Vector6D vJ = MS * q_t[i];
			Vector6D aJ = MS * q_tt[i]; //needs to be checked!
			if (parameters.linkParents[i] == CObjectKinematicTree::noParent)
			{
				V[i] = vJ;
				A[i] = aJ;
				//A[i] = Xup[i] * negGravity6D; //this is used to compute forces, but no "kinematic" acceleration
			}
			else
			{
				V[i] = Xup[i] * V[parameters.linkParents[i]] + vJ;
				A[i] = Xup[i] * A[parameters.linkParents[i]] + RigidBodyMath::T66SkewMotion(V[i]) * vJ + aJ;
			}
		}
		if (computeAbsoluteTransformations)
		{
			if (GetParameters().linkParents[i] != CObjectKinematicTree::noParent)
			{
				Xup[i] = Xup[i] * Xup[GetParameters().linkParents[i]];
			}
			else
			{
				Xup[i] = Xup[i] * baseTransform;
			}

		}
	}
}

//! compute mass matrix and ODE2LHS; computeMass is optional, not needed for ODE2LHS case
void CObjectKinematicTree::ComputeMassMatrixAndODE2LHS(EXUmath::MatrixContainer* massMatrixC, const ArrayIndex* ltg, Vector* ode2Lhs, 
	Index objectNumber, bool computeMass) const
{
	Index n = NumberOfLinks();

	Transformations66List& Xup = jointTransformationsTemp;
	Vector6DList& MS = motionSubspaces;
	Vector6DList& V = jointVelocities;
	Vector6DList& Avp = jointAccelerations;
	Vector6DList& Fvp = jointForces;
	Transformations66List& IC = linkInertiasT66; //temporary matrices, will be overwritten on every call!

	Xup.SetNumberOfItems(n);
	MS.SetNumberOfItems(n);
	V.SetNumberOfItems(n);
	Avp.SetNumberOfItems(n);
	Fvp.SetNumberOfItems(n);
	IC.SetNumberOfItems(n);

	Vector6D negGravity6D;
	GetNegativeGravity6D(negGravity6D);

	//LinkedDataVector q(GetCNode(0)->GetCurrentCoordinateVector());
	//LinkedDataVector qRef(GetCNode(0)->GetReferenceCoordinateVector());
	tempVector.CopyFrom(GetCNode(0)->GetCurrentCoordinateVector());
	tempVector += GetCNode(0)->GetReferenceCoordinateVector();
	LinkedDataVector q(tempVector);
	LinkedDataVector q_t(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t());

	Transformation66 XJ; //transformation from previous to this link

	//iterate over all joints in kinematic tree
	//propagate velocities and accelerations from base to leaves, also propagate inertia forces to leaves
	for (Index i = 0; i < n; i++)
	{
		//pout << "*********************\nLINK" << i << "(computeMass=" << computeMass << "):\n";

		JointTransformMotionSubspace66(parameters.jointTypes[i], q[i], XJ, MS[i]);
		Transformation66 XL = RigidBodyMath::RotationTranslation2T66Inverse(parameters.jointTransformations[i], parameters.jointOffsets[i]);
		Xup[i] = XJ * XL;
		//pout << "XL=" << XL << "\n";
		//pout << "XJ=" << XJ << "\n";
		//pout << "MS=" << MS << "\n";
		//pout << "Xup=" << Xup << "\n";

		Vector6D vJ = MS[i] * q_t[i];
		if (parameters.linkParents[i] == CObjectKinematicTree::noParent)
		{
			V[i] = vJ;
			Avp[i] = Xup[i] * negGravity6D;
		}
		else
		{
			V[i] = Xup[i] * V[parameters.linkParents[i]] + vJ;
			Avp[i] = Xup[i] * Avp[parameters.linkParents[i]] + RigidBodyMath::T66SkewMotion(V[i]) * vJ;
		}
		IC[i] = RigidBodyMath::InertiaT66FromInertiaParameters(parameters.linkMasses[i], parameters.linkCOMs[i], parameters.linkInertiasCOM[i]);
		if (!computeMass)
		{
			Fvp[i] = IC[i]* Avp[i] + RigidBodyMath::T66SkewForce(V[i]) * (IC[i] * V[i]);
			//pout << "Fvp=" << Fvp[i] << "\n";
			//fvp[i] = self.inertias[i] @ avp[i] + CRF(v[i]) @ self.inertias[i] @ v[i]
		}
		//pout << "IC=" << IC[i] << "\n";
		//pout << "CRF=" << RigidBodyMath::T66SkewForce(V[i]) << "\n";

		//pout << "V=" << V[i] << "\n";
		//pout << "Avp=" << Avp[i] << "\n";

	}


	//	f = np.zeros(n)             #store joint forces
	//	M = np.zeros((n, n))         #store mass matrix
	if (!computeMass)
	{
		//+++++++++++++++++++++++++++++++++
		//add external forces to links
		AddExternalForces6D(Xup, Fvp); //adds global linkTorques / linkForces per link to Fvp using Xup transformations

		//+++++++++++++++++++++++++++++++++
		//now compute forces per joint:
		//propagate forces back from leaves to base:
		Vector& f = *ode2Lhs;
		f.SetNumberOfItems(n);
		f.SetAll(0.); //not needed, is directly written!

		for (Index i = n - 1; i >= 0; i--)
		{
			f[i] = MS[i] * Fvp[i];
			if (parameters.linkParents[i] != noParent)
			{
				Fvp[parameters.linkParents[i]] += Fvp[i] * Xup[i]; //Xup[i].T @ fvp[i]
				//Fvp[parameters.linkParents[i]] += Xup[i].GetTransposed() * Fvp[i]; //Xup[i].T @ fvp[i]
			}
		}

		//	#compute joint forces; REVERSED loop!
		//	for i in reversed(range(n)) :
		//		f[i] = MS[i].T @ fvp[i]
		//		if self.parents[i] != -1:
		//			fvp[self.parents[i]] += Xup[i].T @ fvp[i]

		if (parameters.jointForceVector.NumberOfItems() != 0)
		{
			f -= parameters.jointForceVector; //constant forces per coordinate (RevoluteJoint: local torque in joint)
		}

		for (Index i = 0; i < n; i++)
		{
			if (parameters.jointPositionOffsetVector.NumberOfItems() != 0) //P control checked automatically
			{
				f[i] -= parameters.jointPControlVector[i] * (parameters.jointPositionOffsetVector[i] - q[i]);
			}
			if (parameters.jointVelocityOffsetVector.NumberOfItems() != 0) //P control checked automatically
			{
				f[i] -= parameters.jointDControlVector[i] * (parameters.jointVelocityOffsetVector[i] - q_t[i]);
			}
		}

		if (parameters.forceUserFunction)
		{
			Real t = GetCSystemData()->GetCData().GetCurrent().GetTime();
			Vector userForce;

			EvaluateUserFunctionForce(userForce, cSystemData->GetMainSystemBacklink(), t, objectNumber, tempVector, tempVector2);
			CHECKandTHROW(userForce.NumberOfItems() == n, "CObjectKinematicTree: forceUserFunction return a vector with different size from ObjectKinematicTree system size");
			f -= userForce;
		}

	}
	else
	{
		massMatrixC->SetUseDenseMatrix();
		ResizableMatrix& M = massMatrixC->GetInternalDenseMatrix();
		M.SetScalarMatrix(n, 0.);

		//	#compute composite inertia
		//	#IC = deepcopy(self.inertias) #may cause problems if referenced np.arrays used!
		//	IC = [np.zeros((6, 6))] * n

		//	for i in range(n) :
		//		IC[i] = deepcopy(self.inertias[i])
		for (Index i = n - 1; i >= 0; i--)
		{
			if (parameters.linkParents[i] != noParent)
			{
				IC[parameters.linkParents[i]] += Xup[i].GetTransposed() * IC[i] * Xup[i];
			}
		}
		//	for i in reversed(range(n)) :
		//		if self.parents[i] != -1 :
		//			IC[self.parents[i]] += Xup[i].T @ IC[i] @ Xup[i]

		for (Index i = 0; i < n; i++)
		{
			Vector6D fh = IC[i] * MS[i];
			M(i, i) = MS[i] * fh;
			Index j = i;
			while (parameters.linkParents[j] != noParent)
			{
				fh = fh * Xup[j]; //Xup[j].T @ fh
				j = parameters.linkParents[j];
				M(i, j) = MS[j] * fh;
				M(j, i) = M(i, j);
			}
		}
		//	#compute generalized mass matrix and projected inertia
		//	for i in range(n) :
		//		fh = IC[i] @ MS[i]
		//		M[i, i] = MS[i].T @ fh
		//		j = i
		//		while self.parents[j] > -1:
		//			fh = Xup[j].T @ fh
		//			j = self.parents[j]
		//			M[i, j] = MS[j].T @ fh
		//			M[j, i] = M[i, j]

		//	return[M, f]
	}


}

//! compute object coordinates composed from all nodal coordinates; does not include reference coordinates
void CObjectKinematicTree::AddExternalForces6D(const Transformations66List& Xup, Vector6DList& Fvp) const
{
	Index n = NumberOfLinks();
	Transformations66List& Xa = jointTransformationsTemp;
	Xa.SetNumberOfItems(n);
	
	if (parameters.linkForces.NumberOfItems() != 0 || parameters.linkTorques.NumberOfItems())
	{
		for (Index i = 0; i < n; i++)
		{
			if (parameters.linkParents[i] == CObjectKinematicTree::noParent)
			{
				Xa[i] = Xup[i]; //not needed for forces: *baseTransform;
			}
			else
			{
				Xa[i] = Xup[i] * Xa[parameters.linkParents[i]];
			}
			Vector6D extForce6D(0.); //initialize with zeros, as only parts may be filled
			if (parameters.linkTorques.NumberOfItems())
			{
				extForce6D[0] = parameters.linkTorques[i][0];
				extForce6D[1] = parameters.linkTorques[i][1];
				extForce6D[2] = parameters.linkTorques[i][2];
			}
			if (parameters.linkForces.NumberOfItems() != 0)
			{
				extForce6D[3] = parameters.linkForces[i][0];
				extForce6D[4] = parameters.linkForces[i][1];
				extForce6D[5] = parameters.linkForces[i][2];
			}
			Fvp[i] += RigidBodyMath::T66MotionInverse(Xa[i].GetTransposed()) * extForce6D;
			
		}
	}

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++     KINEMATIC TREE FUNCTIONS      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//OutputVariable functions:

//! return the (global) position of 'localPosition' of linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetPositionKinematicTree(const Vector3D& localPosition, Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetPositionKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, false, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	return pos3D + rot3D * localPosition;
}

//! return the rotation matrix of of linkNumber according to configuration type
Matrix3D CObjectKinematicTree::GetRotationMatrixKinematicTree(Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetRotationMatrixKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, false, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	return rot3D;
}

//! return the (global) velocity of 'localPosition' and linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetVelocityKinematicTree(const Vector3D& localPosition, Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetVelocityKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	Vector3D velLocal({ jointVelocities[linkNumber][3], jointVelocities[linkNumber][4], jointVelocities[linkNumber][5] });
	Vector3D angVelLocal({ jointVelocities[linkNumber][0], jointVelocities[linkNumber][1], jointVelocities[linkNumber][2] });

	return rot3D * (velLocal + angVelLocal.CrossProduct(localPosition));
}

//! return the (global) angular velocity of linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetAngularVelocityKinematicTree(Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetAngularVelocityKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	//Vector3D velLocal({ jointVelocities[linkNumber][3], jointVelocities[linkNumber][4], jointVelocities[linkNumber][5] });
	Vector3D angVelLocal({ jointVelocities[linkNumber][0], jointVelocities[linkNumber][1], jointVelocities[linkNumber][2] });

	return rot3D * angVelLocal;
}

//! return the (local) angular velocity of linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetAngularVelocityLocalKinematicTree(Index linkNumber, ConfigurationType configuration ) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetAngularVelocityLocalKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	//Matrix3D rot3D;
	//Vector3D pos3D;
	//RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	//Vector3D velLocal({ jointVelocities[linkNumber][3], jointVelocities[linkNumber][4], jointVelocities[linkNumber][5] });
	Vector3D angVelLocal({ jointVelocities[linkNumber][0], jointVelocities[linkNumber][1], jointVelocities[linkNumber][2] });

	return angVelLocal;
}

//! return the (global) acceleration of 'localPosition' and linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetAccelerationKinematicTree(const Vector3D& localPosition, Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetAccelerationKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	Vector3D angVelLocal({ jointVelocities[linkNumber][0], jointVelocities[linkNumber][1], jointVelocities[linkNumber][2] });
	Vector3D velLocal({ jointVelocities[linkNumber][3], jointVelocities[linkNumber][4], jointVelocities[linkNumber][5] });
	Vector3D angAccLocal({ jointAccelerations[linkNumber][0], jointAccelerations[linkNumber][1], jointAccelerations[linkNumber][2] });
	Vector3D accLocal({ jointAccelerations[linkNumber][3], jointAccelerations[linkNumber][4], jointAccelerations[linkNumber][5] });

	//d/dt[rot3D * (velLocal + angVelLocal.CrossProduct(localPosition)) ] =
	//     rot3D * (accLocal + angAccLocal.CrossProduct(localPosition) ) +
	//     rot3D * angVelLocal.Cross(velLocal + angVelLocal.CrossProduct(localPosition))
	// = rot3D * (accLocal + angVelLocal.CrossProduct(velLocal) + angVelLocal.CrossProduct(angVelLocal.CrossProduct(localPosition)) + 
	//   angAccLocal.CrossProduct(localPosition) )

	//WRONG: DELETE:
	//return rot3D * (accLocal + angAccLocal.CrossProduct(localPosition) + 
	//	angVelLocal.CrossProduct(angVelLocal.CrossProduct(localPosition)) );
	return rot3D * (accLocal + angVelLocal.CrossProduct(velLocal) + 
		angVelLocal.CrossProduct(angVelLocal.CrossProduct(localPosition)) + 
		angAccLocal.CrossProduct(localPosition));
}

//! return the (global) angular acceleration of linkNumber according to configuration type
Vector3D CObjectKinematicTree::GetAngularAccelerationKinematicTree(Index linkNumber, ConfigurationType configuration) const
{
	CHECKandTHROW(linkNumber < NumberOfLinks(), "CObjectKinematicTree::GetAngularAccelerationKinematicTree: invalid linkNumber");

	ComputeTreeTransformations(configuration, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Matrix3D rot3D;
	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], rot3D, pos3D);

	Vector3D angAccLocal({ jointAccelerations[linkNumber][0], jointAccelerations[linkNumber][1], jointAccelerations[linkNumber][2] });

	return rot3D * angAccLocal;
}

//! OutputVariableTypes independent of meshNodeNumber
OutputVariableType CObjectKinematicTree::GetOutputVariableTypesSuperElement(Index meshNodeNumber) const
{
	//independent of meshNodeNumber!!!
	OutputVariableType ovt = (OutputVariableType)(
		(Index)OutputVariableType::Position +
		(Index)OutputVariableType::Displacement +
		(Index)OutputVariableType::Velocity +
		(Index)OutputVariableType::VelocityLocal +
		(Index)OutputVariableType::Acceleration +
		(Index)OutputVariableType::AccelerationLocal +
		(Index)OutputVariableType::Rotation +
		(Index)OutputVariableType::RotationMatrix +
		(Index)OutputVariableType::AngularVelocity +
		(Index)OutputVariableType::AngularVelocityLocal +
		(Index)OutputVariableType::Acceleration +
		(Index)OutputVariableType::AccelerationLocal
		);

	return ovt;

}

void CObjectKinematicTree::GetOutputVariableKinematicTree(OutputVariableType variableType, const Vector3D& localPosition, Index linkNumber, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPositionKinematicTree(localPosition, linkNumber, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPositionKinematicTree(localPosition, linkNumber, configuration) - 
		GetPositionKinematicTree(localPosition, linkNumber, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocityKinematicTree(localPosition, linkNumber, configuration)); break;
	case OutputVariableType::VelocityLocal: value.CopyFrom(GetRotationMatrixKinematicTree(linkNumber, configuration).GetTransposed()*GetVelocityKinematicTree(localPosition, linkNumber, configuration)); break; //inefficient, but useful
	case OutputVariableType::Acceleration: value.CopyFrom(GetAccelerationKinematicTree(localPosition, linkNumber, configuration)); break;
	case OutputVariableType::AccelerationLocal: value.CopyFrom(GetRotationMatrixKinematicTree(linkNumber, configuration).GetTransposed()*
		GetAccelerationKinematicTree(localPosition, linkNumber, configuration)); break;

	case OutputVariableType::Rotation: {
		Matrix3D rotMat = GetRotationMatrixKinematicTree(linkNumber, configuration);
		Vector3D rot = RigidBodyMath::RotationMatrix2RotXYZ(rotMat);
		value.CopyFrom(rot);
		break;
	}
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = GetRotationMatrixKinematicTree(linkNumber, configuration);
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocityKinematicTree(linkNumber, configuration)); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(GetAngularVelocityLocalKinematicTree(linkNumber, configuration)); break;
	case OutputVariableType::AngularAcceleration: value.CopyFrom(GetAngularAccelerationKinematicTree(linkNumber, configuration)); break;
	case OutputVariableType::AngularAccelerationLocal: value.CopyFrom(GetRotationMatrixKinematicTree(linkNumber, configuration).GetTransposed()*
		GetAngularAccelerationKinematicTree(linkNumber, configuration)); break;

	default:
		SysError("CObjectKinematicTree::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//AccessFunction:
//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectKinematicTree::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::AngularVelocity_qt +
		//(Index)AccessFunctionType::DisplacementMassIntegral_q + //is included in recursive formula, more efficient!
		(Index)AccessFunctionType::KinematicTree);
}

//! just make sure that this overwritten function is not called!
void CObjectKinematicTree::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	SysError("CObjectKinematicTree:GetAccessFunctionBody not available");
}


//! just make sure that this overwritten function is not called!
void CObjectKinematicTree::GetAccessFunctionSuperElement(AccessFunctionType accessType, const Matrix& weightingMatrix, const ArrayIndex& meshNodeNumbers, Matrix& value) const
{
	CHECKandTHROWstring("CObjectKinematicTree::GetAccessFunctionSuperElement: Function called without intention; use MarkerKinematicTree instead of MarkerSuperElement");
}


//! this function replaces regular AccessFunction for kinematic tree, per link
void CObjectKinematicTree::ComputeRigidBodyMarkerDataKT(const Vector3D& localPosition, Index linkNumber, bool computeJacobian, MarkerData& markerData) const
{
	//CHECKandTHROWstring("CObjectKinematicTree::GetAccessFunctionKinematicTree: not implemented yet");

	//for info on what to compute, compare to CObjectBody.h : ComputeRigidBodyMarkerData

	//compute position, orientation, velocity, angularVelocity
	//position jacobian and rotation jacobian on demand (computeJacobian==true)

	ComputeTreeTransformations(ConfigurationType::Current, true, true, jointTransformationsTemp, jointVelocities, jointAccelerations);

	Vector3D pos3D;
	RigidBodyMath::T66toRotationTranslationInverse(jointTransformationsTemp[linkNumber], markerData.orientation, pos3D);

	markerData.position = pos3D + markerData.orientation * localPosition;

	Vector3D velLocal({ jointVelocities[linkNumber][3], jointVelocities[linkNumber][4], jointVelocities[linkNumber][5] });
	Vector3D angVelLocal({ jointVelocities[linkNumber][0], jointVelocities[linkNumber][1], jointVelocities[linkNumber][2] });

	markerData.velocity = markerData.orientation * (velLocal + angVelLocal.CrossProduct(localPosition));
	markerData.angularVelocityLocal = angVelLocal;
	markerData.velocityAvailable = true;

	//for jacobian links need to be processed backwards via parent links

	//pout << "******************\n" << "link=" << linkNumber << ", locPos=" << localPosition << "\n";

	if (computeJacobian)
	{
		ComputeJacobian(linkNumber, markerData.position, jointTransformationsTemp, markerData.positionJacobian, markerData.rotationJacobian);
	}
}

//! compute rot+pos jacobian of (global) position at linkNumber, using pre-computed joint transformations
void CObjectKinematicTree::ComputeJacobian(Index linkNumber, const Vector3D& position, const Transformations66List& jointTransformations, 
	ResizableMatrix& positionJacobian, ResizableMatrix& rotationJacobian) const
{
	//put this jacobian function into a separate member function in future (to be able to access jacobian)
	Index n = NumberOfLinks();
	positionJacobian.SetNumberOfRowsAndColumns(3, n);
	rotationJacobian.SetNumberOfRowsAndColumns(3, n);
	positionJacobian.SetAll(0.); //not all entries will be filled!
	rotationJacobian.SetAll(0.); //...

	Index cnt = 0; //just for safety, if parents are having loop?
	Index i = linkNumber;
	Matrix3D rotJoint;	//position at joint axis of computation
	Vector3D posJoint;	//position at joint axis of computation
	while (i != noParent && cnt++ < n)
	{
		//pout << "i=" << i << ", cnt=" << cnt << ", n=" << n << ", parent=" << parameters.linkParents[i] << "\n";
		RigidBodyMath::T66toRotationTranslationInverse(jointTransformations[i], rotJoint, posJoint);

		Vector3D localAxis(0.);

		localAxis[Joint::AxisNumber(parameters.jointTypes[i])] = 1.;
		Vector3D axis = rotJoint * localAxis; //jacobian of global rotation/position
		if (Joint::IsRevolute(parameters.jointTypes[i]))
		{
			rotationJacobian.SetColumnVector(axis, i); 
			positionJacobian.SetColumnVector(axis.CrossProduct(position - posJoint), i);
			//pout << "posJoint=" << posJoint << "\n";
		}
		else if (Joint::IsPrismatic(parameters.jointTypes[i]))
		{
			positionJacobian.SetColumnVector(rotJoint*axis, i);
		}
		else { CHECKandTHROWstring("CObjectKinematicTree::ComputeJacobian: illegal jointType"); }

		i = parameters.linkParents[i];
	}
	CHECKandTHROW(cnt <= n, "CObjectKinematicTree::ComputeJacobian: too many iterations when computing jacobian; check parent indices");
}
















//needed:???
//! return the (global) position of a mesh node according to configuration type; this is the node position transformed by the motion of the reference frame; meshNodeNumber is the local node number of the (underlying) mesh
Vector3D CObjectKinematicTree::GetMeshNodePosition(Index meshNodeNumber, ConfigurationType configuration) const
{
	CHECKandTHROWstring("CObjectKinematicTree::GetMeshNodePosition: called without intention!");
	return Vector3D();
}

//! return the (global) velocity of a mesh node according to configuration type; this is the node position transformed by the motion of the reference frame; meshNodeNumber is the local node number of the (underlying) mesh
Vector3D CObjectKinematicTree::GetMeshNodeVelocity(Index meshNodeNumber, ConfigurationType configuration) const
{
	CHECKandTHROWstring("CObjectKinematicTree::GetMeshNodeVelocity: called without intention!");
	return Vector3D();
}

