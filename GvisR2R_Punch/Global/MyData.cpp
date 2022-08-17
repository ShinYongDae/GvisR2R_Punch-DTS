// MyData.cpp: implementation of the CMyData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "GlobalDefine.h"
#include "GlobalFunc.h"
#include "MyData.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//++++++++++++++++++++++++++++++++++++++
// Global math func

// 두선분의 벡터 방향을 나타낸다.
// 시계방향일 경우 (1)을 반시계 방향일경우 (-1)을  리턴한다.
// 두선분이 동일한 방향이라면 0를 리턴한다.

double GlobalMathematics::GetDistance(CfPoint3D FirstPoint,CfPoint3D SecondPoint)
{
	return sqrt(pow(SecondPoint.x-FirstPoint.x,2)+pow(SecondPoint.y-FirstPoint.y,2));
}

CfPoint::CfPoint()
{
/* random filled */
}

CfPoint::CfPoint(double initfX, double initfY)
{
	x=initfX;
	y=initfY;
}
CfPoint::CfPoint(FPOINT initfPt)
{
	*(FPOINT*)this = initfPt;
}
CfPoint::CfPoint(FSIZE initfSize)
{
//	*(FSIZE*)this = initfSize;
	x = initfSize.cx;
	y = initfSize.cy;
}
CfPoint::CfPoint(DWORD dwPoint)
{
	x = (short)LOWORD(dwPoint);
	y = (short)HIWORD(dwPoint);
}
CfPoint::~CfPoint()
{

}
void CfPoint::Offset(double fXOffset,double fYOffset)
{
	x += fXOffset;
	y += fYOffset;	
}

void CfPoint::Offset(FPOINT fPoint)
{
	x += fPoint.x;
	y += fPoint.y;	
}
void CfPoint::Offset(FSIZE fSize)
{
	x += fSize.cx;
	y += fSize.cy;
}

CfPoint CfPoint::Round( CfPoint fPoint, int nPos)
{
	CfPoint fTemp;
	fTemp.x = fPoint.x * pow( 10.0, nPos );
	fTemp.x = floor( fTemp.x + 0.5 );
	fTemp.x *= pow( 10.0, -nPos );

	fTemp.y = fPoint.y * pow( 10.0, nPos );
	fTemp.y = floor( fTemp.y + 0.5 );
	fTemp.y *= pow( 10.0, -nPos );

	return fTemp;
}

BOOL CfPoint::operator ==(FPOINT fPoint)
{
	return (x == fPoint.x && y == fPoint.y);
}

BOOL CfPoint::operator !=(FPOINT fPoint)
{
	return (x != fPoint.x || y != fPoint.y);
}

void CfPoint::operator +=(FSIZE  fSize)
{
	x += fSize.cx; y += fSize.cy;
}
void CfPoint::operator +=(FPOINT fPoint)
{
	x += fPoint.x; y += fPoint.y;
}
void CfPoint::operator -=(FSIZE  fSize)
{
	x += fSize.cx; y += fSize.cy;
}
void CfPoint::operator -=(FPOINT fPoint)
{
	x -= fPoint.x; y -= fPoint.y;
}
CfPoint CfPoint::operator +(FSIZE fSize ) const
{
	return CfPoint(x + fSize.cx, y + fSize.cy);
}
CfPoint CfPoint::operator +(FPOINT fPoint ) const
{
	return CfPoint(x + fPoint.x, y + fPoint.y);
}
CfRect CfPoint::operator +( const FRECT* lpfRect ) const
{
	return CfRect(lpfRect->left+x,lpfRect->top+y,lpfRect->right+x,lpfRect->bottom+y);
}
CfSize CfPoint::operator -(FPOINT fPoint ) const
{
	return CfSize(x - fPoint.x, y - fPoint.y);
}
CfPoint CfPoint::operator -(FSIZE fSize ) const
{
	return CfPoint(x - fSize.cx, y - fSize.cy);
}
CfRect CfPoint::operator -( const FRECT* lpfRect ) const
{
	return CfRect(lpfRect->left-x,lpfRect->top-y,lpfRect->right-x,lpfRect->bottom-y);
}
/*
CfPoint CfPoint::operator -(FPOINT fPoint ) const
{
	return CfPoint(x - fPoint.x, y - fPoint.y);
}
*/
//	CfRect CfPoint::operator -(const RECT* lpRect ) const
CfPoint CfPoint::operator -() const
{
	return CfPoint(-x, -y);
}

/////////////////////////////////////////////////////////////////////////////////////////////

CfPoint3D::CfPoint3D()
{
/* random filled */
}
CfPoint3D::CfPoint3D(double initfX, double initfY, double initfZ)
{
	x=initfX;
	y=initfY;
	z=initfZ;
}
CfPoint3D::CfPoint3D(FPOINT3D initfPt)
{
	*(FPOINT3D*)this = initfPt;
}

CfPoint3D::~CfPoint3D()
{

}
void CfPoint3D::Offset(double fXOffset,double fYOffset,double fZOffset)
{
	x += fXOffset;
	y += fYOffset;
	z += fZOffset;
}

void CfPoint3D::Offset(FPOINT3D fPoint)
{
	x += fPoint.x;
	y += fPoint.y;	
	z += fPoint.z;	
}

CfPoint3D CfPoint3D::Round( CfPoint3D fPoint, int nPos)
{
	CfPoint3D fTemp;
	fTemp.x = fPoint.x * pow( 10.0, nPos );
	fTemp.x = floor( fTemp.x + 0.5 );
	fTemp.x *= pow( 10.0, -nPos );

	fTemp.y = fPoint.y * pow( 10.0, nPos );
	fTemp.y = floor( fTemp.y + 0.5 );
	fTemp.y *= pow( 10.0, -nPos );

	fTemp.z = fPoint.z * pow( 10.0, nPos );
	fTemp.z = floor( fTemp.z + 0.5 );
	fTemp.z *= pow( 10.0, -nPos );

	
	return fTemp;
}

BOOL CfPoint3D::operator ==(FPOINT3D fPoint)
{
	return (x == fPoint.x && y == fPoint.y && z == fPoint.z);
}

BOOL CfPoint3D::operator !=(FPOINT3D fPoint)
{
	return (x != fPoint.x || y != fPoint.y || z != fPoint.z);
}

void CfPoint3D::operator +=(FPOINT3D fPoint)
{
	x += fPoint.x; y += fPoint.y; z += fPoint.z;
}

void CfPoint3D::operator -=(FPOINT3D fPoint)
{
	x -= fPoint.x; y -= fPoint.y; z -= fPoint.z;
}

CfPoint3D CfPoint3D::operator +(FPOINT3D fPoint ) const
{
	return CfPoint3D(x + fPoint.x, y + fPoint.y, z + fPoint.z);
}

CfPoint3D CfPoint3D::operator -(FPOINT3D fPoint ) const
{
	return CfPoint3D(x - fPoint.x, y - fPoint.y, z - fPoint.z);
}

//130703 LGH ADD
CfPoint3D::CfPoint3D(CfPoint3D& vt) 
{
	x = vt.x; 
	y = vt.y;
	z = vt.z;
	
//	r = vt.r;
//	g = vt.g;
//	b = vt.b;
}

CfPoint3D CfPoint3D::operator + (CfPoint3D& fPoint) 
{	
	CfPoint3D sum;
	sum.x = x + fPoint.x;
	sum.y = y + fPoint.y;
	sum.z = z + fPoint.z;
	return sum;
}


void CfPoint3D:: operator = (CfPoint3D& fPoint)
{
	x = fPoint.x;
	y = fPoint.y;
	z = fPoint.z;

// 
// 	r = fPoint.r;
// 	g = fPoint.g;
//	b = fPoint.b;
}

CfPoint3D CfPoint3D::operator - (CfPoint3D& fPoint)
{
	CfPoint3D sum;
	sum.x = x - fPoint.x;
	sum.y = y - fPoint.y;
	sum.z = z - fPoint.z;
	return sum;
}
	
CfPoint3D CfPoint3D::operator / (float div)
{
	CfPoint3D fDivison;
	
	if(div==0.0)
	{
		fDivison.x=0.0;
		fDivison.y=0.0;
		fDivison.z=0.0;
	}
	else
	{

		fDivison.x = x /div;
		fDivison.y = y /div;
		fDivison.z = z /div;
	}

	return fDivison;
}

CfPoint3D CfPoint3D::operator *(CfPoint3D& fDivison)
{
	CfPoint3D fMultiPly;

	fMultiPly.x = y*fDivison.z-z*fDivison.y;
	fMultiPly.y = z*fDivison.x-x*fDivison.z;
	fMultiPly.z = x*fDivison.y-y*fDivison.x;

	return fMultiPly;
}

CfPoint3D CfPoint3D::operator * (double dDivison)
{
	CfPoint3D fMultiPly;

	if(dDivison=0.0)
	{
		fMultiPly.x = 0.0;
		fMultiPly.y = 0.0;
		fMultiPly.z = 0.0;
	}
	else
	{
		fMultiPly.x = x *dDivison;
		fMultiPly.y = y *dDivison;
		fMultiPly.z = z *dDivison;
	}

	return fMultiPly;
}

