// NmcAxis.cpp : implementation file
//
#include "../stdafx.h"


#include "NmcDevice.h"

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"
//#include "Adlink_IO.h"

extern CMainFrame* pMainFrame;
extern CGvisR2R_PunchView* pGlobalView;
extern CGvisR2R_PunchDoc* pGlobalDoc;
//extern CAdlink_IO* pIO;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool g_bThreadHomeMotionRun = false;

/////////////////////////////////////////////////////////////////////////////
// CNmcAxis

CNmcAxis::CNmcAxis(CNmcDevice* pParent)
{
	m_pParent = pParent;
	m_fVel = 0.0;
	m_fAcc = 0.0;

	m_bOrigin = FALSE;
	m_nMoveDir = STOP_DIR;

	m_nSamplingTimeMsec = 100;
	m_bRsaHomeThreadAlive = FALSE;
}

CNmcAxis::~CNmcAxis()
{
	if (m_bRsaHomeThreadAlive)
	{
		m_ThreadTaskRsa.Stop();
		m_bRsaHomeThreadAlive = FALSE;
	}
	//if (g_bThreadHomeMotionRun)
	//{
	//	g_bThreadHomeMotionRun = false;
	//	Sleep(100);
	//}
}

/////////////////////////////////////////////////////////////////////////////
// CNmcAxis message handlers

BOOL CNmcAxis::IsAmpFault()
{
	MC_STATUS ms = MC_OK;
	UINT32 Status = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &Status);

	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsAmpFault"));
		return TRUE;
	}

	if ((Status & mcDriveFault) || (Status & mcErrorStop))
		return TRUE;

	return FALSE;
}

unsigned int CNmcAxis::GetState()
{
	MC_STATUS ms = MC_OK;
	UINT32 Status = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &Status);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetState"));
		return MPIStateERROR;
	}

	if ((Status & mcErrorStop) || (Status & mcDriveFault))
		return MPIStateERROR;
	else if((Status & mcStandStill) && (Status & mcReadyForPowerOn))
		return MPIStateIDLE;
	else if (Status & mcStopping)
		return MPIStateSTOPPING;
	else if((Status & mcAccelerating) || (Status & mcDecelerating) || (Status & mcHoming))
		return MPIStateMOVING;
	else
		return MPIStateIDLE;

	return MPIStateIDLE;
}


BOOL CNmcAxis::CheckAxisDone()
{
	return IsAxisDone();

	//MC_STATUS ms = MC_OK;
	//UINT32 state = 0x00000000;

	//ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	//if (ms != MC_OK)
	//{
	//	SaveLog(_T("Error-MC_ReadAxisStatus"));
	//	return FALSE;
	//}

	//if (state & mcErrorStop)
	//{
	//	AmpFaultReset();
	//	ClearStatus();

	//	return TRUE;
	//}
	//else if (state & mcStandStill)
	//	return TRUE;
	//else if (CheckMotionDone())
	//	return TRUE;

	//return FALSE;
}


BOOL CNmcAxis::ClearStatus()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);

	if (state & mcErrorStop)
	{
		((CNmcDevice*)m_pParent)->RestoreSwEmergency();
		Sleep(30);
	}

	ms = MC_Reset(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID);
	Sleep(30);

	if (ms == MC_OK)
		return TRUE;

	return FALSE;
}



BOOL CNmcAxis::CheckMotionDone()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
		return FALSE;
	if (state ^ mcConstantVelocity && state ^ mcAccelerating && state ^ mcDecelerating)
	{
		MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
		if ((state & mcStandStill) > 0)
			return TRUE;
		else if((state& mcASSynchroMotion) >0)
		{
			return TRUE;
		}
	}


	if (state & mcErrorStop)
	{
		ClearStatus();
		Sleep(100);
		return TRUE;
	}

	return FALSE;
}


double CNmcAxis::GetActualPosition()
{
	MC_STATUS ms = MC_OK;
	double dPos;
	double dCurPulse = 0.f, dCurPos = 0.f;

	//ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpActualPosition, &dPos);
	ms =  MC_ReadActualPosition(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &dPos);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetActualPosition()"));
		return 0.0;
	}
	dCurPos = PulseToLen(dPos);

	return dCurPos;
}

double CNmcAxis::GetCommandPosition()
{
	MC_STATUS ms = MC_OK;
	double dPos;
	double dCurPulse = 0.f, dCurPos = 0.f;

	//ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpCommandedPosition, &dPos);
	ms = MC_ReadCommandedPosition(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &dPos);

	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetCommandPosition()"));
		return 0.0;
	}
	dCurPos = PulseToLen(dPos);

	return dCurPos;
}

void CNmcAxis::SetCommandPosition(double dPos)
{
	SetPosition(dPos);
}


BOOL CNmcAxis::SetPosition(double fPos)
{
	MC_STATUS ms = MC_OK;
	int error = 0;
	double dPos = LenToPulse(fPos);

	if (((CNmcDevice*)m_pParent)->m_bGantryEnabled && m_stParam.Motor.nAxisID == SCAN_AXIS)
	{
#ifdef SCAN_S_AXIS
		((CNmcDevice*)m_pParent)->GantryEnable(SCAN_AXIS, SCAN_S_AXIS, FALSE);
		Sleep(50);

		//ms = MC_SetPosition(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, false, mcImmediately);
		//if (ms == MC_LIMIT_ERROR_PARAM_3)
		//{
		//	return FALSE;
		//}
		//Sleep(50);

		((CNmcDevice*)m_pParent)->GantryEnable(SCAN_AXIS, SCAN_S_AXIS, TRUE);
		Sleep(50);
#endif
	}
	else
	{
		ms = MC_SetPosition(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, false, mcImmediately);
		if (ms == MC_LIMIT_ERROR_PARAM_3)
		{
			return FALSE;
		}
	}
	return TRUE;
}


double CNmcAxis::PulseToLen(double fData)
{
	if (m_stParam.Motor.fPosRes != 0.0)
	{
		fData *= m_stParam.Motor.fPosRes;
		return fData;// /4.0;
	}
	else
		return -1.0;
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NMC Code for VRS 

void CNmcAxis::SetParentClassHandle(HWND hwnd)
{
	m_hParentClass = hwnd;
}

int CNmcAxis::CheckError(INT nErrCode)
{
	char strErrMsg[MAX_PATH] = { "", };

	if (nErrCode)
	{
		if (nErrCode != MC_OK)
		{
			CString strMsg;
			strMsg.Format(_T("error code : %d , message : "), nErrCode);
			strMsg += strErrMsg;
			SaveLog(strMsg);
		}
	}

	return (int)nErrCode;
}

BOOL CNmcAxis::InitAxis()
{
	double f = pow(2, 31) - 600;   //2018.08.21 kjs
	//if (m_stParam.Motor.bType == SERVO_MOTOR)
	//{
	//}
	//else if (m_stParam.Motor.bType == STEPPER)
	//{
	//}

	short axes[] = { 0, 1, 2 };
	short JoyStick[] = { 5, 6, 7 };
	//pGlobalView->m_pMotion->m_pMmcDevice->SetJoystickEnable(0, 0x00);
	//((CNmcDevice*)FromHandle(m_hParentClass))->SetJoystickEnable(0, 0x00);

	return TRUE;
}

BOOL CNmcAxis::GetAmpEnable()
{
	MC_STATUS ms = MC_OK;
	UINT32 Status = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &Status);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetAmpEnable()"));
		return FALSE;
	}

	return ((Status & mcPowerOn) ? TRUE : FALSE);
}

BOOL CNmcAxis::IsAmpReady()
{
	MC_STATUS ms = MC_OK;
	UINT32 Status = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &Status);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsAmpReady()"));
		return FALSE;
	}

	if ((Status & mcStandStill) && !(Status & mcDisabled))
		return TRUE;

	return FALSE;
}



BOOL CNmcAxis::IsMotionDone()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsMotionDone()"));
		return FALSE; 
	}
	if ( !(state & mcContinuousMotion) && !(state & mcConstantVelocity) && !(state & mcAccelerating) && !(state & mcDecelerating) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CNmcAxis::IsStandStill()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsStandStill()"));
		return FALSE;
	}
	if ((state & mcStandStill) > 0)
		return TRUE;

	return FALSE;
}

BOOL CNmcAxis::IsAxisDone()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsMotionDone()"));
		return FALSE;
	}
	if ((state & mcStandStill) && !(state & mcContinuousMotion) && !(state & mcConstantVelocity) && !(state & mcAccelerating) && !(state & mcDecelerating))
	{		
		return TRUE;
	}

	return FALSE;
}
BOOL CNmcAxis::IsStopping()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsStopping()"));
		return FALSE;
	}
	if (state & mcStopping)
		return TRUE;

	return FALSE;
}


BOOL CNmcAxis::WaitUntilMotionDone(int mSec)
{
#ifdef RI_SYSTEM
	return 1;
#endif

	DWORD CurTimer, StartTimer;
	MSG message;

	Sleep(10);
	StartTimer = GetTickCount();
	CurTimer = GetTickCount();
	ULONGLONG nWaitTick = GetTickCount64();
	while (!IsMotionDone() && mSec > int(CurTimer - StartTimer))
	{
		CurTimer = GetTickCount();
	//	if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	//	{
	//		::TranslateMessage(&message);
	//		::DispatchMessage(&message);
	//	}
		Sleep(10);

		int nState = CheckAxisState();

		if (CheckAmpFaultSwitch())
		{
			CmdBufferClear();
			ClearStatus();
			AmpFaultReset();
			ClearStatus();

			return FALSE;
		}
		else if (GetAmpEnable() == 0)
		{
			return FALSE;
		}
		else
		{
			if (IsAxisDone())
			{
				Sleep(50);	// Absolutely needed in case of Fastek easy servo for Z Axis....
			}
		}

		int nEx = CheckExceptionEvent();
		Sleep(10);
		if (nEx & ST_AMP_FAULT || nEx & ST_ERROR_LIMIT || nEx & ST_AMP_POWER_ONOFF)
		{
			CmdBufferClear();
			ClearStatus();
			return FALSE;
		}

		if (int(CurTimer - StartTimer) > 1)
		{
			if (nEx & ST_INPOSITION_STATUS)
			{
				return TRUE;
			}

			if (int(CurTimer - StartTimer) > 3)
			{
				if (nEx & ST_X_POS_LIMIT || nEx & nEx & ST_X_NEG_LIMIT)
				{
					return FALSE;
				}
			}
		}

		if (nEx & ST_AMP_FAULT || nEx & ST_ERROR_LIMIT)
		{
			return FALSE;
		}
	}

	if (mSec > int(CurTimer - StartTimer))
		return TRUE;

	return FALSE;
}

BOOL CNmcAxis::WaitUntilAxisDone(unsigned int nWaitTime)
{
	DWORD CurTimer, dwStartTimer, dwSettleTimer, dwElapsedTime, dwSettlingTime;

	MSG message;

	double dDiffPos = 0.0;
	BOOL bDone = FALSE;

	dwStartTimer = GetTickCount();
	CurTimer = GetTickCount();

	// Check Positioning error
	if (m_stParam.Motor.bType == STEPPER)	//Stepping motor no encoder
	{
		return WaitUntilMotionDone(nWaitTime);
	}

	dwSettlingTime = 0;
	bDone = TRUE;

	dwSettleTimer = GetTickCount();
	ULONGLONG nTimeTick = GetTickCount64();
	int nRetryCnt = 0;
	do
	{

		if (IsAxisDone())
		{
			dDiffPos = fabs(GetCommandPosition() - GetActualPosition());
			Sleep(100);
			if (dDiffPos > GetInposition())
			{
				if (nRetryCnt >= 10)
				{
					bDone = FALSE;

					break;
				}
				else
				{
					nRetryCnt++;
					dwSettleTimer = GetTickCount(); // reset to dwSettleTimer
					dwSettlingTime = 0;
					Sleep(10);
				}
			}
			else
			{
				bDone = TRUE;
				dwSettlingTime = GetTickCount() - dwSettleTimer;
				return TRUE;
			}
			dwElapsedTime = GetTickCount() - dwStartTimer;

			if (dwElapsedTime > nWaitTime)
			{
				bDone = FALSE;
				break;
			}
		}
		else
		{
			dwSettleTimer = GetTickCount(); // reset to dwSettleTimer
			dwSettlingTime = 0;

			if (GetTickCount64() - nTimeTick >= nWaitTime)
			{
				return FALSE;
			}
		}

		//	if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		//	{
		//		::TranslateMessage(&message);
		//		::DispatchMessage(&message);
		//	}

		int nState = CheckAxisState();

		if (CheckAmpFaultSwitch())
		{
			if (nState == 0)
			{
				StopVelocityMove(0);
			}
			ClearStatus();
			AmpFaultReset();
			ClearStatus();

			return FALSE;
		}
		else if (GetAmpEnable() == 0)
		{
			return FALSE;
		}

		int nEx = CheckExceptionEvent();
		if (nEx & ST_AMP_FAULT || nEx & ST_ERROR_LIMIT || nEx & ST_AMP_POWER_ONOFF)
		{
			return FALSE;
		}
		Sleep(10);
	} while (dwSettlingTime < 1000);

	return bDone;
}


BOOL CNmcAxis::CheckLimitSwitch(int nDir) // PLUS (1), Minus (-1)
{
	INT nState = 0;
	if (nDir > 0)
		nState = CheckPosLimitSwitch();
	else
		nState = CheckNegLimitSwitch();
	return nState;
}

BOOL CNmcAxis::CheckPosLimitSwitch()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;


	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
		return FALSE;

	if (state & mcLimitSwitchPos)
		return TRUE;

	return FALSE;
}

BOOL CNmcAxis::CheckNegLimitSwitch()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms != MC_OK)
		return FALSE;

	if (state & mcLimitSwitchNeg)
		return TRUE;

	return FALSE;
}

double CNmcAxis::LenToPulse(double fData)
{
	if (m_stParam.Motor.fPosRes != 0.0)
	{
		fData /= m_stParam.Motor.fPosRes;
		return fData;
	}
	return (double)(INVALIDE_DOUBLE);
}

double CNmcAxis::GetMovingTotalTime(double dLen, double dVel, double dAcc, double dJerk)
{
	if (dLen == INVALIDE_DOUBLE)		return (double)(INVALIDE_DOUBLE);
	if (dVel == INVALIDE_DOUBLE)		dVel = m_stParam.Speed.fVel;
	if (dAcc == INVALIDE_DOUBLE)		dAcc = m_stParam.Speed.fAcc;
	if (dJerk == INVALIDE_DOUBLE)		dJerk = m_stParam.Speed.fMinJerkTime;

	return dAcc / dJerk + dVel / dAcc + dLen / dVel;
}

double CNmcAxis::GetAccTime(double dVel, double dAcc, double dJerk)
{
	if (dVel == INVALIDE_DOUBLE)		dVel = m_stParam.Speed.fVel;
	if (dAcc == INVALIDE_DOUBLE)		dAcc = m_stParam.Speed.fAcc;
	if (dJerk == INVALIDE_DOUBLE)		dJerk = m_stParam.Speed.fMinJerkTime;
	double dAccTime = dVel / dAcc;
	if (dAccTime < 0)		dAccTime *= -1;

	return dAccTime;
}

double CNmcAxis::GetJerkTime(double dAcc, double dJerk)
{
	if (dAcc == INVALIDE_DOUBLE)		dAcc = m_stParam.Speed.fAcc;
	if (dJerk == INVALIDE_DOUBLE)		dJerk = m_stParam.Speed.fMinJerkTime;

	return dAcc / dJerk;
}

double CNmcAxis::GetVelTime(double dLen, double dVel, double dAcc, double dJerk)
{
	if (dLen == INVALIDE_DOUBLE)		return (double)(INVALIDE_DOUBLE);
	if (dVel == INVALIDE_DOUBLE)		dVel = m_stParam.Speed.fVel;
	if (dAcc == INVALIDE_DOUBLE)		dAcc = m_stParam.Speed.fAcc;
	if (dJerk == INVALIDE_DOUBLE)		dJerk = m_stParam.Speed.fMinJerkTime;

	return dLen / dVel - dVel / dAcc - dAcc / dJerk;
}

BOOL CNmcAxis::StartVelocityMove(double fVel, double fAcc)
{
	CString strTitleMsg=_T(""), strMsg=_T("");

	if (!WaitStandStill())
		return FALSE;
	CheckStatusAndClear();

/*
	while (!IsStandStill())
	{
		strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "Waiting for Motor stand still."));

		if (pGlobalView->GetSafeHwnd())
		{
			strMsg = _T("Waiting for Motor stand still.");
			pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_GREEN, 30000);
		}

		Sleep(100);
	}

	if(strTitleMsg != _T(""))
		pGlobalView->ClearDispMessage();
*/

	if (fVel == INVALIDE_DOUBLE)		fVel = m_stParam.Speed.fVel;
	if (fAcc == INVALIDE_DOUBLE)		fAcc = m_stParam.Speed.fAcc;

	if (fabs(fVel) > m_stParam.Speed.fMaxVel)
	{
		if(fVel>0)
			fVel = m_stParam.Speed.fMaxVel;
		else
			fVel = -m_stParam.Speed.fMaxVel;

		//SaveLog(_T("Exceed Maximum Speed"));
		//return FALSE;
	}

	if (!WaitUntilAxisDone(TEN_SECOND))
	{
		return FALSE;
	}

	int nAxisId = m_stParam.Motor.nAxisID;

#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_S_AXIS)
	{
		double fPos;

		if (fVel > 0.0)
			fPos = 1000.0;
		else
			fPos = -1000.0;

		return StartGantrySlaveMove(INC, fPos, fVel, fAcc, fAcc, 0.0, 3);
	}

	if (nAxisId == SCAN_AXIS)
	{
		while (!IsMotionDoneGantrySlave())
		{
			Sleep(100);
		}
	}
#endif

	if (IsAmpFault())
	{
		ClearStatus();
		Sleep(30);
	}


	double dVel, dAcc;
	INT nAccTime;

	MC_STATUS err = MC_OK;
	MC_DIRECTION enDir = mcPositiveDirection;
	if (fVel < 0.0)
	{
		enDir = mcNegativeDirection;
		fVel *= -1.0;
	}

	dVel = LenToPulse(fVel);
	dAcc = LenToPulse(fAcc);

	err = MC_MoveVelocity(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dVel, dAcc, dAcc, 0, enDir, mcAborting);

	if (err != MC_OK)
	{
		if (err & MC_LIMIT_POSITION_OVER)
			return TRUE;
		return FALSE;
	}
	return TRUE;
}

