#if !defined(AFX_DLGMYPASSWORD_H__ACB6CB36_6D12_49C5_B9B1_809BD567CE14__INCLUDED_)
#define AFX_DLGMYPASSWORD_H__ACB6CB36_6D12_49C5_B9B1_809BD567CE14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMyPassword.h : header file
//
#include "MyPic.h"
#include "MyBtn.h"
#include "MyGroup.h"
#include "MyLabel.h"
#include "MyEdit.h"

#define TIM_KILL_FOCUS				221
#define TIM_WATCHDOG_MYPASSWORD		222

#ifndef FONT_SIZE_PW
	#define FONT_SIZE_PW	62
#endif
#ifndef LINE_SPACE_PW
	#define LINE_SPACE_PW	20
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMyPassword dialog

class CDlgMyPassword : public CDialog
{
	BOOL m_bFont;
	CFont m_Font;
	BOOL m_bLoadImg;
	CRect* m_pRect;
	CMyPic myPic;
	int m_nSel;
	CMyLabel myLabel;
	CMyEdit myEdit[2];
	CMyBtn myBtn;
	CMyGroup myGroup;
	CString m_strMsg;
	int m_nCtrlId;
	BOOL m_bTIM_DLG_MY_PASSWORD, m_bTIM_DLG_MY_PASSWORD_Lock;

	void AtDlgShow();
	void AtDlgHide();
	void InitPic();
	void LoadImg();
	void DelImg();
	void InitLabel();
	void InitBtn();
	void InitGroup();
	void InitEdit();
	void KillFocus(int nID);
	void SetFont(CString srtFntName, int nSize=10, BOOL bBold=FALSE);


// Construction
public:
	CDlgMyPassword(CWnd* pParent = NULL);   // standard constructor
	~CDlgMyPassword();

	CWnd* m_pParent;
	int m_nRtnVal;

	BOOL Create();
	void HideSubDlg(int nDlgID);
	void SetMsg(CString strMsg, int nCtrlId=0);
	afx_msg LRESULT  OnMyPw(WPARAM wPara, LPARAM lPara);

// Dialog Data
	//{{AFX_DATA(CDlgMyPassword)
	enum { IDD = IDD_DLG_MY_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMyPassword)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMyPassword)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnButtonSym00();
	afx_msg void OnButtonSym01();
	afx_msg void OnButtonSym02();
	afx_msg void OnButtonSym03();
	afx_msg void OnButtonSym04();
	afx_msg void OnButtonSym05();
	afx_msg void OnButtonSym06();
	afx_msg void OnButtonSym07();
	afx_msg void OnButtonSym08();
	afx_msg void OnButtonSym09();
	afx_msg void OnButtonSym10();
	afx_msg void OnButtonSym11();
	afx_msg void OnButtonNum1();
	afx_msg void OnButtonNum2();
	afx_msg void OnButtonNum3();
	afx_msg void OnButtonNum4();
	afx_msg void OnButtonNum5();
	afx_msg void OnButtonNum6();
	afx_msg void OnButtonNum7();
	afx_msg void OnButtonNum8();
	afx_msg void OnButtonNum9();
	afx_msg void OnButtonNum0();
	afx_msg void OnButtonMinus();
	afx_msg void OnButtonEqual();
	afx_msg void OnButtonCharQ();
	afx_msg void OnButtonCharW();
	afx_msg void OnButtonCharE();
	afx_msg void OnButtonCharR();
	afx_msg void OnButtonCharT();
	afx_msg void OnButtonCharY();
	afx_msg void OnButtonCharU();
	afx_msg void OnButtonCharI();
	afx_msg void OnButtonCharO();
	afx_msg void OnButtonCharP();
	afx_msg void OnButtonEsc();
	afx_msg void OnButtonCharA();
	afx_msg void OnButtonCharS();
	afx_msg void OnButtonCharD();
	afx_msg void OnButtonCharF();
	afx_msg void OnButtonCharG();
	afx_msg void OnButtonCharH();
	afx_msg void OnButtonCharJ();
	afx_msg void OnButtonCharK();
	afx_msg void OnButtonCharL();
	afx_msg void OnButtonCharZ();
	afx_msg void OnButtonCharX();
	afx_msg void OnButtonCharC();
	afx_msg void OnButtonCharV();
	afx_msg void OnButtonCharB();
	afx_msg void OnButtonCharN();
	afx_msg void OnButtonCharM();
	afx_msg void OnButtonComma();
	afx_msg void OnButtonDot();
	afx_msg void OnButtonEnter();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonSpace();
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
	afx_msg void OnButtonBs();
	afx_msg void OnButtonDel();
	afx_msg void OnSetfocusEdit11();
	afx_msg void OnSetfocusEdit21();
	afx_msg void OnBtn00();
	afx_msg void OnChangeEdit11();
	afx_msg void OnChangeEdit21();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnCheckShift();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYPASSWORD_H__ACB6CB36_6D12_49C5_B9B1_809BD567CE14__INCLUDED_)