void CfPoint3D::Normalize()
{
	double mag = Magnitude();

	if(mag!=0.)
	{
		x/=(float)mag;
		y/=(float)mag;
		z/=(float)mag;
	}

}

double CfPoint3D::Magnitude()
{
	return sqrt( SQR(x) + SQR(y) + SQR(z) );
}
//ADD END




/////////////////////////////////////////////////////////////////////////////////////////////
CfSize::CfSize()
{
}
CfSize::CfSize(double initfCX,double initfCY)
{
	cx = initfCX;
	cy = initfCY;
}
CfSize::CfSize(FSIZE initfSize)
{
	*(FSIZE*)this = initfSize;
}
CfSize::CfSize(FPOINT initfPt)
{
//	*(FPOINT*)this = initfPt; 
	cx = initfPt.x;
	cy = initfPt.y;
}
CfSize::CfSize(DWORD dwSize)
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}
CfSize::~CfSize()
{

}

// Checks for equality between two sizes.
// Returns nonzero if the sizes are equal, otherwize 0.
BOOL CfSize::operator ==(FSIZE fSize) const
{
	return (cx == fSize.cx && cy == fSize.cy);
}

// Checks for inequality between two sizes.
// Returns nonzero if the sizes are not equal, otherwise 0.
BOOL CfSize::operator !=(FSIZE fSize ) const
{
	return (cx != fSize.cx || cy != fSize.cy);
}

// Adds a fSize to this CfSize.
void CfSize::operator += (FSIZE fSize)
{
	cx += fSize.cx; cy += fSize.cy;
}

// Subtracts a size from this CfSize.
void CfSize::operator -=(FSIZE fSize)
{
	cx -= fSize.cx; cy -= fSize.cy;
}

// This operation adds two CSize values.
CfSize CfSize::operator +(FSIZE fSize) const
{
	return CfSize(cx+fSize.cx, cy+fSize.cy);
}

// The cx and cy members of this CSize value are added
// to the x and y data members of the POINT value
CfPoint CfSize::operator +(FPOINT fPoint ) const
{
	return CfPoint(cx + fPoint.x, cy + fPoint.y);
}

// This operation subtracts two CSize values.
CfSize CfSize::operator -(FSIZE fSize) const
{
	return CfSize(cx-fSize.cx, cy-fSize.cy);
}

// This operation offsets (moves) aFPOINT or CfPoint value by the additive inverse 
// of this CfSize value. The cx and cy of this CfSize value are subtracted from 
// the x and y data members of the FPOINT value. 
// It is analogous to the version of CfPoint::operator - that takes aFSIZE parameter.
CfPoint CfSize::operator -(FPOINT fPoint) const
{
	return CfPoint(cx-fPoint.x, cy-fPoint.y);
}

// This operation offsets (moves) aRECT or CRect value by the additive inverse of this CSize value.
// The cx and cy members of this CSize value are subtracted from the left, top, right, and bottom data members of the RECT value. 
// It is analogous to the version of CRect::operator - that takes aSIZE parameter.
//CfRect operator -(const FRECT* lpfRect ) const;
//{
//}

// This operation returns the additive inverse of this CSize value.
CfSize CfSize::operator -() const
{
	return CfSize(-cx,-cy);
}
/////////////////////////////////////////////////////////////////////////////////////////////
// CfSize3D Class
/////////////////////////////////////////////////////////////////////////////////////////////
CfSize3D::CfSize3D()
{
}
CfSize3D::CfSize3D(double initfCX,double initfCY,double initfCZ)
{
	cx = initfCX;
	cy = initfCY;
	cz = initfCZ;
}
CfSize3D::CfSize3D(FSIZE3D initfSize)
{
	*(FSIZE3D*)this = initfSize;
}
CfSize3D::CfSize3D(FPOINT3D initfPt)
{
	cx = initfPt.x;
	cy = initfPt.y;
	cz = initfPt.z;
}

CfSize3D::~CfSize3D()
{

}

// Checks for equality between two sizes.
// Returns nonzero if the sizes are equal, otherwize 0.
BOOL CfSize3D::operator ==(FSIZE3D fSize) const
{
	return (cx == fSize.cx && cy == fSize.cy && cz == fSize.cz);
}

// Checks for inequality between two sizes.
// Returns nonzero if the sizes are not equal, otherwise 0.
BOOL CfSize3D::operator !=(FSIZE3D fSize ) const
{
	return (cx != fSize.cx || cy != fSize.cy || cz != fSize.cz);
}

// Adds a fSize to this CfSize.
void CfSize3D::operator += (FSIZE3D fSize)
{
	cx += fSize.cx; cy += fSize.cy; cz += fSize.cz;
}

// Subtracts a size from this CfSize.
void CfSize3D::operator -=(FSIZE3D fSize)
{
	cx -= fSize.cx; cy -= fSize.cy; cz -= fSize.cz;
}

// This operation adds two CSize values.
CfSize3D CfSize3D::operator +(FSIZE3D fSize) const
{
	return CfSize3D(cx+fSize.cx, cy+fSize.cy, cz+fSize.cz);
}

// The cx and cy members of this CSize value are added
// to the x and y data members of the POINT value
CfPoint3D CfSize3D::operator +(FPOINT3D fPoint ) const
{
	return CfPoint3D(cx + fPoint.x, cy + fPoint.y, cz + fPoint.z);
}

// This operation subtracts two CSize values.
CfSize3D CfSize3D::operator -(FSIZE3D fSize) const
{
	return CfSize3D(cx-fSize.cx, cy-fSize.cy, cz-fSize.cz);
}

// This operation offsets (moves) aFPOINT or CfPoint value by the additive inverse 
// of this CfSize value. The cx and cy of this CfSize value are subtracted from 
// the x and y data members of the FPOINT value. 
// It is analogous to the version of CfPoint::operator - that takes aFSIZE parameter.
CfPoint3D CfSize3D::operator -(FPOINT3D fPoint) const
{
	return CfPoint3D(cx-fPoint.x, cy-fPoint.y, cz-fPoint.z);
}

// This operation offsets (moves) aRECT or CRect value by the additive inverse of this CSize value.
// The cx and cy members of this CSize value are subtracted from the left, top, right, and bottom data members of the RECT value. 
// It is analogous to the version of CRect::operator - that takes aSIZE parameter.
//CfRect operator -(const FRECT* lpfRect ) const;
//{
//}

// This operation returns the additive inverse of this CSize value.
CfSize3D CfSize3D::operator -() const
{
	return CfSize3D(-cx,-cy,-cx);
}