double CNmcAxis::GetActVel()
{
	MC_STATUS ms = MC_OK;
	double dVel;

	ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpActualVelocity, &dVel);
	
	if (ms != MC_OK)
	{
		return 0.0;
	}

	return PulseToLen(dVel);
}


BOOL CNmcAxis::StopVelocityMove(BOOL bWait)
{
	MC_STATUS err = MC_OK;

	err = MC_Halt(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, 500000.0, 0.0, mcAborting);

	if (bWait)
	{
		if (!WaitUntilMotionDone(TEN_SECOND))
			return FALSE;

		Sleep(50);		// prevent Jerk problem at stop. 

		if (!ClearStatus())
			return FALSE;

		Sleep(10);
	}
	return TRUE;
}

void CNmcAxis::SetNegHWLimitAction(INT nAction)
{
	MC_STATUS ms = MC_OK;

	ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableHWLimitNeg, (nAction != NO_EVENT));
}

INT CNmcAxis::GetNegLimitAction()
{
	INT nAction = E_STOP_EVENT;
	MC_STATUS ms = MC_OK;
	bool Value;

	ms = MC_ReadBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableHWLimitNeg, &Value);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetNegLimitAction()"));
		return nAction;
	}
	if (Value)
		nAction = E_STOP_EVENT;
	else
		nAction = NO_EVENT;

	return nAction;
}

void CNmcAxis::SetPosHWLimitAction(INT nAction)
{
	MC_STATUS ms = MC_OK;

	ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableHWLimitPos, (nAction != NO_EVENT));
}

INT CNmcAxis::GetPosLimitAction()
{
	INT nAction = E_STOP_EVENT;
	MC_STATUS ms = MC_OK;
	bool Value;

	ms = MC_ReadBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableHWLimitPos, &Value);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetNegLimitAction()"));
		return nAction;
	}
	if (Value)
		nAction = E_STOP_EVENT;
	else
		nAction = NO_EVENT;

	return (int)nAction;
}

//2021 08 13 lgh add 
//amp enable이 안되었을수도 있고 에러 상태인경우 StandStill이 안될수도 있을 것같음. 무한 루프 방지
BOOL CNmcAxis::WaitStandStill()
{
	CString strTitleMsg = _T(""), strMsg = _T("");

	BOOL bFirst = 1;

	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);

	if (ms != MC_OK)
	{
		SaveLog(_T("Error-IsStandStill()"));
		return FALSE;
	}

	if ((state & mcStandStill) > 0)
		return TRUE;

	//amp enable이 안되었을수도 있고 에러 상태인경우 StandStill이 안될수도 있을것?
	while (1)
	{
		ms = MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	
		if (ms != MC_OK)
		{
			SaveLog(_T("Error-IsStandStill()"));
			return FALSE;
		}

		if ((state & mcStandStill) > 0)
			return TRUE;

		//AMP FAULT이거나 켜지지 않은 상태에서 빠져나가게 한다. 무한 루프 방지
		if ((state & mcErrorStop) > 0)
			return TRUE;

		if ((state & mcDisabled) > 0)
			return TRUE;

		if ((state & mcSensorStop) > 0)
			return TRUE;

		//앰프 ENABLE이 아니면 빠져나간다.
		if ((state & mcPowerOn) == 0)
			return TRUE;

		if (bFirst)
		{
			strTitleMsg.Format(_T("%s Waiting for Motor stand still."), m_stParam.Motor.sName);

			bFirst = 0;
			SetMainMessage(strTitleMsg);
		}

		if (g_bDestroy)
			return FALSE;

		Sleep(0);
	}

	return TRUE;
}

void CNmcAxis::CheckStatusAndClear()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (state & mcStandStill)
	{
		return;
	}

	if (state & mcErrorStop)
	{
		ClearStatus();
		Sleep(100);
	}

	ms = MC_ReadStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);

	if (state & mcDisabled)
	{
		SetAmpEnable(0);
		ClearStatus();
		Sleep(100);
		SetAmpEnable(1);
	}
}

BOOL CNmcAxis::StartPtPMotion(double fPos, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait)
{
	int error = 0;
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	CString strTitleMsg = _T(""), strMsg = _T("");

	if (!WaitStandStill())
		return FALSE;

	CheckStatusAndClear();

	double dJerkTime = GetAccTime(fVel, fAcc); //0.2; // [sec]

	double dPos = LenToPulse(fPos);
	double dVel = LenToPulse(fVel);
	double dAcc = LenToPulse(fAcc);
	double dDec = LenToPulse(fDec);
	double fJerk = (fAcc / dJerkTime);
	double dJerk = LenToPulse(fJerk);

	int nAxisId = m_stParam.Motor.nAxisID;

#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_S_AXIS)
	{
		BOOL bRtn = StartGantrySlaveMove(bAbs, fPos, fVel, fAcc, fAcc, fJerk, 3);
		if (bWait)
			WaitUntilMotionDone(TEN_SECOND);
		return bRtn;
	}

	if (nAxisId == SCAN_AXIS)
	{
		while (!IsMotionDoneGantrySlave())
		{
			Sleep(100);
		}
	}
#endif

	if (IsAmpFault())
	{
		ClearStatus();
		Sleep(30);
	}

	if (bAbs)
	{
		// absolute coordinate move
		if (!bWait)
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(001) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(002) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}

			WaitUntilMotionDone(ONE_MINUTE);
		}
	}
	else
	{
		// incremental coordinate move
		if (!bWait)
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}

			WaitUntilMotionDone(ONE_MINUTE);
		}
	}

	return TRUE;
}


BOOL CNmcAxis::CheckInposition()
{
	INT nDone = 0;
	nDone = IsAxisDone();
	return (nDone ? TRUE : FALSE);
}


BOOL CNmcAxis::CheckInMotion()
{
	INT nDone = 0;
	nDone = CheckMotionDone();
	return (nDone ? TRUE : FALSE);
}

double CNmcAxis::GetPosRes()
{
	return m_stParam.Motor.fPosRes;
}

BOOL CNmcAxis::SetAmpEnable(BOOL bOn)
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;
	CString strTitleMsg = _T(""), strMsg = _T("");

	if (bOn)
	{
		if (GetAmpEnable())
		{
			return TRUE;
		}
	}
	ClearStatus();
	Sleep(100);

	ms = MC_Power(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, bOn);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetAmpEnable()"));
		return FALSE;
	}

	if (bOn)
	{
		WaitStandStill();
		/*
		while (!IsStandStill())
		{
			strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "Waiting for Motor stand still."));

			if (pGlobalView->GetSafeHwnd())
			{
				strMsg = _T("Waiting for Motor stand still.");
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_GREEN, 30000);
			}
			Sleep(100);
		}
		*/

		Sleep(500); // Fastek EasyServo Z축 서보 On시 Delay필요....	
	}

	/*
	if (strTitleMsg != _T(""))
		pGlobalView->ClearDispMessage();
	*/

	if (ms != MC_OK)
	{
		SaveLog(_T("Failed Amp Enable !!!"));
		return FALSE;
	}

	if (!GetAmpEnable() && bOn)
	{
		INT nAction = GetNegLimitAction();
		if (CheckLimitSwitch(MINUS))
		{
			nAction = GetNegLimitAction();
			SetNegHWLimitAction(E_STOP_EVENT);//ABORT_EVENT);
			Sleep(100);
			SetNegHWLimitAction(NO_EVENT);//ABORT_EVENT);
			Sleep(100);
		}
		else if (CheckLimitSwitch(PLUS))
		{
			nAction = GetPosLimitAction();
			SetPosHWLimitAction(E_STOP_EVENT);//ABORT_EVENT);
			Sleep(100);
			SetPosHWLimitAction(NO_EVENT);//ABORT_EVENT);
			Sleep(100);
		}

		ClearStatus();
		Sleep(100);

		ms = MC_Power(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, bOn);
		if (ms != MC_OK)
		{
			SaveLog(_T("Error-SetAmpEnable()"));
			return FALSE;
		}

		Sleep(500);

		if (CheckLimitSwitch(MINUS))
		{
			SetNegHWLimitAction(nAction);//ABORT_EVENT);
			Sleep(100);
		}
		else if (CheckLimitSwitch(PLUS))
		{
			SetPosHWLimitAction(nAction);//ABORT_EVENT);
			Sleep(100);
		}

		if (!GetAmpEnable())
			return FALSE;
	}
	else if (GetAmpEnable() && !bOn)
	{
		return FALSE;
	}
	
	if (bOn)
	{
		Sleep(100);

		ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
		if (ms != MC_OK)
		{
			SaveLog(_T("Error-MC_ReadAxisStatus()"));
			return FALSE;
		}

		if (state & mcDriveFault)
		{
			SaveLog(_T("Failed Amp Enable - mcDriveFault !!!"));
			return FALSE;
		}

		if (!IsAmpReady()) // Fastek EasyServo Z축 서보 On시 확인 필요....
		{
			ms = MC_Power(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, 0);
			ClearStatus();
			Sleep(500);
			ms = MC_Power(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, 1);
			Sleep(500);
			ClearStatus();
			Sleep(1000);

			if (!IsAmpReady())
			{
				SaveLog(_T("Error-IsAmpReady()"));
			}
		}
	}

	return TRUE;
}

BOOL CNmcAxis::SetEStopRate(int nStopTime) // [mSec]
{
	MC_STATUS ms = MC_OK;
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, IMMEDIATE);
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, DECEL);
	return ((ms == MC_OK) ? TRUE : FALSE);
}

BOOL CNmcAxis::SetEStop() // [mSec]
{
	MC_STATUS ms = MC_OK;
	double dDec = LenToPulse(m_stParam.Speed.fDec);
	double dJerk = 0.0;// m_stParam(m_stParam.Speed.fMinJerkTime);
	ms = MC_Halt(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dDec, dJerk, mcAborting);
	return ((ms == MC_OK) ? TRUE : FALSE);
}

BOOL CNmcAxis::SetStopRate(int nStopTime) // [mSec]
{
	MC_STATUS ms = MC_OK;
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, IMMEDIATE);
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, DECEL);
	return ((ms == MC_OK) ? TRUE : FALSE);
}

BOOL CNmcAxis::SetStop() // [mSec]
{
	MC_STATUS ms = MC_OK;
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, IMMEDIATE);
	//ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpmcpEStopType, DECEL);

	double dDec = m_stParam.Speed.fDec;
	double dJerk = m_stParam.Speed.fMinJerkTime;

	//ms = MC_Stop(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, true, dDec, dJerk);
	ms = MC_Halt(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dDec, dJerk, mcAborting);

	return ((ms == MC_OK) ? TRUE : FALSE);
}

BOOL CNmcAxis::SetHomeAction(INT nAction)
{
	MC_STATUS ms = MC_OK;
	UINT32 bitnum = -1;

	switch (nAction)
	{
	case NO_EVENT:
		bitnum = 255;	//Disable
		break;

	case STOP_EVENT:
		bitnum = 2;		//Default Home Limit Bit Number
		break;

	case E_STOP_EVENT:
		bitnum = 2;		//Default Home Limit Bit Number
		break;

	case ABORT_EVENT:
		bitnum = 2;		//Default Home Limit Bit Number
		break;

	default:
		break;
	}

	ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpHomeInputNum, bitnum);

	return ((ms == MC_OK) ? TRUE : FALSE);
}

void CNmcAxis::EnableSwLimit(BOOL bEnable)
{
	SetPosSoftwareLimit(m_stParam.Motor.fPosLimit, NO_EVENT);
	SetNegSoftwareLimit(m_stParam.Motor.fNegLimit, NO_EVENT);
	return;

	if (!bEnable)
	{
		SetPosSoftwareLimit(m_stParam.Motor.fPosLimit, NO_EVENT);
		SetNegSoftwareLimit(m_stParam.Motor.fNegLimit, NO_EVENT);
	}
	else
	{
		SetPosSoftwareLimit(m_stParam.Motor.fPosLimit, E_STOP_EVENT);
		SetNegSoftwareLimit(m_stParam.Motor.fNegLimit, E_STOP_EVENT);
	}
	Sleep(10);
}

BOOL CNmcAxis::CheckEmgSwitch()
{
	return ((CNmcDevice*)m_pParent)->ReadIn(EMERGENCY_SWITCH);
}

BOOL CNmcAxis::StartHomeThread()
{
	m_bOrigin = FALSE;
	m_ThreadTask.Start(nullptr, this, HomeThreadProc);// Start the thread
	return TRUE;
}

void CNmcAxis::StopHomeThread()
{
	m_ThreadTask.Stop();// Stop the thread
}

void CNmcAxis::SetAlarmCall(LPVOID lpContext, int nErrCode, CString sMsg)
{
	CNmcAxis* pThread = reinterpret_cast<CNmcAxis*>(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);
	int nAxisID = pThread->m_stParam.Motor.nAxisID;
	CString strMsg, strTitleMsg;

	strTitleMsg.Format(_T("%s %s"), pThread->m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));
	strMsg.Format(_T("Axis #%d %s"), nAxisID, sMsg);

	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);

	pThread->m_bHomeThreadAlive = FALSE;
	pParent->m_sMotionError = strMsg;
	pParent->m_nMotError = nErrCode;
	pParent->m_nInterlockStatus = TRUE;
}

BOOL CNmcAxis::ChkMotionAlarmCall(LPVOID lpContext)
{
	CNmcAxis* pThread = reinterpret_cast< CNmcAxis* >(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);
	INT nAct = (INT)NO_EVENT;
	int nAxisID = pThread->m_stParam.Motor.nAxisID;
	CString strMsg, strTitleMsg;

	if (pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir) && pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
	{	
		nAct = (INT)MPIActionE_STOP;
		pThread->SetPosHWLimitAction(nAct);
		pThread->SetNegHWLimitAction(nAct);
		
		pThread->SetAlarmCall(lpContext, 1, _T("Pos and Neg Limit Sensor Error!!! Please restart the AOI"));

		if (pGlobalDoc->m_bUseInlineAutomation)
			CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

		return TRUE;
	}

	return FALSE;
}

BOOL CNmcAxis::ChkInlineAlarmCall(LPVOID lpContext)
{
	CNmcAxis* pThread = reinterpret_cast<CNmcAxis*>(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);
	INT nAct = (INT)NO_EVENT;
	int nAxisID = pThread->m_stParam.Motor.nAxisID;
	CString strMsg;

	if (pParent->ReadIn(LD_SHK_TABLE_LOCK))
	{
		pThread->SetEStop();

		if (pGlobalDoc->m_nSelectedLanguage == KOREA)
		{
			SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			strMsg = _T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오");
		}
		else
		{
			SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));
			strMsg = _T("Table lock status. Please try again after moving the loader / unloader");
		}

		//pThread->m_bHomeThreadAlive = FALSE;
		//pParent->m_sMotionError = strMsg;
		//pParent->m_nMotError = 1;
		//pParent->m_nInterlockStatus = TRUE;
		pThread->SetAlarmCall(lpContext, 1, strMsg);

		if (pGlobalDoc->m_bUseInlineAutomation)
			CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

		return TRUE;
	}

	return FALSE;
}

BOOL CNmcAxis::ChkCleanDoor(LPVOID lpContext)
{
	CNmcAxis* pThread = reinterpret_cast<CNmcAxis*>(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);
	INT nAct = (INT)NO_EVENT;
	int nAxisID = pThread->m_stParam.Motor.nAxisID;
	CString strMsg, strTitleMsg;

	if (!CCleanDoor::m_pCleanDoor->GetInputBit(IN_CLEAN_DOOR_AREA_SENSOR))
	{
		pThread->SetEStop();
		pThread->SetAmpEnable(FALSE);	// Abort

#if CUSTOMER_COMPANY != SUMITOMO	//120822 hjc add
		pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);	// 20130726 ljh
#else
		pGlobalView->TowerLampControl(TOWER_LAMP_YELLOW, ON);	// 20130726 ljh
#endif
		pGlobalView->m_pIO->BuzzerOnOff(ON);

		pParent->m_nInterlockType = SAFETY_AREA_SENSOR;

		pThread->m_bOrigin = FALSE;
		if (pParent->m_nInterlockStatus == 0)
		{
			strTitleMsg.Format(_T("%s %s"), pThread->m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));

			if (pGlobalView->GetSafeHwnd())
				strMsg = _T("[MSG568] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG14");
			else
				strMsg = _T("Detected Clean Door Area Sensor!!!!");

			pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 30000);
		}
		pThread->m_bHomeThreadAlive = FALSE;

		pParent->m_nInterlockStatus = TRUE;

		return TRUE;
	}

	return FALSE;
}

