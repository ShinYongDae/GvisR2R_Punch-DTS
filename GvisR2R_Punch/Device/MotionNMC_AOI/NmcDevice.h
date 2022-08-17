#if !defined(AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_)
#define AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NmcDevice.h : header file
//



#include "NMC_Util.h"
#include "NmcAxis.h"
#include "BufferControl.h"
#include "nmc_appmotion.h"



class CNmcDevice : public CMotionControlAdapter
{
public:
	int RestoreSwEmergency();
	CBufferControl* m_pBufferControl; // for Listed Motion...= (RTAF)

	BOOL m_OutSegInfo[10][32];
	BOOL m_OutSegInfoOn[10][32];

	double m_dVel[MAX_AXIS];
	double m_dAcc[MAX_AXIS];
	double m_dDec[MAX_AXIS];
	double m_dJerk[MAX_AXIS];
	double m_fAccPeriod[MAX_AXIS]; // [%]
	double  m_fPosLimit[MAX_AXIS];			// Positive Software Limit
	double  m_fNegLimit[MAX_AXIS];			// Negative Software Limit
	double	m_fMaxAccel[MAX_AXIS];			// Maximum Acceleration in G

	//	CNmcAxis* m_pAxis[8]; // Number of Axis : 0 ~ 7
	CNmcAxis* m_pAxis[MAX_AXIS]; // Number of Axis : 0 ~ 3 20110920 hyk mod
	DWORD m_dwDeviceIoOut;

	CString m_strErrMsg;
	int m_nTotalAxisNum;


	int m_nTriggerOrgPos;
	BOOL m_bListMotion;
	BOOL m_bInterpolationMotion;

	BOOL m_bErrMap;
	BOOL m_bAppliedEC, m_bLoadedEC;

	int ToInt32(UINT8* Data);

	CString ToCString(I16 data);
	CString ToCStringH(I16 data);
	CString ToCString(I32 data);
	CString ToCStringH(U16 data);


	BOOL InitAxisParam(int nAxis);
	BOOL ResetAxesGroup();

public:
// Construction

	CNmcDevice(CMotion* pMotion);

// Attributes
	BOOL Recovery();
	void ClearSWEmg();

	virtual BOOL NMC_CheckNmcConnection()
	{
		return CheckNmcConnection();
	}

	BOOL CheckNmcConnection();

	void NMC_GetTriggetMode(int nEcatAddr, int vAxisID, int& mode);
	virtual void NMC_TriggerOneShot(int nEcatAddr, int vAxisId, double dWidth);

