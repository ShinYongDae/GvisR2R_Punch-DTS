#if !defined(AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_)
#define AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Motion.h : header file
//



#include "MotionParam.h"


#ifdef USE_ZMP
//#include "ZmpControl.h"
#include "MotionOld.h"
#else
#ifdef USE_NMC
#include "NmcDevice.h"
#include "MotionNew.h"
#endif
#endif




#define TIM_TOOL_PATH	10
#define	TIME_OUT_MS				300000	// 5 minute


/////////////////////////////////////////////////////////////////////////////
// CMotion window
class CMotion 
{
// Construction
public:
#ifdef USE_ZMP
	CMotionOld* m_pMotionOld;
#else
#ifdef USE_NMC
	CMotionNew* m_pMotionNew;
#endif
#endif

	CMotion(CWnd* pParent = NULL, CString strFilePath = _T(""));


// Attributes
public:
	int GetTotAxis();
	MotionControl GetParamParamCtrl();
	MotionMotion* GetParamMotion();
	MotionAxis* GetParamAxis();
	MotionMotor* GetParamMotor();
	BOOL *GetDO();

	CString GetMotionParamFilepath();

// Operations
public:
#ifdef USE_ZMP
	CZmpControl *GetMotionControl();
#else
#ifdef USE_NMC
	CNmcDevice *GetMotionControl();
#endif
#endif

	BOOL *GetHomeActionAbort();

	void JogUp(int nMsId,int nDir);
	void JogDn(int nMsId,int nDir);
	void SetJogSpd(int nSpd = 2);
	long SetNotifyFlush();
	float GetEStopTime(int nMotionId);
	void SetEStopTime(int nMotionId, float fEStopTime);
	void ResetEStopTime(int nMotionId);


	
	BOOL Abort(int nMotionId);
	long SetInposition(int nAxis, double fInpRange);
	double GetInposition(int nAxis);
	long GetState(int nMotionId);
	BOOL ObjectMapping();
	BOOL InitXmpBoard();
	BOOL SearchHome();
	BOOL SearchHomePos(int nMotionId, BOOL bThread=TRUE);
	BOOL IsHomeDone();
	BOOL IsHomeDone(int nMotionId);
	BOOL IsHomeSuccess();
	BOOL IsHomeSuccess(int nMotionId);
	BOOL IsMotionDone(int nMotionId);
	BOOL IsInPosition(int nMotionId);
	BOOL ServoOnOff(int nAxis, BOOL bOn);
	BOOL AmpReset(int nAxisId);
	BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs=ABS, BOOL bWait=NO_WAIT);
	BOOL MoveRatio(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs=ABS, BOOL bWait=NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs = ABS, BOOL bWait = WAIT);


	double LengthToPulse(int nIdxMotor, double fLength);
	double GetActualPosition(int nAxisId);
	long GetAxisState(int nAxisId);
	double GetVelocity(int nAxisId);
	double GetAxisVel(int nAxisId);
	BOOL GetAxisAmpIsEnable(int nAxisId);
	BOOL SetCommandPos(int nAxisId, double fPos);
	BOOL SetActualPosition(int nAxisId, double fPos);

	BOOL Stop(int nMotionId);
	BOOL EStop(int nMotionId);
	BOOL Clear(int nMotionId);
	BOOL IsEnable(int nMotionId);
	BOOL SetVMove(int nMotionId, double fVel, double fAcc);
	BOOL VMove(int nMotionId, int nDir=1);


	BOOL ReadBit(BYTE cBit, BOOL bInput=TRUE);
	unsigned long ReadAllBit(BOOL bInput);
	void WriteBit(BYTE cBit, BOOL bOn);
	void WriteData(long lData);







	BOOL VMoveStop(int nMotionId, int nDir);
	BOOL ChkLimit(int nMotionId, int nDir);


	double GetPosSWLimitValue(int nAxisId);
	double GetNegSWLimitValue(int nAxisId);
	double GetCommandPosition(int nAxisId);
	BOOL WaitMotionDone(int nAxisId);

	BOOL IsNegSWLimit(int nMotorId);
	BOOL IsPosSWLimit(int nMotorId);
	BOOL IsNegHWLimit(int nMotorId);
	BOOL IsPosHWLimit(int nMotorId);
	int SearchMachineStroke(int nMsId);
	int GetAxisStroke(int nMotorId, double fPlusMargin = 0.0, double fMinusMargin = 0.0);	// search and set software limit position 
	void ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate);
	void ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate);
	BOOL GetLimitInput(int nMotorId, int nDir); 
	BOOL StartVelMove(int nAxisId, double fVel, double fAcc);
	BOOL IsEmergency();
	void TowerLampControl(int nLamp, BOOL bOnOff);
	void Alarm(BOOL bOnOff);
	void SetAlarm(BOOL bState);
	void SetHomeAction(int nMotorID, MPIAction Action);
	void ResetHomeAction(int nMotorID);
	BOOL GetOrgInput(int nMotorId);
	void CheckAxisDustCover();

#if USE_SSR_L_UL == USE
	void MotorPowerOnOff(BOOL OnOff);
	int FaultRestoreAndCheckReadyStatus();
	BOOL GetMotionCreated();
	BOOL CheckShinkoDoorOpen();
	BOOL IsServoOn(int nMotorID);
	double PulseToLength(int nAxisId, long nPulse);
	BOOL SetCommandPosition(int nAxisId, double fData);
	CString GetMotorName(int nAxisId);
	BOOL IsAmpFault(int nMotorId);
	BOOL ClearMotionFault(int nMsId);
	BOOL CheckDoorOpenAndExitManual();
	BOOL ReadBitSeg(BYTE cBit, BOOL bInput, int nSeg);
	void WriteBitSeg(int cBit, BOOL bOn, int nSeg);
#endif
	BOOL IsLimitSensor(CString &strMsg);
	BOOL GetPositiveOfLimitSensor();
	int GetAxisOfLimitSensor();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotion)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMotion();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMotion)
	//}}AFX_MSG
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_)
