// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix* pMatrix;

CMatrix::CMatrix()
{
	pMatrix = this;
}

CMatrix::~CMatrix()
{

}


// 피보팅을 처리하는 함수 구현
void CMatrix::pivoting(matrix* m, int currentRow, int currentCol)
{
	double *temp;
	temp =  new double[m->cols*2];

	double max = fabs(m->data[currentRow][currentCol]); // 우선 현재행,현재열 값을 최대값으로 지정

	for(int i=currentRow+1; i<m->rows; i++)
	{
		if(fabs(m->data[i][currentCol]) > max)
		{
			temp = m->data[currentRow];
			m->data[currentRow] = m->data[i];
			m->data[i] = temp;
			max = m->data[currentRow][currentCol];
		}
	}
	free(temp);
}

// 임의 크기의 행렬의 가우시안소거법 함수 구현
void CMatrix::gaussianElimination(matrix* m)
{
	double temp;
	int i,j,k;

	pivoting(m,0,0);

	for(i=0; i<m->rows-1; i++)
	{
		for(j=i+1; j<m->rows; j++)
		{
			pivoting(m,i,i);

			temp = m->data[j][i] / m->data[i][i];
			for(k=0; k < (m->cols*2); k++)
			{
				m->data[j][k] = m->data[j][k] - m->data[i][k]*temp;
			}
		}
	}
}

// 역행렬이 존재하는지 확인하는 함수 구현
int CMatrix::existINV(matrix* m)
{
	int size = m->rows;
	double temp = 1.0;
	
	gaussianElimination(m);
	
	for(int i=0; i<size; i++)
	{
		// 가우시안 소거법으로 상삼각 행렬을 만든후 대각요소의 곱을 구함
		temp *= m->data[i][i];
	}

	if(temp != 0) // 대각곱이 0이 아니면 역행렬이 존재하고 0이면 역행렬이 존재하지 않는다
		return 1;
	else
		return 0;
}

// 역행렬을 구하는 함수 구현
matrix* CMatrix::solveInverse(matrix* m)
{
	int size = m->rows;
	matrix* inv;

	inv = matrix_new(m->rows,m->cols);
	for(int i=0; i<size; i++)
	{
		// 열의 인덱스
		inv->data[size-1][i] = m->data[size-1][size+i] / m->data[size-1][size-1];
		for(int j=size-2; j>=0; j--)
		{
			double temp = 0.0;
			for(int k=size-1; k>j; k--)
			{
				temp += (m->data[j][k]*inv->data[k][i]);
			}
			inv->data[j][i] = (m->data[j][size+i]-temp) / m->data[j][j];
		}
	}
	return inv;
}

/*
// 역행렬이 존재하는지 확인하는 함수 구현
int CMatrix::existINV(double **matrix, int size)
{
	double temp = 1.0;
	gaussianElimination(matrix, size, 2*size);
	for(int i=0; i<size; i++)
	{
		// 가우시안 소거법으로 상삼각 행렬을 만든후 대각요소의 곱을 구함
		temp *= matrix[i][i];
	}
	if(temp != 0) // 대각곱이 0이 아니면 역행렬이 존재하고 0이면 역행렬이 존재하지 않는다
	return 1;
	else
	return 0;
}



// 임의 크기의 행렬의 가우시안소거법 함수 구현
void CMatrix::gaussianElimination(double **matrix, int rowSize, int colSize)
{
	double temp;
	int i,j,k;

	pivoting(matrix,0,0,rowSize,colSize);

	for(i=0; i<rowSize-1; i++)
	{
		for(j=i+1; j<rowSize; j++)
		{
			pivoting(matrix,i,i,rowSize,colSize);

			temp = matrix[j][i] / matrix[i][i];
			for(k=0; k<colSize; k++)
			{
				matrix[j][k] = matrix[j][k] - matrix[i][k]*temp;
			}
		}
	}
}



// 피보팅을 처리하는 함수 구현
void CMatrix::pivoting(double **matrix, int currentRow, int currentCol, int rowSize, int colSize)
{
	double *temp;
	temp =  new double[colSize];

	double max = fabs(matrix[currentRow][currentCol]); // 우선 현재행,현재열 값을 최대값으로 지정

	for(int i=currentRow+1; i<rowSize; i++)
	{
		if(fabs(matrix[i][currentCol]) > max)
		{
			temp = matrix[currentRow];
			matrix[currentRow] = matrix[i];
			matrix[i] = temp;
			max = matrix[currentRow][currentCol];
		}
	}
}
*/

