// MyData.h: interface for the CMyData class.
// 본클래스는 CPoint,CSize,CRect를 double형으로 확장한 클래스임.
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////                                 MyData.h Ver 1.5                              ////
////												                               ////
////                               Corp   : GIGAVIS                                ////
////																			   ////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDATA_H__B8E4F205_0089_4CD3_A7D4_EB5CEF56B11D__INCLUDED_)
#define AFX_MYDATA_H__B8E4F205_0089_4CD3_A7D4_EB5CEF56B11D__INCLUDED_

#include <afxtempl.h>
// #include "GlobalDefine.h"

#define SQR(X) ((X)*(X))

// #ifndef PI
// 	#define PI 3.141592653589793f
// #endif
// 
// #ifndef DEG2RAD
// 	#define DEG2RAD  (PI/180.0);
// #endif
// 
// #ifndef RAD2DEG
// 	#define  RAD2DEG  (180.0/PI);
// #endif

#define PI		3.14159265358979
const double DEG2RAD = PI/180.0;
const double RAD2DEG = 180.0/PI;

//보간 모드 
#define G_BILINEAR 5
#define G_DONTCARE -1 // 사용하지 않는 함수 인자에 사용.

enum VectDir{CCW=-1,PARALLEL,CW};


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class GInterPolation;

struct Matrix3by3{
	// Constructor.
	// Parameters: see member variables.
	Matrix3by3(double pv0 =0.0, double pv1=0.0, double pv2=0.0,double pv3=0.0, double pv4=0.0, double pv5=0.0,double pv6=0.0, double pv7=0.0, double pv8=1.0)
	{
		v0 = pv0;
		v1 = pv1;
		v2 = pv2;

		v3 = pv3;
		v4 = pv4;
		v5 = pv5;

		v6 = pv6;
		v7 = pv7;
		v8 = pv8;
	}
	double v0, v1, v2;
	double v3, v4, v5;
	double v6, v7, v8;
};
typedef CArray<Matrix3by3, Matrix3by3> CArMatrix3by3;

// The FPOINTC structure defines the x- and y-coordinates of a point.
struct FPOINTC{
	// Constructor.
	// Parameters: see member variables.
	FPOINTC(float px=0.0, float py=0.0)
	{
		x = px;
		y = py;
	}	
	float x; // Specifies the x-coordinate of a point.
	float y; // Specifies the y-coordinate of a point.
};
typedef CArray<FPOINTC, FPOINTC> CArFPoint;

// The FRECTC structure defines the Left-Bottom and Right-Top coordinates of a point.
struct FRECTC{
	// Constructor.
	// Parameters: see member variables.
	FRECTC(float px1=0.0,float py1=0.0,float px2=0.0,float py2=0.0)
	{
		X1 = px1;
		Y1 = py1;
		X2 = px2;
		Y2 = py2;
	}	
	float X1; // Specifies the X1-coordinate of a Left-Bottom point.
	float Y1; // Specifies the Y1-coordinate of a Left-Bottom point.
	float X2; // Specifies the X1-coordinate of a Right-Top point.
	float Y2; // Specifies the Y1-coordinate of a Right-Top point.
};
typedef CArray<FRECTC, FRECTC> CArFRect;


// The FPOINT structure defines the x- and y-coordinates of a point.
typedef struct tagfPOINT {
   double x; // Specifies the x-coordinate of a point.
   double y; // Specifies the y-coordinate of a point.
} FPOINT;

typedef struct tagfPOINT3D {
   double x; // Specifies the x-coordinate of a point.
   double y; // Specifies the y-coordinate of a point.
   double z; // Specifies the z-coordinate of a point.
   
//    //130702 LGH ADD THIS PARAM
//    double r; // Color RED 0~1.0
//    double g; // Color GREEN 0~1.0
//   double b; // Color BLUE 0~1.0
   
} FPOINT3D;

// The FSIZE structure specifies the width and height of a rectangle. 
typedef struct tagfSIZE {
    double cx; // Specifies the x-extent when a function returns. 
    double cy; // Specifies the y-extent when a function returns. 
}FSIZE;

// The FSIZE structure specifies the width and height of a rectangle. 
typedef struct tagfSIZE3D {
    double cx; // Specifies the x-extent when a function returns. 
    double cy; // Specifies the y-extent when a function returns. 
    double cz; // Specifies the z-extent when a function returns. 
}FSIZE3D;

// The FPOINT structure defines start and end of the x- and y-coordinates of a line.
typedef struct tagfLINE {
   FPOINT p1;	// Specifies the x,y-coordinate of a line.
   FPOINT p2;	 // Specifies the x,y-coordinate of a line.
} FLINE;

// The FPOINT structure defines start and end of the x- and y-coordinates of a line.
typedef struct tagfLINE3D {
   FPOINT3D p1; // Specifies the x,y-coordinate of a line.
   FPOINT3D p2;	 // Specifies the x,y-coordinate of a line.
} FLINE3D;

// The FRECT structure defines the coordinates of the upper-left and 
// lower-right corners of a rectangle.
typedef struct tagfRECT {
   double left; // Specifies the x-coordinate of the upper-left corner of a rectangle.
   double top; // Specifies the y-coordinate of the upper-left corner of a rectangle.
   double right; // Specifies the x-coordinate of the lower-right corner of a rectangle.
   double bottom; // Specifies the y-coordinate of the lower-right corner of a rectangle.
}FRECT,*LPCFRECT;

