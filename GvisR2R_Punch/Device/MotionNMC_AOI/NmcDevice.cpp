// NmcDevice.cpp : implementation file
//
#include "../stdafx.h"



//#include "gvisaoi.h"
#include "NmcDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//bool g_bThreadHomeMotionRun = false;
//CNmcDevice* g_pNmcDevice = NULL;
//void _thread_ListMotionRun(LPVOID pArg);

/////////////////////////////////////////////////////////////////////////////
// CNmcDevice

CNmcDevice* g_pNMCDevice = nullptr;

CNmcDevice::CNmcDevice(CMotion* pParent)
{
	g_pNMCDevice = this;
	m_nRTAFSeqIndex = 0;
	m_pParent = pParent;
	m_pBufferControl = NULL;

	m_nInterlockStatus = 0;

	m_nOffsetIoOption = 0;

	m_strErrMsg = _T("");

	m_pParamCtrl = &(((CMotion*)m_pParent)->m_ParamCtrl);
	m_pParamMotion = ((CMotion*)m_pParent)->m_pParamMotion;
	m_pParamAxis = ((CMotion*)m_pParent)->m_pParamAxis;
	m_pParamMotor = ((CMotion*)m_pParent)->m_pParamMotor;

	m_nTotalAxisNum = m_pParamCtrl->nTotAxis;

	for (int i = 0; i < m_nTotalAxisNum; i++)	//20110922 hyk mod
		m_pAxis[i] = NULL;

	m_dwDeviceIoOut = 0xFFFFFFFF;

	m_nTriggerOrgPos = 0;

	m_nGroupID_RTAF = NMC_GROUPID_RTAF;
	m_nGroupID_ErrMapXY = NMC_GROUPID_ERRMAP;
	m_nGroupID_Interpolation = NMC_GROUPID_INTERPOLRATION;

	m_bListMotion = FALSE;
	m_bInterpolationMotion = FALSE;
	m_dStartScanPos = 0.0;
	m_dStartFocusPos = 0.0;
	m_bLogAddList = FALSE;
	m_sAddListLogTitle = _T("");

	m_bErrMap = FALSE;
	m_bAppliedEC = FALSE;
	m_bLoadedEC = FALSE;

	//m_nSamplingTimeMsec = 100;
	//g_pNmcDevice = this;

	m_bUseGantry = FALSE;
	m_lGantryMaster = -1;
	m_lGantrylSlave = -1;
	m_lGantryEnable = -1;
	m_bGantryEnabled = FALSE;
	m_bExit = 0;

	m_nBoardId = 0;
	m_nDevIdIoIn = 19;
	m_nDevIdIoOut = 19;
	m_nOffsetAxisID = 1;

	m_nBoardId = 0;
	m_dwDeviceIoOut = 0;

	for (int i = 0; i < MAX_AXIS; i++)
	{
		m_dVel[i] = 0;//	0x000001a50fdf6340 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_dAcc[i] = 0;//	0x000001a50fdf6370 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_dDec[i] = 0;//	0x000001a50fdf63a0 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_dJerk[i] = 0;//	0x000001a50fdf63d0 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_fAccPeriod[i] = 0;//	0x000001a50fdf6400 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_fPosLimit[i] = 0;//	0x000001a50fdf6430 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_fNegLimit[i] = 0;//	0x000001a50fdf6460 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
		m_fMaxAccel[i] = 0;//	0x000001a50fdf6490 {-6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, -6.2774385622041925e+66, ...}	double[6]
	}

	for (int k = 0; k < 8; k++)
		m_bSetTriggerConf[k] = FALSE;

	for (int k = 0; k < 10; k++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_OutSegInfo[k][j]=0;
			m_OutSegInfoOn[k][j]=0;
		}
	}

	InitDevice(1);
}

//2021 08 11 lgh add
BOOL CNmcDevice::Abort(int nMotionId)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	CNmcAxis* axes=CNmcDevice::GetAxis(nMotionId);

	if (axes)
		return axes->SetAmpEnable(FALSE);
	else
		return FALSE;
}

BOOL CNmcDevice::EStop(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	CNmcAxis* axes = CNmcDevice::GetAxis(nMsId);

	if (axes)
		return axes->SetEStop();
	else
		return FALSE;
}

void CNmcDevice::ResetStepper(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return;

	CNmcAxis* axes = CNmcDevice::GetAxis(nMsId);

	if (axes)
		axes->ClearStatus();
}

BOOL CNmcDevice::Clear(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	CNmcAxis* axes = GetAxis(nMsId);

	if (axes)
		return axes->ClearStatus();
	else
		return FALSE;
}

BOOL CNmcDevice::IsStop(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	CNmcAxis* axes = GetAxis(nMsId);

	if (axes)
		return axes->IsAxisDone();
	else
		return FALSE;

}

BOOL CNmcDevice::Stop(int nMotionId)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	CNmcAxis* axes = GetAxis(nMotionId);

	if (axes)
		return axes->Stop();
	else
		return FALSE;
}

void CNmcDevice::SetStopTime(int nMotionId, float fStopTime)
{
	if (nMotionId >= MAX_AXIS)
		return;

	CNmcAxis* axes = GetAxis(nMotionId);

	if (axes)
		axes->SetStopRate((int)fStopTime);
}

BOOL CNmcDevice::IsMotionDone(int nMotionId)
{
	CNmcAxis* axes = GetAxis(nMotionId);

	if (axes)
		return (BOOL)(axes->CheckMotionDone() ? TRUE : FALSE);
	else
		return FALSE;
}

BOOL CNmcDevice::IsInPosition(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		return (BOOL)(axes->CheckInMotion() ? TRUE : FALSE);
	else
		return FALSE;
}

CNmcDevice::~CNmcDevice()
{
	for (int i = 0; i < m_nTotalAxisNum; i++)	//20110922 hyk mod
	{
		if (!m_pAxis[i])
		{
			delete m_pAxis[i];
			m_pAxis[i] = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNmcDevice message handlers

int CNmcDevice::GetAxisState(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return 0;

	return (m_pAxis[nAxisId]->GetState());
}

BOOL CNmcDevice::SetActualPosition(int nAxisId, double fData)
{
	if (nAxisId >= MAX_AXIS)
		return 0;

	return m_pAxis[nAxisId]->SetPosition(fData);
}

BOOL CNmcDevice::SetCommandPosition(int nAxisId, double fData)
{
	if (nAxisId >= MAX_AXIS)
		return 0;

	return m_pAxis[nAxisId]->SetPosition(fData);
}

void CNmcDevice::SetErrorLimitAction(int nMotorID, int Action)
{

}

double CNmcDevice::GetPosSWLimitValue(int nAxisId)
{
	return m_pAxis[nAxisId]->GetPosSoftwareLimit();

}

double CNmcDevice::GetNegSWLimitValue(int nAxisId)
{
	return m_pAxis[nAxisId]->GetNegSoftwareLimit();
}

BOOL CNmcDevice::ServoOn(int nMotorId, BOOL bOn)
{
	if (nMotorId >= MAX_AXIS)
		return 0;

	return m_pAxis[nMotorId]->SetAmpEnable(bOn);
}

BOOL CNmcDevice::GetOrgInput(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0;

	return m_pAxis[nMotorId]->CheckHomeSwitch();
}

BOOL CNmcDevice::IsLimit(int nMotionId, int nDir)
{
	if (nMotionId >= MAX_AXIS)
		return 0;

	if (PLUS == nDir)
		return m_pAxis[nMotionId]->CheckPosLimitSwitch();
	else
		return m_pAxis[nMotionId]->CheckNegLimitSwitch();
}

void CNmcDevice::ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue)
{
	if (nMotorId >= MAX_AXIS)
		return;

	int nAct = m_pAxis[nMotorId]->GetPosLimitAction();

	m_pAxis[nMotorId]->SetPosSoftwareLimit(fErrorLimitValue, nAct);
}

void CNmcDevice::ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue)
{
	if (nMotorId >= MAX_AXIS)
		return;

	int nAct = m_pAxis[nMotorId]->GetNegLimitAction();
	m_pAxis[nMotorId]->SetNegSoftwareLimit(fErrorLimitValue, nAct);
}


void CNmcDevice::SetNegSoftwareLimit(int nMotorId, double fLimitVal, int nAction)
{
	if (nMotorId >= MAX_AXIS)
		return;

	m_pAxis[nMotorId]->SetNegSoftwareLimit(fLimitVal, nAction);
}

void CNmcDevice::SetPosSoftwareLimit(int nMotorId, double fLimitVal, int nAction)
{
	if (nMotorId >= MAX_AXIS)
		return;

	m_pAxis[nMotorId]->SetPosSoftwareLimit(fLimitVal, nAction);
}


BOOL CNmcDevice::IsAmpFault(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0;

	return m_pAxis[nMotorId]->CheckAmpFaultSwitch();
}

double CNmcDevice::GetPositionResolution(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0;

	return m_pAxis[nMotorId]->GetPositionResolution();
}

void CNmcDevice::SetCleanDoorFaultLimit()
{
}

BOOL CNmcDevice::SetHomeAction(int nMsIdx, int Action)
{
	return 1;
}

void CNmcDevice::ResetHomeAction(int nMsIdx)
{
}

BOOL CNmcDevice::SetSettleTime(int nAxisId, float fTime)
{
	return 1;
}

unsigned long CNmcDevice::GetSliceOut(int nSegment)
{
	long OutData = ReadOut();
	long nOut = 0;

	if (nSegment == 2)
	{
		nOut = ((OutData & 0x0000FFFF) >> 0);
	}
	else if (nSegment == 3)
	{
		nOut = ((OutData & 0xFFFF0000) >> 16);
	}
	else if (nSegment == 5)
	{
		long nData = 0;

		UINT8 val[2];

		MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 4, 1, (UINT8*)&val[0]);
		MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 5, 1, (UINT8*)&val[1]);

		nData |= val[0] & 0xFF;
		nData |= (val[1] & 0xFF) << 8;

		return nData;
	}
	else
		return FALSE;

	return ((unsigned long)nOut);
}

unsigned long CNmcDevice::GetSliceIo(int nSegment)
{
	unsigned long value = 0;
	long rtn = 0;

	if (nSegment == 0 || nSegment == 1)
		In32(&value);

	if (nSegment == 0)
		rtn |= (value << 0) & 0x0000FFFF;
	else if (nSegment == 1)
		rtn |= (value >> 16) & 0x0000FFFF;
	else if(nSegment == 4)
	{
		UINT8 val[2];
			
		MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 4, 1, (UINT8*)&val[0]);
		MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 5, 1, (UINT8*)&val[1]);

		rtn |= (val[0] << 0) & 0x000000FF;
		rtn |= (val[1] << 8) & 0x0000FF00;
	}

	return rtn;
}

void CNmcDevice::SetParam()
{
	int i = 0;

	//Axis Object
	for (i = 0; i < m_nTotalAxisNum; i++)
	{
		InitAxisParam(i);
	}
}

/*
MotionControl m_ParamCtrl;
MotionMotion* m_pParamMotion;
MotionAxis* m_pParamAxis;
MotionMotor* m_pParamMotor;
*/
BOOL CNmcDevice::DestroyDevice()
{
	if (m_pBufferControl)
	{
		delete m_pBufferControl;
		m_pBufferControl = NULL;
	}

	for (int iAxis = 0; iAxis < MAX_AXIS; iAxis++)	// 20110922 hyk mod
	{
		if (m_pAxis[iAxis])
		{
			if (!m_pAxis[iAxis]->IsMotionDone())
			{
				m_pAxis[iAxis]->Stop();
				m_pAxis[iAxis]->CmdBufferClear();
			}
			m_pAxis[iAxis]->SetAmpEnable(FALSE);
		}
	}

	//	if(get_mmc_error() != MMC_OK)
	MC_STATUS ms = MC_OK;
	char cstrErrorMsg[MAX_ERR_LEN];
	MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Failed MMC Board Exit !!!"));
		return FALSE;
	}

	ResetAxesGroup();

	//Delete Memory Allocation For Axis Object.
	//	for (int i=0; i<8; i++)
	for (int i = 0; i < 4; i++)	//20110922 hyk mod
	{
		if (m_pAxis[i])
		{
			delete(m_pAxis[i]);
			m_pAxis[i] = NULL;
		}
	}
	return TRUE;
}

