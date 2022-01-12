#pragma once
using namespace System;
#include "matrix.h"
#include "../inc/Kernel.h"

ref class SVM_Learn
{
public:
	SVM_Learn(int KernelType, int MeasureSize);
	~SVM_Learn(void);
	int mLearn(void);
	int Set_Answer(int DataNumber, int ans);
	int Set_Data(int DataNumber, int DimNumber, double data);

	_stKernel*  oKernel;
private:
	double mF_lambda(void);
	double mF_lambda_const(int select1, int select2);
	double mFi(int select);

	
	int         vMeasureSize;

	double      vC;

	_stMatrix*     vH;
	_stMatrix*     vLambda;

	
	_stMatrix*     vMeasure;
	array<int,1>^  vY;
	
};