// The FQUAD structure defines the coordinates of the upper-left and 
// lower-right corners of a quadrilateral.
typedef struct tagfQUAD {
   FPOINT lefttop; // Specifies the x,y-coordinate of the upper-left corner of a quadrilateral.
   FPOINT righttop; // Specifies the x,y-coordinate of the upper-right corner of a quadrilateral.
   FPOINT rightbottom; // Specifies the x,y-coordinate of the lower-right corner of a quadrilateral.
   FPOINT leftbottom; // Specifies the x,y-coordinate of the lower-left corner of a quadrilateral.
}FQUAD,*LPCFQUAD;

typedef struct tagfQUAD3D {
   FPOINT3D lefttop;	// Specifies the x,y-coordinate of the upper-left corner of a quadrilateral in 3d.
   FPOINT3D righttop;	// Specifies the x,y-coordinate of the upper-right corner of a quadrilateral in 3d.
   FPOINT3D rightbottom; // Specifies the x,y-coordinate of the lower-right corner of a quadrilateral in 3d.
   FPOINT3D leftbottom; // Specifies the x,y-coordinate of the lower-left corner of a quadrilateral in 3d.
}FQUAD3D,*LPCFQUAD3D;

class CfPoint;
class CfPoint3D;
class CfSize;
class CfSize3D;
class CfLine;
class CfLine3D;
class CfRect;
class CfQuad;
class CfQuad3D;

class CfPoint : public tagfPOINT  
{
public:
	CfPoint();
	CfPoint(double initfX, double initfY);
	CfPoint(FPOINT initfPt);
	CfPoint(FSIZE initfSize);
	CfPoint(DWORD dwPoint);
	virtual ~CfPoint();

	void	Offset(double fXOffset,double fYOffset);
	void	Offset(FPOINT fPoint);
	void	Offset(FSIZE fSize);
	CfPoint Round( CfPoint fPoint, int nPos = 3);

	BOOL    operator ==(FPOINT fPoint); // Nonzero if the points are equal; otherwise 0.
	BOOL    operator !=(FPOINT fPoint); // Nonzero if the points are not equal; otherwise 0.

	void	operator +=(FSIZE  fSize);
	void	operator +=(FPOINT fPoint);
	void	operator -=(FSIZE  fSize);
	void	operator -=(FPOINT fPoint);
	
	CfPoint operator +(FSIZE fSize ) const;
	CfPoint operator +(FPOINT fPoint ) const;
	CfRect operator +( const FRECT* lpfRect ) const;

	CfSize operator -(FPOINT fPoint ) const;
	CfPoint operator -(FSIZE fSize ) const;
//	CfPoint operator -(FPOINT fPoint ) const; 본 함수는 CfSize에 선언됨.
	CfRect operator -(const FRECT* lpfRect ) const;
	CfPoint operator -() const;
};

class CfPoint3D : public tagfPOINT3D  
{
public:
	CfPoint3D();
	CfPoint3D(double initfX, double initfY, double initfZ);
	CfPoint3D(FPOINT3D initfPt);
	virtual ~CfPoint3D();

	void	Offset(double fXOffset,double fYOffset,double fZOffset);
	void	Offset(FPOINT3D fPoint);
	CfPoint3D Round( CfPoint3D fPoint, int nPos = 3);

	BOOL    operator ==(FPOINT3D fPoint); // Nonzero if the points are equal; otherwise 0.
	BOOL    operator !=(FPOINT3D fPoint); // Nonzero if the points are not equal; otherwise 0.

	void	operator +=(FPOINT3D fPoint);
	void	operator -=(FPOINT3D fPoint);
	
	CfPoint3D operator +(FPOINT3D fPoint ) const;
	CfPoint3D operator -(FPOINT3D fPoint ) const;

	//130703 LGH ADD START
	CfPoint3D(CfPoint3D& vt);
	CfPoint3D operator + (CfPoint3D& fPoint);
	void operator = (CfPoint3D& fPoint);
	CfPoint3D operator - (CfPoint3D& fPoint);
	CfPoint3D operator / (float div);
	CfPoint3D operator * (CfPoint3D& fDivison);
	CfPoint3D operator * (double dDivison);
	void Normalize();
	double Magnitude();
	//ADD END

};

class CfSize : public tagfSIZE  
{
public:
	CfSize();
	CfSize(double initfCX,double initfCY);
	CfSize(FSIZE initfSize);
	CfSize(FPOINT initfPt);
	CfSize(DWORD dwfSize);
	virtual ~CfSize();

	BOOL operator ==(FSIZE fSize) const; // Checks for equality between two sizes. Returns nonzero if the sizes are equal, otherwize 0.
	BOOL operator !=(FSIZE fSize) const; // Checks for inequality between two sizes. Returns nonzero if the sizes are not equal, otherwise 0.
	void operator +=(FSIZE fSize); // Adds a size to this CfSize.
	void operator -=(FSIZE fSize); // Subtracts a size from this CfSize.
	CfSize operator +(FSIZE fSize) const; // This operation adds two CSize values.
	CfPoint operator +(FPOINT fPoint ) const; // The cx and cy members of this CSize value are added to the x and y data members of the POINT value
//	CfRect operator +(const FRECT* lpfRect ) const;

	CfSize operator -(FSIZE fSize) const;
	CfPoint operator -(FPOINT fPoint) const;
//	CfRect operator -(const FRECT* lpfRect ) const;
	CfSize operator -()const;
};

class CfSize3D : public tagfSIZE3D  
{
public:
	CfSize3D();
	CfSize3D(double initfCX,double initfCY,double initfCZ);
	CfSize3D(FSIZE3D initfSize);
	CfSize3D(FPOINT3D initfPt);
	virtual ~CfSize3D();

