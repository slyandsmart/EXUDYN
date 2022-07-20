/** ***********************************************************************************************
* @brief        CObjectALEANCFCable2D implementation
*
* @author       Gerstmayr Johannes
* @date         2019-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectALEANCFCable2D.h"

//! Computational function: compute mass matrix
void CObjectALEANCFCable2D::PreComputeMassTerms() const
{
	const int ns = 4; //number of shape functions
	CObjectANCFCable2DBase::PreComputeMassTerms(); //compute massMatrix, but only in first step

	Real L = GetParameters().physicsLength;
	Real rhoAmoving = GetParameters().physicsMassPerLength * GetParameters().physicsMovingMassFactor; //take only moving part here!

	if (!massTermsALEComputed)
	{
		preComputedM1.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
		preComputedM2.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
		preComputedB1.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
		preComputedB2.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix

		Vector4D SV0 = ComputeShapeFunctions(0, L);
		Vector4D SVL = ComputeShapeFunctions(L, L);
		Vector4D SV0_x = ComputeShapeFunctions_x(0, L);
		Vector4D SVL_x = ComputeShapeFunctions_x(L, L);

		for (Index i = 0; i < ns; i++)
		{
			for (Index j = 0; j < ns; j++)
			{
				//compute B1=B' and B2=B'' terms: B'=[S^T*S']_0^L, B'' = [S'^T*S']_0^L
				preComputedB1(i * 2, j * 2) += rhoAmoving * (SVL[i] * SVL_x[j] - SV0[i] * SV0_x[j]);
				preComputedB1(i * 2 + 1, j * 2 + 1) += rhoAmoving * (SVL[i] * SVL_x[j] - SV0[i] * SV0_x[j]);
				preComputedB2(i * 2, j * 2) += rhoAmoving * (SVL_x[i] * SVL_x[j] - SV0_x[i] * SV0_x[j]);
				preComputedB2(i * 2 + 1, j * 2 + 1) += rhoAmoving * (SVL_x[i] * SVL_x[j] - SV0_x[i] * SV0_x[j]);
			}
		}

		//compute M1=M' and M2=M'' terms:
		Real a = 0; //integration interval [a,b]
		Real b = L;
		Index cnt = 0;
		for (auto item : EXUmath::gaussRuleOrder7Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector4D SV = ComputeShapeFunctions(x, L);
			Vector4D SV_x = ComputeShapeFunctions_x(x, L);
			Vector4D SVint_x = SV_x;

			SVint_x *= rhoAmoving * (0.5*(b - a)*EXUmath::gaussRuleOrder7Weights[cnt]);
			cnt++;

			for (Index i = 0; i < ns; i++)
			{
				for (Index j = 0; j < ns; j++)
				{
					//M'=M1=int(rhoA * S^T*S'), M''=int(rhoA * S'^T*S')
					preComputedM1(i * 2, j * 2) += SV[i] * SVint_x[j];
					preComputedM1(i * 2 + 1, j * 2 + 1) += SV[i] * SVint_x[j];
					preComputedM2(i * 2, j * 2) += SV_x[i] * SVint_x[j];
					preComputedM2(i * 2 + 1, j * 2 + 1) += SV_x[i] * SVint_x[j];
				}
			}
		}
		massTermsALEComputed = true;
	}

}

//! Computational function: compute mass matrix
void CObjectALEANCFCable2D::ComputeMassMatrix(EXUmath::MatrixContainer& massMatrixC, const ArrayIndex& ltg, Index objectNumber) const
{
	Matrix& massMatrix = massMatrixC.GetInternalDenseMatrix();
	PreComputeMassTerms();
	const int ns = 4; //number of shape functions

	//if (!massMatrixComputed)
	//{
	//	CObjectANCFCable2DBase::ComputeMassMatrix(massMatrix, objectNumber); //also fills in massMatrix, but only in first step
	//	//==>moves result into 'precomputedMassMatrix'
	//}

	Real L = GetParameters().physicsLength;
	Real rhoAmoving = GetParameters().physicsMassPerLength * GetParameters().physicsMovingMassFactor; //take only moving part here!

	//if (!massTermsALEComputed)
	//{
	//	preComputedM1.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
	//	preComputedM2.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
	//	preComputedB1.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
	//	preComputedB2.SetScalarMatrix(nODE2coordinates, 0.); //set 8x8 matrix
	//	
	//	Vector4D SV0 = ComputeShapeFunctions(0, L);
	//	Vector4D SVL = ComputeShapeFunctions(L, L);
	//	Vector4D SV0_x = ComputeShapeFunctions_x(0, L);
	//	Vector4D SVL_x = ComputeShapeFunctions_x(L, L);

	//	for (Index i = 0; i < ns; i++)
	//	{
	//		for (Index j = 0; j < ns; j++)
	//		{
	//			//compute B1=B' and B2=B'' terms: B'=[S^T*S']_0^L, B'' = [S'^T*S']_0^L
	//			preComputedB1(i * 2, j * 2)			+= rhoAmoving * (SVL[i] * SVL_x[j]	- SV0[i] * SV0_x[j]);
	//			preComputedB1(i * 2 + 1, j * 2 + 1) += rhoAmoving * (SVL[i] * SVL_x[j]	- SV0[i] * SV0_x[j]);
	//			preComputedB2(i * 2, j * 2)			+= rhoAmoving * (SVL_x[i] * SVL_x[j]- SV0_x[i] * SV0_x[j]);
	//			preComputedB2(i * 2 + 1, j * 2 + 1) += rhoAmoving * (SVL_x[i] * SVL_x[j]- SV0_x[i] * SV0_x[j]);
	//		}
	//	}

	//	//compute M1=M' and M2=M'' terms:
	//	Real a = 0; //integration interval [a,b]
	//	Real b = L;
	//	Index cnt = 0;
	//	for (auto item : EXUmath::gaussRuleOrder7Points)
	//	{
	//		Real x = 0.5*(b - a)*item + 0.5*(b + a);
	//		Vector4D SV = ComputeShapeFunctions(x, L);
	//		Vector4D SV_x = ComputeShapeFunctions_x(x, L);
	//		Vector4D SVint_x = SV_x;

	//		SVint_x *= rhoAmoving * (0.5*(b - a)*EXUmath::gaussRuleOrder7Weights[cnt]);
	//		cnt++;

	//		for (Index i = 0; i < ns; i++)
	//		{
	//			for (Index j = 0; j < ns; j++)
	//			{
	//				//M'=M1=int(rhoA * S^T*S'), M''=int(rhoA * S'^T*S')
	//				preComputedM1(i * 2, j * 2)         += SV[i] * SVint_x[j];
	//				preComputedM1(i * 2 + 1, j * 2 + 1) += SV[i] * SVint_x[j];
	//				preComputedM2(i * 2, j * 2)         += SV_x[i] * SVint_x[j];
	//				preComputedM2(i * 2 + 1, j * 2 + 1) += SV_x[i] * SVint_x[j];
	//			}
	//		}
	//	}
	//	massTermsALEComputed = true;
	//}

	//pout << "preComputedM1=" << preComputedM1 << "\n";
	//pout << "preComputedM2=" << preComputedM2 << "\n";
	//pout << "preComputedB1=" << preComputedB1 << "\n";
	//pout << "preComputedB2=" << preComputedB2 << "\n";


	//compute ALE mass terms:
	//++++++++++++++++++++++++++++++
	//Term M'*q:
	massMatrix.SetNumberOfRowsAndColumns(nODE2coordinates + 1, nODE2coordinates + 1);
	massMatrix.SetSubmatrix(precomputedMassMatrix); //set 8x8 matrix at (0,0)
	//pout << "Mass1=" << precomputedMassMatrix << "\n";

	ConstSizeVector<nODE2coordinates> mq(nODE2coordinates);    //M'*qANCF
	ConstSizeVector<nODE2coordinates> qANCF(nODE2coordinates); //element coordinates

	ComputeCurrentObjectCoordinates(qANCF);
	//pout << "qANCF=" << qANCF << "\n";

	EXUmath::MultMatrixVector(preComputedM1, qANCF, mq);
	//pout << "mq=" << mq << "\n";

	//fill in Term m and m^T into last row/last column of mass matrix:
	if (parameters.physicsUseCouplingTerms)
	{
		for (Index i = 0; i < 2 * ns; i++)
		{
			massMatrix(i, nODE2coordinates) = mq[i];//fill last (9th) column of matrix
			massMatrix(nODE2coordinates, i) = mq[i];	//fill last (9th) row of matrix
		}
	}
	else
	{
		for (Index i = 0; i < 2 * ns; i++)
		{
			massMatrix(i, nODE2coordinates) = 0;//fill last (9th) column of matrix
			massMatrix(nODE2coordinates, i) = 0;	//fill last (9th) row of matrix
		}
	}


	//++++++++++++++++++++++++++++++
	//Term \mu:
	EXUmath::MultMatrixVector(preComputedM2, qANCF, mq); //M''*q
	if (1 || parameters.physicsUseCouplingTerms)
	{
		//this term is approximately the mass of the cable parameters.physicsLength*parameters.physicsMassPerLength:
		massMatrix(nODE2coordinates, nODE2coordinates) = qANCF * mq; //mu = q^T*M''*q; check if this does any Vector conversion
	}
	else
	{
		massMatrix(nODE2coordinates, nODE2coordinates) = rhoAmoving * L;
	}
	//pout << "mu  =" << qANCF * mq << "\n";
	//pout << "mass=" << parameters.physicsLength*parameters.physicsMassPerLength << "\n";

	//pout << "Mass =" << massMatrix << "\n";
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
void CObjectALEANCFCable2D::ComputeODE2LHS(Vector& ode2Lhs, Index objectNumber) const
{
	PreComputeMassTerms();

	//old code, which failes for LinkedDataMatrix, because ResizeMatrix()
	//if (!massTermsALEComputed)
	//{
	//	//must set already correct size of LinkedDataMatrix here:
	//	ConstSizeMatrix< (nODE2coordinates + 1)*(nODE2coordinates + 1)> temp(nODE2coordinates + 1, nODE2coordinates + 1);
	//	//Matrix temp;
	//	LinkedDataMatrix linkedTemp(temp);
	//	ComputeMassMatrix(linkedTemp, objectNumber); //temp matrix is not used
	//	//now preComputedM,M1,M2,B1 and B2 matrices are available
	//}

	ode2Lhs.SetNumberOfItems(nODE2coordinates + 1);
	//LinkedDataVector ode2LhsANCF(ode2Lhs, 0, nODE2coordinates);


	//ConstSizeVector<nODE2coordinates +1> qANCF;
	//ConstSizeVector<nODE2coordinates +1> qANCF_t;
	//ComputeCurrentObjectCoordinates(qANCFALE);
	//ComputeCurrentObjectVelocities(qANCFALE_t);
	//ComputeODE2LHStemplate<Real>(ode2Lhs, qANCFALE, qANCFALE_t);
	CObjectANCFCable2DBase::ComputeODE2LHS(ode2Lhs, objectNumber); //compute stiffness terms
	
	//ode2Lhs.SetNumberOfItems(nODE2coordinates + 1);
	//ode2Lhs.CopyFrom(temp, 0, 0, nODE2coordinates); //copies 8 terms
	
	Real vALE = ((CNodeODE2*)GetCNode(2))->GetCurrentCoordinate_t(0);

	ConstSizeVector<nODE2coordinates> qANCF;
	ConstSizeVector<nODE2coordinates> qANCF_t;
	ComputeCurrentObjectCoordinates(qANCF);
	ComputeCurrentObjectVelocities(qANCF_t);

	ConstSizeVector<nODE2coordinates> temp;
	//++++++++++++++++++++++++++++++
	//Term Q_vq_t (scalar): 2*v*\dot q^T M'' q + 0.5*v^2*q^T B'' q
	EXUmath::MultMatrixVector(preComputedM2, qANCF, temp);
	Real Qvqt = 2.*vALE*(qANCF_t*temp);

	EXUmath::MultMatrixVector(preComputedB2, qANCF, temp);
	Qvqt += 0.5*vALE*vALE*(qANCF*temp);
	//pout << "QV=" << Qvqt << "\n";

	ode2Lhs[nODE2coordinates] += Qvqt; //this term is not added, because it does not exist in standard ANCF

	//++++++++++++++++++++++++++++++
	//Term Q_q_tv (1x8 vector): 2*v*M' \dot q + v^2(B'-M'')q
	EXUmath::MultMatrixVector(preComputedM1, qANCF_t, temp);
	temp *= 2.*vALE;

	ConstSizeMatrix<nODE2coordinates*nODE2coordinates> mTemp(nODE2coordinates, nODE2coordinates);
	mTemp.CopyFrom(preComputedB1);
	mTemp -= preComputedM2; //check if data is allocated or copied!

	ConstSizeVector<nODE2coordinates> temp2;
	EXUmath::MultMatrixVector(mTemp, qANCF, temp2);
	temp2 *= vALE * vALE;

	//pout << "QA=" << temp + temp2 << "\n";

	//fill in all forces due to velocity term
	for (Index i = 0; i < nODE2coordinates; i++)
	{
		ode2Lhs[i] += temp[i] + temp2[i]; //these terms are added to conventional elastic forces
	}

}

////! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
//AccessFunctionType CObjectALEANCFCable2D::GetAccessFunctionTypes() const
//{
//	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
//}

#define CObjectALEANCFCable2D_USE_ALE_MASSTERM 1 //0 or 1
//! provide Jacobian at localPosition in "value" according to object access
void CObjectALEANCFCable2D::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	Real L = GetParameters().physicsLength;

	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 4;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate
		Vector4D SV = ComputeShapeFunctions(x, L);
		value.SetNumberOfRowsAndColumns(3, 8); //3D velocity, 8 coordinates qt

		////OLD:
		//value.SetAll(0.);
		//value(0, 0) = SV[0];
		//value(1, 1) = SV[0];
		//value(0, 2) = SV[1];
		//value(1, 3) = SV[1];
		//value(0, 4) = SV[2];
		//value(1, 5) = SV[2];
		//value(0, 6) = SV[3];
		//value(1, 7) = SV[3];

		if (localPosition[1] == 0)
		{
			value.SetAll(0.);
			value(0, 0) = SV[0];
			value(1, 1) = SV[0];
			value(0, 2) = SV[1];
			value(1, 3) = SV[1];
			value(0, 4) = SV[2];
			value(1, 5) = SV[2];
			value(0, 6) = SV[3];
			value(1, 7) = SV[3];
		}
		else
		{
			Real y = localPosition[1];
			Vector4D SV_x = ComputeShapeFunctions_x(x, L);
			Vector2D r_x = ComputeSlopeVector(x, ConfigurationType::Current);
			Real norm = r_x.GetL2Norm();
			Real normInv = 0;
			Vector2D n({ -r_x[1], r_x[0] });
			if (norm != 0.)
			{
				normInv = 1. / norm;
			}
			else
			{
				CHECKandTHROWstring("CObjectANCFCable2DBase::GetPosition(...): slope vector has length 0!");
			}
			n *= normInv;
			//p = r(localPosition[0]) + localPosition[1] * n; n=1/sqrt(rx^T*rx)*[-rx[1],rx[0]]
			//dp/dq = S + (ry^T*S_x)/(rx^T*rx) (3/2) * n + 1/sqrt(rx^T*rx) * S_x^perpendicular
			Real norm3 = norm * norm * norm; //could be SIMPLIFIED, because n also contains 1/norm ....

			//pout << "  slope=" << r_x << ", norm=" << norm << ", n=" << n << "\n";
			//
			for (Index i = 0; i < 4; i++)
			{
				Vector2D Svec[2]; //SV_x
				Svec[0] = Vector2D({ SV_x[i],0 });
				Svec[1] = Vector2D({ 0, SV_x[i] });
				Vector2D SvecP[2];
				SvecP[0] = Vector2D({ 0., SV_x[i] });
				SvecP[1] = Vector2D({ -SV_x[i], 0. });

				for (Index j = 0; j < 2; j++)
				{
					Real u = -y * (r_x*Svec[j]) / norm3;
					value(0, i * 2 + j) = u * n[0] + y * normInv * SvecP[j][0];
					value(1, i * 2 + j) = u * n[1] + y * normInv * SvecP[j][1];

					value(j, i * 2 + j) += SV[i];
				}

			}
		}



		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		const Index ns = 4;   //number of shape functions

		Real xLoc = localPosition[0]; //only x-coordinate
		Vector2D slope = ComputeSlopeVector(xLoc, ConfigurationType::Current);
		Real x = slope[0]; //x-slopex
		Real y = slope[1]; //y-slopex

		Vector4D SVx = ComputeShapeFunctions_x(xLoc, L);
		Real fact0 = -y / (x*x + y * y);
		Real fact1 = x / (x*x + y * y);

		value.SetNumberOfRowsAndColumns(3, 8);
		value.SetAll(0.); //last row not necessary to set to zero ... 
		for (Index i = 0; i < ns; i++)
		{
			value(2, i*2) = SVx[i] * fact0; //last row of jacobian
			value(2, i*2 + 1) = SVx[i] * fact1;
		}

		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 4;   //number of shape functions

		value.SetNumberOfRowsAndColumns(3, 8 + CObjectALEANCFCable2D_USE_ALE_MASSTERM); //3D velocity, 8 coordinates qt
		value.SetAll(0.);

		Real L = GetParameters().physicsLength;
		Real rhoA = GetParameters().physicsMassPerLength;

		Index cnt = 0;
		Real a = 0; //integration interval [a,b]
		Real b = L;

		Vector4D SV({ 0.,0.,0.,0. });
		
		for (auto item : EXUmath::gaussRuleOrder3Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector4D SVloc = ComputeShapeFunctions(x, L);
			SVloc *= rhoA * (0.5*(b - a)*EXUmath::gaussRuleOrder3Weights[cnt++]);
			SV += SVloc;
		}

		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 2) = SV[1];
		value(1, 3) = SV[1];
		value(0, 4) = SV[2];
		value(1, 5) = SV[2];
		value(0, 6) = SV[3];
		value(1, 7) = SV[3];

		//additional term that drives the ALE coordinate if subjected to gravity in axial direction
		if (CObjectALEANCFCable2D_USE_ALE_MASSTERM)
		{
			Vector2D SVale(0);
			Index cnt = 0;
			//perform numerical integration over rhoA * r'
			for (auto item : EXUmath::gaussRuleOrder3Points)
			{
				Real x = 0.5*(b - a)*item + 0.5*(b + a);
				Vector2D slope = ComputeSlopeVector(x, ConfigurationType::Current);
				slope *= rhoA * (0.5*(b - a)*EXUmath::gaussRuleOrder3Weights[cnt++]);
				SVale += slope;
			}
			value(0, 8) = SVale[0];
			value(1, 8) = SVale[1];
		}

		break;
	}
	default:
		SysError("CObjectALEANCFCable2D:GetAccessFunctionBody illegal accessType");
	}
}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectALEANCFCable2D::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Vector3D v = CObjectANCFCable2DBase::GetVelocity(localPosition, configuration); //velocity without Eulerian term

	Vector2D rx = ComputeSlopeVector(localPosition[0], configuration);
	Real vALE = ((CNodeODE2*)GetCNode(2))->GetCurrentCoordinateVector_t()[0];
	v[0] += vALE * rx[0]; //add Eulerian term
	v[1] += vALE * rx[1]; //add Eulerian term

	return v;
}


