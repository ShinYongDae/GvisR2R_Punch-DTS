#if !defined(AFX_DLGMENU04_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_)
#define AFX_DLGMENU04_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu04.h : header file
//

//#include "MyLabel.h"
#include "MyBtn.h"
#include "MyStatic.h"

#define MAX_MENU04_STC				135
#define MAX_MENU04_STC_DATA			58
#define MAX_MENU04_BTN				12

#define TIM_BUF_ENC_				400

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu04 dialog

class CDlgMenu04 : public CDialog
{
	CMyBtn myBtn[MAX_MENU04_BTN];
	CMyStatic myStcTitle[MAX_MENU04_STC];
	CMyStatic myStcData[MAX_MENU04_STC_DATA];

	BOOL m_bLoadImg;
	BOOL m_bTIM_BUF_ENC;

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();

	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);
	void Disp();
	void DispBufEnc();
	void ResetMotion(int nMsId);

	void SwMyBtnDown(int nCtrlID);
	void SwMyBtnUp(int nCtrlID);


// Construction
public:
	CDlgMenu04(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu04();

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

	void UpdateData();

// Dialog Data
	//{{AFX_DATA(CDlgMenu04)
	enum { IDD = IDD_DLG_MENU_04 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu04)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu04)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnStc004();
	afx_msg void OnStc104();
	afx_msg void OnStc108();
	afx_msg void OnStc007();
	afx_msg void OnStc010();
	afx_msg void OnStc014();
	afx_msg void OnStc22();
	afx_msg void OnStc018();
	afx_msg void OnStc021();
	afx_msg void OnStc024();
	afx_msg void OnStc204();
	afx_msg void OnStc209();
	afx_msg void OnStc25();
	afx_msg void OnStc28();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnBtnBuffHome2();
	afx_msg void OnBtnBuffInitMove2();
	afx_msg void OnBtnBuffInitSave2();
	afx_msg void OnStc48();
	afx_msg void OnStc52();
	afx_msg void OnStc55();
	afx_msg void OnStc58();
	afx_msg void OnStc62();
	afx_msg void OnStc65();
	afx_msg void OnStc68();
	afx_msg void OnStc71();
	afx_msg void OnStc86();
	afx_msg void OnStc75();
	afx_msg void OnStc87();
	afx_msg void OnStc78();
	afx_msg void OnStc88();
	afx_msg void OnStc81();
	afx_msg void OnStc89();
	afx_msg void OnStc84();
	afx_msg void OnStc93();
	afx_msg void OnStc97();
	afx_msg void OnStc110();
	afx_msg void OnStc113();
	afx_msg void OnStc117();
	afx_msg void OnStc120();
	afx_msg void OnStc123();
	afx_msg void OnStc126();
	afx_msg void OnStc141();
	afx_msg void OnStc130();
	afx_msg void OnStc142();
	afx_msg void OnStc133();
	afx_msg void OnStc143();
	afx_msg void OnStc136();
	afx_msg void OnStc144();
	afx_msg void OnStc139();
	afx_msg void OnStc148();
	afx_msg void OnStc152();
	afx_msg void OnStc158();
	afx_msg void OnStc162();
	afx_msg void OnStc165();
	afx_msg void OnStc170();
	afx_msg void OnStc212();
	afx_msg void OnStc221();
	afx_msg void OnStc222();
	afx_msg void OnBtnResetMkL();
	afx_msg void OnBtnResetMkR();
	afx_msg void OnStc69();
	afx_msg void OnStc124();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU04_H__75EE004F_9202_4E98_8378_A75CC1777324__INCLUDED_)