	BOOL operator ==(FSIZE3D fSize) const; // Checks for equality between two sizes. Returns nonzero if the sizes are equal, otherwize 0.
	BOOL operator !=(FSIZE3D fSize) const; // Checks for inequality between two sizes. Returns nonzero if the sizes are not equal, otherwise 0.
	void operator +=(FSIZE3D fSize); // Adds a size to this CfSize.
	void operator -=(FSIZE3D fSize); // Subtracts a size from this CfSize.
	CfSize3D operator +(FSIZE3D fSize) const; // This operation adds two CSize values.
	CfPoint3D operator +(FPOINT3D fPoint ) const; // The cx and cy members of this CSize value are added to the x and y data members of the POINT value
//	CfRect operator +(const FRECT* lpfRect ) const;

	CfSize3D operator -(FSIZE3D fSize) const;
	CfPoint3D operator -(FPOINT3D fPoint) const;
//	CfRect operator -(const FRECT* lpfRect ) const;
	CfSize3D operator -()const;
};

class CfLine : public tagfLINE
{
public:
	CfLine();
	CfLine(FPOINT start,FPOINT end);
	CfLine(double sx,double sy,double ex,double ey);
	virtual ~CfLine();
	double Length();
	double Angle();
	CfPoint CenterPoint(void);

};

class CfLGrid : public tagfLINE
{
public:
	double m_fStartX, m_fStartY, m_fPitchX, m_fPitchY, m_fSpeed, m_fRepRate;
	int m_nCols, m_nRows;

public:
	CfLGrid();
	CfLGrid(FPOINT start,FPOINT end);
	CfLGrid(double sx,double sy,double ex,double ey);
	virtual ~CfLGrid();
	double Length();
	double Angle();
	CfPoint CenterPoint(void);
	void Set(double fStartX, double fStartY, double fPitchX, double fPitchY, int nCols, int nRows, double fSpeed, double fRepRate);
};

class CfArc : public tagfLINE
{
public:
	CfPoint m_fptCenter;
	double m_fRadius, m_fStartAngle, m_fArcAngle, m_fSpotSize, m_fSpeed, m_fRepRate;
	int m_nDir;

public:
	CfArc();
	CfArc(FPOINT start,FPOINT end);
	CfArc(double sx,double sy,double ex,double ey);
	virtual ~CfArc();
	double Length();
	double Angle();
	void Set(CfPoint fptCenter, double fRadius, double fStartAngle, double fArcAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir=1);
};

class CfCircle : public tagfLINE
{
public:
	CfPoint m_fptCenter;
	double m_fRadius, m_fStartAngle, m_fSpotSize, m_fSpeed, m_fRepRate;
	int m_nDir;

public:
	CfCircle();
	CfCircle(FPOINT start,FPOINT end);
	CfCircle(double sx,double sy,double ex,double ey);
	virtual ~CfCircle();
	double Length();
	double Angle();
	void Set(CfPoint fptCenter, double fRadius, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir=1);
};

class CfTrepan : public tagfLINE
{
public:
	CfPoint m_fptCenter;
	double m_fOuterDiameter, m_fInnerDiameter, m_fStartAngle, m_fSpotSize, m_fSpeed, m_fRepRate;
	int m_nDir;

public:
	CfTrepan();
	CfTrepan(FPOINT start,FPOINT end);
	CfTrepan(double sx,double sy,double ex,double ey);
	virtual ~CfTrepan();
	double Length();
	double Angle();
	void Set(CfPoint fptCenter, double fOuterDiameter, double fInnerDiameter, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir=1);
};

class CfSpiral : public tagfLINE
{
public:
	CfPoint m_fptCenter;
	double m_fOuterDiameter, m_fInnerDiameter, m_fStartAngle, m_fRadialPitch, m_fSpotSize, m_fSpeed, m_fRepRate;
	int m_nDir;
	BOOL m_bLastCircle;

public:
	CfSpiral();
	CfSpiral(FPOINT start,FPOINT end);
	CfSpiral(double sx,double sy,double ex,double ey);
	virtual ~CfSpiral();
	double Length();
	double Angle();
	void Set(CfPoint fptCenter, double fOuterDiameter, double fInnerDiameter, double fRadialPitch, double fStartAngle, double fSpotSize, double fSpeed, double fRepRate, int nDir=1, BOOL bLastCircle=FALSE);
};

class CfLine3D : public tagfLINE3D
{
public:
	CfLine3D();
	CfLine3D(FPOINT3D start,FPOINT3D end);
	CfLine3D(double sx,double sy,double sz,double ex,double ey,double ez);
	virtual ~CfLine3D();
	double Length();
	CfPoint3D CenterPoint(void);
};


