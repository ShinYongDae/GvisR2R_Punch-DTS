// NmcDevice.cpp : implementation file
//
#include "stdafx.h"
#include "../Global/GlobalDefine.h"

#ifdef USE_NMC

#include "safelock.h"
#include "Motion.h"
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

CNmcDevice::CNmcDevice(CWnd* pParent)
{
	m_pParent = pParent;
	m_pBufferControl = NULL;

	m_nInterlockStatus = 0;

	m_nOffsetIoOption = 0;

	m_strErrMsg = _T("");

	m_pParamCtrl = &(((CMotion*)m_pParent)->m_ParamCtrl);
	m_pParamMotion = ((CMotion*)m_pParent)->m_pParamMotion;
	m_pParamAxis = ((CMotion*)m_pParent)->m_pParamAxis;
	m_pParamMotor = ((CMotion*)m_pParent)->m_pParamMotor;

	m_nTotalMotion = m_pParamCtrl->nTotMotion;
	m_nTotalAxis = m_pParamCtrl->nTotAxis;

	m_dVel = new double[m_nTotalAxis] {0.0};
	m_dAcc = new double[m_nTotalAxis] {0.0};
	m_dDec = new double[m_nTotalAxis] {0.0};
	m_dJerk = new double[m_nTotalAxis] {0.0};
	m_fAccPeriod = new double[m_nTotalAxis] {0.0};
	m_fPosLimit = new double[m_nTotalAxis] {0.0};
	m_fNegLimit = new double[m_nTotalAxis] {0.0};
	m_fMaxAccel = new double[m_nTotalAxis] {0.0};
	m_pAxis = new CNmcAxis *[m_nTotalAxis];
	m_bOrigin = new BOOL[m_nTotalAxis]{FALSE};
	m_fMaxVel = new double[m_nTotalAxis] {0.0};

	for (int i = 0; i < m_nTotalAxis; i++)	//20110922 hyk mod
		m_pAxis[i] = NULL;

	m_dwDeviceIoOut = 0xFFFFFFFF;

	m_nTriggerOrgPos = 0;

	//m_nGroupID_RTAF = NMC_GROUPID_RTAF;
	//m_nGroupID_ErrMapXY = NMC_GROUPID_ERRMAP;
	m_nGroupID_Interpolation[0] = NMC_GROUPID_INTERPOLRATION_X0Y0;
	m_nGroupID_Interpolation[1] = NMC_GROUPID_INTERPOLRATION_X1Y1;

	m_bListMotion = FALSE;
	m_bInterpolationMotion[0] = FALSE;
	m_bInterpolationMotion[1] = FALSE;
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

}

CNmcDevice::~CNmcDevice()
{
	if (m_dVel)
	{
		delete m_dVel;
		m_dVel = NULL;
	}
	if (m_dAcc)
	{
		delete m_dAcc;
		m_dAcc = NULL;
	}
	if (m_dDec)
	{
		delete m_dDec;
		m_dDec = NULL;
	}
	if (m_dJerk)
	{
		delete m_dJerk;
		m_dJerk = NULL;
	}
	if (m_fAccPeriod)
	{
		delete m_fAccPeriod;
		m_fAccPeriod = NULL;
	}
	if (m_fPosLimit)
	{
		delete m_fPosLimit;
		m_fPosLimit = NULL;
	}
	if (m_fNegLimit)
	{
		delete m_fNegLimit;
		m_fNegLimit = NULL;
	}
	if (m_fMaxAccel)
	{
		delete m_fMaxAccel;
		m_fMaxAccel = NULL;
	}
	if (m_bOrigin)
	{
		delete m_bOrigin;
		m_bOrigin = NULL;
	}
	if (m_fMaxVel)
	{
		delete m_fMaxVel;
		m_fMaxVel = NULL;
	}

	for (int i = 0; i < m_nTotalAxis; i++)
	{
		if (m_pAxis[i])
		{
			delete m_pAxis[i];
			m_pAxis[i] = NULL;
		}
	}

	if (m_pAxis)
	{
		delete[] m_pAxis;
		m_pAxis = NULL;
	}
}


BEGIN_MESSAGE_MAP(CNmcDevice, CWnd)
	//{{AFX_MSG_MAP(CNmcDevice)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNmcDevice message handlers

double CNmcDevice::GetAxisState(int nAxisId)
{
	//if (nAxisId >= m_nTotalMotion)
	if (nAxisId >= m_nTotalAxis)
		return 0.0;

	return (m_pAxis[nAxisId]->GetState());
}

//unsigned long CNmcDevice::GetSliceOut(int nSegment)
//{
//	long OutData = ReadOut();
//	long nOut = 0;
//
//	if (nSegment == 2)
//	{
//		nOut = ((OutData & 0x0000FFFF) >> 0);
//	}
//	else if (nSegment == 3)
//	{
//		nOut = ((OutData & 0xFFFF0000) >> 16);
//	}
//	else
//		return FALSE;
//
//	return ((unsigned long)nOut);
//}

void CNmcDevice::SetParam()
{
	int i = 0;

	//Axis Object
	for (i = 0; i < m_nTotalAxis; i++)
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

	for (int iAxis = 0; iAxis < m_nTotalMotion; iAxis++)	// 20110922 hyk mod
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
	//for (int i = 0; i < m_nTotalMotion; i++)	//20110922 hyk mod
	for (int i = 0; i < m_nTotalAxis; i++)	//20110922 hyk mod
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
	//if (nAxis >= m_nTotalMotion)
	if (nAxis >= m_nTotalAxis)
		return FALSE;

	//m_pParamCtrl = &(((CMotion*)m_pParent)->m_ParamCtrl);
	//m_pParamMotion = ((CMotion*)m_pParent)->m_pParamMotion;
	//m_pParamAxis = ((CMotion*)m_pParent)->m_pParamAxis;
	//m_pParamMotor = ((CMotion*)m_pParent)->m_pParamMotor;
	//m_nTotalAxis = m_pParamCtrl->nTotAxis;

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
	m_pAxis[nAxis]->m_stParam.Speed.fMaxVel = m_pParamMotor[nAxis].fLeadPitch*(m_pParamMotor[nAxis].fRatingSpeed / 60.); // [mm/s]
	m_pAxis[nAxis]->m_stParam.Speed.fRatingRPM = m_pParamMotor[nAxis].fRatingSpeed; // [rev/min]

	m_pAxis[nAxis]->m_stParam.Motor.nAxisID = nAxis;
	m_pAxis[nAxis]->m_stParam.Motor.bType = m_pParamMotor[nAxis].bType;				// Motor Type 0: servo 1: stepper
	//m_pAxis[nAxis]->m_stParam.Motor.bSafety = m_pParamMotor[nAxis].bSafety;			// Safety Flag
	m_pAxis[nAxis]->m_stParam.Motor.fPosLimit = m_pParamMotor[nAxis].fPosLimit;		// Positive Software Limit
	m_pAxis[nAxis]->m_stParam.Motor.fNegLimit = m_pParamMotor[nAxis].fNegLimit;		// Negative Software Limit
	//m_pAxis[nAxis]->m_stParam.Motor.nVibOffset = m_pParamMotor[nAxis].nVibOffset;	// Vibration offset

	m_pAxis[nAxis]->m_stParam.Home.bIndex = m_pParamMotion[nAxis].Home.bIndex = FALSE;
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

	//m_pAxis[nAxis]->SetNegHWLimitAction(E_STOP_EVENT);
	//Sleep(100);
	//m_pAxis[nAxis]->SetPosHWLimitAction(E_STOP_EVENT);
	//Sleep(100);
	//m_pAxis[nAxis]->SetHomeAction(E_STOP_EVENT);
	//Sleep(100);
	//m_pAxis[nAxis]->SetNegSoftwareLimit(m_pParamMotor[nAxis].fNegLimit, E_STOP_EVENT);
	//Sleep(100);
	//m_pAxis[nAxis]->SetPosSoftwareLimit(m_pParamMotor[nAxis].fPosLimit, E_STOP_EVENT);
	//Sleep(100);

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
	//if (nAxis >= m_nTotalMotion)
	if (nAxis >= m_nTotalAxis)
		return FALSE;

	if (!m_pAxis[nAxis])
	{
		m_pAxis[nAxis] = new CNmcAxis(this);
		m_pAxis[nAxis]->SetParentClassHandle(this->GetSafeHwnd());
	}

	//if (!InitAxisParam(nAxis))
	//	return FALSE;


	return TRUE;
}