BOOL CNmcDevice::ResetAxesGroup()
{
	MC_STATUS ms = MC_OK;
	UINT16 nAxesNum = 2;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = 0;
	UINT16 PositionCount = 2;	// 2축 직선보간운동

	MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	if (GroupStatus & GroupStandby)
	{
		ms = MC_GroupDisable(m_nBoardId, GroupNo);
		Sleep(100);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error - MC_GroupDisable :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);

			return FALSE;
		}
		else
		{
			//MC_GroupReadStatus를 통해 GroupDisabled 되었는지 확인
			MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
			if (GroupStatus & GroupDisabled)
			{
				//GroupDisabled가 On이면 GroupDisable 완료
				//AfxMessageBox(_T("GroupDisable Done"));
				return TRUE;
			}
			else
			{
				//GroupDisable 실패하면 Status 값 출력
				msg.Format(_T("GroupDisable Fail, GroupStatus: 0x%04x"), GroupStatus);
				AfxMessageBox(msg);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CNmcDevice::InitAxisParam(int nAxis)
{
	if (nAxis >= MAX_AXIS)
		return FALSE;

	//m_pParamCtrl = &(((CMotion*)m_pParent)->m_ParamCtrl);
	//m_pParamMotion = ((CMotion*)m_pParent)->m_pParamMotion;
	//m_pParamAxis = ((CMotion*)m_pParent)->m_pParamAxis;
	//m_pParamMotor = ((CMotion*)m_pParent)->m_pParamMotor;
	//m_nTotalAxisNum = m_pParamCtrl->nTotAxis;

	if (!m_pAxis[nAxis])
		return FALSE;

	m_pAxis[nAxis]->m_stParam.Motor.sName = m_pParamAxis[nAxis].sName;
	m_pAxis[nAxis]->m_stParam.Motor.fLeadPitch = m_pParamMotor[nAxis].fLeadPitch; // [mm]
	m_pAxis[nAxis]->m_stParam.Motor.nEncPulse = m_pParamMotor[nAxis].nEncPulse; // [pulse/rev]
	m_pAxis[nAxis]->m_stParam.Motor.nEncMul = m_pParamMotor[nAxis].nEncMul;
	m_pAxis[nAxis]->m_stParam.Motor.fGearRatio = m_pParamMotor[nAxis].fGearRatio; // [load/roater]
	m_pAxis[nAxis]->m_stParam.Motor.fInpRange = m_pParamAxis[nAxis].fInpRange;
	//m_pAxis[nAxis]->m_stParam.Motor.nInpTime = m_pParamMotor[nAxis].nInpTime;
	m_pAxis[nAxis]->m_stParam.Motor.fPosRes = m_pParamMotor[nAxis].fLeadPitch / (double)(m_pParamMotor[nAxis].nEncPulse*m_pParamMotor[nAxis].nEncMul*m_pParamMotor[nAxis].fGearRatio); // [mm]
	m_pAxis[nAxis]->m_stParam.Speed.fAccPeriod = m_pParamAxis[nAxis].fAccPeriod;
	m_pAxis[nAxis]->m_stParam.Speed.fMaxAcc = m_pParamAxis[nAxis].fMaxAccel; // [mm/s2]
	m_pAxis[nAxis]->m_stParam.Speed.fVel = m_pParamAxis[nAxis].fSpd; // [mm/s]
	m_pAxis[nAxis]->m_stParam.Speed.fAcc = m_pParamAxis[nAxis].fAcc; // [mm/s2]
	m_pAxis[nAxis]->m_stParam.Speed.fMinJerkTime = m_pParamAxis[nAxis].fMinJerkTime;
	m_pAxis[nAxis]->m_stParam.Speed.fMaxVel = m_pParamAxis[nAxis].fSpd;// m_pParamMotor[nAxis].fLeadPitch*(m_pParamMotor[nAxis].fRatingSpeed / 60.); // [mm/s]
	m_pAxis[nAxis]->m_stParam.Speed.fRatingRPM = m_pParamMotor[nAxis].fRatingSpeed; // [rev/min]

	m_pAxis[nAxis]->m_stParam.Motor.nAxisID = nAxis;
	m_pAxis[nAxis]->m_stParam.Motor.bType = m_pParamMotor[nAxis].bType;				// Motor Type 0: servo 1: stepper
	//m_pAxis[nAxis]->m_stParam.Motor.bSafety = m_pParamMotor[nAxis].bSafety;			// Safety Flag
	m_pAxis[nAxis]->m_stParam.Motor.fPosLimit = m_pParamMotor[nAxis].fPosLimit;		// Positive Software Limit
	m_pAxis[nAxis]->m_stParam.Motor.fNegLimit = m_pParamMotor[nAxis].fNegLimit;		// Negative Software Limit
	//m_pAxis[nAxis]->m_stParam.Motor.nVibOffset = m_pParamMotor[nAxis].nVibOffset;	// Vibration offset

	m_pAxis[nAxis]->m_stParam.Home.bIndex = m_pParamMotion[nAxis].Home.bIndex;
	m_pAxis[nAxis]->m_stParam.Home.nDir = m_pParamMotion[nAxis].Home.nDir > 0 ? 1 : -1 ;			// Initial Home Dir, TRUE:plus-dir	FALSE:minus-dir
	m_pAxis[nAxis]->m_stParam.Home.f1stSpd = m_pParamMotion[nAxis].Home.f1stSpd;	// PreHomming Speed
	m_pAxis[nAxis]->m_stParam.Home.f2ndSpd = m_pParamMotion[nAxis].Home.f2ndSpd;	// Homming Speed
	m_pAxis[nAxis]->m_stParam.Home.fAcc = m_pParamMotion[nAxis].Home.fAcc;			// Homming Accel
	m_pAxis[nAxis]->m_stParam.Home.fShift = m_pParamMotion[nAxis].Home.fShift;		// shift
	m_pAxis[nAxis]->m_stParam.Home.fOffset = m_pParamMotion[nAxis].Home.fOffset;	// offset
	m_pAxis[nAxis]->m_stParam.Home.fMargin = m_pParamMotion[nAxis].Home.fMargin;	// Margin between Origin & index After Homming
	m_pAxis[nAxis]->m_stParam.Home.bStatus = FALSE;
	m_pAxis[nAxis]->m_stParam.Home.fEscLen = m_pParamMotion[nAxis].Home.fEscLen;	// Escape length from sensor touch position

	m_pAxis[nAxis]->m_stParam.Speed.fDec = m_pParamAxis[nAxis].fDec;					// Deceleration
	m_pAxis[nAxis]->m_stParam.Speed.fJogFastSpd = m_pParamAxis[nAxis].fJogFastSpd;		// Speed
	m_pAxis[nAxis]->m_stParam.Speed.fJogMidSpd = m_pParamAxis[nAxis].fJogMidSpd;		// Speed
	m_pAxis[nAxis]->m_stParam.Speed.fJogLowSpd = m_pParamAxis[nAxis].fJogLowSpd;		// Speed
	m_pAxis[nAxis]->m_stParam.Speed.fJogAcc = m_pParamAxis[nAxis].fJogAcc;				// Acceleration

	m_pAxis[nAxis]->m_stParam.Io.bPosLimit = m_pParamMotor[nAxis].bPosLimitLevel;	// 정방향 리미트 스위치 신호 레벨
	m_pAxis[nAxis]->m_stParam.Io.bNegLimit = m_pParamMotor[nAxis].bNegLimitLevel;	// 역방향 리미트 스위치 신호 레벨
	m_pAxis[nAxis]->m_stParam.Io.bOrg = m_pParamMotor[nAxis].bHomeLevel;				// 원점 스위치 신호 레벨
	m_pAxis[nAxis]->m_stParam.Io.bAmpFault = m_pParamMotor[nAxis].bAmpFaultLevel;	// Amp Fault 신호 레벨
	m_pAxis[nAxis]->m_stParam.Io.bAmpEnable = m_pParamMotor[nAxis].bAmpEnableLevel;	// Amp Enable 신호 레벨

	//m_pAxis[nAxis]->m_stParam.Gain.nP = sttMotion.Gain.nP;					// P Gain
	//m_pAxis[nAxis]->m_stParam.Gain.nI = sttMotion.Gain.nI;					// I Gain
	//m_pAxis[nAxis]->m_stParam.Gain.nD = sttMotion.Gain.nD;					// D Gain
	//m_pAxis[nAxis]->m_stParam.Gain.nAccelFF = sttMotion.Gain.nAccelFF;		// Acceleration Feed Forward
	//m_pAxis[nAxis]->m_stParam.Gain.nVelFF = sttMotion.Gain.nVelFF;			// Velocity Feed Forward
	//m_pAxis[nAxis]->m_stParam.Gain.nILimit = sttMotion.Gain.nILimit;		// Integral Limit
	//m_pAxis[nAxis]->m_stParam.Gain.nOffset = sttMotion.Gain.nOffset;		// Command Offset
	//m_pAxis[nAxis]->m_stParam.Gain.nDACLimit = sttMotion.Gain.nDACLimit;	// DAC Limit
	//m_pAxis[nAxis]->m_stParam.Gain.nShift = sttMotion.Gain.nShift;			// Gain Shift
	//m_pAxis[nAxis]->m_stParam.Gain.nFrictFF = sttMotion.Gain.nFrictFF;		// Friction Feed Forward

	m_pAxis[nAxis]->m_fVel = m_pParamAxis[nAxis].fSpd;
	m_pAxis[nAxis]->m_fAcc = m_pParamAxis[nAxis].fAcc;
	m_pAxis[nAxis]->m_fEStopTime = m_pParamMotion[nAxis].dEStopTime;
	m_fMaxVel[nAxis] = m_pParamMotor[nAxis].fLeadPitch*(m_pParamMotor[nAxis].fRatingSpeed / 60.); // [mm/s]

	double dLength = 0.0;

	m_pAxis[nAxis]->SetNegHWLimitAction(E_STOP_EVENT);
	Sleep(100);
	m_pAxis[nAxis]->SetPosHWLimitAction(E_STOP_EVENT);
	Sleep(100);
	m_pAxis[nAxis]->SetHomeAction(E_STOP_EVENT);
	Sleep(100);
	m_pAxis[nAxis]->SetNegSoftwareLimit(m_pParamMotor[nAxis].fNegLimit, E_STOP_EVENT);
	Sleep(100);
	m_pAxis[nAxis]->SetPosSoftwareLimit(m_pParamMotor[nAxis].fPosLimit, E_STOP_EVENT);
	Sleep(100);

	/*
	<In - Position Check Type 설정>
	0: None(In - Position Check 하지 않음)
	1 : External Drive(In - Position State를 외부 Drive에서 받음)
	2 : Internal(In - Position을 내부에서 판단)(Default)
	3 : External Drive + Internal
	*/
	m_pAxis[nAxis]->SetInPosEnable(1);
	Sleep(100);

	if (m_pParamMotor[nAxis].bType == 0)		// Servo
	{
		m_pAxis[nAxis]->SetInPosLength(m_pParamAxis[nAxis].fInpRange);
		Sleep(100);
	}
	m_pAxis[nAxis]->SetEStopRate(20.0);
	Sleep(100);

	return TRUE;
}


BOOL CNmcDevice::CreateAxis(int nAxis)
{
	if (nAxis >= MAX_AXIS)
		return FALSE;

	if (!m_pAxis[nAxis])
	{
		m_pAxis[nAxis] = new CNmcAxis(this);
	//	m_pAxis[nAxis]->SetParentClassHandle(this->GetSafeHwnd());
	}

	//if (!InitAxisParam(nAxis))
	//	return FALSE;


	return TRUE;
}

CNmcAxis* CNmcDevice::GetAxis(int nAxis)
{
	if (nAxis >= MAX_AXIS)
		return FALSE;

	CString strMsg;
	if (!m_pAxis[nAxis])
	{
		strMsg.Format(_T("Didn't create %d axis."), nAxis);
		AfxMessageBox(strMsg);
	}
	return m_pAxis[nAxis];

}

int CNmcDevice::NMC_In32(unsigned long * value)
{
	return In32(value);
}

int CNmcDevice::NMC_Out32(long value)
{
	return Out32(value);
}

BOOL CNmcDevice::CreateBackwardSequenceDataForUniHeader(double dOrgFocus)
{
	return 0;
}

BOOL CNmcDevice::SearchHomePos(int nMotionId, BOOL bThread)
{
	BOOL bRtn = 0;
#if USE_NMC_HOME ==1
	int nDir = m_pParamMotion[nMotionId].Home.nDir == HOMING_DIR_CW ? HOMING_DIR_CW : HOMING_DIR_CCW;
	int mc = NMC_StartRsaHoming(nMotionId, !bThread, HOMING_LIMIT_SWITCH, nDir);
	bRtn = (mc == MC_OK) ? TRUE : FALSE;

	return bRtn;
#else
	if (bThread)
	{
		bRtn = NMC_StartHomeThread(nMotionId);
	}
	else
	{
		bRtn = NMC_StartHomming(nMotionId);
		if (bRtn)
		{
			SetOriginStatus(nMotionId, TRUE);
		}
		else
			SetOriginStatus(nMotionId, FALSE);
	}
#endif

	return bRtn;
}

BOOL CNmcDevice::InitDevice(int nDevice)
{
	MC_STATUS ms = MC_OK;
	TCHAR msg[MAX_ERR_LEN];

	UINT16 MstDevIDs[MAX_BOARD_CNT];
	UINT16 MstCnt;

	char cstrErrorMsg[MAX_ERR_LEN];
	int i = 0, j = 0;
	ULONGLONG StartTimer;
	UINT8	MstMode;

	ms = MC_GetMasterMap(MstDevIDs, &MstCnt);

	if (ms != MC_OK) //MC_OK가 아닐 경우 Error 출력
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		_stprintf(msg, _T("Error - MC_GetMasterMap :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
		TRACE(msg);
		return FALSE;
	}
	else
	{
		_stprintf(msg, _T("Board Count = %d , Board List : [%d],[%d],[%d],[%d]\n"),
			MstCnt,     // 설치된 보드 개수
			MstDevIDs[0],    // 첫번째 보드 ID
			MstDevIDs[1],    // 두번째 보드 ID
			MstDevIDs[2],    // 세번째 보드 ID
			MstDevIDs[3]);   // 네번째 보드 ID

		TRACE(msg);
	}

	for (i = 0; i < MstCnt; i++)
	{
		if (MstDevIDs[i] == 0xCCCC)
		{
			return FALSE;
		}
		ms = MC_MasterInit(MstDevIDs[i]);
		if (ms != MC_OK) //MC_OK가 아닐경우 Error 출력
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			_stprintf(msg, _T("Error - MC_MasterInit :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
			TRACE(msg);

			return FALSE;
		}
	}

	for (i = 0; i < MstCnt; i++)
	{
		ms = MC_MasterSTOP(MstDevIDs[i]);
		Sleep(100);
		if (ms != MC_OK) //MC_OK가 아닐 경우 Error 출력
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			_stprintf(msg, _T("Error - MC_MasterSTOP :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
			TRACE(msg);

			return FALSE;
		}
	}

	for (i = 0; i < MstCnt; i++) // 20211027 - syd
	{
		StartTimer = GetTickCount64();
		while (1)
		{
			ms = MasterGetCurMode(MstDevIDs[i], &MstMode);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				_stprintf(msg, _T("Error - MasterGetCurMode :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
				TRACE(msg);

				return FALSE;
			}
			else		//MC_OK 리턴되면 Master State확인
			{
				if (MstMode == EcMstMode::eMM_IDLE)
					break;
			}

			Sleep(100);

			if (GetTickCount64() - StartTimer > 5000)
			{
				TRACE(_T("Check Master IDLE Status Time Out\n"));

				return FALSE;
			}
		}
	}

	for (i = 0; i < MstCnt; i++)
	{
		ms = MC_MasterRUN(MstDevIDs[i]);
		Sleep(100);
		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			_stprintf(msg, _T("Error - MC_MasterRUN :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
			TRACE(msg);

			return FALSE;
		}
		else
		{
			//MC_OK 리턴되면 Master State확인
			StartTimer = GetTickCount64();
			while (1)
			{
				//Sleep(100);
				ms = MasterGetCurMode(MstDevIDs[i], &MstMode);
				if (ms != MC_OK)
				{
					MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
					_stprintf(msg, _T("Error - MasterGetCurMode :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
					TRACE(msg);

					return FALSE;
				}

				if (MstMode == EcMstMode::eMM_RUN)
				{
					//Master State = RUN
					break;
				}

				//Master가 Error상태이거나 통신이 끊긴 상태인지 확인
				if (MstMode == EcMstMode::eMM_ERR || MstMode == EcMstMode::eMM_LINKBROKEN)
				{
					TRACE(_T("Master State is ERROR or LINKBROKEN  State\n"));

					return FALSE;
				}

				if (GetTickCount64() - StartTimer > 5000)
				{
					TRACE(_T("Check Master Run Status Time Out\n"));

					return FALSE;
				}
			}
		}
	}

	UINT8 SlaveState;
	for (i = 0; i < MstCnt; i++)
	{
		Sleep(100);

		StartTimer = GetTickCount64();
		while (1)
		{
			ms = SlaveGetCurState(MstDevIDs[i], 1, &SlaveState);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				_stprintf(msg, _T("Error - SlaveGetCurState :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
				TRACE(msg);
				SaveLog(msg);
				return FALSE;
			}
			else
			{

				if (SlaveState == eST_OP)
				{
					_stprintf(msg, _T("Board%d, EcatAddr%d 통신 정상\n"), MstDevIDs[i], 1);
					TRACE(msg);

					break;
				}

				if (GetTickCount64() - StartTimer > 5000)
				{
					TRACE(_T("Get Current Slave State Time Out, 0x%08X\n"), SlaveState);
					_stprintf(msg,_T("Get Current Slave State Time Out, 0x%08X\n"), SlaveState);
					SaveLog(msg);
					return FALSE;
				}
			}
		}
	}



	return TRUE;
}


BOOL CNmcDevice::Recovery()
{
	MC_STATUS ms = MC_OK;
	TCHAR msg[MAX_ERR_LEN];

	UINT16 MstDevIDs[MAX_BOARD_CNT];
	UINT16 MstCnt;

	char cstrErrorMsg[MAX_ERR_LEN];
	int i = 0, j = 0;
	ULONGLONG StartTimer;
	UINT8	MstMode;

	ms = MC_GetMasterMap(MstDevIDs, &MstCnt);

	if (ms != MC_OK) //MC_OK가 아닐 경우 Error 출력
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		_stprintf(msg, _T("Error - MC_GetMasterMap :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
		TRACE(msg);
		return FALSE;
	}
	else
	{
		_stprintf(msg, _T("Board Count = %d , Board List : [%d],[%d],[%d],[%d]\n"),
			MstCnt,     // 설치된 보드 개수
			MstDevIDs[0],    // 첫번째 보드 ID
			MstDevIDs[1],    // 두번째 보드 ID
			MstDevIDs[2],    // 세번째 보드 ID
			MstDevIDs[3]);   // 네번째 보드 ID

		TRACE(msg);
	}

	for (i = 0; i < MstCnt; i++)
	{
		if (MstDevIDs[i] == 0xCCCC)
		{
			return FALSE;
		}
	}

	for (i = 0; i < MstCnt; i++)
	{
		ms = MC_MasterSTOP(MstDevIDs[i]);
		Sleep(100);
		if (ms != MC_OK) //MC_OK가 아닐 경우 Error 출력
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			_stprintf(msg, _T("Error - MC_MasterSTOP :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
			TRACE(msg);

			return FALSE;
		}
	}

	for (i = 0; i < MstCnt; i++)
	{
		ms = MC_MasterRUN(MstDevIDs[i]);
		Sleep(100);
		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			_stprintf(msg, _T("Error - MC_MasterRUN :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
			TRACE(msg);

			return FALSE;
		}
		else
		{
			//MC_OK 리턴되면 Master State확인
			StartTimer = GetTickCount64();
			while (1)
			{
				//Sleep(100);
				ms = MasterGetCurMode(MstDevIDs[i], &MstMode);
				if (ms != MC_OK)
				{
					MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
					_stprintf(msg, _T("Error - MasterGetCurMode :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
					TRACE(msg);

					return FALSE;
				}

				if (MstMode == EcMstMode::eMM_RUN)
				{
					//Master State = RUN
					break;
				}

				//Master가 Error상태이거나 통신이 끊긴 상태인지 확인
				if (MstMode == EcMstMode::eMM_ERR || MstMode == EcMstMode::eMM_LINKBROKEN)
				{
					TRACE(_T("Master State is ERROR or LINKBROKEN  State\n"));

					return FALSE;
				}

				if (GetTickCount64() - StartTimer > 5000)
				{
					TRACE(_T("Check Master Run Status Time Out\n"));

					return FALSE;
				}
			}
		}
	}

	UINT8 SlaveState;
	for (i = 0; i < MstCnt; i++)
	{
		Sleep(100);

		StartTimer = GetTickCount64();
		while (1)
		{
			ms = SlaveGetCurState(MstDevIDs[i], 1, &SlaveState);
			if (ms != MC_OK)
			{
				MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
				_stprintf(msg, _T("Error - SlaveGetCurState :: 0x%08X, %hs\n"), ms, cstrErrorMsg);
				TRACE(msg);
				SaveLog(msg);
				return FALSE;
			}
			else
			{

				if (SlaveState == eST_OP)
				{
					_stprintf(msg, _T("Board%d, EcatAddr%d 통신 정상\n"), MstDevIDs[i], 1);
					TRACE(msg);

					break;
				}

				if (GetTickCount64() - StartTimer > 5000)
				{
					TRACE(_T("Get Current Slave State Time Out, 0x%08X\n"), SlaveState);
					_stprintf(msg, _T("Get Current Slave State Time Out, 0x%08X\n"), SlaveState);
					SaveLog(msg);
					return FALSE;
				}
			}
		}
	}

	for(int k=0; k<10; k++)
		for (int j = 0; j < 32; j++)
		{
			if (m_OutSegInfo[k][j] ==1)
			{
				MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, k, j, m_OutSegInfoOn[k][j]);				
			}
		}


	for (int nAxis = 0; nAxis < MAX_AXIS; nAxis++) // 20100507-syd
	{
		ServoOn(nAxis, 0);
		Clear(nAxis);
	}


	return TRUE;
}

void CNmcDevice::ClearSWEmg()
{
	UINT8 status = 0;
	MC_RestoreSWEmergency(m_nBoardId, &status);
}



int CNmcDevice::In32(unsigned long *value)
{
	int nRtn = MC_OK;
	UINT uBitSize = 32;
	UINT uDataSize = uBitSize / 8;
	unsigned long nData = 0;

	UINT8 val[4];
	uDataSize = 1;		// [Byte]

	//IO 상태 Read
	MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 0, uDataSize, (UINT8*)&val[0]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 1, uDataSize, (UINT8*)&val[1]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 2, uDataSize, (UINT8*)&val[2]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoIn, BUF_IN, 3, uDataSize, (UINT8*)&val[3]);

	nData |= (val[0] << 0) & 0x000000FF;
	nData |= (val[1] << 8) & 0x0000FF00;
	nData |= (val[2] << 16) & 0x00FF0000;
	nData |= (val[3] << 24) & 0xFF000000;

	*value = (nData);

	return nRtn;
}

int CNmcDevice::Out32(long value)
{
	int nRtn = MC_OK;
	UINT uBitSize = 32;
	UINT uDataSize = uBitSize / 8;	// [Byte]
	//UINT8* bData = new BYTE[uDataSize];
	long val;

	////IO 상태 Read
	//MC_IO_READ(m_nBoardId, m_nDevIdIoIn, 0, 0, uDataSize, bData);
	//val = (long)bData;
	//val = ~val;

	//if (val != value)
	{
		UINT8 val[4];
		uDataSize = 1;		// [Byte]

		val[0] = (value >> 0)	& 0xFF;
		val[1] = (value >> 8)	& 0xFF;
		val[2] = (value >> 16)	& 0xFF;
		val[3] = (value >> 24)	& 0xFF;

		//value = ~value;
		//val[0] = ~val[0];
		//val[1] = ~val[1];
		//val[2] = ~val[2];
		//val[3] = ~val[3];

		//IO 상태 Write		 
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 0, uDataSize, (UINT8*)&val[0]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 1, uDataSize, (UINT8*)&val[1]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 2, uDataSize, (UINT8*)&val[2]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 3, uDataSize, (UINT8*)&val[3]);
	}
	//delete bData;

	return nRtn;
}

BOOL CNmcDevice::Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	double fVel = m_pAxis[nMotionId]->m_stParam.Speed.fVel;
	double fAcc = m_pAxis[nMotionId]->m_stParam.Speed.fAcc;
	double fDec = m_pAxis[nMotionId]->m_stParam.Speed.fDec;

	return m_pAxis[nMotionId]->StartPtPMotion(*pTgtPos, fVel, fAcc, fDec, bAbs, bWait);
}

BOOL CNmcDevice::Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	return m_pAxis[nMotionId]->StartSCurveMove(*pTgtPos, dRatio, bAbs, bWait);
}

BOOL CNmcDevice::Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait, BOOL bOptimize)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	return m_pAxis[nMotionId]->StartPtPMotion(*pTgtPos, dSpd, dAcc, dDec,bAbs, bWait);
}

BOOL CNmcDevice::InitListMotion()
{
	m_dStartScanPos = 0.0;
	m_dStartFocusPos = 0.0;

	if (!m_pBufferControl)
		m_pBufferControl = new CBufferControl(m_nBoardId, m_nGroupID_RTAF, SCAN_AXIS + m_nOffsetAxisID, FOCUS_AXIS + m_nOffsetAxisID);

	//if (IsListMotion())
	//	UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF); //20211001-syd

	ClearUserBuffer();
	//Grouping2Ax(m_nBoardId, m_nGroupID_RTAF, SCAN_AXIS, FOCUS_AXIS); //20211001-syd
	m_bListMotion = TRUE;

	return BufferInit();
}


void CNmcDevice::RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos)
{
	m_dStartScanPos = dStartScanPos;
	m_dStartFocusPos = dStartFocusPos;
}

void CNmcDevice::LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir, CString sPath)
{
	CString strPath;
	strPath.Format(_T("%sAddList.txt"), pGlobalDoc->m_strSharedDir);
	if (nScanDir == FORWARD)
		m_sAddListLogTitle.Format(_T("Forward"));
	else
		m_sAddListLogTitle.Format(_T("BackWard"));

	CFileFind finder;
	CFile file;
	if (finder.FindFile(strPath))
	{
		DeleteFile(strPath);
	}

	if (!file.Open(strPath, CFile::modeCreate | CFile::modeWrite, nullptr))
	{
		m_bLogAddList = FALSE;
		AfxMessageBox(_T("Fail to create file."));
	}
	else
	{
		m_bLogAddList = TRUE;
		file.Close();
	}

	if (m_bLogAddList)
	{
		CString strData, strIdx;
		strIdx.Format(_T("Index%03d"), GetAddListNum() - 1);
		strData.Format(_T("%.3f, %.3f, %.3f"), dStartScanPos, dStartFocusPos, fScanVel);
		::WritePrivateProfileString(m_sAddListLogTitle, strIdx, strData, strPath);
	}

}

double CNmcDevice::GetStartListMotionPos(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		if (m_dStartScanPos != 0.0)
			return m_dStartScanPos;
		else
			return GetActualPosition(SCAN_AXIS);
		break;
	case 1:
		if (m_dStartFocusPos != 0.0)
			return m_dStartFocusPos;
		else
			return GetActualPosition(FOCUS_AXIS);
		break;
	}

	return 0.0;
}

//void CNmcDevice::AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed) //20211001-syd
//{
//	double dPos1, dPos2, dVel;
//	dPos1 = m_pAxis[SCAN_AXIS]->LenToPulse(fScanMotPos);
//	dPos2 = m_pAxis[FOCUS_AXIS]->LenToPulse(dFocusMotPos);
//	dVel = m_pAxis[SCAN_AXIS]->LenToPulse(dScanSpeed);
//
//	m_pBufferControl->AddBufferMotion2Ax(dPos1, dPos2, dVel);
//	//2021 09 23 lgh 여기서 그냥 바로 넣는다 1000개 이상 나오지가 않음.
//	m_pBufferControl->BufferToMMCE(m_pBufferControl->m_nAddListIndex);
//
//	m_pBufferControl->m_nAddListIndex++;
//	return;
//}

void CNmcDevice::AddList(double fScanMotPos, double dFocusMotPos) //20211001-syd
{
	double dPrevPos1, dPos1, dPos2, dVel;
	dPrevPos1 = m_pAxis[SCAN_AXIS]->LenToPulse(m_dPrevPos1);
	dPos1 = m_pAxis[SCAN_AXIS]->LenToPulse(fScanMotPos);
	dPos2 = m_pAxis[FOCUS_AXIS]->LenToPulse(dFocusMotPos);
	if(m_bListMotion==0)	
		InitListMotion();

	m_pBufferControl->AddBufferMotion2Ax(dPrevPos1, dPos1, dPos2);

	m_dPrevPos1 = fScanMotPos;
	m_pBufferControl->m_nAddListIndex++;
	return;
}

void CNmcDevice::StartListMotion()
{
	m_dPrevPos1 = GetActualPosition(SCAN_AXIS); //20211001-syd
	m_pBufferControl->StartMotion();
	return;
}

void CNmcDevice::StopListMotion()
{
	m_pBufferControl->StopMotion();
	return;
}

void CNmcDevice::ShutdownHomeThreadAll()
{
	m_bExit = 1;
	for (int i = 0; i < m_nTotalAxisNum; i++)	//20110922 hyk mod
	{
		if (!m_pAxis[i])
		{
			m_pAxis[i]->StopHomeThread();
		}
	}
	m_bExit = 0;
}


double CNmcDevice::GetActualVelocity(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return 0.0;

	return m_pAxis[nAxisId]->GetActVel(); // [mm]
}

void CNmcDevice::ClearAutoFocusSmoothingData()	// 20151201 - syd
{
	m_dTempOffsetAf[0] = 0.0;
	m_dTempOffsetAf[1] = 0.0;
	m_dTempOffsetAf[2] = 0.0;
	m_dTempOffsetAf[4] = 0.0;
}

double CNmcDevice::SetAutoFocusSmoothing(double dFocusOffset)	// 20151201 - syd
{
	double dVal = 0.0;
	int nIdx = m_nSeqIdx % 4;
	m_dTempOffsetAf[nIdx] = dFocusOffset;

	switch (m_nSeqIdx)
	{
	case 0:
		dVal = m_dTempOffsetAf[0];
		break;
	case 1:
		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1]) / 2.0;
		break;
	case 2:
		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1] + m_dTempOffsetAf[2]) / 3.0;
		break;
	default:
		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1] + m_dTempOffsetAf[2] + m_dTempOffsetAf[3]) / 4.0;
		break;
	}

	return dVal;
}

