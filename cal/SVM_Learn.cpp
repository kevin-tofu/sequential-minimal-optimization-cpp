//#include "../stdafx.h"
#include "../inc/SVM_Learn.h"

#include "../inc/cal.h"

//100000.0
#define def_KKTThreshold  0.00

/*!
@param   
@return   
@note     
*/
SVM_Learn::SVM_Learn(int KernelType, int MeasureSize)
{
	this->vY = gcnew array<int,1>(MeasureSize);

	this->oKernel = fKernel_New(KernelType);

	this->vMeasureSize = MeasureSize;

	this->vMeasure = fMat_New(this->oKernel->vDim_measure, MeasureSize);
	
	this->vH = fMat_New(MeasureSize, MeasureSize);
	this->vLambda = fMat_New(MeasureSize, 1);

	/**/
	for(int lLoop = 0; lLoop < MeasureSize; lLoop++)
	{
		fMat(this->vLambda, lLoop, 0) = 0.0;
	}

	this->vC = 1.0 * Math::Pow(10, 10);
}

/*!
@param   
@return   
@note     
*/
SVM_Learn::~SVM_Learn(void)
{
	fMat_Delete(this->vMeasure);
	fMat_Delete(this->vLambda);
	
	fMat_Delete(this->vH);

	fKernel_Delete(this->oKernel);

	
	//fMat_Delete(this->vOmega0);

}

/*!
@param   
@return   
@note     
*/
int SVM_Learn::Set_Answer(int DataNumber, int ans)
{
	int lErrorCounter = 0;

	this->vY[DataNumber] = ans;

	return lErrorCounter;
}

/*!
@param   
@return   
@note     
*/
int SVM_Learn::Set_Data(int DataNumber, int DimNumber, double data)
{
	int lErrorCounter = 0;

	//this->vMeasure = fMat_New(dimention, measure);
	fMat(this->vMeasure, DimNumber, DataNumber) = data;

	return lErrorCounter;
}

/*!
@param   
@return   
@note     
*/
double SVM_Learn::mF_lambda(void)
{
	double ret = 0;
	int lMeasureSize = this->vMeasureSize;
	
	//m1
	_stMatrix* mat1 = fMat_New(lMeasureSize, 1);

	//mm m1
	fMat_Mlt(mat1, this->vH, this->vLambda);

	//λtＨλ
	ret += fMat_InnerProduct(this->vLambda, mat1);

	//-1/2 λtＨλ
	ret *= -0.5;

	//λt 1
	for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
	{
		ret += fMat(this->vLambda, lLoop, 0);
	}

	fMat_Delete(mat1);

	return ret;
}

/*!
@param   
@return   
@note     
*/
double SVM_Learn::mF_lambda_const(int select1, int select2)
{
	int lMeasureSize = this->vMeasureSize;
	double ret = 0;
	double temp1 = 0;

	ret += fMat(this->vLambda, select1, 0);
	ret += fMat(this->vLambda, select2, 0);

	ret += -0.5 * fMat(this->vLambda, select1, 0) * fMat(this->vLambda, select1, 0) * fMat(this->vH, select1, select1);
	ret += -0.5 * fMat(this->vLambda, select2, 0) * fMat(this->vLambda, select2, 0) * fMat(this->vH, select2, select2);

	ret += - fMat(this->vLambda, select1, 0) * fMat(this->vLambda, select2, 0) * this->vY[select1] * this->vY[select2] * fMat(this->vH, select1, select2);

	temp1 = 0;

	for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
	{
		if((lLoop != select1)
	     ||(lLoop != select2)
		 )
		{
			temp1 += fMat(this->vLambda, lLoop, 0) * this->vY[lLoop] * fMat(this->vH, select1, lLoop);
		}
	}

	ret += -fMat(this->vLambda, select1, 0) * this->vY[select1] * temp1;

	temp1 = 0;

	for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
	{
		if((lLoop != select1)
	     ||(lLoop != select2)
		 )
		{
			temp1 += fMat(this->vLambda, lLoop, 0) * this->vY[lLoop] * fMat(this->vH, select2, lLoop);
		}
	}
	ret += -fMat(this->vLambda, select2, 0) * this->vY[select2] * temp1;

	return ret;
}



/*!
@param   
@return   
@note     
*/
double SVM_Learn::mFi(int select)
{
	double ret = 0;

	int lMeasureSize = this->vMeasureSize;

	ret = this->vY[select];

	for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
	{
		ret += -fMat(this->vLambda, lLoop, 0) * this->vY[lLoop] * fMat(this->vH, select, lLoop);
	}

	return ret;
}

