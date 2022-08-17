#if !defined(AFX_MOTIONNEW_H__A8D61E2C_EE72_4D14_8452_9B10B0D2E36D__INCLUDED_)
#define AFX_MOTIONNEW_H__A8D61E2C_EE72_4D14_8452_9B10B0D2E36D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionNew.h : header file
//


#ifdef USE_NMC

#include "NmcDevice.h"

#define MAX_SEQ			16
#define MAX_CMD			2
#define LAST_CMD		7

#define SCAN_MODE_SAFE	0
#define SCAN_MODE_ASYNC	1
#define SCAN_MODE_HOLD 2



typedef struct _tagStructScanParam
{
	_tagStructScanParam()
	{
		nScanMode = SCAN_MODE_SAFE;
		bHoldAction = 0;
		dHoldingPosX = 0;
		dNextSwathStartPosY = 0;
		bScanPosMove = 0;
	}

	int nScanMode;
	BOOL bHoldAction;
	double dHoldingPosX;
	double dNextSwathStartPosY;
	BOOL bScanPosMove;
}SCAN_STRUCT;



class CMotionNew : public CMotionAdapter
{
public:
	CMotionNew(CWnd* pParent = NULL);

	UINT16	m_nBoardId;
	UINT16	m_nDevIdIoIn;
	UINT16	m_nDevIdIoOut;
	INT		m_nOffsetAxisID;
	int		m_nGroupID_RTAF;
	int		m_nGroupID_ErrMapXY;
	int		m_nGroupID_Interpolation;

	CString m_sErrorMappingFilePath;
	double m_dScanSpeed;
	double m_dScanVel;

	int In32(long *value);
	int Out32(long value);
	double GetSpeedProfile(int nMode, int nAxisID, double fMovingLength, double &fVel, double &fAcc, double &fJerk, int nSpeedType = LOW_SPEED);
	BOOL ClearStatus(int nMsId);
	BOOL GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk);
	double GetMotionTime(double dLen, double dVel, double dAcc, double dJerk);
	BOOL CreateObject();

	CNmcDevice *m_pMotionCard;


	BOOL m_bMonitorThreadAlive;
	double m_fStartPos;


	BOOL m_bUseGantry;
	long m_lGantryMaster;
	long m_lGantrylSlave;
	long m_lGantryEnable;
	BOOL m_bGantryEnabled;

	double	m_fMaxVel[MAX_AXIS];
	double  m_fPosLimit[MAX_AXIS];			// Positive Software Limit
	double  m_fNegLimit[MAX_AXIS];			// Negative Software Limit
	double	m_fMaxAccel[MAX_AXIS];		// Maximum Acceleration in G

	double m_fVel[MAX_AXIS];
	double m_fAcc[MAX_AXIS];
	double m_fDec[MAX_AXIS];
	double m_fAccPeriod[MAX_AXIS]; // [%]
	double m_fMinJerkTime[MAX_AXIS];

	static CMotionNew* GetMotion();

