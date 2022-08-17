#if !defined(AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_)
#define AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu03.h : header file
//

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyGroup.h"

#define MAX_MENU03_STC				93
#define MAX_MENU03_BTN				87
#define MAX_MENU03_GRP				10

#define TIM_MENU03_DISP				300
#define TIM_CHK_DONE_MK				301
#define TIM_CHK_DONE_AOI			302
#define TIM_CHK_DONE_MKAOI			303
#define TIM_CHK_MREG				304
#define TIM_SET_RC_RELATION			306
#define TIM_SW_RC_RELATION			307
#define TIM_SW_UC_RELATION			308
#define TIM_AOI_RESET_OFF			309
#define TIM_AOI_LOTEND_OFF			310
#define TIM_CHK_DONE_READY			311
#define TIM_CHK_DONE_BUF_HOME		312
#define TIM_CHK_DONE_BUF_INIT		313
// #define TIM_CHK_SOL_MK				312
// #define TIM_CHK_SOL_AOI				313

#define TIM_CHK_DONE_ENGRAVE		314

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 dialog

class CDlgMenu03 : public CDialog
{
	CMyBtn myBtn[MAX_MENU03_BTN];
	CMyLabel myStcTitle[MAX_MENU03_STC];
	CMyGroup myGrp[MAX_MENU03_GRP];

	BOOL m_bLoadImg;
	BOOL m_bTIM_MENU03_DISP;
	BOOL m_bTIM_CHK_DONE_MK;
	BOOL m_bTIM_CHK_DONE_AOI;
// 	BOOL m_bTIM_CHK_SOL_MK;
// 	BOOL m_bTIM_CHK_SOL_AOI;
	BOOL m_bTIM_CHK_DONE_MKAOI;
	BOOL m_bTIM_CHK_DONE_BUF_HOME;
	BOOL m_bTIM_CHK_DONE_BUF_INIT;
	BOOL m_bTIM_CHK_DONE_READY;
	BOOL m_bTIM_SET_RC_RELATION;
	BOOL m_bTIM_SW_RC_RELATION;
	BOOL m_bTIM_SW_UC_RELATION;

	double m_dPrevEnc[MAX_AXIS];
	int m_nDir[MAX_AXIS];
	BOOL m_bBufInitPosMove;

	BOOL m_bTIM_CHK_DONE_ENGRAVE;


	void AtDlgShow();
	void AtDlgHide();

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitBtn();
	void InitGroup();

