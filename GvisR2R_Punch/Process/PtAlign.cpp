   // PtAlign.cpp: implementation of the CPtAlign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PtAlign.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMatrix* pMatrix;

CPtAlign::CPtAlign()
{
	m_pMatrix = NULL;

	for(int i = 0; i < 4; i++)
	{
		m_fRefX[i] = m_fRefY[i] = m_fTgtX[i] = m_fTgtY[i] = 0.0;
	}
}

CPtAlign::~CPtAlign()
{
	if(m_pMatrix)
		m_Matrix.matrix_free(m_pMatrix);
}

/////////////////////////////////////////////////
// make translate matrix for point align
// average: 5.76us @ Intel(R) Core(TM) i5-2500 CPU 3.30GHz
void CPtAlign::SetAlignData(double Rx1,double Ry1,double Tx1,double Ty1)
{
	if(m_pMatrix)
		m_Matrix.matrix_free(m_pMatrix);
	
	m_fRefX[0] = Rx1; m_fRefY[0] = Ry1;
	m_fTgtX[0] = Tx1; m_fTgtY[0] = Ty1;

	m_pMatrix = m_Matrix.translate_matrix( m_fRefX[0], m_fRefY[0], m_fTgtX[0], m_fTgtY[0] );

}

/////////////////////////////////////////////////
// make linear matrix for linear align
// average: 5.76us @ Intel(R) Core(TM) i5-2500 CPU 3.30GHz
void CPtAlign::SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Tx1,double Ty1, double Tx2,double Ty2)
{
	if(m_pMatrix)
		m_Matrix.matrix_free(m_pMatrix);
	
	m_fRefX[0] = Rx1; 	m_fRefY[0] = Ry1;
	m_fRefX[1] = Rx2;	m_fRefY[1] = Ry2;
	
	m_fTgtX[0] = Tx1;	m_fTgtY[0] = Ty1;
	m_fTgtX[1] = Tx2;	m_fTgtY[1] = Ty2;

	double fRefTheta = atan2(Ry2-Ry1,Rx2-Rx1);	
	double fTgtTheta = atan2(Ty2-Ty1,Tx2-Tx1);
	double fTheta = fTgtTheta-fRefTheta;
	
	double fSinTheta = sin(fTheta);
	double fCosTheta = cos(fTheta);

	double fScale = sqrt((Tx2-Tx1)*(Tx2-Tx1)+(Ty2-Ty1)*(Ty2-Ty1))/sqrt((Rx2-Rx1)*(Rx2-Rx1)+(Ry2-Ry1)*(Ry2-Ry1));

	double fOffsetX = Tx1;
	double fOffsetY = Ty1;

	m_pMatrix = m_Matrix.linear_matrix(fSinTheta,fCosTheta,fScale,fScale,fOffsetX,fOffsetY);

}

/////////////////////////////////////////////////
// make projection matrix for affine align
// average: 5.76us @ Intel(R) Core(TM) i5-2500 CPU 3.30GHz
void CPtAlign::SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Rx3,double Ry3,double Tx1,double Ty1,double Tx2,double Ty2,double Tx3,double Ty3)
{
	if(m_pMatrix)
		m_Matrix.matrix_free(m_pMatrix);

	double Rx4,Ry4,Tx4,Ty4;

	// Affine 사각형을 만들기 위한 임의의 한점을 찾는다.
//	Rx4 = Rx1-Rx2+Rx3;	
//	Ry4 = Ry1-Ry2+Ry3;	
//	Tx4 = Tx1-Tx2+Tx3;	
//	Ty4 = Ty1-Ty2+Ty3;	
	
	m_fRefX[0] = Rx1; m_fRefY[0] = Ry1;
	m_fRefX[1] = Rx2; m_fRefY[1] = Ry2;
	m_fRefX[2] = Rx3; m_fRefY[2] = Ry3;
//	m_fRefX[3] = Rx4; m_fRefY[3] = Ry4;
	
	m_fTgtX[0] = Tx1; m_fTgtY[0] = Ty1;
	m_fTgtX[1] = Tx2; m_fTgtY[1] = Ty2;
	m_fTgtX[2] = Tx3; m_fTgtY[2] = Ty3;
//	m_fTgtX[3] = Tx4; m_fTgtY[3] = Ty4;

	m_pMatrix = m_Matrix.affine_matrix( m_fRefX, m_fRefY, m_fTgtX, m_fTgtY ); 

}

