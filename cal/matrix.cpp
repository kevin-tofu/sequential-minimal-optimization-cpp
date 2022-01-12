/// <summary>
/// class : matrix
/// name  : 渡邉 浩平
/// Date  : 2010/12/14
/// 
/// 履歴
///
/// </summary>

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cal.h"

#include "../inc/matrix.h"


//入力の行列数が正しいかチェックする
#define def_InputCheck

//row    : 行
//column : 列 

//@note      要check
//は動作未確認

///
///Dynamically allocated
///
//new del
_stMatrix *fMat_New(int input_row, int input_col);
int fMat_Delete(_stMatrix* Mat);
_stMatrix* fMat_NewCopy(_stMatrix* This);
_stMatrix* fMat_NewCopy_rowVector(_stMatrix* This, int input_row);
_stMatrix* fMat_NewCopy_colVector(_stMatrix* This, int input_col);
matrix new_matrix(int nrow, int ncol);
void free_matrix(matrix a);
int fMat_Transpose2(_stMatrix* This);
int fMat_Mlt2(_stMatrix* A, _stMatrix* B);
int fMat_MltTrans2(_stMatrix* A, _stMatrix* B);
int fMat_InverseMatrix_Gauss2(_stMatrix* Input);
int fMat_TransMlt2(_stMatrix* A, _stMatrix* B);

//property
int fMat_Set(_stMatrix* This, int input_row, int input_col, def_ElementType_mat input);
def_ElementType_mat fMat_Get(_stMatrix* This, int input_row, int input_col);
def_ElementType_mat fMat_GetDiag(_stMatrix* This, int input_row);

//cal
int fMat_SetConst(_stMatrix* This, def_ElementType_mat input);
int fMat_Zero(_stMatrix* This);
int fMat_UnitMatrix(_stMatrix* This);
int fMat_Add(_stMatrix* C,_stMatrix* A,_stMatrix* B);
int fMat_Add2(_stMatrix* A,_stMatrix* B);
int fMat_Mlt(_stMatrix* C,_stMatrix* A,_stMatrix* B);
int fMat_MltTrans(_stMatrix* C,_stMatrix* A,_stMatrix* B);
int fMat_TransMlt(_stMatrix* C, _stMatrix* A, _stMatrix* B);
int fMat_Sub(_stMatrix* C,_stMatrix* A,_stMatrix* B);
int fMat_Sub2(_stMatrix* A,_stMatrix* B);
int fMat_UD_Degradation(_stMatrix* U, _stMatrix* D, _stMatrix* input);
int fMat_Multiplier(_stMatrix* output, _stMatrix* input, def_ElementType_mat Multiplier);
int fMat_Multiplier2(_stMatrix* This, def_ElementType_mat Multiplier);
int fMat_Multiplier_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row, def_ElementType_mat Multiplier);
int fMat_Multiplier_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col, def_ElementType_mat Multiplier);
int fMat_Multiplier2_rowVector(_stMatrix* This, int output_row, def_ElementType_mat Multiplier);
int fMat_Multiplier2_colVector(_stMatrix* This, int output_col, def_ElementType_mat Multiplier);
int fMat_Copy(_stMatrix* output, _stMatrix* input);
int fMat_Copy_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row);
int fMat_Copy_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col);
int fMat_Copy_colVector_TO_row(_stMatrix* output, int output_row, _stMatrix* input, int input_col);
int fMat_Copy_rowVector_TO_column(_stMatrix* output, int output_col, _stMatrix* input, int input_row);
int fMat_Add2_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row);
int fMat_Add2_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col);
int fMat_InverseMatrix_Gauss(_stMatrix* Output, _stMatrix* Input);
int fMat_CholeskyDecomposition(_stMatrix* Output, _stMatrix* Input);
int fMat_Transpose(_stMatrix* output, _stMatrix* input);
int fMat_Check_SymmetricMatrix(_stMatrix* A);
def_ElementType_mat fMat_InnerProduct(_stMatrix* A, _stMatrix* B);
int fMat_InnerMatrix(_stMatrix* C, _stMatrix* A, _stMatrix* B);
int fMat_GetMaxElement_NotDiag(_stMatrix* input, int* p, int* q, def_ElementType_mat* max);

//typedef def_ElementType_mat *vector, **matrix;



/*!
@param    
@param    
@return  
@note     行列領域の配列部分確保
@note     	//nrow:行、ncol:列
*/
matrix new_matrix(int nrow, int ncol)
{
	matrix a;

#ifdef def_ShortMemory
	a = (matrix)malloc(sizeof(def_ElementType_mat) * nrow * ncol);
#else
	int i;

	a = (matrix)malloc((nrow + 1) * sizeof(void *));//
	
	if (a == NULL) return NULL;  /* 記憶領域不足 */
	
	for (i = 0; i < nrow; i++)
	{
		a[i] = (vector)malloc(sizeof(def_ElementType_mat) * ncol);
		
		if (a[i] == NULL)
		{
			while (--i >= 0) free(a[i]);

			free(a);  return NULL;  /* 記憶領域不足 */
		}
	}

	a[nrow] = NULL;  /* 行の数を自動判断するための工夫 */
#endif
	return a;
}


/*!

@param    
@param    
@return  
@note     行列領域開放
*/
void free_matrix(matrix a)
{
	
#ifdef def_ShortMemory
	free(a);
#else
	matrix b;

	b = a;
	
	while (*b != NULL) free(*b++);

	free(a);
#endif
}


/*!

@param    
@param    
@return  
@note     行列領域確保
*/
_stMatrix* fMat_New(int input_row, int input_col)
{
	
	_stMatrix* retAddress =  (_stMatrix*)malloc(sizeof(_stMatrix));
	//arg set
	retAddress->row = input_row;
	retAddress->column = input_col;

	//
	matrix Mataddress = new_matrix(input_row, input_col);
	retAddress->Mat = Mataddress;
	//

	fMat_Zero(retAddress);

	return retAddress;
}