CNmcAxis* CNmcDevice::GetAxis(int nAxis)
{
	//if (nAxis >= m_nTotalMotion)
	if (nAxis >= m_nTotalAxis)
		return FALSE;

	CString strMsg;
	if (!m_pAxis[nAxis])
	{
		strMsg.Format(_T("Didn't create %d axis."), nAxis);
		AfxMessageBox(strMsg);
	}
	return m_pAxis[nAxis];

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


	//ms = MC_Init();	// 시스템에 설치된 모든 MMCE 보드를 초기화
	//if (ms != MC_OK)
	//{
	//	wsprintf(s, _T("MC Init Fail.%d\n"));
	//	AfxMessageBox(s, MB_OK);
	//	return FALSE;
	//}

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

					return FALSE;
				}
			}
		}
	}



	return TRUE;
}

int CNmcDevice::In32(long *value)
{
	int nRtn = MC_OK;
	UINT uBitSize = 32;
	UINT uDataSize = uBitSize / 8;
	long nData = 0;

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

	//*value = (long)(~nData);
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
	if (nMotionId >= m_nTotalMotion)
		return FALSE;

//#ifdef NO_ACCEPTABLE_VELRATIO_MOVE // 함수 인자의 관계를 정확하게 하기위해 모든 초기화 인자를 뺀다.. 161012 lgh
//	return (m_pMotion[nMotionId].Move(pTgtPos, bAbs, bWait, OPTIMIZED));
//#else
//	return (m_pMotion[nMotionId].Move(pTgtPos, bAbs, bWait));
//#endif

	int nAxisId = m_pParamMotion[nMotionId].stMsMap.nMappingAxis[0];
	//double fVel = m_pParamAxis[nMotionId].fSpd;
	//double fAcc = m_pParamAxis[nMotionId].fAcc;
	//double fDec = m_pParamAxis[nMotionId].fDec;
	double fVel = m_pParamAxis[nAxisId].fSpd;
	double fAcc = m_pParamAxis[nAxisId].fAcc;
	double fDec = m_pParamAxis[nAxisId].fDec;

	double fTime = 0.0, fJerk = 0.0;
	double fPos = *pTgtPos;
	double fLength = fabs(fPos - GetActualPosition(nMotionId));
	double dJerkTime = (fVel / fAcc);
	//dJerkTime = (dJerkTime > 0.0) ? dJerkTime : -dJerkTime;
	fJerk = (fAcc / dJerkTime);

	BOOL bOptimize = TRUE;
	if (fLength <= 0.000000001)
		return TRUE;

	if (bOptimize)
		fTime = GetSpeedProfile(TRAPEZOIDAL, nMotionId, fLength, fVel, fAcc, fJerk, MIDDLE_SPEED);//S_CURVE
	if (!GetAxis(nMotionId)->StartSCurveMove(fPos, fVel, fAcc, fJerk, bAbs, bWait))
		return FALSE;

	return TRUE;
}

BOOL CNmcDevice::Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait)
{
	if (nMotionId >= m_nTotalMotion)
		return FALSE;

	//#ifdef NO_ACCEPTABLE_VELRATIO_MOVE // 함수 인자의 관계를 정확하게 하기위해 모든 초기화 인자를 뺀다.. 161012 lgh
	//	return (m_pMotion[nMotionId].Move(pTgtPos, bAbs, bWait, OPTIMIZED));
	//#else
	//	return (m_pMotion[nMotionId].Move(pTgtPos, dRatio, bAbs, bWait));
	//#endif

	int nAxisId = m_pParamMotion[nMotionId].stMsMap.nMappingAxis[0];
	//double fVel = (m_pParamAxis[nMotionId].fSpd * dRatio) / 100.0;
	//double fAcc = (m_pParamAxis[nMotionId].fAcc * dRatio) / 100.0;
	//double fDec = (m_pParamAxis[nMotionId].fDec * dRatio) / 100.0;
	double fVel = (m_pParamAxis[nAxisId].fSpd * dRatio) / 100.0;
	double fAcc = (m_pParamAxis[nAxisId].fAcc * dRatio) / 100.0;
	double fDec = (m_pParamAxis[nAxisId].fDec * dRatio) / 100.0;

	double fTime = 0.0, fJerk = 0.0;
	double fPos = *pTgtPos;
	double fLength = fabs(fPos - GetActualPosition(nMotionId));
	double dJerkTime = (fVel / fAcc);
	//dJerkTime = (dJerkTime > 0.0) ? dJerkTime : -dJerkTime;
	fJerk = (fAcc / dJerkTime);

	BOOL bOptimize = TRUE;
	if (fLength <= 0.000000001)
		return TRUE;

	if (bOptimize)
		fTime = GetSpeedProfile(TRAPEZOIDAL, nMotionId, fLength, fVel, fAcc, fJerk, MIDDLE_SPEED);//S_CURVE
	if (!GetAxis(nMotionId)->StartSCurveMove(fPos, fVel, fAcc, fJerk, bAbs, bWait))
		return FALSE;

	return TRUE;
}

BOOL CNmcDevice::Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait, BOOL bOptimize)
{
	if (nMotionId >= m_nTotalMotion)
		return FALSE;

//	return (m_pMotion[nMotionId].Move(pTgtPos, dSpd, dAcc, dDec, bAbs, bWait, bOptimize));

	double fTime = 0.0, fJerk = 0.0;
	double fPos = *pTgtPos;
	double fLength = fabs(fPos - GetActualPosition(nMotionId));
	double dJerkTime = (dSpd / dAcc);
	//dJerkTime = (dJerkTime > 0.0) ? dJerkTime : -dJerkTime;
	fJerk = (dAcc / dJerkTime);

	if (bOptimize)
		fTime = GetSpeedProfile(TRAPEZOIDAL, nMotionId, fLength, dSpd, dAcc, fJerk, MIDDLE_SPEED);//S_CURVE
	if (!GetAxis(nMotionId)->StartSCurveMove(fPos, dSpd, dAcc, fJerk, bAbs, bWait))
		return FALSE;

	return TRUE;
}

BOOL CNmcDevice::Move(int nMotionId, double dTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait, BOOL bOptimize)
{
	double fTime = 0.0;
	double fLength = fabs(dTgtPos - GetActualPosition(nMotionId));
	double dJerkTime = (dSpd / dAcc);
	double fJerk = (dAcc / dJerkTime);

	if (fLength <= 0.000000001)
		return TRUE;

	if (bOptimize)
		fTime = GetSpeedProfile(TRAPEZOIDAL, nMotionId, fLength, dSpd, dAcc, fJerk, MIDDLE_SPEED);//S_CURVE

	return (GetAxis(nMotionId)->StartSCurveMove(dTgtPos, dSpd, dAcc, fJerk, bAbs, bWait));
}

//BOOL CNmcDevice::InitListMotion()
//{
//	m_dStartScanPos = 0.0;
//	m_dStartFocusPos = 0.0;
//
//	if (!m_pBufferControl)
//		m_pBufferControl = new CBufferControl(m_nBoardId, m_nGroupID_RTAF, SCAN_AXIS + m_nOffsetAxisID, FOCUS_AXIS + m_nOffsetAxisID);
//
//	if (IsListMotion())
//		UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);
//
//	ClearUserBuffer();
//	Grouping2Ax(m_nBoardId, m_nGroupID_RTAF, SCAN_AXIS, FOCUS_AXIS);
//	return BufferInit();
//}


