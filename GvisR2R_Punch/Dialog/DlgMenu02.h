#if !defined(AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_)
#define AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu02.h : header file
//

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyStatic.h"
#include "MyGL.h"
#include "../Global/MyData.h"
#include "DlgUtil01.h"
#include "DlgUtil03.h"
#include "DlgUtil06.h"

#define MAX_MENU02_STC				23
#define MAX_MENU02_STC_DATA			17
#define MAX_MENU02_BTN				35

#define MAX_MENU02_STC_DUO			15
#define MAX_MENU02_STC_DATA_DUO		17
#define MAX_MENU02_BTN_DUO			29

#define TIM_LIGHT_UP				200
#define TIM_LIGHT_DN				201
#define TIM_LIGHT_UP2				210
#define TIM_LIGHT_DN2				211
#define TIM_BUF_ENC					202
#define TIM_MARKING_OFF				203
#define TIM_MARKING2_OFF			204
#define TIM_SHOW_MK_TEST			206
#define TIM_SHOW_ELEC_TEST			205

#define TIM_2D_READING				207
#define TIM_TEST_2D_READING			208


#define WAIT_MARKING				50		// [mSec]

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu02 dialog

class CDlgMenu02 : public CDialog
{
	CDlgUtil01 *m_pDlgUtil01;
	CMyStatic myStcPcsImg;//, myStcPinImg;

	CMyBtn myBtn[MAX_MENU02_BTN];
	CMyLabel myStcTitle[MAX_MENU02_STC];
	CMyStatic myStcData[MAX_MENU02_STC_DATA];

	CMyBtn myBtn2[MAX_MENU02_BTN_DUO];
	CMyLabel myStcTitle2[MAX_MENU02_STC_DUO];
	CMyStatic myStcData2[MAX_MENU02_STC_DATA_DUO];

	CMyGL *m_pPcsGL;//, *m_pPinGL;
	BOOL m_bLoadImg, m_bLockTimer;

	BOOL m_bTIM_LIGHT_UP, m_bTIM_LIGHT_DN, m_bTIM_LIGHT_UP2, m_bTIM_LIGHT_DN2, m_bTIM_BUF_ENC;
	double m_dStOffsetX, m_dStOffsetY;
	double m_dCurPosX[2], m_dCurPosY[2];
	int m_nSelectCam0Pos, m_nSelectCam1Pos;
	int m_nSpd;
	unsigned long m_lChk;
	int m_nMoveAlign[2];

	ULONGLONG m_stTime;

	void LoadImg();
	void DelImg();

	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();
//	void InitGL();
	void InitSlider();
	void DispBufEnc();
	void DispAxisPos();
	void DispCenterMark();
	void MarkingOff();
	void MarkingOff2();
	CfPoint GetPcsCenterOffset(CPoint pt);
	void MsClr(int nMsId);
	void ChkElecTest();
	void ChkMarkingTest2();