// Home thread body
UINT CNmcAxis::HomeThreadProc(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CNmcAxis* pThread = reinterpret_cast< CNmcAxis* >(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);


	DWORD dwTimePeriod = 10; // 10ms sec sleep
	DWORD dwST, dwEd;
	pThread->m_bHomeThreadAlive = TRUE;

	BOOL bMotDone, bInPos;

	int nAxisID = pThread->m_stParam.Motor.nAxisID;
	int nMotorID = pThread->m_stParam.Motor.nAxisID;

	// Software Limit값 임시 퇴피.
	double fPrevPosLimit = pThread->m_stParam.Motor.fPosLimit;
	double fPrevNegLimit = pThread->m_stParam.Motor.fNegLimit;

	BOOL bError = 0;

	CString strTitleMsg;
	CString strMsg;

	double fSpeed, fMachineSpeed, fENewStopTime, dTempVal;
	double dResolution = pThread->GetPosRes();
	if (dResolution <= 0.01)
	{
		fSpeed = 1.0 / dResolution; // [mm/s]
		fMachineSpeed = pThread->PulseToLen(fSpeed) / 2.0;
		fENewStopTime = float(fMachineSpeed / 1000.0);
		if (fENewStopTime<0.01)
			fENewStopTime = 0.01;
	}
	else
	{
		fMachineSpeed = (dResolution*1000.0) / 2.0; // [mm/s]
		fENewStopTime = float(fMachineSpeed / 1000.0);
		fENewStopTime = 0.05;
	}

	BOOL bLimitEvent = FALSE;
	BOOL bChkState = FALSE;
	int nExeStatus = 0;

	strTitleMsg.Format(_T("%s %s"), pThread->m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "MOTOR_POSITION_INITIALIZE"));
	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	if (nMotorID == SCAN_AXIS)
	{
#ifdef MOTION_ACTION
		if (AoiParam()->m_bUseProductDetectSensor)
		{
			if (pGlobalView->m_pIO->ReadBit(PRODUCT_DETECT_SENSOR))
			{
				if (pGlobalView->m_pIO->ReadBit(VACUUME_SOLENOID, FALSE) == 0 || pGlobalView->m_pIO->ReadBit(AIR_BLOWER_RELAY, FALSE) == 0)
				{
					pGlobalView->m_pIO->AirControl(TRUE);
				}
			}
		}
#endif
	}

	CString strAxisName = pThread->m_stParam.Motor.sName;

	double dStartPos = 0.0;
	double dCurrPos = 0.0;
	BOOL bSixStepTrigger = 0;
	ULONGLONG nOriginTick = GetTickCount64();
	if (pThread->m_stParam.Home.bIndex == 0)
	{
		nOriginTick = GetTickCount64();
		while (!pThread->m_bOrigin && WAIT_OBJECT_0 != ::WaitForSingleObject(pThread->m_ThreadTask.GetShutdownEvent(), dwTimePeriod))
		{

			if (pGlobalDoc->m_bMotionAlarm || pGlobalDoc->m_bUseInlineAutomation)
			{
				if(pThread->ChkMotionAlarmCall(lpContext))
				{
					bError = TRUE;
					return 0;
				}
			}

			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if(pThread->ChkInlineAlarmCall(lpContext))
				{
					return MEI_MOTION_ERROR;
				}
			}

			BOOL bClampError = 0;
			if (AoiParam()->m_bUseTableClamp)
			{
				bClampError = GetClampObject()->ClampStatusError();
			}

#ifdef CLEAN_DOOR_AXIS
			if (nMotorID == CLEAN_DOOR_AXIS)
			{
				if(pThread->ChkCleanDoor(lpContext))
				{
					return 0;
				}
			}
#endif

			Sleep(10);

			switch (nExeStatus)
			{
			case 0:
				pThread->m_bOrigin = FALSE;
				if (pThread->IsAxisDone())
				{
					// software limit value 
					pThread->SetPosSoftwareLimit(5000.0, pThread->GetPosLimitAction());
					pThread->SetNegSoftwareLimit(-5000.0, pThread->GetNegLimitAction());

					pThread->SetPosHWLimitAction(MPIActionNONE);
					pThread->SetNegHWLimitAction(MPIActionNONE);

					if (pThread->m_stParam.Motor.bType == STEPPER) // 20180413-syd : Modified for Fastech synqnet servo pack.
					{
						//pThread->SetErrorLimitAction(MPIActionNONE);
						//Sleep(30);
						//pThread->SetCommandPosition(0.0);
						//pThread->SetActualPosition(0.0);
						pThread->SetPosition(0.0);
						Sleep(30);
						//pThread->Clear();
						pThread->ClearStatus();
						Sleep(30);
						//pThread->m_pObjectMotor[nMotorID].Enable(TRUE);
						pThread->SetAmpEnable(TRUE);
						Sleep(30);
					}
					nExeStatus++;
					nOriginTick = GetTickCount64();
				}
				else
				{
					if (pThread->GetState() == MPIStateERROR)
					{
						pThread->ClearStatus();
						Sleep(100);

						if (GetTickCount64() - nOriginTick >= 15000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
					}
				}

				break;
			case 1:
				if (pGlobalDoc->m_bMotionAlarm || pGlobalDoc->m_bUseInlineAutomation)
				{
					if (pThread->GetState() == MPIStateERROR || !pThread->GetAmpEnable()) // 20180413-syd : Modified for Fastech synqnet servo pack.
					{
						//strMsg.Format(_T("Axis #%d %s"), nAxisID, _T("Failed that Motor servo On."));
						//pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg;	// 20180517 - syd
						//pParent->m_nMotError = 2;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 2, _T("Failed that Motor servo On."));

						return 0;
					}
				}
				nExeStatus++;
				nOriginTick = GetTickCount64();
				break;
			case 2:

				//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉毆.
				if (pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir))
				{
					if (pThread->m_stParam.Home.nDir == PLUS)
						pThread->SetPosHWLimitAction(MPIActionNONE);
					else
						pThread->SetNegHWLimitAction(MPIActionNONE);

					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
					{
						if (GetTickCount64() - nOriginTick >= 15000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}

					if (!pThread->GetAmpEnable())
					{
						pThread->SetAmpEnable(TRUE);
						Sleep(50);
						if (!pThread->GetAmpEnable())
						{
							if (GetTickCount64() - nOriginTick >= 15000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}
							break;
						}
					}

					if (dStartPos == 0.0)
						dStartPos = pThread->GetActualPosition(); // 20180427 - syd

					//pThread->SetVMove(pThread->m_stParam.Home.f2ndSpd, pThread->m_stParam.Home.fAcc);
					//pThread->VMove((-pThread->m_stParam.Home.nDir));// , pThread->m_stParam.Home.fAcc);
					pThread->VMove(pThread->m_stParam.Home.f2ndSpd*(double)(-pThread->m_stParam.Home.nDir), pThread->m_stParam.Home.fAcc);

					nExeStatus++;
					nOriginTick = GetTickCount64();
					bChkState = FALSE;
				}
				else if (pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
				{
					if (-pThread->m_stParam.Home.nDir == PLUS)
						pThread->SetPosHWLimitAction(MPIActionNONE);
					else
						pThread->SetNegHWLimitAction(MPIActionNONE);

					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
					{
						if (GetTickCount64() - nOriginTick >= 15000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}

					if (!pThread->GetAmpEnable())
					{
						pThread->SetAmpEnable(TRUE);
						Sleep(50);
						if (!pThread->GetAmpEnable())
						{
							if (GetTickCount64() - nOriginTick >= 15000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}
							break;
						}
					}

					if (dStartPos == 0.0)
						dStartPos = pThread->GetActualPosition(); // 20180427 - syd

					//pThread->SetVMove(pThread->m_stParam.Home.f2ndSpd, pThread->m_stParam.Home.fAcc);
					//pThread->VMove(pThread->m_stParam.Home.nDir);// , pThread->m_stParam.Home.fAcc);
					pThread->VMove(pThread->m_stParam.Home.f2ndSpd*pThread->m_stParam.Home.nDir, pThread->m_stParam.Home.fAcc);
					nExeStatus++;
					nOriginTick = GetTickCount64();
					bChkState = FALSE;
				}
				else
				{
					pThread->SetPosHWLimitAction(MPIActionE_STOP);
					pThread->SetNegHWLimitAction(MPIActionE_STOP);


					nExeStatus = 5;
					nOriginTick = GetTickCount64();
					bChkState = TRUE;
				}
				break;
			case 3:
				if (!pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir) && !pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
				{
					dStartPos = 0.0;
					Sleep(100);
					pThread->SetEStop();
					nExeStatus++;
					nOriginTick = GetTickCount64();
				}
				else
				{
					if (pGlobalDoc->m_bMotionAlarm)
					{
						if (pThread->m_stParam.Home.fEscLen < 0.0)
							dTempVal = -1.0*pThread->m_stParam.Home.fEscLen;
						else
							dTempVal = pThread->m_stParam.Home.fEscLen;
						dCurrPos = pThread->GetActualPosition(); // 20180427 - syd
						if (dCurrPos - dStartPos > dTempVal || dStartPos - dCurrPos > dTempVal)
						{	 
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));

							return 0;
						}
					}

					Sleep(30);
					if (pThread->GetState() == MPIStateERROR)
					{
						pThread->ClearStatus();
						Sleep(100);
						nExeStatus = 2;
						nOriginTick = GetTickCount64();
					}
				}

				break;
			case 4:
				if (pThread->IsAxisDone())
				{
					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
					{
						if (GetTickCount64() - nOriginTick >= 15000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}

					pThread->SetPosHWLimitAction(MPIActionE_STOP);
					pThread->SetNegHWLimitAction(MPIActionE_STOP);
					nExeStatus++;
					nOriginTick = GetTickCount64();
				}
				else
				{
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}
				break;
			case 5:
				//2. -축을 제외한 다른위치에 모타가 있을때 .
				// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 .

				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP2_MOVE_ORIGIN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				pThread->ClearStatus();
				Sleep(100);

				if (pThread->GetState() == MPIStateERROR)
				{
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
					break;
				}

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(50);
					if (!pThread->GetAmpEnable())
					{
						if (GetTickCount64() - nOriginTick >= 30000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}
				}

				//pThread->SetVMove(pThread->m_stParam.Home.f1stSpd, pThread->m_stParam.Home.fAcc);
				//if (pThread->VMove(pThread->m_stParam.Home.nDir))// , pThread->m_stParam.Home.fAcc))
				if (pThread->VMove(pThread->m_stParam.Home.f1stSpd*pThread->m_stParam.Home.nDir, pThread->m_stParam.Home.fAcc))
				{
					nExeStatus++;
					nOriginTick = GetTickCount64();
					bChkState = FALSE;
					Sleep(10);
				}
				else
				{
					nOriginTick = GetTickCount64();
					nExeStatus = 0;
				}
				break;
			case 6:
				bMotDone = pThread->IsAxisDone();

				if (bMotDone)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						Sleep(100);
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						pThread->ClearStatus();
						Sleep(100);
						if (pThread->GetState() == MPIStateERROR)
						{
							if (GetTickCount64() - nOriginTick >= 30000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}

							break;
						}
						nExeStatus++;
						nOriginTick = GetTickCount64();
					}
					else
					{
						if (pThread->GetState() == MPIStateERROR)
						{
							Sleep(100);
							if (pThread->m_stParam.Home.nDir == PLUS)
								pThread->SetPosHWLimitAction(MPIActionNONE);
							else
								pThread->SetNegHWLimitAction(MPIActionNONE);

							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
							{
								if (GetTickCount64() - nOriginTick >= 30000)
								{
									bError = TRUE;
									pThread->SetEStop();

									//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
									//if (pParent->m_nInterlockStatus == 0)
									//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
									//pThread->m_bHomeThreadAlive = FALSE;
									//pParent->m_sMotionError = strMsg; // 20180517 - syd
									//pParent->m_nMotError = 3;
									//pParent->m_nInterlockStatus = TRUE;
									pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

									return 0;
								}
								break;
							}
							nExeStatus++;
							nOriginTick = GetTickCount64();
						}
					}
				}
				else
				{
					//2분 이내에 원점복귀가 되지 않으면 알람 처리
					if (GetTickCount64() - nOriginTick >= 1000 * 120)
					{
						nOriginTick = GetTickCount64();
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}
				break;
			case 7:
				pThread->ClearStatus();
				Sleep(100);

				if (pThread->GetState() == MPIStateERROR)
				{
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
					break;
				}

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(100);
					if (!pThread->GetAmpEnable())
					{
						if (GetTickCount64() - nOriginTick >= 30000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}
				}

				//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP3_ESCAPE_ORIGIN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉毆 .
				if (pThread->m_stParam.Home.fEscLen < 0.0)
					dTempVal = -1.0*pThread->m_stParam.Home.fEscLen;
				else
					dTempVal = pThread->m_stParam.Home.fEscLen;

				if (pThread->StartPtPMove(dTempVal*(-(double)pThread->m_stParam.Home.nDir),
					pThread->m_stParam.Home.f2ndSpd,
					pThread->m_stParam.Home.fAcc,
					pThread->m_stParam.Home.fAcc,
					INC,
					NO_WAIT))
					//OPTIMIZED,
					//S_CURVE))
				{
					Sleep(100);
					nExeStatus++;
					nOriginTick = GetTickCount64();
					bChkState = TRUE;
					dwST = GetTickCount();
				}
				else
				{
					bError = TRUE;

					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}

				break;
			case 8:
				bMotDone = pThread->IsAxisDone();
				bInPos = pThread->IsInPosition();

				if (GetTickCount() - dwST > 3000)
				{
					bMotDone = TRUE;
				}

				if (bMotDone && bInPos)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						if (pThread->GetState() == MPIStateERROR)
						{
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
							{
								if (GetTickCount64() - nOriginTick >= 30000)
								{
									bError = TRUE;
									pThread->SetEStop();

									//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
									//if (pParent->m_nInterlockStatus == 0)
									//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
									//pThread->m_bHomeThreadAlive = FALSE;
									//pParent->m_sMotionError = strMsg; // 20180517 - syd
									//pParent->m_nMotError = 3;
									//pParent->m_nInterlockStatus = TRUE;
									pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

									return 0;
								}
								break;
							}
						}
						nExeStatus = 7;
						nOriginTick = GetTickCount64();
					}
					else
					{
						Sleep(100);

						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionE_STOP);
						else
							pThread->SetNegHWLimitAction(MPIActionE_STOP);

						if (pThread->GetState() == MPIStateERROR)
						{
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
							{
								if (GetTickCount64() - nOriginTick >= 30000)
								{
									bError = TRUE;
									pThread->SetEStop();

									//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
									//if (pParent->m_nInterlockStatus == 0)
									//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
									//pThread->m_bHomeThreadAlive = FALSE;
									//pParent->m_sMotionError = strMsg; // 20180517 - syd
									//pParent->m_nMotError = 3;
									//pParent->m_nInterlockStatus = TRUE;
									pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

									return 0;
								}
								break;
							}
						}

						pThread->SetEStopRate((float)fENewStopTime);
						nExeStatus++;
						nOriginTick = GetTickCount64();
					}
				}
				else
				{
					if (GetTickCount64() - nOriginTick >= 120 * 1000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}

				break;
			case 9:
				//4. -축을 제외한 다른위치에 모타가 있을때 ..
				// -방향으로 움직이면서 일단 -Limit를 찾음 .
				// Move to the negative limit : 
				// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
				// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
				pThread->ClearStatus();
				Sleep(100);
				if (pThread->GetState() == MPIStateERROR)
				{
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
					break;
				}

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(50);
					if (!pThread->GetAmpEnable())
					{
						if (GetTickCount64() - nOriginTick >= 30000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}
				}

				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP4_ORIGIN_RETURN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				//pThread->SetVMove(fMachineSpeed, 10.0*fMachineSpeed);
				//if (pThread->VMove(pThread->m_stParam.Home.nDir))//, 10.0*fMachineSpeed))
				if (pThread->VMove(fMachineSpeed*pThread->m_stParam.Home.nDir, 10.0*fMachineSpeed))
				{
					nExeStatus++;
					bChkState = FALSE;
					nOriginTick = GetTickCount64();
				}
				else
				{
					nExeStatus = 0;
					nOriginTick = GetTickCount64();
				}
				break;
			case 10:
				bMotDone = pThread->IsAxisDone();

				if (bMotDone)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						pThread->ClearStatus();
						Sleep(100);
						if (pThread->GetState() == MPIStateERROR)
						{
							if (GetTickCount64() - nOriginTick >= 30000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}
							break;
						}

						//pThread->m_pObjectAxis[nAxisID].SetCmdPos(0.0);
						//pThread->m_pObjectAxis[nAxisID].SetActPos(0.0);
						pThread->SetPosition(0.0);
						nExeStatus++;
						nOriginTick = GetTickCount64();
					}
					else
					{
						if (pThread->GetState() == MPIStateERROR)
						{
							Sleep(100);
							if (pThread->m_stParam.Home.nDir == PLUS)
								pThread->SetPosHWLimitAction(MPIActionNONE);
							else
								pThread->SetNegHWLimitAction(MPIActionNONE);

							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
							{
								if (GetTickCount64() - nOriginTick >= 30000)
								{
									bError = TRUE;
									pThread->SetEStop();

									//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
									//if (pParent->m_nInterlockStatus == 0)
									//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
									//pThread->m_bHomeThreadAlive = FALSE;
									//pParent->m_sMotionError = strMsg; // 20180517 - syd
									//pParent->m_nMotError = 3;
									//pParent->m_nInterlockStatus = TRUE;
									pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

									return 0;
								}
								break;
							}
							//pThread->m_pObjectAxis[nAxisID].SetCmdPos(0.0);
							//pThread->m_pObjectAxis[nAxisID].SetActPos(0.0);
							pThread->SetPosition(0.0);
							nExeStatus++;
							nOriginTick = GetTickCount64();
						}
					}
				}
				else
				{
					if (GetTickCount64() - nOriginTick >= 120 * 1000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}
				break;

			case 11:
				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP5_ORIGIN_SHIFT"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				if (pThread->m_stParam.Home.fShift != 0.0)
				{
					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
					{
						if (GetTickCount64() - nOriginTick >= 30000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}

					if (!pThread->GetAmpEnable())
					{
						pThread->SetAmpEnable(TRUE);
						Sleep(50);
						if (!pThread->GetAmpEnable())
						{
							if (GetTickCount64() - nOriginTick >= 30000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}
							break;
						}
					}

					if (pThread->m_stParam.Home.fShift < 0.0)
						dTempVal = -1.0*pThread->m_stParam.Home.fShift;
					else
						dTempVal = pThread->m_stParam.Home.fShift;

					if (pThread->StartPtPMove(dTempVal*(-(double)pThread->m_stParam.Home.nDir),
						pThread->m_stParam.Home.f2ndSpd,
						pThread->m_stParam.Home.fAcc,
						pThread->m_stParam.Home.fAcc,
						INC,
						NO_WAIT))
						//OPTIMIZED,
						//S_CURVE))
					{
						Sleep(100);
						nExeStatus++;
						nOriginTick = GetTickCount64();
					}
					else
					{
						bError = TRUE;
						if (GetTickCount64() - nOriginTick >= 10000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
					}
				}
				else
				{
					nOriginTick = GetTickCount64();
					nExeStatus = 13;
				}
				bChkState = TRUE;
				break;
			case 12:
				bMotDone = pThread->IsAxisDone();
				bInPos = pThread->IsInPosition();
				if (bSixStepTrigger == 0)
				{
					strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP6_ORIGIN_OFFSET"));
					pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
					bSixStepTrigger = 1;
				}
				if (bMotDone && bInPos)
				{
					Sleep(100);
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						if (pThread->GetState() == MPIStateERROR)
						{
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
							{
								if (GetTickCount64() - nOriginTick >= 30000)
								{
									bError = TRUE;
									pThread->SetEStop();

									//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
									//if (pParent->m_nInterlockStatus == 0)
									//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
									//pThread->m_bHomeThreadAlive = FALSE;
									//pParent->m_sMotionError = strMsg; // 20180517 - syd
									//pParent->m_nMotError = 3;
									//pParent->m_nInterlockStatus = TRUE;
									pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

									return 0;
								}
								break;
							}
						}
						nExeStatus = 11;
						nOriginTick = GetTickCount64();
						bSixStepTrigger = 0;
					}
					else
					{
						bMotDone = pThread->IsAxisDone();
						bInPos = pThread->IsInPosition();

						if (bMotDone && bInPos)
						{
							Sleep(100);
							pThread->SetNegHWLimitAction(MPIActionE_STOP);
							pThread->SetPosHWLimitAction(MPIActionE_STOP);
							pThread->SetEStopRate(pThread->m_fEStopTime);
							nExeStatus++;
							bSixStepTrigger = 0;
							nOriginTick = GetTickCount64();
						}
						else
						{
							if (GetTickCount64() - nOriginTick >= 30000)
							{
								bError = TRUE;
								pThread->SetEStop();

								//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
								//if (pParent->m_nInterlockStatus == 0)
								//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
								//pThread->m_bHomeThreadAlive = FALSE;
								//pParent->m_sMotionError = strMsg; // 20180517 - syd
								//pParent->m_nMotError = 3;
								//pParent->m_nInterlockStatus = TRUE;
								pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

								return 0;
							}
						}
					}
				}
				else
				{
					if (GetTickCount64() - nOriginTick >= 60000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}
				break;
			case 13:
				//pThread->SetErrorLimitAction(MPIActionNONE);
				Sleep(30);
				if (abs(pThread->GetCommandPosition() - pThread->m_stParam.Home.fOffset) >= 1.0)
					pThread->SetPosition(pThread->m_stParam.Home.fOffset);

				while (abs(pThread->GetCommandPosition() - pThread->m_stParam.Home.fOffset) >= 1.0)
				{
					Sleep(30);
					pThread->SetPosition(pThread->m_stParam.Home.fOffset);

					if (GetTickCount64() - nOriginTick >= 60000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}

				if (abs(pThread->GetActualPosition() - pThread->m_stParam.Home.fOffset) >= 1.0)
					pThread->SetPosition(pThread->m_stParam.Home.fOffset);

				while (abs(pThread->GetActualPosition() - pThread->m_stParam.Home.fOffset) >= 1.0)
				{
					Sleep(30);
					pThread->SetPosition(pThread->m_stParam.Home.fOffset);
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
				}
				//pThread->SetErrorLimitAction(MPIActionABORT);
				nExeStatus++;
				nOriginTick = GetTickCount64();
				break;
			case 14:
				pThread->SetPosSoftwareLimit(fPrevPosLimit, MPIActionE_STOP);
				pThread->SetNegSoftwareLimit(fPrevNegLimit, MPIActionE_STOP);
				pThread->AmpFaultReset();
				pThread->SetOriginPos();

				//if (nMotorID == FOCUS_AXIS)
				//{
				//	if (pThread->StartPtPMove(-30,
				//		pThread->m_stParam.Home.f2ndSpd,
				//		pThread->m_stParam.Home.fAcc,
				//		pThread->m_stParam.Home.fAcc,
				//		ABS,
				//		WAIT))
				//		//OPTIMIZED,
				//		//S_CURVE))
				//	{

				//	}
				//}

				nExeStatus++;
				nOriginTick = GetTickCount64();
				break;
			case 15:
				pThread->ClearStatus();
				Sleep(100);

				if (pThread->GetState() != MPIStateIDLE && pThread->GetState() != MPIStateSTOPPED)
				{
					if (GetTickCount64() - nOriginTick >= 30000)
					{
						bError = TRUE;
						pThread->SetEStop();

						//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
						//if (pParent->m_nInterlockStatus == 0)
						//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 3;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

						return 0;
					}
					break;
				}

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(50);
					if (!pThread->GetAmpEnable())
					{
						if (GetTickCount64() - nOriginTick >= 30000)
						{
							bError = TRUE;
							pThread->SetEStop();

							//strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("Amp Fault Detected.. please re try Machine Ready"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("Amp Fault Detected.. please retry Machine Ready"));

							return 0;
						}
						break;
					}
				}

				//if (pThread->m_stParam.Motor.bType == STEPPER)
				//	pThread->m_pObjectMotor[nMotorID].SetErrorLimitAction(MPIActionABORT);



				nExeStatus++;
				nOriginTick = GetTickCount64();
				break;

			case 16:
				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_ORIGIN_RETURN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);		//20110114 hjc mod
				pThread->m_bOrigin = TRUE;

				break;
			}

#ifdef THETA_AXIS
			if (bClampError || pParent->m_nInterlockStatus || (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_pIO->CheckSaftyOnlyBit()) || pGlobalView->m_pIO->CheckEmgSwitch() || pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage() == 0)
			{
#else
			if (bClampError || pParent->m_nInterlockStatus || (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_pIO->CheckSaftyOnlyBit()) || pGlobalView->m_pIO->CheckEmgSwitch() || pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage() == 0)
			{
#endif

#if CUSTOMER_COMPANY != SUMITOMO	//120822 hjc add
				pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);	// 20130726 ljh
#else
				pGlobalView->TowerLampControl(TOWER_LAMP_YELLOW, ON);	// 20130726 ljh
#endif
				pGlobalView->m_pIO->BuzzerOnOff(ON);

				pThread->m_bOrigin = FALSE;

				pThread->SetEStop();

				if (AoiParam()->m_bRTRDoorInterlock)
				{
					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG564] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG8");

					pParent->m_nInterlockType = FRONT_SIDE_DOOR_SENSOR;
				}

				if ((pGlobalDoc->m_bUseSaftyAreaSensor && pGlobalView->m_pIO->CheckSaftyOnlyBit()))
				{
					pParent->m_nInterlockType = SAFETY_AREA_SENSOR;
					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG568] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG14");
				}

				if (pGlobalDoc->m_bEmergencyStatus)
				{
					pParent->m_nInterlockType = EMERGENCY_STOP;

					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG534] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG2");
				}

				if (!pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage())
				{
					pParent->m_nInterlockType = DOOR_OPEN;
					pGlobalView->m_pIO->CheckIBIDoorStatus(strMsg);
				}

				if (strMsg.IsEmpty())
				{
					if (pParent->m_nMotError == 1)
					{
						strMsg.Format(_T("Motor_%s : %s"), strAxisName, _T("Pos and Neg Limit Sensor Error!!! Please restart the AOI"));
					}
					else  if (pParent->m_nMotError == 2)
					{
						strMsg.Format(_T("Axis #%s %s"), strAxisName, _T("Failed that Motor servo On."));
					}
					else if (pParent->m_nMotError == 3)
					{
						strMsg.Format(_T("Error - Motor_%s %s"), strAxisName, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));
					}
					else if (pParent->m_nMotError == 4)
					{
						strMsg.Format(_T("%s %s"), strAxisName, _T("SearchIndex VMove Error!!! Please re start the AOI"));
					}
				}

				if (strMsg.IsEmpty())
					strMsg.Format(_T("%s"), _T("Interlock Status!!! Please re start the AOI"));

				if (pParent->m_nInterlockStatus == 0)
					pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);

				pThread->m_bHomeThreadAlive = FALSE;

				pParent->m_nInterlockStatus = TRUE;
				return 0;;
			}

		}
	}