//void CNmcDevice::RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos)
//{
//	m_dStartScanPos = dStartScanPos;
//	m_dStartFocusPos = dStartFocusPos;
//}

//void CNmcDevice::LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir, CString sPath)
//{
//	CString strPath;
//	strPath.Format(_T("%sAddList.txt"), pGlobalDoc->m_strSharedDir);
//	if (nScanDir == FORWARD)
//		m_sAddListLogTitle.Format(_T("Forward"));
//	else
//		m_sAddListLogTitle.Format(_T("BackWard"));
//
//	CFileFind finder;
//	CFile file;
//	if (finder.FindFile(strPath))
//	{
//		DeleteFile(strPath);
//	}
//
//	if (!file.Open(strPath, CFile::modeCreate | CFile::modeWrite, nullptr))
//	{
//		m_bLogAddList = FALSE;
//		AfxMessageBox(_T("Fail to create file."));
//	}
//	else
//	{
//		m_bLogAddList = TRUE;
//		file.Close();
//	}
//
//	if (m_bLogAddList)
//	{
//		CString strData, strIdx;
//		strIdx.Format(_T("Index%03d"), GetAddListNum() - 1);
//		strData.Format(_T("%.3f, %.3f, %.3f"), dStartScanPos, dStartFocusPos, fScanVel);
//		::WritePrivateProfileString(m_sAddListLogTitle, strIdx, strData, strPath);
//	}
//
//}

//double CNmcDevice::GetStartListMotionPos(int nIndex)
//{
//	switch (nIndex)
//	{
//	case 0:
//		if (m_dStartScanPos != 0.0)
//			return m_dStartScanPos;
//		else
//			return GetActualPosition(SCAN_AXIS);
//		break;
//	case 1:
//		if (m_dStartFocusPos != 0.0)
//			return m_dStartFocusPos;
//		else
//			return GetActualPosition(FOCUS_AXIS);
//		break;
//	}
//
//	return 0.0;
//}

//void CNmcDevice::AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed)
//{
//	double dPos1, dPos2, dVel;
//	dPos1 = m_pAxis[SCAN_AXIS]->LenToPulse(fScanMotPos);
//	dPos2 = m_pAxis[FOCUS_AXIS]->LenToPulse(dFocusMotPos);
//	dVel = m_pAxis[SCAN_AXIS]->LenToPulse(dScanSpeed);
//
//	m_pBufferControl->AddBufferMotion2Ax(dPos1, dPos2, dVel);
//
//	m_pBufferControl->m_nAddListIndex++;
//	return;
//}

//void CNmcDevice::StartListMotion()
//{
//	m_pBufferControl->StartMotion();
//	return;
//}
//
//void CNmcDevice::StopListMotion()
//{
//	m_pBufferControl->StopMotion();
//	return;
//}

//void CNmcDevice::ShutdownHomeThreadAll()
//{
//	m_bExit = TRUE;
//#ifdef USE_ZMP
//	int nTotMs = m_nTotalMotion;
//	for (int i = 0; i < nTotMs; i++)
//	{
//		m_pMotion[i].StopHomeThread();
//	}
//#endif
//	m_bExit = 0;
//	//	return bAllHome;
//}


double CNmcDevice::GetActualVelocity(int nAxisId)
{
	if (nAxisId >= m_nTotalMotion)
		return 0.0;

//	return m_pAxis[nAxisId].GetActVel(); // [mm]
	return 0.0;
}

//void CNmcDevice::ClearAutoFocusSmoothingData()	// 20151201 - syd
//{
//	m_dTempOffsetAf[0] = 0.0;
//	m_dTempOffsetAf[1] = 0.0;
//	m_dTempOffsetAf[2] = 0.0;
//	m_dTempOffsetAf[4] = 0.0;
//}

//double CNmcDevice::SetAutoFocusSmoothing(double dFocusOffset)	// 20151201 - syd
//{
//	double dVal = 0.0;
//	int nIdx = m_nSeqIdx % 4;
//	m_dTempOffsetAf[nIdx] = dFocusOffset;
//
//	switch (m_nSeqIdx)
//	{
//	case 0:
//		dVal = m_dTempOffsetAf[0];
//		break;
//	case 1:
//		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1]) / 2.0;
//		break;
//	case 2:
//		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1] + m_dTempOffsetAf[2]) / 3.0;
//		break;
//	default:
//		dVal = (m_dTempOffsetAf[0] + m_dTempOffsetAf[1] + m_dTempOffsetAf[2] + m_dTempOffsetAf[3]) / 4.0;
//		break;
//	}
//
//	return dVal;
//}

//void CNmcDevice::StopSequence()
//{
//	ClearUserBuffer();
//}

//void CNmcDevice::ClearUserBuffer()
//{
//	int i = 0;
//
//	for (i = 0; i < 2048; i++)
//	{
//		m_dUserBuffer[i] = 0;
//	}
//}

// 속도,가속도 지정.
BOOL CNmcDevice::SetVMove(int nAxisID, double fVel, double fAcc)
{
	if (nAxisID >= m_nTotalMotion)
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
	if (nMotionId >= m_nTotalMotion)
		return FALSE;

	if (GetAxis(nMotionId)->CheckAmpFaultSwitch() || GetAxis(nMotionId)->CheckLimitSwitch(MINUS) || GetAxis(nMotionId)->CheckLimitSwitch(PLUS))
	{
		GetAxis(nMotionId)->SetAmpEnable(FALSE);
		Sleep(50);
		GetAxis(nMotionId)->AmpFaultReset();
		Sleep(50);
		GetAxis(nMotionId)->ClearStatus();
		Sleep(50);
		GetAxis(nMotionId)->SetAmpEnable(TRUE);
		Sleep(50);
	}

	return (GetAxis(nMotionId)->VMove(nDir));
}

void CNmcDevice::SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID)
{
	m_nBoardId = nBoardId;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = nOffsetAxisID;
//	m_nGroupID = nGroupID;

	for (int nAxis = 0; nAxis < m_nTotalAxis; nAxis++)
	{
		if (m_pAxis[nAxis])
			m_pAxis[nAxis]->SetConfigure(nBoardId, nDevIdIoIn, nDevIdIoOut, nOffsetAxisID);
	}
}

void CNmcDevice::OutBit(long bit, bool flag)
{
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

	//nData |= (val[0] >> 0) & 0xFF;
	//nData |= (val[1] >> 8) & 0xFF;
	//nData |= (val[2] >> 16) & 0xFF;
	//nData |= (val[3] >> 24) & 0xFF;

	nData |= val[0] & 0xFF;
	nData |= (val[1] & 0xFF) << 8;
	nData |= (val[2] & 0xFF) << 16;
	nData |= (val[3] & 0xFF) << 24;

	return (nData);
}

BOOL CNmcDevice::ReadIn(long bit)
{
	long nData;
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


BOOL CNmcDevice::TriggerSetRange(int encAxisId, int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
{
	double dOffset = 0.0;
	MC_STATUS ms = MC_OK;

#ifdef SCAN_S_AXIS
	dOffset = GetAxis(SCAN_S_AXIS)->GetActualPosition();
#endif

	DOUBLE dStartPos = -1.0 * m_pAxis[encAxisId]->LenToPulse(dStPos - dOffset);		// Function Module Encoder counting is reversed...
	DOUBLE dEndPos = -1.0 * m_pAxis[encAxisId]->LenToPulse(dEdPos - dOffset);		// Function Module Encoder counting is reversed...
	//DOUBLE dStartPos = -1.0 * m_pAxis[encAxisId]->LenToPulse(dStPos);		// Function Module Encoder counting is reversed...
	//DOUBLE dEndPos = -1.0 * m_pAxis[encAxisId]->LenToPulse(dEdPos);		// Function Module Encoder counting is reversed...
	UINT16 usPeriod = (UINT16)(dTrigRes / (m_pAxis[encAxisId]->m_stParam.Motor.fPosRes * 1000.0) + 0.5); // 반올림....
	UINT16 usPulseWidth = (UINT16)((dPulseWd * 1000.0) / 50.0);
	
	ms = MC_WriteIntervalTrigParameterFM(m_nBoardId, nEcatAddr, vAxisId, dStartPos, dEndPos, usPeriod, usPulseWidth); // dStartPos[pulse], dEndPos[pulse], usPeriod[pulse], usPulseWidth[1/50nSec]
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerSetRange()"));
		return FALSE;
	}
	Sleep(100);

	ms = MC_WriteIntervalTrigEnableFM(m_nBoardId, nEcatAddr, vAxisId, true);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-EnableTriggerSetRange()"));
		return FALSE;
	}
	Sleep(100);

	return TRUE;
}