void CNmcDevice::StopSequence()
{
	ClearUserBuffer();
}

void CNmcDevice::ClearUserBuffer()
{
	int i = 0;

	for (i = 0; i < 2048; i++)
	{
		m_dUserBuffer[i] = 0;
	}
}

// 속도,가속도 지정.
BOOL CNmcDevice::SetVMove(int nAxisID, double fVel, double fAcc)
{
	if (nAxisID >= MAX_AXIS)
		return FALSE;

	if (fVel == 0.0 || fAcc <= 0.0)
		return FALSE;

	double dDelay = 0.0;
	//m_Traj.velocity = m_pObjectMotor[nMotorID].Len2Pulse(fVel); // [mm/sec] -> [counts/sec]
	//m_Traj.acceleration = m_pObjectMotor[nMotorID].Len2Pulse(fAcc); // [m/sec^2] -> [counts/sec^2]
	//m_Traj.deceleration = m_pObjectMotor[nMotorID].Len2Pulse(fAcc); // [m/sec^2] -> [counts/sec^2]
	//m_Traj.jerkPercent = 15.0;
	//m_ParamVMove.velocity.trajectory = &m_Traj;
	//m_ParamVMove.attributes.delay = &dDelay;

	//GetAxis(nAxisID)->LenToPulse(fVel);
	m_dVel[nAxisID] = fVel;
	m_dAcc[nAxisID] = fAcc;

	GetAxis(nAxisID)->SetVMove(fVel, fAcc);

	return TRUE;
}