class CfRect : public tagfRECT   
{
public:
	CfRect();
	CfRect(double left, double top, double right, double bottom);
	virtual ~CfRect();
	double Width();
	double Height();
	CfSize Size();
	CfPoint TopLeft();
	CfPoint BottomRight();
	CfPoint CenterPoint();
	BOOL IsRectEmpty(); // Determines whether CRect is empty. A rectangle is empty if the width and/or height are 0 or negative. Differs from IsRectNull, which determines whether all coordinates of the rectangle are zero.
	BOOL IsRectNull(); // Determines whether the top, left, bottom, and right values of CRect are all equal to 0.
	BOOL PtInRect(CfPoint fPt); // A point is within CRect if it lies on or within all four sides.
	void SetRect(double x1,double y1,double x2,double y2);
	void SetRectEmpty(); // Makes CfRect a null rectangle by setting all coordinates to zero
	void CopyRect(LPCFRECT lpSrcfRect); // Copies the lpSrcfRect rectangle into CfRect
	BOOL EqualRect(LPCFRECT lpfRect); // Nonzero if the two rectangles have the same top, left, bottom, and right values; otherwise 0.
	void InflateRect(double x,double y);
	void InflateRect(FSIZE fSize);
	void InflateRect(LPCFRECT lpfRect);
	void InflateRect(double l,double t,double r,double b);
	void DeflateRect(double x,double y);
	void DeflateRect(FSIZE fSize);
	void DeflateRect(LPCFRECT lpfRect);
	void DeflateRect(double l,double t,double r,double b);
	void NormalizeRect(); // Normalizes CfRect so that both the height and width are positive
	void OffsetRect(double x,double y);
	void OffsetRect(FPOINT fPoint);
	void OffsetRect(FSIZE fSize);
	BOOL SubtractRect(LPCFRECT lpfRectSrc1, LPCFRECT lpfRectSrc2 );
	BOOL IntersectRect(LPCFRECT lpfRect1, LPCFRECT lpfRect2);
	BOOL UnionRect(LPCFRECT lpfRect1, LPCFRECT lpfRect2);

public:
	double m_fWidth, m_fHeight;
};

class CfQuad : public tagfQUAD 
{
public:
	CfQuad();
	CfQuad(FPOINT fptLT, FPOINT fptRT, FPOINT fptRB, FPOINT fptLB);
	CfQuad(FRECT rect);
	CfQuad(double ltx,double lty,double rtx,double rty,double rbx,double rby,double lbx,double lby);
	virtual ~CfQuad();
	void OffsetQuad(double x,double y);
	void OffsetQuad(FPOINT fPoint);
	void OffsetQuad(FSIZE fSize);
	CfPoint TopLeft();
	CfPoint TopRight();
	CfPoint BottomRight();
	CfPoint BottomLeft();
	CfPoint CenterPoint();
	BOOL DiagonalPoint(FPOINT &fPoint);
	CfPoint CentralPoint();
	BOOL IsQuadEmpty();
	BOOL IsQuadNull();
	BOOL PtInQuad(FPOINT fPt);
	void SetQuad(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4);
	void SetQuadEmpty();
	void CopyQuad(LPCFQUAD lpSrcfQuad); // Copies the lpSrcfQuad quadrilateral into CfQuad
	BOOL EqualQuad(LPCFQUAD lpfQuad); // Nonzero if the two quadrilateralS have the same top, left, bottom, and right values; otherwise 0.
};

struct MeshTriangle
{
	CfPoint3D Pt1;
	CfPoint3D Pt2;
	CfPoint3D Pt3;
	CfPoint3D fNrm;
};


class GInterPolation  //현재는 Bilinear 차후 tri kri등..넣을것임
{
public:
int m_nInterPolationMode; 
CfPoint3D m_fptBLP1;							
CfPoint3D m_fptBLP2;
CfPoint3D m_fptBLP3;
CfPoint3D m_fptBLP4;
BOOL m_bCaculated;
void SetInterPolationMode(int mode);
BOOL CalculateInterPolation(CfPoint3D LeftBottom,CfPoint3D LeftTop,CfPoint3D RightTop,CfPoint3D RightBottom,CfPoint3D fpt,double& dResult);
BOOL CalculateInterPolation(CfQuad3D fQuad,CfPoint3D fpt,double& dResult);
BOOL GetBilinearIntersectPoint(CfPoint3D &fpt1,CfPoint3D &fpt2,CfPoint3D &fpt3,CfPoint3D &fpt4);
BOOL GetBilinearIntersectPoint3D(CfQuad3D Qaud,CfPoint3D fCurPos,double& ResultHeight,CfPoint3D &fpt1,CfPoint3D &fpt2,CfPoint3D &fpt3,CfPoint3D &fpt4);
};

/*
class GPickFunction
{
	
public:
	int	 GetNearestPtIndex(CArfQuad3D CurrQaud3DArray,CfPoint3D fWorldPos,int Flag);
	int	 GetNearestQuadIndex(CArfQuad3D CurrQaud3DArray,CfPoint3D fWorldPos,int FlagReserved);
	BOOL GetNearestQuad3D(CArfQuad3D CurrQaud3DArray,CfPoint3D fWorldPos,CfQuad3D& GetQaud);


};
*/

class CfQuad3D : public tagfQUAD3D , public GInterPolation
{
public:
	CfQuad3D();
	CfQuad3D(FPOINT3D fptLT, FPOINT3D fptRT, FPOINT3D fptRB, FPOINT3D fptLB);
	CfQuad3D(double ltx,double lty,double ltz,double rtx,double rty,double rtz,double rbx,double rby,double rbz,double lbx,double lby,double lbz);
	CfQuad3D(CfPoint3D fptLT, CfPoint3D fptRT, CfPoint3D fptRB, CfPoint3D fptLB); //130708 lgh add
	
	// 하나의 쿼드에는 4개의 삼각형을 일단 만든다...
	MeshTriangle m_QuadTriangles[4];
	void MakeQuadricTriangle(); 
	CfPoint3D m_CenterPoint3D;
	// 하나의 쿼드에는 좌상좌하우하우상을 하나의 포인터 배열로 관리하겠다.
	CfPoint3D m_QuadArray[4]; 
	void MakeQuadricArray(BOOL SetValue);