double CMatrix::GetMinor(matrix* m, int row, int col)
{
	int c, r;
	double dMinor;
	if (m->cols==2 && m->rows == 2)
	{	
		// 2*2 matrix
		return m->data[!col][!row];
	}
	matrix* minor; 

	minor = matrix_new(m->rows-1, m->cols-1);
	for (r=0; r< m->rows-1; r++)
	{
		for (c=0; c< m->cols-1; c++)
		{
			minor->data[r][c] = m->data[r+(r>=col)][c+(c>=row)];
		}
	}
	dMinor = GetDeterminant(minor);
	
	matrix_free(minor);
	
	return dMinor;
}

double CMatrix::GetDeterminant(matrix* m)
{
	double dDet = 0.;
	int row;
	if (m->cols==2 && m->rows == 2)
	{
		// 2*2 matrix
		return (m->data[0][0] * m->data[1][1]) - (m->data[0][1] * m->data[1][0]);
	}
	for (row=0; row<m->rows; row++)
	{
		dDet += m->data[0][row] * GetCofactor(m, row, 0);

	}
	return dDet;
}

double CMatrix::GetCofactor(matrix* m, int row, int col)
{
	double dCofactor = ((row+col)%2 == 0) ? 1. : -1.;
	return dCofactor *= GetMinor(m, row, col);
}

matrix* CMatrix::matrix_adjoint(matrix* m)
{
	int col, row;
	matrix* temp;


	temp = matrix_new(m->cols, m->rows);

	for (col=0; col<m->cols; col++)
	{
		for (row=0; row<m->rows; row++)
		{
			temp->data[col][row] = GetCofactor(m, row, col);
		}
	}

	matrix* adj;
	
	// "Transpose" all elements of the previous matrix 
	// in other words swap their positions over the diagonal (the diagonal stays the same):
	adj = matrix_transpose(temp); 
	matrix_free(temp);

	return adj;
}

matrix* CMatrix::matrix_inv2(matrix* m)
{
	double dDet;
	dDet = matrix_det(m);

	if (dDet == 0)
	{
		return NULL;
	}

	matrix* inv;
	int c, r;

	inv = matrix_adjoint(m);

	for (c=0; c<inv->cols; c++)
	{
		for (r=0; r<inv->rows; r++)
		{
			inv->data[c][r] /= dDet;
		}
	}
	return inv;
}


/////////////////////////////////////////////////////

matrix* CMatrix::matrix_new( int rows, int cols )
{ 
	matrix* m; 
	int i; 

	m = (matrix*)malloc(sizeof(matrix)); 

	m->rows = rows; 
	m->cols = cols; 

	m->data = (double**)malloc(sizeof(double*)*rows); 
	for( i = 0 ; i < rows ; i++ )
	{ 
		m->data[i] = (double*)malloc(sizeof(double)*cols); 
		memset( m->data[i], 0, sizeof(double)*cols ); 
	} 

	return m; 

} 

void CMatrix::matrix_free( matrix* m )
{ 
	int i; 

	for( i = 0 ; i < m->rows ; i++ )
	{ 
		free(m->data[i]);         
	} 
	free(m->data); 
	free(m); 
} 

void CMatrix::matrix_init( matrix* m, double* src )
{ 
	int i, j; 
	int width = m->cols; 

	for( j = 0 ; j < m->rows ; j++ ) 
		for( i = 0 ; i < width ; i++ ) 
			m->data[j][i] = src[j*width+i]; 
} 

void CMatrix::matrix_extract( double* dst, matrix* m )
{ 
	int i, j; 
	int width = m->cols; 

	for( j = 0 ; j < m->rows ; j++ ) 
		for( i = 0 ; i < width ; i++ ) 
			dst[j*width+i] = m->data[j][i]; 
} 

void CMatrix::matrix_load_identity( matrix* m )
{ 

	int i, j, iter; 

	if( m->cols != m->rows ) 
		return; 

	iter = m->cols; 
	for( j = 0 ; j < iter ; j++ ) 
	{
		for( i = 0 ; i < iter ; i++ ) 
		{
			if( i == j ) 
				m->data[j][i] = 1.0; 
			else 
				m->data[j][i] = 0.0;
		}
	}
} 

matrix* CMatrix::matrix_transpose( matrix* m )
{ 

	int i, j; 
	matrix* t; 

	t = matrix_new( m->cols, m->rows ); 
	for( j = 0 ; j < m->rows ; j++ )
	{ 
		for( i = 0 ; i < m->cols ; i++ )
		{ 
			t->data[i][j] = m->data[j][i]; 
		} 
	} 
	return t; 
} 