/*!

@param    
@param    
@return  
@note     行列領域削除
*/
int fMat_Delete(_stMatrix* Mat)
{

	free_matrix(Mat->Mat);

	free(Mat);

	return 0;
}




/*!

@param    
@param    
@return  
@note     行列領域割り当て
@note     未使用。バグあるかも
*/
//int fMat___dd(_stMatrix* Mat, double** point, double* valArg, int row, int col)
//{
//	int row_loop = 0;
//
//	Mat->column = row;
//	Mat->row = col;
//
//	Mat->Mat = point;
//
//	for(row_loop = 0; row_loop < row; row_loop++)
//	{
//		*(*point + row_loop) = valArg[row];
//	}
//
//
//	return 0;
//}

/*!

@param    
@param    
@return  
@note     行列コピー
@note     行列を複製（領域確保）し、値もコピーする。
*/
_stMatrix* fMat_NewCopy(_stMatrix* This)
{
	_stMatrix* retAddress;
	int row_len = This->row;
	int col_len = This->column;
	int row_loop = 0;
	int col_loop = 0;

	retAddress = fMat_New(row_len, col_len);

	for(row_loop = 0; row_loop < row_len; row_loop++)
	{
		for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			//copy value
			//retAddress, row_loop][col_loop] = This, row_loop][col_loop];
			fMat(retAddress, row_loop, col_loop) = fMat(This, row_loop, col_loop);
		}
	}

	return retAddress;
}


/*!

@param    _stMatrix*        : コピー元のアドレス
@param    int input_row    : コピーする行番号
@return  
@note     行列コピー
@note     新たな行列を作成（領域確保）し、指定した行のみコピーする。
*/
_stMatrix* fMat_NewCopy_rowVector(_stMatrix* This, int input_row)
{
	_stMatrix* retAddress;
	int row_len = This->row;
	int col_len = This->column;
	//int row_loop = 0;
	int col_loop = 0;

	//retAddress = fMat_New(row, col);
	retAddress = fMat_New(1, col_len);

	//for(row_loop = 0; row_loop < row; row_loop++)
	{
		for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			//retAddress, 0][col_loop] = This, input_row][col_loop];
			fMat(retAddress, 0, col_loop) = fMat(This, input_row, col_loop);
		}
	}

	return retAddress;
}

/*!

@param    _stMatrix*        : コピー元のアドレス
@param    int input_col    : コピーする列番号
@return  
@note     行列コピー
@note     新たな行列を作成（領域確保）し、指定した列のみコピーする。
*/
_stMatrix* fMat_NewCopy_colVector(_stMatrix* This, int input_col)
{
	_stMatrix* retAddress;
	int row_len = This->row;
	int col_len = This->column;
	int row_loop = 0;
	//int col_loop = 0;

	//retAddress = fMat_New(row, col);
	retAddress = fMat_New(row_len, 1);

	for(row_loop = 0; row_loop < row_len; row_loop++)
	{
		//for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			//retAddress, row_loop][0] = This, row_loop][input_col];
			fMat(retAddress, row_loop, 0) = fMat(This, row_loop, input_col);
		}
	}

	return retAddress;
}