	virtual ~CfQuad3D();
	void OffsetQuad(double x,double y,double z);
	void OffsetQuad(FPOINT3D fPoint);
	CfPoint3D TopLeft();
	CfPoint3D TopRight();
	CfPoint3D BottomRight();
	CfPoint3D BottomLeft();
	CfPoint3D CenterPoint();
	BOOL DiagonalPoint(FPOINT3D &fPoint);
	BOOL IsQuadEmpty();
	BOOL IsQuadNull();
	BOOL PtInQuad(FPOINT3D fPt);
	void SetQuad(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double x4,double y4,double z4);
	void SetQuadEmpty();
	void CopyQuad(LPCFQUAD3D lpSrcfQuad); // Copies the lpSrcfQuad quadrilateral into CfQuad
	BOOL EqualQuad(LPCFQUAD3D lpfQuad); // Nonzero if the two quadrilateralS have the same top, left, bottom, and right values; otherwise 0.
};

typedef CArray<CPoint, CPoint> CArPt;						// #include <afxtempl.h>		// MFC support for Templete(CArray)
typedef CArray<CfPoint, CfPoint> CArfPt;
typedef CArray<CfRect, CfRect> CArfRect;
typedef CArray<CfQuad, CfQuad> CArfQuad;
typedef CArray<CfPoint3D, CfPoint3D> CArfPt3D;
typedef CArray<CfQuad3D, CfQuad3D> CArfQuad3D;





struct structEnumSerial{
	CString strPortName;
	CString strPortDesc;
	int nPortNum;
};




//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// OpenGLView Redo/Undo History structure
typedef struct tagRedoUndo{
	UINT	nActCode;
	CfPoint3D fptCamPos;
	CfPoint3D fptCamAngle;
}REDO_UNDO,*LPCREDO_UNDO;

class CRedoUndo;

class CRedoUndo : public tagRedoUndo 
{
public:
	CRedoUndo();
	CRedoUndo(UINT	nActCode,FPOINT fptCamPos, FPOINT fptCamAngle);
	CRedoUndo(UINT	nActCode,double fCamPosX,double fCamPosY,double fCamPosZ,double fptCamAngleX,double fptCamAngleY,double fptCamAngleZ);
	virtual ~CRedoUndo();
};
typedef CArray<CRedoUndo, CRedoUndo> CArRedoUndo;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Laser cutting machine data structure
typedef struct tagLaserPower
{
	double fVolt;
	double fWatt;
}LASER_POWER,*LPLASER_POWER;

// typedef struct stCutLine
// {
// 	int nPcsId;
// 	int nStX;
// 	int nStY;
// 	int nEdX;
// 	int nEdY;
// 	double dStX;
// 	double dStY;
// 	double dEdX;
// 	double dEdY;
// }CutLine;

// typedef struct tagLCMDATA{
//    int		index;		// data index
//    int		strip;		// strip index
//    int		piece;		// piece index
//    int		col;		// column number
//    int		row;		// row number
//    CfLine	line;		// line 	
//    FPOINT	center;		// line center		
//    double	length;		// line length
//    double	distance;	// line distance from origin
//    BOOL		bad;		// bad piece	
// }LCMDATA,*LPCLCMDDATA;
// 
// class CLcmData;
// class CLcmData : public tagLCMDATA 
// {
// public:
// 	CLcmData();
// 	virtual ~CLcmData();
// 
// 	CLcmData(int nIndex, int nStrip, int nPiece, int nRow, int nCol, FPOINT start, FPOINT end,  BOOL bBad=FALSE);
// 	CLcmData(int nIndex, int nStrip, int nPiece, int nRow, int nCol, double sx,double sy,double ex,double ey, BOOL bBad=FALSE);
// };
// typedef CArray<CLcmData, CLcmData> CArLcmData;
// 
// 
// typedef struct tagLCMDATA_LIST{
// 	CArLcmData *pArLcamData;	
// }LCMDATA_LIST,*LPCLCMDDATA_LIST;
// 
// class CLcmDataList;
// class CLcmDataList : public tagLCMDATA_LIST 
// {
// public:
// 	CLcmDataList();
// 	virtual ~CLcmDataList();
// };
// typedef CArray<CLcmDataList, CLcmDataList> CArLcmDataList;

 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AOI Data file(*.rst) directory structure
struct DEFECT_FILE_PATH{
	CString strName;	// defect file name
	CString strPath;	// defect file path
	
	// Constructor.
	DEFECT_FILE_PATH()
	{
		strName.Empty();
		strPath.Empty();
	}
};
typedef CArray<DEFECT_FILE_PATH, DEFECT_FILE_PATH> CArSerialFilePath;

struct LOT_FILE_PATH{
	CString strName;	// Lot name
	CString strPath;	// lot file path
	CArSerialFilePath *pSerialPath;	
	LOT_FILE_PATH()
	{
		strName.Empty();
		strPath.Empty();
		pSerialPath = NULL;
	}
};
typedef CArray<LOT_FILE_PATH, LOT_FILE_PATH> CArLotFilePath;

struct LAYER_FILE_PATH{
	CString strName;	// layer name
	CString strPath;	// layer file path
	CArLotFilePath *pLotPath;
	LAYER_FILE_PATH()
	{
		strName.Empty();
		strPath.Empty();
		pLotPath = NULL;
	}
};
typedef CArray<LAYER_FILE_PATH, LAYER_FILE_PATH> CArLayerFilePath;

struct MODEL_FILE_PATH{
	CString strName;	// model name
	CString strPath;	// model file path
	CArLayerFilePath *pLayerPath;
	MODEL_FILE_PATH()
	{
		strName.Empty();
		strPath.Empty();
		pLayerPath = NULL;
	}
};
typedef CArray<MODEL_FILE_PATH, MODEL_FILE_PATH> CArModelFilePath;