BOOL CNmcDevice::VMove(int nMotionId, int nDir)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

	return (GetAxis(nMotionId)->VMove(nDir));
}

void CNmcDevice::SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID)
{
	m_nBoardId = nBoardId;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = nOffsetAxisID;
//	m_nGroupID = nGroupID;

	for (int nAxis = 0; nAxis < m_nTotalAxisNum; nAxis++)
	{
		if (m_pAxis[nAxis])
			m_pAxis[nAxis]->SetConfigure(nBoardId, nDevIdIoIn, nDevIdIoOut, nOffsetAxisID);
	}
}

void CNmcDevice::OutBit(long bit, bool flag)
{

	if (bit < 8)
	{
		MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 0, bit, flag);
		m_OutSegInfo[0][bit] = 1;
		m_OutSegInfoOn[0][bit] = flag;
	}
	else if (bit < 16)
	{
		MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 1, bit - 8, flag);
		m_OutSegInfo[1][bit-8] = 1;
		m_OutSegInfoOn[1][bit-8] = flag;
	}
	else if (bit < 24)
	{
		MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 2, bit - 16, flag);
		m_OutSegInfo[2][bit] = 1;
		m_OutSegInfoOn[2][bit-16] = flag;
	}
	else
	{
		MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 3, bit - 24, flag);
		m_OutSegInfo[3][bit] = 1;
		m_OutSegInfoOn[3][bit-24] = flag;
	}
	return;
	
	m_dwDeviceIoOut = ReadOut();

	if (flag)
	{
		m_dwDeviceIoOut |= (0x00000001 << bit); //OFF is High Voltage.
	}
	else
	{
		m_dwDeviceIoOut &= ~(0x00000001 << bit); //ON is Low Voltage.
	}

	Out32((long)m_dwDeviceIoOut);
	
}

 BOOL CNmcDevice::SetSliceIo(int nSegment, int nBit, BOOL bOn)
{
	 if (nSegment == 2 || nSegment == 3)
	 {

		 if (nSegment == 2)
		 {
			 if (nBit < 8)
			 {
				 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 0, nBit, bOn);
				 m_OutSegInfo[0][nBit] = 1;
				 m_OutSegInfoOn[0][nBit] = bOn;
			 }
			 else if (nBit < 16)
			 {
				 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 1, nBit - 8, bOn);
				 m_OutSegInfo[1][nBit-8] = 1;
				 m_OutSegInfoOn[1][nBit-8] = bOn;
			 }
		 }
		 else
		 {
			 if (nBit < 8)
			 {
				 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 2, nBit, bOn);
				 m_OutSegInfo[2][nBit] = 1;
				 m_OutSegInfoOn[2][nBit] = bOn;
			 }
			 else if (nBit < 16)
			 {
				 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 3, nBit - 8, bOn);
				 m_OutSegInfo[3][nBit-8] = 1;
				 m_OutSegInfoOn[3][nBit-8] = bOn;
			 }
		 }

		 /*
		 // if (nSegment == 2)
		 //	 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 4, nBit, bOn);
		 // else
		 //	 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 5, nBit - 16, bOn);
		 
		 m_dwDeviceIoOut = ReadOut();
		 if (bOn)
			 m_dwDeviceIoOut |= (0x00000001 << nBit); //OFF is High Voltage.
		 else
			 m_dwDeviceIoOut &= ~(0x00000001 << nBit); //ON is Low Voltage.

		 Out32((long)m_dwDeviceIoOut);
		 */
	 }
	 else if (nSegment == 5)
	 {
		 if (nBit < 8)
		 {
			 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 4, nBit, bOn);
			 m_OutSegInfo[4][nBit] = 1;
			 m_OutSegInfoOn[4][nBit] = bOn;
		 }
		 else if (nBit < 16)
		 {
			 MC_IO_WRITE_BIT(m_nBoardId, m_nDevIdIoOut, 5, nBit - 8, bOn);
			 m_OutSegInfo[5][nBit-8] = 1;
			 m_OutSegInfoOn[5][nBit-8] = bOn;
		 }

		 /*
		 UINT8 val[2];

		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 4, 1, (UINT8*)&val[0]);
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 5, 1, (UINT8*)&val[1]);
	
		 int nData = 0;
		 nData |= val[0] & 0xFF;
		 nData |= (val[1] & 0xFF) << 8;

		 if (bOn)
			 nData |= (0x00000001 << nBit); //OFF is High Voltage.
		 else
			 nData &= ~(0x00000001 << nBit); //ON is Low Voltage.

		 val[0] = (nData >> 0) & 0xFF;
		 val[1] = (nData >> 8) & 0xFF;

		 MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 4, 1, (UINT8*)&val[0]);
		 MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 5, 1, (UINT8*)&val[1]);
		 */
	 }

	return 1;
}

 BOOL CNmcDevice::SetSliceIo(int nSegment, unsigned long ulOut)
{
	UINT8 val[2];

	val[0] = (ulOut >> 0) & 0xFF;
	val[1] = (ulOut >> 8) & 0xFF;
		 
	if (nSegment == 2)
	{
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 0, 1, (UINT8*)&val[0]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 1, 1, (UINT8*)&val[1]);
	}
	else if(nSegment ==3)
	{
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 2, 1, (UINT8*)&val[0]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 3, 1, (UINT8*)&val[1]);
	}
	else if (nSegment == 5)
	{
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 4, 1, (UINT8*)&val[0]);
		MC_IO_WRITE(m_nBoardId, m_nDevIdIoOut, 5, 1, (UINT8*)&val[1]);
	}
	 
	return 1;
}

 BOOL CNmcDevice::IsOutSliceIo(int nSegment, int nBit)
 {
	 int nRtn = MC_OK;
	 UINT uBitSize = 32;
	 UINT uDataSize = uBitSize / 8;
	 long nData = 0;

	 UINT8 val[4];
	 uDataSize = 1;		// [Byte]

	//IO 상태 Read
	 if (nSegment == 2)
	 {
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 0, uDataSize, (UINT8*)&val[0]);
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 1, uDataSize, (UINT8*)&val[1]);
	 }
	 else if (nSegment == 3)
	 {
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 2, uDataSize, (UINT8*)&val[0]);
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 3, uDataSize, (UINT8*)&val[1]);
	 }
	 else if (nSegment == 5)
	 {
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 4, uDataSize, (UINT8*)&val[0]);
		 MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 5, uDataSize, (UINT8*)&val[1]);
	 }

	 nData |= val[0] & 0xFF;
	 nData |= (val[1] & 0xFF) << 8;

	 int nRet = ((nData >> nBit) & 0x01);

	 return nRet;
 }


long CNmcDevice::ReadOut()
{
	int nRtn = MC_OK;
	UINT uBitSize = 32;
	UINT uDataSize = uBitSize / 8;
	long nData = 0;

	UINT8 val[4];
	uDataSize = 1;		// [Byte]

						//IO 상태 Read
	MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 0, uDataSize, (UINT8*)&val[0]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 1, uDataSize, (UINT8*)&val[1]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 2, uDataSize, (UINT8*)&val[2]);
	MC_IO_READ(m_nBoardId, m_nDevIdIoOut, BUF_OUT, 3, uDataSize, (UINT8*)&val[3]);

	nData |= val[0] & 0xFF;
	nData |= (val[1] & 0xFF) << 8;
	nData |= (val[2] & 0xFF) << 16;
	nData |= (val[3] & 0xFF) << 24;

	return (nData);
}

BOOL CNmcDevice::ReadIn(long bit)
{
	unsigned long nData;
	In32(&nData);

	return ((nData>>bit) & 0x01) ? TRUE : FALSE;
}

BOOL CNmcDevice::ReadOut(long bit)
{
	long nData = ReadOut();

	return ((nData >> bit) & 0x01) ? TRUE : FALSE;
}

BOOL CNmcDevice::GantryEnable(long lMaster, long lSlave, long lOnOff)
{
	TCHAR msg[MAX_ERR_LEN];
	MC_STATUS ms = MC_OK;
	char cstrErrorMsg[MAX_ERR_LEN];

	UINT32 state = 0x00000000;
	DWORD nTick = GetTickCount();

	ms = MC_GantryReadStatus(m_nBoardId, 0, &state);

	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-MC_GantryReadStatus(220)"));
		return FALSE;
	}

	nTick = GetTickCount();
	while ( !((state & mcGantry_MotionCompleted) && (state & mcGantry_YawStable)) && state)
	{
		if (GetTickCount() - nTick >= 60000)
		{
			AfxMessageBox(_T("Error-StartRsaHoming(221)"));
			return FALSE;
		}
		Sleep(100);

		ms = MC_GantryReadStatus(m_nBoardId, 0, &state);
		if (ms != MC_OK)
		{
			AfxMessageBox(_T("Error-StartRsaHoming(202)"));
			return FALSE;
		}
	}


	if(lOnOff)
	{
		ms = MC_GantryEnable(m_nBoardId, 0, lMaster + m_nOffsetAxisID, lSlave + m_nOffsetAxisID, 50, 10000000);
		if (ms != MC_OK)
		{
			AfxMessageBox(_T("Error-MC_GantryEnable"));
			return FALSE;
		}
		m_bGantryEnabled = TRUE;
		Sleep(50);
		return TRUE;
	}
	else
	{
		ms = MC_GantryDisable(m_nBoardId, 0);
		if (ms != MC_OK)
		{
			AfxMessageBox(_T("Error-MC_GantryDisable"));
			return TRUE;		
		}
		m_bGantryEnabled = FALSE;
		Sleep(50);
		return FALSE;
	}

	//Sleep(300);

	return TRUE;
}

