#if !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
#define AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Smac.h : header file
//
#include "Rs232.h"

#define TIMER_FIRST_SMAC_GET_POS			100
#define TIMER_SECOND_SMAC_GET_POS			101
#define TIMER_FIRST_SMAC_CHECK_END_CMD		110
#define TIMER_SECOND_SMAC_CHECK_END_CMD		111
#define TIMER_FIRST_SMAC_CHECK_ERROR_CODE	120
#define TIMER_SECOND_SMAC_CHECK_ERROR_CODE	121

/////////////////////////////////////////////////////////////////////////////
// CSmac window

class CSmac : public CWnd
{
// Construction
	CWnd*			m_pParent;
	CCriticalSection m_cs;
	BOOL m_bCh[MAX_VOICE_COIL];
	int m_nCh;
	CString m_sRcvRs232;
	BOOL Send(CString str);

public:
	CSmac(CWnd* pParent =NULL);

// Attributes
public:
	CRs232 m_Rs232;
	BOOL m_bRs232Run;

	CString m_strReceiveVoiceCoilFirstCam, m_strReceiveVoiceCoilSecondCam;

	BOOL m_bReturnFirstSmacCmdEnd, m_bReturnSecondSmacCmdEnd, m_bRunTimerCheckFirstSmacEnd, m_bRunTimerCheckSecondSmacEnd;

	CString m_strFirstSmacEndCmd, m_strSecondSmacEndCmd;
	BOOL m_bTimerStop, m_bFeedingRollerUp;

// Operations
public:
	afx_msg LRESULT OnReceiveRs232(WPARAM wP, LPARAM lP);
	CString Rcv();

	void Init();
	void SetCh(int nCh);
	void Reset(int nCh);
	void Close();

	double GetSmacMeasureOfSurface(int nCamNum);
	void SendStringToFirstCamVoiceCoil(CString strSend);
	void SendStringToSecondCamVoiceCoil(CString strSend);
	BOOL WaitSmacCmdEnd(int nCamNum, CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL SetWaitSmacCmdEnd0(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL SetWaitSmacCmdEnd1(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL WaitSmacCmdEnd0(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL WaitSmacCmdEnd1(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	double GetSmacPosition(int nCamNum);
	void SearchHomeSmac(int nCamNum);
	void SearchHomeSmac0();
	void SearchHomeSmac1();
	BOOL IsDoneSearchHomeSmac0();
	BOOL IsDoneSearchHomeSmac1();
	int CheckSmacErrorCode(int nCamNum);
	CString DisplaySmacErrorList(int nErrCode);
	void Wait(int imSecond);
	void CalcAveSmacHeight(int nCamNum);
	void ResetSmac(int nCamNum);
	CString GetSmacStatus(int nCamNum);
	void MoveSmacFinalPos(int nCamNum);
	void MoveSmacShiftPos(int nCamNum);
	void MoveSmacShiftPos0();
	void MoveSmacShiftPos1();
	BOOL IsDoneMoveSmacShiftPos0();
	BOOL IsDoneMoveSmacShiftPos1();
	void MoveSmacMeasPos(int nCamNum);

	void SetMarking(int nCamNum);
	void SetMarkShiftData(int nCamNum);
	void SetMarkFinalData(int nCamNum);
	void SetMark(int nCamNum);
	BOOL IsDoneMark(int nCamNum);
	
	void SetProbing(int nCamNum);
	void SetProbShiftData(int nCamNum);
	void SetProbFinalData(int nCamNum);
	void SetProb(int nCamNum);
	BOOL IsDoneProb(int nCamNum);
	void MoveProbFinalPos(int nCamNum);

	BOOL SetCmdEndChk(int nCamNum, CString strEndCmd);
	void ClearReceive();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmac)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmac();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmac)
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