struct DATA_FILE_PATH{
	CString strName;	// model name
	CString strPath;	// model file path
	CArModelFilePath *pModelPath;
	DATA_FILE_PATH()
	{
		strName.Empty();
		strPath.Empty();
		pModelPath = NULL;
	}
};
typedef CArray<DATA_FILE_PATH, DATA_FILE_PATH> CArDataFilePath;


//////////////////////////////////////////////////////////
//	Type Definition for PCB Parameter
typedef struct tagDefectInfo{
	int	nShortKey;			// Short Key Code		
	CString strName;		// Modified Defct Name
	CString strNameOrg;		// Original Defct Name
	CString strEngName;		// Modified English Defct Name
	CString strEngNameOrg;	// Original English Defct Name
	CString strTitleName;	// Title Defct Name
	CString strCriticalName;// Critical Defct Name
	int		nCritical;		// Critical defect code
	COLORREF crColor;		// Defct Color
	BOOL	bLargeFont;		//
	BOOL	bViewDefect;	// View Flag
	int		nDelay;			// Point Delay Time
}sttDefectInfo;

typedef struct tagDEFECT_PARAM{
	int nCamID;				// Camera ID
	CfPoint fptOrgDefPos;	// Original Defect Position
	CfPoint fptDefPos;		// Modified Defect Position
	CfPoint fptOffset;		// Offset from left(X),Bottom(Y)
	int nDefCode;			// Original Defect Code by AOI
	int nArea;				// Area Number
	int nCell;				// Cell Number on Reference
	CPoint ptCellPos;		// Cell Position on Reference
	float fDefSize;			// Defect size
	int nPiece;				// Piece number
	int nStrip;				// Strip number
	int nNewCode;			// Modified defect code by VRS
	int nGridId;			// Grid Id for Fixed Defect search
	int nFreq;				// Frequency 
	int nDefID;				// RST Defect Order
}DEFECT_PARAM;
typedef CArray<DEFECT_PARAM, DEFECT_PARAM> CArDefect;

typedef struct tagPIECE_PARAM{
	int		nCol;				// Column Index
	int		nRow;				// Row Index
	CfRect	frcOrgPiece;		// Original Piece Rectangle define
	CfRect	frcPiece;			// Piece Rectangle define
	CArDefect *pArPieceDef;		// Defect Information in piece area
}PIECE_PARAM;
typedef CArray<PIECE_PARAM, PIECE_PARAM> CArPiece;

typedef struct tagSTRIP_PARAM{
	int		nCol;				// Column Index
	int		nRow;				// Row Index
	CfRect	frcOrgStrip;		// Original Strip Rectangle define
	CfRect	frcStrip;			// Strip Rectangle define
	CArDefect *pArStripDef;		// Defect Information in strip area
}STRIP_PARAM;
typedef CArray<STRIP_PARAM, STRIP_PARAM> CArStrip;

typedef struct PanelParam{
	CString	strModel;		// Product Model No
	CString	strLayer;		// Product Layer No
	CString	strLot;			// Product Lot No
	CString	strSerial;		// Product SerialNo
	CString	strProductDate;	// Product Date

	CfPoint fptPin;			// Fixed Pin Position
	CfRect  frcWorkArea;	// Working Area 
	CfRect	frcOrgBoard;	// Original PCB Rectangle define
	CfRect	frcBoard;		// Modified PCB Rectangle define

	CArDefect *pArDef;		// Defect Information 

	int		nColOfPiece;	// Column of piece
	int		nRowOfPiece;	// Row of piece

	CfPoint	fptStart;		// Piece Start Position
	CfSize  fszSize;		// Piece Size
	CfSize  fszPitch;		// Piece Pitch X and Y
	CArPiece *pArPiece;		// Piece CArray Pointer

	CArStrip *pArStrip;		// Strip CArray Pointer
	BOOL	bFidMark[4];	// Fiducial Mark Flag
	CfPoint fptFidMark[4];	// Fiducial Mark Position
}sttPanelParam;
typedef CArray<sttPanelParam, sttPanelParam> CArPanel;

typedef struct tagSERIAL_PARAM{
	CString	strModel;		// Product Model Name
	CString	strLayer;		// Product Layer Name
	CString	strLot;			// Product Lot Name
	CString	strSerial;		// Product SerialNo
	CString	strProductDate;	// Product Date

	CfPoint fptPin;			// Fixed Pin Position
	CfRect  frcWorkArea;	// Working Area 
	CfRect	frcOrgBoard;	// Original PCB Rectangle define
	CfRect	frcBoard;		// Modified PCB Rectangle define

	int		nColOfPiece;	// number of column of piece
	int		nRowOfPiece;	// number of Row of piece

	int		nColOfStrip;	// number of column of strip
	int		nRowOfStrip;	// number of Row of strip

	CArDefect *pArDef;		// Defect Information 
	CArPiece *pArPiece;		// Piece CArray Pointer
	CArStrip *pArStrip;		// Strip CArray Pointer
}SERIAL_PARAM;
typedef CArray<SERIAL_PARAM, SERIAL_PARAM> CArLot;

typedef struct tagLOT_PARAM{
	CString	strModel;		// Product Model Name
	CString	strLayer;		// Product Layer Name
	CString	strLot;			// Product Lot Name
	CArLot *pArLot;			// Defect Information in lot
}LOT_PARAM;

typedef CArray<LOT_PARAM, LOT_PARAM> CArLayer;