BOOL CNmcDevice::GetGantry(long lMaster, long lSlave, long *lOnOff)
{
	TCHAR msg[MAX_ERR_LEN];
	MC_STATUS ms = MC_OK;
	char cstrErrorMsg[MAX_ERR_LEN];
	UINT32 Status = 0;
	CString sMsg;

	//ms = MC_ReadStatus(m_nBoardId, lMaster + m_nOffsetAxisID, &Status);
	ms = MC_GantryReadStatus(m_nBoardId, 0, &Status);
	if (ms != MC_OK)
	{
		*lOnOff = FALSE;
		AfxMessageBox(_T("Error-MC_GantryStatus"));
		return FALSE;
	}
	else if(Status & mcGantry_Fault)
	{
		*lOnOff = FALSE;
		sMsg.Format(_T("Error-MC_GantryStatus (0x%08x)"), Status);
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else if (Status & mcGantry_MotionCompleted)// || Status & mcGantry_YawStable
	{
		*lOnOff = TRUE;
	}
	else
		*lOnOff = FALSE;

	return TRUE;
}

BOOL CNmcDevice::StartGantrySlaveMove(int nAxisId, BOOL bAbs, double fPos, double fVel, double fAcc, double fDec, double fJerk, byte nDispMode)
{
	return m_pAxis[nAxisId]->StartGantrySlaveMove(bAbs, fPos, fVel, fAcc, fDec, fJerk, nDispMode);
}

BOOL CNmcDevice::TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
{
	double dOffset = 0.0;
	MC_STATUS ms = MC_OK;

#ifdef SCAN_S_AXIS
	dOffset = GetAxis(SCAN_S_AXIS)->GetActualPosition();
#endif
	ULONGLONG nTickTestTime = GetTickCount64();
	ULONGLONG nTickTestTimeEnd = GetTickCount64();
	BOOL bENCInvert = 1;
	CString strMsg;

	if(!m_bSetTriggerConf[vAxisId])
	{

		if (vAxisId < 5)
#if MACHINE_MODEL == INSMART_ULTRA2
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, 0);
#else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 0, 1, 1, 1, 0);
#endif
		else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 1, 1, 1, 0);


		if(ms == MC_OK)
			m_bSetTriggerConf[vAxisId] = TRUE;

	//	ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 4);
	}
	


	/*Trigger Configuration 설정, 선택 Trigger Index 의 Trigger 출력을 위한 기본 파라미터를  설정 한다.
	BoardID,
	DeviceID : BoardID, FunctionModule DeviceID
	TriggerIndex : Trigger Index (1~3, 5~7 선택 가능)
	EncSorceIndex : Encoder Sorce (0~7 선택 가능)
	InvertEncDir : Trigger Position 설정 시 방향 반전. (false : NoInvert, true : Invert)
	EncRatio : Trigger Position 설정 시 Ratio 반영, Encoder Ratio 값 설정
	VLevel : (0:24V, 1:5V, 2:Both)  (5V 신호는 각 축 Axis Cable 11,12 Pin, 24V 신호는 DIO Cable 에서 출력 된다)
	ActiveLevel : Trigger 출력 Active Level 설정 (true : High, false : Low)
	Mode : 0:Interval, 1:Table, 2:OneShot, 3 : Frequency*/

	//if (vAxisId < 5)
	//	ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 0);
	//else
	//	ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 4);


	if (vAxisId < 5)
	{
		double nEncPos = 0;// pMotion->NMC_GetTriggerEnc(pMotion->m_nTriggerEtherCatAddr, 0x2410);

#if MACHINE_MODEL == INSMART_ULTRA2
		nEncPos = NMC_GetTriggerEnc(nEcatAddr, 0x2410) * -1.0;
#else
		nEncPos = NMC_GetTriggerEnc(nEcatAddr, 0x2410);// *-1.0;
#endif


		double dCurPosition = GetActualPosition(SCAN_AXIS);

		double dCurPos = m_pAxis[SCAN_AXIS]->LenToPulse(dCurPosition);

		double dPulseDiff = fabs(dCurPos - nEncPos);

		//double dStartPos = m_pAxis[SCAN_AXIS]->LenToPulse(dStPos) - m_pAxis[SCAN_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
		//double dEndPos = m_pAxis[SCAN_AXIS]->LenToPulse(dEdPos) - m_pAxis[SCAN_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
		double dStartPos = m_pAxis[SCAN_AXIS]->LenToPulse(dStPos)- dPulseDiff;		// Function Module Encoder counting is reversed...
		double dEndPos = m_pAxis[SCAN_AXIS]->LenToPulse(dEdPos) - dPulseDiff;		// Function Module Encoder counting is reversed...
		UINT16 usPeriod = (UINT16)(dTrigRes / (m_pAxis[SCAN_AXIS]->m_stParam.Motor.fPosRes * 1000.0) + 0.5); // 반올림....
		UINT16 usPulseWidth = (UINT16)((dPulseWd * 1000.0) / 50.0);

		//TR_SetIntervalTriggerData(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, usPulseWidth);
		MC_STATUS status=TR_SetIntervalTriggerData_WEN(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, usPulseWidth, true, 1);

#ifdef _DEBUG
		nTickTestTimeEnd = GetTickCount64() - nTickTestTime;

		strMsg.Format(_T("TR_SetIntervalTriggerEnable 3:%dms"), nTickTestTimeEnd);
		SaveLog(strMsg);
#endif

		//TR_SetIntervalTriggerEnable(m_nBoardId, nEcatAddr, vAxisId, 1, 1);
	}
	else
	{

		double nEncPos = 0;// pMotion->NMC_GetTriggerEnc(pMotion->m_nTriggerEtherCatAddr, 0x2410);

		nEncPos = NMC_GetTriggerEnc(nEcatAddr, 0x3410);

		double dCurPosition = GetActualPosition(VERIFY_AXIS);

		double dCurPos = m_pAxis[VERIFY_AXIS]->LenToPulse(dCurPosition);
		
		double dPulseDiff=fabs(dCurPos - nEncPos);
		
		double dStartPos = m_pAxis[VERIFY_AXIS]->LenToPulse(dStPos) - dPulseDiff;// -m_pAxis[VERIFY_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
		double dEndPos = m_pAxis[VERIFY_AXIS]->LenToPulse(dEdPos) - dPulseDiff;// -m_pAxis[VERIFY_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
		UINT16 usPeriod = (UINT16)(dTrigRes / (m_pAxis[VERIFY_AXIS]->m_stParam.Motor.fPosRes * 1000.0) + 0.5); // 반올림....
																											   //UINT16 usPulseWidth = (UINT16)((dPulseWd * 1000.0) / 50.0);

		//TR_SetIntervalTriggerData(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, 10000);
		TR_SetIntervalTriggerData_WEN(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, 10000, true, 1);
		TR_SetIntervalTriggerData_WEN(m_nBoardId, nEcatAddr, vAxisId+1, dStartPos, dEndPos, usPeriod, 10000, true, 1);


		//TR_SetIntervalTriggerEnable(m_nBoardId, nEcatAddr, vAxisId, 1, 1);
	}

#ifdef _DEBUG
	nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
	strMsg;
	strMsg.Format(_T("TR_SetIntervalTriggerEnable end:%dms"), nTickTestTimeEnd);
	SaveLog(strMsg);
#endif

	return TRUE;
}


//BOOL CNmcDevice::TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//{
//	double dOffset = 0.0;
//	MC_STATUS ms = MC_OK;
//
//#ifdef SCAN_S_AXIS
//	dOffset = GetAxis(SCAN_S_AXIS)->GetActualPosition();
//#endif
//	ULONGLONG nTickTestTime = GetTickCount64();
////	NMC_Trigger trigger;
//
//	BOOL bENCInvert = 1;
//
//	//if (vAxisId < 5)
//	//{
//	//	ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, 0);
//	//	ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 0);
//	//}
//	//else
//	//{
//	//	ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 0, 1, 0, 0);
//	//	ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 4);
//	//}
//	if (vAxisId < 5)
//		ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, 0);
//	else
//		ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 1, 1, 1, 0);
//
//#ifdef _DEBUG
//	ULONGLONG nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
//	CString strMsg;
//	strMsg.Format(_T("TR_SetIntervalTriggerEnable 1:%dms"), nTickTestTimeEnd);
//	//pMainFrame->DispStatusBar(strMsg);
//	SaveLog(strMsg);
//#endif
//
//	/*Trigger Configuration 설정, 선택 Trigger Index 의 Trigger 출력을 위한 기본 파라미터를  설정 한다.
//	BoardID, 
//	DeviceID : BoardID, FunctionModule DeviceID
//	TriggerIndex : Trigger Index (1~3, 5~7 선택 가능)
//	EncSorceIndex : Encoder Sorce (0~7 선택 가능)
//	InvertEncDir : Trigger Position 설정 시 방향 반전. (false : NoInvert, true : Invert)
//	EncRatio : Trigger Position 설정 시 Ratio 반영, Encoder Ratio 값 설정
//	VLevel : (0:24V, 1:5V, 2:Both)  (5V 신호는 각 축 Axis Cable 11,12 Pin, 24V 신호는 DIO Cable 에서 출력 된다)
//	ActiveLevel : Trigger 출력 Active Level 설정 (true : High, false : Low)
//	Mode : 0:Interval, 1:Table, 2:OneShot, 3 : Frequency*/
//
//	if (vAxisId < 5)
//		ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 0);
//	else
//		ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 4);
//
//#ifdef _DEBUG
//	nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
//	strMsg;
//	strMsg.Format(_T("TR_SetIntervalTriggerEnable 2:%dms"), nTickTestTimeEnd);
//	//pMainFrame->DispStatusBar(strMsg);
//	SaveLog(strMsg);
//#endif
//
//	if (vAxisId < 5)
//	{
//		double dCurPosition = GetActualPosition(SCAN_AXIS);
//
//		double dStartPos = m_pAxis[SCAN_AXIS]->LenToPulse(dStPos) - m_pAxis[SCAN_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
//		double dEndPos = m_pAxis[SCAN_AXIS]->LenToPulse(dEdPos) - m_pAxis[SCAN_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
//		UINT16 usPeriod = (UINT16)(dTrigRes / (m_pAxis[SCAN_AXIS]->m_stParam.Motor.fPosRes * 1000.0) + 0.5); // 반올림....
//		UINT16 usPulseWidth = (UINT16)((dPulseWd * 1000.0) / 50.0);
//
//		TR_SetIntervalTriggerData(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, usPulseWidth);
//		//	Sleep(50);
//
//#ifdef _DEBUG
//		nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
//		strMsg;
//		strMsg.Format(_T("TR_SetIntervalTriggerEnable 3:%dms"), nTickTestTimeEnd);
//		//pMainFrame->DispStatusBar(strMsg);
//		SaveLog(strMsg);
//#endif
//
//		TR_SetIntervalTriggerEnable(m_nBoardId, nEcatAddr, vAxisId, 1, 1);
//		//Sleep(10);
//	}
//	else
//	{
//		double dCurPosition = GetActualPosition(VERIFY_AXIS);
//
//		double dStartPos = m_pAxis[VERIFY_AXIS]->LenToPulse(dStPos) - m_pAxis[VERIFY_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
//		double dEndPos = m_pAxis[VERIFY_AXIS]->LenToPulse(dEdPos) - m_pAxis[VERIFY_AXIS]->LenToPulse(dCurPosition);		// Function Module Encoder counting is reversed...
//		UINT16 usPeriod = (UINT16)(dTrigRes / (m_pAxis[VERIFY_AXIS]->m_stParam.Motor.fPosRes * 1000.0) + 0.5); // 반올림....
//		//UINT16 usPulseWidth = (UINT16)((dPulseWd * 1000.0) / 50.0);
//
//		TR_SetIntervalTriggerData(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, 10000);
//		//	Sleep(50);
//#ifdef _DEBUG
//		nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
//		strMsg;
//		strMsg.Format(_T("TR_SetIntervalTriggerEnable 3:%dms"), nTickTestTimeEnd);
//		//pMainFrame->DispStatusBar(strMsg);
//		SaveLog(strMsg);
//#endif
//
//		TR_SetIntervalTriggerEnable(m_nBoardId, nEcatAddr, vAxisId, 1, 1);
//		//Sleep(10);
//	}
//
//#ifdef _DEBUG
//	nTickTestTimeEnd = GetTickCount64() - nTickTestTime;
//    strMsg;
//	strMsg.Format(_T("TR_SetIntervalTriggerEnable end:%dms"), nTickTestTimeEnd);
//	//pMainFrame->DispStatusBar(strMsg);
//	SaveLog(strMsg);
//#endif
//
//	return TRUE;
//}
void CNmcDevice::NMC_GetTriggetMode(int nEcatAddr, int vAxisId, int& mode)
{
	MC_STATUS ms = MC_OK;

	UINT32 uRspsDataSize = 0;
	UINT8 Data[4];
	int nSdoIdx = 0;
	if (vAxisId == 1)
	{
		nSdoIdx = 0x2411;
	}
	else if (vAxisId == 2)
	{
		nSdoIdx = 0x2811;
	}
	else if (vAxisId == 3)
	{
		nSdoIdx = 0x2c11;
	}
	else if (vAxisId == 5)
	{
		nSdoIdx = 0x3411;
	}
	else if (vAxisId == 6)
	{
		nSdoIdx = 0x3811;
	}
	else if (vAxisId == 7)
	{
		nSdoIdx = 0x3c11;
	}

	ms = MasterGetSDODataEcatAddr(m_nBoardId, nEcatAddr, nSdoIdx, 0, 4, &uRspsDataSize, Data);

	int iCurPos = ToInt32(Data);

	CString str;
	str.Format(_T("%x"), iCurPos);

	mode = str.GetAt(0)-'0';

	if (mode == 3)
		mode = 0;
	else if (mode == 5)
		mode = 1;
	else if (mode == 7)
		mode = 2;
	else
		mode = 3;
}






void CNmcDevice::NMC_TriggerOneShot(int nEcatAddr, int vAxisId,double dWidth)
{
	MC_STATUS ms = MC_OK;

	int nMode = 0;
	NMC_GetTriggetMode(nEcatAddr, vAxisId, nMode);

	//one shot 모드가 아니면 모드로 체인지
	if (nMode != Mode_OneShot)
	{
		if (vAxisId < 5)
		{
#if MACHINE_MODEL == INSMART_ULTRA2
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, Mode_OneShot);
#else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 0, 1, 1, 1, Mode_OneShot);
#endif
		}
		else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 1, 1, 1, Mode_OneShot);
	}

	UINT16 usPulseWidth = (UINT16)((dWidth * 1000.0) / 50.0);

	TR_TriggerOneShot(m_nBoardId, nEcatAddr, vAxisId, 10000);
}


BOOL CNmcDevice::TriggerStop(int nEcatAddr, int vAxisId)
{
	//NMC_Trigger trigger;

	MC_STATUS ms = MC_OK;

	//TR_SetIntervalTriggerEnable(m_nBoardId, nEcatAddr, vAxisId, 0, 1);
	TR_SetIntervalTriggerData_WEN(m_nBoardId, nEcatAddr, vAxisId, 1000, 2000, 200, 100, false, 1);

	//Sleep(50);

	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerStop()"));
		return FALSE;
	}

