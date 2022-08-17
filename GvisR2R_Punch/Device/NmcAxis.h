#if !defined(AFX_NMCAXIS_H__6E11F38A_87B6_4101_B697_25830237033A__INCLUDED_)
#define AFX_NMCAXIS_H__6E11F38A_87B6_4101_B697_25830237033A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NmcAxis.h : header file
//

//#ifdef USE_NMC



//#include "NMC_AOI.h"
//#include "type_def.h"

#include "../Process/ThreadTask.h"

#define TEN_SECOND		10000		 // Wait for max 10 minute.
#define TEN_MINUTE		600000		 // Wait for max 10 minute.
#define INVALIDE_DOUBLE	0xFFFFFFFFFFFFFFFF




class CNmcAxis : public CWnd
{
	CWnd* m_pParent;


	BOOL HomeReady();
	BOOL EscapeHomeDirLimitSens();
	BOOL FirstMoveToHomeDirSens();
	BOOL FirstMoveToShiftPos();
	BOOL SecondMoveToHomeDirSens();
	BOOL SecondMoveToShiftPos();
	void FinalizeHome();

public:
// Construction

	CNmcAxis(CWnd* pParent=NULL);

// Attributes
public: 
	BOOL   m_bOrigin; // 원점 복귀 상태 플래그
	CThreadTask m_ThreadTask; // CThreadTask class, handles the threading code
	BOOL m_bHomeThreadAlive;
	double m_fEStopTime;
	double m_fVel, m_fAcc;

	int m_nExeStatus;

	BOOL IsStandStill();
	BOOL IsAmpReady();
	BOOL IsAmpFault();
	BOOL IsGroupMotion();
	double GetState();
	BOOL CheckAxisDone();
	BOOL ClearStatus();
	BOOL CheckMotionDone();
	double GetActualPosition();
	double GetCommandPosition();
	void SetCommandPosition(double dPos);
	void SetActualPosition(double dPos);
	BOOL SetPosition(double fPos);
	void ResetEasyServo();
	BOOL CheckEmgSwitch();
	void SetVMove(double fVel, double fAcc);

	BOOL StartHomeThread();
	void StopHomeThread();
	static UINT HomeThreadProc(LPVOID lpContext);
	void SetAlarmCall(LPVOID lpContext, int nErrCode, CString sMsg);
	BOOL ChkMotionAlarmCall(LPVOID lpContext);
	//BOOL ChkInlineAlarmCall(LPVOID lpContext);
	//BOOL ChkCleanDoor(LPVOID lpContext);

	BOOL StartGantrySlaveMove(BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk = 0.0, byte nDispMode = 3);
	BOOL IsMotionDoneGantrySlave();
	BOOL IsAxisDone();

	void EnableSensorStop(int nSensorIndex, BOOL bEnable); // nSensorIndex : 0 ~ 4 , bEnable 
	BOOL EStop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNmcAxis)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNmcAxis();

	// Generated message map functions

	CThreadTask m_ThreadTaskRsa; // CThreadTask class, handles the threading code
	int m_nSamplingTimeMsec;
	BOOL m_bRsaHomeThreadAlive;

	static UINT RsaHomeThreadProc(LPVOID lpContext);
	int StartRsaHoming(BOOL bWait, int nMode, int nDir);
	int GetBoardID();
	int GetvAxisID();
	BOOL IsGantry();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// NMC Code for VRS 

private:

	UINT16	m_nBoardId;
	UINT16	m_nDevIdIoIn;
	UINT16	m_nDevIdIoOut;

	INT		m_nOffsetAxisID;
	HWND	m_hParentClass;

	int m_nMoveDir;

	int CheckError(INT nErrCode);