BOOL CNmcDevice::TriggerStop(int nEcatAddr, int vAxisId)
{
	MC_STATUS ms = MC_OK;

	ms = MC_WriteIntervalTrigEnableFM(m_nBoardId, nEcatAddr, vAxisId, false);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-TriggerStop()"));
		return FALSE;
	}
	Sleep(100);

	return TRUE;
}

BOOL CNmcDevice::TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx)
{
	MC_STATUS ms = MC_OK;

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
	Sleep(30);

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
	Sleep(30);

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
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-MC_GroupDisable()"));
		return ms;
	}
	ms = MC_UngroupAllAxes(m_nBoardId, nGroupNum);
	if (ms != MC_OK)
	{
		AfxMessageBox(_T("Error-MC_UngroupAllAxes()"));
		return ms;
	}
	Sleep(5);

	if (m_nGroupID_Interpolation[0] == nGroupNum)
		m_bInterpolationMotion[0] = FALSE;
	if (m_nGroupID_Interpolation[1] == nGroupNum)
		m_bInterpolationMotion[1] = FALSE;

	return ms;
}

void CNmcDevice::UnGroup2Ax(int nBdNum, int nGroupNum)
{
	UnGroup2Ax(nGroupNum);

	return;
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

		//if (m_nGroupID_RTAF == nGroupNum)
		//{
		//	if (ms == MC_OK)
		//		m_bListMotion = TRUE;
		//	else
		//		m_bListMotion = FALSE;
		//}
		//else if (m_nGroupID_ErrMapXY == nGroupNum)
		//{
		//	if (ms == MC_OK)
		//		m_bErrMap = TRUE;
		//	else
		//		m_bErrMap = FALSE;
		//}
		//else if (m_nGroupID_Interpolation == nGroupNum)
		//{
		//	if (ms == MC_OK)
		//		m_bInterpolationMotion = TRUE;
		//	else
		//		m_bInterpolationMotion = FALSE;
		//}
		if (m_nGroupID_Interpolation[0] == nGroupNum)
		{
			if (ms == MC_OK)
				m_bInterpolationMotion[0] = TRUE;
			else
				m_bInterpolationMotion[0] = FALSE;
		}
		if (m_nGroupID_Interpolation[1] == nGroupNum)
		{
			if (ms == MC_OK)
				m_bInterpolationMotion[1] = TRUE;
			else
				m_bInterpolationMotion[1] = FALSE;
		}

		return ms;
	}

	if(m_pBufferControl)
		ms = m_pBufferControl->Grouping2Ax(); //20211001-syd
		//ms = m_pBufferControl->Grouping2Ax(nBdNum, nGroupNum, nAxisNumX + m_nOffsetAxisID, nAxisNumY + m_nOffsetAxisID);

	//if (m_nGroupID_RTAF == nGroupNum)
	//{
	//	if (ms == MC_OK)
	//		m_bListMotion = TRUE;
	//	else
	//		m_bListMotion = FALSE;
	//}
	//else if (m_nGroupID_ErrMapXY == nGroupNum)
	//{
	//	if (ms == MC_OK)
	//		m_bErrMap = TRUE;
	//	else
	//		m_bErrMap = FALSE;
	//}
	//else if (m_nGroupID_Interpolation == nGroupNum)
	//{
	//	if (ms == MC_OK)
	//		m_bInterpolationMotion = TRUE;
	//	else
	//		m_bInterpolationMotion = FALSE;
	//}
	if (m_nGroupID_Interpolation[0] == nGroupNum)
	{
		if (ms == MC_OK)
			m_bInterpolationMotion[0] = TRUE;
		else
			m_bInterpolationMotion[0] = FALSE;
	}
	if (m_nGroupID_Interpolation[1] == nGroupNum)
	{
		if (ms == MC_OK)
			m_bInterpolationMotion[1] = TRUE;
		else
			m_bInterpolationMotion[1] = FALSE;
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

double CNmcDevice::GetActualPosition(int nAxisId)
{
	return m_pAxis[nAxisId]->GetActualPosition();
}

double CNmcDevice::CalSpeedProfile(int nAxisID, double fLength, double &fVel, double &fAcc, double &fDec)
{
	if (nAxisID >= m_nTotalMotion)
		return 0.0;

	double fAccTime, fVelocityTime, fTotalTime;

	double fVelocity = fVel;
	double fAccPeriod = ((CMotion*)m_pParent)->m_pParamAxis[nAxisID].fAccPeriod;			// m_fAccPeriod[nAxisID];
	double fAccLength;
	fAcc = ((CMotion*)m_pParent)->m_pParamAxis[nAxisID].fAcc;							// m_fAcc[nAxisID];

	if (fAcc <= 0.0 || fAcc > ((CMotion*)m_pParent)->m_pParamAxis[nAxisID].fMaxAccel)	// m_fMaxAccel[nAxisID])
		fAcc = m_fMaxAccel[nAxisID];

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
		if (fAccTime < ((CMotion*)m_pParent)->m_pParamAxis[nAxisID].fMinJerkTime)	// m_fMinJerkTime[nAxisID])
			fAccTime = ((CMotion*)m_pParent)->m_pParamAxis[nAxisID].fMinJerkTime;	// m_fMinJerkTime[nAxisID];
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

BOOL CNmcDevice::IsGroupStatusStandby()
{
	if (!m_pBufferControl)
		return FALSE;

	return TRUE;
	//return m_pBufferControl->IsGroupStatusStandby();
}

BOOL CNmcDevice::IsListMotion()
{
	return m_bListMotion;
}

BOOL CNmcDevice::IsInterpolationMotion(int nId)
{
	return m_bInterpolationMotion[nId];
}

int CNmcDevice::GetAddListNum()
{
	return m_pBufferControl->m_nAddListIndex;
}

void CNmcDevice::EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable) // nSensorIndex : 0 ~ 4 , bEnable
{
	m_pAxis[nAxisId]->EnableSensorStop(nSensorIndex, bEnable);
}


/*

BOOL CNmcDevice::InitErrMap()
{
	int ms = MC_OK;

	//if (IsInterpolationMotion())
	//	UnGroup2Ax(m_nBoardId, m_nGroupID_ErrMapXY);

	//ms = Grouping2Ax(m_nBoardId, m_nGroupID_ErrMapXY, SCAN_AXIS, CAM_AXIS);

	//if (ms == MC_OK)
	//	m_bErrMap = TRUE;
	//else
	//{
	//	m_bErrMap = FALSE;
	//	return FALSE;
	//}

	return TRUE;
}


BOOL CNmcDevice::LoadErrorCompensationTable()
{
	MC_STATUS ms = MC_OK;
	//char sEcPath[MAX_PATH];
	//strcpy(sEcPath, NMC_EC_PATH);
	TCHAR sEcPath[MAX_PATH];
	wsprintf(sEcPath, TEXT("%s"), NMC_EC_PATH);

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

	if (bEnable)
	{
		ms = MC_ChangeErrorCompensationMode(m_nBoardId, nAxisID, NMC_EC_TYPE_MODE, 1, NMC_EC_MAPID);//NMC_EC_AXISID
		if (ms != MC_OK)
		{
			CString sMsg;
			sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
			AfxMessageBox(sMsg);
			return FALSE;
		}
		m_bAppliedEC = TRUE;
	}
	else
	{
		ms = MC_ChangeErrorCompensationMode(m_nBoardId, nAxisID, NMC_EC_TYPE_MODE, 0, NMC_EC_MAPID);//NMC_EC_AXISID
		if (ms != MC_OK)
		{
			CString sMsg;
			sMsg.Format(_T("Error-%s"), GetErrorMsgEC(ms));
			AfxMessageBox(sMsg);
			return FALSE;
		}
		m_bAppliedEC = FALSE;
	}

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
*/
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

BOOL CNmcDevice::TwoStartPosMove0(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	MC_STATUS ms = MC_OK;
	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X0Y0;
	UINT16 PositionCount = 2;	// 2축 직선보간운동
	UINT16 nAxesNum = 2;
	UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };

	DWORD CurTimer, StartTimer;
	MSG message;
	CString sMsg;

	DWORD nTick = GetTickCount();

	if (m_pAxis[nMsId0]->IsGroupMotion() && m_pAxis[nMsId1]->IsGroupMotion())
	{
		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		if ( !( (GroupStatus & GroupStandby) && (GroupStatus & InPosition) ) )
		{
			while (TRUE)
			{
				MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
				Sleep(10);
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
					AfxMessageBox(_T("Time out 30 seconds."));
					break;
				}
			}
		}
	}
	else
	{ 
		StartTimer = GetTickCount();
		CurTimer = GetTickCount();

		while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
		{
			CurTimer = GetTickCount();
			Sleep(10);
		}

		if (TEN_SECOND < int(CurTimer - StartTimer))
		{
			sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
			AfxMessageBox(sMsg);
			return FALSE;
		}
	}

	//ms = MC_Reset(m_nBoardId, nMsId0 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId0 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	//ms = MC_Reset(m_nBoardId, nMsId1 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId1 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	double dPulse0 = GetAxis(nMsId0)->LenToPulse(fPos0);
	double dPulse1 = GetAxis(nMsId1)->LenToPulse(fPos1);

	double fSpeed = fVel;
	double dVel = GetAxis(nMsId0)->LenToPulse(fSpeed);
	double dAcc = GetAxis(nMsId0)->LenToPulse(fAcc);
	double dJerk = GetAxis(nMsId0)->LenToPulse(fSpeed / m_pParamAxis[nMsId0].fMinJerkTime);	// 0;

	double ardPosVal[2] = { dPulse0, dPulse1 };
	MC_DIRECTION pDirArray[2] = { mcPositiveDirection, mcPositiveDirection };
	UINT8 ErrorStopMode = 1;	// ErrorStop이 발생할 경우 처리 방법 선택(0: Error발생축만 정지, 1: 모든축 동작 정지)
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	if (!m_pAxis[nMsId0]->IsGroupMotion() || !m_pAxis[nMsId1]->IsGroupMotion())
	{
		// MMCE0-Axis1를 Group의 Identity0로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
		Sleep(10);
		// MMCE0-Axis2를 Group의 Identity1로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
		Sleep(10);
		ms = MC_GroupEnable(m_nBoardId, GroupNo);
		Sleep(30);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);
			return FALSE;
		}
		//else
		//{
		//	//MC_GroupReadStatus를 통해 GroupEnable 되었는지 확인
		//	MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		//	if (GroupStatus & GroupStandby)
		//	{
		//		;
		//	}
		//	else
		//	{
		//		//GroupEnable 실패하면 Status 출력
		//		msg.Format(_T("GroupEnable Fail, GroupStatus: 0x%04x"), GroupStatus);
		//		AfxMessageBox(msg);
		//		return FALSE;
		//	}
		//}

		m_bInterpolationMotion[0] = TRUE;
	}

	//MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//if (GroupStatus & GroupStandby)
	//{
	//	;
	//}
	//else
	//{
	//}

	//Sleep(30);
	// mcACS 모드만 지원, BufferMode 선택, mcTMNone 모드만 지원, TransitionParameterCount: Reserved, TransitionParameter: Reserved
	ms = MC_MoveLinearAbsolute(m_nBoardId, GroupNo, PositionCount, ardPosVal, dVel, dAcc, dAcc, dJerk, mcACS, mcAborting, mcTMNone, 0, 0);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);
		return FALSE;
	}

	nTick = GetTickCount();

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