/*
	MC_STATUS ms = MC_OK;

	ms = MC_WriteIntervalTrigEnableFM(m_nBoardId, nEcatAddr, vAxisId, false);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerStop()"));
		return FALSE;
	}
	Sleep(100);
*/
	return TRUE;
}

BOOL CNmcDevice::TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx)
{
	MC_STATUS ms = MC_OK;

	//NMC_Trigger trigger;

	//if (vAxisId < 5)
	//	ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, 0);
	//else
	//	ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 1, 1, 1, 0);

	if (!m_bSetTriggerConf[vAxisId])
	{

		if (vAxisId < 5)
		{
#if MACHINE_MODEL == INSMART_ULTRA2
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 1, 1, 1, 1, 0);
#else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 0, 0, 1, 1, 1, 0);
#endif
		}
		else
			ms = TR_SetTriggerConfig(m_nBoardId, nEcatAddr, vAxisId, 4, 0, 1, 1, 1, 0);

		if(ms == MC_OK)
			m_bSetTriggerConf[vAxisId] = TRUE;
	}

	if (vAxisId < 5)
		ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 0);
	else
		ms = TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, 4);

	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerSetOriginPos()"));
		return FALSE;
	}

	//return TR_TriggerEncoderCounterClear(m_nBoardId, nEcatAddr, vAxisId);

	/*
	UINT32 uRspsDataSize = 0;
	UINT8 Data[4];
	double dCurPos;

	Sleep(100);
	ms = MasterGetSDODataEcatAddr(m_nBoardId, nEcatAddr, nSdoIdx, 7, 4, &uRspsDataSize, Data);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-MasterGetSDODataEcatAddr()"));
		return FALSE;
	}
	Sleep(100);

	int iCurPos = ToInt32(Data);

	dCurPos = (double)iCurPos;

	ms = MC_WriteParameter(m_nBoardId, vAxisId, 2147, -dCurPos); // ??? -dCurPos
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerSetOriginPos()"));
		return FALSE;
	}

	m_nTriggerOrgPos = -dCurPos;
	Sleep(100);
	*/

	return TRUE;
}

int CNmcDevice::GetTriggerEnc(int nEcatAddr, int nSdoIdx)
{
	MC_STATUS ms = MC_OK;

	UINT32 uRspsDataSize = 0;
	UINT8 Data[4];

	ms = MasterGetSDODataEcatAddr(m_nBoardId, nEcatAddr, nSdoIdx, 7, 4, &uRspsDataSize, Data);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-GetTriggerEncCnt()"));
		return 0;
	}
	//Sleep(30);

	int iCurPos = ToInt32(Data);

	return(iCurPos);
}


int CNmcDevice::NMC_GetTriggerCounter(int nEcatAddr, int nSdoIdx)
{
	MC_STATUS ms = MC_OK;

	UINT32 uRspsDataSize = 0;
	UINT8 Data[4];

	ms = MasterGetSDODataEcatAddr(m_nBoardId, nEcatAddr, nSdoIdx, 2,4, &uRspsDataSize, Data);
	if (ms != MC_OK)
	{
		//AfxMessageBox(_T("Error-GetTriggerEncCnt()"));
		return 0;
	}
	//Sleep(30);

	int iCurPos = ToInt32(Data);

	return(iCurPos);
}

CString CNmcDevice::GetTriggerEncCnt(int nEcatAddr, int nSdoIdx)
{
	MC_STATUS ms = MC_OK;

	UINT32 uRspsDataSize = 0;
	UINT8 Data[4];


	ms = MasterGetSDODataEcatAddr(m_nBoardId, nEcatAddr, nSdoIdx, 7, 4, &uRspsDataSize, Data);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-GetTriggerEncCnt()"));
		return _T("");
	}
	//Sleep(30);

	int iCurPos = ToInt32(Data);

	return(ToCString((I32)iCurPos));

	//I32 EncValue;
	//double dCurPos = (double)iCurPos;

	//double dScale = 0;
	//ms = MC_ReadParameter(m_nBoardId, vAxisId, 2071, &dScale);

	//EncValue = (I32)(dCurPos / dScale);

	//return(ToCString(EncValue));
}


int CNmcDevice::UnGroup2Ax(int nGroupNum)
{
	MC_STATUS ms;

	ms = MC_GroupDisable(m_nBoardId, nGroupNum);
	ms = MC_UngroupAllAxes(m_nBoardId, nGroupNum);
	Sleep(5);

	return ms;
}

void CNmcDevice::UnGroup2Ax(int nBdNum, int nGroupNum)
{
	if (m_nGroupID_RTAF == nGroupNum)
		;// m_bListMotion = FALSE;
	else if(m_nGroupID_ErrMapXY == nGroupNum)
		m_bErrMap = FALSE;
	else if (m_nGroupID_Interpolation == nGroupNum)
		m_bInterpolationMotion = FALSE;

	//if (m_pBufferControl)
	//	m_pBufferControl->UnGroup2Ax(nBdNum, nGroupNum);
	//else //20211001-syd
		UnGroup2Ax(nGroupNum);

	return;

	//MC_STATUS ms = MC_OK;

	//ms = MC_GroupDisable(m_nBoardId, m_nGroupID);
	//if (ms != MC_OK)
	//{
	//	AfxMessageBox(_T("Error-MC_GroupDisable()"));
	//	return;
	//}

	//ms = MC_UngroupAllAxes(m_nBoardId, m_nGroupID);
	//if (ms != MC_OK)
	//{
	//	AfxMessageBox(_T("Error-MC_UngroupAllAxes()"));
	//	return;
	//}

	//Sleep(5);

}

int CNmcDevice::Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ms; // MC_STATUS

	if (!m_pBufferControl)
	{
		ms = (int)MC_GroupDisable(nBdNum, nGroupNum);
		ms = (int)MC_UngroupAllAxes(nBdNum, nGroupNum);
		Sleep(5);

		ms = (int)MC_AddAxisToGroup(nBdNum, nAxisNumX + m_nOffsetAxisID, nGroupNum, 0);
		ms = (int)MC_AddAxisToGroup(nBdNum, nAxisNumY + m_nOffsetAxisID, nGroupNum, 1);

		ms = (int)MC_GroupEnable(nBdNum, nGroupNum);

		if (m_nGroupID_RTAF == nGroupNum)
		{
			;
			//if (ms == MC_OK)
			//	m_bListMotion = TRUE;
			//else
			//	m_bListMotion = FALSE;
		}
		else if (m_nGroupID_ErrMapXY == nGroupNum)
		{
			if (ms == MC_OK)
				m_bErrMap = TRUE;
			else
				m_bErrMap = FALSE;
		}
		else if (m_nGroupID_Interpolation == nGroupNum)
		{
			if (ms == MC_OK)
				m_bInterpolationMotion = TRUE;
			else
				m_bInterpolationMotion = FALSE;
		}

		return ms;
	}

	if(m_pBufferControl)
		ms = m_pBufferControl->Grouping2Ax(); //20211001-syd
		//ms = m_pBufferControl->Grouping2Ax(nBdNum, nGroupNum, nAxisNumX + m_nOffsetAxisID, nAxisNumY + m_nOffsetAxisID);

	if (m_nGroupID_RTAF == nGroupNum)
	{
		;
		//if (ms == MC_OK)
		//	m_bListMotion = TRUE;
		//else
		//	m_bListMotion = FALSE;
	}
	else if (m_nGroupID_ErrMapXY == nGroupNum)
	{
		if (ms == MC_OK)
			m_bErrMap = TRUE;
		else
			m_bErrMap = FALSE;
	}
	else if (m_nGroupID_Interpolation == nGroupNum)
	{
		if (ms == MC_OK)
			m_bInterpolationMotion = TRUE;
		else
			m_bInterpolationMotion = FALSE;
	}

	return ms;
}

bool CNmcDevice::BufferInit()
{
	if (!m_pBufferControl)
		return FALSE;

	return m_pBufferControl->BufferInit();
}

int CNmcDevice::ToInt32(UINT8* Data)
{
	int iData = 0;

	iData |= (Data[0] << 0) & 0x000000FF;
	iData |= (Data[1] << 8) & 0x0000FF00;
	iData |= (Data[2] << 16) & 0x00FF0000;
	iData |= (Data[3] << 24) & 0xFF000000;

	return iData;
}

CString CNmcDevice::ToCString(I16 data)
{
	CString str;
	str.Format(_T("%d"), data);
	return str;
}

CString CNmcDevice::ToCStringH(I16 data)
{
	CString str;
	str.Format(_T("%x"), data);
	return str;
}

CString CNmcDevice::ToCString(I32 data)
{
	CString str;
	str.Format(_T("%d"), data);
	return str;
}

CString CNmcDevice::ToCStringH(U16 data)
{
	CString str;
	str.Format(_T("%x"), data);
	return str;
}

double CNmcDevice::Len2Pulse(int nIdxMotor, double fLength)
{
	double fPulse = 0.0; 
	fPulse = m_pAxis[nIdxMotor]->LenToPulse(fLength);
	return fPulse;
}

double CNmcDevice::Pulse2Len(int nAxisId, long nPulse)
{
	return m_pAxis[nAxisId]->PulseToLen((double)nPulse);
}

BOOL CNmcDevice::NMC_StartHomeThread(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		return axes->StartHomeThread();
	else
		return 0;
}

BOOL CNmcDevice::NMC_StartHomming(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		return axes->StartHomming();
	else
		return 0;
}

void CNmcDevice::SetOriginStatus(int nAxisId, BOOL bStatus)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		axes->SetOriginStatus(bStatus);
}

BOOL CNmcDevice::GetOriginStatus(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);
	
	if (axes)
		return axes->GetOriginStatus();
	else
		return 0;
}

void CNmcDevice::SetNegSWLimitAction(int nMotorId, int Action)
{
	CNmcAxis* axes = GetAxis(nMotorId);

	if (axes)
		axes->SetNegHWLimitAction(Action);
}

void CNmcDevice::SetPosSWLimitAction(int nMotorId, int Action)
{
	CNmcAxis* axes = GetAxis(nMotorId);

	if (axes)
		axes->SetPosHWLimitAction(Action);
}

BOOL CNmcDevice::IsServoOn(int nMotorID)
{
	CNmcAxis* axes = GetAxis(nMotorID);

	if (axes)
		return axes->GetAmpEnable();

	return FALSE;
}

void CNmcDevice::SetEStopTime(int nMotionId, float fEStopTime)
{
}

BOOL CNmcDevice::SetMsAction(long lMS, int action)
{
	return 0;
}

void CNmcDevice::SetNegSWLimitDirection(int nMotorId, BOOL bDirection)
{
}

void CNmcDevice::SetPosSWLimitDirection(int nMotorId, BOOL bDirection)
{
}

BOOL CNmcDevice::InitRecorder()
{
	return 1;
}

BOOL CNmcDevice::GetRecordLastData(double & dScanPos, double & dThickPos, int bDir)
{
	return 0;
}

BOOL CNmcDevice::IsNegSWLimit(int nMotorId)
{
	CNmcAxis* axes = GetAxis(nMotorId);

	if (axes)
	{
		return (GetActualPosition(nMotorId) <= axes->GetNegSoftwareLimit()) ? TRUE : FALSE;
	}
	else
		return 0;
}

BOOL CNmcDevice::IsPosSWLimit(int nMotorId)
{
	CNmcAxis* axes = GetAxis(nMotorId);

	if (axes)
	{
		return (GetActualPosition(nMotorId) >= axes->GetPosSoftwareLimit()) ? TRUE : FALSE;
	}
	else
		return 0;
}

BOOL CNmcDevice::IsHomeLimit(int nMotorId)
{
	return 0;
}

BOOL CNmcDevice::SetErrMap(CMyFileErrMap * pMyFile)
{
	return ApplyErrorCompensateionTable(NMC_EC_AXISID, TRUE);
}

BOOL CNmcDevice::ResetErrMap()
{
	return ApplyErrorCompensateionTable(NMC_EC_AXISID, 0);
}

BOOL CNmcDevice::AdjustErrMapData(double dAdjVal)
{
	return 0;
}

BOOL CNmcDevice::LoadErrMapFile(CString sPath)
{
	TCHAR sEcPath[MAX_PATH];
	_tcscpy(sEcPath, sPath);
	char *pText = TCHARToChar(sEcPath);

	return LoadErrorCompensationTable(NMC_EC_TYPE_FILE, NMC_EC_MAPID, pText);
}

BOOL CNmcDevice::SetConfigureAxis(int nAxisCount)
{
	SetConfigure(m_nBoardId, m_nDevIdIoIn, m_nDevIdIoOut, m_nOffsetAxisID);
	return 1;
}

