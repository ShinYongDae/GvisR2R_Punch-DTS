#if !defined(AFX_MYSPEC_H__D56231C1_18D4_4151_8722_DE707E9F0D9B__INCLUDED_)
#define AFX_MYSPEC_H__D56231C1_18D4_4151_8722_DE707E9F0D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySpec.h : header file
//

#include "MyData.h"

/////////////////////////////////////////////////////////////////////////////
// CMySpec window

class CMySpec : public CWnd
{
	CString m_sModel, m_sLayer, m_sPath;

// Construction
public:
	CMySpec();
	CMySpec(CString sModel, CString sLayer);

// Attributes
public:
// 	double m_dPcsOffsetX, m_dPcsOffsetY;
	double m_dPinPosX[2], m_dPinPosY[2];

// Operations
public:
	BOOL Load();
	BOOL MakeDir();
	BOOL MakeDir(CString sModel, CString sLayer);
	BOOL Save();
// 	BOOL SavePcsOffset();
// 	BOOL SavePcsOffset(CfPoint fPt);
	BOOL SavePinPos();
	BOOL SavePinPos(int nCam, CfPoint fPt);
// 	BOOL Shift2Mk(int nSerial);
	BOOL IsPinMkData();
	BOOL IsPinData();
// 	BOOL IsMkOffsetData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpec)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySpec();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySpec)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPEC_H__D56231C1_18D4_4151_8722_DE707E9F0D9B__INCLUDED_)
