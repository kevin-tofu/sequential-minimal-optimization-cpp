#pragma once

#include "../inc/matrix.h"

typedef struct Tag_stKernel
{
	int         vType;          //!<
	int         vDim_measure;   //!<
	int         vDim_kernel;    //!<
	
	_stMatrix*     vOmega;
	double         vOmega0;     //!<
	//_stMatrix*  vKernel;      //!<
	
	
}_stKernel;

extern _stKernel* fKernel_New(int type);
extern int fKernel_Initialize(_stKernel* This, int type);
extern int fKernel_Delete(_stKernel* This);
extern double fKernel_Main(_stKernel* This, _stMatrix* mat1, _stMatrix* mat2);
extern int fKernel_NonlinearTransform(_stKernel* This, _stMatrix* Input);