/////////////////////////////////////////////////
// make projection matrix for perspective align
// average: 5.75us @ Intel(R) Core(TM) i5-2500 CPU 3.30GHz
void CPtAlign::SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Rx3,double Ry3,double Rx4,double Ry4,
							double Tx1,double Ty1,double Tx2,double Ty2,double Tx3,double Ty3,double Tx4,double Ty4)
{
	if(m_pMatrix)
		m_Matrix.matrix_free(m_pMatrix);
	
	m_fRefX[0] = Rx1; m_fRefY[0] = Ry1;
	m_fRefX[1] = Rx2; m_fRefY[1] = Ry2;
	m_fRefX[2] = Rx3; m_fRefY[2] = Ry3;
	m_fRefX[3] = Rx4; m_fRefY[3] = Ry4;
	
	m_fTgtX[0] = Tx1; m_fTgtY[0] = Ty1;
	m_fTgtX[1] = Tx2; m_fTgtY[1] = Ty2;
	m_fTgtX[2] = Tx3; m_fTgtY[2] = Ty3;
	m_fTgtX[3] = Tx4; m_fTgtY[3] = Ty4;

	m_pMatrix = m_Matrix.projection_matrix( m_fRefX, m_fRefY, m_fTgtX, m_fTgtY ); 
}

/////////////////////////////////////////////////////////////////
// Point transformation 
// 1) displacement
// m_fRefX[0],m_fRefY[0]: Cad 데이타로부터 추출된 인식마크의 X 및 Y 위치  
// m_fTgtX[0],m_fTgtY[0]: Target상의 인식마크의 X 및 Y 위치
// Rx0,Ry0           : Allignment하고자 하는 Point의 X,Y 위치
// *Tx0,*Ty0         : Allignment Result
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::PointAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0) 
{
	if(!m_pMatrix)
		return FALSE;

	Tx0 = m_pMatrix->data[0][0]*Rx0 + m_pMatrix->data[0][1]*Ry0 + m_pMatrix->data[0][2]; 
	Ty0 = m_pMatrix->data[1][0]*Rx0 + m_pMatrix->data[1][1]*Ry0 + m_pMatrix->data[1][2]; 
	return TRUE;

	// method 2
	Tx0 = Rx0 - (m_fRefX[0]-m_fTgtX[0]);
	Ty0 = Ry0 - (m_fRefY[0]-m_fTgtY[0]);
	return TRUE;
}

BOOL CPtAlign::PointAlignment(CfPoint fptRef,CfPoint &fptTgt)
{
	if(!m_pMatrix)
		return FALSE;

	fptTgt.x = m_pMatrix->data[0][0]*fptRef.x + m_pMatrix->data[0][1]*fptRef.y + m_pMatrix->data[0][2]; 
	fptTgt.y = m_pMatrix->data[1][0]*fptRef.x + m_pMatrix->data[1][1]*fptRef.y + m_pMatrix->data[1][2]; 
	return TRUE;

	// method 2
	fptTgt.x = fptRef.x - (m_fRefX[0]-m_fTgtX[0]);
	fptTgt.y = fptRef.y - (m_fRefY[0]-m_fTgtY[0]);
	return TRUE;

}