///////////////////////////////////////////////////////
// CfLine Class
///////////////////////////////////////////////////////
CfLine::CfLine(){}
CfLine::CfLine(FPOINT fptStart,FPOINT fptEnd){
	p1=fptStart;
	p2=fptEnd;
}
CfLine::CfLine(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfLine::~CfLine(){}

double CfLine::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

CfPoint CfLine::CenterPoint()
{
	return CfPoint((p2.x-p1.x)/2.0+p1.x,(p2.y-p1.y)/2.0+p1.y);
}

double CfLine::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

///////////////////////////////////////////////////////
// CfLGrid Class
///////////////////////////////////////////////////////
CfLGrid::CfLGrid()
{
	m_fStartX = 0.0; m_fStartY = 0.0; m_fPitchX = 0.0; 
	m_fPitchY = 0.0; m_nCols = 0; m_nRows = 0;
	m_fSpeed = 0.0; m_fRepRate = 0.0;
}
CfLGrid::CfLGrid(FPOINT fptStart,FPOINT fptEnd)
{
	p1=fptStart;
	p2=fptEnd;
}
CfLGrid::CfLGrid(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfLGrid::~CfLGrid(){}

double CfLGrid::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

CfPoint CfLGrid::CenterPoint()
{
	return CfPoint((p2.x-p1.x)/2.0+p1.x,(p2.y-p1.y)/2.0+p1.y);
}

double CfLGrid::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void CfLGrid::Set(double fStartX, double fStartY, double fPitchX, double fPitchY, int nCols, int nRows, double fSpeed, double fRepRate)
{
	m_fStartX = fStartX; m_fStartY = fStartY; m_fPitchX = fPitchX; 
	m_fPitchY = fPitchY; m_nCols = nCols; m_nRows = nRows;
	m_fSpeed = fSpeed; m_fRepRate = fRepRate;
}


///////////////////////////////////////////////////////
// CfArc Class
///////////////////////////////////////////////////////
CfArc::CfArc()
{
	m_fptCenter = CfPoint(0.0, 0.0);
	m_fRadius = 0.0; m_fStartAngle = 0.0; m_fArcAngle = 0.0; 
	m_fSpotSize = 0.0; m_fSpeed = 0.0; m_fRepRate = 0.0; m_fSpotSize = 0.0;
	m_nDir = 1;
}
CfArc::CfArc(FPOINT fptStart,FPOINT fptEnd)
{
	p1=fptStart;
	p2=fptEnd;
}
CfArc::CfArc(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfArc::~CfArc(){}

double CfArc::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

double CfArc::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void CfArc::Set(CfPoint fptCenter, double fRadius, double fStartAngle, double fArcAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir)
{
	m_fptCenter = fptCenter;
	m_fRadius = fRadius; m_fStartAngle = fStartAngle; m_fArcAngle = fArcAngle; m_fSpeed = fSpeed; 
	m_fSpotSize = fRepRate; m_fRepRate = fRepRate; m_fSpotSize = fSpotSize;
	m_nDir = nDir;
}


///////////////////////////////////////////////////////
// CfCircle Class
///////////////////////////////////////////////////////
CfCircle::CfCircle()
{
	m_fptCenter = CfPoint(0.0, 0.0);
	m_fRadius = 0.0; m_fStartAngle = 0.0;
	m_fSpotSize = 0.0; m_fSpeed = 0.0; m_fRepRate = 0.0; m_fSpotSize = 0.0;
	m_nDir = 1;
}
CfCircle::CfCircle(FPOINT fptStart,FPOINT fptEnd)
{
	p1=fptStart;
	p2=fptEnd;
}
CfCircle::CfCircle(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfCircle::~CfCircle(){}

double CfCircle::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

double CfCircle::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void CfCircle::Set(CfPoint fptCenter, double fRadius, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir)
{
	m_fptCenter = fptCenter;
	m_fRadius = fRadius; m_fStartAngle = fStartAngle; m_fSpeed = fSpeed; 
	m_fSpotSize = fRepRate; m_fRepRate = fRepRate; m_fSpotSize = fSpotSize;
	m_nDir = nDir;
}


///////////////////////////////////////////////////////
// CfTrepan Class
///////////////////////////////////////////////////////
CfTrepan::CfTrepan()
{
	m_fptCenter = CfPoint(0.0, 0.0);
	m_fOuterDiameter = 0.0; m_fInnerDiameter = 0.0; m_fStartAngle = 0.0;
	m_fSpotSize = 0.0; m_fSpeed = 0.0; m_fRepRate = 0.0; m_fSpotSize = 0.0;
	m_nDir = 1;
}
CfTrepan::CfTrepan(FPOINT fptStart,FPOINT fptEnd)
{
	p1=fptStart;
	p2=fptEnd;
}
CfTrepan::CfTrepan(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfTrepan::~CfTrepan(){}

double CfTrepan::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

double CfTrepan::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void CfTrepan::Set(CfPoint fptCenter, double fOuterDiameter, double fInnerDiameter, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir)
{
	m_fptCenter = fptCenter;
	m_fOuterDiameter = fOuterDiameter; m_fInnerDiameter = fInnerDiameter; m_fStartAngle = fStartAngle; m_fSpeed = fSpeed; 
	m_fSpotSize = fRepRate; m_fRepRate = fRepRate; m_fSpotSize = fSpotSize;
	m_nDir = nDir;
}


///////////////////////////////////////////////////////
// CfSpiral Class
///////////////////////////////////////////////////////
CfSpiral::CfSpiral()
{
	m_fptCenter = CfPoint(0.0, 0.0);
	m_fRadialPitch = 0.0;
	m_fOuterDiameter = 0.0; m_fInnerDiameter = 0.0; m_fStartAngle = 0.0;
	m_fSpotSize = 0.0; m_fSpeed = 0.0; m_fRepRate = 0.0; m_fSpotSize = 0.0;
	m_nDir = 1;
	m_bLastCircle = FALSE;
}
CfSpiral::CfSpiral(FPOINT fptStart,FPOINT fptEnd)
{
	p1=fptStart;
	p2=fptEnd;
}
CfSpiral::CfSpiral(double sx,double sy,double ex,double ey)
{
	p1 = CfPoint(sx,sy);
	p2 = CfPoint(ex,ey);
}
CfSpiral::~CfSpiral(){}

double CfSpiral::Angle()
{
	return RAD2DEG*(atan2((p2.y - p1.y),(p2.x - p1.x)));
}

double CfSpiral::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}

void CfSpiral::Set(CfPoint fptCenter, double fOuterDiameter, double fInnerDiameter, double fRadialPitch, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir, BOOL bLastCircle)
{
	m_fptCenter = fptCenter;
	m_fRadialPitch = fRadialPitch;
	m_fOuterDiameter = fOuterDiameter; m_fInnerDiameter = fInnerDiameter; m_fStartAngle = fStartAngle; m_fSpeed = fSpeed; 
	m_fSpotSize = fRepRate; m_fRepRate = fRepRate; m_fSpotSize = fSpotSize;
	m_nDir = nDir;
	m_bLastCircle = bLastCircle;
}


///////////////////////////////////////////////////////
// CfLine3D Class
///////////////////////////////////////////////////////
CfLine3D::CfLine3D(){}
CfLine3D::CfLine3D(FPOINT3D fptStart,FPOINT3D fptEnd){
	p1=fptStart;
	p2=fptEnd;
}
CfLine3D::CfLine3D(double sx,double sy,double sz,double ex,double ey,double ez)
{
	p1 = CfPoint3D(sx,sy,sz);
	p2 = CfPoint3D(ex,ey,ez);
}
CfLine3D::~CfLine3D(){}

CfPoint3D CfLine3D::CenterPoint()
{
	return CfPoint3D((p2.x-p1.x)/2.0+p1.x,(p2.y-p1.y)/2.0+p1.y,(p2.z-p1.z)/2.0+p1.z);
}

double CfLine3D::Length()
{
	return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));
}




//////////////////////////////////////////////////////////////////////
// CfRect Class
//////////////////////////////////////////////////////////////////////
CfRect::CfRect(){}
CfRect::CfRect(double l, double t, double r, double b)
{
	left = l;	top = t;	right = r;	bottom = b;
	m_fWidth = r - l;
	m_fHeight= t - b;
}
CfRect::~CfRect(){}

double CfRect::Width(){
	return m_fWidth = right-left;
}
double CfRect::Height(){
	return m_fHeight = top-bottom;
}
CfSize CfRect::Size(){
	CfSize fSize;
	fSize.cx = m_fWidth;
	fSize.cy = m_fHeight;
	return fSize;
}
CfPoint CfRect::TopLeft(){
	CfPoint fPt(left,top);
	return fPt;
}
CfPoint CfRect::BottomRight(){
	CfPoint fPt(right,bottom);
	return fPt;
}
CfPoint CfRect::CenterPoint(){
	CfPoint fPt;
	fPt.x = (right-left)/2.;
	fPt.y = (bottom-top)/2.;
	return fPt;
}
BOOL CfRect::IsRectEmpty(){
	if(m_fHeight <=0. || m_fWidth <= 0.)
		return TRUE;
	return FALSE;
}
BOOL CfRect::IsRectNull(){
	if(left == 0. && top == 0. && right == 0. && bottom == 0.)
		return TRUE;
	return FALSE;
}
BOOL CfRect::PtInRect(CfPoint fPt){
	if(fPt.x >= left && fPt.x < right && fPt.y < top && fPt.y >= bottom)
		return TRUE;
	return FALSE;
}
void CfRect::SetRect(double x1,double y1,double x2,double y2)
{
	left = x1;
	top = y1;
	right = x2;
	bottom = y2;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::SetRectEmpty(){
	left = right = top = bottom = 0.;
	m_fWidth = 0.;
	m_fHeight = 0.;
}
void CfRect::CopyRect(LPCFRECT lpSrcfRect){
	left = lpSrcfRect->left;
	right = lpSrcfRect->right;
	top = lpSrcfRect->top;
	bottom = lpSrcfRect->bottom;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
BOOL CfRect::EqualRect(LPCFRECT lpfRect){
	if( lpfRect->left == left && lpfRect->top == top &&
		lpfRect->right == right && lpfRect->bottom == bottom)
		return TRUE;
	return FALSE;
}
void CfRect::InflateRect(double x,double y){
	left = left - x;
	right = right + x;
	top = top - y;
	bottom = bottom + y;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::InflateRect(FSIZE fSize){
	left = left - fSize.cx;
	right = right + fSize.cx;
	top = top - fSize.cy;
	bottom = bottom + fSize.cy;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::InflateRect(LPCFRECT lpfRect){
	left = left - lpfRect->left;
	right = right + lpfRect->right;
	top = top - lpfRect->top;
	bottom = bottom + lpfRect->bottom;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::InflateRect(double l,double t,double r,double b ){
	left = left - l;
	right = right + t;
	top = top - r;
	bottom = bottom + b;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::DeflateRect(double x,double y){
	left = left + x;
	right = right - x;
	top = top + y;
	bottom = bottom - y;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::DeflateRect(FSIZE fSize){
	left = left + fSize.cx;
	right = right - fSize.cx;
	top = top + fSize.cy;
	bottom = bottom - fSize.cy;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::DeflateRect(LPCFRECT lpfRect){
	left = left + lpfRect->left;
	right = right - lpfRect->right;
	top = top + lpfRect->top;
	bottom = bottom - lpfRect->bottom;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::DeflateRect(double l,double t,double r,double b ){
	left = left + l;
	right = right - t;
	top = top + r;
	bottom = bottom - b;
	m_fWidth = right - left;
	m_fHeight = bottom - top;
}
void CfRect::NormalizeRect(){
	if(left > right)
		swap(left,right);
	if(top > bottom)
		swap(top,bottom);
}
void CfRect::OffsetRect(double x,double y){
	left += x;
	right += x;
	top += y;
	bottom += y;
}
void CfRect::OffsetRect(FPOINT fPoint){
	left += fPoint.x;
	right += fPoint.x;
	top += fPoint.y;
	bottom += fPoint.y;
}
void CfRect::OffsetRect(FSIZE fSize){
	left += fSize.cx;
	right += fSize.cx;
	top += fSize.cy;
	bottom += fSize.cy;
}
BOOL CfRect::SubtractRect(LPCFRECT lpfRectSrc1, LPCFRECT lpfRectSrc2 )
{
	BOOL bChanged = FALSE;
	int nOverlap = 0;
	left = lpfRectSrc1->left;
	top = lpfRectSrc1->top;
	right = lpfRectSrc1->right;
	bottom = lpfRectSrc1->bottom;
	if((lpfRectSrc2->left<=lpfRectSrc1->left) && (lpfRectSrc2->right>=lpfRectSrc1->right)) 
	{
		if((lpfRectSrc2->top > lpfRectSrc1->top) && (lpfRectSrc2->top < lpfRectSrc1->bottom))
		{
			bottom = lpfRectSrc2->top;
			bChanged = TRUE;
		}
		if((lpfRectSrc2->bottom < lpfRectSrc1->bottom) && (lpfRectSrc2->bottom > lpfRectSrc1->top))
		{
			top = lpfRectSrc2->bottom;
			bChanged = TRUE;
		}
		nOverlap++;
	}
	if((lpfRectSrc2->top<=lpfRectSrc1->top) && (lpfRectSrc2->bottom>=lpfRectSrc1->bottom)) 
	{
		if((lpfRectSrc2->left > lpfRectSrc1->left) && (lpfRectSrc2->left < lpfRectSrc1->right))
		{
			right = lpfRectSrc2->left;
			bChanged = TRUE;
		}
		if((lpfRectSrc2->right < lpfRectSrc1->right) && (lpfRectSrc2->right > lpfRectSrc1->left))
		{
			left = lpfRectSrc2->right;
			bChanged = TRUE;
		}
		nOverlap++;
	}
	if(nOverlap == 2)
		left = right = top = bottom = 0.;
	return bChanged;
}

BOOL CfRect::IntersectRect(LPCFRECT lpfRect1, LPCFRECT lpfRect2){
	BOOL bChanged = FALSE;
	left = 0.;
	top = 0.;
	right = 0.;
	bottom = 0.;

	if((lpfRect2->left > lpfRect1->left) && (lpfRect2->left < lpfRect1->right)) 
	{
		left = lpfRect2->left;
		bChanged = TRUE;
	}

	if((lpfRect2->right > lpfRect1->left) && (lpfRect2->right < lpfRect1->right)) 
	{
		right = lpfRect2->right;
		bChanged = TRUE;
	}
	
	if((lpfRect2->top > lpfRect1->top) && (lpfRect2->top < lpfRect1->bottom)) 
	{
		top = lpfRect2->top;
		bChanged = TRUE;
	}
	
	if((lpfRect2->bottom > lpfRect1->top) && (lpfRect2->bottom < lpfRect1->bottom)) 
	{
		bottom = lpfRect2->bottom;
		bChanged = TRUE;
	}
	return bChanged;
}

BOOL CfRect::UnionRect(LPCFRECT lpfRect1, LPCFRECT lpfRect2){

	return FALSE;
}

CfQuad::CfQuad(){}
CfQuad::~CfQuad(){}
CfQuad::CfQuad(FPOINT fptLT, FPOINT fptRT, FPOINT fptRB, FPOINT fptLB)
{
	lefttop = fptLT;
	righttop = fptRT;
	rightbottom = fptRB;
	leftbottom = fptLB;
}
CfQuad::CfQuad(FRECT rect)
{
	lefttop = CfPoint(rect.left,rect.top);
	righttop = CfPoint(rect.right,rect.top);
	rightbottom = CfPoint(rect.right,rect.bottom);
	leftbottom = CfPoint(rect.left,rect.bottom);
}
CfQuad::CfQuad(double ltx,double lty,double rtx,double rty,double rbx,double rby,double lbx,double lby)
{
	lefttop = CfPoint(ltx,lty);
	righttop = CfPoint(rtx,rty);
	rightbottom = CfPoint(rbx,rby);
	leftbottom = CfPoint(lbx,lby);
}
void CfQuad::OffsetQuad(double x,double y){
	lefttop.x += x; 
	lefttop.y += y; 
	righttop.x += x; 
	righttop.y += y; 
	rightbottom.x += x; 
	rightbottom.y += y; 
	leftbottom.x += x; 
	leftbottom.y += y; 
}
void CfQuad::OffsetQuad(FPOINT fPoint){
	lefttop.x += fPoint.x; 
	lefttop.y += fPoint.y; 
	righttop.x += fPoint.x; 
	righttop.y += fPoint.y; 
	rightbottom.x += fPoint.x; 
	rightbottom.y += fPoint.y; 
	leftbottom.x += fPoint.x; 
	leftbottom.y += fPoint.y; 
}
void CfQuad::OffsetQuad(FSIZE fSize){
	lefttop.x += fSize.cx; 
	lefttop.y += fSize.cy; 
	righttop.x += fSize.cx; 
	righttop.y += fSize.cy; 
	rightbottom.x += fSize.cx; 
	rightbottom.y += fSize.cy; 
	leftbottom.x += fSize.cx; 
	leftbottom.y += fSize.cy;
}
CfPoint CfQuad::TopLeft()
{
	CfPoint fPt(lefttop.x,lefttop.y);
	return fPt;
}
CfPoint CfQuad::TopRight()
{
	CfPoint fPt(righttop.x,righttop.y);
	return fPt;
}
CfPoint CfQuad::BottomRight()
{
	CfPoint fPt(rightbottom.x,rightbottom.y);
	return fPt;
}
CfPoint CfQuad::BottomLeft()
{
	CfPoint fPt(leftbottom.x,leftbottom.y);
	return fPt;
}

// 4각형의 중심점을 리턴한다.
CfPoint CfQuad::CenterPoint(){
	
	// 사각형으로 부터 4개의 선분을 추출한다.
	CfLine flnLeft(BottomLeft(),TopLeft());
	CfLine flnTop(TopLeft(),TopRight());
	CfLine flnRight(TopRight(),BottomRight());
	CfLine flnBottom(BottomRight(),BottomLeft());

	// 상하 및 좌우 2개의 대변의 중심점을 잇는 선분의 교점을 구한다.
	CfPoint fPt;
	GetIntersectPoint(flnLeft.CenterPoint(),
					flnRight.CenterPoint(),
					flnTop.CenterPoint(),
					flnBottom.CenterPoint(),fPt);
	return fPt;	
}

// 4각형의 대각선의 교점을 리턴한다.
BOOL CfQuad::DiagonalPoint(FPOINT &fPt)
{
	return GetIntersectPoint(TopLeft(),BottomRight(),TopRight(),BottomLeft(),fPt);
}

// 4각형의 무게 중심점을 리턴한다.
CfPoint CfQuad::CentralPoint()
{
	double p[8];
	double area = 0.0;
	double cx = 0.0;
	double cy = 0.0;
	double x1,y1,x2,y2;
	int i,j,k,l;

	p[0] = BottomLeft().x;
	p[1] = BottomLeft().y;
	p[2] = BottomRight().x;
	p[3] = BottomRight().y;
	p[4] = TopRight().x;
	p[5] = TopRight().y;
	p[6] = TopLeft().x;
	p[7] = TopLeft().y;

	for(i = 0; i < 4; i++)
	{
		j = (i+1)%4;
		k = i*2;
		l = j*2;

		x1 = p[k];
		y1 = p[k+1];
		x2 = p[l];
		y2 = p[l+1];

		area += ((x1 * y2) - (x2 * y1));
		cx += ((x1 + x2) * ((x1 * y2) - (x2 * y1)));
		cy += ((y1 + y2) * ((x1 * y2) - (x2 * y1)));

	}

	area /= 2.0;
	area = fabs(area);

//	fPt.x = cx / (area * 6.0);
//	fPt.y = cy / (area * 6.0);

	return CfPoint(cx / (area * 6.0),cy / (area * 6.0));

}

// Determines whether CRect is empty.
// A rectangle is empty if the width and/or height are 0 or negative.
BOOL CfQuad::IsQuadEmpty(){
//	if(m_fHeight <=0. || m_fWidth <= 0.)
//		return TRUE;
	return FALSE;
}
// Nonzero if CRect’s top, left, bottom, and right values are all equal to 0;
// otherwise 0.
BOOL CfQuad::IsQuadNull(){
	if(lefttop.x == 0.		&& lefttop.y == 0.		&& righttop.x == 0.		&& righttop.y == 0. && 
	   rightbottom.x == 0.	&& rightbottom.y == 0.	&& leftbottom.x == 0.	&& leftbottom.y == 0.)
		return TRUE;
	return FALSE;
}
BOOL CfQuad::PtInQuad(FPOINT fPt)
{
	// This will only handle convex quadrilaterals.
	double angle_12x = GetInteriorAngle( lefttop,righttop,fPt);
	double angle_123 = GetInteriorAngle( lefttop,righttop,rightbottom);
	if ( angle_12x > angle_123 )
		return FALSE;

	double angle_23x = GetInteriorAngle( righttop,rightbottom,fPt);
	double angle_234 = GetInteriorAngle( righttop,rightbottom,leftbottom);
	if ( angle_23x > angle_234 )
		return FALSE;

	double angle_34x = GetInteriorAngle( rightbottom,leftbottom,fPt);
	double angle_341 = GetInteriorAngle( rightbottom,leftbottom,lefttop);
	if ( angle_34x > angle_341 )
		return FALSE;

	double angle_41x = GetInteriorAngle(leftbottom,lefttop,fPt);
	double angle_412 = GetInteriorAngle(leftbottom,lefttop,righttop);
	if ( angle_41x > angle_412 )
		return FALSE;

	return TRUE;
}
void CfQuad::SetQuad(double x1,double y1,double x2,double y2,
					 double x3,double y3,double x4,double y4)
{
	lefttop = CfPoint(x1,y1);
	righttop = CfPoint(x2,y2);
	rightbottom = CfPoint(x3,y3);
	leftbottom = CfPoint(x4,y4);
}
void CfQuad::SetQuadEmpty()
{
	lefttop = CfPoint(0.,0.);
	righttop = CfPoint(0.,0.);
	rightbottom = CfPoint(0.,0.);
	leftbottom = CfPoint(0.,0.);
}
// Copies the lpSrcfQuad quadrilateral into CfQuad
void CfQuad::CopyQuad(LPCFQUAD lpSrcfQuad)
{
	*(FQUAD*)this = *lpSrcfQuad;
}
// Nonzero if the two quadrilateralS have the same top, left, bottom, and right values;
// otherwise 0.
BOOL CfQuad::EqualQuad(LPCFQUAD lpfQuad)
{
	if((CfPoint)lefttop == lpfQuad->lefttop && (CfPoint)lpfQuad->righttop == righttop &&
		(CfPoint)lpfQuad->rightbottom == rightbottom && (CfPoint)lpfQuad->leftbottom == leftbottom)
		return TRUE;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
CfQuad3D::CfQuad3D(){}
CfQuad3D::~CfQuad3D(){}
CfQuad3D::CfQuad3D(FPOINT3D fptLT, FPOINT3D fptRT, FPOINT3D fptRB, FPOINT3D fptLB)
{
	lefttop = fptLT;
	righttop = fptRT;
	rightbottom = fptRB;
	leftbottom = fptLB;
}

CfQuad3D::CfQuad3D(CfPoint3D fptLT, CfPoint3D fptRT, CfPoint3D fptRB, CfPoint3D fptLB)
{
	lefttop = fptLT;
	righttop = fptRT;
	rightbottom = fptRB;
	leftbottom = fptLB;
}


CfQuad3D::CfQuad3D(double ltx,double lty,double ltz,double rtx,double rty,double rtz,double rbx,double rby,double rbz,double lbx,double lby,double lbz)
{
	lefttop = CfPoint3D(ltx,lty,ltz);
	righttop = CfPoint3D(rtx,rty,rtz);
	rightbottom = CfPoint3D(rbx,rby,rbz);
	leftbottom = CfPoint3D(lbx,lby,lbz);
}


void CfQuad3D::MakeQuadricArray(BOOL SetValue)
{
	if(SetValue == TRUE)
	{
		m_QuadArray[0]=(CfPoint3D)leftbottom; // 좌하 
		m_QuadArray[1]=(CfPoint3D)lefttop; // 좌상 
		m_QuadArray[2]=(CfPoint3D)righttop;
		m_QuadArray[3]=(CfPoint3D)rightbottom;
	}
	else
	{
		leftbottom=m_QuadArray[0]; // 좌하 
		lefttop=m_QuadArray[1]; // 좌상 
		righttop=m_QuadArray[2];
		rightbottom=m_QuadArray[3];

	}
}

void CfQuad3D::MakeQuadricTriangle()
{
//	CenterPoint();

	m_QuadTriangles[0].Pt1 = (CfPoint3D)lefttop;
	m_QuadTriangles[0].Pt2 = (CfPoint3D)leftbottom; 
	m_QuadTriangles[0].Pt3 = (CfPoint3D)m_CenterPoint3D;
				
				//CREATE FACE 2
	m_QuadTriangles[1].Pt1 = (CfPoint3D)leftbottom;
	m_QuadTriangles[1].Pt2 = (CfPoint3D)rightbottom;
	m_QuadTriangles[1].Pt3 = (CfPoint3D)m_CenterPoint3D; 
				
				//CREATE FACE 3
	m_QuadTriangles[2].Pt1 = (CfPoint3D)rightbottom;
	m_QuadTriangles[2].Pt2 = (CfPoint3D)righttop; 
	m_QuadTriangles[2].Pt3 = (CfPoint3D)m_CenterPoint3D;

				//CREATE FACE 4
	m_QuadTriangles[3].Pt1 = (CfPoint3D)righttop; 
	m_QuadTriangles[3].Pt2 = (CfPoint3D)lefttop; 
	m_QuadTriangles[3].Pt3 = (CfPoint3D)m_CenterPoint3D; 
}

void CfQuad3D::OffsetQuad(double x,double y,double z)
{
	lefttop.x += x; 
	lefttop.y += y; 
	lefttop.z += z; 
	righttop.x += x; 
	righttop.y += y; 
	righttop.z += z; 
	rightbottom.x += x; 
	rightbottom.y += y; 
	rightbottom.z += z; 
	leftbottom.x += x; 
	leftbottom.y += y; 
	leftbottom.z += z; 
}
void CfQuad3D::OffsetQuad(FPOINT3D fPoint){
	lefttop.x += fPoint.x; 
	lefttop.y += fPoint.y; 
	lefttop.z += fPoint.z; 

	righttop.x += fPoint.x; 
	righttop.y += fPoint.y; 
	righttop.z += fPoint.z; 
	
	rightbottom.x += fPoint.x; 
	rightbottom.y += fPoint.y; 
	rightbottom.z += fPoint.z; 
	
	leftbottom.x += fPoint.x; 
	leftbottom.y += fPoint.y; 
	leftbottom.z += fPoint.z; 
}

CfPoint3D CfQuad3D::TopLeft()
{
	CfPoint3D fPt(lefttop.x,lefttop.y,lefttop.z);
	return fPt;
}
CfPoint3D CfQuad3D::TopRight()
{
	CfPoint3D fPt(righttop.x,righttop.y,righttop.z);
	return fPt;
}
CfPoint3D CfQuad3D::BottomRight()
{
	CfPoint3D fPt(rightbottom.x,rightbottom.y,rightbottom.z);
	return fPt;
}
CfPoint3D CfQuad3D::BottomLeft()
{
	CfPoint3D fPt(leftbottom.x,leftbottom.y,leftbottom.z);
	return fPt;
}

// 4각형의 중심점을 리턴한다.
CfPoint3D CfQuad3D::CenterPoint()
{
	
	// 사각형으로 부터 4개의 선분을 추출한다.
	/////////////////////////
	CfLine3D flnLeft(BottomLeft(),TopLeft());
	CfLine3D flnTop(TopLeft(),TopRight());
	CfLine3D flnRight(TopRight(),BottomRight());
	CfLine3D flnBottom(BottomRight(),BottomLeft());

	// 상하 및 좌우 2개의 대변의 중심점을 잇는 선분의 교점을 구한다.
	CfPoint3D fPt;
	GetIntersectPoint(flnLeft.CenterPoint(),
					flnRight.CenterPoint(),
					flnTop.CenterPoint(),
					flnBottom.CenterPoint(),fPt);

	double dTempHeight;
	CfQuad3D TempQuad;

	TempQuad.leftbottom=leftbottom;
	TempQuad.lefttop=lefttop;
	TempQuad.rightbottom=rightbottom;
	TempQuad.righttop=righttop;

	SetInterPolationMode(G_BILINEAR);
	m_CenterPoint3D=fPt;
	CalculateInterPolation(TempQuad,fPt,dTempHeight);
	m_CenterPoint3D.z = dTempHeight;

	return m_CenterPoint3D;	
}

// 4각형의 대각선의 교점을 리턴한다.
BOOL CfQuad3D::DiagonalPoint(FPOINT3D &fPt){
	return GetIntersectPoint(TopLeft(),BottomRight(),TopRight(),BottomLeft(),fPt);
}
// Determines whether CRect is empty.
// A rectangle is empty if the width and/or height are 0 or negative.
BOOL CfQuad3D::IsQuadEmpty(){
//	if(m_fHeight <=0. || m_fWidth <= 0.)
//		return TRUE;
	return FALSE;
}

// Nonzero if CRect’s top, left, bottom, and right values are all equal to 0;
// otherwise 0.
BOOL CfQuad3D::IsQuadNull(){
	if(	lefttop.x == 0.		&& lefttop.y == 0.		&&	lefttop.z == 0.	&&
		righttop.x == 0.	&& righttop.y == 0.		&&  righttop.z == 0. && 
		rightbottom.x == 0.	&& rightbottom.y == 0.	&&	rightbottom.z == 0.	&&
		leftbottom.x == 0.	&& leftbottom.y == 0.	&&	leftbottom.z == 0.)
		return TRUE;
	return FALSE;
}
BOOL CfQuad3D::PtInQuad(FPOINT3D fPt)
{
	// This will only handle convex quadrilaterals.
	double angle_12x = GetInteriorAngle( lefttop,righttop,fPt);
	double angle_123 = GetInteriorAngle( lefttop,righttop,rightbottom);
	if ( angle_12x > angle_123 )
		return FALSE;

	double angle_23x = GetInteriorAngle( righttop,rightbottom,fPt);
	double angle_234 = GetInteriorAngle( righttop,rightbottom,leftbottom);
	if ( angle_23x > angle_234 )
		return FALSE;

	double angle_34x = GetInteriorAngle( rightbottom,leftbottom,fPt);
	double angle_341 = GetInteriorAngle( rightbottom,leftbottom,lefttop);
	if ( angle_34x > angle_341 )
		return FALSE;

	double angle_41x = GetInteriorAngle(leftbottom,lefttop,fPt);
	double angle_412 = GetInteriorAngle(leftbottom,lefttop,righttop);
	if ( angle_41x > angle_412 )
		return FALSE;

	return TRUE;
}
void CfQuad3D::SetQuad(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double x4,double y4,double z4)
{
	lefttop = CfPoint3D(x1,y1,z1);
	righttop = CfPoint3D(x2,y2,z2);
	rightbottom = CfPoint3D(x3,y3,z3);
	leftbottom = CfPoint3D(x4,y4,z4);
}
void CfQuad3D::SetQuadEmpty()
{
	lefttop = CfPoint3D(0.,0.,0.);
	righttop = CfPoint3D(0.,0.,0.);
	rightbottom = CfPoint3D(0.,0.,0.);
	leftbottom = CfPoint3D(0.,0.,0.);
}
// Copies the lpSrcfQuad quadrilateral into CfQuad
void CfQuad3D::CopyQuad(LPCFQUAD3D lpSrcfQuad)
{
	*(FQUAD3D*)this = *lpSrcfQuad;
}
// Nonzero if the two quadrilateralS have the same top, left, bottom, and right values;
// otherwise 0.
BOOL CfQuad3D::EqualQuad(LPCFQUAD3D lpfQuad)
{
	if((CfPoint3D)lefttop == lpfQuad->lefttop && (CfPoint3D)lpfQuad->righttop == righttop &&
		(CfPoint3D)lpfQuad->rightbottom == rightbottom && (CfPoint3D)lpfQuad->leftbottom == leftbottom)
		return TRUE;
	return FALSE;
}

CDefectParam::CDefectParam()
{
	nCamID = 0;				// Camera ID
	fptOrgDefPos = CfPoint(0.0,0.0);	// Original Defect Position
	fptDefPos = CfPoint(0.0,0.0);		// Modified Defect Position
	fptOffset = CfPoint(0.0,0.0);		// Offset from left(X),Bottom(Y)
	nDefCode = 0;			// Original Defect Code by AOI
	nArea = 0;				// Area Number
	nCell = 0;				// Cell Number
	ptCellPos = CPoint(0,0);		
	fDefSize = 0.0;				// Defect Size
	nPiece = 0;				// Piece number
	nStrip = 0;				// Strip number
	nNewCode = 0;			// Modified defect code by VRS
	nGridId = 0;			// Grid Id for Fixed Defect search
	nFreq = 0;				// Frequency 
}

CDefectParam::~CDefectParam()
{

}

BOOL CDefectParam::operator ==(DEFECT_PARAM Param)
{
	return (nCamID == Param.nCamID && fptOrgDefPos == Param.fptOrgDefPos && fptDefPos == Param.fptDefPos &&
		fptOffset == Param.fptOffset && nDefCode == Param.nDefCode && nArea == Param.nArea && nCell == Param.nCell &&
		ptCellPos == Param.ptCellPos && fDefSize == Param.fDefSize && nPiece == Param.nPiece && nStrip == Param.nStrip &&
		nNewCode == Param.nNewCode && nGridId == Param.nGridId && nFreq == Param.nFreq);
}

BOOL CDefectParam::operator !=(DEFECT_PARAM DefectParam)
{
	return (nCamID != DefectParam.nCamID || fptOrgDefPos != DefectParam.fptOrgDefPos || fptDefPos != DefectParam.fptDefPos ||
		fptOffset != DefectParam.fptOffset || nDefCode != DefectParam.nDefCode || nArea != DefectParam.nArea || nCell != DefectParam.nCell ||
		ptCellPos != DefectParam.ptCellPos || fDefSize != DefectParam.fDefSize || nPiece != DefectParam.nPiece || nStrip != DefectParam.nStrip ||
		nNewCode != DefectParam.nNewCode || nGridId != DefectParam.nGridId || nFreq != DefectParam.nFreq);
}

CPieceParam::CPieceParam()
{
	nCol = 0;
	nRow = 0;
	frcOrgPiece= CfRect(0.0,0.0,0.0,0.0);		// Original Piece Rectangle define
	frcPiece = CfRect(0.0,0.0,0.0,0.0);			// Piece Rectangle define
	pArPieceDef = NULL;		// Defect Information in piece area
}

CPieceParam::~CPieceParam()
{

}

void CPieceParam::operator = (PIECE_PARAM Param)
{
	frcOrgPiece = Param.frcOrgPiece;
	frcPiece = Param.frcPiece;
	pArPieceDef = Param.pArPieceDef;
}

CStripParam::CStripParam()
{
	nCol = 0;
	nRow = 0;
	frcOrgStrip= CfRect(0.0,0.0,0.0,0.0);		// Original Piece Rectangle define
	frcStrip = CfRect(0.0,0.0,0.0,0.0);			// Piece Rectangle define
	pArStripDef = NULL;		// Defect Information in piece area
}

CStripParam::~CStripParam()
{

}

void CStripParam::operator = (STRIP_PARAM Param)
{
	frcOrgStrip = Param.frcOrgStrip;
	frcStrip = Param.frcOrgStrip;
	pArStripDef = Param.pArStripDef;
}

CSerialParam::CSerialParam()
{
	strModel = _T("");		// Product Model No
	strLayer = _T("");		// Product Layer No
	strLot = _T("");			// Product Lot No
	strSerial = _T("");		// Product SerialNo
	strProductDate = _T("");	// Product Date

	fptPin = CfPoint(0.0,0.0);			// Fixed Pin Position
	frcWorkArea = CfRect(0.0,0.0,0.0,0.0);	// Working Area 
	frcOrgBoard = CfRect(0.0,0.0,0.0,0.0);	// Original PCB Rectangle define
	frcBoard = CfRect(0.0,0.0,0.0,0.0);		// Modified PCB Rectangle define

	nColOfPiece = 0;	// Column of piece
	nRowOfPiece = 0;	// Row of piece

	nColOfStrip = 0;	// number of column of strip
	nRowOfStrip = 0;	// number of Row of strip

	pArDef = NULL;			// Defect Information 
	pArPiece = NULL;		// Piece CArray Pointer
	pArStrip = NULL;		// Strip CArray Pointer
}

CSerialParam::~CSerialParam()
{

}

void CSerialParam::operator = (SERIAL_PARAM Param)
{
	strModel =Param.strModel;		// Product Model No
	strLayer = Param.strLayer;		// Product Layer No
	strLot = Param.strLot;			// Product Lot No
	strSerial = Param.strSerial;		// Product SerialNo
	strProductDate = Param.strProductDate;	// Product Date

	fptPin = Param.fptPin;			// Fixed Pin Position
	frcWorkArea = Param.frcWorkArea;	// Working Area 
	frcOrgBoard = Param.frcOrgBoard;	// Original PCB Rectangle define
	frcBoard = Param.frcBoard;		// Modified PCB Rectangle define

	nColOfPiece = Param.nColOfPiece;	// Column of piece
	nRowOfPiece = Param.nRowOfPiece;	// Row of piece

	nColOfStrip = Param.nColOfStrip;	// number of column of strip
	nRowOfStrip = Param.nRowOfStrip;	// number of Row of strip

	pArDef = Param.pArDef;			// Defect Information 
	pArPiece = Param.pArPiece;		// Piece CArray Pointer
	pArStrip = Param.pArStrip;		// Strip CArray Pointer
}

CLotParam::CLotParam()
{
	strModel.Empty();		// Product Model Name
	strLayer.Empty();		// Product Layer Name
	strLot.Empty();			// Product Lot Name
	pArLot = NULL;			// Defect Information in lot
}

CLotParam::~CLotParam()
{

}

void CLotParam::operator = (LOT_PARAM Param)
{
	strModel = Param.strModel;		// Product Model Name
	strLayer = Param.strLayer;		// Product Layer Name
	strLot = Param.strLot;			// Product Lot Name
	pArLot = Param.pArLot;			// Defect Information in lot
}

CLayerParam::CLayerParam()
{
	strModel = _T("");		// Product Model No
	strLot = _T("");			// Product Lot No
	strLayer = _T("");		// Product Layer No

	fptPin = CfPoint(0.0,0.0);			// Fixed Pin Position
	frcWorkArea = CfRect(0.0,0.0,0.0,0.0);	// Working Area 
	frcOrgBoard = CfRect(0.0,0.0,0.0,0.0);	// Original PCB Rectangle define
	frcBoard = CfRect(0.0,0.0,0.0,0.0);		// Modified PCB Rectangle define

	nColOfPiece = 0;	// Column of piece
	nRowOfPiece = 0;	// Row of piece

	nColOfStrip = 0;	// number of column of strip
	nRowOfStrip = 0;	// number of Row of strip

	pArLayer = NULL;		// Layer CArray Pointer
}

CLayerParam::~CLayerParam()
{

}

void CLayerParam::operator = (LAYER_PARAM Param)
{
	strModel =Param.strModel;		// Product Model No
	strLot = Param.strLot;			// Product Lot No
	strLayer = Param.strLayer;		// Product Layer No

	fptPin = Param.fptPin;			// Fixed Pin Position
	frcWorkArea = Param.frcWorkArea;	// Working Area 
	frcOrgBoard = Param.frcOrgBoard;	// Original PCB Rectangle define
	frcBoard = Param.frcBoard;		// Modified PCB Rectangle define

	nColOfPiece = Param.nColOfPiece;	// Column of piece
	nRowOfPiece = Param.nRowOfPiece;	// Row of piece

	nColOfStrip = Param.nColOfStrip;	// number of column of strip
	nRowOfStrip = Param.nRowOfStrip;	// number of Row of strip

	pArLayer = Param.pArLayer;		// Layer CArray Pointer
}

CModelParam::CModelParam()
{
	strModel.Empty();			// Product Model No
	pArModel = NULL;		// Model CArray Pointer
}

CModelParam::~CModelParam()
{

}

void CModelParam::operator = (MODEL_PARAM Param)
{
	strModel =Param.strModel;		// Product Model No
	pArModel = Param.pArModel;		// Model CArray Pointer
}

//++++++++++++++++++++++++++++++++++++++++
CUserInfo::CUserInfo()
{
	ID = _T("");	// ID
	PLevel = _T("");	// Permission Level
	PW = _T("");	// PERSONAL Password
}

CUserInfo::~CUserInfo()
{

}

void CUserInfo::operator = (USER_INFO Param)
{
	ID = Param.ID;	// ID
	PLevel = Param.PLevel;	// 부서
	PW = Param.PW;	// PERSONAL Password
}

CRedoUndo::CRedoUndo()
{
	// initialize
	nActCode = 0;
	fptCamPos = CfPoint3D(0.0,0.0,0.0);
	fptCamAngle = CfPoint3D(0.0,0.0,0.0);
}

CRedoUndo::~CRedoUndo()
{
}

CRedoUndo::CRedoUndo(UINT nActCode,FPOINT fptCamPos, FPOINT fptCamAngle)
{
	nActCode = nActCode;
	fptCamPos = fptCamPos;
	fptCamAngle = fptCamAngle;
}

CRedoUndo::CRedoUndo(UINT nActCode,double fCamPosX,double fCamPosY,double fCamPosZ,double fptCamAngleX,double fptCamAngleY,double fptCamAngleZ)
{
	nActCode = nActCode;
	fptCamPos =		CfPoint3D(fCamPosX,fCamPosY,fCamPosZ);
	fptCamAngle =	CfPoint3D(fptCamAngleX,fptCamAngleY,fptCamAngleZ);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
// Laser Cutting machine data structure
// CLcmData::CLcmData()
// {
// 	// initialize
// 	index = 0;				// data number
// 	strip = 0;				// strip index
// 	piece = 0;				// piece index
// 	row = 0;				// row number
// 	col = 0;				// column number
// 	line = CfLine(0,0,0,0);	// cut line 	
// 	center = CfPoint(0.0,0.0);	// line center		
// 	length = 0.0;				// line length
// 	distance = 0.0;			// distance from origin
// 	bad = FALSE;			// bad piece flag
// }
// 
// CLcmData::~CLcmData()
// {
// }
// 
// CLcmData::CLcmData(int nIndex,int nStrip, int nPiece, int nRow, int nCol, FPOINT start, FPOINT end, BOOL bBad)
// {
// 	index = nIndex;
// 	strip = nStrip;
// 	piece = nPiece;	
// 	row = nRow;
// 	col = nCol;
// 	line.p1 = start;
// 	line.p2 = end;
// 	center.x = (end.x-start.x)/2.0+start.x;
// 	center.y = (end.y-start.y)/2.0+start.y;
// 	length = sqrt((end.x-start.x)*(end.x-start.x)+(end.y-start.y)*(end.y-start.y));
// 	distance = sqrt((center.x*center.x)+(center.y*center.y));
// 	bad = bBad;
// }
// 
// CLcmData::CLcmData(int nindex, int nStrip, int nPiece, int nRow, int nCol, double sx,double sy,double ex,double ey, BOOL bBad)
// {
// 	index = nindex;
// 	strip = nStrip;
// 	piece = nPiece;	
// 
// 	row = nRow;
// 	col = nCol;
// 
// 	line.p1.x = sx;
// 	line.p1.y = sy;
// 	line.p2.x = ex;
// 	line.p2.y = ey;
// 
// 	center.x = (ex-sx)/2.0+sx;
// 	center.y = (ey-sy)/2.0+sy;
// 
// 	length = sqrt((ex-sx)*(ex-sx)+(ey-sy)*(ey-sy));
// 	distance = sqrt((center.x*center.x)+(center.y*center.y));
// 	
// 	bad = bBad;
// }
// 
// CLcmDataList::CLcmDataList()
// {
// 	// pointer initialize
// 	pArLcamData = NULL;
// }
// 
// CLcmDataList::~CLcmDataList()
// {
// }









void GInterPolation::SetInterPolationMode(int mode)
{
	
	m_nInterPolationMode = mode;
	m_bCaculated=FALSE;

}
BOOL GInterPolation::CalculateInterPolation(CfPoint3D LeftBottom,CfPoint3D LeftTop,CfPoint3D RightTop,CfPoint3D RightBottom,CfPoint3D fpt,double& dResult)
{
	BOOL Ret = CalculateInterPolation(CfQuad3D(LeftTop,RightTop,RightBottom,LeftBottom),fpt,dResult);
	return Ret;
}

BOOL GInterPolation::GetBilinearIntersectPoint(CfPoint3D &fpt1,CfPoint3D &fpt2,CfPoint3D &fpt3,CfPoint3D &fpt4)
{
	if(!m_bCaculated) return FALSE;

	fpt1 = m_fptBLP1;
	fpt2 = m_fptBLP2;
	fpt3 = m_fptBLP3;
	fpt4 = m_fptBLP4;
	m_bCaculated=FALSE;
	return TRUE;
}

BOOL GInterPolation::GetBilinearIntersectPoint3D(CfQuad3D Qaud,CfPoint3D fCurPos,double& ResultHeight,CfPoint3D &fpt1,CfPoint3D &fpt2,CfPoint3D &fpt3,CfPoint3D &fpt4)
{	
	CfPoint3D Intersect_fpt_Left;
	CfPoint3D Intersect_fpt_Right;
	CfPoint3D Intersect_fpt_Top;
	CfPoint3D Intersect_fpt_Bottom;
	CfPoint3D CurrentPostion = fCurPos;
	double dHeightValue;

	CalculateInterPolation(Qaud,fCurPos,dHeightValue); // Interpolation에서 상속받은 함수.. Bilinear 보간함수. 
	ResultHeight=dHeightValue;
	GetBilinearIntersectPoint(Intersect_fpt_Bottom,Intersect_fpt_Top,Intersect_fpt_Left,Intersect_fpt_Right);
		
	CalculateInterPolation(Qaud,Intersect_fpt_Bottom,dHeightValue); // Interpolation에서 상속받은 함수.. Bilinear 보간함수. 
	Intersect_fpt_Bottom.z=dHeightValue;
	CalculateInterPolation(Qaud,Intersect_fpt_Top,dHeightValue); // Interpolation에서 상속받은 함수.. Bilinear 보간함수. 
	Intersect_fpt_Top.z=dHeightValue;
	CalculateInterPolation(Qaud,Intersect_fpt_Left,dHeightValue); // Interpolation에서 상속받은 함수.. Bilinear 보간함수. 
	Intersect_fpt_Left.z=dHeightValue;
	CalculateInterPolation(Qaud,Intersect_fpt_Right,dHeightValue); // Interpolation에서 상속받은 함수.. Bilinear 보간함수. 
	Intersect_fpt_Right.z=dHeightValue;

	//Calculatedhk GetBilinear는 한쌍이다.
	fpt1 = Intersect_fpt_Bottom;
	fpt2 = Intersect_fpt_Top;
	fpt3 = Intersect_fpt_Left;
	fpt4 = Intersect_fpt_Right;

	return TRUE;
}



BOOL GInterPolation::CalculateInterPolation(CfQuad3D fQuad,CfPoint3D fpt,double& dResult)
{
	double fH1=0,fH2=0,fH0=0;
	double a1x,a2x,a1y,a2y,c1x,c2y,q1y;
	double b1x,b2x,b1y,b2y,d1x,d2y,q1x;
	double a,b,c,R1,R2;

	if(fQuad.IsQuadNull())
	{
		return FALSE;
	}

	switch(m_nInterPolationMode)
	{
		case G_BILINEAR:

		a1x = fQuad.leftbottom.x;
		b1x = fQuad.rightbottom.x-fQuad.leftbottom.x;
		a2x = fQuad.lefttop.x;
		b2x = fQuad.righttop.x-fQuad.lefttop.x;

		a1y = fQuad.leftbottom.y;
		b1y = fQuad.rightbottom.y-fQuad.leftbottom.y;

		a2y = fQuad.lefttop.y;
		b2y = fQuad.righttop.y-fQuad.lefttop.y;
		
						
		c1x = a2x-a1x;
		d1x = b2x-b1x;
							
		c2y = a2y-a1y;
		d2y = b2y-b1y;
							
		q1y = fpt.y-a1y;
		q1x = fpt.x-a1x;					

		a=b1x * d2y - b1y * d1x;
		b=q1y*d1x-b1y*c1x+b1x*c2y-q1x*d2y;
		c=q1y*c1x - q1x*c2y;
	
		if(a != 0.0 )
			R1 = (-b+sqrt(b*b - 4*a*c))/(2*a);
		else
			R1 = -c/b;
						
		a1x = fQuad.leftbottom.x;
		b1x = fQuad.lefttop.x-fQuad.leftbottom.x;
		a2x = fQuad.rightbottom.x;
		b2x = fQuad.righttop.x-fQuad.rightbottom.x;

		a1y = fQuad.leftbottom.y;
		b1y = fQuad.lefttop.y-fQuad.leftbottom.y;
		a2y = fQuad.rightbottom.y;
		b2y = fQuad.righttop.y-fQuad.rightbottom.y;						
									
		c1x = a2x - a1x;
		d1x = b2x-b1x;
								
		c2y = a2y-a1y;
		d2y = b2y-b1y;
		q1y = fpt.y-a1y;
		q1x = fpt.x-a1x;

		a=b1x * d2y - b1y * d1x;
		b=q1y*d1x-b1y*c1x+b1x*c2y-q1x*d2y;
		c=q1y*c1x - q1x*c2y;

		if(a != 0.0 )
			R2 = (-b-sqrt(b*b - 4*a*c))/(2*a);
		else
			R2 = -c/b;

		m_fptBLP1.x = fQuad.leftbottom.x+(fQuad.rightbottom.x-fQuad.leftbottom.x)*R1;
		m_fptBLP1.y = fQuad.leftbottom.y+(fQuad.rightbottom.y-fQuad.leftbottom.y)*R1;							

		m_fptBLP2.x = fQuad.lefttop.x+(fQuad.righttop.x-fQuad.lefttop.x)*R1;
		m_fptBLP2.y = fQuad.lefttop.y+(fQuad.righttop.y-fQuad.lefttop.y)*R1;
		
		m_fptBLP3.y = fQuad.leftbottom.y+(fQuad.lefttop.y-fQuad.leftbottom.y)*R2;
		m_fptBLP3.x = fQuad.leftbottom.x+(fQuad.lefttop.x-fQuad.leftbottom.x)*R2;
		
		m_fptBLP4.y = fQuad.rightbottom.y+(fQuad.righttop.y-fQuad.rightbottom.y)*R2;
		m_fptBLP4.x = fQuad.rightbottom.x+(fQuad.righttop.x-fQuad.rightbottom.x)*R2;

		fH1 = (fQuad.rightbottom.z-fQuad.leftbottom.z)*R1 +fQuad.leftbottom.z;
		fH2 = (fQuad.righttop.z-fQuad.lefttop.z)*R1 +fQuad.lefttop.z;
		dResult = (fH2-fH1)*(R2) +fH1;

	m_bCaculated=TRUE;

	return TRUE;
		break;
	}
	return TRUE;
}



BOOL GPickFunction::GetNearestQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,CfQuad3D& GetQaud)
{
	int nQuadIndex =GetNearestQuadIndex(CurrQaud3DArray,fWorldPos,G_DONTCARE);
	if(nQuadIndex!=-1)
	{
		GetQaud= CurrQaud3DArray.GetAt(nQuadIndex);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


int GPickFunction::GetNearestQuadIndex(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,int Flag)
{
	int nQuadArrSize=CurrQaud3DArray.GetSize();

	if(nQuadArrSize ==0)
	{
		return -1;
	}
	
	register int i;
	double dLen=0.0; // 거리 
	double fMin,fMax;
	int nQuadIndex=-1;
	fMin = fMax = 1000000000000.0;

	CfPoint3D   TempFpt;
	CfQuad3D TempQuad;
	
	for(i=0; i<nQuadArrSize; i++)
	{
		TempQuad=CurrQaud3DArray.GetAt(i);
		
		TempFpt=TempQuad.CenterPoint();

		dLen = GetDistance(fWorldPos,TempFpt);	
		
		if(dLen < fMin)
		{
				fMin = dLen;
				nQuadIndex = i;
		}	

	}
		
	if(fMin < fMax)
	{
		return nQuadIndex;
	}
	else
	{
		return -1;
	}

}

int GPickFunction::GetNearestFptIndexInQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos)
{
	if(CurrQaud3DArray.GetSize()==0)
	{
		return -1;
	}

	int nQuadPickIndex = GetNearestQuadIndex(CurrQaud3DArray,fWorldPos,G_DONTCARE);

	if(nQuadPickIndex==-1)
	{
		return -1;
	}

	CfQuad3D TempQaud=CurrQaud3DArray.GetAt(nQuadPickIndex);

	if(TempQaud.IsQuadNull()==TRUE)
	{
		return -1;
	}

	TempQaud.MakeQuadricArray(TRUE);

	int nPtIndex=GetNearestPtIndex(&TempQaud.m_QuadArray[0],4,fWorldPos);

	return nPtIndex;
}


BOOL GPickFunction::GetNearestFptInQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,CfPoint3D& ResultFpt)
{
	int nQuadPickIndex = GetNearestQuadIndex(CurrQaud3DArray,fWorldPos,G_DONTCARE);

	if(nQuadPickIndex==-1)
	{
		return FALSE;
	}

	CfQuad3D TempQaud=CurrQaud3DArray.GetAt(nQuadPickIndex);
	
	if(TempQaud.IsQuadNull()==TRUE)
	{
		return FALSE;
	}

	TempQaud.MakeQuadricArray(TRUE);

	int nPtIndex=GetNearestPtIndex(TempQaud.m_QuadArray,4,fWorldPos);

	if(nPtIndex==-1)
	{
		return FALSE;
	}
	else
	{
		ResultFpt = TempQaud.m_QuadArray[nPtIndex];
		return TRUE;
	}
}

int GPickFunction::GetNearestPtIndex(CfPoint3D* CurrfptArray,int PtNum,CfPoint3D fWorldPos)
{
	register int i=0;

	double dLen=0.0; // 거리 
	double fMin,fMax;
	int nCenterIndex=-1;
	fMin = fMax = 100.0;
	

		if(CurrfptArray==NULL)
		{
			return -1;
		}
		
		for(i=0; i<PtNum; i++)
		{
			dLen = GetDistance(fWorldPos,CurrfptArray[i]);	

			if(dLen < fMin)
			{
				fMin = dLen;
				nCenterIndex = i;
			}
		}

	if(fMin < fMax)
	{
		return nCenterIndex;
	}
	else
	{
		return -1;
	}
}


int GPickFunction::GetNearestPointIndexArr(CArfPt3D* Arr,CfPoint3D fpt)
{

	CfPoint3D fpt1;
	int index =-1;
	double fLen;
	double fMin,fMax;
	
	if(Arr==NULL)
		return -1;

	fMin = fMax = 1000000000000.0;;

	int n;
	int nNumOfPoint = Arr->GetSize();

	for(n = 0; n < nNumOfPoint; n++)
	{
		fpt1 = Arr->GetAt(n);

		fLen =  GetDistance(fpt,fpt1);	
		if(fLen < fMin)
		{
			fMin = fLen;
			index = n;
		}
	}

	if(fMin < fMax)
		return index;
	else
		return -1;

}