	virtual BOOL Abort(int nMotionId);
	virtual BOOL EStop(int nMsId);
	virtual void ResetStepper(int nMsId);
	virtual BOOL Clear(int nMsId);
	virtual BOOL IsStop(int nMsId);
	virtual BOOL Stop(int nMotionId);
	virtual void SetStopTime(int nMotionId, float fStopTime);
	virtual BOOL IsMotionDone(int nMotionId);
	virtual BOOL IsInPosition(int nAxisId);
	virtual double GetActualPosition(int nAxisId);
	virtual double GetCommandPosition(int nAxisId);
	virtual int GetAxisState(int nAxisId);
	virtual BOOL SetActualPosition(int nAxisId, double fData);
	virtual BOOL SetCommandPosition(int nAxisId, double fData);
	virtual void SetErrorLimitAction(int nMotorID, int Action);
	virtual BOOL SetSettleTime(int nAxisId, float fTime);
	virtual double GetPosSWLimitValue(int nAxisId);
	virtual double GetNegSWLimitValue(int nAxisId);
	virtual BOOL ServoOn(int nMotorId, BOOL bOn);
	virtual BOOL GetOrgInput(int nMotorId);
	virtual BOOL IsLimit(int nMotionId, int nDir);
	virtual void ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue);
	virtual void ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue);
	virtual BOOL IsAmpFault(int nMotorId);
	virtual double GetPositionResolution(int nMotorId);
	virtual int GetSeqIdx() { return 0; }
	virtual void AddSeqIdx(int nAdd = 1){}
	virtual void ResetSeqIdx(){}
	virtual void SetCleanDoorFaultLimit();
	virtual BOOL SetHomeAction(int nMsIdx, int Action);
	virtual void ResetHomeAction(int nMsIdx);
	virtual void SetNegSoftwareLimit(int nMotorId, double fLimitVal, int nAction);
	virtual void SetPosSoftwareLimit(int nMotorId, double fLimitVal, int nAction);
	virtual	double Len2Pulse(int nIdxMotor, double fLength);
	virtual double Pulse2Len(int nAxisId, long nPulse);
	virtual BOOL NMC_StartHomeThread(int nAxisId);
	virtual BOOL NMC_StartHomming(int nAxisId);
	virtual void SetOriginStatus(int nAxisId, BOOL bStatus = FALSE);
	virtual BOOL GetOriginStatus(int nAxisId);
	virtual void SetNegSWLimitAction(int nMotorId, int Action);
	virtual void SetPosSWLimitAction(int nMotorId, int Action);
	virtual BOOL IsServoOn(int nMotorID);
	virtual void SetEStopTime(int nMotionId, float fEStopTime);
	virtual BOOL SetMsAction(long lMS, int action);
	virtual void SetNegSWLimitDirection(int nMotorId, BOOL bDirection);
	virtual void SetPosSWLimitDirection(int nMotorId, BOOL bDirection);
	virtual BOOL InitRecorder();
	virtual BOOL GetRecordLastData(double &dScanPos, double &dThickPos, int bDir = FORWARD);
	virtual BOOL IsNegSWLimit(int nMotorId);
	virtual BOOL IsPosSWLimit(int nMotorId);
	virtual BOOL IsHomeLimit(int nMotorId);
	virtual BOOL SetErrMap(CMyFileErrMap *pMyFile = NULL);
	virtual BOOL ResetErrMap();
	virtual BOOL AdjustErrMapData(double dAdjVal);
	virtual BOOL LoadErrMapFile(CString sPath);
	virtual BOOL SetConfigureAxis(int nAxisCount);
	virtual long CreateAutoFocusingSeq3_31_RTAF(structRTAFInfo* pPointArr, double dRefHeightAtTestPoint, int nTotal, int nDir);
	virtual BOOL NMC_CreateObject(int nTotalAxis);

	virtual long CreateAutoFocusingSeq(BOOL bFirst, double fScanMotPos, double dFocusOffset, int bDir, double fHeightOfLaserSensor, int nSwath, int nHeightIndex);

	virtual MPIMotion MPI_MotionPtr(int Axis);
	virtual MPISequence MPI_SequencePtr(int nSeqid);
	virtual int GetInterlockStatus()
	{
		return m_nInterlockStatus;
	}


	virtual int NMC_In32(unsigned long *value);
	virtual int NMC_Out32(long value);

	virtual BOOL NMC_TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)
	{
		return TriggerSetRange( nEcatAddr,  vAxisId,  dStPos,  dEdPos,  dPulseWd,  dTrigRes);
	}
	virtual BOOL NMC_TriggerStop(int nEcatAddr, int vAxisId)
	{
		return TriggerStop(nEcatAddr, vAxisId);
	}

	virtual BOOL NMC_TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx)
	{
		return TriggerSetOriginPos(nEcatAddr, vAxisId, nSdoIdx);
	}
	virtual CString NMC_GetTriggerEncCnt(int nEcatAddr, int nSdoIdx)
	{
		return GetTriggerEncCnt(nEcatAddr, nSdoIdx);
	}
	virtual int NMC_GetTriggerEnc(int nEcatAddr, int nSdoIdx)
	{
		return GetTriggerEnc(nEcatAddr, nSdoIdx);
	}

	virtual void NMC_EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable)
	{
		NMC_EnableSensorStop(nAxisId, nSensorIndex, bEnable);
	}


	virtual BOOL NMC_GantryEnable(long lMaster, long lSlave, long lOnOff)
	{
		return GantryEnable( lMaster,  lSlave,  lOnOff);
	}

	virtual BOOL NMC_StartGantrySlaveMove(int nAxisId, BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk = 0.0, byte nDispMode = 3)
	{
		return StartGantrySlaveMove( nAxisId,  bAbs,  fPos,  fVel,  fAcc,  fDec,  fJerk ,  nDispMode);
	}

	virtual void NMC_RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos)
	{
		return RestoreStartListMotionPos( dStartScanPos,  dStartFocusPos);
	}

	virtual void NMC_StartListMotion()
	{
		StartListMotion();
	}

	virtual void NMC_StopListMotion()
	{
		StopListMotion();
	}

	virtual void NMC_LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir = FORWARD, CString sPath = _T("C:\\AoiWork\\AddList.txt"))
	{
		LoggingAddList(dStartScanPos, dStartFocusPos, fScanVel, nScanDir, sPath);
	}

	virtual BOOL NMC_IsListMotion()
	{
		return IsListMotion();
	}

	virtual BOOL NMC_IsInterpolationMotion()
	{
		return IsInterpolationMotion();
	}


	virtual void NMC_AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed)
	{
		//AddList( fScanMotPos,  dFocusMotPos,  dScanSpeed);//20211001-syd
		AddList(fScanMotPos, dFocusMotPos);
	}

	virtual int NMC_GetAddListNum();

	virtual BOOL NMC_InitListMotion()
	{
		return InitListMotion();
	}

	virtual BOOL GetRecords()
	{
		return 0;
	}

	virtual BOOL InitRecords()
	{
		return 0;
	}

	virtual BOOL CreateBackwardSequenceDataForUniHeader(double dOrgFocus);

	virtual void NMC_UnGroup2Ax(int nBdNum, int nGroupNum)
	{
		UnGroup2Ax(nBdNum, nGroupNum);
	}

	virtual int NMC_UnGroup2Ax(int nGroupNum)
	{
		return UnGroup2Ax(nGroupNum);
	}

	virtual BOOL IsEncoderFault(int nAxisId)
	{	
		return 0;//
	}

	virtual double NMC_GetStartListMotionPos(int nIndex)
	{
		return GetStartListMotionPos(nIndex);
	}

	virtual void ResetAutoFocusingSeq()
	{
		StopListMotion();
	}

	virtual void NMC_OutBit(long bit, bool flag)
	{
		OutBit(bit, flag);
	}

	virtual long NMC_ReadOut()
	{
		return ReadOut();
	}

	virtual BOOL NMC_ReadIn(long bit)
	{
		return ReadIn(bit);
	}

	virtual BOOL NMC_ReadOut(long bit)
	{
		return ReadOut(bit);
	}

	virtual BOOL SetSliceIo(int nSegment, int nBit, BOOL bOn);
	virtual BOOL SetSliceIo(int nSegment, unsigned long ulOut);
	virtual BOOL IsOutSliceIo(int nSegment, int nBit);


	virtual BOOL IsLimitError(int nMotorId)
	{
		return 0;
	}

	virtual float GetEStopTime(int nMotionId)
	{
		return 0.05;
	}

	virtual unsigned long GetSliceIo(int nSegment);

	virtual unsigned long GetSliceOut(int nSegment);

	virtual void SetPosHWLimitDirection(int nMotorId, BOOL bDirection)
	{

	}

	virtual void SetNegHWLimitDirection(int nMotorId, BOOL bDirection)
	{

	}

	virtual void SetPosHWLimitAction(int nMotorId, int Action)
	{
		if (nMotorId >= MAX_AXIS)
			return;

		return (m_pAxis[nMotorId]->SetPosHWLimitAction(Action));
	}

	virtual void SetNegHWLimitAction(int nMotorId, int Action)
	{
		if (nMotorId >= MAX_AXIS)
			return;

		return (m_pAxis[nMotorId]->SetNegHWLimitAction(Action));
	}

	virtual void SetPosSWLimitValue(int nMotorId, double fErrorLimitValue)
	{		
		SetPosSoftwareLimit(nMotorId, fErrorLimitValue, E_STOP_EVENT);
	}

	virtual void SetNegSWLimitValue(int nMotorId, double fErrorLimitValue)
	{
		SetNegSoftwareLimit(nMotorId, fErrorLimitValue, E_STOP_EVENT);
	}

	virtual BOOL CheckMotionConfig()// 20180417-syd
	{
		return 1;
	}

	virtual long GetState(int nMotionId)
	{
		if (nMotionId >= MAX_AXIS)
			return 0;

		return (m_pAxis[nMotionId]->GetState());
	}

	virtual long StartRecorder()
	{
		return 1;
	}

	virtual long StopRecorder()
	{
		return 1;
	}

	virtual BOOL SearchHomePos(int nMotionId, BOOL bThread = TRUE);

	BOOL NMC_TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = S_CURVE)
	{
		return TwoStartPosMove(nMsId0, nMsId1, fPos0, fPos1, fVelRatio, bAbs, bWait, bOptimize, bMotionType);
	}

	BOOL NMC_TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED)
	{
		return TwoStartPosMove( nMsId0,  nMsId1,  fPos0,  fPos1,  fVel,  fAcc,  fDec,  bAbs ,  bWait,  bMotionType,  bOptimize );
	}

	virtual BOOL NMC_StartPtPMove(int nAxis, double fPos, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait)
	{
		if (nAxis >= MAX_AXIS)
			return 0;

		return (m_pAxis[nAxis]->StartPtPMove(  fPos,  fVel,  fAcc,  fDec,  bAbs,  bWait));
	}
	virtual BOOL NMC_StartPtPMotion(int nAxis, double fPos, double fVel, double fAcc, double fDec, BOOL bAbs = TRUE, BOOL bWait = TRUE)
	{
		if (nAxis >= MAX_AXIS)
			return 0;

		return (m_pAxis[nAxis]->StartPtPMotion(  fPos,  fVel,  fAcc,  fDec,  bAbs ,  bWait ));
	}
	virtual BOOL NMC_StartSCurveMove(int nAxis, double fPos, double fVel, double fAcc, double fJerk, BOOL bAbs = TRUE, BOOL bWait = TRUE)
	{
		if (nAxis >= MAX_AXIS)
			return 0;

		return (m_pAxis[nAxis]->StartSCurveMove(   fPos,  fVel,  fAcc,  fJerk,  bAbs  ,  bWait ));
	}
	virtual BOOL NMC_StartSCurveMove(int nAxis, double fPos, double fVelRatio, BOOL bAbs = TRUE, BOOL bWait = TRUE)
	{
		if (nAxis >= MAX_AXIS)
			return 0;

		return (m_pAxis[nAxis]->StartSCurveMove(  fPos,  fVelRatio,  bAbs ,  bWait));
	}

	virtual int NMC_StartRsaHoming(int nAxisId, BOOL bWait = TRUE, int nMode = HOMING_LIMIT_SWITCH, int nDir = HOMING_DIR_CCW)
	{
		return StartRsaHoming(nAxisId, bWait,  nMode ,  nDir);
	}


	BOOL m_bSetTriggerConf[8];  // [TriggerIndex]

	BOOL InitDevice(int nDevice);
	BOOL CreateAxis(int nAxis);

	virtual void SetParam();
	CNmcAxis* GetAxis(int nAxis);

	int In32(unsigned long *value);
	int Out32(long value);
	virtual BOOL VMove(int nMotionId, int nDir);
	void SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID);
	void OutBit(long bit, bool flag);
	long ReadOut();
	BOOL ReadIn(long bit);
	BOOL ReadOut(long bit);
	BOOL GantryEnable(long lMaster, long lSlave, long lOnOff);
	virtual BOOL GetGantry(long lMaster, long lSlave, long *lOnOff);
	BOOL StartGantrySlaveMove(int nAxisId, BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk=0.0, byte nDispMode=3);

	double m_dPrevPos1; //20211001-syd
	BOOL TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
	BOOL TriggerStop(int nEcatAddr, int vAxisId);
	BOOL TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx);
	CString GetTriggerEncCnt(int nEcatAddr, int nSdoIdx);
	int GetTriggerEnc(int nEcatAddr, int nSdoIdx);

	virtual int NMC_GetTriggerCounter(int nEcatAddr, int nSdoIdx);


	void UnGroup2Ax(int nBdNum, int nGroupNum);
	int UnGroup2Ax(int nGroupNum);



	virtual void StopSequence();
	int Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY);
	bool BufferInit();
	BOOL IsGroupStatusStandby();
	BOOL IsListMotion();
	BOOL IsInterpolationMotion();
	int GetAddListNum();



	double CalSpeedProfile(int nAxisID, double fLength, double &fVel, double &fAcc, double &fDec);
	
	void EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable); // nSensorIndex : 0 ~ 4 , bEnable
	BOOL LoadErrorCompensationTable();
	BOOL LoadErrorCompensationTable(int eType, int eMapId, char* ePath); // nType: 0 [1D (Axis)] , 1 [2D (Gantry)] , 2 [3D (Group)] ; eMapId : Range[ 0 ~ 3 (Table1 ~ Table4) ]
	BOOL ApplyErrorCompensateionTable(int nAxisID, BOOL bEnable);





	CString GetErrorMsgEC(MC_STATUS nErrCode);
	int GetErrorCompensationStatus();
	int GetErrorCompensationType();
	BOOL InitErrMap();
	BOOL TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio = 100.0, BOOL bAbs = TRUE, BOOL bWait = TRUE, BOOL bOptimize = OPTIMIZED, int bMotionType = S_CURVE);
	BOOL TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs = ABS, BOOL bWait = WAIT, int bMotionType = S_CURVE, BOOL bOptimize = OPTIMIZED);
	int DisableGroup2Ax(int nGroupNum);

	int StartRsaHoming(int nAxisId, BOOL bWait=TRUE, int nMode=HOMING_LIMIT_SWITCH, int nDir=HOMING_DIR_CCW);
	virtual BOOL SetGantry(long lMaster, long lSlave, long lOnOff);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNmcDevice)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNmcDevice();

	CMotion* m_pParent;