typedef struct tagLAYER_PARAM{
	CString	strModel;		// Product Model Name
	CString	strLayer;		// Product Layer Name
	CString	strLot;			// Product Lot Name

	CfPoint fptPin;			// Fixed Pin Position
	CfRect  frcWorkArea;	// Working Area 
	CfRect	frcOrgBoard;	// Original PCB Rectangle define
	CfRect	frcBoard;		// Modified PCB Rectangle define

	int		nColOfPiece;	// Column of piece
	int		nRowOfPiece;	// Row of piece

	int		nColOfStrip;	// number of column of strip
	int		nRowOfStrip;	// number of Row of strip
	
	int		nNumOfPiece;		// Column Index
	int		nNumOfStrip;		// Row Index

	CArLayer *pArLayer;			// Defect Information in piece area
}LAYER_PARAM;
typedef CArray<LAYER_PARAM, LAYER_PARAM> CArModel;

typedef struct tagMODEL_PARAM{
	CString	strModel;		// Product Model Name
	CArModel *pArModel;		// Defect Information in piece area
}MODEL_PARAM;


class CDefectParam;
class CPieceParam;
class CStripParam;

class CSerialParam;
class CLotParam;
class CLayerParam;
class CModelParam;

class CDefectParam : public tagDEFECT_PARAM  
{
public:
	CDefectParam();
	virtual ~CDefectParam();
	BOOL    operator ==(DEFECT_PARAM Param); // Nonzero if the points are equal; otherwise 0.
	BOOL    operator !=(DEFECT_PARAM Param); // Nonzero if the points are not equal; otherwise 0.
};