BOOL CNmcDevice::TwoStartPosMove1(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	//MC_STATUS ms = MC_OK;
	//DWORD CurTimer, StartTimer;
	//MSG message;
	//CString sMsg;

	//StartTimer = GetTickCount();
	//CurTimer = GetTickCount();
	//ULONGLONG nWaitTick = GetTickCount64();

	//while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
	//{
	//	CurTimer = GetTickCount();
	//	Sleep(30);
	//}

	//if (TEN_SECOND < int(CurTimer - StartTimer))
	//{
	//	sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
	//	AfxMessageBox(sMsg);
	//	return FALSE;
	//}

	MC_STATUS ms = MC_OK;
	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X1Y1;
	UINT16 PositionCount = 2;	// 2축 직선보간운동
	UINT16 nAxesNum = 2;
	UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };

	DWORD CurTimer, StartTimer;
	MSG message;
	CString sMsg;

	DWORD nTick = GetTickCount();

	if (m_pAxis[nMsId0]->IsGroupMotion() && m_pAxis[nMsId1]->IsGroupMotion())
	{
		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		if (!((GroupStatus & GroupStandby) && (GroupStatus & InPosition)))
		{
			while (TRUE)
			{
				MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
				Sleep(10);
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
					AfxMessageBox(_T("Time out 30 seconds."));
					break;
				}
			}
		}
	}
	else
	{
		StartTimer = GetTickCount();
		CurTimer = GetTickCount();

		while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
		{
			CurTimer = GetTickCount();
			Sleep(10);
		}

		if (TEN_SECOND < int(CurTimer - StartTimer))
		{
			sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
			AfxMessageBox(sMsg);
			return FALSE;
		}
	}
	
	//MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//if (GroupStatus & GroupStandby)
	//{
	//	while (TRUE)
	//	{
	//		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//		Sleep(30);
	//		if (GroupStatus & InPosition)
	//		{
	//			MC_ReadStatus(m_nBoardId, arnAxes[0], &AxisStatus);
	//			MC_ReadStatus(m_nBoardId, arnAxes[1], &AxisStatus2);
	//			if ((AxisStatus & mcStandStill) > 0 && (AxisStatus2 & mcStandStill) > 0)
	//			{
	//				break;
	//			}
	//		}
	//		if (GetTickCount() - nTick > 30000)
	//		{
	//			AfxMessageBox(_T("Time out 30 seconds."));
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	StartTimer = GetTickCount();
	//	CurTimer = GetTickCount();

	//	while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
	//	{
	//		CurTimer = GetTickCount();
	//		Sleep(30);
	//	}

	//	if (TEN_SECOND < int(CurTimer - StartTimer))
	//	{
	//		sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
	//		AfxMessageBox(sMsg);
	//		return FALSE;
	//	}
	//}

	//ms = MC_Reset(m_nBoardId, nMsId0 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId0 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	//ms = MC_Reset(m_nBoardId, nMsId1 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId1 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	double dPulse0 = GetAxis(nMsId0)->LenToPulse(fPos0);
	double dPulse1 = GetAxis(nMsId1)->LenToPulse(fPos1);

	double fSpeed = fVel;
	double dVel = GetAxis(nMsId0)->LenToPulse(fSpeed);
	double dAcc = GetAxis(nMsId0)->LenToPulse(fAcc);
	double dJerk = GetAxis(nMsId0)->LenToPulse(fSpeed / m_pParamAxis[nMsId0].fMinJerkTime);	// 0;

	//UINT16 nAxesNum = 2;
	//UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };
	double ardPosVal[2] = { dPulse0, dPulse1 };
	MC_DIRECTION pDirArray[2] = { mcPositiveDirection, mcPositiveDirection };
	UINT8 ErrorStopMode = 1;	// ErrorStop이 발생할 경우 처리 방법 선택(0: Error발생축만 정지, 1: 모든축 동작 정지)
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	//UINT32 GroupStatus = 0;
	//UINT32 AxisStatus = 0;
	//UINT32 AxisStatus2 = 0;
	//UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X1Y1;
	//UINT16 PositionCount = 2;	// 2축 직선보간운동


	if (!m_pAxis[nMsId0]->IsGroupMotion() || !m_pAxis[nMsId1]->IsGroupMotion())
	{
		// MMCE0-Axis1를 Group의 Identity0로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
		Sleep(10);
		// MMCE0-Axis2를 Group의 Identity1로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
		Sleep(10);
		ms = MC_GroupEnable(m_nBoardId, GroupNo);
		Sleep(30);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);
			return FALSE;
		}

		m_bInterpolationMotion[1] = TRUE;
	}

	//MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//if (GroupStatus & GroupStandby)
	//{
	//	;
	//}
	//else
	//{
	//	// MMCE0-Axis1를 Group의 Identity0로 추가
	//	ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
	//	Sleep(100);
	//	// MMCE0-Axis2를 Group의 Identity1로 추가
	//	ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
	//	Sleep(100);
	//	ms = MC_GroupEnable(m_nBoardId, GroupNo);
	//	Sleep(300);

	//	if (ms != MC_OK)
	//	{
	//		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
	//		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
	//		AfxMessageBox(msg);
	//		return FALSE;
	//	}
	//	else
	//	{
	//		//MC_GroupReadStatus를 통해 GroupEnable 되었는지 확인
	//		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//		if (GroupStatus & GroupStandby)
	//		{
	//			;
	//		}
	//		else
	//		{
	//			//GroupEnable 실패하면 Status 출력
	//			msg.Format(_T("GroupEnable Fail, GroupStatus: 0x%04x"), GroupStatus);
	//			AfxMessageBox(msg);
	//			return FALSE;
	//		}
	//	}

	//	m_bInterpolationMotion[1] = TRUE;
	//}

	//Sleep(30);
	// mcACS 모드만 지원, BufferMode 선택, mcTMNone 모드만 지원, TransitionParameterCount: Reserved, TransitionParameter: Reserved
	ms = MC_MoveLinearAbsolute(m_nBoardId, GroupNo, PositionCount, ardPosVal, dVel, dAcc, dAcc, dJerk, mcACS, mcAborting, mcTMNone, 0, 0);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);
		return FALSE;
	}

	//DWORD nTick = GetTickCount();
	nTick = GetTickCount();

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

