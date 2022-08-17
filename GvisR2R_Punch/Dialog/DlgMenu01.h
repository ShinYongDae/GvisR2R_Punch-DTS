#if !defined(AFX_DLGMENU01_H__FD6374B9_4CE8_4830_8693_759B27C002B1__INCLUDED_)
#define AFX_DLGMENU01_H__FD6374B9_4CE8_4830_8693_759B27C002B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu01.h : header file
//

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"

#define MAX_MENU01_STC_DEFINFO		12

#define MAX_MENU01_STC_TITLE		67
#define MAX_MENU01_STC_DATA			83
#define MAX_MENU01_BTN				23

#define TIM_DISP_DEF_IMG			100
#define TIM_DISP_MK_INFO			101
#define TIM_DISP_MK_CNT				102
#define TIM_WINK_MK_L				103
#define TIM_WINK_MK_R				104

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu01 dialog

class CDlgMenu01 : public CDialog
{
	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU01_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU01_STC_TITLE];
	CMyStatic myStcData[MAX_MENU01_STC_DATA];
	CMyBtn myBtn[MAX_MENU01_BTN];

	CRect m_RectTitle[2];
	CRect m_RectCAD[12];
	CRect m_RectDef[12];
	CRect m_RectInfo[12];

	BOOL m_bLoadImg;
	CMyGL* m_pMyGL;
	int m_nIdxMkInfo[2];		// [0]:AOI-Up , [1]:AOI-Dn
	int m_nSerial, m_nDef[2];	// [0]:AOI-Up , [1]:AOI-Dn
	BOOL m_bTIM_DISP_DEF_IMG;
	BOOL m_bTIM_DISP_MK_CNT;
	int m_nIdxDef[2];			// [0]:AOI-Up , [1]:AOI-Dn

	int m_nWinkMk[2]; // [nCam]
	BOOL m_bWinkMk[2]; // [nCam]
	BOOL m_bTIM_WINK_MK_L, m_bTIM_WINK_MK_R;
	BOOL m_bLowPartialSpd;
	int m_nSerialDispMkInfo;

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitStcDefInf();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();
	void InitMkInfo();
	void InitMkInfoUp();
	void InitMkInfoDn();
	void SelDisp();

	void ShowDefInfoUp(int nIdx); // nIdx : 0 ~ 11 (12ea)
	void ShowDefInfoDn(int nIdx); // nIdx : 0 ~ 11 (12ea)
	void DispMkInfo();
	void DispMkInfoUp();
	void DispMkInfoDn();
	void DispMkInfo(int nSerial);
	void DispMkInfoUp(int nSerial);
	void DispMkInfoDn(int nSerial);

	void ShiftDefInfoUp();
	void ShiftDefInfoDn();
	void InitDefImg();
	void InitDefImgUp();
	void InitDefImgDn();
	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);

	BOOL IsWinkMkL();
	void WinkMkL(BOOL bOn);
	BOOL IsWinkMkR();
	void WinkMkR(BOOL bOn);

// Construction
public:
	CDlgMenu01(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu01();

	CRect* m_pRect;
	BOOL m_bLastProc, m_bLastProcFromUp;
	BOOL m_bLotEnd, m_bEnableMkStrip;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

	void ChkUserInfo(BOOL bOn);
	afx_msg LRESULT OnDrawReelMap(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyStaticRedraw(WPARAM wPara, LPARAM lPara);

	void SetRgbStcDef();
	void SetTitleStcDef();

	void SelMap(int nSel);
	BOOL OpenReelmap(CString sPath); // for Test
	void OpenReelmap(int nSelRmap);
	void DispReelmap(int nSerial, BOOL bDumy=FALSE);
	void DispMain(CString sMsg, COLORREF rgb=RGB_GREEN);
	CString GetDispMain();
	void RefreshRmap();
	void DispLotTime();
	void DispStTime();
	void DispRunTime();
	void DispEdTime();
	CString GetStTime();
	CString GetRunTime();
	CString GetEdTime();
	int GetSerial();
	BOOL SetSerial(int nSerial, BOOL bDumy=FALSE);
	void UpdateWorking();
	void UpdateData();
	void UpdateRst();
	void ResetMkInfo();
	void ResetMkInfoUp();
	void ResetMkInfoDn();
	void SetRmapRgn();
	void ResetLotTime();
	void InitGL();
	void InitDefInfo();
	void InitDefInfoUp();
	void InitDefInfoDn();
	void ClrCadImg();
	void ClrCadImgUp();
	void ClrCadImgDn();
	void ClrDefImg();
	void ClrDefImgUp();
	void ClrDefImgDn();
//	void ClrRmap();
	void SetPnlNum();
	void SetPnlDefNum();
	void ClrInfo();
	void ResetGL();
	void LotEnd();
	int GetCurSerial();
	void ResetSerial();
	void InitCadImg();
	void InitCadImgUp();
	void InitCadImgDn();
	BOOL IsDoneDispMkInfo();
	void UpdateLotTime();
	void DispDef();
	void DispStripRatio();
	void DispTotRatio();

	void SetLastProc();
	BOOL IsLastProc();
	void SetStripAllMk();
	void SetTempStop(BOOL bOn);
	BOOL GetChkStrip(int nStrip); // 0, 1, 2, 3

	BOOL SetSerialReelmap(int nSerial, BOOL bDumy=FALSE);
	BOOL SetSerialMkInfo(int nSerial, BOOL bDumy=FALSE);

	void DispMkCnt();
	void WriteMkCnt();
	void ChkMkLimit();

	void EnableBtn(BOOL bEnable=TRUE);
	BOOL IsEnableBtn();
	void ChkPartialSpd();
	void ResetLastProc();
	void UpdateTotVel(CString sVel);
	void SetDualTest(BOOL bOn);
	void GetCtrlPos();
	void ShiftCtrlPos(BOOL bOrg);

	afx_msg LRESULT OnMyBtnDblClk(WPARAM wPara, LPARAM lPara);

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

	void SwMyBtnDown(int nCtrlID);
	void SwMyBtnUp(int nCtrlID);

	void ChkTpStop();

// Dialog Data
	//{{AFX_DATA(CDlgMenu01)
	enum { IDD = IDD_DLG_MENU_01 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu01)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu01)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnChkUserInfo();
	afx_msg void OnPaint();
	//afx_msg void OnChkTpStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnChkEjectBuffer();
	afx_msg void OnChkReview();
	afx_msg void OnStcReviewLen();
	afx_msg void OnChkReviewMove();
	afx_msg void OnChkLotEnd();
	afx_msg void OnChkDefUp();
	afx_msg void OnChkDefDn();
	afx_msg void OnChkDefAll();
	afx_msg void OnStcUp();
	afx_msg void OnStcDn();
	afx_msg void OnBtnMkAll();
	afx_msg void OnChkRemarking();
	afx_msg void OnChkElec();
	afx_msg void OnChkElecStop();
	afx_msg void OnChk2layer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedChkMk1();
	//afx_msg void OnBnClickedChkMk2();
	//afx_msg void OnBnClickedChkMk3();
	//afx_msg void OnBnClickedChkMk4();
	//afx_msg void OnDoubleclickedChkMk1();
	//afx_msg void OnDoubleclickedChkMk2();
	//afx_msg void OnDoubleclickedChkMk3();
	//afx_msg void OnDoubleclickedChkMk4();
	//afx_msg void OnBnClickedChkTpStop();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU01_H__FD6374B9_4CE8_4830_8693_759B27C002B1__INCLUDED_)
