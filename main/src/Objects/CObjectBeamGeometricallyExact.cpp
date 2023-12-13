/** ***********************************************************************************************
* @brief        CObjectBeamGeometricallyExact implementation
*
* @author       Gerstmayr Johannes
* @date         2021-03-21 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Pymodules/PyMatrixVector.h"

#include "Linalg/KinematicsBasics.h"
#include "Autogenerated/MainObjectBeamGeometricallyExact.h"

//#include "Main/CSystemData.h" //if only C class imported
//#include "Autogenerated/CObjectBeamGeometricallyExact.h"



//! special function which writes pyObject into local data
void MainObjectBeamGeometricallyExact::SetInternalBeamSection(const py::object& pyObject)
{
	if (py::isinstance<PyBeamSection>(pyObject)) //this must be the C-object
	{
		PyBeamSection bs(py::cast<PyBeamSection>(pyObject));
		GetCObjectBeamGeometricallyExact()->GetParameters().physicsAxialShearStiffness =
			Vector3D({ bs.stiffnessMatrix(0,0),bs.stiffnessMatrix(1,1),bs.stiffnessMatrix(2,2) });
		GetCObjectBeamGeometricallyExact()->GetParameters().physicsTorsionalBendingStiffness =
			Vector3D({ bs.stiffnessMatrix(3,3),bs.stiffnessMatrix(4,4),bs.stiffnessMatrix(5,5) });

		GetCObjectBeamGeometricallyExact()->GetParameters().physicsCrossSectionInertia = bs.inertia;
		GetCObjectBeamGeometricallyExact()->GetParameters().physicsMassPerLength = bs.massPerLength;

		//CHECK that there are no parameters in BeamSection which are not processed:
		PyBeamSection bsCheck;
		bsCheck = GetInternalBeamSection();
		if (!(bs.stiffnessMatrix == bsCheck.stiffnessMatrix))
		{
			PyError("ObjectBeamGeometricallyExact: BeamSection stiffnessMatrix contains values which can not be used");
		}
		if (!(bs.inertia == bsCheck.inertia))
		{
			PyError("ObjectBeamGeometricallyExact: BeamSection inertia contains values which can not be used");
		}
	}
	else
	{
		PyError("ObjectBeamGeometricallyExact: expected BeamSection, but received: " + STDstring(py::str(pyObject)));
	}
}

//! AUTO: special function which returns BeamSection converted from local data
PyBeamSection MainObjectBeamGeometricallyExact::GetInternalBeamSection() const
{
	PyBeamSection bs;
	Vector3D kAS = GetCObjectBeamGeometricallyExact()->GetParameters().physicsAxialShearStiffness;
	bs.stiffnessMatrix(0, 0) = kAS[0];
	bs.stiffnessMatrix(1, 1) = kAS[1];
	bs.stiffnessMatrix(2, 2) = kAS[2];

	Vector3D kKappa = GetCObjectBeamGeometricallyExact()->GetParameters().physicsTorsionalBendingStiffness;
	bs.stiffnessMatrix(3, 3) = kKappa[0];
	bs.stiffnessMatrix(4, 4) = kKappa[1];
	bs.stiffnessMatrix(5, 5) = kKappa[2];

	bs.inertia = GetCObjectBeamGeometricallyExact()->GetParameters().physicsCrossSectionInertia;
	bs.massPerLength = GetCObjectBeamGeometricallyExact()->GetParameters().physicsMassPerLength;

	return bs;
}

////! AUTO: special function which writes pyObject into local data
//void MainObjectBeamGeometricallyExact::SetInternalBeamSectionGeometry(const py::object& pyObject)
//{
//
//}
//
////! AUTO: special function which returns BeamSectionGeometry converted from local data
//BeamSectionGeometry MainObjectBeamGeometricallyExact::GetInternalBeamSectionGeometry() const
//{
//
//}

//! number of ODE2 coordinates from both nodes
Index CObjectBeamGeometricallyExact::GetODE2Size() const
{
	return GetCNode(0)->GetNumberOfODE2Coordinates() + GetCNode(1)->GetNumberOfODE2Coordinates();
}

//! shape function with x in [-L/2,L/2] and values (1,0) at x=-L/2 and (0,1) at x=L/2
Vector2D CObjectBeamGeometricallyExact::ComputeShapeFunctions(Real x) const
{
	Real lElem = parameters.physicsLength;
	return Vector2D({ (lElem*0.5 - x) / lElem, (lElem*0.5 + x) / lElem });
}

//Vector2D CObjectBeamGeometricallyExact::ComputeShapeFunctions_x(Real x) const
//{
//	Real lElem = parameters.physicsLength;
//	return Vector2D({ -1 / lElem , 1 / lElem });
//}

//! map element coordinates (position or velocity level) given by nodal vectors q0 and q1 onto compressed shape function vector to compute position, etc.
Vector3D CObjectBeamGeometricallyExact::MapVectors(const Vector2D& SV, const Vector3D& q0, const Vector3D& q1) const
{
	return Vector3D({ SV[0] * q0[0] + SV[1] * q1[0], 
					  SV[0] * q0[1] + SV[1] * q1[1], 
					  SV[0] * q0[2] + SV[1] * q1[2]});
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! Computational function: compute mass matrix
void CObjectBeamGeometricallyExact::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber, bool computeInverse) const
{
	CHECKandTHROW(!computeInverse, "CObjectBeamGeometricallyExact::ComputeMassMatrix: computeMassMatrixInversePerBody=True is not possible for this type of element; change solver settings");

	Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	massMatrix.SetScalarMatrix(GetODE2Size(), 0.); //set 6x6 matrix

	const Index nDim3D = 3;
	const Index nDisplacementCoordinates = 3;
	Index nNode0 = GetCNode(0)->GetNumberOfODE2Coordinates();
	Real L = parameters.physicsLength;
	Real intFactL = 0.5*L; //integration weight, using 2 points Lobatto
	//set mass terms in first 3 diagonal entries and set remaining entries to zero (the last 4x4 entries will be overwritten when filling in inertia terms)
	for (Index i = 0; i < nDim3D; i++)
	{
		massMatrix(i, i) = parameters.physicsMassPerLength * intFactL;
		massMatrix(i+nNode0, i + nNode0) = parameters.physicsMassPerLength * intFactL;
	}

	//ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> rot = ((CNodeRigidBody*)GetCNode(0))->GetRotationParameters();
	ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> Glocal;

	for (Index i = 0; i < GetNumberOfNodes(); i++)
	{
		((CNodeRigidBody*)GetCNode(i))->GetGlocal(Glocal);// RigidBodyMath::EP2Glocal(rot);

		ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> GlocalInertia(CNodeRigidBody::maxRotationCoordinates, nDim3D);

		EXUmath::MultMatrixTransposedMatrix(Glocal, intFactL*parameters.physicsCrossSectionInertia, GlocalInertia);

		EXUmath::MultMatrixMatrix2SubmatrixTemplate<ConstSizeMatrix<12>, ConstSizeMatrix<12>, Matrix>(GlocalInertia, Glocal, massMatrix, 
			nDisplacementCoordinates + i*nNode0, nDisplacementCoordinates + i*nNode0);

	}
	//pout << "M=" << massMatrix << "\n";

}

//#define useAverageRotation
//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to 'ode2Lhs'
void CObjectBeamGeometricallyExact::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	ode2Lhs.SetNumberOfItems(GetODE2Size());
	ode2Lhs.SetAll(0.);

	const Index nDim3D = 3;
	//const Index nDisplacementCoordinates = 3;
	Index nNode0 = GetCNode(0)->GetNumberOfODE2Coordinates();
	Real L = parameters.physicsLength;
	//Real intFactL = 0.5*L; //integration weight, using 2 points Lobatto

	const CNodeRigidBody* node0 = (CNodeRigidBody*)GetCNode(0);
	const CNodeRigidBody* node1 = (CNodeRigidBody*)GetCNode(1);
	HomogeneousTransformation HT0(node0->GetRotationMatrix(), node0->GetPosition());
	HomogeneousTransformation HT1(node1->GetRotationMatrix(), node1->GetPosition());

	Vector3D incDisp;
	Vector3D incRot;
	HT0.GetRelativeMotionTo(HT1, incDisp, incRot);

	Matrix6D TexpInv[2];
	TexpInv[0] = -1.*EXUlie::TExpSE3Inv(-incDisp, -incRot);
	TexpInv[1] = EXUlie::TExpSE3Inv(incDisp, incRot);

#ifdef useAverageRotation
	Matrix3D A = ((CNodeRigidBody*)GetCNode(0))->GetRotationMatrix() * EXUlie::ExpSO3(0.5*incRot); //compute rotation at mid-span; is this correct?
#endif


	//pout << "incDisp=" << incDisp << ", ";
	//pout << "incRot=" << incRot << "\n";

	Vector6D K6D({
		parameters.physicsAxialShearStiffness[0],
		parameters.physicsAxialShearStiffness[1],
		parameters.physicsAxialShearStiffness[2],
		parameters.physicsTorsionalBendingStiffness[0],
		parameters.physicsTorsionalBendingStiffness[1],
		parameters.physicsTorsionalBendingStiffness[2]});

	//future: h0 must contain (pre-deformed) reference configuration!!!
	Vector6D h0(0.);
	h0[0] = L;
	Vector6D h({ incDisp[0], incDisp[1], incDisp[2], incRot[0] , incRot[1] , incRot[2] });

	Vector6D eps = 1. / L * (h - h0); //deformation as incremental motion

	ConstSizeVector<CNodeRigidBody::maxRotationCoordinates + nDim3D> res(nNode0);
	ConstSizeVector<CNodeRigidBody::maxRotationCoordinates + nDim3D> res2(nNode0);

	ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> G;
	ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> Glocal;
	Vector3D omegaLocal;
	Real intFactL = 0.5*L; //integration weight, for quadratic velocity vector (approximated!)

	for (Index i = 0; i < GetNumberOfNodes(); i++)
	{
		((CNodeRigidBody*)GetCNode(i))->CollectCurrentNodeData1(Glocal, omegaLocal);
		((CNodeRigidBody*)GetCNode(i))->GetG(G);
		//omegaLocal = ((CNodeRigidBody*)GetCNode(i))->GetAngularVelocityLocal();

		EXUmath::MultVectorComponents(K6D, eps, res);
		//EXUmath::MultMatrixVectorTemplate(K6D, eps, res);
		EXUmath::MultMatrixTransposedVectorTemplate(TexpInv[i], res, res2);

		Vector3D resPos({ res2[0], res2[1], res2[2] }); //this is the residual on incremental positions
		Vector3D resRot({ res2[3], res2[4], res2[5] }); //this is the residual on incremental rotations

		ConstSizeVector< CNodeRigidBody::maxRotationCoordinates> resRotPar; //residual on rotation parameters
#ifndef useAverageRotation
		Matrix3D A = ((CNodeRigidBody*)GetCNode(i))->GetRotationMatrix();
#endif
		//EXUmath::ApplyTransformation33(A.GetTransposed(), G); //not correct; K6D is in local coordinates => K6D*Glocal gives global equations

		//EXUmath::MultMatrixTransposedVectorTemplate(G, A*resRot, resRotPar);
		EXUmath::MultMatrixTransposedVectorTemplate(Glocal, resRot, resRotPar);
		resPos = (A*resPos); //resPos is transformed from local into global coordinates

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//compute quadratic velocity vector per node:
		Vector3D temp = intFactL * omegaLocal.CrossProduct(parameters.physicsCrossSectionInertia * omegaLocal);


		ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> forcesQV; //forces acting on rotation coordinates
		EXUmath::MultMatrixTransposedVectorTemplate(Glocal, temp, forcesQV); //using Glocal because of omegaLocal

		Vector3D Glocal_tTheta_t(0.); //used twice!

		//additional term, if not Euler Parameters
		if ((((CNodeRigidBody*)GetCNode(i))->GetType() & Node::Type::RotationEulerParameters) == 0 &&
			(((CNodeRigidBody*)GetCNode(i))->GetType() & Node::Type::RotationRotationVector) == 0) //for Euler parameters or rotation vector, the following terms vanish
		{
			ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> Glocal_t; //store this term for case with COM!=0
			LinkedDataVector rot_t = ((CNodeRigidBody*)GetCNode(i))->GetRotationParameters_t();//store this term for case with COM!=0

			//compute: forces2 = Glocal^T * localInertia * Glocal_t * rot_t
			ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> forces2;
			Vector3D temp2;

			((CNodeRigidBody*)GetCNode(i))->GetGlocal_t(Glocal_t);
			EXUmath::MultMatrixVector(Glocal_t, rot_t, Glocal_tTheta_t); //Glocal_tTheta_t stored for later usage!
			EXUmath::MultMatrixVector(parameters.physicsCrossSectionInertia, Glocal_tTheta_t, temp2);

			EXUmath::MultMatrixTransposedVectorTemplate(Glocal, temp2, forces2);
			forcesQV += forces2;

		}
		resRotPar += forcesQV;
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		for (Index j = 0; j < nDim3D; j++)
		{
			ode2Lhs[j + i * nNode0] += resPos[j];
		}
		for (Index j = 0; j < resRotPar.NumberOfItems(); j++)
		{
			ode2Lhs[j + nDim3D + i * nNode0] += resRotPar[j];
		}
	}
	
}

//! Computational function: compute jacobian (dense or sparse mode, see parent CObject function)
void CObjectBeamGeometricallyExact::ComputeJacobianODE2_ODE2(EXUmath::MatrixContainer& jacobianODE2, JacobianTemp& temp, Real factorODE2, Real factorODE2_t,
	Index objectNumber, const ArrayIndex& ltg) const
{
	jacobianODE2.SetUseDenseMatrix(true);
	jacobianODE2.GetInternalDenseMatrix().SetNumberOfRowsAndColumns(ltg.NumberOfItems(), ltg.NumberOfItems());
	Index dimJacobian = ltg.NumberOfItems();
	//jacobianODE2.GetInternalDenseMatrix().SetScalarMatrix(dimJacobian, 0.);

	const Index nDim3D = 3;

    Index nNode0 = GetCNode(0)->GetNumberOfODE2Coordinates();

    //put this check into checkPreAssembleConsistencies.cpp
	CHECKandTHROW( (nNode0 + GetCNode(1)->GetNumberOfODE2Coordinates() == dimJacobian) &&
		(nNode0 <= (CNodeRigidBody::maxRotationCoordinates + nDim3D) ) &&
		(GetCNode(1)->GetNumberOfODE2Coordinates() <= (CNodeRigidBody::maxRotationCoordinates + nDim3D) ),
		"CObjectBeamGeometricallyExact::ComputeJacobianODE2_ODE2: nodal coordinates mismatch; the nodes cannot be used with this beam element");

	//const Index nDisplacementCoordinates = 3;
	//Index nNode0 = GetCNode(0)->GetNumberOfODE2Coordinates();
	Real L = parameters.physicsLength;
	//Real intFactL = 0.5*L; //integration weight, using 2 points Lobatto

	const CNodeRigidBody* node0 = (CNodeRigidBody*)GetCNode(0);
	const CNodeRigidBody* node1 = (CNodeRigidBody*)GetCNode(1);
	HomogeneousTransformation HT0(node0->GetRotationMatrix(), node0->GetPosition());
	HomogeneousTransformation HT1(node1->GetRotationMatrix(), node1->GetPosition());

	Vector3D incDisp;
	Vector3D incRot;
	HT0.GetRelativeMotionTo(HT1, incDisp, incRot);

	Matrix6D TexpInv[2];
	TexpInv[0] = -1.*EXUlie::TExpSE3Inv(-incDisp, -incRot);
	TexpInv[1] = EXUlie::TExpSE3Inv(incDisp, incRot);

#ifdef useAverageRotation
	Matrix3D A = ((CNodeRigidBody*)GetCNode(0))->GetRotationMatrix() * EXUlie::ExpSO3(0.5*incRot); //compute rotation at mid-span
#endif

	Matrix6D K6D(6, 6, 0.);
	K6D(0, 0) = parameters.physicsAxialShearStiffness[0];
	K6D(1, 1) = parameters.physicsAxialShearStiffness[1];
	K6D(2, 2) = parameters.physicsAxialShearStiffness[2];
	K6D(3, 3) = parameters.physicsTorsionalBendingStiffness[0];
	K6D(4, 4) = parameters.physicsTorsionalBendingStiffness[1];
	K6D(5, 5) = parameters.physicsTorsionalBendingStiffness[2];

	Real Linv = 1./L; //integration weight, for quadratic velocity vector (approximated!)
	K6D *= Linv* factorODE2;

	//inefficient approach using large matrices:
	ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> G;
	ConstSizeMatrix<CNodeRigidBody::maxRotationCoordinates * nDim3D> Glocal;
	ConstSizeMatrix<2*(CNodeRigidBody::maxRotationCoordinates + nDim3D) * (2 * nDim3D)> P;
	ConstSizeMatrix<2*(CNodeRigidBody::maxRotationCoordinates + nDim3D) * (2 * nDim3D)> KP;

	P.SetNumberOfRowsAndColumns(2 * nDim3D, dimJacobian);
	KP.SetNumberOfRowsAndColumns(2 * nDim3D, dimJacobian);

	for (Index i = 0; i < GetNumberOfNodes(); i++)
	{
		Index nDC = ((CNodeRigidBody*)GetCNode(i))->GetNumberOfDisplacementCoordinates(); //standard = 3
		//((CNodeRigidBody*)GetCNode(i))->GetGlocal(Glocal);
		((CNodeRigidBody*)GetCNode(i))->GetG(G);
		const Index maxSize = CNodeRigidBody::maxRotationCoordinates + CNodeRigidBody::maxDisplacementCoordinates;
		Index nCoords = ((CNodeRigidBody*)GetCNode(i))->GetNumberOfODE2Coordinates();

		ConstSizeMatrix< maxSize*maxSize> Tnew;
		ConstSizeMatrix< maxSize*maxSize> TnodeCoords(6, nCoords, 0.); //this is the mapping to node coordinates, if nodes are not Lie group nodes
#ifndef useAverageRotation
		Matrix3D A = ((CNodeRigidBody*)GetCNode(i))->GetRotationMatrix();
#endif
		TnodeCoords.SetSubmatrix(A.GetTransposed(), 0, 0);

		EXUmath::ApplyTransformation33(A.GetTransposed(), G); //Glocal but with averaged rotations
		TnodeCoords.SetSubmatrix(G, nDC, nDC);

		//!!!!!!!!! MISSING !!!!!!!!!!!
		//Glocal_q chain rule + RotT_q chain rule for position part

		EXUmath::MultMatrixMatrixTemplate(TexpInv[i], TnodeCoords, Tnew);
		P.SetSubmatrix(Tnew, 0, i*nNode0); //offset columns for second node

		EXUmath::MultMatrixMatrixTemplate(K6D, Tnew, TnodeCoords);
		KP.SetSubmatrix(TnodeCoords, 0, i*nNode0); //offset columns for second node
	}
	//derivative of Glocal not considered!!!
	EXUmath::MultMatrixTransposedMatrixTemplate(P, KP, jacobianODE2.GetInternalDenseMatrix()); //fill matrix directly into jacobian

	//pout << "**********\n";
	//pout << "K6D" << i << "=" << K6D << "\n";
	//pout << "TexpInv" << i << "=" << TexpInv[i] << "\n";
	//pout << "TnodeCoords" << i << "=" << TnodeCoords << "\n";
	//pout << "Klocal" << i << "=" << Klocal << "\n";

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//wrong: does not include node-node coupling:
	//for (Index i = 0; i < GetNumberOfNodes(); i++)
	//{
	//	Index nDC = ((CNodeRigidBody*)GetCNode(i))->GetNumberOfDisplacementCoordinates(); //standard = 3
	//	((CNodeRigidBody*)GetCNode(i))->GetGlocal(Glocal);
	//	Index nCoords = ((CNodeRigidBody*)GetCNode(i))->GetNumberOfODE2Coordinates();
	//	const Index maxSize = CNodeRigidBody::maxRotationCoordinates + CNodeRigidBody::maxDisplacementCoordinates;
	//	ConstSizeMatrix< maxSize*maxSize> Tnew;
	//	ConstSizeMatrix< maxSize*maxSize> TnodeCoords(6,nCoords, 0.); //this is the mapping to node coordinates, if nodes are not Lie group nodes
	//	TnodeCoords.SetSubmatrix(((CNodeRigidBody*)GetCNode(i))->GetRotationMatrix().GetTransposed(), 0, 0);
	//	//TnodeCoords.SetSubmatrix(((CNodeRigidBody*)GetCNode(i))->GetRotationMatrix(), 0, 0);
	//	TnodeCoords.SetSubmatrix(Glocal, nDC, nDC);

	//	EXUmath::MultMatrixMatrixTemplate(TexpInv[i], TnodeCoords, Tnew);

	//	ConstSizeMatrix< maxSize*maxSize> temp;
	//	ConstSizeMatrix< maxSize*maxSize> Klocal;
	//	EXUmath::MultMatrixTransposedMatrixTemplate(Tnew, K6D, temp);
	//	EXUmath::MultMatrixMatrixTemplate(temp, Tnew, Klocal);

	//	//pout << "**********\n";
	//	//pout << "K6D" << i << "=" << K6D << "\n";
	//	//pout << "TexpInv" << i << "=" << TexpInv[i] << "\n";
	//	//pout << "TnodeCoords" << i << "=" << TnodeCoords << "\n";
	//	//pout << "Klocal" << i << "=" << Klocal << "\n";

	//	jacobianODE2.GetInternalDenseMatrix().SetSubmatrix(Klocal, 
	//                                                     i*nCoords, i*nCoords, ==> this is wrong, should be: i*nNode0
	//                                                     1.);
	//}
	//pout << "jac" << "=" << jacobianODE2.GetEXUdenseMatrix() << "\n";
}


//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectBeamGeometricallyExact::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + 
		(Index)AccessFunctionType::AngularVelocity_qt +
		(Index)AccessFunctionType::JacobianTtimesVector_q+
		(Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in 'value' according to object access
void CObjectBeamGeometricallyExact::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	//Real L = parameters.physicsLength;
	CHECKandTHROWstring("CObjectBeamGeometricallyExact::GetAccessFunctionBody (for MarkerBody): NOT implemented yet; Markers can be only attached to nodes!");

	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate
		Vector2D SV = ComputeShapeFunctions(x);
		value.SetNumberOfRowsAndColumns(3, GetODE2Size()); //3D velocity, 6 coordinates qt

		value.SetAll(0.);
		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 3) = SV[1];
		value(1, 4) = SV[1];

		CHECKandTHROW(localPosition[1] == 0, "CObjectBeamGeometricallyExact::GetAccessFunctionBody (for MarkerBody): only implemented if localPosition[1]==0");

		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//const Index ns = 2;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate

		value.SetNumberOfRowsAndColumns(3, GetODE2Size()); //3D velocity, 6 coordinates qt
		value.SetAll(0.); //last row not necessary to set to zero ... 
		
		Vector2D SV = ComputeShapeFunctions(x);
		value(2, 2) = SV[0];
		value(2, 5) = SV[1];

		break;
	}
	case AccessFunctionType::JacobianTtimesVector_q: //jacobian w.r.t. global position and global orientation!!!
	{
		CHECKandTHROW(localPosition[1] == 0, "CObjectBeamGeometricallyExact::GetAccessFunctionBody [JacobianTtimesVector_q] (for MarkerBody): only implemented if localPosition[1]==0");
		value.SetNumberOfRowsAndColumns(0, 0); //indicates that all entries are zero
		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		value.SetNumberOfRowsAndColumns(3, GetODE2Size()); //3D velocity, 6 coordinates qt
		value.SetAll(0.);

		Real L = parameters.physicsLength;
		Real rhoA = parameters.physicsMassPerLength;

		Vector2D SV = rhoA * L * ComputeShapeFunctions(0.); //0=midpoint at axis

		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 3) = SV[1];
		value(1, 4) = SV[1];
		break;
	}
	default:
		SysError("CObjectBeamGeometricallyExact:GetAccessFunctionBody illegal accessType");
	}

}

//! provide according output variable in 'value'
void CObjectBeamGeometricallyExact::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value, Index objectNumber) const
{
	//CHECKandTHROWstring("CObjectBeamGeometricallyExact::GetOutputVariableBody not implemented!");

	switch (variableType)
	{
	case OutputVariableType::Position:	value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity:	value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	default:
		SysError("CObjectBeamGeometricallyExact::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//! use matrix logarithm to compute relative frame position
HomogeneousTransformation CObjectBeamGeometricallyExact::GetLocalPositionFrame(const Vector3D& localPosition, 
	ConfigurationType configuration) const
{
	Vector2D SV = ComputeShapeFunctions(localPosition[0]); //SV[0] goes from 0 to 1

	const CNodeRigidBody* node0 = (CNodeRigidBody*)GetCNode(0);
	const CNodeRigidBody* node1 = (CNodeRigidBody*)GetCNode(1);
	HomogeneousTransformation HT0(node0->GetRotationMatrix(configuration), node0->GetPosition(configuration));
	HomogeneousTransformation HT1(node1->GetRotationMatrix(configuration), node1->GetPosition(configuration));

	Vector3D incDisp;
	Vector3D incRot;
	HT0.GetRelativeMotionTo(HT1, incDisp, incRot);

	//compute incremental rotation/displacement at some intermediate position:
	incDisp *= SV[0];
	incRot *= SV[0];

	//HT at local position x:
	return HT0 * EXUlie::ExpSE3(incDisp, incRot);
}



//! return the (global) position of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	HomogeneousTransformation HTlocalPosition = GetLocalPositionFrame(localPosition, configuration);

	Vector3D p = HTlocalPosition.GetTranslation();
	
	if (localPosition[1] != 0. || localPosition[2] != 0.)
	{
		p += HTlocalPosition.GetRotation() * Vector3D({0.,localPosition[1],localPosition[2]});
	}

	return p;

}

//! return the (global) displacemnet of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference);
}

//! return the (global) velocity of 'localPosition' according to configuration type
Vector3D CObjectBeamGeometricallyExact::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Vector2D SV = ComputeShapeFunctions(localPosition[0]); //SV[0] goes from 0 to 1

	//Matrix3D A[2];
	Vector3D vel[2];
	Vector3D omegaLoc[2];
	for (Index i = 0; i < 2; i++)
	{
		const CNodeRigidBody* node = (CNodeRigidBody*)GetCNode(i);
		omegaLoc[i] = node->GetAngularVelocityLocal(configuration);
		vel[i] = node->GetVelocity(configuration);
	}

	Vector3D v = MapVectors(SV, vel[0], vel[1]); //interpolated at midline, not consistent with position ...

	//add velocity due to cross section rotation:
	Vector3D pCS({ 0.,localPosition[1],localPosition[2] });
	v += GetLocalPositionFrame(localPosition, configuration).GetRotation()*
		MapVectors(SV, omegaLoc[0], omegaLoc[1]).CrossProduct(pCS);

	return v;
}

//! return configuration dependent rotation matrix of node; returns always a 3D Matrix, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Matrix3D CObjectBeamGeometricallyExact::GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return 	GetLocalPositionFrame(localPosition, configuration).GetRotation();
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector, independent of 2D or 3D object; for rigid bodies, the argument localPosition has no effect
Vector3D CObjectBeamGeometricallyExact::GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Vector2D SV = ComputeShapeFunctions(localPosition[0]); //SV[0] goes from 0 to 1

	Vector3D omegaLoc[2];
	for (Index i = 0; i < 2; i++)
	{
		omegaLoc[i] = ((CNodeRigidBody*)GetCNode(i))->GetAngularVelocity(configuration);
	}

	return MapVectors(SV, omegaLoc[0], omegaLoc[1]); //interpolated
}