BOOL CNmcDevice::TwoStartPosMove0(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	MC_STATUS ms = MC_OK;
	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X0Y0;
	UINT16 PositionCount = 2;	// 2축 직선보간운동
	UINT16 nAxesNum = 2;
	UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };

	DWORD CurTimer, StartTimer;
	MSG message;
	CString sMsg;

	DWORD nTick = GetTickCount();
	if (m_pAxis[nMsId0]->IsGroupMotion() && m_pAxis[nMsId1]->IsGroupMotion())
	{
		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		if (!((GroupStatus & GroupStandby) && (GroupStatus & InPosition)))
		{
			while (TRUE)
			{
				MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
				Sleep(10);
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
					AfxMessageBox(_T("Time out 30 seconds."));
					break;
				}
			}
		}
	}
	else
	{
		StartTimer = GetTickCount();
		CurTimer = GetTickCount();

		while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
		{
			CurTimer = GetTickCount();
			Sleep(10);
		}

		if (TEN_SECOND < int(CurTimer - StartTimer))
		{
			sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
			AfxMessageBox(sMsg);
			return FALSE;
		}
	}

	double dPulse0 = GetAxis(nMsId0)->LenToPulse(fPos0);
	double dPulse1 = GetAxis(nMsId1)->LenToPulse(fPos1);

	double fSpeed = m_pParamAxis[nMsId0].fSpd * (fVelRatio / 100.0);
	double dVel = GetAxis(nMsId0)->LenToPulse(fSpeed);
	double dAcc = GetAxis(nMsId0)->LenToPulse(m_pParamAxis[nMsId0].fAcc);
	double dJerk = GetAxis(nMsId0)->LenToPulse(fSpeed / m_pParamAxis[nMsId0].fMinJerkTime);	// 0;

	double ardPosVal[2] = { dPulse0, dPulse1 };
	MC_DIRECTION pDirArray[2] = { mcPositiveDirection, mcPositiveDirection };
	UINT8 ErrorStopMode = 1;	// ErrorStop이 발생할 경우 처리 방법 선택(0: Error발생축만 정지, 1: 모든축 동작 정지)
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	if (!m_pAxis[nMsId0]->IsGroupMotion() || !m_pAxis[nMsId1]->IsGroupMotion())
	{
		// MMCE0-Axis1를 Group의 Identity0로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
		Sleep(100);
		// MMCE0-Axis2를 Group의 Identity1로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
		Sleep(100);
		ms = MC_GroupEnable(m_nBoardId, GroupNo);
		Sleep(300);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);
			return FALSE;
		}

		m_bInterpolationMotion[0] = TRUE;
	}

	// mcACS 모드만 지원, BufferMode 선택, mcTMNone 모드만 지원, TransitionParameterCount: Reserved, TransitionParameter: Reserved
	ms = MC_MoveLinearAbsolute(m_nBoardId, GroupNo, PositionCount, ardPosVal, dVel, dAcc, dAcc, dJerk, mcACS, mcAborting, mcTMNone, 0, 0);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);
		return FALSE;
	}

	nTick = GetTickCount();

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

