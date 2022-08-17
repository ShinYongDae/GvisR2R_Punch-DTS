#pragma once

#include "afxwin.h"
#include "TcpIpClient.h"
#include "EngraveDef.h"


#define TIM_CONNECT			1
#define TIM_CHECK_CONNECT	10

#define DELAY_CHECK_CONNECT	5000
#define DELAY_RESPONSE		10000

class CEngrave : public CWnd
{
	HWND m_hParentWnd;
	CTcpIpClient* m_pClient;

	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	int m_nServerID;

	BOOL m_bWaitForResponse;
	CString m_strResponse;
	int m_nCmd;
	BOOL m_bCmdF;

	SOCKET_DATA m_SocketData;
	BOOL m_bAutoConnect;
	BOOL m_bTIM_CHECK_CONNECT;
	
	void StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr);
	void StopClient();
	BOOL CheckResponse(int nCmd, CString sResponse);
	BOOL WaitResponse();

public:
	CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent = NULL);
	~CEngrave();

	void SetHwnd(HWND hParentWnd);
	BOOL IsRunning();
	int WriteComm(CString sMsg, DWORD dwTimeout = INFINITE);
	BOOL ReadComm(CString &sData);
	SOCKET_DATA GetSocketData();

	// Thread
	CEvent      m_evtThread;
	CWinThread *m_pThread;
	HANDLE		m_hThread;
	BOOL		m_bModify;
	BOOL		m_bAlive;

	void StartThread();
	void StopThread();
	void WaitUntilThreadEnd(HANDLE hThread);
	static UINT RunThread(LPVOID pParam);

	int OnClientReceived(WPARAM wParam, LPARAM lParam);
	void Close();

	// General Function
	BOOL SendCommand(SOCKET_DATA SocketData, BOOL bWait=FALSE);
	BOOL IsConnected();

	// Communcation

	// GetSysSignal
	void GetSysSignal(SOCKET_DATA SockData);
	void GetSignalMain(SOCKET_DATA SockData);
	void GetSignalTorqueMotor(SOCKET_DATA SockData);
	void GetSignalInductionMotor(SOCKET_DATA SockData);
	void GetSignalCore150mm(SOCKET_DATA SockData);
	void GetSignalEtc(SOCKET_DATA SockData);
	void GetSignalRecoiler(SOCKET_DATA SockData);
	void GetSignalPunch(SOCKET_DATA SockData);
	void GetSignalAOIDn(SOCKET_DATA SockData);
	void GetSignalAOIUp(SOCKET_DATA SockData);
	void GetSignalEngrave(SOCKET_DATA SockData);
	void GetSignalUncoiler(SOCKET_DATA SockData);

	// GetSysData
	void GetSysData(SOCKET_DATA SockData);
	void GetOpInfo(SOCKET_DATA SockData);
	void GetInfo(SOCKET_DATA SockData);
	void GetTotRatio(SOCKET_DATA SockData);
	void GetStTime(SOCKET_DATA SockData);
	void GetRunTime(SOCKET_DATA SockData);
	void GetEdTime(SOCKET_DATA SockData);
	void GetStripRatio(SOCKET_DATA SockData);
	void GetDef(SOCKET_DATA SockData);
	void Get2DReader(SOCKET_DATA SockData);
	void GetEngInfo(SOCKET_DATA SockData);
	void GetFdInfo(SOCKET_DATA SockData);
	void GetAoiInfo(SOCKET_DATA SockData);
	void GetMkInfo(SOCKET_DATA SockData);
	void GetMkInfoLf(SOCKET_DATA SockData);
	void GetMkInfoRt(SOCKET_DATA SockData);

	// SetSysSignal
	BOOL SetSysSignal();
	void SetSignalMain();
	void SetSignalTorqueMotor();
	void SetSignalInductionMotor();
	void SetSignalCore150mm();
	void SetSignalEtc();
	void SetSignalRecoiler();
	void SetSignalPunch();
	void SetSignalAOIDn();
	void SetSignalAOIUp();
	void SetSignalEngrave();
	void SetSignalUncoiler();

	// SetSysData
	BOOL SetSysData();
	void SetOpInfo();
	void SetInfo();
	void SetTotRatio();
	void SetStTime();
	void SetRunTime();
	void SetEdTime();
	void SetStripRatio();
	void SetDef();
	void Set2DReader();
	void SetEngInfo();
	void SetFdInfo();
	void SetAoiInfo();
	void SetMkInfo();
	void SetMkInfoLf();
	void SetMkInfoRt();

	// Communcation - Sub Item

	// SetOpInfo()
	void SetOpName();
	void SetDualTest();
	void SetSampleTest();
	void SetSampleShotNum();
	void SetTestMode();
	void SetRecoilerCcw();
	void SetUncoilerCcw();
	void SetAlignMethode();
	void SetDoorRecoiler();
	void SetDoorAoiUp();
	void SetDoorAoiDn();
	void SetDoorMk();
	void SetDoorEngrave();
	void SetDoorUncoiler();
	void SetSaftyMk();
	void SetCleannerAoiUp();
	void SetCleannerAoiDn();
	void SetUltraSonicAoiDn();
	void SetUltraSonicEngrave();
	void SetTotReelLen();
	void SetOnePnlLen();
	void SetTempPause();
	void SetTempStopLen();
	void SetLotCut();
	void SetLotCutLen();
	void SetLotCutPosLen();
	void SetLmtTotYld();
	void SetLmtPatlYld();
	void SetStripOutRatio();
	void SetCustomNeedRatio();
	void SetNumRangeFixDef();
	void SetNumContFixDef();
	void SetUltraSonicStTim();

	// SetInfo()
	void SetModelUpName();
	void SetModelDnName();
	void SetLotUpName();
	void SetLotDnName();
	void SetLayerUpName();
	void SetLayerDnName();
	void SetOrderNum();
	void SetShotNum();
	void SetTotOpRto();
	void SetTotVel();
	void SetPartVel();
	void SetMkDoneLen();
	void SetAoiDnDoneLen();
	void SetAoiUpDoneLen();
	void SetLotSerial();
	void SetMkVerfyLen();

	// SetTotRatio()
	void SetDefNumUp();
	void SetDefRtoUp();
	void SetGoodNumUp();
	void SetGoodRtoUp();
	void SetTestNumUp();
	void SetDefNumDn();
	void SetDefRtoDn();
	void SetGoodNumDn();
	void SetGoodRtoDn();
	void SetTestNumDn();
	void SetDefNumTot();
	void SetDefRtoTot();
	void SetGoodNumTot();
	void SetGoodRtoTot();
	void SetTestNumTot();

	// Set2DReader()
	void Set2DEngLen();
	void Set2DAoiLen();
	void Set2DMkLen();
	void Set2DMoveVel();
	void Set2DMoveAcc();
	void Set2DOneShotLen();

	// SetEngInfo()
	void SetEngLeadPitch();
	void SetEngPushOffLen();
	void SetEngTqVal();
	void SetEngAoiLen();
	void SetEngFdDiffMax();
	void SetEngFdDiffRng();
	void SetEngFdDiffNum();
	void SetEngBuffInitPos();
	void SetEngBuffCurrPos();

	// SetFdInfo()
	void SetFdVel();
	void SetFdAcc();
	void SetOnePnlVel();
	void SetOnePnlAcc();
	void SetFdDiffMax();
	void SetFdDiffRng();
	void SetFdDiffNum();

	// SetAoiInfo()
	void SetAoiLeadPitch();
	void SetAoiPushOffLen();
	void SetAoiTqVal();
	void SetAoiBuffShotNum();
	void SetAoiMkLen();

	// SetMkInfo()
	void SetMkLeadPitch();
	void SetMkPushOffLen();
	void SetMkBuffInitPos();
	void SetMkBuffCurrPos();
	void SetMkNumLf();
	void SetMkNumRt();
	void SetMkMaxNumLf();
	void SetMkMaxNumRt();

	// SetMkInfoLf()
	void SetMkInitPosLf();
	void SetMkInitVelLf();
	void SetMkInitAccLf();
	void SetMkFnlPosLf();
	void SetMkFnlVelLf();
	void SetMkFnlAccLf();
	void SetMkFnlTqLf();
	void SetMkHgtPosX1Lf();
	void SetMkHgtPosY1Lf();
	void SetMkHgtPosX2Lf();
	void SetMkHgtPosY2Lf();
	void SetMkHgtPosX3Lf();
	void SetMkHgtPosY3Lf();
	void SetMkHgtPosX4Lf();
	void SetMkHgtPosY4Lf();

	// SetMkInfoRt()
	void SetMkInitPosRt();
	void SetMkInitVelRt();
	void SetMkInitAccRt();
	void SetMkFnlPosRt();
	void SetMkFnlVelRt();
	void SetMkFnlAccRt();
	void SetMkFnlTqRt();
	void SetMkHgtPosX1Rt();
	void SetMkHgtPosY1Rt();
	void SetMkHgtPosX2Rt();
	void SetMkHgtPosY2Rt();
	void SetMkHgtPosX3Rt();
	void SetMkHgtPosY3Rt();
	void SetMkHgtPosX4Rt();
	void SetMkHgtPosY4Rt();

	// End for SetSysData()

	// Start Switch

	// Main
	void SwReady(BOOL bOn);
	void SwRun(BOOL bOn);
	void SwReset(BOOL bOn);
	void SwStop(BOOL bOn);
	void SwAuto(BOOL bOn);
	void SwManual(BOOL bOn);

	// Torque Motor
	void SwMkTq(BOOL bOn);
	void SwAoiTq(BOOL bOn);
	void SwEngTq(BOOL bOn);

	// Induction Motor
	void SwRcInductionCcw(BOOL bOn);	// SetOneMetal
	void SwUcInductionCcw(BOOL bOn);	// SetTwoMetal

	// Core 150mm
	void SwRcCore150mm(BOOL bOn);
	void SwUcCore150mm(BOOL bOn);

	// Etc
	void SwAoiEmg(BOOL bOn);
	//void SwBufRolSol(BOOL bOn);
	//void SwMkDnSol(BOOL bOn);

	// Recoiler
	void SwRcRelation(BOOL bOn);
	void SwRcFdCw(BOOL bOn);
	void SwRcFdCcw(BOOL bOn);
	void SwRcReelChuck(BOOL bOn);
	void SwRcDcRlUpDn(BOOL bOn);
	void SwRcReelJoinL(BOOL bOn);
	void SwRcReelJoinR(BOOL bOn);
	void SwRcReelJoinVac(BOOL bOn);
	void SwRcPaperChuck(BOOL bOn);
	void SwRcPaperCw(BOOL bOn);
	void SwRcPaperCcw(BOOL bOn);
	void SwRcRewinder(BOOL bOn);
	void SwRcRewinderReelPaper(BOOL bOn);

	// Punching
	void SwMkRelation(BOOL bOn);
	void SwMkFdCw(BOOL bOn);
	void SwMkFdCcw(BOOL bOn);
	void SwMkFdVac(BOOL bOn);	
	void SwMkPushUp(BOOL bOn);
	void SwMkTblBlw(BOOL bOn);
	void SwMkTblVac(BOOL bOn);	
	void SwMkFdClp(BOOL bOn);	
	void SwMkTqClp(BOOL bOn);
	void SwMkMvOne(BOOL bOn);
	void SwMkLsrPt(BOOL bOn);

	void SwMkDcRSol(BOOL bOn);

	// AOIDn
	void SwAoiDnRelation(BOOL bOn);
	void SwAoiDnFdCw(BOOL bOn);
	void SwAoiDnFdCcw(BOOL bOn);
	void SwAoiDnFdVac(BOOL bOn);
	void SwAoiDnPushUp(BOOL bOn);
	void SwAoiDnTblBlw(BOOL bOn);
	void SwAoiDnTblVac(BOOL bOn);
	void SwAoiDnFdClp(BOOL bOn);
	void SwAoiDnTqClp(BOOL bOn);
	void SwAoiDnMvOne(BOOL bOn);
	void SwAoiDnLsrPt(BOOL bOn);

	void SwAoiDnClrRoll(BOOL bOn);
	void SwAoiDnSonicBlw(BOOL bOn);

	void SwAoiDnTest(BOOL bOn);
	void SwAoiDnReset(BOOL bOn);
	void SwAoiDnLotEnd(BOOL bOn);

	// AOIUp
	void SwAoiUpRelation(BOOL bOn);
	void SwAoiUpFdCw(BOOL bOn);
	void SwAoiUpFdCcw(BOOL bOn);
	void SwAoiUpFdVac(BOOL bOn);
	void SwAoiUpPushUp(BOOL bOn);
	void SwAoiUpTblBlw(BOOL bOn);
	void SwAoiUpTblVac(BOOL bOn);
	void SwAoiUpFdClp(BOOL bOn);
	void SwAoiUpTqClp(BOOL bOn);
	void SwAoiUpMvOne(BOOL bOn);
	void SwAoiUpLsrPt(BOOL bOn);

	void SwAoiUpClrRoll(BOOL bOn);
	void SwAoiUpTqVac(BOOL bOn);

	void SwAoiUpTest(BOOL bOn);
	void SwAoiUpReset(BOOL bOn);
	void SwAoiUpLotEnd(BOOL bOn);

	// Engrave
	void SwEngRelation(BOOL bOn);
	void SwEngFdCw(BOOL bOn);
	void SwEngFdCcw(BOOL bOn);
	void SwEngFdVac(BOOL bOn);
	void SwEngPushUp(BOOL bOn);
	void SwEngTblBlw(BOOL bOn);
	void SwEngTblVac(BOOL bOn);
	void SwEngFdClp(BOOL bOn);
	void SwEngTqClp(BOOL bOn);
	void SwEngMvOne(BOOL bOn);
	void SwEngLsrPt(BOOL bOn);

	void SwEngDcRSol(BOOL bOn);
	void SwEngSonicBlw(BOOL bOn);

	// Uncoiler
	void SwUcRelation(BOOL bOn);
	void SwUcFdCw(BOOL bOn);
	void SwUcFdCcw(BOOL bOn);
	void SwUcReelChuck(BOOL bOn);
	void SwUcDcRlUpDn(BOOL bOn);
	void SwUcReelJoinL(BOOL bOn);
	void SwUcReelJoinR(BOOL bOn);
	void SwUcReelJoinVac(BOOL bOn);
	void SwUcPaperChuck(BOOL bOn);
	void SwUcPaperCw(BOOL bOn);
	void SwUcPaperCcw(BOOL bOn);
	void SwUcClRlUpDn(BOOL bOn);
	void SwUcClRlPshUpDn(BOOL bOn);

	// End Switch

protected:
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientClosed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

