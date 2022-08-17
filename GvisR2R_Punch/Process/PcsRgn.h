#if !defined(AFX_PCSRGN_H__B705C306_4FC5_4B7E_8B64_0B5465DE0F2F__INCLUDED_)
#define AFX_PCSRGN_H__B705C306_4FC5_4B7E_8B64_0B5465DE0F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PcsRgn.h : header file
//
#include "../Global/MyData.h"

/////////////////////////////////////////////////////////////////////////////
// CPcsRgn window

class CPcsRgn : public CWnd
{
	CfPoint m_ptPinPos[2];
	CfPoint m_ptOffset;

// Construction
public:
	CPcsRgn(int nPcs=0);

// Attributes
public:
	int nTotPcs;
	int nCol, nRow;
	CRect rtFrm;
	CRect *pPcs;
	CfPoint *pCenter, *pMkPnt[2]; // [Cam]

// Operations
public:
// 	void SetMkPnt();
	void SetMkPnt(int nCam);	// Pin위치에 의한 정렬.
	void GetMkPnt(int nC, int nR, int &nPcsId, CfPoint &ptPnt);	// nC, nR : Cam(or Reelmap) 기준
	CfPoint GetMkPnt(int nPcsId);
	CfPoint GetMkPnt0(int nPcsId);
	CfPoint GetMkPnt1(int nPcsId);
	void SetPinPos(int nCam, CfPoint ptPnt);
	BOOL GetMkMatrix(int nPcsId, int &nC, int &nR);
	BOOL GetMkMatrix(int nPcsId, int &nStrip, int &nC, int &nR);

	void GetPcsRgn(int nC, int nR, int &nPcsId, CRect &ptRect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcsRgn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPcsRgn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPcsRgn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSRGN_H__B705C306_4FC5_4B7E_8B64_0B5465DE0F2F__INCLUDED_)