BOOL CNmcDevice::TwoStartPosMove1(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	//MC_STATUS ms = MC_OK;
	//DWORD CurTimer, StartTimer;
	//MSG message;
	//CString sMsg;

	//StartTimer = GetTickCount();
	//CurTimer = GetTickCount();
	//ULONGLONG nWaitTick = GetTickCount64();

	//while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
	//{
	//	CurTimer = GetTickCount();
	//	Sleep(30);
	//}

	//if (TEN_SECOND < int(CurTimer - StartTimer))
	//{
	//	sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
	//	AfxMessageBox(sMsg);
	//	return FALSE;
	//}

	MC_STATUS ms = MC_OK;
	UINT32 GroupStatus = 0;
	UINT32 AxisStatus = 0;
	UINT32 AxisStatus2 = 0;
	UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X1Y1;
	UINT16 PositionCount = 2;	// 2축 직선보간운동
	UINT16 nAxesNum = 2;
	UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };

	DWORD CurTimer, StartTimer;
	MSG message;
	CString sMsg;

	DWORD nTick = GetTickCount();
	if (m_pAxis[nMsId0]->IsGroupMotion() && m_pAxis[nMsId1]->IsGroupMotion())
	{
		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
		if (!((GroupStatus & GroupStandby) && (GroupStatus & InPosition)))
		{
			while (TRUE)
			{
				MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
				Sleep(10);
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
					AfxMessageBox(_T("Time out 30 seconds."));
					break;
				}
			}
		}
	}
	else
	{
		StartTimer = GetTickCount();
		CurTimer = GetTickCount();

		while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
		{
			CurTimer = GetTickCount();
			Sleep(10);
		}

		if (TEN_SECOND < int(CurTimer - StartTimer))
		{
			sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
			AfxMessageBox(sMsg);
			return FALSE;
		}
	}

	//MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//if (GroupStatus & GroupStandby)
	//{
	//	while (TRUE)
	//	{
	//		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//		Sleep(30);
	//		if (GroupStatus & InPosition)
	//		{
	//			MC_ReadStatus(m_nBoardId, arnAxes[0], &AxisStatus);
	//			MC_ReadStatus(m_nBoardId, arnAxes[1], &AxisStatus2);
	//			if ((AxisStatus & mcStandStill) > 0 && (AxisStatus2 & mcStandStill) > 0)
	//			{
	//				break;
	//			}
	//		}
	//		if (GetTickCount() - nTick > 30000)
	//		{
	//			AfxMessageBox(_T("Time out 30 seconds."));
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	StartTimer = GetTickCount();
	//	CurTimer = GetTickCount();

	//	while ((!IsMotionDone(nMsId0) || !IsMotionDone(nMsId1)) && TEN_SECOND > int(CurTimer - StartTimer))
	//	{
	//		CurTimer = GetTickCount();
	//		Sleep(30);
	//	}

	//	if (TEN_SECOND < int(CurTimer - StartTimer))
	//	{
	//		sMsg.Format(_T("Error-Wait MotionDone Time Over(TEN_SECOND)."));
	//		AfxMessageBox(sMsg);
	//		return FALSE;
	//	}
	//}

	//ms = MC_Reset(m_nBoardId, nMsId0 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId0 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	//ms = MC_Reset(m_nBoardId, nMsId1 + m_nOffsetAxisID);
	//if (ms != MC_OK)
	//{
	//	sMsg.Format(_T("Error-MC_Reset Axis%d"), nMsId1 + m_nOffsetAxisID);
	//	AfxMessageBox(sMsg);
	//	return FALSE; // Error...
	//}

	double dPulse0 = GetAxis(nMsId0)->LenToPulse(fPos0);
	double dPulse1 = GetAxis(nMsId1)->LenToPulse(fPos1);

	double fSpeed = m_pParamAxis[nMsId0].fSpd * (fVelRatio / 100.0);
	double dVel = GetAxis(nMsId0)->LenToPulse(fSpeed);
	double dAcc = GetAxis(nMsId0)->LenToPulse(m_pParamAxis[nMsId0].fAcc);
	double dJerk = GetAxis(nMsId0)->LenToPulse(fSpeed / m_pParamAxis[nMsId0].fMinJerkTime);	// 0;

	//UINT16 nAxesNum = 2;
	//UINT16 arnAxes[2] = { nMsId0 + m_nOffsetAxisID, nMsId1 + m_nOffsetAxisID };
	double ardPosVal[2] = { dPulse0, dPulse1 };
	MC_DIRECTION pDirArray[2] = { mcPositiveDirection, mcPositiveDirection };
	UINT8 ErrorStopMode = 1;	// ErrorStop이 발생할 경우 처리 방법 선택(0: Error발생축만 정지, 1: 모든축 동작 정지)
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];

	//UINT32 GroupStatus = 0;
	//UINT32 AxisStatus = 0;
	//UINT32 AxisStatus2 = 0;
	//UINT16 GroupNo = NMC_GROUPID_INTERPOLRATION_X1Y1;
	//UINT16 PositionCount = 2;	// 2축 직선보간운동

	if (!m_pAxis[nMsId0]->IsGroupMotion() || !m_pAxis[nMsId1]->IsGroupMotion())
	{
		// MMCE0-Axis1를 Group의 Identity0로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
		Sleep(100);
		// MMCE0-Axis2를 Group의 Identity1로 추가
		ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
		Sleep(100);
		ms = MC_GroupEnable(m_nBoardId, GroupNo);
		Sleep(300);

		if (ms != MC_OK)
		{
			MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
			msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
			AfxMessageBox(msg);
			return FALSE;
		}

		m_bInterpolationMotion[1] = TRUE;
	}

	//MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//if (GroupStatus & GroupStandby)
	//{
	//	;
	//}
	//else
	//{
	//	// MMCE0-Axis1를 Group의 Identity0로 추가
	//	ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[0], GroupNo, 0);
	//	Sleep(100);
	//	// MMCE0-Axis2를 Group의 Identity1로 추가
	//	ms = MC_AddAxisToGroup(m_nBoardId, arnAxes[1], GroupNo, 1);
	//	Sleep(100);
	//	ms = MC_GroupEnable(m_nBoardId, GroupNo);
	//	Sleep(300);

	//	if (ms != MC_OK)
	//	{
	//		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
	//		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
	//		AfxMessageBox(msg);
	//		return FALSE;
	//	}
	//	else
	//	{
	//		//MC_GroupReadStatus를 통해 GroupEnable 되었는지 확인
	//		MC_GroupReadStatus(m_nBoardId, GroupNo, &GroupStatus);
	//		if (GroupStatus & GroupStandby)
	//		{
	//			;
	//		}
	//		else
	//		{
	//			//GroupEnable 실패하면 Status 출력
	//			msg.Format(_T("GroupEnable Fail, GroupStatus: 0x%04x"), GroupStatus);
	//			AfxMessageBox(msg);
	//			return FALSE;
	//		}
	//	}

	//	m_bInterpolationMotion[1] = TRUE;
	//}

	//Sleep(30);
	// mcACS 모드만 지원, BufferMode 선택, mcTMNone 모드만 지원, TransitionParameterCount: Reserved, TransitionParameter: Reserved
	ms = MC_MoveLinearAbsolute(m_nBoardId, GroupNo, PositionCount, ardPosVal, dVel, dAcc, dAcc, dJerk, mcACS, mcAborting, mcTMNone, 0, 0);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);
		return FALSE;
	}

	//DWORD nTick = GetTickCount();
	nTick = GetTickCount();

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
	if (lMaster >= m_nTotalMotion || lSlave >= m_nTotalMotion)
		return FALSE;

	m_bUseGantry = TRUE;
	m_lGantryMaster = lMaster;
	m_lGantrylSlave = lSlave;
	m_lGantryEnable = lOnOff;

	return TRUE;
}



BOOL CNmcDevice::IsHomeDone(int nMotionId)
{
	return !m_pAxis[nMotionId]->m_bHomeThreadAlive;
}

BOOL CNmcDevice::IsHomeDone()
{
	BOOL bAllHome = TRUE;

	//int nTotMs = m_nTotalMotion;
	//for (int i = 0; i<nTotMs; i++)
	int nTotAxis = m_nTotalAxis;
	for (int i = 0; i<nTotAxis; i++)
	{
		if (m_pAxis[i]->m_bHomeThreadAlive && m_pParamMotion[i].Home.bAct)
			bAllHome = FALSE;
	}

	return bAllHome;
}

BOOL CNmcDevice::IsHomeSuccess(int nMotionId)
{
	return m_bOrigin[nMotionId];
}

BOOL CNmcDevice::IsHomeSuccess()
{
	BOOL bAllHome = TRUE;

	int nTotMs = m_nTotalMotion;
	for (int i = 0; i < nTotMs; i++)
	{
		if (!m_bOrigin[i])
			bAllHome = FALSE;
	}

	return bAllHome;
}


BOOL CNmcDevice::IsMotionDone(int nMotionId)
{
	return (m_pAxis[nMotionId]->IsMotionDone());
}

BOOL CNmcDevice::IsInPosition(int nMotionId)
{
	return (m_pAxis[nMotionId]->IsInPosition());
}