/*	else if (pThread->m_stParam.Home.bIndex == 1)
	{
		while (!pThread->m_bOrigin && WAIT_OBJECT_0 != ::WaitForSingleObject(pThread->m_ThreadTask.GetShutdownEvent(), dwTimePeriod))
		{

			if (pGlobalDoc->m_bMotionAlarm)
			{
				if (pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir) && pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
				{	 // 20180427 - syd
					bError = TRUE;
					pThread->SetPosSWLimitAction(MPIActionE_STOP);
					pThread->SetNegSWLimitAction(MPIActionE_STOP);
					pThread->SetPosHWLimitAction(MPIActionE_STOP);
					pThread->SetNegHWLimitAction(MPIActionE_STOP);

					//strMsg.Format(_T("Motor_%d : %s"), nMotorID, _T("Pos and Neg Limit Sensor Error!!! Please restart the AOI"));
					//if (pParent->m_nInterlockStatus == 0)
					//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
					//pThread->m_bHomeThreadAlive = FALSE;
					//pParent->m_sMotionError = strMsg; // 20180517 - syd
					//pParent->m_nMotError = 1;
					//pParent->m_nInterlockStatus = TRUE;
					pThread->SetAlarmCall(lpContext, 1, _T("Pos and Neg Limit Sensor Error!!! Please restart the AOI"));

					return 0;
				}
			}

			BOOL bClampError = 0;
			if (AoiParam()->m_bUseTableClamp)
			{
				bClampError = GetClampObject()->ClampStatusError();
			}

			Sleep(10);
			switch (nExeStatus)
			{
			case 0:
				pThread->m_bOrigin = FALSE;
				if (pThread->IsAxisDone())
				{
					// software limit value 
					pThread->m_pObjectMotor[nMotorID].ChangePosSWLimitValue(1000.0);
					pThread->m_pObjectMotor[nMotorID].ChangeNegSWLimitValue(-1000.0);

					pThread->m_pObjectMotor[nMotorID].SetPosSWLimitAction(MPIActionNONE);
					pThread->m_pObjectMotor[nMotorID].SetNegSWLimitAction(MPIActionNONE);

					if (pThread->m_stParam.Motor.bType == STEPPER) // 20180413-syd : Modified for Fastech synqnet servo pack.
					{
						//pThread->m_pObjectMotor[nMotorID].SetErrorLimitAction(MPIActionNONE);
						Sleep(30);
						//pThread->m_pObjectAxis[nAxisID].SetCmdPos(0.0);
						//pThread->m_pObjectAxis[nAxisID].SetActPos(0.0);
						pThread->SetPosition(0.0);
						Sleep(30);
						pThread->ClearStatus();
						Sleep(30);
						pThread->SetAmpEnable(TRUE);
						Sleep(30);
					}
					nExeStatus++;
				}
				break;
			case 1:
				if (pGlobalDoc->m_bMotionAlarm)
				{
					if (pThread->GetState() == MPIStateERROR || !pThread->GetAmpEnable()) // 20180413-syd : Modified for Fastech synqnet servo pack.
					{
						//strMsg.Format(_T("Axis #%d %s"), nAxisID, _T("Failed that Motor servo On."));
						//pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_sMotionError = strMsg; // 20180517 - syd
						//pParent->m_nMotError = 2;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 2, _T("Failed that Motor servo On."));

						return 0;
						//if (!pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir) && !pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
						//	break;
					}
				}
				nExeStatus++;
				break;
			case 2:

				//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉毆.

				if (pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir))
				{
					if (pThread->m_stParam.Home.nDir == PLUS)
						pThread->SetPosHWLimitAction(MPIActionNONE);
					else
						pThread->SetNegHWLimitAction(MPIActionNONE);

					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
						break;

					if (!pThread->GetAmpEnable())
					{
						pThread->SetAmpEnable(TRUE);
						Sleep(50);
						if (!pThread->GetAmpEnable())
							break;
					}

					if (dStartPos == 0.0)
						dStartPos = pThread->GetActualPosition(); // 20180427 - syd

					pThread->VMove(pThread->m_stParam.Home.f2ndSpd*(-(double)pThread->m_stParam.Home.nDir), pThread->m_stParam.Home.fAcc);
					nExeStatus++;
					bChkState = FALSE;
				}
				else if (pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
				{
					if (-pThread->m_stParam.Home.nDir == PLUS)
						pThread->SetPosHWLimitAction(MPIActionNONE);
					else
						pThread->SetNegHWLimitAction(MPIActionNONE);

					pThread->ClearStatus();
					Sleep(100);

					if (pThread->GetState() == MPIStateERROR)
						break;

					if (!pThread->GetAmpEnable())
					{
						pThread->SetAmpEnable(TRUE);
						Sleep(50);
						if (!pThread->GetAmpEnable())
							break;
					}

					if (dStartPos == 0.0)
						dStartPos = pThread->GetActualPosition(); // 20180427 - syd

					pThread->VMove(pThread->m_stParam.Home.f2ndSpd*(double)pThread->m_stParam.Home.nDir, pThread->m_stParam.Home.fAcc);
					nExeStatus++;
					bChkState = FALSE;
				}
				else
				{
					pThread->SetPosHWLimitAction(MPIActionE_STOP);
					pThread->SetNegHWLimitAction(MPIActionE_STOP);
					nExeStatus = 5;
					bChkState = TRUE;
				}
				break;
			case 3: //20100507-syd
				if (!pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir) && !pThread->CheckLimitSwitch(-pThread->m_stParam.Home.nDir))
				{
					dStartPos = 0.0;
					Sleep(100);
					pThread->SetEStop();
					nExeStatus++;
				}
				else
				{
					if (pGlobalDoc->m_bMotionAlarm)
					{
						if (pThread->m_stParam.Home.fEscLen < 0.0)
							dTempVal = -1.0*pThread->m_stParam.Home.fEscLen;
						else
							dTempVal = pThread->m_stParam.Home.fEscLen;
						dCurrPos = pThread->GetActualPosition(); // 20180427 - syd
						if (dCurrPos - dStartPos > dTempVal || dStartPos - dCurrPos > dTempVal)
						{	 // 20180427 - syd
							bError = TRUE;
							pThread->SetEStop();

							//if (strMsg.IsEmpty())
							//	strMsg.Format(_T("Error - Motor_%d %s"), nMotorID, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));
							//if (pParent->m_nInterlockStatus == 0)
							//	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
							//pThread->m_bHomeThreadAlive = FALSE;
							//pParent->m_sMotionError = strMsg; // 20180517 - syd
							//pParent->m_nMotError = 3;
							//pParent->m_nInterlockStatus = TRUE;
							pThread->SetAlarmCall(lpContext, 3, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));

							return 0;;
						}
					}

					Sleep(30);
					if (pThread->GetState() == MPIStateERROR)
					{
						pThread->ClearStatus();
						Sleep(100);
						nExeStatus = 2;
					}
				}

				break;
			case 4:
				if (pThread->IsAxisDone())
				{
					pThread->ClearStatus();
					Sleep(100);
					if (pThread->GetState() == MPIStateERROR)
						break;

					pThread->SetPosHWLimitAction(MPIActionE_STOP);
					pThread->SetNegHWLimitAction(MPIActionE_STOP);
					nExeStatus++;
				}
				break;
			case 5:
				//2. -축을 제외한 다른위치에 모타가 있을때 .
				// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 .

				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP2_MOVE_ORIGIN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				pThread->ClearStatus();
				Sleep(100);
				if (pThread->GetState() == MPIStateERROR)
					break;

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(50);
					if (!pThread->GetAmpEnable())
						break;
				}

				if (pThread->VMove(pThread->m_stParam.Home.f1stSpd*(double)pThread->m_stParam.Home.nDir, pThread->m_stParam.Home.fAcc))
				{
					nExeStatus++;
					bChkState = FALSE;
				}
				else
				{
					nExeStatus = 0;
				}
				break;
			case 6:
				bMotDone = pThread->IsAxisDone();

				if (bMotDone)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						Sleep(100);
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						pThread->ClearStatus();
						Sleep(100);
						if (pThread->GetState() == MPIStateERROR)
							break;
						nExeStatus++;
					}
					else
					{
						if (pThread->GetState() == MPIStateERROR)
						{
							Sleep(100);
							if (pThread->m_stParam.Home.nDir == PLUS)
								pThread->SetPosHWLimitAction(MPIActionNONE);
							else
								pThread->SetNegHWLimitAction(MPIActionNONE);
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
								break;
							nExeStatus++;
						}
					}
				}
				break;
			case 7:
				pThread->ClearStatus();
				Sleep(100);
				if (pThread->GetState() == MPIStateERROR)
					break;

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(100);
					if (!pThread->GetAmpEnable())
						break;
				}

				//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP3_ESCAPE_ORIGIN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉毆 .
				if (pThread->m_stParam.Home.fEscLen < 0.0)
					dTempVal = -1.0*pThread->m_stParam.Home.fEscLen;
				else
					dTempVal = pThread->m_stParam.Home.fEscLen;

				if (pThread->Move(dTempVal*(-(double)pThread->m_stParam.Home.nDir),
					pThread->m_stParam.Home.f2ndSpd,
					pThread->m_stParam.Home.fAcc,
					pThread->m_stParam.Home.fAcc,
					INC,
					NO_WAIT,
					OPTIMIZED,
					S_CURVE))
				{
					Sleep(100);
					nExeStatus++;
					bChkState = TRUE;
					dwST = GetTickCount();
				}
				else
					bError = TRUE;
				break;
			case 8:
				bMotDone = pThread->IsAxisDone();
				bInPos = pThread->IsInPosition();

				if (GetTickCount() - dwST > 3000)
				{
					bMotDone = TRUE;
				}

				if (bMotDone && bInPos)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						if (pThread->GetState() == MPIStateERROR)
						{
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
								break;
						}
						nExeStatus = 7;
					}
					else
					{
						Sleep(100);
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionE_STOP);
						else
							pThread->SetNegHWLimitAction(MPIActionE_STOP);

						if (pThread->GetState() == MPIStateERROR)
						{
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
								break;
						}

						pThread->SetEStopRate((float)fENewStopTime);
						nExeStatus++;
					}
				}

				break;
			case 9:
				//4. -축을 제외한 다른위치에 모타가 있을때 ..
				// -방향으로 움직이면서 일단 -Limit를 찾음 .
				// Move to the negative limit : 
				// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
				// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
				pThread->ClearStatus();
				Sleep(100);
				if (pThread->GetState() == MPIStateERROR)
					break;

				if (!pThread->GetAmpEnable())
				{
					pThread->SetAmpEnable(TRUE);
					Sleep(50);
					if (!pThread->GetAmpEnable())
						break;
				}

				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP4_ORIGIN_RETURN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

				if (pThread->VMove(fMachineSpeed*(double)pThread->m_stParam.Home.nDir, 10.0*fMachineSpeed))
				{
					nExeStatus++;
					bChkState = FALSE;
				}
				else
				{
					nExeStatus = 0;
				}
				break;
			case 10:
				bMotDone = pThread->IsAxisDone();

				if (bMotDone)
				{
					bLimitEvent = pThread->CheckLimitSwitch(pThread->m_stParam.Home.nDir);

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == PLUS)
							pThread->SetPosHWLimitAction(MPIActionNONE);
						else
							pThread->SetNegHWLimitAction(MPIActionNONE);

						pThread->ClearStatus();
						Sleep(100);
						if (pThread->GetState() == MPIStateERROR)
							break;

						//pThread->m_pObjectAxis[nAxisID].SetCmdPos(0.0);
						//pThread->m_pObjectAxis[nAxisID].SetActPos(0.0);
						pThread->SetPosition(0.0);
						nExeStatus++;
					}
					else
					{
						if (pThread->GetState() == MPIStateERROR)
						{
							Sleep(100);
							if (pThread->m_stParam.Home.nDir == PLUS)
								pThread->SetPosHWLimitAction(MPIActionNONE);
							else
								pThread->SetNegHWLimitAction(MPIActionNONE);
							pThread->ClearStatus();
							Sleep(100);
							if (pThread->GetState() == MPIStateERROR)
								break;

							//pThread->m_pObjectAxis[nAxisID].SetCmdPos(0.0);
							//pThread->m_pObjectAxis[nAxisID].SetActPos(0.0);
							pThread->SetPosition(0.0);
							nExeStatus++;
						}
					}
				}
				break;

			case 11:
				pThread->SetIndexConfig();
				nExeStatus++;
				break;
			case 12:
				pThread->EnableIndex(TRUE);
				nExeStatus++;
				break;
			case 13:
				if (1)
				{
					double dIndexHomeSpeed = pThread->m_stParam.Home.f2ndSpd;

					double dMotionResolution = ((CZmpControl*)pThread->m_pParent)->m_pParamMotor[nMotorID].fLeadPitch / ((CZmpControl*)pThread->m_pParent)->m_pParamMotor[nMotorID].nEncPulse;

					dIndexHomeSpeed = pThread->GetIndexHomeVelocity(0.0005, dMotionResolution * 1000);

					dIndexHomeSpeed = g_max(0, g_min(pThread->m_stParam.Home.f1stSpd, dIndexHomeSpeed));

					strMsg.Format(_T("Index Home Speed : %.3f axis:%s"), dIndexHomeSpeed, pThread->m_pObjectAxis[nMotorID].m_AxisParam.sName);
					pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

					pThread->ClearStatus();
					Sleep(100);

					if (pThread->m_stParam.Home.nDir == 1)
						bLimitEvent = pThread->m_pObjectAxis[nAxisID].IsPosHWLimit();
					else
						bLimitEvent = pThread->m_pObjectAxis[nAxisID].IsNegHWLimit();

					if (bLimitEvent)
					{
						if (pThread->m_stParam.Home.nDir == 1)
						{
							pThread->m_pObjectMotor[nAxisID].SetPosSWLimitAction(MPIActionNONE);
							pThread->m_pObjectMotor[nAxisID].SetPosHWLimitAction(MPIActionNONE);
						}
						else
						{
							pThread->m_pObjectMotor[nAxisID].SetNegSWLimitAction(MPIActionNONE);
							pThread->m_pObjectMotor[nAxisID].SetNegHWLimitAction(MPIActionNONE);
						}
					}

					Sleep(100);

					if (pThread->VMove(dIndexHomeSpeed*(-(double)pThread->m_stParam.Home.nDir), 10.0*dIndexHomeSpeed))
					{
						BOOL bSearchSuccess = 0;
						bSearchSuccess = pThread->SearchIndex();

						pThread->EnableIndex(FALSE);

						nExeStatus = 14;
						bChkState = FALSE;
					}
					else
					{
						bError = TRUE;
						bChkState = TRUE;

						//strMsg.Format(_T("%s"), _T("SearchIndex VMove Error!!! Please re start the AOI"));
						//pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);
						//pThread->m_bHomeThreadAlive = FALSE;
						//pParent->m_nMotError = 4;
						//pParent->m_nInterlockStatus = TRUE;
						pThread->SetAlarmCall(lpContext, 4, _T("SearchIndex VMove Error!!! Please re start the AOI"));

						return 0;
					}
				}
				break;
			case 14:

				if (pThread->Move(0.0,
					pThread->m_stParam.Home.f1stSpd,
					pThread->m_stParam.Home.fAcc,
					pThread->m_stParam.Home.fAcc,
					30.0,
					INC,
					WAIT,
					OPTIMIZED
				))//S_CURVE
				{
					Sleep(100);
					bChkState = TRUE;
				}

				bChkState = TRUE;

				nExeStatus = 15;
				break;


			case 15:
				pThread->ChangePosSWLimitValue(fPrevPosLimit);
				pThread->ChangeNegSWLimitValue(fPrevNegLimit);

				pThread->SetPosSWLimitAction(MPIActionE_STOP);
				pThread->SetNegSWLimitAction(MPIActionE_STOP);
				pThread->SetPosHWLimitAction(MPIActionE_STOP);
				pThread->SetNegHWLimitAction(MPIActionE_STOP);
				pThread->SetMSAction(MPIActionRESET);
				pThread->SetOriginPos();
				nExeStatus++;
				break;
			case 16:
				pThread->ClearStatus();
				Sleep(100);
				nExeStatus++;
				break;
			case 17:
				strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_ORIGIN_RETURN"));
				pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
				pThread->m_bOrigin = TRUE;
				pParent->m_bOrigin[nAxisID] = TRUE;
				break;
			}

			BOOL bInterlockShinko = 0;

			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController->m_state == ECS_EQ_DOWN)
				{
					bInterlockShinko = 1;
				}
			}

#ifdef THETA_AXIS
			if (bClampError || bInterlockShinko || pParent->m_nInterlockStatus || (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_pIO->CheckSaftyOnlyBit()) || pGlobalView->m_pIO->CheckEmgSwitch() || pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage() == 0)
			{
#else
			if (bClampError || bInterlockShinko || pParent->m_nInterlockStatus || (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_pIO->CheckSaftyOnlyBit()) || pGlobalView->m_pIO->CheckEmgSwitch() || pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage() == 0)
			{
#endif

#if CUSTOMER_COMPANY != SUMITOMO	//120822 hjc add
				pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);	// 20130726 ljh
#else
				pGlobalView->TowerLampControl(TOWER_LAMP_YELLOW, ON);	// 20130726 ljh
#endif
				pGlobalView->m_pIO->BuzzerOnOff(ON);

				pThread->m_bOrigin = FALSE;

				pThread->SetEStop();

				if (AoiParam()->m_bRTRDoorInterlock)
				{
					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG564] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG8");

					pParent->m_nInterlockType = FRONT_SIDE_DOOR_SENSOR;
				}

				if ((pGlobalDoc->m_bUseSaftyAreaSensor && pGlobalView->m_pIO->CheckSaftyOnlyBit()))
				{

					pParent->m_nInterlockType = SAFETY_AREA_SENSOR;
					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG568] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG14");
				}

				if (pGlobalDoc->m_bEmergencyStatus)
				{
					pParent->m_nInterlockType = EMERGENCY_STOP;

					if (pGlobalView->GetSafeHwnd())
						strMsg = _T("[MSG534] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "SAFETY SENSOR", "MSG2");
				}

				if (!pGlobalView->m_pIO->CheckIBIDoorStatusNoMessage())
				{
					pParent->m_nInterlockType = DOOR_OPEN;
					pGlobalView->m_pIO->CheckIBIDoorStatus(strMsg);
				}

				if (strMsg.IsEmpty())
				{
					if (pParent->m_nMotError == 1)
					{
						strMsg.Format(_T("Motor_%s : %s"), strAxisName, _T("Pos and Neg Limit Sensor Error!!! Please restart the AOI"));
					}
					else  if (pParent->m_nMotError == 2)
					{
						strMsg.Format(_T("Axis #%s %s"), strAxisName, _T("Failed that Motor servo On."));
					}
					else if (pParent->m_nMotError == 3)
					{
						strMsg.Format(_T("Error - Motor_%s %s"), strAxisName, _T("can't escape Limit Sensor!\r\nPlease restart the AOI"));
					}
					else if (pParent->m_nMotError == 4)
					{
						strMsg.Format(_T("%s %s"), strAxisName, _T("SearchIndex VMove Error!!! Please re start the AOI"));
					}
				}

				if (strMsg.IsEmpty())
					strMsg.Format(_T("%s"), _T("Interlock Status!!! Please re start the AOI"));

				if (pParent->m_nInterlockStatus == 0)
					pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_RED, 3000);

				pThread->m_bHomeThreadAlive = FALSE;
				pParent->
				m_nInterlockStatus = TRUE;

				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);
				}

				return 0;
			}
		}
	}*/

	pThread->m_bHomeThreadAlive = FALSE;

	return 0;
}