/*!

@param    _stMatrix* output
@param    _stMatrix* input
@return  
@note     行列コピー
@note     output = input 行列値を複製する。行数、列数が同じでなければならない
*/
int fMat_Copy(_stMatrix* output, _stMatrix* input)
{
	int lErrorCounter = 0;

	int row_len = input->row;
	int col_len = input->column;
	int row_loop = 0;
	int col_loop = 0;

#ifdef def_InputCheck
	if((row_len == output->row)
	 &&(col_len == output->column)
	 )
#else
	if(true)
#endif
	{

		for(row_loop = 0; row_loop < row_len; row_loop++)
		{
			for(col_loop = 0; col_loop < col_len; col_loop++)
			{
				//output, row_loop][col_loop] = input, row_loop][col_loop];
				fMat(output, row_loop, col_loop) = fMat(input, row_loop, col_loop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@return  
@note     行列コピー。指定した行の値を指定した行に出力する。列数（行の要素数）が同じ必要がある。
@note     指定した行番号が出力先の行番号より小さい
*/
int fMat_Copy_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row_len = input->row;
	int col_len = input->column;
	//int row_loop = 0;
	int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(1, col);

	//for(row_loop = 0; row_loop < row; row_loop++)
#ifdef def_InputCheck
	if((col_len == output->column)
		&&(input_row < row_len)//指定した行番号が出力先の行番号より小さい
		&&(output_row < output->row)
		)
#else
	if(true)
#endif
	{
		for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			//output, output_row][col_loop] = input, input_row][col_loop];
			fMat(output, output_row, col_loop) = fMat(input, input_row, col_loop);
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@return  
@note     指定した行ベクトルを指定した行にAdd2
@note     列数（行の要素数）が同じ必要がある。
@note     指定した行番号が出力先の行番号より小さい
*/
int fMat_Add2_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row = input->row;
	int col = input->column;
	//int row_loop = 0;
	int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(1, col);

	//for(row_loop = 0; row_loop < row; row_loop++)
#ifdef def_InputCheck
	if((col == output->column)
		&&(input_row < row)
		&&(output_row < output->row)
		)
#else
	if(true)
#endif
	{
		for(col_loop = 0; col_loop < col; col_loop++)
		{
			fMat(output, output_row, col_loop) += fMat(input, input_row, col_loop);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@return  
@note     行列コピー。指定した列の値を指定した列に出力する。行数（列の要素数）が同じ必要がある。
@note     指定した列番号が出力先の列番号より小さい  
*/
int fMat_Copy_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row_len = input->row;
	int col_len = input->column;
	int row_loop = 0;
	//int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(row, 1);

#ifdef def_InputCheck
	if((row_len == output->row)
		&&(input_col < col_len)
		&&(output_col < output->column)
		)
#else
	if(true)
#endif
	for(row_loop = 0; row_loop < row_len; row_loop++)
	{
		//for(col_loop = 0; col_loop < col; col_loop++)
		{
			fMat(output, row_loop, output_col) = fMat(input, row_loop, input_col);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}


/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_col
@return  
@note     行列コピー。指定した列を指定した行に出力する。列の要素数（input）と行の要素数（output）が同じ必要がある。
@note     指定した列番号が出力先の列番号より小さい
@note     要check
*/
int fMat_Copy_colVector_TO_row(_stMatrix* output, int output_row, _stMatrix* input, int input_col)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row_len = output->row;
	int col_len = output->column;
	//int row_loop = 0;
	int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(row, 1);

#ifdef def_InputCheck
	if((input->row == output->column)
		&&(input_col < input->column)
		&&(output_row < output->row)
		)
#else
	if(true)
#endif
	//for(row_loop = 0; row_loop < row_len; row_loop++)
	{
		for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			fMat(output, output_row, col_loop) = fMat(input, col_loop, input_col);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_col
@return  
@note     行列コピー。指定した列を指定した行に出力する。列の要素数（input）と行の要素数（output）が同じ必要がある。
@note     指定した列番号が出力先の列番号より小さい
@note     要check
*/
int fMat_Copy_rowVector_TO_column(_stMatrix* output, int output_col, _stMatrix* input, int input_row)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row_len = output->row;
	int col_len = output->column;
	int row_loop = 0;
	//int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(row, 1);

#ifdef def_InputCheck
	if((input->column == output->row)
		&&(input_row < input->row)
		&&(output_col < output->column)
		)
#else
	if(true)
#endif
	for(row_loop = 0; row_loop < row_len; row_loop++)
	{
		//for(col_loop = 0; col_loop < col_len; col_loop++)
		{
			fMat(output, row_loop, output_col) = fMat(input, input_row, row_loop);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@return  
@note     指定した列ベクトルを指定した列にAdd2
@note     行数（列の要素数）が同じ必要がある。
@note     指定した列番号が出力先の列番号より小さい
*/
int fMat_Add2_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col)
{
	int lErrorCounter = 0;
	//_stMatrix* retAddress;
	int row = input->row;
	int col = input->column;
	int row_loop = 0;
	//int col_loop = 0;

	//retAddress = fMat_New(row, col);
	//retAddress = fMat_New(row, 1);
#ifdef def_InputCheck
	if((row == output->row)
		&&(input_col < col)
		&&(output_col < output->column)
		)
#else
	if(true)
#endif
	for(row_loop = 0; row_loop < row; row_loop++)
	{
		//for(col_loop = 0; col_loop < col; col_loop++)
		{
			fMat(output, row_loop, output_col) += fMat(input, row_loop, input_col);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* output
@param    _stMatrix* input
@param    def_ElementType_mat Multiplier
@return  
@note     inputに乗数Multiplierをかけた値をoutputに出力する
@note     A = kB
*/
int fMat_Multiplier(_stMatrix* output, _stMatrix* input, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;
	int rowLoopMax = output->row;
	int colLoopMax = output->column;

#ifdef def_InputCheck
	if((input->row == output->row)
		&&(input->column == output->column)
		)
#else
	if(true)
#endif
	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(output, rowLoop, columnLoop) = fMat(input, rowLoop, columnLoop) * Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}

/*!

@param    _stMatrix* This
@param    def_ElementType_mat Multiplier
@return  
@note     Thisに乗数Multiplierをかけた値をThisに出力する
@note     A = kA
*/
int fMat_Multiplier2(_stMatrix* This, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;
	int rowLoopMax = This->row;
	int colLoopMax = This->column;
#ifdef def_InputCheck
	if(true)
#else
	if(true)
#endif
	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(This, rowLoop, columnLoop) *= Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}


/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@param    def_ElementType_mat Multiplier
@return  
@note     inputの指定した行ベクトルに乗数Multiplierをかけた値をoutputの指定した行ベクトルに出力する
@note     A = kA
*/
int fMat_Multiplier_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int columnLoop;//rowLoop,
	int rowLoopMax = output->row;
	int colLoopMax = output->column;

	//for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
#ifdef def_InputCheck
	if((input->column == output->column)
		&&(output_row < output->row)
		&&(input_row < input->row)
		)
#else
	if(true)
#endif
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(output, output_row, columnLoop) = fMat(input, input_row, columnLoop) * Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}


/*!

@param    _stMatrix* This
@param    def_ElementType_mat Multiplier
@return  
@note     Thisの指定した行ベクトルに乗数Multiplierをかけた値をThisに出力する
@note     A = kA
*/
int fMat_Multiplier2_rowVector(_stMatrix* This, int output_row, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int columnLoop;//rowLoop,
	int rowLoopMax = This->row;
	int colLoopMax = This->column;

	//for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
#ifdef def_InputCheck
	if((output_row < This->row)
		)
#else
	if(true)
#endif
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(This, output_row, columnLoop) *= Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}



/*!

@param    _stMatrix* output
@param    int output_row
@param    _stMatrix* input
@param    int input_row
@param    def_ElementType_mat Multiplier
@return  
@note     inputの指定した列ベクトルに乗数Multiplierをかけた値をoutputの指定した列ベクトルに出力する
@note     A = kA
*/
int fMat_Multiplier_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int rowLoop;//,columnLoop;
	int rowLoopMax = output->row;
	//int colLoopMax = output->column;

#ifdef def_InputCheck
	if((input->row == output->row)
		&&(output_col < output->column)
		&&(input_col < input->column)
		)
#else
	if(true)
#endif
	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		//for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(output, rowLoop, output_col) = fMat(input, rowLoop, input_col) * Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}


/*!

@param    _stMatrix* This
@param    def_ElementType_mat Multiplier
@return  
@note     Thisの指定した列ベクトルに乗数Multiplierをかけた値をThisに出力する
@note     A = kA
*/
int fMat_Multiplier2_colVector(_stMatrix* This, int output_col, def_ElementType_mat Multiplier)
{
	int lErrorCounter = 0;

	int rowLoop;//,columnLoop;
	int rowLoopMax = This->row;
	//int colLoopMax = output->column;

#ifdef def_InputCheck
	if((output_col < This->column)
		)
#else
	if(true)
#endif
	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		//for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(This, rowLoop, output_col) *= Multiplier;
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}


/*!

@param    _stMatrix* This
@return  
@note     指定した行列を0行列にする
@note     A = 0
*/
int fMat_Zero(_stMatrix* This)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;
	int rowLoopMax = This->row;
	int colLoopMax = This->column;

	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(This, rowLoop, columnLoop) = 0;
		}
	}

	return lErrorCounter;
}
/*!

@param    _stMatrix* This
@return  
@note     指定した行列の要素全てに定数を書き込む
@note     A = I
@note     正方行列である必要性をがある。（この制限はなくてもよいかもしれない）
*/
int fMat_SetConst(_stMatrix* This, def_ElementType_mat input)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;
	int rowLoopMax = This->row;
	int colLoopMax = This->column;

	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			fMat(This, rowLoop, columnLoop) = input;
		}
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* This
@return  
@note     指定した行列を単位行列にする
@note     A = I
@note     正方行列である必要性をがある。（この制限はなくてもよいかもしれない）
*/
int fMat_UnitMatrix(_stMatrix* This)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;
	int rowLoopMax = This->row;
	int colLoopMax = This->column;

#ifdef def_InputCheck
	if((rowLoopMax == colLoopMax)
		)
#else
	if(true)
#endif
	
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				if(rowLoop == columnLoop)
					fMat(This, rowLoop, columnLoop) = 1;
				else
					fMat(This, rowLoop, columnLoop) = 0;
			}
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

//領域の入れ替えを行わねばならないので面倒かも。
/*
int fMat_Trans(_stMatrix* This)
{


	return 1;
}
*/
/*
void fMat_Add_rou(matrix c,matrix a, matrix b,int rowLoopMax,int colLoopMax)
{
	int rowLoop,columnLoop;

	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			c[rowLoop][columnLoop] = a[rowLoop][columnLoop] + b[rowLoop][columnLoop];
		}
	}
}
*/


/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     行列の和を出力
@note     Add C = A + B
@note     ab  ab = ab
*/
int fMat_Add(_stMatrix* C,_stMatrix* A,_stMatrix* B)
{
	int lErrorCounter = 0;

	int rowLoop,columnLoop;

	int rowLoopMax = A->row;
	int colLoopMax = A->column;

#ifdef  def_InputCheck
	if((A->row == B->row)
		&&(A->row== C->row)
		&&(A->column == B->column)
		&&(A->column== C->column))
#else
	if(true)
#endif
	{
		//fMat_Add_rou(C->Mat,A->Mat,B->Mat,rowLoopMax,colLoopMax);
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(C, rowLoop, columnLoop) = fMat(A, rowLoop, columnLoop) + fMat(B, rowLoop, columnLoop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}



/*!

@param    _stMatrix* A
@param    _stMatrix* B
@return  
@note     行列の和を出力
@note     Add A += B
*/
int fMat_Add2(_stMatrix* A,_stMatrix* B)
{
	int rowLoop,columnLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;
	int colLoopMax = A->column;

	
#ifdef  def_InputCheck
	if((A->row == B->row)
		&&(A->column == B->column))
#else
	if(true)
#endif		
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(A, rowLoop, columnLoop) += fMat(B, rowLoop, columnLoop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}



/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     行列の差を出力
@note     Add C = A - B
@note     ab  ab = ab

*/
int fMat_Sub(_stMatrix* C,_stMatrix* A,_stMatrix* B)
{
	int rowLoop,columnLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;
	int colLoopMax = A->column;

#ifdef  def_InputCheck
	if((A->row == B->row)
		&&(A->row== C->row)
		&&(A->column == B->column)
		&&(A->column== C->column))
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(C, rowLoop, columnLoop) = fMat(A, rowLoop, columnLoop) - fMat(B, rowLoop, columnLoop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}
	return lErrorCounter;
}




/*!

@param    _stMatrix* A
@param    _stMatrix* B
@return  
@note     行列の差を出力
@note     Add A -= B
*/
int fMat_Sub2(_stMatrix* A,_stMatrix* B)
{
	int rowLoop,columnLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;
	int colLoopMax = A->column;

#ifdef  def_InputCheck
	if((A->row == B->row)
		&&(A->column == B->column))
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(A, rowLoop, columnLoop) -= fMat(B, rowLoop, columnLoop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!
Check_Symmetric Matrix
@param    _stMatrix* A
@return   error counter
@note     対称行列かどうかcheckする。
@note     対称行列でない場合、或いは正方行列でない場合にはエラーS
*/
int fMat_Check_SymmetricMatrix(_stMatrix* A)
{
	int rowLoop,columnLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;
	int colLoopMax = A->column;
/*
	double out[4][4];

	for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
		{
			out[rowLoop][columnLoop] = A, rowLoop][columnLoop];
		}
	}
*/

#ifdef  def_InputCheck

	if((A->column == A->row))	
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < rowLoop + 1; columnLoop++)
			{
				if(fMat(A, rowLoop, columnLoop) != fMat(A, columnLoop, rowLoop))
				{
					lErrorCounter++;
				}
			}
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}

/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     行列の掛け算
@note     Mlt C = AB
@note     am * mb = ab
*/
int fMat_Mlt(_stMatrix* C,_stMatrix* A,_stMatrix* B)
{
	int rowLoop,columnLoop,multiLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;
	int colLoopMax = B->column;
	int mltLoopMax = A->column;

#ifdef  def_InputCheck

	if((A->column == B->row)
		&&(C->row == A->row)
		&&(C->column == B->column))	
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(C, rowLoop, columnLoop) = 0;

				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					fMat(C, rowLoop, columnLoop) += fMat(A, rowLoop, multiLoop) * fMat(B, multiLoop, columnLoop);
				}
			}
		}
	}
	else
	{
		lErrorCounter++;
	}


	return lErrorCounter;
}


/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     転置行列の掛け算
@note     Mlt C = ABt
@note     am * bm = ab
*/
int fMat_MltTrans(_stMatrix* C, _stMatrix* A, _stMatrix* B)
{
	int rowLoop,columnLoop,multiLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->row;//a
	int colLoopMax = B->row;//b
	int mltLoopMax = A->column;

#ifdef  def_InputCheck
	if((A->column == B->column)
		&&(C->row == A->row)
		&&(C->column == B->row))
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				//C, rowLoop][columnLoop] = 0;
				fMat(C, rowLoop, columnLoop) = 0;

				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					//C, rowLoop][columnLoop] += A, rowLoop][multiLoop] * B, columnLoop][multiLoop];
					fMat(C, rowLoop, columnLoop) += fMat(A, rowLoop, multiLoop) * fMat(B, columnLoop, multiLoop);
				}
			}
		}
	}
	else
	{
		lErrorCounter++;
	}
	

	return lErrorCounter;
}

/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     転置行列の掛け算
@note     Mlt C = AtB
@note     ma * mb = ab
@note     要check
*/
int fMat_TransMlt(_stMatrix* C, _stMatrix* A, _stMatrix* B)
{
	int rowLoop,columnLoop,multiLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->column;//a
	int colLoopMax = B->column;//b
	int mltLoopMax = A->row;

#ifdef  def_InputCheck
	if((A->row == B->row)
		&&(C->row == A->column)
		&&(C->column == B->column))
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				//C, rowLoop][columnLoop] = 0;
				fMat(C, rowLoop, columnLoop) = 0;

				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					//C, rowLoop][columnLoop] += A, multiLoop][rowLoop] * B, multiLoop][columnLoop];
					fMat(C, rowLoop, columnLoop) += fMat(A, multiLoop, rowLoop) * fMat(B, multiLoop, columnLoop);
				}
			}
		}
	}
	else
	{
		lErrorCounter++;
	}
	

	return lErrorCounter;
}