// Attributes
public: 
	int m_nRTAFUniSeqLastIndex;
	CCriticalSection m_csGetActualPosition;
	CCriticalSection m_csGetActualVelocity;

	void TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes);	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
	BOOL TriggerStop(int nEcatAddr, int vAxisId);
	BOOL TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx);
	CString GetTriggerEncCnt(int nEcatAddr, int nSdoIdx);



	BOOL GantryEnable(long lMaster, long lSlave, long lOnOff);

	void StopLMI();
	void SetCleanDoorFaultLimit();
	BOOL MoveCleanDoorMotor(double fTgtPos, double dSpd, double dAcc, double dDec, BOOL bWait);
	BOOL MoveCleanDoorMotorRatio(double fTgtPos, double fVelRatio, BOOL bWait);
	long CreateAutoFocusingSeq3_31_RTAF(structRTAFInfo* pPointArr, double dRefHeightAtTestPoint, int nTotal, int nDir);
	int SetRTAFTrigger(int nMotorSwath, int nDir, int nTrgrCh, double dStPos, double dEdPos);
	void EnableRTAF(BOOL bEnable, BOOL bPanelSync = 0, BOOL bPartialMode = 0);
	void SetCurrentMotorSwath(int nSwath);
	void SetOrgMotorStartPos(double dYPos);
	int MoveMotorSafety(double fTgtPos, double dVelRatio, int nMsID, BOOL bWait);
	BOOL FreeAll();
	void LoadParam();
	BOOL LoadAoiMotionParametersFromAxis(int nAxis);
	double GetAdjValErrMapData(double dStdPosX, double dStdPosY);
	BOOL InitAtf();
	void SetConfigure();
	void MotionAbortAll(); //160610	
	void SetMotionParam();
	void ShutdownHomeThreadAll();
	BOOL CreateBackwardSequenceDataForUniHeader(double dOrgFocus);

	// [MS Summary]
	BOOL MotionAbort(int nMsId);
	BOOL Abort(int nMsId);
	BOOL EStop(int nMsId);
	BOOL ClearMotionFault(int nMsId);
	BOOL IsStop(int nMsId);
	BOOL Stop(int nMsId, double fStopTime = 0.001);

	// [Axis Summary]
	BOOL IsMotionDone(int nAxisId);
	BOOL IsInPosition(int nAxisId);
	BOOL CheckMotionDone(int nAxisId);
	BOOL IsAxisDone(int nAxisId);
	BOOL CheckAxisDone(int nAxisId);
	double GetActualPosition(int nAxisId);
	double GetCommandPosition(int nAxisId);
	int GetAxisState(int nAxisId);
	BOOL SetActualPosition(int nAxisId, double fData);
	BOOL SetCommandPosition(int nAxisId, double fData);
	BOOL SetSettleTime(int nAxisId, float fTime);
	void SetErrorLimitAction(int nMotorID, MPIAction Action);

	// [Motor Summary]
	double GetPosSWLimitValue(int nMotorId);
	double GetNegSWLimitValue(int nMotorId);
	BOOL ServoOnOff(int nMotorId, BOOL bOnOff);
	BOOL CheckSWLimitFault(int nMotorId);
	void ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue = 100000000.);
	void ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue = -100000000.);
	BOOL IsAmpFault(int nMotorId);
	BOOL GetLimitInput(int nMotorId, int nDir); // 지정된 방?EFALSE : - , TRUE : +)의 Hardware Limit sensor의 상태를 리턴한다.


												// [General Function]
	BOOL InitBoard();
	BOOL IsReadyToMove(CString& strDispMsg, int nMotor = -1, int nDirection = 2); // 20180525 - syd
	int GotoLoadingPosition(BOOL bWait = WAIT, BOOL bCheckClamp = 1);
	BOOL IsLoadingPosition();
	BOOL MoveScanMotor(double fTgtPos, double fVelRatio = 100.0, BOOL bWait = NO_WAIT);
	BOOL StartPosMove(int nMsId, double fPos, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = S_CURVE);
	BOOL StartPosMove(int nMsId, double fPos, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	BOOL TwoStartPosMove(int nMsIdX, int nMsIdY, double fPosX, double fPosY, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = S_CURVE);
	BOOL TwoStartPosMove(int nMsIdX, int nMsIdY, double fPosX, double fPosY, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	double GetPositionResolution(int nMotorId);
	long CreateAutoFocusingSeq(BOOL bFirst, double fScanMotPos, double dFocusOffset, int bDir, double fHeightOfLaserSensor, int nSwath, int nHeightIndex);
	int GetSeqIdx(); // m_nSeqIdx
	void AddSeqIdx(int nAdd = 1); // m_nSeqIdx
	void ResetSeqIdx();
	BOOL MoveCleanerMotor(double fTgtPos, double fVelRatio = 100.0, BOOL bWait = NO_WAIT);
	BOOL MoveHeightMotor(double fTgtPos, double fVelRatio = 100.0, BOOL bWait = NO_WAIT);
	BOOL MoveChuckMotor(double fTgtPos, double fVelRatio, BOOL bWait = WAIT);
	BOOL MoveVerifyMotor(double fTgtPos, double fVelRatio = 100.0, BOOL bWait = NO_WAIT);
	BOOL MoveThetaMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveMagMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveZoomMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveInspZoomMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveFocusMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveFocus2Motor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveLightMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);
	BOOL MoveCamMotor(double fTgtPos, double fVelRatio = 100.0, BOOL bWait = NO_WAIT);
	BOOL MoveScanPos(double fTgtPosX, double fTgtPosY, BOOL bWait = NO_WAIT);
	int  MoveXY(double fTgtPosX, double fTgtPosY, BOOL bSpeed = LOW_SPEED, BOOL bWait = NO_WAIT, BOOL bCheckClamp = 1);
	BOOL WaitScanEnd(DWORD dwTimeOver = TIME_OUT_MS, BOOL bPumpMsg = 1);
	BOOL WaitGrabEnd(DWORD dwTimeOver = TIME_OUT_MS, BOOL bPumpMsg = 1); //160212 lgh add
	double GetScanEndPosition(double fLineRate, double fOptRes, double fCurPos, double fScanSize, int nScanDir, long nStartDummy = 1500, long nEndDummy = 1500);
	BOOL StartScan(BOOL bPreAlign, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir = FORWARD, long nStartDummy = 1500, long nEndDummy = 1500, double fCurrThicknessByLaserSensor = -100.0, SCAN_STRUCT* pScanParam = nullptr);
	BOOL StartScanForBidirection(BOOL bPreAlign, BOOL bHoldingMotion, double dNextSwathXPos, double dNextSwathStartPos, BOOL& bScanPosMove, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir = FORWARD, long nStartDummy = 1500, long nEndDummy = 1500, double fCurrThicknessByLaserSensor = -100.0);
	void ChangeMagMotorNegSWLimit(double fTgtFocusPos);
	BOOL WaitXYPosEnd(BOOL bMotionDone = TRUE, DWORD dwTimeOver = TIME_OUT_MS, BOOL bPumpMsg = 1);
	BOOL ThetaAdjust(double fAdjAngle, double fMaxAngle, double *fRetThetaPos, BOOL bWait = WAIT);
	BOOL WaitMotorPosEnd(int nAxisId, BOOL bMotionDone = TRUE, DWORD dwTimeOver = TIME_OUT_MS, BOOL bMsgPump = 1);
	long LengthToPulse(int nMotorId, double fLength);
	double PulseToLength(int nAxisId, long nPulse);
	BOOL SearchHomePos(int nAxisId, BOOL bThread = FALSE);
	BOOL SearchThetaHomePosOfGm();
	void SetSWLimitDirection(BOOL bDir);
	int SearchMachineStroke(int nMsId);
	double GetMagAxisStrokeLen(); // m_fMagAxisStrokeLen
	BOOL SetHomeShiftVal(int nMsId, double fShift); // m_MotionParam.Home.fShift
	double GetAccScanLength(double fLineRate, double fOptRes);
	CString CheckAllAxisErrorStatus();
	BOOL GetGantry(long lMaster, long lSlave, long *lOnOff);
	BOOL SetGantry(long lMaster, long lSlave, long lOnOff);
	BOOL CheckOriginRet(int nMsId); // m_bOrigin
	void DBNSetUp();
	BOOL IsOriginDone(int nMsId); // m_bOrigin
	BOOL InitRecorder();
	CString GetMotorName(int nAxisId);
	void Exit(void);
	int WaitHomeEnd(DWORD dwTimeOver = TIME_OUT_MS);
	int WaitMagAdjustEnd(DWORD dwTimeOver = TIME_OUT_MS, BOOL bThread = 0);
	CString GetSystemTime();
	BOOL StopAll(BOOL bWait = NO_WAIT);
	BOOL WaitMoveEnd(int nAxisID, DWORD dwTimeOver = TIME_OUT_MS, BOOL bThread = 0);
	BOOL GetRecordLastData(double &dScanPos, double &dThickPos, int bDir = FORWARD);
	double GetRecordStPosScan(); // m_dRecordStPosScan
	void SetRecordStPosScan(double dRecordStPosScan); // m_dRecordStPosScan
	BOOL GetAutoFocusingData(double fScanCurrPos, double &fScanMotPos, double &dFocusOffset, int bDir = FORWARD);
	long CreateAutoFocusingSeq2(double fScanMotPos, double dFocusOffset, int bDir = FORWARD);
	void StartRecorder();
	void StopRecorder();
	BOOL InitRecords();
	BOOL GetRecords();
	CString DispRecords();
	BOOL MoveScanPosModifiedSpeed(double fTgtPosX, double fTgtPosY, BOOL bWait = NO_WAIT, BOOL bClrSol = FALSE);
	double GetImageEndPosition(double fLineRate, double fOptRes, double fCurPos, double fImageSize, int nScanDir, long nStartDummy = 1500, long nEndDummy = 1500);
	void SetInspClrSol(double dSpdRatio, double dStPosY, double fOptRes, long nStartDummy = 1500);
	void SetInspClrSol(double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir = FORWARD, long nStartDummy = 1500, long nEndDummy = 1500, double fCurrThicknessByLaserSensor = -100.0);
	BOOL StartVelMove(int nAxisId, double fVel, double fAcc, double dPos=0.0);
	double GetVelocity(int nAxisId);
	void ResetAutoFocusingSeq();
	BOOL IsMonitorThreadAlive(); // m_bMonitorThreadAlive
	void SetMonitorThreadAlive(BOOL bAlive); // m_bMonitorThreadAlive


	BOOL IsLimit(int nMotionId, int nDir);
	long GetState(int nMotionId);
	BOOL Clear(int nMotionId);
	BOOL IsServoOn(int nMotorID);
	BOOL IsEncoderFault(int nAxisId);
	void SetStopTime(int nMotionId, float fStopTime);

	unsigned long GetSliceIo(int nSegment);
	unsigned long GetSliceOut(int nSegment);

	BOOL SetSliceIo(int nSegment, int nBit, BOOL bOn);
	BOOL SetSliceIo(int nSegment, unsigned long ulOut);
	BOOL IsOutSliceIo(int nSegment, int nBit);

	BOOL CheckScanEnd();
	BOOL WaitHomeEndFocus(DWORD dwTimeOver = TIME_OUT_MS);
	BOOL InitServoMotor();
	BOOL InitServoMotorNoDestroy();
	BOOL StopMove(int nAxisId, double fStopTime, BOOL bWait = NO_WAIT); // 지정된축을 정지시킨다.
	double CheckThetaLimit(double fAdjAngle);
	BOOL IsLimitError(int nMotorId);
	BOOL IsNegSWLimit(int nMotorId);
	BOOL IsPosSWLimit(int nMotorId);
	long CheckError(int nMotorId, long errNo);

	// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Write Trace Data, 6. Limit Change Value of Interval Focus Pos [mm]
	BOOL InitInspectCamAutoFocusingWithLaserSensor(double dScanVel, double dSampleInterval, double dOffsetInspCamLaser, double dOrgFocusPos, BOOL bWriteTrace = FALSE, double dErrRange = 1.0); 	//20121125-ndy for 2Head Laser Control
																																																	// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Limit Change Value of Interval Focus Pos [mm]
	BOOL InitInspectCamAutoFocusingWithLaserSensor2(double dScanVel, double dSampleInterval, double dOffsetInspCamSol, double dOrgFocusPos, double dErrRange = 2.0, BOOL bBiDir = FALSE); //20150507 - syd

	BOOL SearchThetaReadyPosOfGm();
	int _SearchThetaReadyPosOfGm();
	BOOL GetThetaLimitInputForGm(BOOL bDir);
	BOOL ThetaOriginForGm();
	int GetThetaStrokeForGm(double fPlusMargin, double fMinusMargin);
	BOOL GetOrgInput(int nMotorId);
	int GetAxisStroke(int nMotorId, double fPlusMargin = 0.0, double fMinusMargin = 0.0);	// search and set software limit position 

	BOOL LoadErrMapFile(CString sPath);
	BOOL AdjustErrMapData(double dStdPosX, double dStdPosY);
	BOOL SetErrMap(CMyFileErrMap *pMyFile = NULL);
	BOOL ResetErrMap();
	double GetThetaAngle(double dPos);
	BOOL IsHomming(int nMotor);
	BOOL CheckThetaAxisRange(float fAdjAngle, float fMaxAngle);
	BOOL SetTriggerStartPos(double fStartPos);
	BOOL HoldingMove(int nEventAxis, double dEventPos, int nHoldAxis, double dTagetPos, int nDir);
	BOOL HoldingMoveEx(int nEventAxis, double dEventPos, int nHoldAxis, double dTagetPos, int nDir); //160305 lgh add
	BOOL HoldingStartScan(double fHoldingPosX, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir = FORWARD, long nStartDummy = 1500, long nEndDummy = 1500, double fCurrThicknessByLaserSensor = -100.0);
	BOOL ExchangeVelMove(int nMotor, double fPos, double fVel, double fAcc, double fDec, double fEventPos, double fExchangeVel, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = MotionTypeS_CURVE);
	BOOL StartHoldingMove(int nHoldAxis, int nEventAxis, double dEventPos, double fPos, int nDir, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = MotionTypeS_CURVE);
	BOOL StartHoldingMoveEx(int nHoldAxis, int nEventAxis, double dEventPos, double fPos, int nDir, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = MotionTypeS_CURVE);//160305 lgh add

	int WaitScanAxisEnd(DWORD dwTimeOver = TIME_OUT_MS, BOOL bPump = 1);//160213 lgh mod bool -> int

	BOOL MoveRevolverMotor(double fTgtPos, BOOL bAbs = ABS, BOOL bWait = WAIT);

	BOOL CheckMotionConfig(); // 20180417-syd

	BOOL IsMotionDoneGantrySlave();
	BOOL IsListMotion();
	BOOL IsInterpolationMotion();
	void AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed);
	void RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos);
	void StartListMotion();
	void StopListMotion();
	void LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir = FORWARD, CString sPath = _T("C:\\AoiWork\\AddList.txt"));

	void EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable); // nSensorIndex : 0 ~ 4 , bEnable