/////////////////////////////////////////////////////////////////
// Linear transformation 
// 1) displacement
// 2) scale
// 3) rotation
// m_fRefX[0]..m_fRefX[1],m_fRefY[0]..m_fRefY[1] : Cad 데이타로부터 추출된 2개의 인식마크의 X 및 Y 위치  
// m_fTgtX[0]..m_fTgtX[1],m_fTgtY[0]..m_fTgtY[1] : Target상의 2개의 인식마크의 X 및 Y 위치
// Rx0,Ry0           : Allignment하고자 하는 Point의 X,Y 위치
// *Tx0,*Ty0         : Allignment Result
// 실행시간 약 4.5us @ PIII 550MHz
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::LinearAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0) 
{
	if(!m_pMatrix)
		return FALSE;

	double x = Rx0 - m_fRefX[0];
	double y = Ry0 - m_fRefY[0];

	Tx0 = m_pMatrix->data[0][0]*x + m_pMatrix->data[0][1]*y + m_pMatrix->data[0][2]; 
	Ty0 = m_pMatrix->data[1][0]*x + m_pMatrix->data[1][1]*y + m_pMatrix->data[1][2]; 

	return TRUE;
}

BOOL CPtAlign::LinearAlignment(CfPoint fptRef,CfPoint &fptTgt) 
{
	if(!m_pMatrix)
		return FALSE;

	double x = fptRef.x - m_fRefX[0];
	double y = fptRef.y - m_fRefY[0];

	fptTgt.x = m_pMatrix->data[0][0]*x + m_pMatrix->data[0][1]*y + m_pMatrix->data[0][2]; 
	fptTgt.y = m_pMatrix->data[1][0]*x + m_pMatrix->data[1][1]*y + m_pMatrix->data[1][2]; 

	return TRUE;
}

/////////////////////////////////////////////////////////////////
// Affine transformation 
// 1) displacement
// 2) scale
// 3) rotation
// 4) shear
// m_fRefX[0]..Rx3,m_fRefY[0]..Ry3 : Cad 데이타로부터 추출된 3개의 인식마크의 X 및 Y 위치  
// m_fTgtX[0]..Tx3,m_fTgtY[0]..Ty3 : Target상의 3개의 인식마크의 X 및 Y 위치
// &Rx0,&Ry0           : Allignment하고자 하는 Point의 X,Y 위치
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::AffineAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0) 
{
	if(!m_pMatrix)
		return FALSE;

	Tx0 = ((m_pMatrix->data[0][0]*Rx0 + m_pMatrix->data[0][1]*Ry0 + m_pMatrix->data[0][2])); 
	Ty0 = ((m_pMatrix->data[1][0]*Rx0 + m_pMatrix->data[1][1]*Ry0 + m_pMatrix->data[1][2])); 


	return TRUE;
}

BOOL CPtAlign::AffineAlignment(CfPoint fptRef,CfPoint &fptTgt) 
{
	if(!m_pMatrix)
		return FALSE;

	fptTgt.x = ((m_pMatrix->data[0][0]*fptRef.x + m_pMatrix->data[0][1]*fptRef.y + m_pMatrix->data[0][2])); 
	fptTgt.y = ((m_pMatrix->data[1][0]*fptRef.x + m_pMatrix->data[1][1]*fptRef.y + m_pMatrix->data[1][2])); 

	return TRUE;
}


/////////////////////////////////////////////////////////////////
// Perspective transformation 
// 1) displacement
// 2) scale
// 3) rotation
// 4) shear
// 5) Distortion
// m_fRefX[0]..Rx4,m_fRefY[0]..Ry4 : Cad데이타로부터 추출된 4개의 인식마크 각각의 X 및 Y 위치  
// m_fTgtX[0]..Tx4,m_fTgtY[0]..Ty4 : Target상의 4개의 인식마크 각각의 X 및 Y 위치
// Rx0,Ry0           : Allignment하고자 하는 Point의 X,Y 위치
// *Tx0,*Ty0         : Allignment Result
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::PerspectiveAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0)
{
	if(!m_pMatrix)
		return FALSE;

	double w = m_pMatrix->data[2][0]*Rx0 + m_pMatrix->data[2][1]*Ry0 +m_pMatrix->data[2][2]; 

	Tx0 = ((m_pMatrix->data[0][0]*Rx0 + m_pMatrix->data[0][1]*Ry0 + m_pMatrix->data[0][2]) / w); 
	Ty0 = ((m_pMatrix->data[1][0]*Rx0 + m_pMatrix->data[1][1]*Ry0 + m_pMatrix->data[1][2]) / w); 

	return TRUE;
}

