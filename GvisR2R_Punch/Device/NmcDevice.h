#if !defined(AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_)
#define AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NmcDevice.h : header file
//
#include "NMC_R2R.h"

#ifdef USE_NMC

#ifdef _WIN64
#include "../Device/NMC_Motion.h"

#pragma comment (lib, "lib/NMC_Motion.lib")	//#pragma comment (lib, "../lib/NMC_Motion.lib")
#pragma comment (lib, "lib/NMC_APP_Motion.lib")
//#pragma comment (lib, "lib/NMC_Comm.lib")
//#pragma comment (lib, "lib/NMC_Driver.lib")
#else
#include "Device/MMCE/x86/NMC_Motion.h"
//#pragma comment (lib, "Device/NMC/x86/NMC_Comm.lib")
//#pragma comment (lib, "Device/NMC/x86/NMC_Driver.lib")
#pragma comment (lib, "Device/NMC/x86/NMC_Motion.lib")
#endif



#include "NmcAxis.h"
#include "BufferControl.h"

#include "FileOperation.h"


class CNmcDevice : public CWnd
{
	UINT16	m_nBoardId;
	UINT16	m_nDevIdIoIn;
	UINT16	m_nDevIdIoOut;
	UINT16	m_nOffsetIoOption;
	INT		m_nOffsetAxisID;
	//int		m_nGroupID_RTAF;
	//int		m_nGroupID_ErrMapXY;
	int		m_nGroupID_Interpolation[2];

	CBufferControl* m_pBufferControl; // for Listed Motion...= (RTAF)

	double* m_dVel;
	double* m_dAcc;
	double* m_dDec;
	double* m_dJerk;
	double* m_fAccPeriod;			// [%]
	double*  m_fPosLimit;			// Positive Software Limit
	double*  m_fNegLimit;			// Negative Software Limit
	double*	m_fMaxAccel;			// Maximum Acceleration in G

	//	CNmcAxis* m_pAxis[8]; // Number of Axis : 0 ~ 7
	CNmcAxis ** m_pAxis; // Number of Axis : 0 ~ 3 20110920 hyk mod
	DWORD m_dwDeviceIoOut;

	CString m_strErrMsg;

	double m_dUserBuffer[2048];
	int m_nTriggerOrgPos;
	BOOL m_bListMotion;
	BOOL m_bInterpolationMotion[2];

	BOOL m_bErrMap;
	BOOL m_bAppliedEC, m_bLoadedEC;

	int ToInt32(UINT8* Data);

	CString ToCString(I16 data);
	CString ToCStringH(I16 data);
	CString ToCString(I32 data);
	CString ToCStringH(U16 data);


	BOOL InitAxisParam(int nAxis);
	BOOL ResetAxesGroup();
	int m_nTotalMotion;
	int m_nTotalAxis;

	void SaveLog(CString strMsg);

	CWindowVersion m_cWindowVersionCollect;
	tpOs GetWindowVersion();

public:
// Construction

	CNmcDevice(CWnd* pParent=NULL);

// Attributes
public: 
	CCriticalSection m_csLogLock;

	int m_nRTAFSeqIndex;
	//structRTAFInfo* m_pRTAFPointArr;
	int m_nInterlockType;
	int m_nMotError;
	int m_nInterlockStatus;
	int m_nRTAFUniSeqLastIndex;
	BOOL* m_bOrigin;
	CString m_sMotionError;
	double*	m_fMaxVel;
	double m_dStartScanPos, m_dStartFocusPos;

	BOOL m_bUseGantry;
	long m_lGantryMaster;
	long m_lGantrylSlave;
	long m_lGantryEnable;
	BOOL m_bGantryEnabled;