long CNmcDevice::CreateAutoFocusingSeq3_31_RTAF(structRTAFInfo * pPointArr, double dRefHeightAtTestPoint, int nTotal, int nDir)
{
	int nCmdIndex = 0;
	long lOper = 0;

	long lSeq = 0;
	long val = 0;

	double dScanPos = GetActualPosition(SCAN_AXIS);
	double dCamPos = GetActualPosition(CAM_AXIS);

	int i;

	int nLastIndex = m_nRTAFSeqIndex;
	double fComparePos = 0.;
	long lComparePos = 0;
	double fCompDataPulse;

	//m_pRTAFPointArr
	BOOL bPositiveMove = 0;

	int nLaserID = nDir == FORWARD ? 0 : 1;

	if (m_nRTAFSeqIndex > 0)
	{
		for (i = 0; i < nTotal; i++)
		{
			if (fabs(m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight - pPointArr[i].dHeight) > 0.001)
			{

				if (fabs(pPointArr[i].dHeight - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight) >= AoiParam()->m_dFocusDiffAcceptance)
					continue;

				//real af buffer
				m_pRTAFPointArr[m_nRTAFSeqIndex] = pPointArr[i];

				if (pPointArr[i].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * AoiParam()->m_dLaserSpotInTableZeroPointCheckRatio)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight;
				}

				if (pPointArr[i].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * 0.5 && pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] > 0.1)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight;
				}

				m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToCompareAtMotionPosByScanAxis = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dPosToMeasureAtMotionPosByScanAxis;
				if (m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight >= m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight)
					bPositiveMove = 1;
				else
					bPositiveMove = 0;

				double dDistance = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToMeasureAtMotionPosByScanAxis - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dPosToMeasureAtMotionPosByScanAxis);
				double dFocusDiff = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight);

				m_pRTAFPointArr[m_nRTAFSeqIndex].dTime = fabs(dDistance / m_dScanSpeed);
				m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = fabs(dFocusDiff / m_pRTAFPointArr[m_nRTAFSeqIndex].dTime);

				if (bPositiveMove)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dPosAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dPosDec;
				}
				else
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dNegAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dNegDec;
				}

				if (AoiParam()->m_bUseApplyFixedAccelAndDeaccel == 0)
				{
					double dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
					double dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
					double dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;

					double dOldAcc = m_pParamMotion[FOCUS_AXIS].Speed.fAcc;
					double dOldMaxAcc = m_pParamAxis[FOCUS_AXIS].fMaxAccel;
					double dOldPeriod = m_pParamAxis[FOCUS_AXIS].fAccPeriod;
					m_pParamMotion[FOCUS_AXIS].Speed.fAcc = AoiParam()->m_dFocusAcc;
					m_pParamAxis[FOCUS_AXIS].fMaxAccel = AoiParam()->m_dFocusMaxAcc;
					m_pParamAxis[FOCUS_AXIS].fAccPeriod = AoiParam()->m_dFocusPeriod;
					
					m_pAxis[FOCUS_AXIS]->CalSpeedProfile(dFocusDiff, dVel, dAcc, dDeAcc);

					m_pParamMotion[FOCUS_AXIS].Speed.fAcc = dOldAcc;
					m_pParamAxis[FOCUS_AXIS].fMaxAccel = dOldMaxAcc;
					m_pParamAxis[FOCUS_AXIS].fAccPeriod = dOldPeriod;


#if _MSC_VER > 1700
					if (_isnan(dVel))
					{
						dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
					}
					if (_isnan(dAcc))
					{
						dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
					}
					if (_isnan(dDeAcc))
					{
						dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;
					}
#endif

					if (bPositiveMove > 0)
					{

					}
					else
					{
						dDeAcc /= 2;
					}

					m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = dVel;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = dAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = dDeAcc;
				}

				//절대위치 포커스로 변경
				if (AoiParam()->m_bUseAbsoluteFocusPosition)
					m_pRTAFPointArr[m_nRTAFSeqIndex].dFocus = m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight + AoiParam()->m_dAbsFocusOrgPosCam1;
				else
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dFocus = m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] + pGlobalDoc->m_fRefFocusPosAtTestPoint;
				}
				m_nRTAFSeqIndex++;
			}
		}
	}
	else
	{
		m_pRTAFPointArr[m_nRTAFSeqIndex] = pPointArr[0];

		if (m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * AoiParam()->m_dLaserSpotInTableZeroPointCheckRatio)
		{
			m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
		}

		if (m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * 0.5 && pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] > 0.1)
		{
			m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
		}

		m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToCompareAtMotionPosByScanAxis = dScanPos;
		m_pRTAFPointArr[m_nRTAFSeqIndex].dFocus = m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] + pGlobalDoc->m_fRefFocusPosAtTestPoint;

		if (m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight >= GetActualPosition(FOCUS_AXIS))
			bPositiveMove = 1;
		else
			bPositiveMove = 0;

		double dDistance = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToMeasureAtMotionPosByScanAxis - dScanPos);
		double dFocusDiff = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - GetActualPosition(FOCUS_AXIS));
		m_pRTAFPointArr[m_nRTAFSeqIndex].dTime = dDistance / m_dScanSpeed;
		m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = fabs(dFocusDiff / m_pRTAFPointArr[m_nRTAFSeqIndex].dTime);

		if (bPositiveMove)
		{
			m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dPosAcc;
			m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dPosDec;
		}
		else
		{
			m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dNegAcc;
			m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dNegDec;
		}

		if (AoiParam()->m_bUseApplyFixedAccelAndDeaccel == 0)
		{
			double dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
			double dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
			double dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;
			double dOldAcc = m_pParamMotion[FOCUS_AXIS].Speed.fAcc;
			double dOldMaxAcc = m_pParamAxis[FOCUS_AXIS].fMaxAccel;
			double dOldPeriod = m_pParamAxis[FOCUS_AXIS].fAccPeriod;
			m_pParamMotion[FOCUS_AXIS].Speed.fAcc = AoiParam()->m_dFocusAcc;
			m_pParamAxis[FOCUS_AXIS].fMaxAccel = AoiParam()->m_dFocusMaxAcc;
			m_pParamAxis[FOCUS_AXIS].fAccPeriod = AoiParam()->m_dFocusPeriod;
			m_pAxis[FOCUS_AXIS]->CalSpeedProfile(dFocusDiff, dVel, dAcc, dDeAcc);

			m_pParamMotion[FOCUS_AXIS].Speed.fAcc = dOldAcc;
			m_pParamAxis[FOCUS_AXIS].fMaxAccel = dOldMaxAcc;
			m_pParamAxis[FOCUS_AXIS].fAccPeriod = dOldPeriod;

#if _MSC_VER > 1700
			if (_isnan(dVel))
			{
				dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
			}
			if (_isnan(dAcc))
			{
				dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
			}
			if (_isnan(dDeAcc))
			{
				dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;
			}
#endif

			m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = dVel;
			m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = dAcc;
			m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = dDeAcc;
		}

		m_nRTAFSeqIndex++;

		//for the first!!!
		for (i = 1; i < nTotal; i++)
		{
			if (fabs(m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight - pPointArr[i].dHeight) > 0.001)
			{
				if (fabs(pPointArr[i].dHeight - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight) >= AoiParam()->m_dFocusDiffAcceptance)
					continue;

				//real af buffer
				m_pRTAFPointArr[m_nRTAFSeqIndex] = pPointArr[i];

				if (pPointArr[i].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * AoiParam()->m_dLaserSpotInTableZeroPointCheckRatio)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight;
				}

				if (pPointArr[i].dHeight < pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] * 0.5 && pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] > 0.1)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight;
				}

				if (m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight >= m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight)
					bPositiveMove = 1;
				else
					bPositiveMove = 0;
				m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToCompareAtMotionPosByScanAxis = m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dPosToMeasureAtMotionPosByScanAxis;
				double dDistance = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dPosToMeasureAtMotionPosByScanAxis - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dPosToMeasureAtMotionPosByScanAxis);
				double dFocusDiff = fabs(m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - m_pRTAFPointArr[m_nRTAFSeqIndex - 1].dHeight);

				m_pRTAFPointArr[m_nRTAFSeqIndex].dTime = dDistance / m_dScanSpeed;
				m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = fabs(dFocusDiff / m_pRTAFPointArr[m_nRTAFSeqIndex].dTime);

				if (bPositiveMove)
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dPosAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dPosDec;
				}
				else
				{
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = AoiParam()->m_dNegAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = AoiParam()->m_dNegDec;
				}

				if (AoiParam()->m_bUseApplyFixedAccelAndDeaccel == 0)
				{
					double dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
					double dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
					double dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;
					double dOldAcc = m_pParamMotion[FOCUS_AXIS].Speed.fAcc;
					double dOldMaxAcc = m_pParamAxis[FOCUS_AXIS].fMaxAccel;
					double dOldPeriod = m_pParamAxis[FOCUS_AXIS].fAccPeriod;
					m_pParamMotion[FOCUS_AXIS].Speed.fAcc = AoiParam()->m_dFocusAcc;
					m_pParamAxis[FOCUS_AXIS].fMaxAccel = AoiParam()->m_dFocusMaxAcc;
					m_pParamAxis[FOCUS_AXIS].fAccPeriod = AoiParam()->m_dFocusPeriod;
					m_pAxis[FOCUS_AXIS]->CalSpeedProfile(dFocusDiff, dVel, dAcc, dDeAcc);

					m_pParamMotion[FOCUS_AXIS].Speed.fAcc = dOldAcc;
					m_pParamAxis[FOCUS_AXIS].fMaxAccel = dOldMaxAcc;
					m_pParamAxis[FOCUS_AXIS].fAccPeriod = dOldPeriod;

#if _MSC_VER > 1700
					if (_isnan(dVel))
					{
						dVel = m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity;
					}
					if (_isnan(dAcc))
					{
						dAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel;
					}
					if (_isnan(dDeAcc))
					{
						dDeAcc = m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel;
					}
#endif

					m_pRTAFPointArr[m_nRTAFSeqIndex].dVelocity = dVel;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dAccel = dAcc;
					m_pRTAFPointArr[m_nRTAFSeqIndex].dDeAccel = dDeAcc;
				}


				//절대위치 포커스로 변경
				if (AoiParam()->m_bUseAbsoluteFocusPosition)
					m_pRTAFPointArr[m_nRTAFSeqIndex].dFocus = m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight + AoiParam()->m_dAbsFocusOrgPosCam1;
				else
					m_pRTAFPointArr[m_nRTAFSeqIndex].dFocus = m_pRTAFPointArr[m_nRTAFSeqIndex].dHeight - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] + pGlobalDoc->m_fRefFocusPosAtTestPoint;

				m_nRTAFSeqIndex++;
			}
		}
	}

	int nRealTotal = m_nRTAFSeqIndex - nLastIndex;

	m_nRTAFUniSeqLastIndex = m_nRTAFSeqIndex;

	for (i = nLastIndex; i < m_nRTAFSeqIndex; i++)
	{
		double velocity = Len2Pulse(FOCUS_AXIS, m_pRTAFPointArr[i].dVelocity);
		double acceleration = Len2Pulse(FOCUS_AXIS, m_pRTAFPointArr[i].dAccel);
		double deceleration = Len2Pulse(FOCUS_AXIS, m_pRTAFPointArr[i].dAccel);
		double finalVelocity = 0.0;
		double fjerkPercent = 0.0;
		double faccelJerk = 0.0;
		double fdecelJerk = 0.0;

		if (nDir == FORWARD)
			fComparePos = m_pRTAFPointArr[i].dPosToCompareAtMotionPosByScanAxis - AoiParam()->m_dCompareOffset;// +((CMotion*)m_pParent)->m_dOffsetInspCamLaser;
		else
			fComparePos = m_pRTAFPointArr[i].dPosToCompareAtMotionPosByScanAxis + AoiParam()->m_dCompareOffset;// +((CMotion*)m_pParent)->m_dOffsetInspCamLaser;

		//AddList(fComparePos, m_pRTAFPointArr[i].dFocus, m_dScanSpeed); //20211001-syd
		AddList(fComparePos, m_pRTAFPointArr[i].dFocus);
	}

	return 1;
}

BOOL CNmcDevice::NMC_CreateObject(int nTotalAxis)
{
	for (int nAxis = 0; nAxis < nTotalAxis; nAxis++)
	{
		if (CreateAxis(nAxis) == 0)
			return FALSE;	
	}
	return TRUE;
}

long CNmcDevice::CreateAutoFocusingSeq(BOOL bFirst, double fScanMotPos, double dFocusOffset, int bDir, double fHeightOfLaserSensor, int nSwath, int nHeightIndex)
{

	return 0;
}

MPIMotion CNmcDevice::MPI_MotionPtr(int Axis)
{
	return MPIMotion();
}

MPISequence CNmcDevice::MPI_SequencePtr(int nSeqid)
{
	return MPISequence();
}

int CNmcDevice::NMC_GetAddListNum()
{
	return GetAddListNum();
}

double CNmcDevice::GetActualPosition(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		return axes->GetActualPosition();

	return 0.0;
}

double CNmcDevice::GetCommandPosition(int nAxisId)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		return axes->GetCommandPosition();

	return 0.0;
}

BOOL CNmcDevice::IsGroupStatusStandby()
{
	if (!m_pBufferControl)
		return FALSE;

	return TRUE;
	//return m_pBufferControl->IsGroupStatusStandby(); //20211001-syd
}

BOOL CNmcDevice::IsListMotion()
{
	return m_bListMotion;
}

BOOL CNmcDevice::IsInterpolationMotion()
{
	return m_bInterpolationMotion;
}

int CNmcDevice::GetAddListNum()
{
	return m_pBufferControl->m_nAddListIndex;
}

void CNmcDevice::EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable) // nSensorIndex : 0 ~ 4 , bEnable
{
	m_pAxis[nAxisId]->EnableSensorStop(nSensorIndex, bEnable);
}




BOOL CNmcDevice::InitErrMap()
{
	int ms = MC_OK;

	if (IsInterpolationMotion())
		UnGroup2Ax(m_nBoardId, m_nGroupID_ErrMapXY);

	ms = Grouping2Ax(m_nBoardId, m_nGroupID_ErrMapXY, SCAN_AXIS, CAM_AXIS);

	if (ms == MC_OK)
		m_bErrMap = TRUE;
	else
	{
		m_bErrMap = FALSE;
		return FALSE;
	}

	return TRUE;
}


BOOL CNmcDevice::LoadErrorCompensationTable()
{
	MC_STATUS ms = MC_OK;
	char sEcPath[MAX_PATH];
	strcpy(sEcPath, NMC_EC_PATH);

	ms = MC_WriteErrorCompensationDataFile(m_nBoardId, NMC_EC_TYPE_FILE, NMC_EC_MAPID, sEcPath);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	m_bLoadedEC = TRUE;
	return TRUE;
}




BOOL CNmcDevice::LoadErrorCompensationTable(int eType, int eMapId, char* ePath) // nType: 0 [1D (Axis)] , 1 [2D (Gantry)] , 2 [3D (Group)] ; eMapId : Range[ 0 ~ 3 (Table1 ~ Table4) ]
{
	MC_STATUS ms = MC_OK;

	ms = MC_WriteErrorCompensationDataFile(m_nBoardId, eType, eMapId, ePath);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	m_bLoadedEC = TRUE;
	return TRUE;
}