/*!

@param     _stMatrix* U
@param     _stMatrix* D
@param     _stMatrix* input
@return  
@note      UD分解 input → UD
@note      応用カルマンフィルタ p171
@note      対角行列作成版
*/
int fMat_UD_Degradation(_stMatrix* U, _stMatrix* D, _stMatrix* input)
{
	//Dはいらないかも
	//2version作成する？

	int lErrorCounter = 0;

	int row_input = input->row;
	int col_input = input->column;


	int i_loop = 0;
	int j_loop = 0;
	int k_loop = 0;

#ifdef  def_InputCheck	
	if((row_input == col_input)
		&&(row_input == U->row)
		&&(row_input == D->row)
		&&(col_input == U->column)
		&&(col_input == D->column)
		)
#else
	if(true)
#endif
	{
		//UD分解
		for(i_loop = row_input - 1; i_loop >= 0; i_loop--)
		{
			fMat(D, i_loop, i_loop) = fMat(input, i_loop, i_loop);

			fMat(U, i_loop, i_loop) = 1;

			for(j_loop = 0; j_loop < i_loop; j_loop++)
			{
				//U, j_loop][i_loop] = input, j_loop][i_loop] / D, i_loop][i_loop];
				fMat(U, j_loop, i_loop) = fMat(input, j_loop, i_loop) / fMat(D, i_loop, i_loop);

				for(k_loop = 0; k_loop <= j_loop; k_loop++)
				{
					//input, k_loop][j_loop] -= U, k_loop][i_loop] * D, i_loop][i_loop] * U, j_loop][i_loop];
					fMat(input, k_loop, j_loop) -= fMat(U, k_loop, i_loop) *fMat(D, i_loop, i_loop) * fMat(U, j_loop, i_loop);
				}
			}
		}

		//if(k == 0)
		//U, 0][0] = 1;
		//D, 0][0] = input, 0][0];
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* Output   :  A^-1
@param    _stMatrix* Input    :  A
@return   error数
@note     掃き出し法により逆行列を計算する。
@note     正則行列であるという条件を付け加えるべきかも
*/
int fMat_InverseMatrix_Gauss(_stMatrix* Output, _stMatrix* Input)
{
	int lErrorCounter = 0;
	int row_input = Input->row;
	int col_input = Input->column;

	int i_loop = 0;
	int j_loop = 0;
	int k_loop = 0;

	double lCalBuf = 0;

#ifdef  def_InputCheck	
	if((row_input == col_input)
		&&(row_input == Output->row)
		&&(row_input == Output->column)
		)
#else
	if(true)
#endif
	{
		//単位行列の作成
		fMat_UnitMatrix(Output);

		//
		for(i_loop = 0;i_loop < col_input;i_loop++)
		{
			//lCalBuf = 1 / Input, i_loop][i_loop];
			lCalBuf = 1 / fMat(Input, i_loop, i_loop);

			for(j_loop = 0;j_loop < col_input; j_loop++)
			{
				//Input, i_loop][j_loop] *= lCalBuf;
				fMat(Input, i_loop, j_loop) *= lCalBuf;

				//Output, i_loop][j_loop] *= lCalBuf;
				fMat(Output, i_loop, j_loop) *= lCalBuf;
			}

			for(j_loop = 0;j_loop < col_input;j_loop++)
			{
				if(i_loop != j_loop)
				{
					//lCalBuf = Input, j_loop][i_loop];
					lCalBuf = fMat(Input, j_loop, i_loop);

					for(k_loop = 0;k_loop < col_input;k_loop++)
					{
						//Input, j_loop][k_loop] -= Input, i_loop][k_loop] * lCalBuf;
						fMat(Input, j_loop, k_loop) -= fMat(Input, i_loop, k_loop) * lCalBuf;

						//Output, j_loop][k_loop] -= Output, i_loop][k_loop] * lCalBuf;
						fMat(Output, j_loop, k_loop) -= fMat(Output, i_loop, k_loop) * lCalBuf;
					}
				}
			}
		}

	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    _stMatrix* Output  : A^-1
@param    _stMatrix* Input   : A
@return   error数
@note     inputのコレスキー分解をoutputに出力する
@note	  関数内で入力行列inputが変化するので注意
@note	  関数内で平方根を用いる（Cの場合はMath.h、C++/CLIでnamespace Systemが必要
*/
int fMat_CholeskyDecomposition(_stMatrix* Output, _stMatrix* Input)
{
	int lErrorCounter = 0;
	int row_input = Input->row;
	int col_input = Input->column;

	int i_loop = 0;
	int j_loop = 0;
	int k_loop = 0;

	double lCalBuf = 0;

#ifdef  def_InputCheck	
	if((row_input == col_input)
		&&(row_input == Output->row)
		&&(row_input == Output->column)
		)
#else
	if(true)
#endif
	{
		fMat_Zero(Output);

		for(k_loop = (col_input - 1);k_loop > 0;k_loop--)
		{
			//(8.13式）
			//Output, k_loop][k_loop] = fCal_Sqrt(Input, k_loop][k_loop]);
			fMat(Output, k_loop, k_loop) = fCal_Sqrt(fMat(Input, k_loop, k_loop));

			//j_loopの使い方に注意(k_loopで既にとりうる値が狭めてある
			for(j_loop = 0;j_loop < (k_loop - 0); j_loop++)
			{
				//(8.14式）
				//Output, j_loop][k_loop] = Input, j_loop][k_loop] / Output, k_loop][k_loop];
				fMat(Output, j_loop, k_loop) = fMat(Input, j_loop, k_loop) / fMat(Output, k_loop, k_loop);

				//i_loopの使い方に注意
				for(i_loop = 0;i_loop <= j_loop;i_loop++)
				{
					//(8.15式）
					//Input, i_loop][j_loop] = Input, i_loop][j_loop] - Output, i_loop][k_loop] * Output, j_loop][k_loop];
					fMat(Input, i_loop, j_loop) = fMat(Input, i_loop, j_loop) - fMat(Output, i_loop, k_loop) * fMat(Output, j_loop, k_loop);
				}
			}
		}
		//(8.16式）
		fMat(Output, 0, 0) = fCal_Sqrt(fMat(Input, 0, 0));
		//Output, 0][0] = sqrt(Input, 0][0]);

	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}



/*!

@param    def_ElementType_mat input
@param    int input_row
@param    int input_col
@param    _stMatrix* This
@return  
@note     指定した行列に値を格納する。
*/
int fMat_Set(_stMatrix* This, int input_row, int input_col, def_ElementType_mat input)
{
	int lErrorCounter = 0;

#ifdef  def_InputCheck	
	if((input_row < This->row)
	 &&(input_col < This->column)
		)
#else
	if(true)
#endif
	{
		fMat(This, input_row, input_col) = input;
	}
	else
	{
		lErrorCounter++;
	}

	return lErrorCounter;
}

/*!

@param    int input_row
@param    int input_col
@param    _stMatrix* This
@return  
@note     指定した行列の要素を取得する
*/
def_ElementType_mat fMat_Get(_stMatrix* This, int input_row, int input_col)
{
	def_ElementType_mat ret = 0;

#ifdef  def_InputCheck	
	if((input_row < This->row)
	 &&(input_col < This->column)
		)
#else
	if(true)
#endif
	{
		ret = fMat(This, input_row, input_col);
	}
	else
	{
		ret = -1;
	}

	return ret;
}

/*!

@param    int input_row
@param    _stMatrix* This
@return  
@note     指定した行列の対角要素を取得する
*/
def_ElementType_mat fMat_GetDiag(_stMatrix* This, int input_row)
{
	def_ElementType_mat ret = 0;

#ifdef  def_InputCheck	
	if((input_row < This->row)
	 &&(input_row < This->column)
		)
#else
	if(true)
#endif
	{
		ret = fMat(This, input_row, input_row);
	}
	else
	{
		ret = -1;
	}


	return ret;
}
/*!
@param     _stMatrix* output
@param     _stMatrix* input
@return  
@note      転置行列
@note      A = Bt
@note      要check
*/
int fMat_Transpose(_stMatrix* output, _stMatrix* input)
{
	int rowLoop = 0;
	int columnLoop = 0;
	int lErrorCounter = 0;

	int row = output->row;
	int col = output->column;

	//_stMatrix* trans = fMat_New(col,row);//fMat_New(row,col);

#ifdef  def_InputCheck
	if((output->column == input->row)
		&&(output->column == input->row))	
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < row; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < col; columnLoop++)
			{
				//fMat_Set(This, rowLoop][columnLoop],columnLoop,rowLoop,trans);
				fMat(output, rowLoop, columnLoop) = fMat(input, columnLoop, rowLoop);
			}
		}
	}
	else
	{
		lErrorCounter++;
	}
	
	return lErrorCounter;
}


/*!
@param     _stMatrix* input
@param     int* p
@param     int* q
@return  
@note      対称行列非対角要素の最大要素（絶対値）を探索
@note      jacobi法に使用する
@note      (p,q)に要素番号を格納
@note      対称行列であれば工夫したほうがいいな
*/
int fMat_GetMaxElement_NotDiag(_stMatrix* input, int* p, int* q, def_ElementType_mat* max)
{
	int rowLoop = 0;
	int columnLoop = 0;
	int lErrorCounter = 0;

	int row = input->row;
	int col = input->column;

	int lp,lq;
	def_ElementType_mat lMax = 0;


#ifdef  def_InputCheck
	if((row == col)
	   //(row > 1)
		)	
#else
	if(true)
#endif
	{
		//下三角行列のループ
		for(rowLoop = 1; rowLoop < row; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < rowLoop; columnLoop++)
			{
				//
				if(fCal_Abs(fMat(input, rowLoop, columnLoop)) > lMax)
				{
					lMax = fCal_Abs(fMat(input, rowLoop, columnLoop));
					lp = rowLoop;
					lq = columnLoop;
				}
			}
		}

		*max = lMax;
		*p = lp;
		*q = lq;
	}
	else
	{
		lErrorCounter++;

		*max = lMax;
		*p = 0;
		*q = 0;
	}

	
	
	return lErrorCounter;
}


/*!
@param     _stMatrix* A
@param     _stMatrix* B
@return  
@note     内積計算
@note     (n,1)T (n,1)行列の内積
*/
def_ElementType_mat fMat_InnerProduct(_stMatrix* A, _stMatrix* B)
{
	def_ElementType_mat ret = (def_ElementType_mat)0.0;

	int rowLoop = 0;
	//int columnLoop = 0;
	int lErrorCounter = 0;

	int row = A->row;
	//int col = A->column;

	//_stMatrix* trans = fMat_New(col,row);//fMat_New(row,col);

#ifdef  def_InputCheck
	if((A->row == B->row)
     &&(A->column == 1)
	 &&(B->column == 1)
	 )
#else
	if(true)
#endif
	{
		for(rowLoop = 0; rowLoop < row; rowLoop++)
		{
			ret += fMat(A, rowLoop, 0) * fMat(B, rowLoop, 0);
		}
	}
	else
	{
		lErrorCounter++;
	}

	return ret;
}

/*!
@param     _stMatrix* C
@param     _stMatrix* A
@param     _stMatrix* B
@return  
@note     内積行列の計算
*/
int fMat_InnerMatrix(_stMatrix* C, _stMatrix* A, _stMatrix* B)
{
	int lErrorCounter = 0;

	lErrorCounter++;

	return lErrorCounter;
}

///*------------------------------------------------------------------*///
///注意：関数内で領域確保、開放を行っている
///*------------------------------------------------------------------*///

/*!

@param    _stMatrix* A
@param    _stMatrix* B
@return  
@note     行列の掛け算を出力
@note     MLT2 A = AB (am mb = ab)
@note     要check
*/
int fMat_Mlt2(_stMatrix* A, _stMatrix* B)
{
	int rowLoop = 0;
	int columnLoop = 0;
	int multiLoop = 0;
	int lErrorCounter = 0;

	int row = A->row;
	int col = B->column;
	int mltLoopMax = A->column;

	//_stMatrix* trans = fMat_New(col,row);//fMat_New(row,col);

#ifdef  def_InputCheck
	if((A->column == B->row)
		//&&(B->column == B->row)
		//&&(A->column == B->column)
		)	
#else
	if(true)
#endif
	{
		//領域確保
		_stMatrix* Mataddress = fMat_New(row, col);
		matrix lTemp_Mat;
	
		for(rowLoop = 0; rowLoop < row; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < col; columnLoop++)
			{
				fMat(Mataddress, rowLoop, columnLoop) = 0;
	
				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					fMat(Mataddress, rowLoop, columnLoop) += fMat(A, rowLoop, multiLoop) * fMat(B, multiLoop, columnLoop);
				}
			}
		}
		//仮置き
		lTemp_Mat = A->Mat;

		//書き換え
		A->Mat = Mataddress->Mat;
		A->row = row;
		A->column = col;

		//入れ替え
		Mataddress->Mat = lTemp_Mat;

		//開放
		fMat_Delete(Mataddress);

		
	}
	else
	{
		lErrorCounter++;
	}
	
	return lErrorCounter;
}

