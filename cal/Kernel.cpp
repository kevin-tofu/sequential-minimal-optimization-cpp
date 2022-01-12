/// <summary>
/// class : pca
/// name  : “nç³ _•½
/// Date  : 2010/12/14
/// 
/// —š—ğ
///
/// </summary>
//#include "../stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cal.h"
#include "../inc/Kernel.h"


_stKernel* fKernel_New(int type);
int fKernel_Initialize(_stKernel* This, int type);
int fKernel_Delete(_stKernel* This);
double fKernel_Main(_stKernel* This, _stMatrix* mat1, _stMatrix* mat2);
int fKernel_NonlinearTransform(_stKernel* This, _stMatrix* Input);

_stKernel* fKernel_New(int type)
{
	_stKernel* retAddress = (_stKernel*)malloc(sizeof(_stKernel));

	fKernel_Initialize(retAddress, type);

	retAddress->vOmega = fMat_New(retAddress->vDim_kernel, 1);
	retAddress->vOmega0 = 0;

	fMat_Zero(retAddress->vOmega);

	
	return retAddress;
}

/*!
‰Šú‰»
@param    
@return   
@note     
*/
int fKernel_Initialize(_stKernel* This, int type)
{
	int lErrorCounter = 0;

	This->vType = type;

	This->vDim_measure = 2;
	This->vDim_kernel = 2;
	

	return lErrorCounter;
}

/*!
—Ìˆæíœ
@param
@return
@note
*/
int fKernel_Delete(_stKernel* This)
{
	fMat_Delete(This->vOmega);

	free(This);

	return 0;
}

/*!

@param
@return
@note		k(x,y)
*/
double fKernel_Main(_stKernel* This, _stMatrix* mat1, _stMatrix* mat2)
{
	double ret = 0;
	int lErrorCounter = 0;
	int lLoop1;//, lLoop2;

	if((mat1->row = mat2->row)
	 &&(mat1->row = This->vDim_measure)
	 &&(mat1->column = 1)
	 &&(mat2->column = 1)
	 //&&(true) 
		)
	{
		ret = 0;
		//type‚²‚Æ‚ÉˆÙ‚È‚é
		for(lLoop1 = 0; lLoop1 <  This->vDim_measure; lLoop1++)
		{
			ret += fMat(mat1, lLoop1, 0) * fMat(mat2, lLoop1, 0);
		}

		//ret = fMat_InnerProduct(mat1, mat2);
		
	}
	

	return ret;
}


/*!
”ñüŒ`•ÏŠ·
@param
@return
@note		ƒÓ(x)
*/
//_stMatrix* fKernel_NonlinearTransform(_stMatrix* This, stMatrix* Input)
int fKernel_NonlinearTransform(_stKernel* This, _stMatrix* Input)
{
	int lErrorCounter = 0;
	int judge = 0;
	int ret = 0;
	
	double func = 0;

	func = fMat_InnerProduct(This->vOmega, Input);

	func += This->vOmega0;

	if(func >= 1)
	{
		ret = 1;
	}
	else
	if(func <= -1)
	{
		ret = -1;
	}


	return ret;
}