BOOL CNmcDevice::ApplyErrorCompensateionTable(int nAxisID, BOOL bEnable) // NMC_EC_AXISID
{
	MC_STATUS ms = MC_OK;

	if (!m_bErrMap)
		InitErrMap();

	if (!m_bLoadedEC)
	{
		if (!LoadErrorCompensationTable())
		{
			return FALSE;
		}
	}

	//if (bEnable) //20211001-syd
	//{
	//	ms = MC_ChangeErrorCompensationMode(m_nBoardId, nAxisID, NMC_EC_TYPE_MODE, 1, NMC_EC_MAPID);//NMC_EC_AXISID
	//	if (ms != MC_OK)
	//	{
	//		CString sMsg;
	//		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
	//		AfxMessageBox(sMsg);
	//		return FALSE;
	//	}
	//	m_bAppliedEC = TRUE;
	//}
	//else
	//{
	//	ms = MC_ChangeErrorCompensationMode(m_nBoardId, nAxisID, NMC_EC_TYPE_MODE, 0, NMC_EC_MAPID);//NMC_EC_AXISID
	//	if (ms != MC_OK)
	//	{
	//		CString sMsg;
	//		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
	//		AfxMessageBox(sMsg);
	//		return FALSE;
	//	}
	//	m_bAppliedEC = FALSE;
	//}

	return TRUE;
}

int CNmcDevice::GetErrorCompensationStatus()
{
	MC_STATUS ms = MC_OK;
	UINT8 nStatus = EC_ST_DISABLED; // EC_ST_DISABLED = 0, EC_ST_ENABLED = 1, EC_ST_DISABLING = 2, EC_ST_ENABLING = 3
	UINT16 AxisId = NMC_EC_AXISID;
	UINT8 Type = EC_TP_AXIS;		// 0: Axis, 1: Gantry, 2: Group-2D, 3: Group-3D

	ms = MC_ReadErrorCompensationStatus(m_nBoardId, NMC_EC_MAPID, &AxisId, &Type, &nStatus);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
		AfxMessageBox(sMsg);
		return -1; // Error...
	}

	return (int)nStatus;			// EC_ST_DISABLED = 0, EC_ST_ENABLED = 1, EC_ST_DISABLING = 2, EC_ST_ENABLING = 3
}

int CNmcDevice::GetErrorCompensationType()
{
	MC_STATUS ms = MC_OK;
	UINT8 nStatus = EC_ST_DISABLED; // EC_ST_DISABLED = 0, EC_ST_ENABLED = 1, EC_ST_DISABLING = 2, EC_ST_ENABLING = 3
	UINT16 AxisId = NMC_EC_AXISID;
	UINT8 Type = EC_TP_AXIS;		// 0: Axis, 1: Gantry, 2: Group-2D, 3: Group-3D

	ms = MC_ReadErrorCompensationStatus(m_nBoardId, NMC_EC_MAPID, &AxisId, &Type, &nStatus);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
		AfxMessageBox(sMsg);
		return -1; // Error...
	}

	return (int)Type;				// 0: Axis, 1: Gantry, 2: Group-2D, 3: Group-3D
}

CString CNmcDevice::GetErrorMsgEC(MC_STATUS nErrCode)
{
	CString sMsg;

	if (nErrCode >= EC_ERROR_ADJUST_EC_0000 && nErrCode <= EC_ERROR_ADJUST_EC_FFFF) // 0x000F0000 ~ 0x000FFFFF
	{
		sMsg = _T("EC Table을 적용할 수 없는 Group 상태");
	}
	else
	{
		switch (nErrCode)
		{
		case EC_ERROR_PARAM1:									// 0x00030001
			sMsg = _T("1번째 파라미터 이상");
			break;
		case EC_ERROR_PARAM2:									// 0x00030002
			sMsg = _T("2번째 파라미터 이상");
			break;
		case EC_ERROR_PARAM3:									// 0x00030003
			sMsg = _T("3번째 파라미터 이상");
			break;
		case EC_ERROR_PARAM4:									// 0x00030004
			sMsg = _T("4번째 파라미터 이상");
			break;
		case EC_ERROR_PARAM5:									// 0x00030005
			sMsg = _T("5번째 파라미터 이상");
			break;
		case EC_ERROR_NOT_ALLOC_GROUP:							// 0x00110000
			sMsg = _T("구성되어 있지 않음 Group (Member가 할당되어 있지 않음)");
			break;
		case EC_ERROR_3D_CMDPOS:								// 0x001C0003
			sMsg = _T("3D 명령위치가 규칙을 위반함");
			break;
		case EC_ERROR_3D_MESPOS:								// 0x001D0003
			sMsg = _T("3D 측정위치가 규칙을 위반함");
			break;
		case EC_ERROR_2D_CMDPOS:								// 0x001C0002
			sMsg = _T("2D 명령위치가 규칙을 위반함");
			break;
		case EC_ERROR_2D_MESPOS:								// 0x001D0002
			sMsg = _T("2D 측정위치가 규칙을 위반함");
			break;
		case EC_ERROR_1D_CMDPOS:								// 0x001C0001
			sMsg = _T("1D 명령위치가 규칙을 위반함");
			break;
		case EC_ERROR_1D_MESPOS:								// 0x001D0001
			sMsg = _T("1D 측정위치가 규칙을 위반함");
			break;
		case EC_ERROR_PROHIBIT_CHG_TABLE:						// 0x001E0019
			sMsg = _T("EC Table을 상태를 변경할 수 없는 서보상태");
			break;
		case EC_ERROR_BLANK_GANTRY_ID:							// 0x001E001A
			sMsg = _T("구성되어 있지 않음 Gantry ID");
			break;
		case EC_ERROR_ALREADYED_ENABLE:							// 0x001E0020
			sMsg = _T("EC가 설정된 상태");
			break;
		case EC_ERROR_ALREADYED_DISABLE:						// 0x001E0021
			sMsg = _T("EC가 해제된 상태");
			break;
		case EC_ERROR_TABLE_DIMENSION:							// 0x001E0022
			sMsg = _T("EC Table과 Dimension이 불일치함");
			break;
		case EC_ERROR_NOT_EXIST_TABLE:							// 0x001E0023
			sMsg = _T("EC Table 내용이 없음");
			break;
		case EC_ERROR_CHG_TABLE_LAST_IDX:						// 0x001E0024
			sMsg = _T("EC Table Last Index가 변경됨");
			break;
		default:
			sMsg = _T("Unknown Error");
			break;

		}
	}


	return sMsg;
}

int CNmcDevice::DisableGroup2Ax(int nGroupNum)
{
	MC_STATUS ms;

	ms = MC_GroupDisable(m_nBoardId, nGroupNum);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-MC_GroupDisable(%d)"), nGroupNum);
		AfxMessageBox(sMsg);
		return -1; // Error...
	}
	Sleep(30);

	//ms = MC_UngroupAllAxes(m_nBoardId, nGroupNum);
	//if (ms != MC_OK)
	//{
	//	CString sMsg;
	//	sMsg.Format(_T("Error-MC_UngroupAllAxes(%d)"), nGroupNum);
	//	AfxMessageBox(sMsg);
	//	return -1; // Error...
	//}
	//Sleep(100);

	return ms;
}

BOOL CNmcDevice::TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAb, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	return TRUE;
}

BOOL CNmcDevice::TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	MC_STATUS ms = MC_OK;

	ms = MC_Reset(m_nBoardId, nMsId0 + m_nOffsetAxisID);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId0 + m_nOffsetAxisID);
		AfxMessageBox(sMsg);
		return FALSE; // Error...
	}

	ms = MC_Reset(m_nBoardId, nMsId1 + m_nOffsetAxisID);
	if (ms != MC_OK)
	{
		CString sMsg;
		sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId1 + m_nOffsetAxisID);
		AfxMessageBox(sMsg);
		return FALSE; // Error...
	}

	double dPulse0 = GetAxis(nMsId0)->LenToPulse(fPos0);
	double dPulse1 = GetAxis(nMsId1)->LenToPulse(fPos1);

	double fSpeed = m_pParamAxis[nMsId0].fSpd * (fVelRatio / 100.0);
	double dVel = GetAxis(nMsId0)->LenToPulse(fSpeed);
	double dAcc = GetAxis(nMsId0)->LenToPulse(m_pParamAxis[nMsId0].fAcc);
	double dJerk = GetAxis(nMsId0)->LenToPulse(fSpeed / m_pParamAxis[nMsId0].fMinJerkTime);	// 0;

	UINT16 nAxesNum = 2;
	UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };
	double ardPosVal[2] = { dPulse0, dPulse1 };
	MC_DIRECTION pDirArray[2] = { mcPositiveDirection, mcPositiveDirection };
	UINT8 ErrorStopMode = 1;	// ErrorStop이 발생할 경우 처리 방법 선택(0: Error발생축만 정지, 1: 모든축 동작 정지)
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION;
	UINT16 PositionCount = 2;	// 2축 직선보간운동

	MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	if (GroupStatus & GroupStandby)
	{
		;
	}
	else
	{
		// MMCE0-Axis1를 Group의 Identity0로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
		Sleep(30);
		// MMCE0-Axis2를 Group의 Identity1로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
		Sleep(30);
		ms = MC_GroupEnable(m_nBoardId, GroupNo);
		Sleep(30);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);
			return FALSE;
		}
		else
		{
			//MC_GroupReadStatus를 통해 GroupEnable 되었는지 확인
			MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
			if (GroupStatus & GroupStandby)
			{
				;
			}
			else
			{
				//GroupEnable 실패하면 Status 출력
				msg.Format(_T("GroupEnable Fail, GroupStatus: 0x%04x"), GroupStatus);
				AfxMessageBox(msg);
				return FALSE;
			}
		}

		m_bInterpolationMotion = TRUE;
	}

	Sleep(30);
	// mcACS 모드만 지원, BufferMode 선택, mcTMNone 모드만 지원, TransitionParameterCount: Reserved, TransitionParameter: Reserved
	ms = MC_MoveLinearAbsolute(m_nBoardId, GroupNo, PositionCount, ardPosVal, dVel, dAcc, dAcc, dJerk, mcACS, mcAborting, mcTMNone, 0, 0);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);
		return FALSE;
	}

	DWORD nTick = GetTickCount();

	while (bWait)
	{
		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		Sleep(30);
		if (GroupStatus & InPosition)
		{
			MC_ReadStatus(m_nBoardId, arnAxes[0], &AxisStatus);
			MC_ReadStatus(m_nBoardId, arnAxes[1], &AxisStatus2);
			if ((AxisStatus & mcStandStill) > 0 && (AxisStatus2 & mcStandStill) > 0)
			{
				break;
			}
		}
		if (GetTickCount() - nTick > 30000)
		{
			AfxMessageBox(_T("Time out 10 seconds."));
			break;
		}
	}

	return TRUE;
}

/*****************************************************************************
<Homing Type 설정>
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

int CNmcDevice::StartRsaHoming(int nAxisId, BOOL bWait, int nMode, int nDir)
{



	return GetAxis(nAxisId)->StartRsaHoming(bWait, nMode, nDir);
}

BOOL CNmcDevice::SetGantry(long lMaster, long lSlave, long lOnOff)
{
	if (lMaster >= MAX_AXIS || lSlave >= MAX_AXIS)
		return FALSE;

	m_bUseGantry = TRUE;
	m_lGantryMaster = lMaster;
	m_lGantrylSlave = lSlave;
	m_lGantryEnable = lOnOff;

	return TRUE;
}

BOOL CNmcDevice::CheckNmcConnection() // TRUE: OK , FALSE: Error
{
	MC_STATUS mc;
	UINT16 devicecount = 0;
	UINT16 workingcount = 0;
	UINT8  statusall = 0;

	mc = SlaveGetCurStateAll(m_nBoardId, &devicecount, &workingcount, &statusall);
	if (mc == MC_OK)
	{
		if (devicecount != workingcount || statusall != STATUS_COMBINATION_8)	// 반드시 전체 OP 상태도 체크해야 한다.	
		{
			UINT16* DeviceIDArray = new UINT16[devicecount];
			MasterGetDeviceID(m_nBoardId, DeviceIDArray);
			UINT8 data = 0;
			//문제 발생 시 개별 디바이스의 상태를 확인 하고 로깅 한다.
			for (int i = 0; i < (int)devicecount; i++)
			{
				SlaveGetCurState(m_nBoardId, DeviceIDArray[i], &data);
				if (data != (byte)eST_OP)
				{
					CString strData;
					strData.Format(_T("Ethercat Device:%d is not op, detect fault %d"), DeviceIDArray[i], data);
					SaveLog(strData);
				}
			}

			return FALSE; // Slave 이상 감지
		}
	}

	return TRUE;
}


// AOI 코드 추가
int CNmcDevice::RestoreSwEmergency()	// -1: Fault , 1: Emergency Signal Off complete, 2: Previous Emergency Signal Off-state, 3: Normal
{
	MC_STATUS ms = MC_OK;
	UINT8 status;

	ms = MC_RestoreSWEmergency(m_nBoardId, &status);

	for (int nAxis = 0; nAxis < m_nTotalAxisNum; nAxis++)
	{
		if (GetAxis(nAxis)->IsAmpFault())
		{
			ms = MC_Power(m_nBoardId, GetAxis(nAxis)->m_stParam.Motor.nAxisID + m_nOffsetAxisID, 0);

		//	GetAxis(nAxis)->SetAmpEnable(0);
			GetAxis(nAxis)->AmpFaultReset();
			Sleep(100);
			ms = MC_Power(m_nBoardId, GetAxis(nAxis)->m_stParam.Motor.nAxisID + m_nOffsetAxisID, 1);

		//	GetAxis(nAxis)->SetAmpEnable(1);
		}
	}

	return int(status);
}