	void InitStaticDuo();
	void InitStcTitleDuo();
	void InitStcDataDuo();
	void InitBtnDuo();
	void ShowDlg(int nID);
	
// 	void InitResMeas();
// 	void SetResMeas(int nStep);

// Construction
public:
	CDlgMenu02(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu02();

	CRect* m_pRect;
	int m_nJogSpd;
	int m_nBtnAlignCam0Pos, m_nBtnAlignCam1Pos;
	double m_dMkFdOffsetX[2][4], m_dMkFdOffsetY[2][4];	// 2Cam / 4Point Align
	double m_dAoiUpFdOffsetX, m_dAoiUpFdOffsetY;
	double m_dAoiDnFdOffsetX, m_dAoiDnFdOffsetY;
// 	BOOL m_bMkDnSolOff;

	double m_dOneShotRemainLen;
	void DispOneShotRemainLen();

	CDlgUtil03 *m_pDlgUtil03;
	//CDlgUtil06 *m_pDlgUtil06;


	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();

	afx_msg LRESULT OnDrawPcsImg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnDrawPinImg(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyStaticRedraw(WPARAM wPara, LPARAM lPara);
// 	void OnMyStaticMouseMove(WPARAM wPara, LPARAM lPara);

	int GetJogSpd();
	void SetJogSpd();
	void SetJogSpd2();
	void SetJogSpd(int nSpd);
	void SetJogSpd2(int nSpd);
	void SetLight(int nVal=-1);
	void SetLight2(int nVal=-1);
	void ResetLight();
	void ResetLight2();
	BOOL MovePos(int nPos);
	BOOL MovePos2(int nPos);
	BOOL MovePinPos();
	BOOL MovePinPos2();
	BOOL MoveAlign0(int nPos);
	BOOL Move2PntAlign0(int nPos);
	BOOL Move4PntAlign0(int nPos);
	BOOL MoveAlign1(int nPos);
	BOOL Move2PntAlign1(int nPos);
	BOOL Move4PntAlign1(int nPos);
	BOOL Grab(int nPos, BOOL bMove=FALSE);
	BOOL GetPmRst0(double &dX, double &dY, double &dAgl, double &dScr);
	BOOL GetPmRst1(double &dX, double &dY, double &dAgl, double &dScr);

	BOOL OnePointAlign(CfPoint &ptPnt);
	BOOL OnePointAlign2(CfPoint &ptPnt);
	BOOL TwoPointAlign(int nPos);
	BOOL TwoPointAlign0(int nPos);
	BOOL TwoPointAlign1(int nPos);
	BOOL Do2PtAlign0(int nPos, BOOL bDraw = FALSE);
	BOOL Do2PtAlign1(int nPos, BOOL bDraw = FALSE);
	BOOL FourPointAlign(int nPos);
	BOOL FourPointAlign0(int nPos);
	BOOL FourPointAlign1(int nPos);
	BOOL Do4PtAlign0(int nPos, BOOL bDraw = FALSE);
	BOOL Do4PtAlign1(int nPos, BOOL bDraw = FALSE);

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

	void SwMyBtnDown(int nCtrlID);
	void SwMyBtnUp(int nCtrlID);
	void SwMarking();
	void SwMarking2();

//	void SetMkPos(CfPoint ptOfst);
	void SetMkCurPos(int nCam);
	void SetMkDlyOff(int nCam, int mSec=WAIT_MARKING);
	void SetPinPos(int nCam, CfPoint ptPnt);
//	void DispLocalSpec();
// 	void SetPcsOffset();
	void ChgModel();
	void ChgModelUp();
	void ChgModelDn();

// 	void PinSave();
	void ResetMotion(int nMsId);

	BOOL ShowKeypad(int nCtlID, CPoint ptSt=(0, 0), int nDir=TO_NONE);

	void ResetMkTestBtn();
	void InitCadImg();

	void Grab2PntAlign();
	void Grab4PntAlign();

	void Grab2PntAlign2();
	void Grab4PntAlign2();

// Dialog Data
	//{{AFX_DATA(CDlgMenu02)
	enum { IDD = IDD_DLG_MENU_02 };
	CSliderCtrl	m_LightSlider2;
	CSliderCtrl	m_LightSlider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu02)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu02)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnChkLtOn();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	afx_msg void OnChkJogVf();
	afx_msg void OnChkJogF();
	afx_msg void OnChkJogN();
	afx_msg void OnChkJogS();
	afx_msg void OnChkJogVs();
	afx_msg void OnBtnPinMove();
	afx_msg void OnBtnPinSave();
	afx_msg void OnBtnHomeMove();
	afx_msg void OnChkResPosSt();
	afx_msg void OnChkMkOffsetSt();
	afx_msg void OnChkMkOffsetEd();
	afx_msg void OnBtnAlignMove();
	afx_msg void OnBtnAlign();
	afx_msg void OnBtnGrab();
	afx_msg void OnBtnBuffHome();
	afx_msg void OnBtnBuffInitMove();
	afx_msg void OnBtnBuffInitSave();
	afx_msg void OnBtnHomeSave();
	afx_msg void OnStcAlignStdScr();
	afx_msg void OnChkLtOn2();
	afx_msg void OnChkResPosSt2();
	afx_msg void OnBtnPinMove2();
	afx_msg void OnBtnHomeMove2();
	afx_msg void OnBtnStartSave2();
	afx_msg void OnBtnPinSave2();
	afx_msg void OnChkMkOffsetSt2();
	afx_msg void OnBtnAlign2();
	afx_msg void OnChkMarkingTest2();
	afx_msg void OnChkMkOffsetEd2();
	afx_msg void OnChkJogVf2();
	afx_msg void OnChkJogF2();
	afx_msg void OnChkJogN2();
	afx_msg void OnChkJogS2();
	afx_msg void OnChkJogVs2();
	afx_msg void OnChkMk0Pos1();
	afx_msg void OnChkMk0Pos2();
	afx_msg void OnChkMk0Pos3();
	afx_msg void OnChkMk0Pos4();
	afx_msg void OnBtnMk0PosMove();
	afx_msg void OnChkMk1Pos1();
	afx_msg void OnChkMk1Pos2();
	afx_msg void OnChkMk1Pos3();
	afx_msg void OnChkMk1Pos4();
	afx_msg void OnBtnMk1PosMove();
	afx_msg void OnBtnMk0PosMeas();
	afx_msg void OnBtnMk1PosMeas();
	afx_msg void OnBtnAlignMove2();
	afx_msg void OnBtnGrab2();
	afx_msg void OnStcAlignStdScr2();
	afx_msg void OnChkElecTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU02_H__20579F9A_60DF_4376_99B4_C63C1CACF17E__INCLUDED_)