BOOL CPtAlign::PerspectiveAlignment(CfPoint fptRef,CfPoint fptTgt)
{
	if(!m_pMatrix)
		return FALSE;

	double w = m_pMatrix->data[2][0]*fptRef.x + m_pMatrix->data[2][1]*fptRef.y +m_pMatrix->data[2][2]; 

	fptTgt.x = ((m_pMatrix->data[0][0]*fptRef.x + m_pMatrix->data[0][1]*fptRef.y + m_pMatrix->data[0][2]) / w); 
	fptTgt.y = ((m_pMatrix->data[1][0]*fptRef.x + m_pMatrix->data[1][1]*fptRef.y + m_pMatrix->data[1][2]) / w); 

	return TRUE;
}


BOOL CPtAlign::GetBilinearPointOnRef(CfPoint &fpt1,CfPoint &fpt2,CfPoint &fpt3,CfPoint &fpt4)
{
	fpt1 = m_fptRefBLPt;	// Bottom line intersect point on Reference quad
	fpt2 = m_fptRefTLPt;	// top line intersect point on Reference quad
	fpt3 = m_fptRefLLPt;	// Left line intersect point on Reference quad
	fpt4 = m_fptRefRLPt;	// Right side line intersect point on Reference quad

	return TRUE;
}

BOOL CPtAlign::GetBilinearPointOnTgt(CfPoint &fpt1,CfPoint &fpt2,CfPoint &fpt3,CfPoint &fpt4)
{
	fpt1 = 	m_fptTgtBLPt;	// Bottom line intersect point on Target quad
	fpt2 = 	m_fptTgtTLPt;	// top line intersect point on Target quad
	fpt3 = 	m_fptTgtLLPt;	// Left line intersect point on Target quad
	fpt4 = 	m_fptTgtRLPt;	// Right side line intersect point on Target quad

	return TRUE;
}