	void Disp();
	void DispMain();
	void DispRecoiler();
	void DispPunch();
	void DispAoiDn();
	void DispAoiUp();
	void DispEngrave();
	void DispUncoiler();

// 	void MyBtnDown(int nCtrlID);
// 	void MyBtnUp(int nCtrlID);
	void ChkDoneMkAoi();
	void DoReady();
	BOOL DoReset();
	void SwRcRelationTim();
	void SwUcRelationTim();

	
// Construction
public:
	CDlgMenu03(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu03();

	CRect* m_pRect;
	int m_nFdSpd;
	BOOL m_bVMoveMkFdVac, m_bVMoveAoiFdVac;

	BOOL Create();

	BOOL GetRun();	
	BOOL GetReset();
	BOOL GetReady();
	BOOL GetRelation();
	BOOL GetCw();
	void SetCw(BOOL bOn);
	BOOL GetCcw();
	void SetCcw(BOOL bOn);
	void SetMkOnePnl(BOOL bOn);
	BOOL GetMkOnePnl();				// not used
	void SetAoiOnePnl(BOOL bOn);
	BOOL GetAoiOnePnl();			// not used
	void TimMoveMk(int nDir=M_CW) ;	// not used
	void MoveMk(int nDir=M_CW);
	void MoveMk(double dOffset);
	void TimMoveAoi(int nDir=M_CW);	// not used
	void MoveAoi(int nDir=M_CW);
	void MoveAoi(double dOffset);
	void StopMk();					// not used
	void StopAoi();					// not used
	BOOL GetMkTq();
	BOOL GetAoiTq();
	void SetMkTq(BOOL bOn);
	void SetAoiTq(BOOL bOn);
	void ChkDoneMk();
	void ChkDoneAoi();
	void ChkBufHomeDone();
	void ChkBufInitDone();
	void ChkReadyDone();
	//void DispCwCcwLamp(BOOL bRun);
	void DoManual();
	void DoAuto();
	void InitRelation();

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

 	void SwMyBtnDown(int nCtrlID);
 	void SwMyBtnUp(int nCtrlID);
	void SwEngraveBtn(int nCtrlID, BOOL bOn);
	void SwMpeBtn(int nCtrlID, long lData);

	BOOL IsMkFdVac();		// not used
	BOOL IsMkTqVac();		// not used
	BOOL IsMkTblVac();		// not used
	BOOL IsMkTblBlw();		// not used

	BOOL IsAoiTest();
	BOOL IsAoiTblVac();
	BOOL IsAoiTestDone();
	BOOL IsAoiTblVacDone();
	BOOL IsStop();
	BOOL IsAoiReset();
	BOOL IsAoiLdRun();
	BOOL IsAoiFdVac();
	BOOL IsAoiDustBlw();
	BOOL IsBufRolSol();

	// Main
	void SwReset(BOOL bOn);
	void SwReady(BOOL bOn);
	void SwRun(BOOL bOn);
	void SwStop(BOOL bOn);

	// Torque Motor
	void SwMkTq(BOOL bOn);
	void SwAoiTq(BOOL bOn);
	void SwEngraveTq(BOOL bOn);

	// Punching
	void SwMkRelation(BOOL bOn);		// not used
	void SwMkTblBlw(BOOL bOn);			// not used
	void SwMkFdVac(BOOL bOn);			// not used
	void SwMkTqVac(BOOL bOn);			// not used
	void SwMkTblVac(BOOL bOn);			// not used
	void SwMkLsrPt(BOOL bOn);			// not used
	void SwMkFdClp(BOOL bOn);			// not used
// 	void SwMkDnSol(BOOL bOn);			// not used

	void SwBufRolSol(BOOL bOn);

	// AOI
	void SwAoiRelation(BOOL bOn);
	void SwAoiDustBlw(BOOL bOn);
	void SwAoiTblBlw(BOOL bOn);
	void SwAoiFdVac(BOOL bOn);
	void SwAoiTqVac(BOOL bOn);
	void SwAoiTest(BOOL bOn);
	void SwAoiReset(BOOL bOn);
	void SwAoiLotEnd(BOOL bOn);
	void SwAoiTblVac(BOOL bOn);
	void SwAoiLsrPt(BOOL bOn);
	void SwAoiFdClp(BOOL bOn);
	void SwAoiTqClp(BOOL bOn);

	// Uncoiler
	void SwUcRelation(BOOL bOn);
	void SwUcDcRlUpDn(BOOL bOn);
	void SwUcClRlUpDn(BOOL bOn);
	void SwUcClRlPshUpDn(BOOL bOn);
	void SwUcReelJoinL(BOOL bOn);
	void SwUcReelJoinR(BOOL bOn);
	void SwUcReelWheel(BOOL bOn);
	void SwUcPprChuck(BOOL bOn);
	void SwUcReelChuck(BOOL bOn);

	// Recoiler
	void SwRcRelation(BOOL bOn);
	void SwRcDcRlUpDn(BOOL bOn);
	void SwRcReelJoinL(BOOL bOn);
	void SwRcReelJoinR(BOOL bOn);
	void SwRcReelWheel(BOOL bOn);
	void SwRcPprChuck(BOOL bOn);
	void SwRcReelChuck(BOOL bOn);

	void SwAoiEmg(BOOL bOn);

	void SwReset();
	void SwReady();
	void SwRun();
	void SwStop();
	void SwMkTq();			// not used
	void SwAoiTq();
	void SwMkRelation();	// not used
	void SwMkTblBlw();		// not used
	void SwMkFdVac();		// not used
	void SwMkTqVac();		// not used
	void SwMkTblVac();		// not used
	void SwMkLsrPt();		// not used
	void SwMkFdClp();
// 	void SwMkDnSol();
	void SwBufRolSol();
	void SwAoiRelation();
	void SwAoiDustBlw();
	void SwAoiTblBlw();
	void SwAoiFdVac();
	void SwAoiTqVac();
	void SwAoiTblVac();
	void SwAoiLsrPt();
	void SwAoiFdClp();
	void SwAoiTqClp();
	void SwUcRelation();
	void SwUcDcRlUpDn();
	void SwUcClRlUpDn();
	void SwUcClRlPshUpDn();
	void SwUcReelJoinL();
	void SwUcReelJoinR();
	void SwUcReelWheel();
	void SwUcPprChuck();
	void SwUcReelChuck();
	void SwRcRelation();
	void SwRcDcRlUpDn();
	void SwRcReelJoinL();
	void SwRcReelJoinR();
	void SwRcReelWheel();
	void SwRcPprChuck();
	void SwRcReelChuck();	

	void SetCore150mmRecoiler(BOOL bOn);
	void SetCore150mmUncoiler(BOOL bOn);

	void SetDualTest(BOOL bOn=TRUE);


	BOOL GetEngraveTq();
	void SetEngraveTq(BOOL bOn);
	void SetEngraveOnePnl(BOOL bOn);
	void MoveEngrave(int nDir = M_CW);
	void MoveEngrave(double dOffset);
	void ChkDoneEngrave();

	BOOL IsEngraveFdVac();
	BOOL IsEngraveTqVac();
	BOOL IsEngraveTblVac();
	BOOL IsEngraveTblBlw();

	void SetEngraveCleanner(BOOL bOn);
	void SetAoiDnCleanner(BOOL bOn);

	void UpdateData();
	void UpdateSignal();

// Dialog Data
	//{{AFX_DATA(CDlgMenu03)
	enum { IDD = IDD_DLG_MENU_03 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu03)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu03)
	afx_msg void OnChk24();
	afx_msg void OnChk62();
	afx_msg void OnChk16();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedChk79();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_)
