// PtAlign.h: interface for the CPtAlign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTALIGN_H__9563AB71_69E9_44A1_99D4_334AD141EB11__INCLUDED_)
#define AFX_PTALIGN_H__9563AB71_69E9_44A1_99D4_334AD141EB11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Global/GlobalDefine.h"
#include "../Global/GlobalFunc.h"
#include "Matrix.h"
#include "../Global/MyData.h"

class CPtAlign  
{
public:
	CPtAlign();
	virtual ~CPtAlign();

	double m_fRefX[4],m_fRefY[4],m_fTgtX[4],m_fTgtY[4];

	CfPoint	m_fptRefBLPt;	// Bottom line intersect point on Reference quad
	CfPoint	m_fptRefTLPt;	// top line intersect point on Reference quad
	CfPoint	m_fptRefLLPt;	// Left line intersect point on Reference quad
	CfPoint	m_fptRefRLPt;	// Right side line intersect point on Reference quad

	CfPoint	m_fptTgtBLPt;	// Bottom line intersect point on Target quad
	CfPoint	m_fptTgtTLPt;	// top line intersect point on Target quad
	CfPoint	m_fptTgtLLPt;	// Left line intersect point on Target quad
	CfPoint	m_fptTgtRLPt;	// Right side line intersect point on Target quad

	CMatrix m_Matrix;
	matrix *m_pMatrix;

	BOOL PointAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0);
	BOOL PointAlignment(CfPoint fptRef,CfPoint &fptTgt);
	BOOL LinearAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0); 
	BOOL LinearAlignment(CfPoint fptRef,CfPoint &fptTgt); 
	BOOL AffineAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0);
	BOOL AffineAlignment(CfPoint fptRef,CfPoint &fptTgt);
	BOOL PerspectiveAlignment(double Rx0,double Ry0,double &Tx0,double &Ty0) ;
	BOOL PerspectiveAlignment(CfPoint fptRef,CfPoint fptTgt);
	BOOL BilinearAlignment(double Rx1,double Ry1,double Tx1,double Ty1,
							double Rx2,double Ry2,double Tx2,double Ty2,
							double Rx3,double Ry3,double Tx3,double Ty3,
							double Rx4,double Ry4,double Tx4,double Ty4,
							double Rx,double Ry,double *Tx,double *Ty);
	BOOL BilinearAlignment(CfPoint fptRefLB,CfPoint fptRefRB,CfPoint fptRefRT,CfPoint fptRefLT,
							CfPoint fptTgtLB,CfPoint fptTgtRB,CfPoint fptTgtRT,CfPoint fptTgtLT,
							CfPoint fptRef,  CfPoint &fptTgt);
	BOOL BilinearAlignment(CfQuad fQuadRef,CfQuad fQuadTgt,CfPoint fptRef,  CfPoint &fptTgt);
	BOOL BilinearAlignment(CfQuad3D fQuadRef,CfQuad3D fQuadTgt,CfPoint3D fptRef,  CfPoint3D &fptTgt);


	void SetAlignData(double Rx1,double Ry1,double Tx1,double Ty1);
	void SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Tx1,double Ty1, double Tx2,double Ty2);
	void SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Rx3,double Ry3,
					double Tx1,double Ty1,double Tx2,double Ty2,double Tx3,double Ty3);
	void SetAlignData(double Rx1,double Ry1,double Rx2,double Ry2,double Rx3,double Ry3,double Rx4,double Ry4,
					double Tx1,double Ty1,double Tx2,double Ty2,double Tx3,double Ty3,double Tx4,double Ty4);

	// for debug
	BOOL GetBilinearPointOnRef(CfPoint &fpt1,CfPoint &fpt2,CfPoint &fpt3,CfPoint &fpt4);
	BOOL GetBilinearPointOnTgt(CfPoint &fpt1,CfPoint &fpt2,CfPoint &fpt3,CfPoint &fpt4);

};

#endif // !defined(AFX_PTALIGN_H__9563AB71_69E9_44A1_99D4_334AD141EB11__INCLUDED_)
