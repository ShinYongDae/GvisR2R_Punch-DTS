#if !defined(AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_)
#define AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Motion.h : header file
//

#include "MotionParam.h"
//#include "XmpControl.h"
#include "NmcDevice.h"
#include "MyFileErrMap.h"

#define TIM_TOOL_PATH	10
#define	TIME_OUT_MS				300000	// 5 minute


/////////////////////////////////////////////////////////////////////////////
// CMotion window

class CMotion : public CWnd
{
	CWnd* m_pParent;
	UINT16	m_nBoardId;
	UINT16	m_nDevIdIoIn;
	UINT16	m_nDevIdIoOut;
	INT		m_nOffsetAxisID;
	int		m_nGroupID_Interpolation[2];

#ifdef USE_NMC
	//CXmpControl *m_pMotionCard;
	CNmcDevice *m_pMotionCard;
#endif
	DWORD m_dwSt, m_dwEd;
	double m_dStMsLsrOffsetX, m_dStMsLsrOffsetY, m_dEdMsLsrOffsetX, m_dEdMsLsrOffsetY;

	void FreeAll();
	void LoadParam();
	BOOL LoadErrMapFile(CString sPath);
	BOOL InitNmcBoard();
	void SetConfigure();
	void SetMotionParam();
	BOOL CreateObject();
// Construction
public:
	CMotion(CWnd* pParent=NULL);
	CCriticalSection m_csGetActualPosition;
	CCriticalSection m_csGetActualVelocity;
	CCriticalSection m_csReadAdc;

// Attributes
public:
	CString m_sPathMotionParam;
	CMyFileErrMap *m_pMyFileErrMap;
	int m_nTotAxis;
	MotionSingle* m_pParam;

	MotionControl m_ParamCtrl;
	MotionMotion* m_pParamMotion;
	MotionAxis* m_pParamAxis;
	MotionMotor* m_pParamMotor;

	double m_dPinPosX[2], m_dPinPosY[2];
	double m_dAlignPosX[2][2], m_dAlignPosY[2][2]; // [nCam][nPos]
	double m_dStBufPos, m_dScaleFdBuf;

	double m_dBaseFocusZ;
	double m_dPanelThicknessByUser;
	BOOL m_bSetPinPos, m_bResetElementID;
	BOOL m_bSetPnlThickness;
	int m_nElementID;
	BOOL m_bMarkOffset;
	double m_dMarkOffsetX, m_dMarkOffsetY;
	BOOL m_bDO[32];
	double m_dFeedRate[4];
	double m_dTotMarkDist, m_dTotJumpDist, m_dTotMarkTime, m_dTotJumpTime;
	unsigned long m_InputFlag;

	BOOL m_bUseSaftyCover, m_bUseVrsSkip;

// Operations
public:
	double GetMotionTime(double dLen,double dVel,double dAcc,double dJerk);
	double GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk,double dMinJerkTime);
	double GetSpeedProfile(int nMode,int nAxisID,double fMovingLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType = HIGH_SPEED);
	double GetSpeedProfile0(int nMode,int nAxisID,double fMovingLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType = HIGH_SPEED);
	double GetSpeedProfile1(int nMode,int nAxisID,double fMovingLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType = HIGH_SPEED);
	//long SetNotifyFlush();
	//float GetEStopTime(int nMotionId);
	//void SetEStopTime(int nMotionId, float fEStopTime);
	//void ResetEStopTime(int nMotionId);
	
	BOOL Abort(int nMotionId);
	//long SetInposition(int nAxis, double fInpRange);
	long GetState(int nMotionId);
	void LoadConfig();
	BOOL ObjectMapping();
	BOOL InitBoard();
// 	BOOL SearchHomeBuf(BOOL bInitPos=TRUE);
	BOOL SearchHome();
	BOOL SearchHomePos(int nMotionId, BOOL bThread=TRUE);
	BOOL IsHome(int nMotionId);
	BOOL IsHomeDone();
	BOOL IsHomeDone(int nMotionId);
	BOOL IsMotionDone(int nMotionId);
	BOOL IsInPosition(int nMotionId);
	BOOL ServoOnOff(int nAxis, BOOL bOn);
	BOOL AmpReset(int nMsId);
	BOOL Move(int nMotionId, double *pTgtPos, BOOL bAbs=ABS, BOOL bWait=NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs=ABS, BOOL bWait=NO_WAIT);
	BOOL Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs=ABS, BOOL bWait=WAIT);
	BOOL Move0(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs=ABS, BOOL bWait=WAIT);
	BOOL Move1(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs=ABS, BOOL bWait=WAIT);
	BOOL Move(int nMotionId, double dTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs=ABS, BOOL bWait=WAIT);
	//BOOL MovePath(int nMotionId);
	//BOOL SetErrMap(CMyFileErrMap *pMyFile);
	double GetActualPosition(int nAxisId);  
	BOOL Stop(int nMotionId);
	BOOL EStop(int nMotionId);
	BOOL Clear(int nMotionId);
	BOOL SetVMove(int nMotionId, double fVel, double fAcc);
	BOOL VMove(int nMotionId, int nDir=1);

	//long ReadAdc(int nSegment, int nCh);
	//BOOL ReadBit(BYTE cBit, BOOL bInput=TRUE);
	//unsigned long ReadAllBit(BOOL bInput);
	//void WriteBit(BYTE cBit, BOOL bOn);
	//void WriteData(long lData);
	//
	//unsigned short Read(int nSegment);
	//void Write(int nSegment, unsigned short nOut);

	//int GetElementID(int nMotionId);
//	BOOL SetFeedRate(int nMotionId, double dFeedRate);
	void SetPinPos(int nCam, int nMotionId, double &dX, double &dY);
	void SetPinPos(int nCam, double dX, double dY);
	void GetPinPos(int nCam, double &dX, double &dY);
// 	void SetStBufPos(double dPos);


	BOOL VMoveStop(int nMotionId, int nDir);
	BOOL IsLimit(int nMotionId, int nDir);
	void GetCamPinPos(double &dX, double &dY);
	void GetCamStPos(double &dX, double &dY);
	void GetMarkOffset(double &dX, double &dY);
	void SetMarkOffset(double dX, double dY);

	void SetR2RConf();
	BOOL SetTorque(int nAxisId, double dKgf);
	double GetActualVelocity(int nAxisId);

// 	void SetLeadPitch(int nAxisId, double dPitch);
	double GetLeadPitch(int nAxisId);

	afx_msg LRESULT OnBufThreadDone(WPARAM wPara, LPARAM lPara);
	//long SetOriginPos(int nAxisId);
// 	void SetBufInitPos(double dPos);
	BOOL IsEnable(int nMsId);
	BOOL IsServoOn(int nMotorID);

	BOOL SetCollision(double dCollisionMargin);
	BOOL ChkCollision();
	void GetData(long *addressActPos1, long *addressActPos2, long *addressDifferenceStored);

	void WriteData(long lData);
	void WriteBit(BYTE cBit, BOOL bOn);
	BOOL ReadBit(BYTE cBit, BOOL bInput = TRUE);
	unsigned long ReadAllBit(BOOL bInput);
	void MotionAbortAll();
	BOOL MotionAbort(int nMsId);

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Motion_H__A031D5EE_C246_46F7_903B_C45B12333A5D__INCLUDED_)