/*!

@param    _stMatrix* A
@param    _stMatrix* B
@return   
@note     要check バグあり?
@note     A = A * Bt (am bm = ab)
*/
int fMat_MltTrans2(_stMatrix* A, _stMatrix* B)
{
	int rowLoop = 0;
	int columnLoop = 0;
	int multiLoop = 0;
	int lErrorCounter = 0;

	int row = A->row;
	int col = B->row;
	int mltLoopMax = A->column;

	//_stMatrix* trans = fMat_New(col,row);//fMat_New(row,col);

#ifdef  def_InputCheck
	if((A->column == B->column)
		//&&(B->column == B->row)
		//&&(A->column == B->column)
		)	
#else
	if(true)
#endif
	{
		//領域確保
		_stMatrix* Mataddress = fMat_New(row, col);
		matrix lTemp_Mat;
	
		for(rowLoop = 0; rowLoop < row; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < col; columnLoop++)
			{
				fMat(Mataddress, rowLoop, columnLoop) = 0;
	
				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					fMat(Mataddress, rowLoop, columnLoop) += fMat(A, rowLoop, multiLoop) * fMat(B, columnLoop, multiLoop);
				}
			}
		}

		//仮置き
		lTemp_Mat = A->Mat;

		//書き換え
		A->Mat = Mataddress->Mat;
		A->row = row;
		A->column = col;

		//入れ替え
		Mataddress->Mat = lTemp_Mat;

		//開放
		fMat_Delete(Mataddress);
	}
	else
	{
		lErrorCounter++;
	}
	
	return lErrorCounter;
}




