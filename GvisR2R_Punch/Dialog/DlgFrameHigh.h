#if !defined(AFX_DLGFRAMEHIGH_H__577CFAC6_F3B9_4463_B480_30BDB4C68E92__INCLUDED_)
#define AFX_DLGFRAMEHIGH_H__577CFAC6_F3B9_4463_B480_30BDB4C68E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFrameHigh.h : header file
//

#include "MyBtn.h"
#include "MyPic.h"
#include "MyLabel.h"
#include "MyStatic.h"

#define MAX_FRMHIGH_LABEL	5
#define MAX_FRMHIGH_STC		11

#define TIM_SIG_AOI			50

/////////////////////////////////////////////////////////////////////////////
// CDlgFrameHigh dialog

class CDlgFrameHigh : public CDialog
{
	CRect* m_pRect;
	CMyPic myPic;
	CMyLabel myLabel[MAX_FRMHIGH_LABEL];
	CMyBtn myChkMenu01, myChkMenu02, myChkMenu03, 
			myChkMenu04, myChkMenu05;
	CMyStatic myStc[MAX_FRMHIGH_STC];

	BOOL m_bTIM_SIG_AOI;
	CString m_sStc[3];

	void AtDlgShow();
	void AtDlgHide();
	void LoadImg();
	void DelImg();
	void InitStatic();
	void InitPic();
	void InitLabel();
	void InitBtn();
	void DispSigAoi();
	void DispFdOffset();
	void SetChk(int nID);
	void ChkFdEnc();

// Construction
public:
	CDlgFrameHigh(CWnd* pParent = NULL);   // standard constructor
	~CDlgFrameHigh();

	int m_nMkLastShot, m_nAoiLastShot[2]; // [Up/Dn]

	BOOL Create();
	void ChkMenu01();
	void ChkMenu02();
	void ChkMenu03();
	void ChkMenu04();
	void ChkMenu05();

	void SetMkLastShot(int nSerial);
	void SetAoiLastShot(int nAoi, int nSerial);

	void SetDualTest(BOOL bOn=TRUE);

	void SetEngraveLastShot(int nSerial);

// Dialog Data
	//{{AFX_DATA(CDlgFrameHigh)
	enum { IDD = IDD_DLG_FRAME_HIGH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFrameHigh)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFrameHigh)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChkMenu01();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChkMenu02();
	afx_msg void OnChkMenu03();
	afx_msg void OnChkMenu04();
	afx_msg void OnChkMenu05();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFRAMEHIGH_H__577CFAC6_F3B9_4463_B480_30BDB4C68E92__INCLUDED_)