double CNmcDevice::GetSpeedProfile(int nMode, int nAxisID, double fLength, double &fVel, double &fAcc, double &fJerk, int nSpeedType)
{
	double fMotionTime, fAccTime, fVelocityTime, fSettleTime, fTotalTime;

	double fMaxVelocity = m_pParamMotor[nAxisID].fLeadPitch*(m_pParamMotor[nAxisID].fRatingSpeed / 60.); // [mm/s]
	double fMaxAccel = m_pParamAxis[nAxisID].fMaxAccel; //[mm/s2]
	double fVelocity = m_pParamAxis[nAxisID].fSpd;
	double fMinJerkTime = m_pParamAxis[nAxisID].fMinJerkTime;
	double fMaxJerk = fMaxAccel / fMinJerkTime; //[mm/s3]
	double fAccPeriod = m_pParamAxis[nAxisID].fAccPeriod;
	double fAccLength = 0.0;

	if (nMode == S_CURVE)
	{

		if (nSpeedType == LOW_SPEED)
		{
			fVelocity /= 3.0;
			fMaxAccel /= 3.0;
			fMaxJerk /= 3.0;
			fMinJerkTime *= 3.0;
		}
		if (nSpeedType == MIDDLE_SPEED)
		{
			fVelocity /= 2.0;
			fMaxAccel /= 2.0;
			fMaxJerk /= 2.0;
			fMinJerkTime *= 2.0;
		}

		GetSCurveVelocity(fLength, fVelocity, fMaxAccel, fMaxJerk, fMinJerkTime);
		fVel = fVelocity;
		fAcc = fMaxAccel;
		fJerk = fMaxJerk;

		fMotionTime = GetMotionTime(fLength, fVelocity, fAcc, fJerk);
		fSettleTime = 0.1;
		fTotalTime = fMotionTime + fSettleTime;
		//		fJerkTime = GetJerkTime(fAcc,fJerk);
		//		fAccTime = GetAccTime(fVelocity,fAcc,fJerk);
		//		fVelocityTime = GetVelTime(fLength,fVelocity,fAcc,fJerk);
		//		fSettleTime = 0.1;
		//		fTotalTime = fJerkTime*4 + fAccTime*2 + fVelocityTime + fSettleTime;
	}
	else if (nMode == TRAPEZOIDAL)
	{
		fAcc = m_pParamAxis[nAxisID].fAcc;


		if (fAcc <= 0.0 || fAcc > fMaxAccel)
			fAcc = fMaxAccel;


		if (fAccPeriod > 50.0)
			fAccPeriod = 50.0;

		if (fAccPeriod < 50.0) // trapezoidal profile
		{

			fAccLength = fLength - (fLength*fAccPeriod*2.0 / 100.0);
			fAccTime = sqrt(2.0*fAccLength / fAcc);
		}
		else // triangular profile
		{

			fAccLength = fLength / 2.0;
			fAccTime = sqrt(2.0*fAccLength / fAcc);
		}


		double fAccTimeToMaxSpeed = fMaxVelocity / fAcc;
		if (fAccTime > fAccTimeToMaxSpeed)
		{
			fAccTime = fAccTimeToMaxSpeed;
			fVel = fMaxVelocity;
		}
		else
		{
			fVel = fAcc*fAccTime;
		}

		// 정속 구간의 이동시간을 구한다.
		fVelocityTime = (fLength - (fAccLength*2.0)) / fVel;
		//위치 정정 시간을 설정한다.
		double fSettleTime = 0.1;

		fTotalTime = fAccTime*2.0 + fVelocityTime + fSettleTime;

		if (nSpeedType == LOW_SPEED)
		{
			fVel /= 3.0;
			fAcc /= 3.0;
		}
		else if (nSpeedType == MIDDLE_SPEED)
		{
			fVel /= 2.0;
			fAcc /= 2.0;
		}

	}
	return fTotalTime;
}

double CNmcDevice::GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk, double dMinJerkTime)
{
	do
	{
		if ((dVel / dAcc) < (dAcc / dJerk))
			dVel = dAcc*dAcc / dJerk;
		double fTemp = (dLen / dVel - (dVel / dAcc + dAcc / dJerk));
		if (fTemp > 0)
			break;
		else if (fabs(fTemp) <= 0.000000001)
			break;
		else
		{
			double a = 1 / dAcc;
			double b = dAcc / dJerk;
			double c = -dLen;
			double r1 = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
			double r2 = (-b - sqrt(b*b - 4 * a*c)) / (2 * a);

			if (r1 > r2)
			{
				dVel = r1;
				if (dVel / dAcc < dAcc / dJerk)
					dAcc = sqrt(dJerk*dVel);
			}
			else
			{
				AfxMessageBox(_T("Calculation Error at Speed profile of S-Curve motion"));
			}
		}
	} while (1);

	return dVel;
}


double CNmcDevice::GetMotionTime(double dLen, double dVel, double dAcc, double dJerk)
{
	return dAcc / dJerk + dVel / dAcc + dLen / dVel;
}

int CNmcDevice::RestoreSwEmergency()	// -1: Fault , 1: Emergency Signal Off complete, 2: Previous Emergency Signal Off-state, 3: Normal
{
	MC_STATUS ms = MC_OK;
	UINT8 status;

	ms = MC_RestoreSWEmergency(m_nBoardId, &status);
	if (ms != MC_OK)
	{
		//CString sMsg;
		//sMsg.Format(_T("Error-MC_RestoreSWEmergency"));
		//AfxMessageBox(sMsg);
		return FALSE; // Error...
	}

	for (int nAxis = 0; nAxis < m_nTotalAxis; nAxis++)
	{
		if(GetAxis(nAxis)->IsAmpFault())
			GetAxis(nAxis)->AmpFaultReset();
	}
	return int(status);
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

BOOL CNmcDevice::IsHome(int nMotionId)
{
	return (GetAxis(nMotionId)->m_bOrigin);
}

void CNmcDevice::SetOriginStatus(int nAxisId, BOOL bStatus)
{
	CNmcAxis* axes = GetAxis(nAxisId);

	if (axes)
		axes->SetOriginStatus(bStatus);
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




BOOL CNmcDevice::Stop(int nMotionId)
{
	return (GetAxis(nMotionId)->Stop());
}

BOOL CNmcDevice::EStop(int nMotionId)
{
	return (GetAxis(nMotionId)->EStop());
}

BOOL CNmcDevice::Clear(int nMotionId)
{
	return (GetAxis(nMotionId)->AmpFaultReset());
}

BOOL CNmcDevice::VMoveStop(int nMotionId, int nDir)
{
	return (GetAxis(nMotionId)->Stop());
}

BOOL CNmcDevice::IsLimit(int nMotionId, int nDir)
{
	BOOL bCheck = FALSE;
	if(nDir == M_CW)
		bCheck = GetAxis(nMotionId)->CheckPosLimitSwitch();
	else
		bCheck = GetAxis(nMotionId)->CheckNegLimitSwitch();

	return bCheck;
}

long CNmcDevice::GetState(int nMotionId)
{
	return (GetAxis(nMotionId)->GetState());
}

BOOL CNmcDevice::Abort(int nMotionId)
{
	return (GetAxis(nMotionId)->SetAmpEnable(FALSE));
}
BOOL CNmcDevice::IsEnable(int nMsId)
{
	BOOL bEnable;
	bEnable = GetAxis(nMsId)->GetAmpEnable();
	return(bEnable);
}

BOOL CNmcDevice::IsServoOn(int nMotorID)
{
	return IsEnable(nMotorID);
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

void CNmcDevice::SaveLog(CString strMsg)
{
	CSafeLock lock(&m_csLogLock);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;
	CFileOperation fileOp(GetWindowVersion());
	GetModuleFileName(NULL, szPath, MAX_PATH);
	pszPos = _tcsrchr(szPath, '\\');
	*pszPos = NULL;

	_stprintf(szPath, _T("C:\\R2RSet\\Log"));
	fileOp.DoCreateFolder(szPath, NULL);
	COleDateTime time = COleDateTime::GetCurrentTime();

	_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));

	CString strDate;
	CString strContents;

	CTime now;
	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));


	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	CFile file;

	CFileFind finder;
	BOOL bFound = FALSE;
	if (finder.FindFile(szFile))
		bFound = TRUE;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	file.SeekToEnd();

	if (!bFound)
	{
#ifdef UNICODE
		USHORT uBOM = 0xfeff;

		WORD MASK = 0xfeff;
		file.Write(&MASK, sizeof(WORD));
#endif
	}

	file.Write(strContents, strContents.GetLength() * sizeof(TCHAR));

	file.Flush();
	file.Close();
}

tpOs CNmcDevice::GetWindowVersion()
{
	return m_cWindowVersionCollect.GetOsInfo();
}

#endif	// #ifdef USE_NMC