/*!

@param     _stMatrix* This
@param    
@return  
@note      転置行列
@note      新たな領域を作成し、元のポインタ先を開放、その後ポインタを入れ替える
@note      A = At
@note      要check 
*/
int fMat_Transpose2(_stMatrix* This)
{
	int rowLoop = 0;
	int columnLoop = 0;
	int lErrorCounter = 0;

	int row = This->column;
	int col = This->row;

	//
	_stMatrix* Mataddress = fMat_New(row, col);
	matrix lTemp_Mat;
	
	for(rowLoop = 0; rowLoop < row; rowLoop++)
	{
		for(columnLoop = 0; columnLoop < col; columnLoop++)
		{
			//fMat_Set(This, rowLoop][columnLoop],columnLoop,rowLoop,trans);
			fMat(Mataddress, rowLoop, columnLoop) = fMat(This, columnLoop, rowLoop);
		}
	}

	//仮置き
	lTemp_Mat = This->Mat;

	//置き換え
	This->Mat = Mataddress->Mat;
	This->row = row;
	This->column = col;

	//入れ替え
	Mataddress->Mat = lTemp_Mat;

	//開放
	fMat_Delete(Mataddress);
	
	return lErrorCounter;
}

/*!

@param    _stMatrix* Input   :  A^-1
@return   error数
@note     ガウスの消去法により逆行列を計算する。
@note     動的確保を行う
@note     A = A^-1
@note     正則行列であるという条件を付け加えるべきかも
*/
int fMat_InverseMatrix_Gauss2(_stMatrix* Input)
{
	int lErrorCounter = 0;
	int row_input = Input->row;
	int col_input = Input->column;

	int i_loop = 0;
	int j_loop = 0;
	int k_loop = 0;

	double lCalBuf = 0;

	

#ifdef  def_InputCheck	
	if((row_input == col_input)
		//&&(row_input == Output->row)
		//&&(row_input == Output->column)
		)
#else
	if(true)
#endif
	{
		//確保
		_stMatrix* Mataddress = fMat_New(row_input, row_input);
		matrix lTemp_Mat;
		

		//単位行列の作成
		for(i_loop = 0;i_loop < row_input; i_loop++)
		{
			for(j_loop = 0;j_loop < row_input; j_loop++)
			{
				if(i_loop == j_loop)
				{
					fMat(Mataddress, i_loop, j_loop) = 1;
				}
				else
				{
					fMat(Mataddress, i_loop, j_loop) = 0;
				}
			}
		}
		//
		for(i_loop = 0;i_loop < col_input;i_loop++)
		{
			lCalBuf = 1 / fMat(Input, i_loop, i_loop);

			for(j_loop = 0;j_loop < col_input; j_loop++)
			{
				fMat(Input, i_loop, j_loop) *= lCalBuf;

				fMat(Mataddress, i_loop, j_loop) *= lCalBuf;
			}

			for(j_loop = 0;j_loop < col_input;j_loop++)
			{
				if(i_loop != j_loop)
				{
					lCalBuf = fMat(Input, j_loop, i_loop);

					for(k_loop = 0;k_loop < col_input;k_loop++)
					{
						fMat(Input, j_loop, k_loop) -= fMat(Input, i_loop, k_loop) * lCalBuf;

						fMat(Mataddress, j_loop, k_loop) -= fMat(Mataddress, i_loop, k_loop) * lCalBuf;
					}
				}
			}
		}
		//仮置き
		lTemp_Mat = Input->Mat;

		//ポインタを書き換える
		Input->Mat = Mataddress->Mat;

		//入れ替え
		Mataddress->Mat = lTemp_Mat;

		//開放
		fMat_Delete(Mataddress);
	}
	else
	{
		lErrorCounter++;
	}

	//fMat_Delete(Mataddress);

	return lErrorCounter;
}

