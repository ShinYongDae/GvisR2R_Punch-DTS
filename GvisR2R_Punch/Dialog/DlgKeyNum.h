#if !defined(AFX_DlgKeyNum_H__BE21C98F_136C_11D7_B135_000102406579__INCLUDED_)
#define AFX_DlgKeyNum_H__BE21C98F_136C_11D7_B135_000102406579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKeyNum.h : header file
//

#include "MyBtn.h"
#include "MyPic.h"
#include "MyEdit.h"
#include "MyStatic.h"
#include "MyLabel.h"
#include "../Global/GlobalDefine.h"

#ifndef FONT_SIZE_KEYNUM
	#define FONT_SIZE_KEYNUM	32
#endif
#ifndef LINE_SPACE_KEYNUM
	#define LINE_SPACE_KEYNUM	5
#endif

#define TIM_SHOW_KEYPADNUM	0
#define TIM_KILL_FOCUS_	1

#define MAX_STATIC_KEYPAD	4

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyNum dialog

class CDlgKeyNum : public CDialog
{
	CWnd* m_pParent;
	CPoint m_ptSt;
	int m_nDir;
	
	HBITMAP m_hBmpNum0, m_hBmpNum1, m_hBmpNum2, m_hBmpNum3, m_hBmpNum4, m_hBmpNum5, m_hBmpNum6,
			m_hBmpNum7, m_hBmpNum8, m_hBmpNum9, m_hBmpBs, m_hBmpDel, m_hBmpDot, m_hBmpLeft,
			m_hBmpMinus, m_hBmpRight, m_hBmpEsc, m_hBmpEnter;
	CBitmap *m_pBmpNum0, *m_pBmpNum1, *m_pBmpNum2, *m_pBmpNum3, *m_pBmpNum4, *m_pBmpNum5, *m_pBmpNum6,
			*m_pBmpNum7, *m_pBmpNum8, *m_pBmpNum9, *m_pBmpBs, *m_pBmpDel, *m_pBmpDot, *m_pBmpLeft,
			*m_pBmpMinus, *m_pBmpRight, *m_pBmpEsc, *m_pBmpEnter;
	CRect* m_Rect;
	CMyPic myPic;
	CPoint m_stPoint;
	BOOL m_dnMouse, m_bInit;
	CMyEdit myEdit;
	CMyLabel myLabel;
	CMyStatic myStatic[MAX_STATIC_KEYPAD];
	BOOL m_bTIM_DLG_KEYBDNUM, m_bTIM_DLG_KEYBDNUM_Lock;


	void InitBtn();
	void InitPic();
	void InitEdit();
	void InitLabel();
	void InitStatic();
	void KillFocus(int nID);
	void LoadImg();
	void DelImg();
	void DispVal();

// Construction
public:
	int strPosition;
	CString* m_pstrEdit, m_strM, m_strCurr, m_strMin, m_strMax;
	CDlgKeyNum(CString *strFileName, CRect* rect=NULL, CPoint ptSt=(0, 0), int nDir=TO_NONE, CWnd* pParent=NULL);  // standard constructor
	~CDlgKeyNum();

	BOOL SetCtrlColor(const UINT nID, CString strText, COLORREF bgColor, COLORREF fgColor);
	void SetPasswordStyle();
	afx_msg LRESULT OnMyKeyPadNum(WPARAM wPara, LPARAM lPara);

// Dialog Data
	//{{AFX_DATA(CDlgKeyNum)
	enum { IDD = IDD_DLG_KEY_NUM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKeyNum)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgKeyNum)
	afx_msg void OnButtonBs();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonDot();
	afx_msg void OnButtonEnter();
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonMinus();
	afx_msg void OnButtonNum0();
	afx_msg void OnButtonNum1();
	afx_msg void OnButtonNum2();
	afx_msg void OnButtonNum3();
	afx_msg void OnButtonNum4();
	afx_msg void OnButtonNum5();
	afx_msg void OnButtonNum6();
	afx_msg void OnButtonNum7();
	afx_msg void OnButtonNum8();
	afx_msg void OnButtonNum9();
	afx_msg void OnButtonRight();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonEsc();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgKeyNum_H__BE21C98F_136C_11D7_B135_000102406579__INCLUDED_)