#if TRIGGER_TYPE == CONVEX_TRIGGER || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
							  //  Convex Driver =========================================================================
	BOOL m_bEnabelConvexDriveTrig[MAX_AXIS];

	BOOL IsEnableConvexDriveTrig(int nAxisId);
	long SetConvexDriveTrigMode(int nAxisID);
	long GetConvexDriveTrigEnable(int nAxisID, BOOL* bEnable);
	long EnableConvexDriveTrig(int nAxisID); //Enable 트리거
	long DisableConvexDriveTrig(int nAxisID); //Disable 트리거
	long SetConvexDriveTrigOriginPosition(int nAxisID, double fPos); //드라이틒E트리거 원점 설정, 원점복귀 후 원점에 큱E?위치값으로 설정해야함. 단위 : [mm]
	long SetConvexDriveTrigPulseWidth(int nAxisID, double fPulseWidth);	//드라이틒E트리거 펄스 ?E설정, 단위 : [mSec] 
	long SetConvexDriveTrigStartPosition(int nAxisID, double fPos); //트리거 시작 위치. 단위 : [mm]
	long SetConvexDriveTrigEndPosition(int nAxisID, double fPos); // 트리거 끝 위치 (단, 끝 위치가 시작 위치보다 커야 함.) 단위 : [mm] 
	long SetConvexDriveTrigSpace(int nAxisID, double fTrigSpace); // 트리거 간격. (“끝 위치 - 시작 위치” 값이 간격의 배수이엉雹 함.) 단위 : [mm] 
	long SetConvexDriveTrigOneshot(int nAxisID);
	long SetConvexDriveTrigDirection(int nAxisID, long dir);

#endif

	// Display Motor position to Live Image by overlay method ,khc
	CWinThread* m_pMonitorThread;
	HANDLE		m_hMonitorThread;
	CEvent      m_evtMonitorThread;
	static UINT MonitorThread(LPVOID pParam);
	void StartMonitorThread();
	void StopMonitorThread();
	void WaitUntilMonitorThreadEnd(HANDLE hThread);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionNew)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMotionNew();

	// Generated message map functions

	double CalSpeedProfile(int nAxisID, double fLength, double &fVel, double &fAcc, double &fDec);

protected:
	//{{AFX_MSG(CMotionNew)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


#endif	// #ifdef USE_NMC

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONNEW_H__A8D61E2C_EE72_4D14_8452_9B10B0D2E36D__INCLUDED_)