/*!

@param    _stMatrix* A
@param    _stMatrix* B
@return  
@note     転置行列の掛け算
@note     Mlt A = AtB
@note     ma * mb = ab
@note     要check
*/
int fMat_TransMlt2(_stMatrix* A, _stMatrix* B)
{
	int rowLoop,columnLoop,multiLoop;
	int lErrorCounter = 0;

	int rowLoopMax = A->column;//a
	int colLoopMax = B->column;//b
	int mltLoopMax = A->row;

#ifdef  def_InputCheck
	if((A->row == B->row)
		//&&(C->row == A->column)
		//&&(C->column == B->column)
		)
#else
	if(true)
#endif
	{
		//領域確保
		_stMatrix* Mataddress = fMat_New(rowLoopMax, colLoopMax);
		matrix lTemp_Mat;

		for(rowLoop = 0; rowLoop < rowLoopMax; rowLoop++)
		{
			for(columnLoop = 0; columnLoop < colLoopMax; columnLoop++)
			{
				fMat(Mataddress, rowLoop, columnLoop) = 0;

				for(multiLoop = 0; multiLoop < mltLoopMax; multiLoop++)
				{
					fMat(Mataddress, rowLoop, columnLoop) += fMat(A, multiLoop, rowLoop) * fMat(B, multiLoop, columnLoop);
				}
			}
		}

		//仮置き
		lTemp_Mat = A->Mat;

		//書き換え
		A->Mat = Mataddress->Mat;
		A->row = rowLoopMax;
		A->column = colLoopMax;

		//入れ替え
		Mataddress->Mat = lTemp_Mat;

		//開放
		fMat_Delete(Mataddress);
	}
	else
	{
		lErrorCounter++;
	}
	return lErrorCounter;
}


/*!

@param    _stMatrix* A
@param    _stMatrix* B
@param    _stMatrix* C
@return  
@note     行列の掛け算
@note     Mlt A = ABAt
@note     ab * bb * ba = aa
@note     作成予定
*/