BOOL CNmcAxis::StartHomming()
{
	CString strTitleMsg;
	CString strMsg;

	strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "MOTOR_POSITION_INITIALIZE"));
	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	if (!HomeReady())
		return FALSE;

	// Step1 Check! Limit Sensor Activated and then Escape the position	
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP1_ESCAPE_ORIGIN"));
	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
	if (!EscapeHomeDirLimitSens())
		return FALSE;

	// Step2 Move to Limit Sensor Position
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP2_MOVE_ORIGIN #1"));
	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
	if (!FirstMoveToHomeDirSens())
		return FALSE;

	//Sleep(100);
	//SetPosition(0.0);

	// Step3 Escape from Limit Sensor Position
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP3_ESCAPE_ORIGIN #1"));
	if (!FirstMoveToShiftPos())
		return FALSE;

	// Step4 Home Search by Limit Sensor Touch
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP4_MOVE_ORIGIN #2"));
	if (!SecondMoveToHomeDirSens())
		return FALSE;

	Sleep(300);
	SetPosition(0.0);
	Sleep(300);

	// Step5 Home Shift by Specified Position
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP5_ESCAPE_ORIGIN #2"));
	if (!SecondMoveToShiftPos())
		return FALSE;

	// Step6 Set Zero Position
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP6_SET_ORIGIN"));
	Sleep(300);
	SetOriginPos();
	Sleep(300);

	FinalizeHome();

	// Step7 Origin Return Finished
	return TRUE;
}

BOOL CNmcAxis::HomeReady()
{
	int nRoof = 0;

	CmdBufferClear();
	Sleep(30);
	AmpFaultReset();
	Sleep(30);

	if (!GetAmpEnable())
	{
		for (nRoof = 0; nRoof < 10; nRoof++)
		{
			SetAmpEnable(TRUE);
			Sleep(30);
			if (GetAmpEnable())
				break;
		}
		if (!GetAmpEnable())
			return FALSE;
	}
	Sleep(30);
	SetCommandPosition(GetActualPosition());
	Sleep(30);

	EnableSwLimit(FALSE);
	Sleep(30);
	ClearStatus();
	Sleep(30);

	return TRUE;
}

BOOL CNmcAxis::EscapeHomeDirLimitSens()
{
	ULONGLONG nOriginTick = GetTickCount64();
	MSG message;

	if (CheckLimitSwitch(m_stParam.Home.nDir))
	{
		SetNegHWLimitAction(NO_EVENT);
		Sleep(30);
		SetPosHWLimitAction(NO_EVENT);
		Sleep(30);

		EnableSwLimit(FALSE);
		Sleep(30);
		SetCommandPosition(GetActualPosition());
		Sleep(30);

		AmpFaultReset();
		Sleep(30);
		ClearStatus();
		Sleep(30);

		if (!StartVelocityMove(m_stParam.Home.f1stSpd*(-m_stParam.Home.nDir), m_stParam.Home.fAcc))
		{
			Sleep(100);
			if (IsAxisDone())
			{
				Sleep(30);
				double dPos = GetActualPosition();
				Sleep(30);
				SetCommandPosition(dPos);
				Sleep(30);

				AmpFaultReset();
				Sleep(30);
				ClearStatus();
				Sleep(30);

				if (!StartVelocityMove(m_stParam.Home.f1stSpd*(-m_stParam.Home.nDir), m_stParam.Home.fAcc))
				{
					Sleep(30);
					SetMainMessage(_T("SearchHomePos Fail : StartVelocityMove Fail axisdone"));
					return FALSE;
				}
			}
			else
			{
				SetMainMessage(_T("SearchHomePos Fail : StartVelocityMove Fail"));
				return FALSE;
			}
		}
		Sleep(30);
		EnableSwLimit(FALSE);
		Sleep(30);
		int nRet = 0;
		while (CheckLimitSwitch(m_stParam.Home.nDir))
		{
			if (g_bAlarmStatus)
			{
				StopVelocityMove();
				return FALSE;
			}

			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}

			// 계속 -방향으로 이동한다.
			if (CheckEmgSwitch()) //EMERGENCY_SWITCH
			{
				StopVelocityMove();
				return FALSE;
			}

			int nState = CheckAxisState();

			if (CheckAmpFaultSwitch())
			{
				SetAmpEnable(FALSE);
				Sleep(30);
				AmpFaultReset();
				Sleep(30);
				ClearStatus();
				Sleep(30);
				SetAmpEnable(TRUE);
				Sleep(30);

				if (GetAmpEnable() == 0)
				{
					SetAmpEnable(1);
				}
				SetCommandPosition(GetActualPosition());

				AmpFaultReset();
				Sleep(30);
				ClearStatus();
				Sleep(30);
				if (!StartVelocityMove(m_stParam.Home.f1stSpd*(-m_stParam.Home.nDir), m_stParam.Home.fAcc))
				{
					if (IsAxisDone())
					{
						double dPos = GetActualPosition();
						SetCommandPosition(dPos);

						AmpFaultReset();
						Sleep(30);
						ClearStatus();
						Sleep(30);
						if (!StartVelocityMove(m_stParam.Home.f1stSpd*(-m_stParam.Home.nDir), m_stParam.Home.fAcc))
						{

						}
					}
					else
					{

					}
				}

				nRet++;
			}

			if (nRet > 3)
			{
				StopVelocityMove();
				return FALSE;
			}

			if (GetTickCount64() - nOriginTick >= 60000)
			{
				StopVelocityMove();
				return FALSE;
			}
		}
		Sleep(30);
		StopVelocityMove();
		Sleep(30);
	}

	ClearStatus();
	Sleep(30);

	if (!WaitUntilMotionDone(TEN_SECOND))
	{
		return FALSE;
	}

	if (!GetAmpEnable())
	{
		SetAmpEnable(TRUE);
		Sleep(30);
	}

	return TRUE;
}

BOOL CNmcAxis::FirstMoveToHomeDirSens()
{
	ULONGLONG nOriginTick;
	MSG message;

	SetNegHWLimitAction(E_STOP_EVENT);
	SetPosHWLimitAction(E_STOP_EVENT);
	EnableSwLimit(FALSE);
	//SetHomeAction(E_STOP_EVENT);
	ClearStatus();

	EnableSwLimit(FALSE);
	AmpFaultReset();
	ClearStatus();
	EnableSwLimit(FALSE);

	if (!StartVelocityMove(m_stParam.Home.f1stSpd*m_stParam.Home.nDir, m_stParam.Home.fAcc))
	{
		double dPos = GetActualPosition();
		SetCommandPosition(dPos);

		AmpFaultReset();
		ClearStatus();

		if (!StartVelocityMove(m_stParam.Home.f1stSpd*m_stParam.Home.nDir, m_stParam.Home.fAcc))
		{
			return FALSE;
		}
	}

	if (!WaitUntilAxisDone(TEN_SECOND * 6))
	{
		return FALSE;
	}

	AmpFaultReset();
	ClearStatus();

	nOriginTick = GetTickCount64();
	while (!CheckLimitSwitch(m_stParam.Home.nDir))
	{

		if (CheckEmgSwitch())
		{
			StopVelocityMove();
			return FALSE;
		}

		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		// 계속 -방향으로 이동한다.

		if (CheckEmgSwitch())
		{
			return FALSE;
		}

		if (GetTickCount64() - nOriginTick >= 120000)
		{
			return FALSE;
		}

		if (CheckAmpFaultSwitch())
		{
			StopVelocityMove();
			AmpFaultReset();
			ClearStatus();
			return FALSE;
		}
	}
	StopVelocityMove();

	AmpFaultReset();
	ClearStatus();

	if (!GetAmpEnable())
	{
		SetAmpEnable(TRUE);
		Sleep(100);
	}

	WaitUntilMotionDone(TEN_SECOND);

	return TRUE;
}

BOOL CNmcAxis::FirstMoveToShiftPos()
{
	ULONGLONG nOriginTick;
	MSG message;

	SetNegHWLimitAction(NO_EVENT);
	SetPosHWLimitAction(NO_EVENT);
	EnableSwLimit(FALSE);
	AmpFaultReset();
	ClearStatus();

	StartPtPMotion(m_stParam.Home.fShift, m_stParam.Home.f2ndSpd, m_stParam.Home.fAcc, m_stParam.Home.fAcc, INC, NO_WAIT);
	nOriginTick = GetTickCount64();
	while (!CheckMotionDone())
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		// 해제후 계속 -방향으로 이동한다.
		if (CheckEmgSwitch())
		{
			return FALSE;
		}

		if (GetTickCount64() - nOriginTick >= 120000)
		{
			return FALSE;
		}
	}

	if (!GetAmpEnable())
	{
		SetAmpEnable(TRUE);
		Sleep(100);
	}

	ClearStatus();
	EnableSwLimit(FALSE);

	return TRUE;
}

