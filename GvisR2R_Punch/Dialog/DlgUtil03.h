#if !defined(AFX_DLGUTIL03_H__D03BBD81_0416_4513_A64F_9F8D79A93C97__INCLUDED_)
#define AFX_DLGUTIL03_H__D03BBD81_0416_4513_A64F_9F8D79A93C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUtil03.h : header file
//

#include "MyStatic.h"
#include "MyBtn.h"

#define MAX_UTIL03_STC_DATA	(66+1)
#define MAX_SPREAD_COL		6	
#define MAX_SPREAD_ROW		11	
#define MAX_UTIL03_BTN		7

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil03 dialog

class CDlgUtil03 : public CDialog
{
	BOOL m_bLoadImg;
	CRect* m_pRect;
	CMyStatic myStcData[MAX_UTIL03_STC_DATA];
	CMyBtn myBtn[MAX_UTIL03_BTN];
	int nScrlV, nScrlH;

	void AtDlgShow();
	void AtDlgHide();
	void LoadImg();
	void DelImg();
	void InitStc();
	void InitBtn();
	void MoveMkPos(int nStcId);
	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);

// Construction
public:
	CDlgUtil03(CWnd* pParent = NULL);   // standard constructor
	~CDlgUtil03();

	BOOL Create();
	void Disp(int nDir);
	void SetScrlBarMax(int nMaxH, int nMaxV);
	void SetScrlBar(int nCurH, int nCurV);
// 	void DoDispenseTest();
// 	void SetFurge();

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

// Dialog Data
	//{{AFX_DATA(CDlgUtil03)
	enum { IDD = IDD_DLG_UTIL_03 };
	CScrollBar	m_scrollV;
	CScrollBar	m_scrollH;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUtil03)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUtil03)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnMk();
	afx_msg void OnStc00();
	afx_msg void OnStc01();
	afx_msg void OnStc02();
	afx_msg void OnStc03();
	afx_msg void OnStc04();
	afx_msg void OnStc05();
	afx_msg void OnStc06();
	afx_msg void OnStc07();
	afx_msg void OnStc08();
	afx_msg void OnStc09();
	afx_msg void OnStc10();
	afx_msg void OnStc11();
	afx_msg void OnStc12();
	afx_msg void OnStc13();
	afx_msg void OnStc14();
	afx_msg void OnStc15();
	afx_msg void OnStc16();
	afx_msg void OnStc17();
	afx_msg void OnStc18();
	afx_msg void OnStc19();
	afx_msg void OnStc20();
	afx_msg void OnStc21();
	afx_msg void OnStc22();
	afx_msg void OnStc23();
	afx_msg void OnStc24();
	afx_msg void OnStc25();
	afx_msg void OnStc26();
	afx_msg void OnStc27();
	afx_msg void OnStc28();
	afx_msg void OnStc29();
	afx_msg void OnStc30();
	afx_msg void OnStc31();
	afx_msg void OnStc32();
	afx_msg void OnStc33();
	afx_msg void OnStc34();
	afx_msg void OnStc35();
	afx_msg void OnStc36();
	afx_msg void OnStc37();
	afx_msg void OnStc38();
	afx_msg void OnStc39();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnAllMkTest();
	afx_msg void OnChkNoMk();
	afx_msg void OnStc010();
	afx_msg void OnStc110();
	afx_msg void OnStc210();
	afx_msg void OnStc40();
	afx_msg void OnStc41();
	afx_msg void OnStc42();
	afx_msg void OnStc43();
	afx_msg void OnStc44();
	afx_msg void OnStc45();
	afx_msg void OnStc46();
	afx_msg void OnStc47();
	afx_msg void OnStc48();
	afx_msg void OnStc49();
	afx_msg void OnStc410();
	afx_msg void OnStc50();
	afx_msg void OnStc51();
	afx_msg void OnStc52();
	afx_msg void OnStc53();
	afx_msg void OnStc54();
	afx_msg void OnStc55();
	afx_msg void OnStc56();
	afx_msg void OnStc57();
	afx_msg void OnStc58();
	afx_msg void OnStc59();
	afx_msg void OnStc510();
	afx_msg void OnStc310();
	afx_msg void OnBtnMkMoveInit();
	afx_msg void OnBtnMkHome();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEditYshiftRatio();
	afx_msg void OnCheckYshift();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckPcs();
	afx_msg void OnBnClickedCheckPcsDn();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUTIL03_H__D03BBD81_0416_4513_A64F_9F8D79A93C97__INCLUDED_)