double CMatrix::matrix_det(matrix* m)
{
	double dDet = 0.;// value of the determinant
	int rows = m->rows;
	int cols = m->cols;

	if (rows == cols)
	{
		// this is a square matrix
		if (rows == 1)
		{
			// this is a 1 x 1 matrix
			dDet = m->data[0][0];
		}
		else if (rows == 2)
		{
			// this is a 2 x 2 matrix
			// the determinant of [a11,a12;a21,a22] is det = a11*a22-a21*a12
			dDet = m->data[0][0] * m->data[1][1] - m->data[01][0] * m->data[0][1];
		}
		else
		{
			// this is a matrix of 3 x 3 or larger
			int c;
			for (c = 0; c < cols; c++)
			{
				matrix *minor = matrix_minor(m,0,c);
				//dDet += -pow(-1, 1+c) * m->data[0][c] * matrix_det(minor);
				dDet += -(c%2 + c%2 - 1) * m->data[0][c] * matrix_det(minor); // faster than with pow()
				matrix_free(minor);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("Matrix must be square"));
	}
	return dDet;

}

////////////////////////////////
//	returns the minor from the given matrix where the selected row and column are removed
matrix* CMatrix::matrix_minor(matrix* m,int row, int col)
{
	matrix* minor;

	if (row >= 0 && row < m->rows && col >= 0 && col < m->cols)
	{
		minor = matrix_new(m->rows - 1, m->cols - 1);

		// copy the content of the matrix to the minor, except the selected
		for (int r = 0; r < (m->rows - (row+1 >= m->rows)); r++)
		{
			for (int c = 0; c < (m->cols - (col+1 >= m->cols)); c++)
			{
				minor->data[r - (r > row)][c - (c > col)] = m->data[r][c];
			}
		}
	}
	else
	{
		AfxMessageBox(_T("Index for minor out of range"));
	}

	return minor;
}

matrix* CMatrix::matrix_multiple( matrix* a, matrix* b )
{ 
	matrix* m; 
	int cols, rows, iter; 
	int i, j, k; 

	if( a->cols != b->rows ) 
		return NULL; 

	rows = a->rows; 
	cols = b->cols; 

	iter = a->cols; 

	m = matrix_new( rows, cols ); 
	for( j = 0 ; j < rows ; j++ )
	{ 
		for( i = 0 ; i < cols ; i++ )
		{ 
			for( k = 0 ; k < iter ; k++ )
			{ 
				m->data[j][i] += a->data[j][k] * b->data[k][i];       
			} 
		} 
	} 
	return m; 
} 

// This function inverts a matrix based on the Gauss Jordan method.
// The function returns inverse matrix pointer on success, 0 on failure.
matrix* CMatrix::matrix_inv( matrix* m )
{ 
	matrix *n; 
	int iter, i, j, k; 

	double pivot; 

	double tmp; 
	int    max_key; 

	if( m->cols != m->rows ) 
		return NULL; 

	iter = m->rows; 
	n = matrix_new( m->rows, m->cols*2 ); 

	// copy it 
	for( j = 0 ; j < iter ; j++ ) 
		for( i = 0 ; i < iter ; i++ ) 
			n->data[j][i] = m->data[j][i]; 

	// insert identity matrix  
	for( i = 0 ; i < iter ; i++ ) 
		n->data[i][i+iter] = 1.0; 

	// start gauss elimination 
	for( i = 0 ; i < iter ; i++ )
	{ 
		// find max 
		max_key = i; 
		for( j = i+1 ; j < iter ; j++ ) 
			if( n->data[j][i] > n->data[max_key][i] ) 
				max_key = j; 

		// swap with current rows 
		if( max_key != i )
		{ 
			for( j = 0 ; j < iter*2 ; j++ )
			{ 
				tmp = n->data[i][j]; 
				n->data[i][j] = n->data[max_key][j]; 
				n->data[max_key][j] = tmp; 
			} 
		} 

		// normalize 
		pivot = n->data[i][i];
		if(pivot == 0) // modify by khc 
		{
			matrix_free(n);
			return NULL;
		}
		for( j = i+1 ; j < iter*2 ; j++ ) 
			n->data[i][j] /= pivot; 

		for( j = i+1 ; j < iter ; j++ )
		{ 
			pivot = n->data[j][i]; 
			n->data[j][i] = 0.0; 
			for( k = i+1 ; k < iter*2 ; k++ )
			{ 
				n->data[j][k] -= n->data[i][k]*pivot; 
			} 
		} 
	} 

	for( i = iter-2 ; i >= 0 ; i-- )
	{ 
		for( j = i ; j >= 0 ; j-- )
		{ 
			pivot = n->data[j][i+1]; 
			for( k = 0 ; k < iter*2 ; k++ )
			{ 
				n->data[j][k] -= n->data[i+1][k]*pivot; 
			} 
		} 
	} 

	matrix *inv; 	
	inv = matrix_new( m->rows, m->cols ); 

	// copy it 
	for( j = 0 ; j < iter ; j++ ) 
		for( i = 0 ; i < iter ; i++ ) 
			inv->data[j][i] = n->data[j][i+iter]; 

	matrix_free(n); 

	return inv; 
} 

void CMatrix::matrix_print( matrix* m )
{ 
	int i, j; 
	for( j = 0 ; j < m->rows ; j++ )
	{ 
		for( i = 0 ; i < m->cols ; i++ )
		{ 
			fprintf( stdout, "%3.5lf ", m->data[j][i] ); 
		} 
		fprintf( stdout, "\n" ); 
	} 

	fprintf( stdout, "\n" ); 
}

void CMatrix::matrix_print( matrix* m, CString &strMatrix)
{
	strMatrix = _T("");
	CString strElement;
	int i, j; 
	for( j = 0 ; j < m->rows ; j++ )
	{ 
		for( i = 0 ; i < m->cols ; i++ )
		{ 
			strElement.Format(_T("%3.5lf "), m->data[j][i] );
			strMatrix += strElement;
		} 
		strMatrix += _T("\n");
	} 
	strMatrix += _T("\n");
} 

int CMatrix::bound_check( int x, int y )
{ 
	if( x < 0 || x >= IMAGE_WIDTH || y < 0 || y >= IMAGE_HEIGHT ) 
		return 0; 
	return 1; 
 } 
  
void CMatrix::fwarping( unsigned char* dst, unsigned char* src, matrix* p )
{ 
	int i, j, x, y; 
	double w; 

	memset( dst, 0, sizeof(unsigned char)*IMAGE_WIDTH*IMAGE_HEIGHT ); 
	if( p->rows != 3 || p->cols != 3 ) 
		return; 

	for( j = 0 ; j < IMAGE_HEIGHT ; j++ )
	{ 
		for( i = 0 ; i < IMAGE_WIDTH ; i++ )
		{ 
			w = p->data[2][0]*i + p->data[2][1]*j + p->data[2][2]; 

			x = (int)((p->data[0][0]*i + p->data[0][1]*j + p->data[0][2]) / w); 
			y = (int)((p->data[1][0]*i + p->data[1][1]*j + p->data[1][2]) / w); 

			if( !bound_check(x, y) ) 
				continue; 

			dst[y*IMAGE_WIDTH+x] = src[j*IMAGE_WIDTH+i]; 
		} 
	} 
} 
  
void CMatrix::fwarping_i( unsigned char* dst, unsigned char* src, matrix* p )
{ 
	double px, py, w; 
	int i, j, x, y; 

	double* pixel; 
	double* ratio; 

	double  wx[2], wy[2]; 

	pixel = (double*)malloc(sizeof(double)*IMAGE_WIDTH*IMAGE_HEIGHT); 
	ratio = (double*)malloc(sizeof(double)*IMAGE_WIDTH*IMAGE_HEIGHT); 

	memset( pixel, 0, sizeof(double)*IMAGE_WIDTH*IMAGE_HEIGHT); 
	memset( ratio, 0, sizeof(double)*IMAGE_WIDTH*IMAGE_HEIGHT); 

	if( p->rows != 3 || p->cols != 3 ) 
		return; 

	for( j = 0 ; j < IMAGE_HEIGHT ; j++ )
	{ 
		for( i = 0 ; i < IMAGE_WIDTH ; i++ )
		{ 
			w = p->data[2][0]*i + p->data[2][1]*j + p->data[2][2]; 

			px = (p->data[0][0]*i + p->data[0][1]*j + p->data[0][2]) / w; 
			py = (p->data[1][0]*i + p->data[1][1]*j + p->data[1][2]) / w; 

			x = (int)floor(px); 
			y = (int)floor(py); 

			wx[1] = px - x; 
			wx[0] = 1.0 - wx[1]; 

			wy[1] = py - y; 
			wy[0] = 1.0 - wy[1]; 

			if( bound_check( x, y ) )
			{ 
				pixel[y*IMAGE_WIDTH+x] += src[j*IMAGE_WIDTH+i]*wx[0]*wy[0]; 
				ratio[y*IMAGE_WIDTH+x] += wx[0]*wy[0]; 
			} 

			if( bound_check( x+1, y ) )
			{ 
				pixel[y*IMAGE_WIDTH+(x+1)] += src[j*IMAGE_WIDTH+i]*wx[1]*wy[0]; 
				ratio[y*IMAGE_WIDTH+(x+1)] += wx[1]*wy[0]; 
			} 

			if( bound_check( x, y+1 ) )
			{ 
				pixel[(y+1)*IMAGE_WIDTH+x] += src[j*IMAGE_WIDTH+i]*wx[0]*wy[1]; 
				ratio[(y+1)*IMAGE_WIDTH+x] += wx[0]*wy[1]; 
			} 

			if( bound_check( x+1, y+1 ) )
			{ 
				pixel[(y+1)*IMAGE_WIDTH+(x+1)] += src[j*IMAGE_WIDTH+i]*wx[1]*wy[1]; 
				ratio[(y+1)*IMAGE_WIDTH+(x+1)] += wx[1]*wy[1]; 
			} 
		} 
	}
	
	for( j = 0 ; j < IMAGE_HEIGHT ; j++ )
	{
		for( i = 0 ; i < IMAGE_WIDTH ; i++ ) 
		{
			//dst[j*IMAGE_WIDTH+i] = (unsigned char)round(pixel[j*IMAGE_WIDTH+i]/ratio[j*IMAGE_WIDTH+i]); 
			dst[j*IMAGE_WIDTH+i] = (unsigned char)(pixel[j*IMAGE_WIDTH+i]/ratio[j*IMAGE_WIDTH+i]+0.5);
		}
	}
	
	free(ratio); 
	free(pixel); 

} 

void CMatrix::bwarping( unsigned char* dst, unsigned char* src, matrix* p )
{ 
	double w; 

	int i, j, x, y; 

	matrix* inv = matrix_inv(p); 
	if(inv)
	{
		for( j = 0 ; j < IMAGE_HEIGHT ; j++ )
		{ 
			for( i = 0 ; i < IMAGE_WIDTH ; i++ )
			{ 
				w = inv->data[2][0]*i + inv->data[2][1]*j + inv->data[2][2]; 

				x = (int)((inv->data[0][0]*i + inv->data[0][1]*j + inv->data[0][2])/w); 
				y = (int)((inv->data[1][0]*i + inv->data[1][1]*j + inv->data[1][2])/w); 

				if( !bound_check(x, y) ) 
					continue; 

				dst[j*IMAGE_WIDTH+i] = src[y*IMAGE_WIDTH+x]; 
			} 
		} 
		matrix_free(inv); 
	}
} 

void CMatrix::bwarping_i( unsigned char* dst, unsigned char* src, matrix* p )
{ 

	double w, pixel, ratio, px, py; 

	double wx[2]; 
	double wy[2]; 
	int i, j, x, y; 

	matrix* inv = matrix_inv(p); 
	if(inv)
	{
		for( j = 0 ; j < IMAGE_HEIGHT ; j++ )
		{ 
			for( i = 0 ; i < IMAGE_WIDTH ; i++ )
			{ 
				ratio = pixel = 0.0; 
				w = inv->data[2][0]*i + inv->data[2][1]*j + inv->data[2][2]; 

				px = (inv->data[0][0]*i + inv->data[0][1]*j + inv->data[0][2]) / w; 
				py = (inv->data[1][0]*i + inv->data[1][1]*j + inv->data[1][2]) / w; 

				wx[1] = px - floor(px); 
				wx[0] = 1.0 - wx[1]; 

				wy[1] = py - floor(py); 
				wy[0] = 1.0 - wy[1]; 

				x = floor(px); 
				y = floor(py); 

				if( bound_check(x, y))
				{ 
					pixel += wx[0]*wy[0]*src[y*IMAGE_WIDTH+x]; 
					ratio += wx[0]*wy[0]; 
				} 
				if( bound_check(x+1, y))
				{ 
					pixel += wx[1]*wy[0]*src[y*IMAGE_WIDTH+x+1]; 
					ratio += wx[1]*wy[0]; 
				} 
				if( bound_check(x, y+1))
				{ 
					pixel += wx[0]*wy[1]*src[(y+1)*IMAGE_WIDTH+x]; 
					ratio += wx[0]*wy[1]; 
				} 
				if( bound_check(x+1, y+1))
				{ 
					pixel += wx[1]*wy[1]*src[(y+1)*IMAGE_WIDTH+x+1]; 
					ratio += wx[1]*wy[1]; 
				} 
				
				dst[j*IMAGE_WIDTH+i] = (unsigned char)floor( pixel/ratio + 0.5 ); 
			} 
		} 
		matrix_free(inv); 
	}
} 

matrix* CMatrix::translate_matrix( double x, double y)
{ 
	matrix* translate; 

	translate = matrix_new( 3, 3 ); 

	translate->data[0][0] = 1; 
	translate->data[0][1] = 0; 
	translate->data[0][2] = x; // X방향 이동 증분

	translate->data[1][0] = 0; 
	translate->data[1][1] = 1; 
	translate->data[1][2] = y; // Y방향 이동 증분

	translate->data[2][0] = 0; 
	translate->data[2][1] = 0; 
	translate->data[2][2] = 1; 

	return translate; 
}


matrix* CMatrix::translate_matrix( double x, double y, double _x, double _y)
{ 
	matrix* translate; 

	translate = matrix_new( 3, 3 ); 

	translate->data[0][0] = 1; 
	translate->data[0][1] = 0; 
	translate->data[0][2] = _x-x; // X방향 이동 증분

	translate->data[1][0] = 0; 
	translate->data[1][1] = 1; 
	translate->data[1][2] = _y-y; // Y방향 이동 증분

	translate->data[2][0] = 0; 
	translate->data[2][1] = 0; 
	translate->data[2][2] = 1; 

	return translate; 
}

void CMatrix::GetTranslatePoint(double x1, double y1, double &x2, double &y2, matrix* t )
{ 
	if( t->rows != 3 || t->cols != 3 ) 
		return; 
	
	x2 = (t->data[0][0]*x1 + t->data[0][1]*y1 + t->data[0][2]); 
	y2 = (t->data[1][0]*x1 + t->data[1][1]*y1 + t->data[1][2]); 

	return;
	// method 2
	
	matrix* b;
	b = matrix_new(3,1);

	b->data[0][0] = x1; 
	b->data[1][0] = y1; 
	b->data[2][0] = 1; 	

	matrix *c;
	c = matrix_multiple( t, b ); 

	x2 = c->data[0][0]; 
	y2 = c->data[1][0];
	
	matrix_free(c);
	matrix_free(b);

} 

matrix* CMatrix::scale_matrix( double fScaleX, double fScaleY)
{
	matrix* scale;

	scale = matrix_new(3,3);
	
	scale->data[0][0] = fScaleX; 
	scale->data[0][1] = 0; 
	scale->data[0][2] = 0;

	scale->data[1][0] = 0; 
	scale->data[1][1] = fScaleY; 
	scale->data[1][2] = 0;

	scale->data[2][0] = 0; 
	scale->data[2][1] = 0; 
	scale->data[2][2] = 1; 

	return scale;
}

void CMatrix::GetScalePoint(double x1, double y1, double &x2, double &y2, matrix* s )
{
	if( s->rows != 3 || s->cols != 3 ) 
		return; 
	
	x2 = (s->data[0][0]*x1 + s->data[0][1]*y1 + s->data[0][2]); 
	y2 = (s->data[1][0]*x1 + s->data[1][1]*y1 + s->data[1][2]); 

	return;
	
	// method 2
	matrix* b;
	b = matrix_new(3,1);

	b->data[0][0] = x1; 
	b->data[1][0] = y1; 
	b->data[2][0] = 1; 	

	matrix *c;
	c = matrix_multiple(s, b ); 

	x2 = c->data[0][0]; 
	y2 = c->data[1][0];
	
	matrix_free(c);
	matrix_free(b);

}

matrix* CMatrix::rotation_matrix( double fSinTheta, double fCosTheta)
{
	matrix* rotate;

	rotate = matrix_new(3,3);
	
	rotate->data[0][0] = fCosTheta; 
	rotate->data[0][1] = -fSinTheta; 
	rotate->data[0][2] = 0;

	rotate->data[1][0] = fSinTheta; 
	rotate->data[1][1] = fCosTheta; 
	rotate->data[1][2] = 0;

	rotate->data[2][0] = 0; 
	rotate->data[2][1] = 0; 
	rotate->data[2][2] = 1; 

	return rotate;
}

void CMatrix::GetRotatePoint(double x1, double y1, double &x2, double &y2, matrix* r )
{
	if( r->rows != 3 || r->cols != 3 ) 
		return; 
	
	x2 = (r->data[0][0]*x1 + r->data[0][1]*y1 + r->data[0][2]); 
	y2 = (r->data[1][0]*x1 + r->data[1][1]*y1 + r->data[1][2]); 

	return;
	
	// method 2
	matrix* b;
	b = matrix_new(3,1);

	b->data[0][0] = x1; 
	b->data[1][0] = y1; 
	b->data[2][0] = 1; 	

	matrix *c;
	c = matrix_multiple(r, b ); 

	x2 = c->data[0][0]; 
	y2 = c->data[1][0];
	
	matrix_free(c);
	matrix_free(b);

}

matrix* CMatrix::linear_matrix( double fSinTheta, double fCosTheta, double fScaleX, double fScaleY, double fOffsetX, double fOffsetY)
{
	matrix *translate;
	translate = pMatrix->translate_matrix(fOffsetX,fOffsetY);

	matrix *scale;
	scale = pMatrix->scale_matrix(fScaleX,fScaleY);

	matrix *rotate;
	rotate = pMatrix->rotation_matrix(fSinTheta,fCosTheta);

	matrix *a; // scale * rotate
	a = pMatrix->matrix_multiple(scale,rotate);

	matrix* linear; // translate
	linear = pMatrix->matrix_multiple(translate,a);

	pMatrix->matrix_free(a);
	pMatrix->matrix_free(rotate);
	pMatrix->matrix_free(scale);
	pMatrix->matrix_free(translate);

	return linear; 
}

matrix* CMatrix::affine_matrix( double* x, double* y, double* _x, double* _y)
{
	matrix* a; 
	matrix* b;
	matrix* c; 
	matrix* a_inv; 
	matrix* affine; 

	a = matrix_new( 6, 6 ); 
	b = matrix_new( 6, 1 ); 

    a->data[0][0] = x[0];
    a->data[0][1] = y[0];
    a->data[0][2] = 1.;

    a->data[1][0] = x[1];
    a->data[1][1] = y[1];
    a->data[1][2] = 1.;

    a->data[2][0] = x[2];
    a->data[2][1] = y[2];
    a->data[2][2] = 1.;

    a->data[3][3] = x[0];
    a->data[3][4] = y[0];
    a->data[3][5] = 1.;

    a->data[4][3] = x[1];
    a->data[4][4] = y[1];
    a->data[4][5] = 1.;

    a->data[5][3] = x[2];
    a->data[5][4] = y[2];
    a->data[5][5] = 1.;

	b->data[0][0] = _x[0]; 
	b->data[1][0] = _x[1]; 
	b->data[2][0] = _x[2]; 
	b->data[3][0] = _y[0]; 
	b->data[4][0] = _y[1]; 
	b->data[5][0] = _y[2]; 

	a_inv = matrix_inv(a); // get inverse matrix by gauss elimination 
	if(a_inv==NULL)
	{
		a_inv= matrix_inv2(a); // get inverse matrix by general method 
		if(a_inv==NULL)
		{
			matrix_free(b);
			matrix_free(a);
			AfxMessageBox(_T("Not found inverse matrix"));
			return NULL;
		}
	}
	
	c = matrix_multiple( a_inv, b ); 

	affine = matrix_new( 3, 3 ); 

	affine->data[0][0] = c->data[0][0]; 
	affine->data[0][1] = c->data[1][0]; 
	affine->data[0][2] = c->data[2][0];


	affine->data[1][0] = c->data[3][0]; 
	affine->data[1][1] = c->data[4][0]; 
	affine->data[1][2] = c->data[5][0]; 

	affine->data[2][0] = 0; 
	affine->data[2][1] = 0; 
	affine->data[2][2] = 1.0; 

	matrix_free(c); 
	matrix_free(b); 
	matrix_free(a); 

	matrix_free(a_inv);

	return affine; 
}

matrix* CMatrix::projection_matrix( double* x, double* y, double* _x, double* _y)
{ 
	matrix* a; 
	matrix* b; 
	matrix* c;
	matrix* a_inv; 
	matrix* projection; 

	a = matrix_new( 8, 8 ); 
	b = matrix_new( 8, 1 ); 

	a->data[0][0] = x[0]; 
	a->data[0][1] = y[0]; 
	a->data[0][2] = 1.0; 
	a->data[0][6] = -1 * _x[0] * x[0]; 
	a->data[0][7] = -1 * _x[0] * y[0]; 

	a->data[1][0] = x[1]; 
	a->data[1][1] = y[1]; 
	a->data[1][2] = 1.0; 
	a->data[1][6] = -1 * _x[1] * x[1]; 
	a->data[1][7] = -1 * _x[1] * y[1]; 

	a->data[2][0] = x[2]; 
	a->data[2][1] = y[2]; 
	a->data[2][2] = 1.0; 
	a->data[2][6] = -1 * _x[2] * x[2]; 
	a->data[2][7] = -1 * _x[2] * y[2]; 

	a->data[3][0] = x[3]; 
	a->data[3][1] = y[3]; 
	a->data[3][2] = 1.0; 
	a->data[3][6] = -1 * _x[3] * x[3]; 
	a->data[3][7] = -1 * _x[3] * y[3]; 

	a->data[4][3] = x[0]; 
	a->data[4][4] = y[0]; 
	a->data[4][5] = 1.0; 
	a->data[4][6] = -1 * x[0] * _y[0]; 
	a->data[4][7] = -1 * y[0] * _y[0]; 

	a->data[5][3] = x[1]; 
	a->data[5][4] = y[1]; 
	a->data[5][5] = 1.0; 
	a->data[5][6] = -1 * x[1] * _y[1]; 
	a->data[5][7] = -1 * y[1] * _y[1]; 

	a->data[6][3] = x[2]; 
	a->data[6][4] = y[2]; 
	a->data[6][5] = 1.0; 
	a->data[6][6] = -1 * x[2] * _y[2]; 
	a->data[6][7] = -1 * y[2] * _y[2]; 

	a->data[7][3] = x[3]; 
	a->data[7][4] = y[3]; 
	a->data[7][5] = 1.0; 
	a->data[7][6] = -1 * x[3] * _y[3]; 
	a->data[7][7] = -1 * y[3] * _y[3]; 

	b->data[0][0] = _x[0]; 
	b->data[1][0] = _x[1]; 
	b->data[2][0] = _x[2]; 
	b->data[3][0] = _x[3]; 
	b->data[4][0] = _y[0]; 
	b->data[5][0] = _y[1]; 
	b->data[6][0] = _y[2]; 
	b->data[7][0] = _y[3]; 

	a_inv = matrix_inv(a); // get inverse matrix by gauss elimination 
	if(a_inv==NULL)
	{
		a_inv= matrix_inv2(a); // get inverse matrix by general method 
		if(a_inv==NULL)
		{
			matrix_free(b);
			matrix_free(a);
			AfxMessageBox(_T("Not found inverse matrix"));
			return NULL;
		}
	}
	
	c = matrix_multiple( a_inv, b ); 

	projection = matrix_new( 3, 3 ); 
	
	projection->data[0][0] = c->data[0][0]; 
	projection->data[0][1] = c->data[1][0]; 
	projection->data[0][2] = c->data[2][0]; 

	projection->data[1][0] = c->data[3][0]; 
	projection->data[1][1] = c->data[4][0]; 
	projection->data[1][2] = c->data[5][0]; 

	projection->data[2][0] = c->data[6][0]; 
	projection->data[2][1] = c->data[7][0]; 
	projection->data[2][2] = 1.0; 

	matrix_free(c); 
	matrix_free(b); 
	matrix_free(a); 
	matrix_free(a_inv); 

	return projection; 

}
void CMatrix::GetAffinePoint(double x1, double y1, double &x2, double &y2, matrix* p )
{
	if( p->rows != 3 || p->cols != 3 ) 
		return; 

	x2 = (p->data[0][0]*x1 + p->data[0][1]*y1 + p->data[0][2]); 
	y2 = (p->data[1][0]*x1 + p->data[1][1]*y1 + p->data[1][2]); 
}

void CMatrix::GetPerspectivePoint(double x1, double y1, double &x2, double &y2, matrix* p )
{ 
	double w; 

	if( p->rows != 3 || p->cols != 3 ) 
		return; 

	w = p->data[2][0]*x1 + p->data[2][1]*y1 + p->data[2][2]; 

	x2 = ((p->data[0][0]*x1 + p->data[0][1]*y1 + p->data[0][2]) / w); 
	y2 = ((p->data[1][0]*x1 + p->data[1][1]*y1 + p->data[1][2]) / w); 

} 
 
