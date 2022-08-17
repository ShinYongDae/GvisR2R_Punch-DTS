#if !defined(AFX_LIGHT_H__591F22C4_F67B_4CD9_A6D7_3797BE638A27__INCLUDED_)
#define AFX_LIGHT_H__591F22C4_F67B_4CD9_A6D7_3797BE638A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Light.h : header file
//

#include "Rs232.h"



/////////////////////////////////////////////////////////////////////////////
// CLight window

class CLight : public CWnd
{
	CWnd*			m_pParent;
	CCriticalSection m_cs;
	BOOL m_bCh[MAX_LIGHT_CH];
	int m_nCh[MAX_LIGHT_CH];
	CString m_sRcvRs232;
	BOOL Send(CString str);

// Construction
public:
	CLight(CWnd* pParent =NULL);

// Attributes
public:
	CRs232 m_Rs232;
	BOOL m_bRs232Run;

// Operations
public:
	afx_msg LRESULT OnReceiveRs232(WPARAM wP, LPARAM lP);
	CString Rcv();

	void Init();
	BOOL Set(int nCh, int nVal=-1);
	int Get(int nCh);
	void Reset(int nCh);
	void Close();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLight)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLight();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLight)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHT_H__591F22C4_F67B_4CD9_A6D7_3797BE638A27__INCLUDED_)