BOOL CNmcAxis::SecondMoveToHomeDirSens()
{
	ULONGLONG nOriginTick;
	MSG message;

	SetNegHWLimitAction(E_STOP_EVENT);
	SetPosHWLimitAction(E_STOP_EVENT);
	AmpFaultReset();
	ClearStatus();
	EnableSwLimit(FALSE);

	if (!StartVelocityMove(m_stParam.Home.f2ndSpd*m_stParam.Home.nDir, m_stParam.Home.fAcc))
	{
		if (IsAxisDone())
		{
			double dPos = GetActualPosition();
			SetCommandPosition(dPos);

			AmpFaultReset();
			ClearStatus();

			if (!StartVelocityMove(m_stParam.Home.f2ndSpd*m_stParam.Home.nDir, m_stParam.Home.fAcc))
			{
				SetMainMessage(_T("SearchHomePos Fail : StartVelocityMove Fail axisdone f2ndSpd"));
				return FALSE;
			}
		}
		else
		{
			SetMainMessage(_T("SearchHomePos Fail : StartVelocityMove Fail f2ndSpd"));
			return FALSE;
		}
	}

	AmpFaultReset();
	ClearStatus();
	nOriginTick = GetTickCount64();
	while (!CheckLimitSwitch(m_stParam.Home.nDir))
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		// 해제후 계속 -방향으로 이동한다.
		if (CheckEmgSwitch())
		{
			return FALSE;
		}

		if (GetTickCount64() - nOriginTick >= 60000)
		{
			return FALSE;
		}

		int nState = CheckAxisState();
		if (nState == ABORT_EVENT)
		{
			StopVelocityMove();
			AmpFaultReset();
			ClearStatus();
			return FALSE;
		}
	}
	StopVelocityMove();

	if (!GetAmpEnable())
	{
		SetAmpEnable(TRUE);
		Sleep(100);
	}

	return TRUE;
}

BOOL CNmcAxis::SecondMoveToShiftPos()
{
	ULONGLONG nOriginTick;
	MSG message;

	SetNegHWLimitAction(NO_EVENT);//E_STOP_EVENT);
	SetPosHWLimitAction(NO_EVENT);//E_STOP_EVENT);
	ClearStatus();   // Event 해제
	EnableSwLimit(FALSE);
	if (m_stParam.Home.fShift != 0.0)
	{
		if (!GetAmpEnable())
		{
			SetAmpEnable(TRUE);
			Sleep(100);
		}

		WaitUntilMotionDone(TEN_SECOND);
		Sleep(100);

		AmpFaultReset();
		ClearStatus();

		if (!StartPtPMotion(m_stParam.Home.fShift, m_stParam.Home.f2ndSpd, m_stParam.Home.fAcc, m_stParam.Home.fAcc, INC, WAIT))
		{
			ClearStatus();
			SetAmpEnable(FALSE);
			SetAmpEnable(TRUE);
			ClearStatus();

			WaitUntilMotionDone(TEN_SECOND);
			Sleep(100);

			AmpFaultReset();
			ClearStatus();

			if (!StartPtPMotion(m_stParam.Home.fShift, m_stParam.Home.f2ndSpd, m_stParam.Home.fAcc, m_stParam.Home.fAcc, ABS, WAIT))
			{
				return FALSE;
			}
		}
		nOriginTick = GetTickCount64();
		while (!IsAxisDone())
		{
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}

			//  -방향으로 이동한다.

			if (CheckEmgSwitch())
			{
				return FALSE;
			}

			if (GetTickCount64() - nOriginTick >= 60000)
			{
				return FALSE;
			}

			int nState = CheckAxisState();
			if (CheckAmpFaultSwitch())
			{
				StopVelocityMove();
				AmpFaultReset();
				ClearStatus();
				return FALSE;
			}
		}

	}

	ClearStatus();   // Event 해제

	return TRUE;
}

void CNmcAxis::FinalizeHome()
{
	SetHomeAction(NO_EVENT);
	SetOriginStatus(TRUE);

	EnableSwLimit();
	SetPosHWLimitAction(E_STOP_EVENT);
	SetNegHWLimitAction(E_STOP_EVENT);

	if (!GetAmpEnable())
	{
		SetAmpEnable(TRUE);
		Sleep(100);
	}
	ClearStatus();
}

void CNmcAxis::SetOriginStatus(BOOL bStatus)
{
	int nAxisID = m_stParam.Motor.nAxisID;
	m_stParam.Home.bStatus = bStatus;
	m_bOrigin = TRUE;
}

BOOL CNmcAxis::GetOriginStatus()
{
	return m_bOrigin;// = TRUE;
}



int CNmcAxis::CmdBufferClear()
{
	int nRtn = 0;
	ClearStatus();
	return nRtn;
}


BOOL CNmcAxis::Stop(int nRate)	//For iRate * 10 msec, Stopping.
{
	MC_STATUS ms = MC_OK;
	char cstrErrorMsg[MAX_ERR_LEN];
	TCHAR msg[MAX_ERR_LEN];

	DWORD CurTimer = 0, StartTimer = 0;
	MSG message;
	BOOL bStop = FALSE;

	bStop = SetStop();

	StartTimer = GetTickCount();
	CurTimer = GetTickCount();
	while (IsStopping() && CurTimer - StartTimer < 1000.)
	{
		CurTimer = GetTickCount();
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	if (CurTimer - StartTimer >= 1000.)
	{
		SaveLog(_T("Error Motion Done On Axis Stop\nFor 1000msec!!!"));
	}

	ClearStatus();
	Sleep(30);
	SetAmpEnable(TRUE);
	Sleep(30);

	ms = MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
	if (ms != MC_OK || !bStop)
	{
		_stprintf(msg, _T("Error - MC_GetErrorMessage :: 0x%08X, %hs"), ms, CharToString(cstrErrorMsg));
		SaveLog(msg);
		return FALSE;
	}
	return TRUE;
}

BOOL CNmcAxis::IsInPosition()
{
	MC_STATUS ms = MC_OK;
	UINT32 Status = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &Status);


	if (Status & mcMotionComplete)
		return TRUE;

	return FALSE;
}

double CNmcAxis::GetInposition()
{
	MC_STATUS ms = MC_OK;
	double Length;

	ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpInPositionWindowSize, &Length);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetInposition()"));
		return 0.0;
	}
	double dInpos = PulseToLen(Length);
	return dInpos;
}

BOOL CNmcAxis::StartSCurveMove(double fPos, double fVel, double fAcc, double fJerk, BOOL bAbs, BOOL bWait)
{
	MC_STATUS ms = MC_OK;
	int error = 0;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	CString strTitleMsg = _T(""), strMsg = _T("");

	if (!WaitStandStill())
		return FALSE;

	CheckStatusAndClear();

/*
	while (!IsStandStill())
	{
		strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "Waiting for Motor stand still."));

		if (pGlobalView->GetSafeHwnd())
		{
			strMsg = _T("Waiting for Motor stand still.");
			pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_GREEN, 30000);
		}

		Sleep(100);
	}

	if (strTitleMsg != _T(""))
		pGlobalView->ClearDispMessage();
*/

	double dPos = LenToPulse(fPos);
	double dVel = LenToPulse(fVel);
	double dAcc = LenToPulse(fAcc);
	double dJerkTime = GetAccTime(fVel, fAcc, fJerk);
	double dJerk = LenToPulse(fAcc / dJerkTime);

	int nAxisId = m_stParam.Motor.nAxisID;

#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_S_AXIS)
	{
		BOOL bRtn = StartGantrySlaveMove(bAbs, fPos, fVel, fAcc, fAcc, fJerk, 3);
		if (bWait)
			WaitUntilMotionDone(TEN_SECOND);
		return bRtn;
	}

	if (nAxisId == SCAN_AXIS)
	{
		while (!IsMotionDoneGantrySlave())
		{
			Sleep(100);
		}
	}
#endif

	if (IsAmpFault())
	{
		ClearStatus();
		Sleep(30);
	}

	// symmetrical trapezoidal motion sequence
	if (bAbs)
	{
		// absolute coordinate move
		if (!bWait)
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(003) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(004) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
			WaitUntilMotionDone(TEN_SECOND);
		}
	}
	else
	{
		// incremental coordinate move
		if (!bWait)
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
			WaitUntilMotionDone(TEN_SECOND);
		}
	}
	return ((ms == MC_OK) ? TRUE : FALSE);

	return FALSE;
}

BOOL CNmcAxis::StartSCurveMove(double fPos, double fVelRatio, BOOL bAbs, BOOL bWait)
{
	MC_STATUS ms = MC_OK;
	int error = 0;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];
	
	CString strTitleMsg = _T(""), strMsg = _T("");

	if (!WaitStandStill())
		return FALSE;
	CheckStatusAndClear();

	/*
	while (!IsStandStill())
	{
		strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "Waiting for Motor stand still."));

		if (pGlobalView->GetSafeHwnd())
		{
			strMsg = _T("Waiting for Motor stand still.");
			pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_GREEN, 30000);
		}
		Sleep(100);
	}
	if (strTitleMsg != _T(""))
		pGlobalView->ClearDispMessage();
	*/

	double fVel = m_stParam.Speed.fVel * (fVelRatio / 100.0);
	double fAcc = m_stParam.Speed.fAcc * (fVelRatio / 100.0);

	fVel = (fVel > 0) ? fVel : -1.0 * fVel;
	fAcc = (fAcc > 0) ? fAcc : -1.0 * fAcc;

	double dPos = LenToPulse(fPos);
	double dVel = LenToPulse(fVel);
	double dAcc = LenToPulse(fAcc);
	double dJerkTime = m_stParam.Speed.fMinJerkTime / (fVelRatio / 100.0);
	double fJerk = fAcc / dJerkTime;
	double dJerk = LenToPulse(fJerk);

	int nAxisId = m_stParam.Motor.nAxisID;

#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_S_AXIS)
	{
		BOOL bRtn = StartGantrySlaveMove(bAbs, fPos, fVel, fAcc, fAcc, fJerk, 3);
		if(bWait)
			WaitUntilMotionDone(TEN_SECOND);
		return bRtn;
	}

	if (nAxisId == SCAN_AXIS)
	{
		while (!IsMotionDoneGantrySlave())
		{
			Sleep(100);
		}
	}
#endif

	if (IsAmpFault())
	{
		ClearStatus();
		Sleep(30);
	}

	// symmetrical trapezoidal motion sequence
	if (bAbs)
	{
		// absolute coordinate move
		if (!bWait)
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(005) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(006) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
			WaitUntilMotionDone(TEN_SECOND);
		}
	}
	else
	{
		// incremental coordinate move
		if (!bWait)
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dAcc, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);
				return FALSE;
			}
			WaitUntilMotionDone(TEN_SECOND);
		}
	}
	return ((ms == MC_OK) ? TRUE : FALSE);

	return FALSE;
}


double CNmcAxis::GetNegSoftwareLimit()
{
	MC_STATUS ms = MC_OK;
	double dPos = 0;
	ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpSWLimitNeg, &dPos);

	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetNegSoftwareLimit()"));
		return m_stParam.Motor.fNegLimit;
	}

	return PulseToLen(dPos);

//	return m_stParam.Motor.fNegLimit;
}

void CNmcAxis::SetNegSoftwareLimit(double fLimitVal, INT nAction)
{
	MC_STATUS ms = MC_OK;

	bool enable = true;

#ifdef SCAN_S_AXIS
	if (SCAN_S_AXIS == m_stParam.Motor.nAxisID)
		return;
#endif
	switch (nAction)
	{
	case NO_EVENT:
		enable = false;
		break;

	case STOP_EVENT:
		enable = true;
		break;

	case E_STOP_EVENT:
		enable = true;
		break;

	case ABORT_EVENT:
		enable = true;
		break;

	default:
		break;
	}

	m_stParam.Motor.fNegLimit = fLimitVal;
	double dPos = LenToPulse(fLimitVal);

	ms = MC_WriteParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpSWLimitNeg, dPos);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetNegSoftwareLimit()"));
		return;
	}

	ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableLimitNeg, enable);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetNegSoftwareLimit()"));
		return;
	}

}

double CNmcAxis::GetPosSoftwareLimit()
{
	MC_STATUS ms = MC_OK;
	double dPos = 0;
	ms = MC_ReadParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpSWLimitPos, &dPos);

	if (ms != MC_OK)
	{
		SaveLog(_T("Error-GetPosSoftwareLimit()"));
		return m_stParam.Motor.fPosLimit;
	}

	return PulseToLen(dPos);
}

void CNmcAxis::SetPosSoftwareLimit(double fLimitVal, INT nAction)
{
	MC_STATUS ms = MC_OK;

#ifdef SCAN_S_AXIS
	if (SCAN_S_AXIS == m_stParam.Motor.nAxisID)
		return;
#endif
	bool enable = true;

	switch (nAction)
	{
	case NO_EVENT:
		enable = false;
		break;

	case STOP_EVENT:
		enable = true;
		break;

	case E_STOP_EVENT:
		enable = true;
		break;

	case ABORT_EVENT:
		enable = true;
		break;

	default:
		break;
	}

	m_stParam.Motor.fPosLimit = fLimitVal;
	double dPos = LenToPulse(fLimitVal);

	ms = MC_WriteParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpSWLimitPos, dPos);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetPosSoftwareLimit()"));
		return;
	}

	ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpEnableLimitPos, enable);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetPosSoftwareLimit()"));
		return;
	}
}

int CNmcAxis::CheckExceptionEvent()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);

	return ((int)state);
}


int CNmcAxis::CheckAxisState()
{
	int nstate = 0;
	nstate = CheckExceptionEvent();
	return nstate;
}

BOOL CNmcAxis::ControllerIdle()
{
	int error = 0;
	;
	return TRUE;
}

INT CNmcAxis::GetStopRate(INT nRate)
{
	int error = 0;
	return nRate;
}

BOOL CNmcAxis::SetVelocity(double fVelocity)
{
	MC_STATUS ms = MC_OK;

	double dPulse = LenToPulse(fVelocity);
	// mcpCommandedVelocity is read only...
	ms = MC_WriteParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpCommandedVelocity, dPulse);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetVelocity()"));
		return FALSE;
	}

	// mcpActualVelocity is read only...
	ms = MC_WriteParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpActualVelocity, dPulse);
	if (ms != MC_OK)
	{
		SaveLog(_T("Error-SetVelocity()"));
		return FALSE;
	}

	return TRUE;
}

BOOL CNmcAxis::CheckHomeSwitch()
{
	return FALSE;

	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	INT nState = 0;
	INT nLevel = 0;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms == MC_OK)
	{
		if (state & mcHomeAbsSwitch)
			return TRUE;
	}

	return FALSE;
}

BOOL CNmcAxis::StartPtPMove(double fPos, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait)
{
	int error = 0;
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	CString strTitleMsg = _T(""), strMsg = _T("");

	if (!WaitStandStill())
		return FALSE;
	CheckStatusAndClear();

/*
	while (!IsStandStill())
	{
		strTitleMsg.Format(_T("%s %s"), m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "Waiting for Motor stand still."));

		if (pGlobalView->GetSafeHwnd())
		{
			strMsg = _T("Waiting for Motor stand still.");
			pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_GREEN, 30000);
		}
		Sleep(100);
	}
	if (strTitleMsg != _T(""))
		pGlobalView->ClearDispMessage();
*/
	double dJerkTime = GetAccTime(fVel, fAcc); //0.2; // [sec]

	double dPos = LenToPulse(fPos);
	double dVel = LenToPulse(fVel);
	double dAcc = LenToPulse(fAcc);
	double dDec = LenToPulse(fDec);
	double fJerk = (fAcc / dJerkTime);
	double dJerk = LenToPulse(fJerk);

	int nAxisId = m_stParam.Motor.nAxisID;

#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_S_AXIS)
	{
		BOOL bRtn = StartGantrySlaveMove(bAbs, fPos, fVel, fAcc, fAcc, fJerk, 3);
		if (bWait)
			WaitUntilMotionDone(TEN_SECOND);
		return bRtn;
	}

	if (nAxisId == SCAN_AXIS)
	{
		while (!IsMotionDoneGantrySlave())
		{
			Sleep(100);
		}
	}
#endif

	if (IsAmpFault())
	{
		ClearStatus();
		Sleep(30);
	}

	if (bAbs)
	{
		// absolute coordinate move
		if (!bWait)
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(007) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);

				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveAbsolute(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcPositiveDirection, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error(008) - MC_MoveAbsolute :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);

				return FALSE;
			}

			WaitUntilMotionDone(TEN_SECOND);
		}
	}
	else
	{
		// incremental coordinate move
		if (!bWait)
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);

				return FALSE;
			}
		}
		else
		{
			ms = MC_MoveRelative(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, mcAborting);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				msg.Format(_T("Error - MC_MoveRelative :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
				SaveLog(msg);

				return FALSE;
			}

			WaitUntilMotionDone(TEN_SECOND);

		}
	}

	return TRUE;
}

BOOL CNmcAxis::SetAmpEnableLevel(int nLevel)
{
	//MC_STATUS ms = MC_OK;

	//ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID, mcpEnableHWLimitNeg, (nAction != NO_EVENT));

	return TRUE;
}

BOOL CNmcAxis::ControllerRun()
{
	int error = 0;
	return TRUE;
}


int CNmcAxis::AmpFaultReset()
{
	MC_STATUS ms = MC_OK;
	//SetAmpEnable(FALSE);
	//Sleep(30);
	UINT32 state = 0x00000000;

	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms == MC_OK)
	{
		if ((state & mcDriveFault) || (state & mcErrorStop))
		{
			m_pParent->ClearSWEmg();
		}
	}

	ms = MC_Reset(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID);
	//Sleep(30);
	//SetAmpEnable(TRUE);
	Sleep(30);
	return ((ms == MC_OK) ? TRUE : FALSE);
}