	BOOL InitDevice(int nDevice);
	BOOL CreateAxis(int nAxis);
	//unsigned long GetSliceOut(int nSegment);
	void SetParam();
	CNmcAxis* GetAxis(int nAxis);
	double GetAxisState(int nAxisId);
	int In32(long *value);
	int Out32(long value);
	BOOL VMove(int nMotionId, int nDir);
	void SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID);
	void OutBit(long bit, bool flag);
	long ReadOut();
	BOOL ReadIn(long bit);
	BOOL ReadOut(long bit);
	BOOL GantryEnable(long lMaster, long lSlave, long lOnOff);
	BOOL GetGantry(long lMaster, long lSlave, long *lOnOff);
	BOOL StartGantrySlaveMove(int nAxisId, BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk=0.0, byte nDispMode=3);

	BOOL TriggerSetRange(int encAxisId, int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
	BOOL TriggerStop(int nEcatAddr, int vAxisId);
	BOOL TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx);
	CString GetTriggerEncCnt(int nEcatAddr, int nSdoIdx);
	int GetTriggerEnc(int nEcatAddr, int nSdoIdx);

	//void StopSequence();
	int Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY);
	bool BufferInit();
	BOOL IsGroupStatusStandby();
	BOOL IsListMotion();
	int GetAddListNum();

	double Len2Pulse(int nIdxMotor, double fLength);
	double Pulse2Len(int nAxisId, long nPulse);
	double CalSpeedProfile(int nAxisID, double fLength, double &fVel, double &fAcc, double &fDec);
	double GetActualPosition(int nAxisId);

	void EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable); // nSensorIndex : 0 ~ 4 , bEnable
	/*
	BOOL LoadErrorCompensationTable();
	BOOL LoadErrorCompensationTable(int eType, int eMapId, char* ePath); // nType: 0 [1D (Axis)] , 1 [2D (Gantry)] , 2 [3D (Group)] ; eMapId : Range[ 0 ~ 3 (Table1 ~ Table4) ]
	BOOL ApplyErrorCompensateionTable(int nAxisID, BOOL bEnable);
	CString GetErrorMsgEC(MC_STATUS nErrCode);
	int GetErrorCompensationStatus();
	int GetErrorCompensationType();
	BOOL InitErrMap();
	*/
	BOOL IsInterpolationMotion(int nId);
	int UnGroup2Ax(int nGroupNum);
	void UnGroup2Ax(int nBdNum, int nGroupNum);

	BOOL TwoStartPosMove0(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	BOOL TwoStartPosMove1(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	BOOL TwoStartPosMove0(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	BOOL TwoStartPosMove1(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	int DisableGroup2Ax(int nGroupNum);

	int StartRsaHoming(int nAxisId, BOOL bWait=TRUE, int nMode=HOMING_LIMIT_SWITCH, int nDir=HOMING_DIR_CCW);
	BOOL SetGantry(long lMaster, long lSlave, long lOnOff);

	double GetSpeedProfile(int nMode, int nAxisID, double fMovingLength, double &fVel, double &fAcc, double &fJerk, int nSpeedType = HIGH_SPEED);
	double GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk, double dMinJerkTime);
	double GetMotionTime(double dLen, double dVel, double dAcc, double dJerk);

	int RestoreSwEmergency();	// -1: Fault , 1: Emergency Signal Off complete, 2: Previous Emergency Signal Off-state, 3: Normal

	BOOL SearchHomePos(int nMotionId, BOOL bThread = TRUE);
	BOOL IsHome(int nMotionId);
	virtual void SetOriginStatus(int nAxisId, BOOL bStatus = FALSE);
	virtual BOOL NMC_StartHomeThread(int nAxisId);
	virtual BOOL NMC_StartHomming(int nAxisId);


	BOOL Stop(int nMotionId);
	BOOL EStop(int nMotionId);
	BOOL Clear(int nMotionId);
	BOOL VMoveStop(int nMotionId, int nDir);
	BOOL IsLimit(int nMotionId, int nDir);
	BOOL Abort(int nMotionId);
	long GetState(int nMotionId);

	BOOL CheckNmcConnection();// TRUE: OK , FALSE: Error

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNmcDevice)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNmcDevice();

private:
	CWnd* m_pParent;

public:
	MotionControl* m_pParamCtrl;
	MotionMotion* m_pParamMotion;
	MotionAxis* m_pParamAxis;
	MotionMotor* m_pParamMotor;

	BOOL m_bExit;

	double m_dTempOffsetAf[10]; // 20151201 - syd
	int	m_nSeqIdx;
	int m_nCaptureCount;
	BOOL m_bWriteTrace, m_bStartAfSeq;
	double m_dCmdTime;
	double m_dSampleInterval;
	double m_dErrRange;
	double m_dOffsetInspCamSol;
	BOOL m_bLogAddList;
	CString m_sAddListLogTitle;



	BOOL DestroyDevice();


	//161012 lgh for motion optimize.. 속도 비율은 없다
#ifdef NO_ACCEPTABLE_VELRATIO_MOVE
	BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait);
	BOOL Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait);
	BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait, BOOL bOptimize);
#else
	BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs = ABS, BOOL bWait = NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs = ABS, BOOL bWait = NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs = ABS, BOOL bWait = WAIT, BOOL bOptimize = OPTIMIZED);
#endif
	BOOL Move(int nMotionId, double dTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs = ABS, BOOL bWait = WAIT, BOOL bOptimize = OPTIMIZED);

	//BOOL InitListMotion();
	//void StartListMotion();
	//void StopListMotion();
	//void AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed);
	//void LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir=FORWARD, CString sPath=_T("C:\\AoiWork\\AddList.txt"));
	//void ShutdownHomeThreadAll();
	double GetActualVelocity(int nAxisId);
	//void RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos);
	//double GetStartListMotionPos(int nIndex);  // 0: ScanAxis, 1:FocusAxis

	//double SetAutoFocusSmoothing(double dFocusOffset);	// 20151201 - syd
	//void ClearAutoFocusSmoothingData();	// 20151201 - syd

	//void ClearUserBuffer();
	BOOL SetVMove(int nAxisID, double fVel, double fAcc);


	BOOL IsHomeDone();
	BOOL IsHomeDone(int nMotionId);
	BOOL IsHomeSuccess();
	BOOL IsHomeSuccess(int nMotionId);

	BOOL IsMotionDone(int nMotionId);
	BOOL IsInPosition(int nMotionId);
	BOOL IsEnable(int nMsId);
	BOOL IsServoOn(int nMotorID);


	// Generated message map functions
protected:
	//{{AFX_MSG(CNmcDevice)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};





#endif	// #ifdef USE_NMC
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_)