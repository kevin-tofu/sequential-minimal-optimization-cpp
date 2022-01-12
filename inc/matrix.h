#pragma once
/// <summary>
/// class : matrix
/// name  : 渡邉 浩平
/// Date  : 2010/12/14
/// 
/// 履歴
///
/// </summary>
#ifndef def_ElementType_mat
	#define def_ElementType_mat double
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

typedef def_ElementType_mat *vector;

#define def_ShortMemory

//要素へのアクセス

#ifdef def_ShortMemory
typedef def_ElementType_mat *matrix;
#define fMat(This, i, j)  (This->Mat[i * This->column + j])
#else
typedef def_ElementType_mat **matrix;
#define fMat(This, i, j)  (This->Mat[i][j])
#endif

typedef struct Tag_stMatrix
{
	//行
	int row;
	//列
	int column;

	matrix Mat;

}_stMatrix;


//new del
extern _stMatrix *fMat_New(int input_row, int input_col);//!< Dynamically allocated
extern int fMat_Delete(_stMatrix* Mat);
extern _stMatrix* fMat_NewCopy(_stMatrix* This);//!< Dynamically allocated
extern _stMatrix* fMat_NewCopy_rowVector(_stMatrix* This, int input_row);//!< Dynamically allocated
extern _stMatrix* fMat_NewCopy_colVector(_stMatrix* This, int input_col);//!< Dynamically allocated


//property
extern int fMat_Set(_stMatrix* This, int input_row, int input_col, def_ElementType_mat input);
extern def_ElementType_mat fMat_Get(_stMatrix* This, int input_row, int input_col);
extern def_ElementType_mat fMat_GetDiag(_stMatrix* This, int input_row);

//cal
extern int fMat_Zero(_stMatrix* This);
extern int fMat_SetConst(_stMatrix* This, def_ElementType_mat input);
extern int fMat_UnitMatrix(_stMatrix* This);
extern int fMat_Add(_stMatrix* C,_stMatrix* A,_stMatrix* B);
extern int fMat_Add2(_stMatrix* A,_stMatrix* B);
extern int fMat_Sub(_stMatrix* C,_stMatrix* A,_stMatrix* B);
extern int fMat_Sub2(_stMatrix* A,_stMatrix* B);
extern int fMat_Mlt(_stMatrix* C,_stMatrix* A,_stMatrix* B);
extern int fMat_Mlt2(_stMatrix* A, _stMatrix* B);//!< Dynamically allocated
extern int fMat_MltTrans(_stMatrix* C,_stMatrix* A,_stMatrix* B);
extern int fMat_MltTrans2(_stMatrix* A, _stMatrix* B);//!< Dynamically allocated
extern int fMat_TransMlt(_stMatrix* C, _stMatrix* A, _stMatrix* B);
extern int fMat_TransMlt2(_stMatrix* A, _stMatrix* B);//!< Dynamically allocated
extern int fMat_Sub(_stMatrix* C,_stMatrix* A,_stMatrix* B);
extern int fMat_Sub2(_stMatrix* A,_stMatrix* B);

extern int fMat_UD_Degradation(_stMatrix* U, _stMatrix* D, _stMatrix* input);

extern int fMat_Multiplier(_stMatrix* output, _stMatrix* input, def_ElementType_mat Multiplier);
extern int fMat_Multiplier2(_stMatrix* This, def_ElementType_mat Multiplier);
extern int fMat_Transpose(_stMatrix* output, _stMatrix* input);
extern int fMat_Transpose2(_stMatrix* This);//!< Dynamically allocated

extern int fMat_Copy(_stMatrix* output, _stMatrix* input);
extern int fMat_Copy_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row);
extern int fMat_Copy_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col);
extern int fMat_Copy_colVector_TO_row(_stMatrix* output, int output_row, _stMatrix* input, int input_col);
extern int fMat_Copy_rowVector_TO_column(_stMatrix* output, int output_col, _stMatrix* input, int input_row);

extern int fMat_Add2_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row);
extern int fMat_Add2_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col);

extern int fMat_Multiplier_rowVector(_stMatrix* output, int output_row, _stMatrix* input, int input_row, def_ElementType_mat Multiplier);
extern int fMat_Multiplier_colVector(_stMatrix* output, int output_col, _stMatrix* input, int input_col, def_ElementType_mat Multiplier);

extern int fMat_Multiplier2_rowVector(_stMatrix* This, int output_row, def_ElementType_mat Multiplier);
extern int fMat_Multiplier2_colVector(_stMatrix* This, int output_col, def_ElementType_mat Multiplier);


extern int fMat_InverseMatrix_Gauss(_stMatrix* Output, _stMatrix* Input);
extern int fMat_InverseMatrix_Gauss2(_stMatrix* Input);//!< Dynamically allocated
extern int fMat_CholeskyDecomposition(_stMatrix* Output, _stMatrix* Input);

extern int fMat_Check_SymmetricMatrix(_stMatrix* A);


extern int fMat_GetMaxElement_NotDiag(_stMatrix* input, int* p, int* q, def_ElementType_mat* max);

extern def_ElementType_mat fMat_InnerProduct(_stMatrix* A, _stMatrix* B);
extern int fMat_InnerMatrix(_stMatrix* C, _stMatrix* A, _stMatrix* B);