BOOL CNmcAxis::CheckAmpFaultSwitch()
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;

	INT nState = 0;
	INT nLevel = 0;
	ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
	if (ms == MC_OK)
	{
		if ((state & mcDriveFault) || (state & mcErrorStop))
		{
			ms = MC_ReadAxisStatus(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, &state);
			if (ms == MC_OK)
			{
				if ((state & mcDriveFault) || (state & mcErrorStop))
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CNmcAxis::SetInPosLength(double dLength)
{
	MC_STATUS ms = MC_OK;
	double dLen = LenToPulse(dLength);
	ms = MC_WriteParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpInPositionWindowSize, dLen);
	return ((ms == MC_OK) ? TRUE : FALSE);
}

/*
In-Position Check Type 설정

0: None (In-Position Check 하지 않음)
1: External Drive (In-Position State를 외부 Drive에서 받음)
2: Internal(In-Position을 내부에서 판단)(Default)
3: External Drive + Internal

*/
BOOL CNmcAxis::SetInPosEnable(int nEnable)
{
	MC_STATUS ms = MC_OK;
	UINT32 enable = nEnable;

	return TRUE;
	//	ms = MC_WriteIntParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, mcpInPositionCheckType, enable);

	//	return ((ms == MC_OK) ? TRUE : FALSE);
}

void CNmcAxis::ResetEasyServo()
{
	ClearStatus();
}

BOOL CNmcAxis::VMove(double fVel, double fAcc)
{
	int nDir;


	if (fVel > 0)
		nDir = PLUS;
	else
		nDir = MINUS;

	return StartVelocityMove(fVel, fAcc);
}

// 지정된 속도,가속도로 이동한다.
BOOL CNmcAxis::VMove(int nDir)
{
	double dVel;

	if (nDir == PLUS)
		dVel = (m_fVel > 0) ? m_fVel : -1.0 * m_fVel;
	else if (nDir == MINUS)
		dVel = (m_fVel < 0) ? m_fVel : -1.0 * m_fVel;
	else
		dVel = m_fVel;

	return StartVelocityMove(dVel, m_fAcc);
}

void CNmcAxis::SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID)
{
	m_nBoardId = nBoardId;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = nOffsetAxisID;
}

void CNmcAxis::SetVMove(double fVel, double fAcc)
{
	m_fVel = fVel;
	m_fAcc = fAcc;
}

BOOL CNmcAxis::IsMotionDoneGantrySlave()
{
	int error = 0;
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];
	UINT32 Status = 0;

	ms = MC_GantryReadStatus(m_nBoardId, 0, &Status);
	if (ms != MC_OK)
	{
		SetEStop();
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error - MC_GantryReadStatus :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
		SaveLog(msg);

		return FALSE;
	}

	if ( (Status & mcGantry_MotionCompleted) && (Status & mcGantry_YawStable) )
		return TRUE;

	return FALSE;
}


double CNmcAxis::CalSpeedProfile(double fLength, double &fVel, double &fAcc, double &fDec)
{
	double fAccTime, fVelocityTime, fTotalTime;

	double fVelocity = fVel;
	double fAccPeriod = m_stParam.Speed.fAccPeriod;
	double fAccLength;
	fAcc = m_fAcc;

	if (fAcc <= 0.0 || fAcc > m_stParam.Speed.fMaxAcc)
		fAcc = m_stParam.Speed.fMaxAcc;

	// 이동거리중 가속 구간을 지정한 파라메타 값 50%로 규제.
	if (fAccPeriod > 50.0)
		fAccPeriod = 50.;

	// 가속 구간의 거리를 구한다.
	if (fAccPeriod < 50)
	{
		// 이동 거리중 가속구간의 거리.
		fAccLength = (fLength*fAccPeriod / 100.);
	}
	else
	{
		// 이동 거리의 1/2구간의 거리.
		fAccLength = fLength / 2.;
	}

	// 가속시간.
	fAccTime = sqrt(2.*fAccLength / fAcc);

	// 이동속도 도달시간.
	double fAccTimeToSpeed = fVelocity / fAcc;

	// 이동 속도.
	if (fAccTime > fAccTimeToSpeed)
	{
		fVel = fVelocity;
		fAcc = fVel / fAccTime; // 가속도 정정.
		fDec = fAcc;
	}
	else
	{
		fVel = fAcc*fAccTime;
		if (fAccTime < m_stParam.Speed.fMinJerkTime)
			fAccTime = m_stParam.Speed.fMinJerkTime;
		fAcc = fVel / fAccTime;
		fDec = fAcc;
	}

	// 정속 구간의 이동시간을 구한다.
	fVelocityTime = (fLength - (fAccLength * 2)) / fVel;
	//위치 정정 시간을 설정한다.
	double fSettleTime = 0.1;

	fTotalTime = fAccTime * 2 + fVelocityTime + fSettleTime;

	return fTotalTime;
}


BOOL CNmcAxis::StartGantrySlaveMove(BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk, byte nDispMode)
{
	int error = 0;
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	while (!IsMotionDoneGantrySlave())
	{
		Sleep(100);
	}

	//if (bOptimize)
	//	CalSpeedProfile(fLength, fVel, fAcc, fDec);

	/*
	mcGantry_MotionCompleted && mcGantry_YawStable


	TCHAR msg[MAX_ERR_LEN];
	MC_STATUS ms = MC_OK;
	char cstrErrorMsg[MAX_ERR_LEN];
	UINT32 Status = 0;
	CString sMsg;

	while()
	{
	ms = MC_GantryReadStatus(m_nBoardId, 0, &Status);
	if (ms != MC_OK)
	{
	SaveLog(_T("Error-MC_GantryStatus"));
	return FALSE;
	}

	if ((Status & mcGantry_MotionCompleted) && (Status & mcGantry_YawStable))
	{
		break;
	}

	}
	*/

	double dJerkTime = GetAccTime(fVel, fAcc); //0.2; // [sec]

	double dVel = fabs(LenToPulse(fVel));
	double dAcc = fabs(LenToPulse(fAcc));
	double dDec = fabs(LenToPulse(fDec));
	double dJerk = fabs(LenToPulse(fAcc / dJerkTime));

	double fCurPos, fTgtPos, fLength;

	if (m_stParam.Motor.bType != SERVO)
		fCurPos = GetCommandPosition();
	else
		fCurPos = GetActualPosition();
	
	if (bAbs)
	{
		fTgtPos = fPos;
		if (fTgtPos >= fCurPos)
			m_nMoveDir = POSITIVE_DIR;
		else
			m_nMoveDir = NEGATIVE_DIR;
		fLength = fabs(fTgtPos - fCurPos);
	}
	else
	{
		fTgtPos = fCurPos + fPos;
		if (fTgtPos >= fCurPos)
			m_nMoveDir = POSITIVE_DIR;
		else
			m_nMoveDir = NEGATIVE_DIR;
		fLength = fabs(fTgtPos - fCurPos);
	}

	double dPos = LenToPulse(fTgtPos);

	// absolute coordinate move
	ms = MC_ChangeGantryAlign(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, dPos, dVel, dAcc, dDec, dJerk, nDispMode);
	if (ms != MC_OK)
	{
		SetEStop();
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error - MC_ChangeGantryAlign :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
		SaveLog(msg);

		return FALSE;
	}
	
	return TRUE;
}

double CNmcAxis::GetPositionResolution()
{
	return (double)(m_stParam.Motor.fLeadPitch*m_stParam.Motor.fGearRatio) / (double)(m_stParam.Motor.nEncPulse*m_stParam.Motor.nEncMul); // [mm]
}

BOOL CNmcAxis::SetOriginPos()
{

	BOOL bRtn;
	int nAxisId = m_stParam.Motor.nAxisID;

	bRtn = SetPosition(m_stParam.Home.fOffset);
	if (!bRtn)
	{
		SaveLog(_T("Error-SetOriginPos()"));
	}


	if (VERIFY_AXIS == nAxisId)
	{
		bRtn = ((CNmcDevice*)m_pParent)->TriggerSetOriginPos(
			m_pParent->m_pParent->m_nTriggerEtherCatAddr,
			m_pParent->m_pParent->m_nTriggerVerifyVx,
			m_pParent->m_pParent->m_nTriggerVerifyIDX);
		bRtn = ((CNmcDevice*)m_pParent)->TriggerSetOriginPos(
			m_pParent->m_pParent->m_nTriggerEtherCatAddr,
			m_pParent->m_pParent->m_nTriggerVerifyVx+1,
			m_pParent->m_pParent->m_nTriggerVerifyIDX);
	}

	// Temparary -> 나중에 상위로 올려야 함....
	if (SCAN_AXIS == nAxisId)
	{
		double dPos = GetActualPosition();
		bRtn = ((CNmcDevice*)m_pParent)->TriggerSetOriginPos(
			m_pParent->m_pParent->m_nTriggerEtherCatAddr,
			m_pParent->m_pParent->m_nTriggerCameraVx, 
			m_pParent->m_pParent->m_nTriggerCameraIDX);
		bRtn = ((CNmcDevice*)m_pParent)->TriggerSetOriginPos(
			m_pParent->m_pParent->m_nTriggerEtherCatAddr, 
			m_pParent->m_pParent->m_nTriggerLaserFVx,
			m_pParent->m_pParent->m_nTriggerLaserFIDX);
		bRtn = ((CNmcDevice*)m_pParent)->TriggerSetOriginPos(
			m_pParent->m_pParent->m_nTriggerEtherCatAddr, 
			m_pParent->m_pParent->m_nTriggerLaserBVx,
			m_pParent->m_pParent->m_nTriggerLaserBIDX);
	}

	return bRtn;
}

void CNmcAxis::EnableSensorStop(int nSensorIndex, BOOL bEnable) // nSensorIndex : 0 ~ 4 , bEnable 
{
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];
	UINT32 ParamNum = 2109;

	if (nSensorIndex > 4 || nSensorIndex < 0)
	{
		SaveLog(_T("Error - Sensor index is out of range. (0 ~ 4)"));
		return;
	}

	switch (nSensorIndex)
	{
	case 0:
		ParamNum = 2109;	// 2109 SensorStop0
		break;
	case 1:
		ParamNum = 2115;	// 2115 SensorStop1
		break;
	case 2:
		ParamNum = 2121;	// 2121 SensorStop2
		break;
	case 3:
		ParamNum = 2127;	// 2127 SensorStop3
		break;
	case 4:
		ParamNum = 2133;	// 2133 SensorStop4
		break;
	}

	ms = MC_WriteBoolParameter(m_nBoardId, m_stParam.Motor.nAxisID + m_nOffsetAxisID, ParamNum, bEnable);

	if (ms != MC_OK)
	{
		SetEStop();
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error - MC_WriteBoolParameter :: 0x%08X, %s"), ms, CharToString(cstrErrorMsg));
		SaveLog(msg);
	}
}

/*****************************************************************************
<Homing Mode 설정>
0: RefPulse
1: AbsSwitch(Default)
2: LimitSwitch
3: Direct

<HomingDir>
typedef enum Home_Direction_tag
{
HOMING_DIR_CCW = 0,
HOMING_DIR_CW
}MC_HOME_AXIS_DIRECTION;

<HomePositionOffset>
Homing 완료 시 위치 값 설정
Default: 0

<HomeCompleteFlagHandle>
Homing 완료 시 “IsHomed” Flag 처리 방법 설정
0: Clear(Default)
1: Not Clear
2: Drive Clear-CSD7N
*****************************************************************************/

int CNmcAxis::EscapeHwPosLimit()
{
	if (CheckLimitSwitch(1))
	{
		MC_STATUS ms = MC_OK;
		UINT32 state = 0x00000000;
		UINT16 vAxisID = (UINT16)(m_stParam.Motor.nAxisID + m_nOffsetAxisID);
		DWORD nTick = 0;
		double dTempVal = 0.0;
		m_bOrigin = FALSE;

		int nAxisID = m_stParam.Motor.nAxisID;

		double dVel = LenToPulse(m_stParam.Home.f1stSpd);
		double dVel2 = LenToPulse(m_stParam.Home.f2ndSpd);
		double dAcc = LenToPulse(m_stParam.Home.fAcc);
		double dJerk = LenToPulse(m_stParam.Home.f1stSpd / m_stParam.Speed.fMinJerkTime);
		//double dOffset = LenToPulse(m_stParam.Home.fOffset);
		double dOffset = 0.0;
		//double dShift = -1.0 * LenToPulse(m_stParam.Home.fShift);

		ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

		if (state & mcErrorStop)
		{
			ClearStatus();
			Sleep(100);
		}

		ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

		if (state & mcDisabled)
		{
			SetAmpEnable(0);
			Sleep(100);
			SetAmpEnable(1);
		}

		ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

		if ((state & mcStandStill) == 0)
		{
			int bOK = 0;
			for (int a = 0; a < 3; a++)
			{
				SetAmpEnable(0);
				ClearStatus();
				Sleep(100);
				SetAmpEnable(1);
				Sleep(500);
				ClearStatus();
				Sleep(500);
				ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
				if ((state & mcStandStill) == 0)
				{
					bOK = 1;
					break;
				}
			}

			if (bOK == 0)
			{
				return FALSE;
			}
		}

		EnableSwLimit(0);
		SetNegHWLimitAction(E_STOP_EVENT);
		SetPosHWLimitAction(NO_EVENT);

		if (VMove(m_stParam.Home.f2ndSpd*(double)(-m_stParam.Home.nDir), m_stParam.Home.fAcc))
		{
			ULONGLONG nTick = GetTickCount64();
			while (GetTickCount64()-nTick<10000)
			{
				if (CheckLimitSwitch(1) == 0)
				{
					StopVelocityMove(0);
					break;
				}
				else
				{
					ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

					if (state & mcDriveFault || state & mcErrorStop)
					{

					}

				}
				Sleep(0);

				if (pGlobalView->GetSafeHwnd())
					pGlobalView->RefreshWindows(1);
				if (pMainFrame->GetSafeHwnd())
					pMainFrame->RefreshWindows(1);
				
			}
		}

		return 1;
	}
	else
		return 0;

}

int CNmcAxis::StartRsaHoming(BOOL bWait, int nMode, int nDir)
{
	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;
	UINT16 vAxisID = (UINT16)(m_stParam.Motor.nAxisID + m_nOffsetAxisID);
	DWORD nTick = 0;
	double dTempVal = 0.0;
	m_bOrigin = FALSE;

	int nAxisID = m_stParam.Motor.nAxisID;

	double dVel = LenToPulse(m_stParam.Home.f1stSpd);
	double dVel2 = LenToPulse(m_stParam.Home.f2ndSpd);
	double dAcc = LenToPulse(m_stParam.Home.fAcc);
	double dJerk = LenToPulse(m_stParam.Home.f1stSpd / m_stParam.Speed.fMinJerkTime);
	//double dOffset = LenToPulse(m_stParam.Home.fOffset);
	double dOffset = 0.0;
	//double dShift = -1.0 * LenToPulse(m_stParam.Home.fShift);

	ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

	if (state & mcErrorStop)
	{
		ClearStatus();
		Sleep(100);
	}

	ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

	if (state & mcDisabled)
	{
		SetAmpEnable(0);
		Sleep(100);
		SetAmpEnable(1);
	}

	ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

	if ((state & mcStandStill) == 0)
	{
		int bOK = 0;
		for (int a = 0; a < 3; a++)
		{
			SetAmpEnable(0);
			ClearStatus();
			Sleep(100);
			SetAmpEnable(1);
			Sleep(500);
			ClearStatus();
			Sleep(500);
			ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
			if ((state & mcStandStill) == 0)
			{
				bOK = 1;
				break;
			}
		}

		if (bOK == 0)
		{
			m_pParent->m_pParent->m_bHommingResult[nAxisID] = 2;

			SaveLog(_T("Error-StartRsaHoming(01)"));
			return FALSE;
		}
	}

	EnableSwLimit(0);
	SetNegHWLimitAction(NO_EVENT);
	SetPosHWLimitAction(NO_EVENT);

	if (IsGantry())
		ms = MC_GantryHome(m_nBoardId, vAxisID, (UINT16)nMode, dVel, dAcc, dAcc, dJerk, dVel2, (UINT8)nDir, dOffset);
	else
	{
		if (nMode != HOMMING_NMC_CONFIG)
		{
			ms = MC_WriteIntParameter(m_nBoardId, vAxisID, mcpHomingType, (UINT32)nMode);
			if (ms != MC_OK)
			{
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 3;

				SaveLog(_T("Error-StartRsaHoming(01)"));
				return FALSE;
			}
			ms = MC_WriteIntParameter(m_nBoardId, vAxisID, mcpHomingDir, (UINT32)nDir);
			if (ms != MC_OK)
			{ 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 4;

				SaveLog(_T("Error-StartRsaHoming(02)")); 
				return FALSE; 
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomingVelocity, dVel);				// 1st Speed
			if (ms != MC_OK) { 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 5;

				SaveLog(_T("Error-StartRsaHoming(03)"));
				return FALSE; 
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomingAcceleration, dAcc);
			if (ms != MC_OK) {

				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 6;

				SaveLog(_T("Error-StartRsaHoming(04)")); return FALSE; 
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomingDeceleration, dAcc);
			if (ms != MC_OK) {

				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 7;

				SaveLog(_T("Error-StartRsaHoming(05)"));
				return FALSE;
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomingJerk, dJerk);
			if (ms != MC_OK) { 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 8;

				SaveLog(_T("Error-StartRsaHoming(06)")); 
				return FALSE;
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomingCreepVelocity, dVel2);			// 2nd Speed
			if (ms != MC_OK) { 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 9;

				SaveLog(_T("Error-StartRsaHoming(07)"));
				return FALSE; 
			}
			ms = MC_WriteParameter(m_nBoardId, vAxisID, mcpHomePositionOffset, dOffset);
			if (ms != MC_OK) { 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 10;

				SaveLog(_T("Error-StartRsaHoming(08)"));
				return FALSE;
			}
			ms = MC_WriteIntParameter(m_nBoardId, vAxisID, mcpHomeCompleteFlagHandle, HOMING_CLEAR);
			if (ms != MC_OK) { 
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 11;

				SaveLog(_T("Error-StartRsaHoming(09)")); 
				return FALSE; 
			}
			Sleep(100);
		}

		ms = MC_Home(m_nBoardId, vAxisID, dOffset, mcAborting);
	}

	if (ms != MC_OK) 
	{
		m_pParent->m_pParent->m_bHommingResult[nAxisID] = 12;

		SaveLog(_T("Error-StartRsaHoming(10)")); 
		return ms; 
	}

	if (bWait)
	{
		int nStep = 1;
		int nRetry = 0;
		BOOL bPossibleRetry = 1;
		do 
		{

			if (pGlobalView->GetSafeHwnd())
				pGlobalView->RefreshWindows(1);
			if (pMainFrame->GetSafeHwnd())
				pMainFrame->RefreshWindows(1);

			if (nRetry > 2)
			{
				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 20;

				SaveLog(_T("Error-StartRsaHoming(01)"));
				return FALSE;
			}

			if (nStep == 0)
			{
				SetNegHWLimitAction(NO_EVENT);
				SetPosHWLimitAction(NO_EVENT);
				EnableSwLimit(0);

				ClearStatus();
				Sleep(100);
				SetAmpEnable(0);
				Sleep(500);
				SetAmpEnable(1);
				Sleep(500);
				ClearStatus();
				Sleep(100);

				ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);

				if ((state & mcStandStill) == 0)
				{
					if (nRetry < 2)
					{
						nRetry++;
						continue;
					}
					else
					{
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 2;

						SaveLog(_T("Error-StartRsaHoming(01)"));
						return FALSE;
					}
				}

				ms = MC_Home(m_nBoardId, vAxisID, dOffset, mcAborting);

				if (ms != MC_OK)
				{
					if (nRetry < 2)
					{
						nRetry++;
						continue;
					}
					else
					{
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 19;

						SaveLog(_T("mc_home was failed"));
						return FALSE;
					}
				}

				nRetry++;
				nStep=1;
			}
			else if (nStep == 1)
			{
				ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
				if (ms != MC_OK)
				{
					if (bPossibleRetry == 0)
					{
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 13;

						SaveLog(_T("Error-StartRsaHoming(11)"));
						return FALSE;
					}
					else
					{
						nStep = 0;
						continue;
					}
				}

				nTick = GetTickCount();
				while (!(state & mcHoming))
				{
					if (GetTickCount() - nTick >= 3000)	break;
					Sleep(100);
					ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
					if (ms != MC_OK) {
						if (bPossibleRetry == 0)
						{
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 14;

							SaveLog(_T("Error-StartRsaHoming(12)"));
							return FALSE;
						}
						else
						{
							nStep = 0;
							break;
						}
					}
				}

				if(nStep==0)
					continue;


				ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
				if (ms != MC_OK) {
					if (bPossibleRetry == 0)
					{
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 15;

						SaveLog(_T("Error-StartRsaHoming(13)"));
						return FALSE;
					}
					else
					{
						nStep = 0;
						continue;
					}
				}

				while (!(state & mcIsHomed))
				{
					if (GetTickCount() - nTick >= 60000*2) {
						if (bPossibleRetry == 0)
						{
							nStep = 0;
							break;
						}
						else
						{
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 16;

							SaveLog(_T("Error-StartRsaHoming(14)"));
							return FALSE;
						}
					}
					Sleep(100);
					ms = MC_ReadAxisStatus(m_nBoardId, vAxisID, &state);
					if (ms != MC_OK)
					{
						if (bPossibleRetry == 0)
						{
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 17;

							SaveLog(_T("Error-StartRsaHoming(15)"));
							return FALSE;
						}
						else
						{
							nStep = 0;
							break;
						}
					}

					if (pIO->IsDoorInterlock() || g_bDestroy)
					{
						SetEStop();
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 24;

						SaveLog(_T("IsDoorInterlock"));
						return FALSE;
					}

					if (m_pParent->ReadIn(EMERGENCY_SWITCH) || g_bDestroy)
					{
						SetEStop();
						m_pParent->m_pParent->m_bHommingResult[nAxisID] = 21;

						SaveLog(_T("Drive Fault"));
						return FALSE;
					}

					if (state & mcDisabled || state & mcDriveFault)
					{
						if (bPossibleRetry == 0)
						{
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 18;

							SaveLog(_T("Drive Fault"));
							return FALSE;
						}
						else
						{


							nStep = 0;
							break;
						}
					}
					else
					{
#ifdef USE_CLEAN_DOOR
						if (!CCleanDoor::m_pCleanDoor->GetInputBit(IN_CLEAN_DOOR_AREA_SENSOR))
						{
							SetEStop();
							SetAmpEnable(FALSE);	// Abort
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 22;

							SaveLog(_T("IN_CLEAN_DOOR_AREA_SENSOR"));
							return FALSE;
						}
#endif

						if (pIO->CheckSaftyOnlyBit())
						{
							SetEStop();
							m_pParent->m_pParent->m_bHommingResult[nAxisID] = 22;

							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

							SaveLog(_T("AREA_SENSOR!!!"));

							return FALSE;
						}

						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (m_pParent->ReadIn(LD_SHK_TABLE_LOCK))
							{
								SetEStop();
								m_pParent->m_pParent->m_bHommingResult[nAxisID] = 23;

								if(CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);
								return FALSE;
							}
						}

					}
				}

				if(nStep==0)
					continue;

				ClearStatus();
				Sleep(100);
				nStep++;
			}
			else if (nStep == 2)
			{
				/*
						if (m_stParam.Home.fShift < 0.0)
							dTempVal = -1.0*m_stParam.Home.fShift;
						else
							dTempVal = m_stParam.Home.fShift;

						StartPtPMove(dTempVal*(-(double)m_stParam.Home.nDir),
							m_stParam.Home.f2ndSpd,
							m_stParam.Home.fAcc,
							m_stParam.Home.fAcc,
							INC,
							WAIT);
				*/

				dTempVal = m_stParam.Home.fShift;

				SetNegHWLimitAction(E_STOP_EVENT);
				SetPosHWLimitAction(E_STOP_EVENT);

				StartPtPMove(dTempVal,
					m_stParam.Home.f2ndSpd,
					m_stParam.Home.fAcc,
					m_stParam.Home.fAcc,
					INC,
					WAIT);

				SetOriginPos();
				Sleep(100);

				ClearStatus();
				Sleep(100);

				ClearStatus();
				Sleep(100);

				EnableSwLimit(0);


				m_bOrigin = TRUE;

				m_pParent->m_pParent->m_bHommingResult[nAxisID] = 1;
				nStep++;				
				break;
			}

		} while (1);
	}
	else
	{
		; // CheckThreadIsHomed();

		if (g_bThreadHomeMotionRun == false)
		{
			m_bOrigin = FALSE;
			m_ThreadTaskRsa.Start(nullptr, this, RsaHomeThreadProc);// Start the thread

			//g_bThreadHomeMotionRun = true;
			//_beginthread(_thread_HomeMotionRun, NULL, NULL);
		}
	  
	}

	return (int)1;
}