public:

	BOOL m_bExit;






	BOOL DestroyDevice();


	//161012 lgh for motion optimize.. 속도 비율은 없다
#ifdef NO_ACCEPTABLE_VELRATIO_MOVE
	virtual BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait);
	virtual BOOL Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait);
	virtual BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait, BOOL bOptimize);
#else
	BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs = ABS, BOOL bWait = NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs = ABS, BOOL bWait = NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs = ABS, BOOL bWait = WAIT, BOOL bOptimize = OPTIMIZED);
#endif
	BOOL InitListMotion();
	void StartListMotion();
	void StopListMotion();
	//void AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed); //20211001-syd
	void AddList(double fScanMotPos, double dFocusMotPos);  //20211001-syd
	void LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir=FORWARD, CString sPath=_T("C:\\AoiWork\\AddList.txt"));
	void ShutdownHomeThreadAll();
	virtual double GetActualVelocity(int nAxisId);
	void RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos);
	double GetStartListMotionPos(int nIndex);  // 0: ScanAxis, 1:FocusAxis

	virtual double SetAutoFocusSmoothing(double dFocusOffset);	// 20151201 - syd
	virtual void ClearAutoFocusSmoothingData();	// 20151201 - syd

	virtual void ClearUserBuffer();
	virtual BOOL SetVMove(int nAxisID, double fVel, double fAcc);

	// Generated message map functions
protected:
	//{{AFX_MSG(CNmcDevice)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()

};

extern CNmcDevice* g_pNMCDevice;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMCDEVICE_H__725C673E_67CF_4B8E_8055_3F8B73413051__INCLUDED_)