class CPieceParam : public tagPIECE_PARAM  
{
public:
	CPieceParam();
	virtual ~CPieceParam();
	void    operator =(PIECE_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};

class CStripParam : public tagSTRIP_PARAM  
{
public:
	CStripParam();
	virtual ~CStripParam();
	void    operator =(STRIP_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};

class CSerialParam : public tagSERIAL_PARAM  
{
public:
	CSerialParam();
	virtual ~CSerialParam();
	void    operator =(SERIAL_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};

class CLotParam : public tagLOT_PARAM  
{
public:
	CLotParam();
	virtual ~CLotParam();
	void    operator =(LOT_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};

class CLayerParam : public tagLAYER_PARAM  
{
public:
	CLayerParam();
	virtual ~CLayerParam();
	void    operator =(LAYER_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};

class CModelParam : public tagMODEL_PARAM  
{
public:
	CModelParam();
	virtual ~CModelParam();
	void    operator = (MODEL_PARAM Param); // Nonzero if the points are equal; otherwise 0.
};


//++++++++++++++++++++++++++++++++++++++++++++
// DTS 

struct RectRegion
{
	CfPoint3D fStart;
	CfPoint3D fEnd;
};

typedef struct tagPiece{
	CString strCode;	// Piece Number
	CString strName;	// Piece Name for display
}sttPieceCodeInfo;
typedef CArray<sttPieceCodeInfo, sttPieceCodeInfo> CArPieceCodeInfo;

typedef struct tagStrip{
	CString strCode;	// Strip Number
	CString strName;	// Strip Name for display
}sttStripCodeInfo;
typedef CArray<sttStripCodeInfo, sttStripCodeInfo> CArStripCodeInfo;

typedef struct tagSerial{
	CString strCode;	// Serial Code
	CString strName;	// Serial Name for display
}sttSerialCodeInfo;
typedef CArray<sttSerialCodeInfo, sttSerialCodeInfo> CArSerialCodeInfo;

typedef struct tagLot{
	CString strCode;	// Lot code
	CString strName;	// Lot Name for display
	CString strModelCode; // Model code
}sttLotCodeInfo;
typedef CArray<sttLotCodeInfo, sttLotCodeInfo> CArLotCodeInfo;

typedef struct tagLayer{
	CString strCode;	// Layer code
	CString strName;	// Layer Name for display
	CString strModelCode;
}sttLayerCodeInfo;
typedef CArray<sttLayerCodeInfo, sttLayerCodeInfo> CArLayerCodeInfo;

typedef struct tagModel{
	CString strCode;	// Model Code
	CString strBizCode;
	CString strName;
	CString strNumOfLayer;
	CString strShipType;
	CString strWidth;
	CString strHeight;
	CString strNumOfStrip;
	CString strNumOfPiece;
	CString strNumOfStripOut;
	CString strCustomerCode;
}sttModelCodeInfo;
typedef CArray<sttModelCodeInfo, sttModelCodeInfo> CArModelCodeInfo;

typedef struct tagBizArea{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttBizAreaCodeInfo;
typedef CArray<sttBizAreaCodeInfo, sttBizAreaCodeInfo> CArBizAreaCodeInfo;

typedef struct tagShipType{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttShipTypeCodeInfo;
typedef CArray<sttShipTypeCodeInfo, sttShipTypeCodeInfo> CArShipTypeCodeInfo;

typedef struct tagCustomer{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttCustomerCodeInfo;
typedef CArray<sttCustomerCodeInfo, sttCustomerCodeInfo> CArCustomerCodeInfo;

typedef struct tagEquipment{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttEquipCodeInfo;
typedef CArray<sttEquipCodeInfo, sttEquipCodeInfo> CArEquipCodeInfo;

typedef struct tagEquipModel{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttEquipModelCodeInfo;
typedef CArray<sttEquipModelCodeInfo, sttEquipModelCodeInfo> CArEquipModelCodeInfo;

typedef struct tagEquipType{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttEquipTypeCodeInfo;
typedef CArray<sttEquipTypeCodeInfo, sttEquipTypeCodeInfo> CArEquipTypeCodeInfo;

typedef struct tagUser{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttUserCodeInfo;
typedef CArray<sttUserCodeInfo, sttUserCodeInfo> CArUserCodeInfo;

typedef struct tagEquipBreak{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttEquipBreakCodeInfo;
typedef CArray<sttEquipBreakCodeInfo, sttEquipBreakCodeInfo> CArEquipBreakCodeInfo;

typedef struct tagUsing{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttUsingCodeInfo;
typedef CArray<sttUsingCodeInfo, sttUsingCodeInfo> CArUsingCodeInfo;

typedef struct tagSide{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttSideCodeInfo;
typedef CArray<sttSideCodeInfo, sttSideCodeInfo> CArSideCodeInfo;

typedef struct tagRotate{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttRotateCodeInfo;
typedef CArray<sttRotateCodeInfo, sttRotateCodeInfo> CArRotateCodeInfo;

typedef struct tagMirror{
	CString strCode;	// Model Code
	CString strName;	// Model Name for display
}sttMirrorCodeInfo;
typedef CArray<sttMirrorCodeInfo, sttMirrorCodeInfo> CArMirrorCodeInfo;

typedef struct tagEquipInfo{
	CString Equip_Code;	
	CString Biz_Area_Name;
	CString Equip_Name;	
	CString IP_Addr;
	CString MAC_Addr;
	CString Equip_Type_Name;
	CString Maker;
	CString Agent;
	CString Install_Loc;
	CString Purchase_Date;
	CString Purchase_Price;
	CString Manager_Name;
	CString Using_Status;
	CString Equip_Model_Name;
	CString Soft_Version;
	CString Register_Name;
	CString Register_Date;
}sttEquipInfo;
typedef CArray<sttEquipInfo, sttEquipInfo> CArEquip;


//++++++++++++++++++++++++++++++++++++++++
// CLogOn class에서 사용
typedef struct tagUSER_INFO{
	CString	ID;		// ID
	CString PLevel;	// Permission level
	CString PW;		// PERSONAL Password
}USER_INFO;
typedef CArray<USER_INFO, USER_INFO> CArUserList;

class CUserInfo;
class CUserInfo : public tagUSER_INFO  
{
public:
	CUserInfo();
	virtual ~CUserInfo();
	void    operator =(USER_INFO Param); // Nonzero if the points are equal; otherwise 0.
};

//
typedef struct tagDispMessage{
	UINT nMsgId;
	CString strTitleMsg;
	CString strMsg;
	COLORREF color;
	DWORD dwDispTime;
	DWORD dwTimeout;
	BOOL bOverWrite;
}DISP_MESSAGE,*LPDISP_MESSAGE;


class GlobalMathematics 
{
public:
	double GetDistance(CfPoint3D FirstPoint,CfPoint3D SecondPoint); // 두점 사이의 거리를 얻는다 
};

class GPickFunction
{
	public:
	int GetNearestPtIndex(CfPoint3D* CurrfptArray,int PtNum,CfPoint3D fWorldPos);
	int	 GetNearestQuadIndex(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,int Flag);
	BOOL GetNearestQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,CfQuad3D& GetQaud);
	BOOL GetNearestFptInQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos,CfPoint3D &ResultFpt);
	int GetNearestFptIndexInQuad3D(CArfQuad3D& CurrQaud3DArray,CfPoint3D fWorldPos);
	int GetNearestPointIndexArr(CArfPt3D* Arr,CfPoint3D fpt);


	
	
};

/*

BaudRate=9600
I/O Buffer Size=4096
Interval TimeOut=-1
Read TimeOut=0
Write TimeOut=0
Data Bit=8
Stop Bit=1
Parity=NO
CTS monitoring for output flow control=NO
RTS flow control mode=DISABLE
DSR monitoring for output flow control=NO
DSR Sensitivity=FALSE
DTR flow control mode=ENABLE
X On/Off Out Control=NO
X On/Off In Control=NO
End of Line Char=


Target Object=0
ABLE=0
REFLECTION=0
ABLE Upper limit=70
ABLE Lower limit=40
Alarm LEVEL=5
Alarm VALUE=60
Display Panel=1
Trigger mode=0
Measurement Mode=0
Minimum display unit=2
Sampling rate=3
Timing synchronization=0
Comparator output format=0
Strobe time=2
Analog through=0
Mutual interference=0
Offset VALUE=0
ToleranceHigh=5.0
ToleranceLow=-5.0
Hysteresis=0.0
Calculation METHOD=0
Calculation Waveform=0
Filter FUNCTION=0
Filter MODE=0

*/


// The DRECT structure defines the coordinates of the upper-left and 
// lower-right corners of a rectangle.
typedef struct tagdRECT {
	tagdRECT(double pleft = 0.0, double pbottom = 0.0, double pright = 0.0, double ptop = 0.0)
	{
		left = pleft;
		bottom = pbottom;
		right = pright;
		top = ptop;
	}
	double left; // Specifies the x-coordinate of the upper-left corner of a rectangle.
	double bottom; // Specifies the y-coordinate of the upper-left corner of a rectangle.
	double right; // Specifies the x-coordinate of the lower-right corner of a rectangle.
	double top; // Specifies the y-coordinate of the lower-right corner of a rectangle.
}DRECT, *LPCDRECT;

// The DPOINT structure defines the x- and y-coordinates of a point.
typedef struct tagdPOINT {
	tagdPOINT(double px = 0.0, double py = 0.0)
	{
		x = px;
		y = py;
	}
	double x; // Specifies the x-coordinate of a point.
	double y; // Specifies the y-coordinate of a point.
} DPOINT;


#endif // !defined(AFX_MYDATA_H__B8E4F205_0089_4CD3_A7D4_EB5CEF56B11D__INCLUDED_)
