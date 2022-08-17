// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__A28E9326_0068_4635_BFE9_05E36008F325__INCLUDED_)
#define AFX_MATRIX_H__A28E9326_0068_4635_BFE9_05E36008F325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IMAGE_WIDTH  640 
#define IMAGE_HEIGHT 480 
#define INFILE  "test1.raw" 
#define OUTFILE "test1_%s.raw" 

typedef struct { 
	int cols; 
	int rows; 
	double** data; 
} matrix; 

class CMatrix  
{
public:
	CMatrix();
	virtual ~CMatrix();

	matrix* matrix_new( int row, int col ); 
	void matrix_free( matrix* m );
	
	matrix* matrix_minor(matrix* m,int row, int col);
	matrix* matrix_multiple( matrix* a, matrix* b ); 
	matrix* matrix_inv( matrix* m ); 
	matrix* matrix_inv2(matrix* m);
	matrix* matrix_transpose( matrix* m ); 
	double matrix_det(matrix* m);

///////////////////////////////////////////////
	void pivoting(matrix* m, int currentRow, int currentCol);
	void gaussianElimination(matrix* m);
	int existINV(matrix *m);
	matrix* solveInverse(matrix* m);

	double GetMinor(matrix* m, int row, int col);
	double GetCofactor(matrix* m, int row, int col);
	double GetDeterminant(matrix* m);
	matrix* matrix_adjoint(matrix* m);



///////////////////////////////////////////////
	void matrix_init( matrix* m, double* src ); 
	void matrix_extract( double* dst, matrix* m ); 
	void matrix_load_identity( matrix* m );
	void matrix_print( matrix* m );
	void matrix_print( matrix* m, CString &strMatrix);

	static int bound_check( int x, int y );
	void fwarping( unsigned char* dst, unsigned char* src, matrix* p );
	void fwarping_i( unsigned char* dst, unsigned char* src, matrix* p );
	void bwarping( unsigned char* dst, unsigned char* src, matrix* p );
	void bwarping_i( unsigned char* dst, unsigned char* src, matrix* p );

	matrix* translate_matrix( double x, double y);
	matrix* translate_matrix( double x, double y, double _x, double _y);
	matrix* scale_matrix( double fScaleX, double fScaleY);
	matrix* rotation_matrix( double fSinTheta, double fCosTheta);
	matrix* linear_matrix( double fSinTheta, double fCosTheta, double fScaleX, double fScaleY, double fOffsetX, double fOffsetY);
	matrix* affine_matrix( double* x, double* y, double* _x, double* _y);
	matrix* projection_matrix( double* x, double* y, double* _x, double* _y);

	void GetTranslatePoint(double x1, double y1, double &x2, double &y2, matrix* p );
	void GetScalePoint(double x1, double y1, double &x2, double &y2, matrix* s );
	void GetRotatePoint(double x1, double y1, double &x2, double &y2, matrix* p );
	
	void GetAffinePoint(double x1, double y1, double &x2, double &y2, matrix* p );
	void GetPerspectivePoint(double x1, double y1, double &x2, double &y2, matrix* p );

};

#endif // !defined(AFX_MATRIX_H__A28E9326_0068_4635_BFE9_05E36008F325__INCLUDED_)