/////////////////////////////////////////////////////////////////
// Rx1..Rx4,Ry1..Ry4 : Cad 데이타로부터 추출된 4개의 인식마크의 X 및 Y 위치  
// Tx1..Tx4,Ty1..Ty4 : Target상의 4개의 인식마크의 X 및 Y 위치
// &Rx0,&Ry0           : Allignment하고자 하는 Point의 X,Y 위치
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::BilinearAlignment(
	double Rx1,double Ry1,double Tx1,double Ty1,
	double Rx2,double Ry2,double Tx2,double Ty2,
	double Rx3,double Ry3,double Tx3,double Ty3,
	double Rx4,double Ry4,double Tx4,double Ty4,
	double Rx,double Ry,double *Tx,double *Ty) 
{
	CfPoint fptTgt;
	
	CfQuad QuadRef(CfPoint(Rx4,Ry4),CfPoint(Rx3,Ry3),CfPoint(Rx2,Ry2),CfPoint(Rx1,Ry1));
	CfQuad QuadTgt(CfPoint(Tx4,Ty4),CfPoint(Tx3,Ty3),CfPoint(Tx2,Ty2),CfPoint(Tx1,Ty1));

	BilinearAlignment(QuadRef,QuadTgt,CfPoint(Rx,Ry),fptTgt);
	*Tx = fptTgt.x;
	*Ty = fptTgt.y;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////
// fptRefLB,fptRefRB,fptRefRT,fptRefLT : Cad 데이타로부터 추출된 4개의 인식마크의 X 및 Y 위치  
// fptTgtLB,fptTgtRB,fptTgtRT,fptTgtLT : Target상의 4개의 인식마크의 X 및 Y 위치
// fptRef           : Allignment하고자 하는 Point의 X,Y 위치
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::BilinearAlignment(
	CfPoint fptRefLB,CfPoint fptRefRB,CfPoint fptRefRT,CfPoint fptRefLT,
	CfPoint fptTgtLB,CfPoint fptTgtRB,CfPoint fptTgtRT,CfPoint fptTgtLT,
	CfPoint fptRef,  CfPoint &fptTgt) 
{
	return BilinearAlignment(CfQuad(fptRefLB,fptRefRB,fptRefRT,fptRefLT),CfQuad(fptTgtLB,fptTgtRB,fptTgtRT,fptTgtLT),fptRef,fptTgt);
}

/////////////////////////////////////////////////////////////////
// fQuadRef : Cad 데이타로부터 추출된 4개의 인식마크의 X 및 Y 위치  
// fQuadTgt : Target상의 4개의 인식마크의 X 및 Y 위치
// fptRef           : Allignment하고자 하는 Point의 X,Y 위치
/////////////////////////////////////////////////////////////////
BOOL CPtAlign::BilinearAlignment(CfQuad fQuadRef,CfQuad fQuadTgt,CfPoint fptRef,  CfPoint &fptTgt) 
{
	double a1x,a2x,a1y,a2y,c1x,c2y,q1y;
	double b1x,b2x,b1y,b2y,d1x,d2y,q1x;
	double A,B,C,R1,R2;
	double first,second;


	a1x = fQuadRef.leftbottom.x;
	b1x = fQuadRef.rightbottom.x-fQuadRef.leftbottom.x;
						
	a2x = fQuadRef.lefttop.x;
	b2x = fQuadRef.righttop.x-fQuadRef.lefttop.x;

	a1y = fQuadRef.leftbottom.y;
	b1y = fQuadRef.rightbottom.y-fQuadRef.leftbottom.y;

	a2y = fQuadRef.lefttop.y;
	b2y = fQuadRef.righttop.y-fQuadRef.lefttop.y;
		
						
	c1x = a2x-a1x;
	d1x = b2x-b1x;
						
	c2y = a2y-a1y;
	d2y = b2y-b1y;
						
	q1y = fptRef.y-a1y;
	q1x = fptRef.x-a1x;					

	// syd-20131015
// 	if( (b1x * d2y) == (b1y * d1x) )
// 		A = 0.0;
// 	else
		A=b1x * d2y - b1y * d1x;

	B=(q1y*d1x-b1y*c1x)+(b1x*c2y-q1x*d2y);
	C=q1y*c1x - q1x*c2y;

	first = B*B;
	second = 4.0*A*C;	

	if(first<second)
		return FALSE;

	if(A != 0.0 && fabs(A) > 0.00000000001)
		R1 = (-B+sqrt(B*B - 4*A*C))/(2*A);
	else
	{
		if(B == 0)
			return FALSE;
		R1 = -C/B;
	}

	a1x = fQuadRef.leftbottom.x;
	b1x = fQuadRef.lefttop.x-fQuadRef.leftbottom.x;
	a2x = fQuadRef.rightbottom.x;
	b2x = fQuadRef.righttop.x-fQuadRef.rightbottom.x;

	a1y = fQuadRef.leftbottom.y;
	b1y = fQuadRef.lefttop.y-fQuadRef.leftbottom.y;
	a2y = fQuadRef.rightbottom.y;
	b2y = fQuadRef.righttop.y-fQuadRef.rightbottom.y;						
							
	c1x = a2x - a1x;
	d1x = b2x-b1x;
							
	c2y = a2y-a1y;
	d2y = b2y-b1y;
	q1y = fptRef.y-a1y;
	q1x = fptRef.x-a1x;

	// syd-20131015
// 	if( (b1x * d2y) == (b1y * d1x) )
// 		A = 0.0;
// 	else
		A=b1x * d2y - b1y * d1x;
	
	B=q1y*d1x-b1y*c1x+b1x*c2y-q1x*d2y;
	C=q1y*c1x - q1x*c2y;

	first = B*B;
	second = 4.0*A*C;

	if(first<second)
		return FALSE;

	if(A != 0.0 && fabs(A) > 0.00000000001)
		R2 = (-B-sqrt(B*B - 4*A*C))/(2*A);
	else
	{
		if(B == 0)
			return FALSE;
		R2 = -C/B;
	}


	// Bottom line intersect point on reference quad
	m_fptRefBLPt.x = fQuadRef.leftbottom.x+(fQuadRef.rightbottom.x-fQuadRef.leftbottom.x)*R1; 
	m_fptRefBLPt.y = fQuadRef.leftbottom.y+(fQuadRef.rightbottom.y-fQuadRef.leftbottom.y)*R1;							

	// top line intersect point on reference quad
	m_fptRefTLPt.x = fQuadRef.lefttop.x+(fQuadRef.righttop.x-fQuadRef.lefttop.x)*R1;
	m_fptRefTLPt.y = fQuadRef.lefttop.y+(fQuadRef.righttop.y-fQuadRef.lefttop.y)*R1;
	
	// Left line intersect point on reference quad
	m_fptRefLLPt.y = fQuadRef.leftbottom.y+(fQuadRef.lefttop.y-fQuadRef.leftbottom.y)*R2;
	m_fptRefLLPt.x = fQuadRef.leftbottom.x+(fQuadRef.lefttop.x-fQuadRef.leftbottom.x)*R2;
	
	// Right side line intersect point on reference quad
	m_fptRefRLPt.y = fQuadRef.rightbottom.y+(fQuadRef.righttop.y-fQuadRef.rightbottom.y)*R2;
	m_fptRefRLPt.x = fQuadRef.rightbottom.x+(fQuadRef.righttop.x-fQuadRef.rightbottom.x)*R2;


	// Bottom line intersect point on target quad
	m_fptTgtBLPt.x = fQuadTgt.leftbottom.x+(fQuadTgt.rightbottom.x-fQuadTgt.leftbottom.x)*R1; 
	m_fptTgtBLPt.y = fQuadTgt.leftbottom.y+(fQuadTgt.rightbottom.y-fQuadTgt.leftbottom.y)*R1;							

	// top line intersect point on target quad
	m_fptTgtTLPt.x = fQuadTgt.lefttop.x+(fQuadTgt.righttop.x-fQuadTgt.lefttop.x)*R1;
	m_fptTgtTLPt.y = fQuadTgt.lefttop.y+(fQuadTgt.righttop.y-fQuadTgt.lefttop.y)*R1;
	
	// Left line intersect point on target quad
	m_fptTgtLLPt.y = fQuadTgt.leftbottom.y+(fQuadTgt.lefttop.y-fQuadTgt.leftbottom.y)*R2;
	m_fptTgtLLPt.x = fQuadTgt.leftbottom.x+(fQuadTgt.lefttop.x-fQuadTgt.leftbottom.x)*R2;
	
	// Right side line intersect point on target quad
	m_fptTgtRLPt.y = fQuadTgt.rightbottom.y+(fQuadTgt.righttop.y-fQuadTgt.rightbottom.y)*R2;
	m_fptTgtRLPt.x = fQuadTgt.rightbottom.x+(fQuadTgt.righttop.x-fQuadTgt.rightbottom.x)*R2;
	
	// 두 직선의 교점 구하기
	double under = (m_fptTgtRLPt.y-m_fptTgtLLPt.y)*(m_fptTgtTLPt.x-m_fptTgtBLPt.x)-(m_fptTgtRLPt.x-m_fptTgtLLPt.x)*(m_fptTgtTLPt.y-m_fptTgtBLPt.y);							
	if(under==0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	double tt = (m_fptTgtRLPt.x-m_fptTgtLLPt.x)*(m_fptTgtBLPt.y-m_fptTgtLLPt.y) - (m_fptTgtRLPt.y-m_fptTgtLLPt.y)*(m_fptTgtBLPt.x-m_fptTgtLLPt.x);							
	double t = tt/under;							

	fptTgt.x = m_fptTgtBLPt.x + t * (m_fptTgtTLPt.x-m_fptTgtBLPt.x);							
	fptTgt.y = m_fptTgtBLPt.y + t * (m_fptTgtTLPt.y-m_fptTgtBLPt.y);							

	return TRUE;
}

BOOL CPtAlign::BilinearAlignment(CfQuad3D fQuadRef,CfQuad3D fQuadTgt,CfPoint3D fptRef,  CfPoint3D &fptTgt) 
{
	double a1x,a2x,a1y,a2y,c1x,c2y,q1y;
	double b1x,b2x,b1y,b2y,d1x,d2y,q1x;
	double A,B,C,R1,R2;
	double first,second;


	a1x = fQuadRef.leftbottom.x;
	b1x = fQuadRef.rightbottom.x-fQuadRef.leftbottom.x;
						
	a2x = fQuadRef.lefttop.x;
	b2x = fQuadRef.righttop.x-fQuadRef.lefttop.x;

	a1y = fQuadRef.leftbottom.y;
	b1y = fQuadRef.rightbottom.y-fQuadRef.leftbottom.y;

	a2y = fQuadRef.lefttop.y;
	b2y = fQuadRef.righttop.y-fQuadRef.lefttop.y;
		
						
	c1x = a2x-a1x;
	d1x = b2x-b1x;
						
	c2y = a2y-a1y;
	d2y = b2y-b1y;
						
	q1y = fptRef.y-a1y;
	q1x = fptRef.x-a1x;					

	// syd-20131015
// 	if( (b1x * d2y) == (b1y * d1x) )
// 		A = 0.0;
// 	else 
		A=b1x * d2y - b1y * d1x;

	B=(q1y*d1x-b1y*c1x)+(b1x*c2y-q1x*d2y);
	C=q1y*c1x - q1x*c2y;

	first = B*B;
	second = 4.0*A*C;	

	if(first<second)
		return FALSE;

	if(A != 0.0 && fabs(A) > 0.00000000001)
		R1 = (-B+sqrt(B*B - 4*A*C))/(2*A);
	else
	{
		if(B == 0)
			return FALSE;
		R1 = -C/B;
	}

	a1x = fQuadRef.leftbottom.x;
	b1x = fQuadRef.lefttop.x-fQuadRef.leftbottom.x;
	a2x = fQuadRef.rightbottom.x;
	b2x = fQuadRef.righttop.x-fQuadRef.rightbottom.x;

	a1y = fQuadRef.leftbottom.y;
	b1y = fQuadRef.lefttop.y-fQuadRef.leftbottom.y;
	a2y = fQuadRef.rightbottom.y;
	b2y = fQuadRef.righttop.y-fQuadRef.rightbottom.y;						
							
	c1x = a2x - a1x;
	d1x = b2x-b1x;
							
	c2y = a2y-a1y;
	d2y = b2y-b1y;
	q1y = fptRef.y-a1y;
	q1x = fptRef.x-a1x;

	// syd-20131015
// 	if( (b1x * d2y) == (b1y * d1x) )
// 		A = 0.0;
// 	else
		A=b1x * d2y - b1y * d1x;
	
	B=q1y*d1x-b1y*c1x+b1x*c2y-q1x*d2y;
	C=q1y*c1x - q1x*c2y;

	first = B*B;
	second = 4.0*A*C;

	if(first<second)
		return FALSE;

	if(A != 0.0 && fabs(A) > 0.00000000001)
		R2 = (-B-sqrt(B*B - 4*A*C))/(2*A);
	else
	{
		if(B == 0)
			return FALSE;
		R2 = -C/B;
	}


	// Bottom line intersect point on reference quad
	m_fptRefBLPt.x = fQuadRef.leftbottom.x+(fQuadRef.rightbottom.x-fQuadRef.leftbottom.x)*R1; 
	m_fptRefBLPt.y = fQuadRef.leftbottom.y+(fQuadRef.rightbottom.y-fQuadRef.leftbottom.y)*R1;							

	// top line intersect point on reference quad
	m_fptRefTLPt.x = fQuadRef.lefttop.x+(fQuadRef.righttop.x-fQuadRef.lefttop.x)*R1;
	m_fptRefTLPt.y = fQuadRef.lefttop.y+(fQuadRef.righttop.y-fQuadRef.lefttop.y)*R1;
	
	// Left line intersect point on reference quad
	m_fptRefLLPt.y = fQuadRef.leftbottom.y+(fQuadRef.lefttop.y-fQuadRef.leftbottom.y)*R2;
	m_fptRefLLPt.x = fQuadRef.leftbottom.x+(fQuadRef.lefttop.x-fQuadRef.leftbottom.x)*R2;
	
	// Right side line intersect point on reference quad
	m_fptRefRLPt.y = fQuadRef.rightbottom.y+(fQuadRef.righttop.y-fQuadRef.rightbottom.y)*R2;
	m_fptRefRLPt.x = fQuadRef.rightbottom.x+(fQuadRef.righttop.x-fQuadRef.rightbottom.x)*R2;


	// Bottom line intersect point on target quad
	m_fptTgtBLPt.x = fQuadTgt.leftbottom.x+(fQuadTgt.rightbottom.x-fQuadTgt.leftbottom.x)*R1; 
	m_fptTgtBLPt.y = fQuadTgt.leftbottom.y+(fQuadTgt.rightbottom.y-fQuadTgt.leftbottom.y)*R1;							

	// top line intersect point on target quad
	m_fptTgtTLPt.x = fQuadTgt.lefttop.x+(fQuadTgt.righttop.x-fQuadTgt.lefttop.x)*R1;
	m_fptTgtTLPt.y = fQuadTgt.lefttop.y+(fQuadTgt.righttop.y-fQuadTgt.lefttop.y)*R1;
	
	// Left line intersect point on target quad
	m_fptTgtLLPt.y = fQuadTgt.leftbottom.y+(fQuadTgt.lefttop.y-fQuadTgt.leftbottom.y)*R2;
	m_fptTgtLLPt.x = fQuadTgt.leftbottom.x+(fQuadTgt.lefttop.x-fQuadTgt.leftbottom.x)*R2;
	
	// Right side line intersect point on target quad
	m_fptTgtRLPt.y = fQuadTgt.rightbottom.y+(fQuadTgt.righttop.y-fQuadTgt.rightbottom.y)*R2;
	m_fptTgtRLPt.x = fQuadTgt.rightbottom.x+(fQuadTgt.righttop.x-fQuadTgt.rightbottom.x)*R2;
	
	// 두 직선의 교점 구하기
	double under = (m_fptTgtRLPt.y-m_fptTgtLLPt.y)*(m_fptTgtTLPt.x-m_fptTgtBLPt.x)-(m_fptTgtRLPt.x-m_fptTgtLLPt.x)*(m_fptTgtTLPt.y-m_fptTgtBLPt.y);							
	if(under==0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	double tt = (m_fptTgtRLPt.x-m_fptTgtLLPt.x)*(m_fptTgtBLPt.y-m_fptTgtLLPt.y) - (m_fptTgtRLPt.y-m_fptTgtLLPt.y)*(m_fptTgtBLPt.x-m_fptTgtLLPt.x);							
	double t = tt/under;							

	fptTgt.x = m_fptTgtBLPt.x + t * (m_fptTgtTLPt.x-m_fptTgtBLPt.x);							
	fptTgt.y = m_fptTgtBLPt.y + t * (m_fptTgtTLPt.y-m_fptTgtBLPt.y);							

	return TRUE;
}