/*!
@param   
@return   
@note     
*/
int SVM_Learn::mLearn()
{
	int lErrorCounter = 0;
	double lTemp1 = 0;
	double lTemp2 = 0;
	int lTemp3_int = 0;
	int lMeasureSize = this->vMeasureSize;
	
	//
	//カーネル行列の作成
	//
	_stMatrix* mat1 = fMat_New(this->oKernel->vDim_measure, 1);
	_stMatrix* mat2 = fMat_New(this->oKernel->vDim_measure, 1);
	_stMatrix* mat3 = fMat_New(this->oKernel->vDim_kernel, 1);

	array<double, 2>^ vHtest = gcnew array<double, 2>(lMeasureSize, lMeasureSize);
	array<double, 2>^ lMeasureTest = gcnew array<double, 2>(lMeasureSize, 2);

	//
	for(int rowLoop = 0; rowLoop < lMeasureSize; rowLoop++)//check
	{
		lMeasureTest[rowLoop,0] = fMat(this->vMeasure, 0, rowLoop);
		lMeasureTest[rowLoop,1] = fMat(this->vMeasure, 1, rowLoop);

		for(int columnLoop = 0; columnLoop < lMeasureSize; columnLoop++)
		{
			fMat_Copy_colVector(mat1, 0, this->vMeasure, rowLoop);
			fMat_Copy_colVector(mat2, 0, this->vMeasure, columnLoop);

			fMat(vH, rowLoop, columnLoop) = (double)this->vY[rowLoop] * (double)this->vY[columnLoop] * fKernel_Main(this->oKernel, mat1, mat2);
			vHtest[rowLoop,columnLoop] = fMat(vH, rowLoop, columnLoop);
		}
	}

	int lSelect1 = 0;
	int lSelect2 = 1;
	double lLambda_Max,lLambda_Min;
	double lDiff_Q;
	double lLambda_opt;
	double lLambda1_old;
	double lLambda2_old;

	double lFi;
	double b_up;
	double b_low;

	array<double,1>^  lE = gcnew array<double,1>(lMeasureSize);

	//int lCounter = -1;

	//vLambda探索ループ
	//KKT条件を大体満たすまで探索
	//
	while(true)//
	{

		//
		//vLambda更新
		//
		lLambda1_old = fMat(this->vLambda, lSelect1, 0);
		lLambda2_old = fMat(this->vLambda, lSelect2, 0);

		if(this->vY[lSelect1] * this->vY[lSelect2] == 1)
		{
			lLambda_Min = Math::Max(0.0, lLambda1_old + lLambda2_old - this->vC);

			lLambda_Max = Math::Min(this->vC, lLambda1_old + lLambda2_old);

		}
		else
		if(this->vY[lSelect1] * this->vY[lSelect2] == -1)
		{
			lLambda_Min = Math::Max(0.0, lLambda1_old - lLambda2_old);

			lLambda_Max = Math::Min(this->vC, this->vC + lLambda1_old - lLambda2_old);
		}
		else
		{
			lErrorCounter++;
		}

		//
		lDiff_Q = 2 * fMat(vH, lSelect1, lSelect2) - fMat(vH, lSelect1, lSelect1) - fMat(vH, lSelect2, lSelect2);

		if(lDiff_Q < 0)
		{
			lLambda_opt = 0;

			//y1
			lLambda_opt = this->vY[lSelect1];

			for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
			{
				lLambda_opt += -(double)this->vY[lLoop] * fMat(vH, lSelect1, lLoop) * fMat(vLambda, lLoop, 0);
			}

			lLambda_opt -= this->vY[lSelect2];

			for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
			{
				lLambda_opt += (double)this->vY[lLoop] * fMat(vH, lSelect2, lLoop) * fMat(vLambda, lLoop, 0);
			}

			lLambda_opt *= -this->vY[lSelect1];

			lLambda_opt /= lDiff_Q;

			lLambda_opt += lLambda1_old;

			
			if((lLambda_Min <= lLambda_opt)
			 &&(lLambda_opt <= lLambda_Max)
			 )
			{
				//範囲内
				;
				;
			}
			else
			{
				//範囲外

				//lLambda_optが範囲内に存在しない場合
				//範囲の両端で値が大きくなる方を選択
				fMat(vLambda, lSelect1, 0) = lLambda_Min;
				fMat(this->vLambda, lSelect2, 0) = - this->vY[lSelect1] * this->vY[lSelect2] * lLambda_Min
								           + this->vY[lSelect1] * this->vY[lSelect2] * lLambda1_old
								           + lLambda2_old;
				lTemp1 = this->mF_lambda();
				//lTemp1 = this->mF_lambda_const(lSelect1, lSelect2);
				fMat(vLambda, lSelect1, 0) = lLambda_Max;
				fMat(this->vLambda, lSelect2, 0) = - this->vY[lSelect1] * this->vY[lSelect2] * lLambda_Max
								           + this->vY[lSelect1] * this->vY[lSelect2] * lLambda1_old
								           + lLambda2_old;
				lTemp2 = this->mF_lambda();
				//lTemp2 = this->mF_lambda_const(lSelect1, lSelect2);

				if(lTemp1 > lTemp2)
				{
					lLambda_opt = lLambda_Min;
				}
				else
				{
					lLambda_opt = lLambda_Max;
				}

			}

			
		}
		else
		{
			//関数は下に凸、区間の両端どちらか
			fMat(vLambda, lSelect1, 0) = lLambda_Min;
			fMat(this->vLambda, lSelect2, 0) = - this->vY[lSelect1] * this->vY[lSelect2] * lLambda_Min
								           + this->vY[lSelect1] * this->vY[lSelect2] * lLambda1_old
								           + lLambda2_old;
			lTemp1 = this->mF_lambda();
			//lTemp1 = this->mF_lambda_const(lSelect1, lSelect2);
			fMat(vLambda, lSelect1, 0) = lLambda_Max;
			fMat(this->vLambda, lSelect2, 0) = - this->vY[lSelect1] * this->vY[lSelect2] * lLambda_Max
								           + this->vY[lSelect1] * this->vY[lSelect2] * lLambda1_old
								           + lLambda2_old;
			lTemp2 = this->mF_lambda();
			//lTemp2 = this->mF_lambda_const(lSelect1, lSelect2);

			if(lTemp1 > lTemp2)
			{
				lLambda_opt = lLambda_Min;
			}
			else
			{
				lLambda_opt = lLambda_Max;
			}
		}

		//
		//vLambda更新
		//
		fMat(this->vLambda, lSelect2, 0) = - this->vY[lSelect1] * this->vY[lSelect2] * lLambda_opt
								           + this->vY[lSelect1] * this->vY[lSelect2] * lLambda1_old
								           + lLambda2_old;

		fMat(vLambda, lSelect1, 0) = lLambda_opt;

#if 1
		//
		//終了条件
		//

		b_up = 1073741824;  //min Fi
		b_low = -1073741824;//max Fi

		for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
		{
			lFi = this->mFi(lLoop);

			if((0 < fMat(this->vLambda, lLoop, 0))
			 &&(fMat(this->vLambda, lLoop, 0) < this->vC)
			 )
			{
				//
				//I3
				//
				/**/
				if(lFi < b_up)
				{
					b_up = lFi;
					lSelect1 = lLoop;
				}
				if(b_low < lFi)
				{
					b_low = lFi;
					lSelect2 = lLoop;
				}
				
			}
			else
			if(((fMat(this->vLambda, lLoop, 0) == 0)&&(this->vY[lLoop] == -1))
			 ||((fMat(this->vLambda, lLoop, 0) == this->vC)&&(this->vY[lLoop] == 1))
			 )
			{
				//
				//I1
				//
				if(lFi < b_up)
				{
					b_up = lFi;
					lSelect1 = lLoop;
				}
			}
			else
			if(((fMat(this->vLambda, lLoop, 0) == 0)&&(this->vY[lLoop] == 1))
			 ||((fMat(this->vLambda, lLoop, 0) == this->vC)&&(this->vY[lLoop] == -1))
			 )
			{
				//
				//I2
				//
				if(b_low < lFi)
				{
					b_low = lFi;
					lSelect2 = lLoop;
				}
			}
		}//Loop

		//
		if(lSelect1 == lSelect2)
		{
			lErrorCounter++;

			/**/
			if(lSelect1 == 0)
			{
				lSelect2++;
			}
			else
			{
				lSelect2--;
			}

		}

		for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
		{
			lE[lLoop] = fMat(this->vLambda, lLoop, 0);
		}
		//
		if(b_low - b_up <= 2 * def_KKTThreshold)
		{
			break;
		}


#else
		//
		//パラメータ算出
		//

		//
		fMat_Zero(this->oKernel->vOmega);

		for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
		{
			//Set
			//要修正 非線形変換を使う
			fMat_Copy_colVector(mat3, 0, this->vMeasure, lLoop);

			//
			fMat_Multiplier2(mat3, fMat(vLambda, lLoop, 0) * (double)this->vY[lLoop]);

			//
			fMat_Add2(this->oKernel->vOmega, mat3);
		}

		//任意のmat3
		fMat_Copy_colVector(mat3, 0, this->vMeasure, 0);
		this->oKernel->vOmega0 = (double)this->vY[0] - fMat_InnerProduct(this->oKernel->vOmega, mat3);


		//
		//終了条件を満たすか
		//




		//lCounter = 0;

		for(int lLoop1 = 0; lLoop1 < lMeasureSize; lLoop1++)
		{
			lTemp1 = 0;
			fMat_Copy_colVector(mat1, 0, this->vMeasure, lLoop1);

			//λ = 0
			for(int lLoop2 = 0; lLoop2 < lMeasureSize; lLoop2++)
			{
				fMat_Copy_colVector(mat2, 0, this->vMeasure, lLoop2);

				lTemp1 += fMat(vLambda, lLoop2, 0) * this->vY[lLoop2] * fKernel_Main(this->oKernel, mat1, mat2);
			}

			lTemp1 -= this->vY[lLoop1];

			lTemp1 += this->oKernel->vOmega0;

			lE[lLoop1] = lTemp1;

			lTemp1 *= this->vY[lLoop1];
			

			if(fMat(vLambda, lLoop1, 0) == 0.0)
			{
				//λ=0

				if(lTemp1 >= -def_KKTThreshold)
				{
					//KKT - OK
				}
				else
				{
					lSelect1 = lLoop1;

					//lCounter++;
				}
			}
			else
			if((0 < fMat(this->vLambda, lLoop1, 0))
			&&(fMat(this->vLambda, lLoop1, 0) < this->vC)
				)
			{
				//0< λ < C

				if(Math::Abs(lTemp1) <= def_KKTThreshold)
				{
					//KKT - OK
					break;
				}
				else
				{
					lSelect1 = lLoop1;

					//lCounter++;
				}

			}
			else
			if(fMat(vLambda, lLoop1, 0) == this->vC)
			{
				//λ= C

				if(lTemp1 <= def_KKTThreshold)
				{
					//KKT - OK
				}
				else
				{
					lSelect1 = lLoop1;

					//SlCounter++;
				}
			}
			else
			{
				lErrorCounter++;
			}
		}

		//
		//変数の選択
		//

		//lSelect1の選択は一つ前にできてる

		lTemp1 = 0.0;
		lTemp2 = 0.0;
		lTemp3_int = 0;

		for(int lLoop1 = 0; lLoop1 < lMeasureSize; lLoop1++)
		{
			//
			lTemp2 = Math::Abs(lE[lSelect1] - lE[lLoop1]);

			if(lTemp2 > lTemp1)
			{
				//最大値更新
				lTemp1 = lTemp2;

				//最大値の配列番号
				lTemp3_int = lLoop1;
			}
		}

		lSelect2 = lTemp3_int;
#endif

	}//while(true)


	//
	//パラメータ算出
	//
	fMat_Zero(this->oKernel->vOmega);

	for(int lLoop = 0; lLoop < lMeasureSize; lLoop++)
	{
		//Set
		//this->vMeasure = fMat_New(this->oKernel->vDim_measure, MeasureSize);
		//要修正 非線形変換を使う
		lErrorCounter += fMat_Copy_colVector(mat3, 0, this->vMeasure, lLoop);

		//
		fMat_Multiplier2(mat3, fMat(vLambda, lLoop, 0) * (double)this->vY[lLoop]);

		//
		fMat_Add2(this->oKernel->vOmega, mat3);

		/*
		if(this->vLambda->Mat[lLoop][0] > 0)
		{
			lSelect1 = lLoop;
		}
		*/
	}

	//任意のmat3
	fMat_Copy_colVector(mat3, 0, this->vMeasure, 0);
	this->oKernel->vOmega0 = (double)this->vY[0] - fMat_InnerProduct(this->oKernel->vOmega, mat3);
	
	lTemp1 = fMat(this->oKernel->vOmega, 0, 0);
	lTemp2 = fMat(this->oKernel->vOmega, 1, 0);

	fMat_Delete(mat1);
	fMat_Delete(mat2);
	fMat_Delete(mat3);

	return lErrorCounter;
}