public:

	typedef struct MOTOR
	{
		CString sName;
		INT		nAxisID;		// Number of Axis : 0 ~ 7
		BOOL	bType;			// Motor Type 0: servo 1: stepper
		BOOL    bSafety;		// Safety Flag
		double	fLeadPitch;		// ex) Ball Screw Lead Pitch
		INT		nEncPulse;		// Encoder Pulse [pulse/rev]
		INT		nEncMul;		// Encoder Multiplier : encoder_ratio = A/B
		double	fGearRatio;		// Gear Ratio
		double	fInpRange;		// Inposition Range [counts]
		unsigned int nInpTime;	// Inposition Time [msec]
		double  fPosLimit;		// Positive Software Limit
		double  fNegLimit;		// Negative Software Limit
		INT		nVibOffset;		// Vibration offset
		INT		nPulseMode;		// 0:CW/CCW, 1:SIGN/PULSE
		INT		nCordDir;		// 0:CORD_CW, 1:CORD_CCW
		INT		nEncoDir;		// 0:ENCODER_CW (-), 1:ENCODER_CCW (+) 
		INT		nEncRatioA;		// encoder_ratio = A/B
		INT		nEncRatioB;		// encoder_ratio = A/B
		double	fPosRes;		// [mm]
		INT		nPosLimitAction;// Positive Software Limit Action
		INT		nNegLimitAction;// Negative Software Limit Action
	}stMotor;

	typedef struct HOME
	{
		BOOL	bIndex;
		BOOL	bStatus;
		int 	nDir;		// Initial Home Dir, TRUE:plus-dir	FALSE:minus-dir
		double	f1stSpd;	// PreHomming Speed
		double	f2ndSpd;	// Homming Speed
		double	fAcc;		// Homming Accel
		double	fShift;		// shift
		double	fOffset;	// offset
		double	fMargin;	// Margin between Origin & index After Homming
		double  fEscLen;// Escape length from sensor touch position
	}stHome;

	typedef struct SPEED
	{
		double  fMaxVel;		// [mm/s]
		double	fRatingRPM;		// Position
		double	fMinJerkTime;	// Minimum Jerk Time in Sec
		double	fMaxAcc;		// Maximum Acceleration in G
		double	fVel;			// Speed
		double	fAcc;			// Acceleration
		double	fDec;			// Deceleration
		double  fAccPeriod;		// each acceleration period
		double	fJogFastSpd;	// Speed
		double	fJogMidSpd;		// Speed
		double	fJogLowSpd;		// Speed
		double	fJogAcc;		// Acceleration
	}stSpeed;

	typedef struct IO
	{
		BOOL	bPosLimit;		// 정방향 리미트 스위치 신호 레벨
		BOOL	bNegLimit;		// 역방향 리미트 스위치 신호 레벨
		BOOL	bOrg;			// 원점 스위치 신호 레벨
		BOOL	bAmpFault;		// Amp Fault 신호 레벨
		BOOL	bAmpEnable;		// Amp Enable 신호 레벨
		BOOL	bAmpReset;		// Amp Reset 신호 레벨
		BOOL	bInp;			// Inposition 신호 레벨
	}stIO;

	typedef struct GAIN
	{
		INT	nP;			// P Gain
		INT	nI;			// I Gain
		INT	nD;			// D Gain
		INT	nAccelFF;	// Acceleration Feed Forward
		INT	nVelFF;		// Velocity Feed Forward
		INT	nILimit;	// Integral Limit
		INT	nOffset;	// Command Offset
		INT	nDACLimit;	// DAC Limit
		INT	nShift;		// Gain Shift
		INT	nFrictFF;	// Friction Feed Forward
	}stGain;

	typedef struct PARAM
	{
		stMotor Motor;
		stHome Home;
		stSpeed Speed;
		stIO Io;
		stGain Gain;
	}stParam;

	stParam m_stParam;




	BOOL SetOriginPos();
	BOOL SetStop();
	BOOL SetInPosLength(double dLength);
	BOOL SetInPosEnable(int nEnable);
	BOOL VMove(int nDir);
	BOOL VMove(double fVel, double fAcc);
	void SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID);
	BOOL IsStopping();
	BOOL IsInPosition();

	BOOL InitAxis();
	//	BOOL SetHomming();
	BOOL CheckLimitSwitch(int nDir); // PLUS (1), Minus (-1)
	BOOL CheckPosLimitSwitch();
	BOOL CheckNegLimitSwitch();
	double GetJerkTime(double dAcc = INVALIDE_DOUBLE, double dJerk = INVALIDE_DOUBLE);
	double GetAccTime(double dVel = INVALIDE_DOUBLE, double dAcc = INVALIDE_DOUBLE, double dJerk = INVALIDE_DOUBLE);
	double GetVelTime(double dLen = INVALIDE_DOUBLE, double dVel = INVALIDE_DOUBLE, double dAcc = INVALIDE_DOUBLE, double dJerk = INVALIDE_DOUBLE);
	double GetMovingTotalTime(double dLen = INVALIDE_DOUBLE, double dVel = INVALIDE_DOUBLE, double dAcc = INVALIDE_DOUBLE, double dJerk = INVALIDE_DOUBLE);
	double LenToPulse(double fData);
	void SetNegHWLimitAction(INT nAction);
	void SetPosHWLimitAction(INT nAction);
	INT GetNegLimitAction();
	INT GetPosLimitAction();
	BOOL CheckInposition();

	BOOL CheckInMotion();
	double GetPosRes();
	BOOL SetEStopRate(int nStopTime); // [mSec]
	BOOL SetEStop();
	BOOL SetHomeAction(INT nAction);
	void SetOriginStatus(BOOL bStatus = FALSE);
	void EnableSwLimit(BOOL bEnable = TRUE);
	int CmdBufferClear();
	double PulseToLen(double fData);
	double GetInposition();
	double GetNegSoftwareLimit();
	double GetPosSoftwareLimit();
	void SetNegSoftwareLimit(double fLimitVal, INT nAction);
	void SetPosSoftwareLimit(double fLimitVal, INT nAction);
	int CheckExceptionEvent();
	BOOL SetStopRate(int nStopTime); // [mSec]
	int CheckAxisState();
	BOOL ControllerIdle();
	INT GetStopRate(INT nRate);
	BOOL SetVelocity(double fVelocity);
	BOOL CheckHomeSwitch();
	BOOL SetAmpEnableLevel(int nLevel);
	BOOL ControllerRun();
	void SetParentClassHandle(HWND hwnd);
	BOOL CheckAmpFaultSwitch();

	BOOL SetAmpEnable(BOOL bOn);
	BOOL GetAmpEnable();
	BOOL IsMotionDone();
	BOOL WaitUntilMotionMove(int mSec = TEN_SECOND);
	BOOL WaitUntilMotionDone(int mSec = TEN_SECOND);
	BOOL StopVelocityMove(BOOL bWait = 1);
	BOOL StartVelocityMove(double fVel = INVALIDE_DOUBLE, double fAcc = INVALIDE_DOUBLE);
	BOOL StartHomming();
	BOOL StartPtPMove(double fPos, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait);
	BOOL StartPtPMotion(double fPos, double fVel, double fAcc, double fDec, BOOL bAbs = TRUE, BOOL bWait = TRUE);
	BOOL StartSCurveMove(double fPos, double fVel, double fAcc, double fJerk, BOOL bAbs = TRUE, BOOL bWait = TRUE);
	BOOL StartSCurveMove(double fPos, double fVelRatio, BOOL bAbs = TRUE, BOOL bWait = TRUE);
	BOOL Stop(int nRate = 10);	//For iRate * 10 msec, Stopping.
	BOOL WaitUntilAxisDone(unsigned int mSec = TEN_SECOND);
	int AmpFaultReset();


protected:
	//{{AFX_MSG(CNmcAxis)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};




//#endif	// #ifdef USE_NMC

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMCAXIS_H__6E11F38A_87B6_4101_B697_25830237033A__INCLUDED_)