int CNmcAxis::GetBoardID()
{
	return m_nBoardId;
}

int CNmcAxis::GetvAxisID()
{
	return (m_stParam.Motor.nAxisID + m_nOffsetAxisID);
}

UINT CNmcAxis::RsaHomeThreadProc(LPVOID lpContext)
{
	CNmcAxis* pThread = reinterpret_cast<CNmcAxis*>(lpContext);
	CNmcDevice* pParent = (CNmcDevice*)(pThread->m_pParent);

	MC_STATUS ms = MC_OK;
	UINT32 state = 0x00000000;
	UINT16 nBoardId = (UINT16)pThread->GetBoardID();
	UINT16 vAxisID = (UINT16)pThread->GetvAxisID();
	DWORD nTick = GetTickCount();
	double dTempVal = 0.0;

	pThread->m_bRsaHomeThreadAlive = TRUE;

	int nAxisID = vAxisID - 1;

	CString strTitleMsg;
	CString strMsg;

	if (pThread->IsGantry())
	{
		ms = MC_GantryReadStatus(nBoardId, 0, &state);

		if (ms != MC_OK)
		{
			pThread->m_bRsaHomeThreadAlive = FALSE;
			SaveLog(_T("Error-StartRsaHoming(200)"));
			return 0;
		}

		nTick = GetTickCount();
		while (!(state & mcGantry_Homing) && pThread->m_bRsaHomeThreadAlive)
		{
			if (GetTickCount() - nTick >= 3000)
			{
				pThread->m_bRsaHomeThreadAlive = FALSE;
				SaveLog(_T("Error-StartRsaHoming(201)"));
				return 0;
			}
			Sleep(pThread->m_nSamplingTimeMsec);

			ms = MC_GantryReadStatus(nBoardId, 0, &state);
			if (ms != MC_OK)
			{
				pThread->m_bRsaHomeThreadAlive = FALSE;
				SaveLog(_T("Error-StartRsaHoming(202)"));
				return 0;
			}
		}

		ms = MC_GantryReadStatus(nBoardId, 0, &state);
		if (ms != MC_OK)
		{
			pThread->m_bRsaHomeThreadAlive = FALSE;
			SaveLog(_T("Error-StartRsaHoming(203)"));
			return 0;
		}

		while (!(state & mcGantry_IsHomed) && pThread->m_bRsaHomeThreadAlive)
		{
			if (GetTickCount() - nTick >= 600000)
			{
				pThread->m_bRsaHomeThreadAlive = FALSE;
				SaveLog(_T("Error-StartRsaHoming(204)"));
				return 0;
			}
			Sleep(pThread->m_nSamplingTimeMsec);

			ms = MC_GantryReadStatus(nBoardId, 0, &state);
			if (ms != MC_OK)
			{
				pThread->m_bRsaHomeThreadAlive = FALSE;
				SaveLog(_T("Error-StartRsaHoming(205)"));
				return 0;
			}
		}
	}
	else
	{
		int nStep = 1;
		int nRetry = 0;
		do 
		{
			if (nRetry > 2)
			{
				return FALSE;
			}

			if (nStep == 0)
			{
				pThread->SetNegHWLimitAction(NO_EVENT);
				pThread->SetPosHWLimitAction(NO_EVENT);
				pThread->EnableSwLimit(0);

				pThread->ClearStatus();
				Sleep(100);
				pThread->SetAmpEnable(0);
				Sleep(100);
				pThread->SetAmpEnable(1);
				Sleep(100);
				pThread->ClearStatus();
				Sleep(100);

				ms = MC_ReadAxisStatus(pThread->m_nBoardId, vAxisID, &state);

				if ((state & mcStandStill) == 0)
				{
					if (nRetry < 2)
					{
						nRetry++;
						continue;
					}
					else
					{
						pThread->m_pParent->m_pParent->m_bHommingResult[nAxisID] = 102;

						SaveLog(_T("Error-StartRsaHoming(01)"));
						return FALSE;
					}
				}

				ms = MC_Home(pThread->m_nBoardId, vAxisID, 0, mcAborting);

				if (ms != MC_OK)
				{
					if (nRetry < 2)
					{
						nRetry++;
						continue;
					}
					else
					{
						pThread->m_pParent->m_pParent->m_bHommingResult[nAxisID] = 109;

						SaveLog(_T("mc_home was failed"));
						return FALSE;
					}
				}

				nRetry++;
				nStep = 1;
			}
			else if (nStep == 1)
			{
				ms = MC_ReadAxisStatus(nBoardId, vAxisID, &state);

				if (ms != MC_OK)
				{
					if (nRetry < 2)
					{
						nStep = 0;
						continue;
					}
					else
					{
						pParent->m_pParent->m_bHommingResult[nAxisID] = 103;

						pThread->m_bRsaHomeThreadAlive = FALSE;
						SaveLog(_T("Error-StartRsaHoming(100)"));
						return 0;
					}
				}

				nTick = GetTickCount();
				while (!(state & mcHoming) && pThread->m_bRsaHomeThreadAlive)
				{
					if (GetTickCount() - nTick >= 5000)
					{
						if ((state & mcIsHomed) == 0)
						{
							if (nRetry < 2)
							{
								nStep = 0;
								break;
							}
							else
							{
								pParent->m_pParent->m_bHommingResult[nAxisID] = 104;

								pThread->m_bRsaHomeThreadAlive = FALSE;
								SaveLog(_T("Error-StartRsaHoming(101)"));
								return 0;
							}
						}
						else
							break;

					}

					if(nStep==0)
						continue;

					Sleep(pThread->m_nSamplingTimeMsec);

					ms = MC_ReadAxisStatus(nBoardId, vAxisID, &state);
					if (ms != MC_OK)
					{
						if (nRetry < 2)
						{
							nStep = 0;
							continue;
						}
						else
						{
							pParent->m_pParent->m_bHommingResult[nAxisID] = 105;

							pThread->m_bRsaHomeThreadAlive = FALSE;
							SaveLog(_T("Error-StartRsaHoming(102)"));
							return 0;
						}
					}
				}

				ms = MC_ReadAxisStatus(nBoardId, vAxisID, &state);
				if (ms != MC_OK)
				{
					if (nRetry < 2)
					{
						nStep = 0;
						continue;
					}
					else
					{
						pParent->m_pParent->m_bHommingResult[nAxisID] = 106;

						pThread->m_bRsaHomeThreadAlive = FALSE;
						SaveLog(_T("Error-StartRsaHoming(103)"));
						return 0;
					}
				}

				while (!(state & mcIsHomed) && pThread->m_bRsaHomeThreadAlive)
				{
					if (GetTickCount() - nTick >= 60000*2)
					{
						if (nRetry < 2)
						{
							nStep = 0;
							break;
						}
						else
						{
							pParent->m_pParent->m_bHommingResult[nAxisID] = 107;

							pThread->m_bRsaHomeThreadAlive = FALSE;
							SaveLog(_T("Error-StartRsaHoming(104)"));
							return 0;
						}
					}
					Sleep(pThread->m_nSamplingTimeMsec);

					ms = MC_ReadAxisStatus(nBoardId, vAxisID, &state);
					if (ms != MC_OK)
					{
						if (nRetry < 2)
						{
							nStep = 0;
							break;
						}
						else
						{
							pParent->m_pParent->m_bHommingResult[nAxisID] = 108;

							pThread->m_bRsaHomeThreadAlive = FALSE;
							SaveLog(_T("Error-StartRsaHoming(105)"));
							return 0;
						}
					}

					if (pIO->IsDoorInterlock() || g_bDestroy)
					{
						pParent->m_pParent->m_bHommingResult[nAxisID] = 110;

						pThread->m_bRsaHomeThreadAlive = FALSE;
						SaveLog(_T("Error-StartRsaHoming(105)"));
						return 0;
					}

					if (pParent->ReadIn(EMERGENCY_SWITCH) || g_bDestroy)
					{
						pThread->SetEStop();
						pParent->m_pParent->m_bHommingResult[nAxisID] = 111;

						SaveLog(_T("Drive Fault"));
						return FALSE;
					}

					if ((state & mcDisabled) || (state & mcDriveFault))
					{
						if (nRetry < 2)
						{
							nStep = 0;
							break;
						}
						else
						{
							pParent->m_pParent->m_bHommingResult[nAxisID] = 109;

							pThread->m_bRsaHomeThreadAlive = FALSE;
							SaveLog(_T("Error-StartRsaHoming(105)"));
							return 0;
						}
					}
					else
					{
#ifdef USE_CLEAN_DOOR
						if (!CCleanDoor::m_pCleanDoor->GetInputBit(IN_CLEAN_DOOR_AREA_SENSOR))
						{
							pThread->SetEStop();
							pParent->m_pParent->m_bHommingResult[nAxisID] = 109;

							pThread->m_bRsaHomeThreadAlive = FALSE;
							SaveLog(_T("Error-StartRsaHoming(105)"));
							return 0;
						}
#endif

						if (pIO->CheckSaftyOnlyBit())
						{
							pThread->SetEStop();
							pThread->m_pParent->m_pParent->m_bHommingResult[nAxisID] = 22;

							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

							SaveLog(_T("AREA_SENSOR!!!"));

							return FALSE;
						}

						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (pParent->ReadIn(LD_SHK_TABLE_LOCK))
							{
								pThread->SetEStop();
								pParent->m_pParent->m_bHommingResult[nAxisID] = 23;

								if (CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

								return FALSE;
							}
						}

						if (g_pNMCDevice->CheckNmcConnection() == 0)
						{
							pThread->SetEStop();
							pParent->m_pParent->m_bHommingResult[nAxisID] = 24;

							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_MOTION_FAIL_ORIGIN_THREAD, 1);

							return FALSE;
						}

					}
				}

				if(nStep==0)
					continue;

				nStep++;
				break;
			}
		} while (1);
	}

	pThread->ClearStatus();
	Sleep(100);

	//pThread->SetPosition(-1.0 * pThread->m_stParam.Home.fShift);
/*
	if (pThread->m_stParam.Home.fShift < 0.0)
		dTempVal = -1.0 * pThread->m_stParam.Home.fShift;
	else
		dTempVal = pThread->m_stParam.Home.fShift;

	pThread->StartPtPMove(dTempVal*(-(double)pThread->m_stParam.Home.nDir),
		pThread->m_stParam.Home.f2ndSpd,
		pThread->m_stParam.Home.fAcc,
		pThread->m_stParam.Home.fAcc,
		INC,
		WAIT);
*/

	pThread->SetNegHWLimitAction(E_STOP_EVENT);
	pThread->SetPosHWLimitAction(E_STOP_EVENT);

	dTempVal = pThread->m_stParam.Home.fShift;

	pThread->StartPtPMove(dTempVal,
		pThread->m_stParam.Home.f2ndSpd,
		pThread->m_stParam.Home.fAcc,
		pThread->m_stParam.Home.fAcc,
		INC,
		WAIT);

	//pThread->SetPosition(pThread->m_stParam.Home.fOffset);
	pThread->SetOriginPos();
	Sleep(100);

	pThread->ClearStatus();
	Sleep(100);

	pThread->ClearStatus();
	Sleep(100);

	pThread->m_bRsaHomeThreadAlive = FALSE;
	pThread->m_bOrigin = TRUE;

	pThread->EnableSwLimit(0);
	strTitleMsg.Format(_T("%s %s"), pThread->m_stParam.Motor.sName, pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));

	strMsg.Format(_T("%s [%s]"), pGlobalView->GetLanguageString("MOTION", "FINISH_ORIGIN_RETURN"), pThread->m_stParam.Motor.sName);
	pGlobalView->OnDispMessage(strTitleMsg, strMsg, RGB_LTGREEN);		//20110114 hjc mod

	pParent->m_pParent->m_bHommingResult[nAxisID] = 1;

	return 0;
}

BOOL CNmcAxis::IsGantry()
{
	if (!((CNmcDevice*)m_pParent)->m_bUseGantry)
		return FALSE;
	else if (((CNmcDevice*)m_pParent)->m_lGantryMaster == m_stParam.Motor.nAxisID)
		return TRUE;

	return FALSE;

	//TCHAR msg[MAX_ERR_LEN];
	//MC_STATUS ms = MC_OK;
	//char cstrErrorMsg[MAX_ERR_LEN];
	//UINT32 Status = 0;
	//CString sMsg;

	////ms = MC_ReadStatus(m_nBoardId, (UINT16)GetvAxisID(), &Status);
	//ms = MC_GantryReadStatus(m_nBoardId, 0, &Status);
	//if (ms != MC_OK)
	//{
	//	SaveLog(_T("Error-MC_GantryStatus"));
	//	return FALSE;
	//}
	//else if (Status & mcGantry_Fault)
	//{
	//	sMsg.Format(_T("Error-MC_GantryStatus (0x%08x)"), Status);
	//	SaveLog(sMsg);
	//	return FALSE;
	//}
	//else if (Status & mcGantry_MotionCompleted)// || Status & mcGantry_YawStable
	//{
	//	return TRUE;
	//}
	//else
	//	return FALSE;

	//return TRUE;
}
