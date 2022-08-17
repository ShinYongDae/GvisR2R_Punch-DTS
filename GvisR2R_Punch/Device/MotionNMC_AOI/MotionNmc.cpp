// MotionNew.cpp : implementation file
//

#include "../stdafx.h"

#ifdef USE_NMC


#include "gvisaoi.h"
#include "MainFrm.h"
#include "GvisAOIDoc.h"
#include "GvisAOIView.h"
//#include "Adlink_IO.h"
#include <math.h>

#include "MotionNew.h"
//#include "NMC_DEFINE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CMotionNew* pMotionNew;
extern CMainFrame* pMainFrame;
extern CGvisAOIDoc* pGlobalDoc;
extern CGvisAOIView* pGlobalView;
extern CAdlink_IO *pIO;
extern BOOL g_bLoopWh;

#ifdef USE_LMI_SENSOR
extern  CLMI g_lmi;
extern AFActorSequence g_SequenceAF;
extern AF_ACTOR_TYPE g_nCurrentActorType;// = AF_ACTOR_TYPE::ACTOR_PTAPPEND;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMotionNew

CMotionNew::CMotionNew(CWnd* pParent)
{
	int i;
	UINT16 nBoardID = 0;
	UINT16 nDevIdIoIn = 1009;
	UINT16 nDevIdIoOut = 1010;

	m_nBoardId = nBoardID;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = 1;
	m_nGroupID_RTAF = NMC_GROUPID_RTAF;
	m_nGroupID_ErrMapXY = NMC_GROUPID_ERRMAP;
	m_nGroupID_Interpolation = NMC_GROUPID_INTERPOLRATION;

	m_bUseGantry = FALSE;
	m_lGantryMaster = -1;
	m_lGantrylSlave = -1;
	m_lGantryEnable = -1;
	m_bGantryEnabled = FALSE;


	m_sErrorMappingFilePath = _T("");

	m_dOrgStartPos = 0.0;
	m_dScanVel = 0.0;

	m_pRecordScanPos = new double[16384];
	m_pRecordFocusPos = new double[16384];

	m_bPartialMode = 0;
	m_bPanelSyncMode = 0;
	m_bEnableRTAF = 0;
	m_nCurMotorSwath = 0;
	//161010 lgh 
	m_stScanParam[0].nScanMode = SCAN_MODE_SAFE;
	m_stScanParam[0].bHoldAction = 0;
	m_stScanParam[0].dHoldingPosX = 0;
	m_stScanParam[0].dNextSwathStartPosY = 0;
	m_stScanParam[0].bScanPosMove = 0;

	m_stScanParam[1].nScanMode = SCAN_MODE_SAFE;
	m_stScanParam[1].bHoldAction = 0;
	m_stScanParam[1].dHoldingPosX = 0;
	m_stScanParam[1].dNextSwathStartPosY = 0;
	m_stScanParam[1].bScanPosMove = 0;
	//161010 end

	m_dCurrFocusPos = 0;
	pMotionNew = this;
	m_pParent = pParent;
	m_dCurrentDBNStartPos = 0;
	m_dCurrentDBNEndPos = 0;
	m_pParamMotion = NULL;
	m_pParamAxis = NULL;
	m_pParamMotor = NULL;
	m_fPosRes = NULL;

	m_pMotionCard = NULL;
	m_bExit = 0;
	for (i = 0; i < MAX_AXIS; i++)
		m_bOrigin[i] = FALSE;

	m_fPrevThicknessByLaserSensor = -100;
#ifndef RI_SYSTEM
#if TRIGGER_TYPE == GVIS_TRIGGER
	m_pGtkDBN = NULL;
#endif
#endif
	m_pMonitorThread = NULL; // monitor thread pointer initilaize
	m_pRevolverLensPos = NULL;
	m_fStartPos = 0.0;

	m_pMyFileErrMap = NULL;

	LoadParam();

	RECT rt = { 0,0,0,0 };

}

CMotionNew::~CMotionNew()
{
	delete[] m_pRecordScanPos;// = new double[16384];
	delete[]  m_pRecordFocusPos;// = new double[16384];


	if (m_pMotionCard)
	{
		if (!m_pMotionCard->DestroyDevice())
			return;

		delete m_pMotionCard;
		m_pMotionCard = NULL;
	}

	if (!m_bExit)
	{
		m_bExit = TRUE;
		DestroyWindow();
	}

	if (m_pMonitorThread)
		StopMonitorThread();

	if (!FreeAll())
		AfxMessageBox(_T("[MSG289] Motion Memory Free Error."));
}


BEGIN_MESSAGE_MAP(CMotionNew, CWnd)
	//{{AFX_MSG_MAP(CMotionNew)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CMotionNew::m_bMotionCreated = 0;
CMotionNew* CMotionNew::m_pMotionNew = nullptr;

CMotionNew* CMotionNew::GetMotion()
{
	if (CMotionNew::m_pMotionNew == nullptr)
	{
		CMotionNew::m_pMotionNew = new CMotionNew(AfxGetMainWnd());
	}

	return (CMotionNew*)CMotionNew::m_pMotionNew;
}

/////////////////////////////////////////////////////////////////////////////
// CMotionNew message handlers



BOOL CMotionNew::LoadAoiMotionParametersFromAxis(int nAxis)
{
	if (nAxis >= MAX_AXIS)
		return FALSE;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisAOIView *pView = (CGvisAOIView *)pFrame->GetActiveView();
	CGvisAOIDoc *pDoc = (CGvisAOIDoc *)pFrame->GetActiveDocument();

	CString sVal, sIdx;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;

	int nID, nCol, i, nAxisID;
	CString strValue;

	// [OBJECT MAPPING]
	for (nID = 0; nID < m_ParamCtrl.nTotAxis; nID++)
	{
		sIdx.Format(_T("AXIS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		{
			if (nAxis == nID)
			{
				token = _tcstok(szData, sep);
				nCol = 0;
				if (token != NULL)
					m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);

				while (token != NULL)
				{
					nCol++;
					if (nCol >= 5)
						break;
					token = _tcstok(NULL, sep);
					if (token != NULL)
						m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);
				}

				m_pParamAxis[nID].stAxisMap.nNumMotor = nCol;
			}
		}
	}


	nID = nAxis;
	sIdx.Format(_T("MS%d"), nID);
	if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nCol = 0;
		if (token != NULL)
		{
			m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
			m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
			for (i = 0; i < m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
				m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
		}

		while (token != NULL)
		{
			nCol++;
			if (nCol >= 5)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
			{
				m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
				m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
				for (i = 0; i < m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
					m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
			}
		}

		m_pParamMotion[nID].stMsMap.nNumAxis = nCol;
	}



	// [AXIS PARAM]
	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AXIS NAME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamAxis[nID].sName = CString(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamAxis[nID].sName = CString(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MAX ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamAxis[nID].fMaxAccel = _tcstod(token, &stopstring)*9.8*1000.0;

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamAxis[nID].fMaxAccel = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MIN JERK TIME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nAxis == nID)
			m_pParamAxis[nID].fMinJerkTime = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamAxis[nID].fMinJerkTime = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("INPOSITION LENGTH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);
		}
	}

	// [MOTOR PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR TYPE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bType = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bType = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR RATING SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR DIRECTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].nDir = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].nDir = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("FEEDBACK SOURCE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bEncoder = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bEncoder = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER PULSE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].nEncPulse = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].nEncPulse = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("LEAD PITCH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER MULTIPLIER"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].nEncMul = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].nEncMul = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("GEAR RATIO"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AMP ENABLE LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AMP FAULT LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("POS LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("NEG LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("HOME SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].bHomeLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].bHomeLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bHomeLevel = 1; // Home sensor active Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);
		}
	}

	// [MOTION PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTION PARAM"), _T("ESTOP TIME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);
		}
	}

	// [HOME PARAM]
	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ACTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.bAct = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.bAct = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("INDEX HOME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.bIndex = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.bIndex = _ttoi(token);
		}
	}
	else
	{
		for (nID = 0; nID < m_ParamCtrl.nTotAxis; nID++)
			m_pParamMotion[nID].Home.bIndex = 0;
	}
	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("SEARCH DIRECTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.nDir = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.nDir = _ttoi(token);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ESCAPE LENGTH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("1'st SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("2'nd SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("SHIFT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("OFFSET"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);
		}
	}

	// [SPEED PARAM]
	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
		{
			m_pParamMotion[nID].Speed.fSpd = _tcstod(token, &stopstring);
			m_pParamAxis[nID].fSpd = m_pParamMotion[nID].Speed.fSpd;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
			{
				m_pParamMotion[nID].Speed.fSpd = _tcstod(token, &stopstring);
				m_pParamAxis[nID].fSpd = m_pParamMotion[nID].Speed.fSpd;
			}
		}
	}

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
		{
			m_pParamMotion[nID].Speed.fAcc = _tcstod(token, &stopstring)*9.8*1000.0;
			m_pParamAxis[nID].fAcc = m_pParamMotion[nID].Speed.fAcc;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
			{
				m_pParamMotion[nID].Speed.fAcc = _tcstod(token, &stopstring)*9.8*1000.0;
				m_pParamAxis[nID].fAcc = m_pParamMotion[nID].Speed.fAcc;
			}
		}
	}

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
		{
			m_pParamMotion[nID].Speed.fDec = _tcstod(token, &stopstring)*9.8*1000.0;
			m_pParamAxis[nID].fDec = m_pParamMotion[nID].Speed.fDec;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
			{
				m_pParamMotion[nID].Speed.fDec = _tcstod(token, &stopstring)*9.8*1000.0;
				m_pParamAxis[nID].fDec = m_pParamMotion[nID].Speed.fDec;
			}
		}
	}


	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
		{
			m_pParamMotion[nID].Speed.fAccPeriod = _tcstod(token, &stopstring);
			m_pParamAxis[nID].fAccPeriod = m_pParamMotion[nID].Speed.fAccPeriod;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
			{
				m_pParamMotion[nID].Speed.fAccPeriod = _tcstod(token, &stopstring);
				m_pParamAxis[nID].fAccPeriod = m_pParamMotion[nID].Speed.fAccPeriod;
			}
		}
	}

	// [JOG PARAM]
	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("FAST SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("MIDDLE SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("LOW SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);
		}
	}

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL && nID == nAxis)
			m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL && nID == nAxis)
				m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}

	return TRUE;
}

void CMotionNew::LoadParam()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisAOIView *pView = (CGvisAOIView *)pFrame->GetActiveView();
	CGvisAOIDoc *pDoc = (CGvisAOIDoc *)pFrame->GetActiveDocument();

	CString sVal, sIdx;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;

	int nID, nCol, i, nAxisID;
	CString strValue;

	// [CONTROL PARAM]
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotMotion = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotion = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL AXIS"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotAxis = _ttoi(szData);
	else
		m_ParamCtrl.nTotAxis = 0;
	m_nTotAxis = m_ParamCtrl.nTotAxis;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTOR"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotMotor = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotor = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL FILTER"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotFilter = _ttoi(szData);
	else
		m_ParamCtrl.nTotFilter = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL CAPTURE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotCapture = _ttoi(szData);
	else
		m_ParamCtrl.nTotCapture = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL SEQUENCE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nTotSequence = _ttoi(szData);
	else
		m_ParamCtrl.nTotSequence = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("SYNQNET TX TIME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nSynqnetTxTime = _ttoi(szData);
	else
		m_ParamCtrl.nSynqnetTxTime = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("Firmware Option"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_ParamCtrl.nFirmwareOption = _ttoi(szData);
	else
		m_ParamCtrl.nFirmwareOption = 0;

	// CreateObject - DataParam
	if (!m_pParamMotion)
	{
		m_pParamMotion = new MotionMotion[m_ParamCtrl.nTotMotion];
		((CMotion*)m_pParent)->m_pParamMotion = m_pParamMotion;
	}
	if (!m_pParamAxis)
	{
		m_pParamAxis = new MotionAxis[m_ParamCtrl.nTotAxis];
		((CMotion*)m_pParent)->m_pParamAxis = m_pParamAxis;
	}
	if (!m_pParamMotor)
	{
		m_pParamMotor = new MotionMotor[m_ParamCtrl.nTotMotor];
		((CMotion*)m_pParent)->m_pParamMotor = m_pParamMotor;
	}

	if (!m_fPosRes)
		m_fPosRes = new double[m_ParamCtrl.nTotMotor];


	// [OBJECT MAPPING]
	for (nID = 0; nID < m_ParamCtrl.nTotAxis; nID++)
	{
		sIdx.Format(_T("AXIS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		{
			token = _tcstok(szData, sep);
			nCol = 0;
			if (token != NULL)
				m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);

			while (token != NULL)
			{
				nCol++;
				if (nCol >= 5)
					break;
				token = _tcstok(NULL, sep);
				if (token != NULL)
					m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);
			}

			m_pParamAxis[nID].stAxisMap.nNumMotor = nCol;
		}
		else
			m_pParamAxis[nID].stAxisMap.nNumMotor = 0;
	}
	for (nID = 0; nID < m_ParamCtrl.nTotMotion; nID++)
	{
		sIdx.Format(_T("MS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		{
			token = _tcstok(szData, sep);
			nCol = 0;
			if (token != NULL)
			{
				m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
				m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
				for (i = 0; i < m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
					m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
			}

			while (token != NULL)
			{
				nCol++;
				if (nCol >= 5)
					break;
				token = _tcstok(NULL, sep);
				if (token != NULL)
				{
					m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
					m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
					for (i = 0; i < m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
						m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
				}
			}

			m_pParamMotion[nID].stMsMap.nNumAxis = nCol;
		}
		else
			m_pParamMotion[nID].stMsMap.nNumAxis = 0;
	}


	// [AXIS PARAM]
	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AXIS NAME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamAxis[nID].sName = CString(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamAxis[nID].sName = CString(token);
		}
	}
	else
		m_pParamAxis[nID].sName = _T(""); // Axis Name

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MAX ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamAxis[nID].fMaxAccel = m_fMaxAccel[nID] = _tcstod(token, &stopstring)*9.8*1000.0;

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamAxis[nID].fMaxAccel = m_fMaxAccel[nID] = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamAxis[nID].fMaxAccel = m_fMaxAccel[nID] = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MIN JERK TIME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamAxis[nID].fMinJerkTime = m_fMinJerkTime[nID] = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamAxis[nID].fMinJerkTime = m_fMinJerkTime[nID] = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamAxis[nID].fMinJerkTime = 0.0; // [sec]


	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("INPOSITION LENGTH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamAxis[nID].fInpRange = 0.0;	// Fine position tolerance of each motor

											// [MOTOR PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR TYPE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bType = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bType = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bType = 1; // 0: Servo Motor, 1: Step Motor

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR RATING SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fRatingSpeed = 0.0; // 0: Servo Motor, 1: Step Motor

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR DIRECTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].nDir = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].nDir = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nDir = 1; // -1: CCW, 1: CW

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("FEEDBACK SOURCE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bEncoder = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bEncoder = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bEncoder = 0; // 1: External Encoder 0: Internal Pulse Out

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER PULSE"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].nEncPulse = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].nEncPulse = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nEncPulse = 0; // [pulse/rev]

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("LEAD PITCH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fLeadPitch = 0.0; // Ball Screw Lead Pitch [mm]

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER MULTIPLIER"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].nEncMul = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].nEncMul = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nEncMul = 0; // Encoder Multiplier

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("GEAR RATIO"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fGearRatio = 0.0; // Gear Ratio


	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AMP ENABLE LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bAmpEnableLevel = 1; // Amp Enable Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AMP FAULT LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bAmpFaultLevel = 1; // Amp Fault Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("POS LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bPosLimitLevel = 1; // Positive Limit sensor active Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("NEG LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bNegLimitLevel = 1; // Negative Limit sensor active Level 0: Low Active 1: High Active

											   //if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("HOME SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
											   //{
											   //	token = _tcstok(szData,sep);
											   //	nID = 0;
											   //	if(token != NULL)
											   //		m_pParamMotor[nID].bHomeLevel = _ttoi(token);

											   //	while( token != NULL )
											   //	{
											   //		nID++;
											   //		if(nID >= m_ParamCtrl.nTotMotor)
											   //			break;
											   //		token = _tcstok(NULL,sep);
											   //		if(token != NULL)
											   //			m_pParamMotor[nID].bHomeLevel = _ttoi(token);
											   //	}
											   //}
											   //else
											   //	m_pParamMotor[nID].bHomeLevel = 1; // Home sensor active Level 0: Low Active 1: High Active
	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("HOME SENSOR LEVEL"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].bHomeLevel = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].bHomeLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bHomeLevel = 1; // Home sensor active Level 0: Low Active 1: High Active


	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fPosLimit = m_fPosLimit[nID] = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fPosLimit = m_fPosLimit[nID] = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fPosLimit = m_fPosLimit[nID] = 0.0; // Sotftware positive motion limit


	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fNegLimit = m_fNegLimit[nID] = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fNegLimit = m_fNegLimit[nID] = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fNegLimit = m_fNegLimit[nID] = 0.0; // Sotftware negative motion limit


	// [MOTION PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTION PARAM"), _T("ESTOP TIME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].dEStopTime = 0.1; // [Sec]


	// [HOME PARAM]
	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ACTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.bAct = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.bAct = _ttoi(token);
		}
	}
	else
		m_pParamMotion[nID].Home.bAct = 0; // 0: Don't 1:Do

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("INDEX HOME"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.bIndex = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.bIndex = _ttoi(token);
		}
	}
	else
	{
		for (nID = 0; nID < m_ParamCtrl.nTotAxis; nID++)
			m_pParamMotion[nID].Home.bIndex = 0;
	}

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("SEARCH DIRECTION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.nDir = _ttoi(token);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.nDir = _ttoi(token);
		}
	}
	else
		m_pParamMotion[nID].Home.nDir = -1; // -1: Minus 1:Plus

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ESCAPE LENGTH"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fEscLen = 0.0; // Escape length from home sensor touch position


	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("1'st SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.f1stSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed for home search 

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("2'nd SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.f2ndSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed for home search 

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fAcc = 0.0; // [mm/s^2] or [deg/s^2]

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("SHIFT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fShift = 0.0; // [mm]

	if (0 < ::GetPrivateProfileString(_T("ORIGIN PARAM"), _T("OFFSET"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fOffset = 0.0; // [mm] or [deg]


	// [SPEED PARAM]

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
		{
			m_pParamMotion[nID].Speed.fSpd = m_fVel[nID] = _tcstod(token, &stopstring);
			m_pParamAxis[nID].fSpd = m_pParamMotion[nID].Speed.fSpd;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
			{
				m_pParamMotion[nID].Speed.fSpd = m_fVel[nID] = _tcstod(token, &stopstring);
				m_pParamAxis[nID].fSpd = m_pParamMotion[nID].Speed.fSpd;
			}
		}
	}
	else
	{
		m_pParamMotion[nID].Speed.fSpd = m_fVel[nID] = 0.0; // [mm/s]
		m_pParamAxis[nID].fSpd = m_pParamMotion[nID].Speed.fSpd; // [mm/s]
	}


	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
		{
			m_pParamMotion[nID].Speed.fAcc = m_fAcc[nID] = _tcstod(token, &stopstring)*9.8*1000.0;
			m_pParamAxis[nID].fAcc = m_pParamMotion[nID].Speed.fAcc;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
			{
				m_pParamMotion[nID].Speed.fAcc = m_fAcc[nID] = _tcstod(token, &stopstring)*9.8*1000.0;
				m_pParamAxis[nID].fAcc = m_pParamMotion[nID].Speed.fAcc;
			}
		}
	}
	else
	{
		m_pParamMotion[nID].Speed.fAcc = m_fAcc[nID] = 0.0; // [G]=9.8*10^6[m/s^2]
		m_pParamAxis[nID].fAcc = m_pParamMotion[nID].Speed.fAcc;
	}


	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
		{
			m_pParamMotion[nID].Speed.fDec = m_fDec[nID] = _tcstod(token, &stopstring)*9.8*1000.0;
			m_pParamAxis[nID].fDec = m_pParamMotion[nID].Speed.fDec;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
			{
				m_pParamMotion[nID].Speed.fDec = m_fDec[nID] = _tcstod(token, &stopstring)*9.8*1000.0;
				m_pParamAxis[nID].fDec = m_pParamMotion[nID].Speed.fDec;
			}
		}
	}
	else
	{
		m_pParamMotion[nID].Speed.fDec = m_fDec[nID] = 0.0; // [G]=9.8*10^6[m/s^2]
		m_pParamAxis[nID].fDec = m_pParamMotion[nID].Speed.fDec;
	}


	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
		{
			m_pParamMotion[nID].Speed.fAccPeriod = m_fAccPeriod[nID] = _tcstod(token, &stopstring);
			m_pParamAxis[nID].fAccPeriod = m_pParamMotion[nID].Speed.fAccPeriod;
		}

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
			{
				m_pParamMotion[nID].Speed.fAccPeriod = m_fAccPeriod[nID] = _tcstod(token, &stopstring);
				m_pParamAxis[nID].fAccPeriod = m_pParamMotion[nID].Speed.fAccPeriod;
			}
		}
	}
	else
	{
		m_pParamMotion[nID].Speed.fAccPeriod = m_fAccPeriod[nID] = 0.0; // [%]
		m_pParamAxis[nID].fAccPeriod = m_pParamMotion[nID].Speed.fAccPeriod;
	}


	// [JOG PARAM]
	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("FAST SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogFastSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("MIDDLE SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogMidSpd = 0.0; // [mm/s] or [deg/s^2] Middle speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("LOW SPEED"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogLowSpd = 0.0; // [mm/s] or [deg/s^2] Middle speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogAcc = 0.0; // [G]=9.8*10^6[m/s^2]


#ifdef USE_ATF_MODULE
	int nHomingParamID;

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("Homing Move"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		for (nHomingParamID = 0; nHomingParamID < 4; nHomingParamID++)
		{
			if (nHomingParamID == 0)
				sVal = _tcstok(szData, sep);
			else
				sVal = _tcstok(NULL, sep);

			m_nAtfHomingParam[nHomingParamID] = _ttoi(sVal);
		}
	}
	else
	{
		for (nHomingParamID = 0; nHomingParamID < 4; nHomingParamID++)
		{
			m_nAtfHomingParam[nHomingParamID] = 0;
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("Homing Speed"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		for (nHomingParamID = 4; nHomingParamID < 8; nHomingParamID++)
		{
			if (nHomingParamID == 4)
				sVal = _tcstok(szData, sep);
			else
				sVal = _tcstok(NULL, sep);

			m_nAtfHomingParam[nHomingParamID] = _ttoi(sVal);
		}
	}
	else
	{
		for (nHomingParamID = 4; nHomingParamID < 8; nHomingParamID++)
		{
			m_nAtfHomingParam[nHomingParamID] = 0;
		}
	}

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("FullStep Per MM"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		m_nAtfFullStepPerMM = _ttoi(szData);
	}
	else
	{
		m_nAtfFullStepPerMM = 500;
	}

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("MicroStep Number"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		m_nAtfMicroStepNum = _ttoi(szData);
	}
	else
	{
		m_nAtfMicroStepNum = 20;
	}

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		m_fAtfPosSWLimitPos = _ttof(szData);
	}
	else
	{
		m_fAtfPosSWLimitPos = 0;
	}

	if (0 < ::GetPrivateProfileString(_T("ATF PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		m_fAtfNegSWLimitPos = _ttof(szData);
	}
	else
	{
		m_fAtfNegSWLimitPos = 0;
	}
#endif


	if (0 < ::GetPrivateProfileString(_T("REVOLVER AXIS"), _T("Total Lens"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		m_nRevolverLensTotal = _ttoi(szData);
	else
		m_nRevolverLensTotal = 0;

	if (m_nRevolverLensTotal > 0)
	{
		if (m_pRevolverLensPos)
		{
			delete[] m_pRevolverLensPos;
			m_pRevolverLensPos = NULL;
		}
		m_pRevolverLensPos = new double[m_nRevolverLensTotal];

		if (0 < ::GetPrivateProfileString(_T("REVOLVER AXIS"), _T("Lens Pos"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
		{
			for (i = 0; i < m_nRevolverLensTotal; i++)
			{
				if (i == 0)
					strValue = _tcstok(szData, sep);
				else
					strValue = _tcstok(NULL, sep);
				m_pRevolverLensPos[i] = _ttof(strValue);
			}
		}
		else
		{
			for (i = 0; i < m_nRevolverLensTotal; i++)
				m_pRevolverLensPos[i] = 0.0;
		}
	}
	else
	{
		if (m_pRevolverLensPos)
		{
			delete[] m_pRevolverLensPos;
			m_pRevolverLensPos = NULL;
		}
	}


	for (i = 0; i < m_ParamCtrl.nTotMotor; i++)
	{
		m_fPosRes[i] = (double)(m_pParamMotor[i].fLeadPitch*m_pParamMotor[i].fGearRatio) / (double)(m_pParamMotor[i].nEncPulse*m_pParamMotor[i].nEncMul); // [mm]
	}
}

void CMotionNew::MotionAbortAll()
{
	for (int nAxisId = 0; nAxisId < MAX_AXIS; nAxisId++)
	{
		MotionAbort(nAxisId);	// equalize Command position and Actual Position
	}
}

void CMotionNew::SetMotionParam()
{
	if (m_pMotionCard)
		m_pMotionCard->SetParam();
}



// [MS Summary]

BOOL CMotionNew::MotionAbort(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
#if CUSTOMER_COMPANY == SUMITOMO	//20120919-ndy for EmergencyStop by HomeSensor
#ifndef _DEBUG		//121031 hjc add
	if (nMsId != SCAN_AXIS || pGlobalDoc->m_bEmergencyStatus != TRUE)
		return Abort(nMsId);
#else
#ifdef EMERGENCY_STOP_HOME_SENSOR	//121031 hjc add
	if (nMsId != SCAN_AXIS || pGlobalDoc->m_bEmergencyStatus != TRUE)
		return Abort(nMsId);
#else
	return Abort(nMsId);
#endif
#endif
#else
	return Abort(nMsId);
#endif
#endif
	return TRUE;
}

BOOL CMotionNew::Abort(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return (m_pMotionCard->Abort(nMsId));
	return m_pMotionCard->GetAxis(nMsId)->SetAmpEnable(FALSE);
#endif
	return TRUE;
}

BOOL CMotionNew::EStop(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return (m_pMotionCard->EStop(nMsId));
	return m_pMotionCard->GetAxis(nMsId)->SetEStop();
#endif
	return TRUE;
}

BOOL CMotionNew::ClearMotionFault(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_pParamMotor[nMsId].bType == STEPPER) // servo
	//	{
	//		m_pMotionCard->m_pMotor[nMsId].ResetEasyServo();
	//		Sleep(50);
	//	}
	//#endif
	//
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	Clear(nMsId);


#endif


	return TRUE;
}

BOOL CMotionNew::IsStop(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//long lSts = GetState(nMsId);
	//if( lSts == MPIStateIDLE ||
	//	lSts == MPIStateSTOPPED ||
	//	lSts == MPIStateSTOPPING ||
	//	lSts == MPIStateERROR )
	//{
	//	m_nMoveDir[nMsId] = STOP_DIR;
	//	return TRUE;
	//}
	//return FALSE;

	return m_pMotionCard->GetAxis(nMsId)->IsAxisDone();
#endif
	return TRUE;
}

BOOL CMotionNew::Stop(int nMsId, double fStopTime)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	SetStopTime(nMsId, (float)fStopTime);
	return (m_pMotionCard->GetAxis(nMsId)->Stop(nMsId));
	//m_pMotionCard->GetAxis(nMsId)->StopVelocityMove(TRUE);
#endif
	return TRUE;
}


// [Axis Summary]

BOOL CMotionNew::IsMotionDone(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return ((BOOL)m_pMotionCard->IsMotionDone(nAxisId));
	return (BOOL)(m_pMotionCard->GetAxis(nAxisId)->CheckMotionDone() ? TRUE : FALSE);
#endif
	return TRUE;
}

BOOL CMotionNew::IsInPosition(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return ((BOOL)m_pMotionCard->IsInPosition(nAxisId));
	return (BOOL)(m_pMotionCard->GetAxis(nAxisId)->CheckInMotion() ? TRUE : FALSE);
#endif
	return TRUE;
}

BOOL CMotionNew::CheckMotionDone(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION
#ifdef USE_ATF_MODULE
	if (nAxisId == FOCUS_AXIS)
	{
		int err;
		int nAtfHWStat;

		if (pGlobalView->m_bATFConnect)
		{
			err = 0;
			nAtfHWStat = 0;

			err = atf_ReadHwStat(&nAtfHWStat);

			if (err == ErrOK)
			{
				if (!(nAtfHWStat & 0x1000))
					return TRUE;
				else
					return FALSE;
			}
			else
			{
				AfxMessageBox(_T("[MSG293] atf_ReadHwStat Function Error!"));
				return TRUE;
			}
		}
		else
		{
			AfxMessageBox(_T("[MSG294] ATF Connect Error!"));
			return TRUE;
		}
	}
#endif
#endif
#ifdef MOTION_ACTION
	if (!IsMotionDone(nAxisId))
		return FALSE;
#endif
	return TRUE;
}

BOOL CMotionNew::IsAxisDone(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//if(!IsMotionDone(nAxisId) || !IsInPosition(nAxisId))
	//	return FALSE;
	return (BOOL)(m_pMotionCard->GetAxis(nAxisId)->IsAxisDone() ? TRUE : FALSE);
#endif
	return TRUE;
}

BOOL CMotionNew::CheckAxisDone(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//if(!IsMotionDone(nAxisId) || !IsInPosition(nAxisId))
	//	return FALSE;
	return (BOOL)(m_pMotionCard->GetAxis(nAxisId)->CheckAxisDone() ? TRUE : FALSE);
#endif
	return TRUE;
}

double CMotionNew::GetActualPosition(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef USE_ATF_MODULE
	if (nAxisId == FOCUS_AXIS)
	{
		int nAbsZPos;
		int err;
		double fPos;

		err = atf_ReadAbsZPos(&nAbsZPos);
		if (err == ErrOK)
		{
			fPos = ((double)nAbsZPos * 1000.0) / ((double)m_nAtfMicroStepNum * (double)m_nAtfFullStepPerMM) / 1000.0;
			return fPos;
		}
		else
		{
			fPos = 0.0;
			return fPos;
		}
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	double dPos = -10000.0;
	m_csGetActualPosition.Lock();
	//dPos = m_pMotionCard->GetActualPosition(nAxisId);
	dPos = m_pMotionCard->GetAxis(nAxisId)->GetActualPosition();
	m_csGetActualPosition.Unlock();
	return(dPos);
#endif
	return 0.0;
}

double CMotionNew::GetCommandPosition(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef USE_ATF_MODULE
	if (nAxisId == FOCUS_AXIS)
	{
		int nAbsZPos;
		int err;
		double fPos;

		err = atf_ReadAbsZPos(&nAbsZPos);
		if (err == ErrOK)
		{
			fPos = ((double)nAbsZPos * 1000.0) / ((double)m_nAtfMicroStepNum * (double)m_nAtfFullStepPerMM) / 1000.0;
			return fPos;
		}
		else
		{
			fPos = 0.0;
			return fPos;
		}
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	double dPos = -10000.0;
	m_csGetActualPosition.Lock();
	//dPos = m_pMotionCard->GetCommandPosition(nAxisId);
	dPos = m_pMotionCard->GetAxis(nAxisId)->GetCommandPosition();
	m_csGetActualPosition.Unlock();
	return(dPos);
#endif
	return 0.0;
}

int CMotionNew::GetAxisState(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return int(m_pMotionCard->GetState(nAxisId));
	int nRtn = 0;
	nRtn = m_pMotionCard->GetAxis(nAxisId)->CheckAxisState();
	if (nRtn & ST_INPOSITION_STATUS)
		return MPIStateIDLE;
	if (nRtn & ST_ERROR_STOP)
		return MPIStateERROR;
	//if (nRtn & ST_AMP_FAULT)
	//	return MPIMessageFATAL_ERROR;
	//if (nRtn & MPIStateSTOPPING)
	//	return MPIMessageFATAL_ERROR;
	if ((nRtn & ST_POS_MOVING) || (nRtn & ST_NEG_MOVING))
		return MPIStateMOVING;
#endif
	return 0;
}

BOOL CMotionNew::SetActualPosition(int nAxisId, double fData)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->SetActualPosition(nAxisId, fData);
	return m_pMotionCard->GetAxis(nAxisId)->SetPosition(fData);
#endif
	return TRUE;
}





void CMotionNew::SetErrorLimitAction(int nMotorID, MPIAction Action)
{
	if (nMotorID >= MAX_AXIS)
		return;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//m_pMotionCard->SetErrorLimitAction(nMotorID, Action);
#endif
	return;
}

BOOL CMotionNew::SetCommandPosition(int nAxisId, double fData)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->SetCommandPosition(nAxisId, fData);
	return m_pMotionCard->GetAxis(nAxisId)->SetPosition(fData);
#endif
	return TRUE;
}

BOOL CMotionNew::SetSettleTime(int nAxisId, float fTime)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->SetSettleTime(nAxisId, fTime);
#endif
	return TRUE;
}


// [Motor Summary]

double CMotionNew::GetPosSWLimitValue(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0.0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->GetPosSWLimitValue(nMotorId);
	return m_fPosLimit[nMotorId];
#endif
	return 0.0;
}

double CMotionNew::GetNegSWLimitValue(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0.0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef USE_ATF_MODULE
	if (nMotorId == FOCUS_AXIS)
		return m_fAtfNegSWLimitPos;
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->GetNegSWLimitValue(nMotorId);
	return m_fNegLimit[nMotorId];
#endif
	return 0.0;
}

BOOL CMotionNew::ServoOnOff(int nMotorId, BOOL bOnOff)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->ServoOn(nMotorId, bOnOff);
	return m_pMotionCard->GetAxis(nMotorId)->SetAmpEnable(bOnOff);
#endif
	return TRUE;
}

BOOL CMotionNew::GetOrgInput(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->GetOrgInput(nMotorId);
	return m_pMotionCard->GetAxis(nMotorId)->CheckHomeSwitch();
#endif
	return TRUE;
}

BOOL CMotionNew::GetLimitInput(int nMotorId, int nDir)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	BOOL bOnOff;

	if (nDir == PLUS)
	{
		bOnOff = IsLimit(nMotorId, CW);
		if (!m_pParamMotor[nMotorId].bPosLimitLevel)
			bOnOff = !bOnOff;
	}
	else
	{
		bOnOff = IsLimit(nMotorId, CCW);
		if (!m_pParamMotor[nMotorId].bNegLimitLevel)
			bOnOff = !bOnOff;
	}

	return bOnOff;
#endif
	return TRUE;
}

BOOL CMotionNew::CheckSWLimitFault(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (GetState(nMotorId) == MPIStateERROR)
	{
		if (IsLimit(nMotorId, CCW) || IsLimit(nMotorId, CW))
		{
			CString strMsg;
			strMsg.Format(_T("%s[%d] Axis: Software limit fault, Auto recovery? "), GetMotorName(nMotorId), nMotorId);
			if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION | MB_YESNO))
			{
				Abort(nMotorId);
				ClearMotionFault(nMotorId);
				ServoOnOff(nMotorId, ON);
			}
		}
		return TRUE;
	}
#endif
	return FALSE;
}

BOOL CMotionNew::IsLimit(int nMotionId, int nDir)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return (m_pMotionCard->IsLimit(nMotionId, nDir));
	if (PLUS == nDir)
		return m_pMotionCard->GetAxis(nMotionId)->CheckPosLimitSwitch();
	else
		return m_pMotionCard->GetAxis(nMotionId)->CheckNegLimitSwitch();
#endif
	return FALSE;
}

long CMotionNew::GetState(int nMotionId)
{
	if (nMotionId >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return MPIStateERROR;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return (m_pMotionCard->GetState(nMotionId));
	return GetAxisState(nMotionId);
#endif
	return 0;
}

void CMotionNew::ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue)
{
	if (nMotorId >= MAX_AXIS)
		return;

	CString strData, strTemp;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;
	int nID = 0, nComma;

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);
		}
	}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	m_pParamMotor[nMotorId].fPosLimit = fErrorLimitValue;
	//m_pMotionCard->ChangePosSWLimitValue(nMotorId, fErrorLimitValue);
	INT nAct = m_pMotionCard->GetAxis(nMotorId)->GetPosLimitAction();
	m_fPosLimit[nMotorId] = fErrorLimitValue;
	m_pMotionCard->GetAxis(nMotorId)->SetPosSoftwareLimit(fErrorLimitValue, nAct);

#endif

	strData.Format(_T(""));
	strTemp.Format(_T(""));

	for (int i = 0; i < nID; i++)
	{
		strTemp.Format(_T("%.3f, "), m_pParamMotor[i].fPosLimit);
		strData.Insert(strData.GetLength(), strTemp);
		strTemp.Format(_T(""));
	}

	nComma = strData.ReverseFind(_T(','));
	strData.Delete(nComma, 2);
	strTemp.Format(_T("\t// Sotftware positive motion limit"));
	strData.Insert(strData.GetLength(), strTemp);

	WritePrivateProfileString(_T("AXIS PARAM"), _T("POSITIVE SOFTWARE LIMIT"), strData, MOTOR_PARAM_PATH);
}

void CMotionNew::ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue)
{
	if (nMotorId >= MAX_AXIS)
		return;

	CString strData, strTemp;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;
	int nID, nComma;

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), MOTOR_PARAM_PATH))
	{
		token = _tcstok(szData, sep);
		nID = 0;
		if (token != NULL)
			m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);

		while (token != NULL)
		{
			nID++;
			if (nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL, sep);
			if (token != NULL)
				m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);
		}
	}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	m_pParamMotor[nMotorId].fNegLimit = fErrorLimitValue;
	//	m_pMotionCard->ChangeNegSWLimitValue(nMotorId, fErrorLimitValue);
	INT nAct = m_pMotionCard->GetAxis(nMotorId)->GetNegLimitAction();
	m_fNegLimit[nMotorId] = fErrorLimitValue;
	m_pMotionCard->GetAxis(nMotorId)->SetNegSoftwareLimit(fErrorLimitValue, nAct);

#endif

	strData.Format(_T(""));
	strTemp.Format(_T(""));

	for (int i = 0; i < nID; i++)
	{
		strTemp.Format(_T("%.3f, "), m_pParamMotor[i].fNegLimit);
		strData.Insert(strData.GetLength(), strTemp);
		strTemp.Format(_T(""));
	}

	nComma = strData.ReverseFind(_T(','));
	strData.Delete(nComma, 2);
	strTemp.Format(_T("\t// Sotftware negative motion limit"));
	strData.Insert(strData.GetLength(), strTemp);

	WritePrivateProfileString(_T("AXIS PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), strData, MOTOR_PARAM_PATH);

}

BOOL CMotionNew::IsAmpFault(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 1;
	}

	//return (m_pMotionCard->IsAmpFault(nMotorId));
	return (m_pMotionCard->GetAxis(nMotorId)->CheckAmpFaultSwitch());
#endif
	return FALSE;
}


// [General Function]

BOOL CMotionNew::InitBoard()
{

	if (!m_pMotionCard)
	{
		CMotionNew::m_bMotionCreated = 0;

		//m_nBoardId = 0;
		//m_nDevIdIoIn = 1009;
		//m_nDevIdIoOut = 1010;

		m_pMotionCard = new CNmcDevice(this);

		if (!m_pMotionCard)
			return FALSE;

		((CMotion*)m_pParent)->m_pMotionCard = m_pMotionCard;
		((CMotion*)m_pParent)->m_bMotionCreated = 1;
		CMotionNew::m_bMotionCreated = 1;
	}

	if (m_pMotionCard)
	{
		CMotionNew::m_bMotionCreated = 0;
		m_pMotionCard->InitDevice(1); // 1 is Number Of NMMC Board.
		CMotionNew::m_bMotionCreated = 1;
	}
	else
		return FALSE;


	if(CreateObject())
		SetConfigure();

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSPRO_R2R_MDS || MACHINE_MODEL == INSMART_R2R_SUMITOMO || MACHINE_MODEL == INSPRO_R2R_EG
	long lOnOff, val;
	CString strMsg;
	if (!GetGantry(SCAN_AXIS, SCAN_S_AXIS, &lOnOff))
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Get. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (!lOnOff) // Gantry Mode
	{
		if (!SetGantry(SCAN_AXIS, SCAN_S_AXIS, TRUE))
		{
			strMsg.Format(_T("[MSG94] Failed Gantry Set. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}

	if (!GetGantry(SCAN_AXIS, SCAN_S_AXIS, &lOnOff))
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Get. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (!lOnOff) // Gantry Mode
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Set. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		AfxMessageBox(strMsg);
		return FALSE;
	}

#endif

#ifdef CLEAN_DOOR_AXIS
	SetCleanDoorFaultLimit();
#endif
	if (!InitAtf())
	{
		AfxMessageBox(_T("[MSG299] Failed-InitAtf()"));
		return FALSE;
	}

	return TRUE;
}

BOOL CMotionNew::CreateObject()
{
	for (int nAxis = 0; nAxis < m_nTotAxis; nAxis++)
	{
		if(m_pMotionCard)
			m_pMotionCard->CreateAxis(nAxis);
	}

	return TRUE;
}

BOOL CMotionNew::IsLoadingPosition()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI

	if (m_bMotionCreated == 0)
	{
		return 0;
	}

#ifdef LIGHT_AXIS
	if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(LIGHT_AXIS))
	{
#else
	if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS))
	{
#endif
		double dY = GetActualPosition(SCAN_AXIS);
		double dX = GetActualPosition(CAM_AXIS);
		double dYmax = pGlobalDoc->m_fLoadingPositionY + m_pParamAxis[SCAN_AXIS].fInpRange * 10;
		double dYmin = pGlobalDoc->m_fLoadingPositionY - m_pParamAxis[SCAN_AXIS].fInpRange * 10;
		double dXmax = pGlobalDoc->m_fLoadingPositionX + m_pParamAxis[CAM_AXIS].fInpRange * 10;
		double dXmin = pGlobalDoc->m_fLoadingPositionX - m_pParamAxis[CAM_AXIS].fInpRange * 10;
		if (dY > dYmin && dY < dYmax && dX > dXmin && dX < dXmax)
			return TRUE;
	}
#endif
	return FALSE;
}


int CMotionNew::GotoLoadingPosition(BOOL bWait, BOOL bCheckClamp)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strMsg;

	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}

	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		if (pGlobalDoc->m_bOperateInterlock)
			pGlobalDoc->m_bOperateInterlock = pIO->CheckOperateInterlock();

		pGlobalDoc->m_bContinuousDefViewStart = FALSE;
		pGlobalDoc->m_bContinuousAOMViewStart = FALSE;

		return SUCCESS;
	}

	if (AoiParam()->m_bUseTableClamp)
	{
		if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));

			return OPERATE_INTERLOCK;
		}
	}

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (g_bUseTableLock &&pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

#ifdef MOTION_ACTION
			StopMove(SCAN_AXIS, 0.01, 0);
#endif
			CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return OPERATE_INTERLOCK;
		}
	}

	if (AoiParam()->m_bUseTableClamp)
	{
		if (GetClampObject()->CheckSafetySensorBit())
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
			}
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("안전 센서 감지 상태!"));
			else
				SetMainMessage(_T("Safety sensor detection status!"));


			pGlobalDoc->m_bContinuousDefViewStart = FALSE;
			pGlobalDoc->m_bContinuousAOMViewStart = FALSE;
			return OPERATE_INTERLOCK;
		}


	}

	if (pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
	{
		strMsg.Format(_T("Glass cover is open."));
		pMainFrame->MessageBox(strMsg);
		return OPERATE_INTERLOCK;
	}

	if (!m_bOrigin[CAM_AXIS])
	{
		if (pGlobalDoc->m_bUseInlineAutomation == 0)
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			strMsg.Format(_T("Camera %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		else
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			CString strError;
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(CAM_AXIS));
			else
				strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(CAM_AXIS));

			SetMainMessage(strError);
		}
		return NOT_ORIGIN;
	}

	if (!m_bOrigin[SCAN_AXIS])
	{
		if (pGlobalDoc->m_bUseInlineAutomation == 0)
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		else
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			CString strError;
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(SCAN_AXIS));
			else
				strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(SCAN_AXIS));

			SetMainMessage(strError);
		}
		return NOT_ORIGIN;
	}

	MoveXY(pGlobalDoc->m_fLoadingPositionX, pGlobalDoc->m_fLoadingPositionY, pGlobalDoc->m_fLoadingSpeed, NO_WAIT, 0);

#if CUSTOMER_COMPANY != SUMITOMO	//120822 hjc add
	pGlobalView->TowerLampControl(TOWER_LAMP_YELLOW);
#endif
	if (bWait == WAIT)
	{
		if (WaitXYPosEnd(CHECK_MOTION_DONE) == TRUE)
		{
			if (AoiParam()->m_bUseTableClamp)
			{
				if (GetClampObject()->IsLoadingPosition() == CLAMP_OK)
				{
#if CUSTOMER_COMPANY == IBIDEN_GAMA || CUSTOMER_COMPANY == IBIDEN_JUNGANG || CUSTOMER_COMPANY == IBIDEN_OGAKI
					if (GetClampObject()->GetOutputBit(OUT_CLAMP_ON) == 0 && GetClampObject()->GetOutputBit(OUT_CLAMP_OFF) == 0)
					{
						int nOnCount = 0;
						int nOffCount = 0;

						if (GetClampObject()->GetInputBit(IN_CLAMP_GA11_ON))
							nOnCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA12_ON))
							nOnCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA13_ON))
							nOnCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA14_ON))
							nOnCount++;


						if (GetClampObject()->GetInputBit(IN_CLAMP_GA11_OFF))
							nOffCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA12_OFF))
							nOffCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA13_OFF))
							nOffCount++;
						if (GetClampObject()->GetInputBit(IN_CLAMP_GA14_OFF))
							nOffCount++;

						if (GetClampObject()->GetInputBit(IN_CLAMP_GA11_ON) && GetClampObject()->GetInputBit(IN_CLAMP_GA12_ON)
							&& GetClampObject()->GetInputBit(IN_CLAMP_GA13_ON) && GetClampObject()->GetInputBit(IN_CLAMP_GA14_ON) || (nOnCount > 0 && nOnCount >= nOffCount))
						{
							GetClampObject()->SetOutputBit(OUT_CLAMP_ON, 1);
						}
						else if (GetClampObject()->GetInputBit(IN_CLAMP_GA11_OFF) && GetClampObject()->GetInputBit(IN_CLAMP_GA12_OFF)
							&& GetClampObject()->GetInputBit(IN_CLAMP_GA13_OFF) && GetClampObject()->GetInputBit(IN_CLAMP_GA14_OFF) || (nOffCount > 0 && nOffCount >= nOnCount))
						{
							GetClampObject()->SetOutputBit(OUT_CLAMP_OFF, 1);
						}
						else
						{
							return OPERATE_INTERLOCK;
						}
					}
#endif
				}
			}

		}
	}
#endif		//20120306-ndy moved for RI

	pGlobalDoc->m_bContinuousDefViewStart = FALSE;
	pGlobalDoc->m_bContinuousAOMViewStart = FALSE;

	return SUCCESS;
}

int CMotionNew::MoveMotorSafety(double fTgtPos, double dVelRatio, int nMsID, BOOL bWait)
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}
#endif

	double dVel = pMotionNew->m_pParamMotion[nMsID].Speed.fSpd * dVelRatio / 100.0;
	double dAcc = pMotionNew->m_pParamMotion[nMsID].Speed.fAcc * dVelRatio / 100.0;//*9.8*1000;
	double dDec = pMotionNew->m_pParamMotion[nMsID].Speed.fDec * dVelRatio / 100.0;//*9.8*1000;

	return StartPosMove(nMsID, fTgtPos, dVel, dAcc, dDec, ABS, bWait, S_CURVE, TRUE);
}

BOOL CMotionNew::MoveScanMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strMsg;

	if (pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
	{
		strMsg.Format(_T("Glass cover is open."));
		pMainFrame->MessageBox(strMsg);
		return FALSE;
	}

	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}

	if (AoiParam()->m_bUseTableClamp)
	{
		if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));

			return FALSE;
		}
	}

	if (!m_bOrigin[SCAN_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		//		CString strMsg;
		strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);


		return FALSE;
	}
	fTgtPos = min(max(fTgtPos, m_pParamMotor[SCAN_AXIS].fNegLimit), m_pParamMotor[SCAN_AXIS].fPosLimit);

	short ret;
#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);
#endif
	if (fTgtPos != m_pParamMotor[SCAN_AXIS].fNegLimit && fTgtPos != m_pParamMotor[SCAN_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(SCAN_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (SCAN_AXIS)"));
#endif
	return TRUE;
}

double CMotionNew::GetSpeedProfile(int nMode, int nAxisID, double fMovingLength, double &fVel, double &fAcc, double &fJerk, int nSpeedType)
{

	double dMotionTime = 0.0, dAccTime = 0.0, dVelocityTime = 0.0, dSettleTime = 0.0, dTotalTime = 0.0;

	double dAccTimeToMaxSpeed = 0.0;
	double dMaxVelocity = 0.0, dMaxAccel = 0.0, dMaxJerk = 0.0;
	double dVelocity = 0.0, dAccel = 0.0, dAccPeriod = 0.0, dAccLength = 0.0;

	//dMaxVelocity = m_fMaxVel[nAxisID];
	dMaxVelocity = m_pMotionCard->m_fMaxVel[nAxisID];
	dMaxAccel = m_fMaxAccel[nAxisID];

	dVelocity = m_fVel[nAxisID];
	dAccel = m_fAcc[nAxisID];

	dMaxJerk = dMaxAccel / m_fMinJerkTime[nAxisID];
	dAccPeriod = m_fAccPeriod[nAxisID];

	// 속도, 가속도 값이 최대 가속도이내로 설정되도록 한다.
	if (dVelocity > dMaxVelocity || dVelocity < 0.0)
		dVelocity = dMaxVelocity;

	if (dAccel > dMaxAccel || dAccel < 0.0)
		dAccel = dMaxAccel;

	if (nSpeedType == LOW_SPEED)
	{
		dVelocity /= 4.;
		dAccel /= 4.;
		dMaxJerk /= 4.;
	}
	if (nSpeedType == MIDDLE_SPEED)
	{
		dVelocity /= 2.;
		dAccel /= 2.;
		dMaxJerk /= 2.;
	}
	if (nSpeedType == HIGH_SPEED)
	{
		dVelocity /= 1.;
		dAccel /= 1.;
		dMaxJerk /= 1.;
	}

	if (nMode == S_CURVE)
	{
		fVel = dVelocity;
		fAcc = dAccel;
		fJerk = dMaxJerk;
		if (GetSCurveVelocity(fMovingLength, fVel, fAcc, dMaxJerk) == FALSE)
		{
			fVel = dVelocity;
			fAcc = dAccel;
			fJerk = dMaxJerk;//dAcc * 10.0;//
		}
		dMotionTime = GetMotionTime(fMovingLength, fVel, fAcc, fJerk);
	}
	else if (nMode == TRAPEZOIDAL)
	{
		// 가속 구간 값이 50%를 초과할경우 50%로 규제한다.
		if (dAccPeriod > 50.0)
			dAccPeriod = 50.0;

		if (dAccPeriod < 50.0) // trapezoidal profile
		{
			// 전체 이동 거리중 가속구간의 거리 및 소요 시간 계산,
			dAccLength = fMovingLength * dAccPeriod * 2.0 / 100.0;
		}
		else // triangular profile
		{
			// 전체 이동 거리의 1/2구간의 거리 및 소요 시간 계산,
			dAccLength = fMovingLength / 2.0;
		}
		dAccTime = sqrt(2.0 * dAccLength / dAccel);

		fAcc = dAccel;

		if (dVelocity < fAcc * dAccTime)
			fVel = dVelocity;
		else
			fVel = fAcc * dAccTime;

		// 정속 구간의 이동시간을 구한다.
		dVelocityTime = (fMovingLength - dAccLength) / fVel;
		dMotionTime = dAccTime * 2 + dVelocityTime;
	}

	dSettleTime = (double)(100.0 / 1000.0);
	dTotalTime = dMotionTime + dSettleTime;

	return dTotalTime;
	//return 0.0;
}

double CMotionNew::GetMotionTime(double dLen, double dVel, double dAcc, double dJerk)
{
	return dAcc / dJerk + dVel / dAcc + dLen / dVel;//[sec]
}

BOOL CMotionNew::GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk)
{
	do {
		if ((dVel / dAcc) < (dAcc / dJerk))
			dVel = dAcc * dAcc / dJerk;

		double fTemp = (dLen / dVel - (dVel / dAcc + dAcc / dJerk));
		if (fTemp > 0 || fabs(fTemp) < 0.000000001)
		{
			break;
		}
		else
		{
			double a = 1 / dAcc;
			double b = dAcc / dJerk;
			double c = -dLen;
			double r1 = (-b + sqrt(b*b - 4 * a * c)) / (2 * a);
			double r2 = (-b - sqrt(b*b - 4 * a * c)) / (2 * a);

			if (r1 > r2)
			{
				dVel = r1;
				if (dVel / dAcc < dAcc / dJerk)
					dAcc = sqrt(dJerk * dVel);
			}
			else
			{
				AfxMessageBox(_T("S-Curve 속도 프로파일 계산 에러 "));
				return FALSE;
			}
		}
	} while (1);
	return TRUE;
}

BOOL CMotionNew::IsListMotion()
{
	return m_pMotionCard->IsListMotion();
}

BOOL CMotionNew::IsInterpolationMotion()
{
	return m_pMotionCard->IsInterpolationMotion();
}

BOOL CMotionNew::IsMotionDoneGantrySlave()
{
	int error = 0;
	MC_STATUS ms = MC_OK;
	CString msg;
	char cstrErrorMsg[MAX_ERR_LEN];
	UINT32 Status = 0;

	ms = MC_GantryReadStatus(m_nBoardId, 0, &Status);
	if (ms != MC_OK)
	{
		MC_GetErrorMessage(ms, MAX_ERR_LEN, cstrErrorMsg);
		msg.Format(_T("Error - MC_GantryReadStatus :: 0x%08X, %s"), ms, cstrErrorMsg);
		AfxMessageBox(msg);

		return FALSE;
	}

	if ((Status & mcGantry_MotionCompleted) && (Status & mcGantry_YawStable))
		return TRUE;

	return FALSE;
}


BOOL CMotionNew::TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{

	while (!IsMotionDoneGantrySlave())
	{
		Sleep(100);
	}

	return TRUE;
}

BOOL CMotionNew::TwoStartPosMove(int nMsId0, int nMsId1, double fPos0, double fPos1, double fVelRatio, BOOL bAbs, BOOL bWait, BOOL bOptimize, int bMotionType)
{
	if (nMsId0 >= MAX_AXIS || nMsId1 >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION

	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}


	while (!IsMotionDoneGantrySlave())
	{
		Sleep(100);
	}

	if (m_pMotionCard->IsListMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);

	if (m_pMotionCard->IsInterpolationMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation);

#endif

	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return OPERATE_INTERLOCK;
	}

#ifdef MOTION_ACTION

	if (AoiParam()->m_bUseTableClamp)
	{
		if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));

			return OPERATE_INTERLOCK;
		}
	}
	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (g_bUseTableLock && (nMsId0 == SCAN_AXIS || nMsId1 == SCAN_AXIS) && pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

#ifdef MOTION_ACTION
			StopMove(SCAN_AXIS, 0.01, 0);
#endif
			CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return OPERATE_INTERLOCK;
		}
	}

	// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

	if (GetState(nMsId0) == MPIStateERROR)
	{
		Clear(nMsId0);
		Sleep(30);
	}

	if (GetState(nMsId1) == MPIStateERROR)
	{
		Clear(nMsId1);
		Sleep(30);
	}

	if (!IsServoOn(nMsId0))
	{
		ServoOnOff(nMsId0, TRUE);
		Sleep(30);
	}

	if (!IsServoOn(nMsId1))
	{
		ServoOnOff(nMsId1, TRUE);
		Sleep(30);
	}

	if (GetAxisState(nMsId0) == MPIStateERROR)
	{
		Clear(nMsId0);
		Sleep(30);
	}

	if (GetAxisState(nMsId1) == MPIStateERROR)
	{
		Clear(nMsId1);
		Sleep(30);
	}

	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount();

	CString strDispMsg;
	int nMoveDir = 2;
	double fCurPos = GetCommandPosition(nMsId0);

	if (bAbs)
	{
		if (fPos0 >= fCurPos)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}
	else
	{
		if (fPos0 >= 0.0)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}

	if (!IsReadyToMove(strDispMsg, nMsId0, nMoveDir))
	{
		if (IDYES == AfxMessageBox(strDispMsg, MB_YESNO | MB_SETFOREGROUND))
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
		}
		else
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
			pGlobalView->ExitProgram();
		}
		return MEI_MOTION_ERROR;
	}

	fCurPos = GetCommandPosition(nMsId1);

	if (bAbs)
	{
		if (fPos1 >= fCurPos)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}
	else
	{
		if (fPos1 >= 0.0)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}

	if (!IsReadyToMove(strDispMsg, nMsId1, nMoveDir))
	{
		if (IDYES == AfxMessageBox(strDispMsg, MB_YESNO | MB_SETFOREGROUND))
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
		}
		else
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
			pGlobalView->ExitProgram();
		}
		return MEI_MOTION_ERROR;
	}

	if (!m_pMotionCard->TwoStartPosMove(nMsId0, nMsId1, fPos0, fPos1, fVelRatio, bAbs, bWait))
		return MEI_MOTION_ERROR;

#endif

	return TRUE;
}
////
BOOL CMotionNew::StartPosMove(int nMsId, double fPos, double fVel, double fAcc, double fDec, BOOL bAbs, BOOL bWait, int bMotionType, BOOL bOptimize)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}
#endif


	if (m_pMotionCard->IsListMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);

	if (m_pMotionCard->IsInterpolationMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation);


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

	if (nMsId == SCAN_AXIS)
	{
		if (AoiParam()->m_bUseTableClamp)
		{
			if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
				else
					SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));

				return OPERATE_INTERLOCK;
			}

			if (GetClampObject()->ClampStatusError())
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

				if (pGlobalDoc->m_nSelectedLanguage == KOREA)
				{
					SetMainMessage(_T("클램프 초기 상태 오류입니다. 클램프의 열림/닫힘 상태를 확인하고 다시 시도하십시오"));

					if (g_ecsNetwork.m_bConnected)
					{
						g_ecsNetwork.SendTerminalMessage(_T("클램프 초기 상태 오류입니다. 클램프의 열림/닫힘 상태를 확인하고 다시 시도하십시오"));
					}

				}
				else
				{
					SetMainMessage(_T("Clamp initial state error. Check the clamp's open / closed status and try again"));

					if (g_ecsNetwork.m_bConnected)
					{
						g_ecsNetwork.SendTerminalMessage(_T("Clamp initial state error. Check the clamp's open / closed status and try again"));
					}
				}
				return OPERATE_INTERLOCK;
			}
		}

		if (pIO->CheckSaftyOnlyBit())
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("안전 센서 감지 상태!"));
			else
				SetMainMessage(_T("Safety sensor detection status!"));

			return OPERATE_INTERLOCK;
		}

		if (pIO->CheckIBIDoorStatusNoMessage() == 0)
		{
			SetMainMessage(_T("Door open detection"));
			return OPERATE_INTERLOCK;
		}
	}

	if (pGlobalDoc->m_bUseInlineAutomation)
	{

		if (g_bUseTableLock && nMsId == SCAN_AXIS && pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

#ifdef MOTION_ACTION
			StopMove(SCAN_AXIS, 0.01, 0);
#endif
			CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return OPERATE_INTERLOCK;
		}
	}


	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return OPERATE_INTERLOCK;
	}

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

	if (GetState(nMsId) == MPIStateERROR)
	{
		Clear(nMsId);
		Sleep(30);

		if (GetState(nMsId) == MPIStateERROR)
		{
			Clear(nMsId);
			Sleep(30);
		}
	}

	if (!IsServoOn(nMsId))
	{
		ServoOnOff(nMsId, TRUE);
		Sleep(30);
	}

	if (GetAxisState(nMsId) == MPIStateERROR)
	{
		Clear(nMsId);
		Sleep(30);
	}

	if (pMotionNew->IsEncoderFault(nMsId) || pMotionNew->IsAmpFault(nMsId))
	{
		return MEI_MOTION_ERROR;
	}


	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();

																					  // 20180525 - syd
	CString strDispMsg;
	int nMoveDir = 2;
	double fCurPos = GetCommandPosition(nMsId);

	if (bAbs)
	{
		if (fPos >= fCurPos)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}
	else
	{
		if (fPos >= 0.0)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}

	if (!IsReadyToMove(strDispMsg, nMsId, nMoveDir))
	{
		if (IDYES == AfxMessageBox(strDispMsg, MB_YESNO | MB_SETFOREGROUND))
		{
			pGlobalView->m_pIO->BuzzerOnOff(OFF);
		}
		else
		{
			pGlobalView->m_pIO->BuzzerOnOff(OFF);
			pGlobalView->ExitProgram();
		}
		return MEI_MOTION_ERROR;
	}

	//if(!m_pMotionCard->Move(nMsId, &fPos, fVel, fAcc, fDec, bAbs, bWait, bOptimize))
	double fTime = 0.0, fJerk = 0.0;
	if(bOptimize)
		fTime = GetSpeedProfile(S_CURVE, nMsId, fVel, fVel, fAcc, fJerk, MIDDLE_SPEED);
	if (!m_pMotionCard->GetAxis(nMsId)->StartSCurveMove(fPos, fVel, fAcc, fJerk, bAbs, bWait))
		return MEI_MOTION_ERROR;
#endif

	return SUCCESS;
}




BOOL CMotionNew::StartPosMove(int nMsId, double fPos, double fVelRatio, BOOL bAbs, BOOL bWait, BOOL bOptimize, int bMotionType)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI

	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}


	if (m_pMotionCard->IsListMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);
	if (m_pMotionCard->IsInterpolationMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation);


	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return OPERATE_INTERLOCK;
		}
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	// 	if(pGlobalDoc->m_bUseAutoFocusSol) //20150507 - syd
	// 	{
	// 		if(m_pParamAxis[nMsId].sName == "CAMERA")
	// 		{
	// 			if(pGlobalView->IsAutoFocusSolDn())
	// 			{
	// 				if(!pGlobalView->AutoFocusSolDn(FALSE))
	// 					return FALSE;
	// 			}
	// 		}
	// 	}
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return OPERATE_INTERLOCK;
	}

	if (AoiParam()->m_bUseTableClamp)
	{
		if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));

			return OPERATE_INTERLOCK;
		}
	}

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (g_bUseTableLock && nMsId == SCAN_AXIS && pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

#ifdef MOTION_ACTION
			StopMove(SCAN_AXIS, 0.01, 0);
#endif
			CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return OPERATE_INTERLOCK;
		}
	}

	// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
		return OPERATE_INTERLOCK;
#endif

	if (GetState(nMsId) == MPIStateERROR)
	{
		Clear(nMsId);
		Sleep(30);
	}

	if (!IsServoOn(nMsId))
	{
		ServoOnOff(nMsId, TRUE);
		Sleep(30);
	}

	if (GetAxisState(nMsId) == MPIStateERROR)
	{
		Clear(nMsId);
		Sleep(30);
	}

	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount();

	// 20180525 - syd
	CString strDispMsg;
	int nMoveDir = 2;
	double fCurPos = GetCommandPosition(nMsId);

	if (bAbs)
	{
		if (fPos >= fCurPos)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}
	else
	{
		if (fPos >= 0.0)
			nMoveDir = POSITIVE_DIR;
		else
			nMoveDir = NEGATIVE_DIR;
	}

	if (!IsReadyToMove(strDispMsg, nMsId, nMoveDir))
	{
		if (IDYES == AfxMessageBox(strDispMsg, MB_YESNO | MB_SETFOREGROUND))
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
		}
		else
		{
			if (pGlobalView->m_pIO)
				pGlobalView->m_pIO->BuzzerOnOff(OFF);
			pGlobalView->ExitProgram();
		}
		return MEI_MOTION_ERROR;
	}

	//if (!m_pMotionCard->Move(nMsId, &fPos, fVelRatio, bAbs, bWait))
	//double fTime = 0.0, fJerk = 0.0;
	//fTime = GetSpeedProfile(S_CURVE, nMsId, fVel, fVel, fAcc, fJerk, MIDDLE_SPEED);
	if (!m_pMotionCard->GetAxis(nMsId)->StartSCurveMove(fPos, fVelRatio, bAbs, bWait))
		return MEI_MOTION_ERROR;
#endif

	return SUCCESS;
}

BOOL CMotionNew::IsReadyToMove(CString& strDispMsg, int nMotor, int nDirection) // 20180517 - syd
{
	BOOL bServoOff = FALSE;
	BOOL bLimitOn = FALSE;
	CString sMotorName = _T("");
	CString sLimit = _T("");
	CString sMsg = _T("");

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	if (!pGlobalDoc->m_bMotionAlarm)	// 20180525 - syd
		return TRUE;

	for (int nAxis = 0; nAxis < MAX_AXIS; nAxis++)
	{
		// Are all motors servo on?
		if (!IsServoOn(nAxis))
		{
			bServoOff = TRUE;
			sMotorName += GetMotorName(nAxis);
			sMotorName += _T(", ");
		}

		// Are all limit sensor off?
		if (nMotor == nAxis)	// 20180525 - syd
		{
			if (nDirection == PLUS)
			{
				if (GetLimitInput(nAxis, PLUS))
				{
					bLimitOn = TRUE;
					sLimit += GetMotorName(nAxis);
					sLimit += _T("(+), ");
				}
			}

			if (nDirection == MINUS)
			{
				if (GetLimitInput(nAxis, MINUS))
				{
					bLimitOn = TRUE;
					sLimit += GetMotorName(nAxis);
					sLimit += _T("(-), ");
				}
			}
		}
		else
		{
			if (GetLimitInput(nAxis, PLUS))
			{
				bLimitOn = TRUE;
				sLimit += GetMotorName(nAxis);
				sLimit += _T("(+), ");
			}

			if (GetLimitInput(nAxis, MINUS))
			{
				bLimitOn = TRUE;
				sLimit += GetMotorName(nAxis);
				sLimit += _T("(-), ");
			}
		}
	}

	if (bServoOff)
	{
		sMotorName = sMotorName.Left(sMotorName.GetLength() - 2);
		strDispMsg.Format(_T("Servo OFF : %s\r\n"), sMotorName);
	}

	//180609 lgh mod
	bLimitOn = 0;
	if (bLimitOn)
	{
		sLimit = sLimit.Left(sLimit.GetLength() - 2);
		if (bServoOff)
		{
			sMsg.Format(_T("Limit Sensor ON : %s\r\n"), sLimit);
			strDispMsg += sMsg;
		}
		else
		{
			strDispMsg.Format(_T("Limit Sensor ON : %s\r\n"), sLimit);
		}
	}

	if (bServoOff || bLimitOn)
	{
		sMsg = _T("continue[Click yes]? \r\n if click the [Cancel or No] program will be destroy");
		strDispMsg += sMsg;
		pGlobalView->m_pIO->BuzzerOnOff(ON);
		return FALSE;
	}

	return TRUE;
}

double CMotionNew::GetPositionResolution(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return 0.0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0.0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->m_pMotor[nMotorId].m_fPosRes;
	return m_fPosRes[nMotorId];
#endif

	return 0.0;
}

void CMotionNew::AddList(double fScanMotPos, double dFocusMotPos, double dScanSpeed)
{
	m_pMotionCard->AddList(fScanMotPos, dFocusMotPos, dScanSpeed);
}


long CMotionNew::CreateAutoFocusingSeq(BOOL bFirst, double fScanMotPos, double dFocusOffset, int bDir, double fHeightOfLaserSensor, int nSwath, int nHeightIndex)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return SUCCESS;
	}
#endif
	
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//return m_pMotionCard->CreateAutoFocusingSeq(bFirst,fScanMotPos, dFocusOffset, bDir, fHeightOfLaserSensor, nSwath, nHeightIndex);
	double fSamplePosOfScanAxis = fScanMotPos + m_dOffsetInspCamLaser;
	double fTargetPosOfFocusAxis = dFocusOffset + m_dOrgFocusPos;

	m_pMotionCard->AddList(fSamplePosOfScanAxis, fTargetPosOfFocusAxis, m_dScanVel);
#endif

	return 0;
}

int CMotionNew::GetSeqIdx()
{
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	return m_pMotionCard->GetSeqIdx();
	//#endif

	return 0;
}

void CMotionNew::AddSeqIdx(int nAdd)
{
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return;
	//	}
	//#endif
	//
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	 m_pMotionCard->AddSeqIdx(nAdd);
	//#endif
}

void CMotionNew::ResetSeqIdx()
{
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return;
	//	}
	//#endif
	//
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	m_pMotionCard->ResetSeqIdx();
	//#endif
}

BOOL CMotionNew::MoveCleanerMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return FALSE;
	}
#endif

#ifdef AUTO_CLEANER_OPTION
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[CLEANER_AXIS])
	{
		TCHAR szData[200];
		CString strMsg;
		if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
			strMsg.Format(_T("Cleaner %s"), szData);
		else
			strMsg.Format(_T("[MSG300] Cleaner ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}
	fTgtPos = min(max(fTgtPos, m_pParamMotor[CLEANER_AXIS].fNegLimit), m_pParamMotor[CLEANER_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[CLEANER_AXIS].fNegLimit && fTgtPos != m_pParamMotor[CLEANER_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(CLEANER_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (CLEANER_AXIS)"));

#endif
#endif
	return TRUE;
}








BOOL CMotionNew::MoveCleanDoorMotorRatio(double fTgtPos, double fVelRatio, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef CLEAN_DOOR_AXIS
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[CLEAN_DOOR_AXIS])
	{
		TCHAR szData[200];
		CString strMsg;
		if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
			strMsg.Format(_T("Cleaner %s"), szData);
		else
			strMsg.Format(_T("[MSG300] Cleaner ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}
	fTgtPos = min(max(fTgtPos, m_pParamMotor[CLEAN_DOOR_AXIS].fNegLimit + 0.1), m_pParamMotor[CLEAN_DOOR_AXIS].fPosLimit - 0.1);
	short ret;
	if (fTgtPos != m_pParamMotor[CLEAN_DOOR_AXIS].fNegLimit && fTgtPos != m_pParamMotor[CLEAN_DOOR_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(CLEAN_DOOR_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (CLEAN_DOOR_AXIS)"));

#endif
#endif
	return TRUE;
}

BOOL CMotionNew::MoveHeightMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef _DEBUG
	long lTemppCnt = GetTickCount();
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}

#ifdef HEIGHT_AXIS

	if (!m_bOrigin[HEIGHT_AXIS])
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Height %s"), szData);
			else
				strMsg.Format(_T("Height ORIGIN_RETURN_FAIL"));


			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Height %s"), szData);
			else
				strMsg.Format(_T("[MSG302] Height ORIGIN_RETURN_FAIL"));
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}
	short ret;

#ifdef USE_ZMP
	if (GetState(HEIGHT_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(HEIGHT_AXIS) == 0.0)
		{
			Stop(HEIGHT_AXIS, 0.1);
			Clear(HEIGHT_AXIS);
			double dActPos = GetActualPosition(HEIGHT_AXIS);
			double dCmdPos = GetCommandPosition(HEIGHT_AXIS);

			long nActPulse = LengthToPulse(HEIGHT_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(HEIGHT_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Clear(HEIGHT_AXIS);
				pMotionNew->SetCommandPosition(HEIGHT_AXIS, dActPos);
				pMotionNew->SetActualPosition(HEIGHT_AXIS, dActPos);
				Sleep(100);
				Clear(HEIGHT_AXIS);
			}
		}
	}
#endif

	fTgtPos = min(max(fTgtPos, m_pParamMotor[HEIGHT_AXIS].fNegLimit), m_pParamMotor[HEIGHT_AXIS].fPosLimit);
	if (fTgtPos != m_pParamMotor[HEIGHT_AXIS].fNegLimit && fTgtPos != m_pParamMotor[HEIGHT_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(HEIGHT_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (HEIGHT_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (HEIGHT_AXIS)"));

		}

	}

#endif //#ifdef MOTION_ACTION
#endif //#ifdef AUTO_FOCUS_OPTION

	return TRUE;
}


int CMotionNew::In32(long *value)
{
	return m_pMotionCard->In32(value);
}

int CMotionNew::Out32(long value)
{
	return m_pMotionCard->Out32(value);
}


void CMotionNew::SetCleanDoorFaultLimit()
{
#ifdef CLEAN_DOOR_AXIS
	//#ifdef USE_ZMP
	//	MPI_RESULT returnValue;
	////	return;
	//	//190606 lgh add for IBI H/W Interlcokj
	//	/* Delete user limit object */
	//	if (m_UserLimitDoor != nullptr)
	//	{
	//		returnValue =
	//			mpiUserLimitDelete(m_UserLimitDoor);
	//	//	//msgCHECK(returnValue);
	//
	//		m_UserLimitDoor = NULL;
	//	}
	//
	//	returnValue =
	//		mpiUserLimitCreate(&m_UserLimitDoor,
	//			m_pMotionCard->m_hControl,
	//			0);
	//
	//
	//	MPIUserLimitConfig  config;
	//
	//	returnValue =
	//		mpiUserLimitConfigDefault(&config);
	//
	//	config.trigger.type = MPIUserLimitTriggerTypeSINGLE_CONDITION;
	//	config.trigger.duration = 0;
	//
	//	config.trigger.condition[0].type = MPIUserLimitConditionTypeMOTOR_DEDICATED_IN;
	//	config.trigger.condition[0].data.motorDedicatedIn.activeHigh = 1;
	//	config.trigger.condition[0].data.motorDedicatedIn.bit = MPIMotorDedicatedInHOME;
	//	config.trigger.condition[0].data.motorDedicatedIn.motorNumber = CLEAN_DOOR_AXIS;
	//	
	//	/*
	//	config.trigger.condition[1].type = MPIUserLimitConditionTypeMOTION_STATUS;
	//	config.trigger.condition[1].data.axisStatus.axisNumber = 8;
	//	config.trigger.condition[1].data.axisStatus.status = MPIStateMOVING;
	//	config.trigger.condition[1].data.axisStatus.isEqual = 1;
	//	*/
	//	
	//	config.action = MPIActionE_STOP_ABORT;
	//	config.actionAxis = CLEAN_DOOR_AXIS;
	//	config.trigger.disableCondition.type=MPIUserLimitConditionTypeNONE;
	//	config.enabled = 1;
	//
	//	returnValue =mpiUserLimitConfigSet(m_UserLimitDoor, &config);
	//
	//	returnValue=mpiUserLimitValidate(m_UserLimitDoor);
	//
	//	int32_t enable;
	//	mpiUserLimitEnableGet(m_UserLimitDoor, &enable);
	//
	//	mpiUserLimitEnableSet(m_UserLimitDoor, 1);
	//
	//#endif

	long InputData;
	int port = 1000;
	int nReturn = In32(port, &InputData);
#endif
}


BOOL CMotionNew::MoveCleanDoorMotor(double fTgtPos, double dSpd, double dAcc, double dDec, BOOL bWait)
{
#ifdef CLEAN_DOOR_AXIS

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[CLEAN_DOOR_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		CString strMsg;
		strMsg.Format(_T("Chuck %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		//	AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}

	fTgtPos = min(max(fTgtPos, m_pParamMotor[CLEAN_DOOR_AXIS].fNegLimit + 0.1), m_pParamMotor[CLEAN_DOOR_AXIS].fPosLimit - 0.1);
	short ret;
	if (fTgtPos != m_pParamMotor[CLEAN_DOOR_AXIS].fNegLimit && fTgtPos != m_pParamMotor[CLEAN_DOOR_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(CLEAN_DOOR_AXIS, fTgtPos, dSpd, dAcc, dDec, ABS, bWait, TRAPEZOIDAL, NO_OPTIMIZED);
	else
		return FALSE;
#endif
#endif
	return TRUE;
}


BOOL CMotionNew::MoveChuckMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{
#ifdef CHUCK_AXIS

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[CHUCK_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		CString strMsg;
		strMsg.Format(_T("Chuck %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}

	fTgtPos = min(max(fTgtPos, m_pParamMotor[CHUCK_AXIS].fNegLimit), m_pParamMotor[CHUCK_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[CHUCK_AXIS].fNegLimit && fTgtPos != m_pParamMotor[CHUCK_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(CHUCK_AXIS, fTgtPos, fVelRatio, ABS, bWait, NO_OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (CHUCK_AXIS)"));
#endif
#endif
	return TRUE;
}

BOOL CMotionNew::MoveVerifyMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef _DEBUG
	long lTemppCnt = GetTickCount();
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}

	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
#ifdef VERIFY_AXIS
	if (!m_bOrigin[VERIFY_AXIS])
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Verify %s"), szData);
			else
				strMsg.Format(_T("Verify ORIGIN_RETURN_FAIL"));


			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Verify %s"), szData);
			else
				strMsg.Format(_T("[MSG303] Verify ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}

#ifdef USE_ZMP
	if (GetState(VERIFY_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(VERIFY_AXIS) == 0.0)
		{

			double dActPos = GetActualPosition(VERIFY_AXIS);
			double dCmdPos = GetCommandPosition(VERIFY_AXIS);

			long nActPulse = LengthToPulse(VERIFY_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(VERIFY_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(VERIFY_AXIS, 0.1);
				Clear(VERIFY_AXIS);
				pMotionNew->SetCommandPosition(VERIFY_AXIS, dActPos);
				pMotionNew->SetActualPosition(VERIFY_AXIS, dActPos);
				Sleep(100);
				Clear(VERIFY_AXIS);
			}
		}
	}
#endif

	fTgtPos = min(max(fTgtPos, m_pParamMotor[VERIFY_AXIS].fNegLimit + 0.01), m_pParamMotor[VERIFY_AXIS].fPosLimit - 0.01);
	short ret;
	if (fTgtPos != m_pParamMotor[VERIFY_AXIS].fNegLimit && fTgtPos != m_pParamMotor[VERIFY_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(VERIFY_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (VERIFY_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (VERIFY_AXIS)"));

		}
	}
#endif
#endif

	return TRUE;
}


BOOL CMotionNew::MoveThetaMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return FALSE;
	}
#endif

#ifdef THETA_AXIS

	if (pGlobalDoc->m_bSaftyAreaSensorStatus)
	{
		if (!pIO->CheckSaftyOnlyBit())
		{
			pGlobalDoc->m_bSaftyAreaSensorStatus = 0;
			pGlobalView->m_bSwSafetyArea = 0;
			pGlobalView->m_bCheckSafetyOnlyBit = 0;
		}
	}

	if (pGlobalDoc->m_bOperateInterlock)
	{
		pGlobalDoc->m_bOperateInterlock = pIO->CheckOperateInterlock();
	}

	if (pGlobalDoc->m_bEmergencyStatus)
	{
		pGlobalDoc->m_bEmergencyStatus = pIO->CheckEmgSwitch();
	}

	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
#ifdef MOTION_ACTION	//20120306-ndy modified for RI


	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}

	if (!m_bOrigin[THETA_AXIS])
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Theta %s"), szData);
			else
				strMsg.Format(_T("Theta ORIGIN_RETURN_FAIL"));

			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			CString strMsg;
			strMsg.Format(_T("Theta %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}


#ifdef USE_ZMP
	if (GetState(THETA_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(THETA_AXIS) == 0.0)
		{

			double dActPos = GetActualPosition(THETA_AXIS);
			double dCmdPos = GetCommandPosition(THETA_AXIS);

			long nActPulse = LengthToPulse(THETA_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(THETA_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(THETA_AXIS, 0.1);
				Clear(THETA_AXIS);
				SetCommandPosition(THETA_AXIS, dActPos);
				SetActualPosition(THETA_AXIS, dActPos);
				Sleep(100);
				Clear(THETA_AXIS);
			}
		}
	}
#endif

	if (!bAbs)
		fTgtPos += GetActualPosition(THETA_AXIS);

	fTgtPos = min(max(fTgtPos, m_pParamMotor[THETA_AXIS].fNegLimit + 0.01), m_pParamMotor[THETA_AXIS].fPosLimit - 0.01);
	short ret;
	if (fTgtPos != m_pParamMotor[THETA_AXIS].fNegLimit && fTgtPos != m_pParamMotor[THETA_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(THETA_AXIS, fTgtPos, 50.0, ABS, bWait, OPTIMIZED);//161004lgh fix no_optimized->optimized
	else
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (MAG_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (MAG_AXIS)"));

		}

	}
#endif
#endif

#ifdef SCAN_S_AXIS
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!bAbs)
		fTgtPos += GetActualPosition(SCAN_S_AXIS);

	fTgtPos = min(max(fTgtPos, m_pParamMotor[SCAN_S_AXIS].fNegLimit), m_pParamMotor[SCAN_S_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[SCAN_S_AXIS].fNegLimit && fTgtPos != m_pParamMotor[SCAN_S_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(SCAN_S_AXIS, fTgtPos, 50.0, ABS, bWait, OPTIMIZED);//161004lgh fix no_optimized->optimized
	else
	{
		ret = StartPosMove(SCAN_S_AXIS, 0.5*(m_pParamMotor[SCAN_S_AXIS].fNegLimit+m_pParamMotor[SCAN_S_AXIS].fPosLimit), 50.0, ABS, bWait, OPTIMIZED);//161004lgh fix no_optimized->optimized
		if(SUCCESS != ret)
			AfxMessageBox(_T("Can't move position to limit. (SCAN_S_AXIS)"));
	}
#endif
#endif

	return TRUE;
}
BOOL CMotionNew::MoveMagMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	// 120113 jsy NO_MAG_AXIS
#ifdef NO_MAG_AXIS
	return TRUE;
#endif

#ifdef MAG_AXIS
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[MAG_AXIS])
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Lens %s"), szData);
			else
				strMsg.Format(_T("Lens ORIGIN_RETURN_FAIL"));

			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			CString strMsg;
			strMsg.Format(_T("Lens %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}


#ifdef USE_ZMP
	if (GetState(MAG_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(MAG_AXIS) == 0.0)
		{

			double dActPos = GetActualPosition(MAG_AXIS);
			double dCmdPos = GetCommandPosition(MAG_AXIS);

			long nActPulse = LengthToPulse(MAG_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(MAG_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(MAG_AXIS, 0.1);
				Clear(MAG_AXIS);

				pMotionNew->SetCommandPosition(MAG_AXIS, dActPos);
				pMotionNew->SetActualPosition(MAG_AXIS, dActPos);
				Sleep(100);
				Clear(MAG_AXIS);
			}

		}
	}
	else
	{

	}
#endif

	if (!bAbs)
		fTgtPos += GetActualPosition(MAG_AXIS);

	// 20100420-syd
	double dCurrFocusPos = GetCommandPosition(FOCUS_AXIS);
	double dNegPosibleLimitValue = m_pParamMotor[MAG_AXIS].fNegLimit + (m_pParamMotor[FOCUS_AXIS].fNegLimit - dCurrFocusPos);
	double dPosPosibleLimitValue = m_pParamMotor[MAG_AXIS].fPosLimit;

	fTgtPos = min(max(fTgtPos, dNegPosibleLimitValue), dPosPosibleLimitValue);

	ChangeMagMotorNegSWLimit(dCurrFocusPos);

	short ret;
	if (fTgtPos != m_pParamMotor[MAG_AXIS].fNegLimit && fTgtPos != m_pParamMotor[MAG_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(MAG_AXIS, fTgtPos, 50.0, ABS, bWait, NO_OPTIMIZED);
	else
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (MAG_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (MAG_AXIS)"));

		}
	}
#endif
#endif

	return TRUE;
}

BOOL CMotionNew::MoveZoomMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
#ifdef ZOOM_AXIS
	if (!m_bOrigin[ZOOM_AXIS])
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Zoom %s"), szData);
			else
				strMsg.Format(_T("Zoom ORIGIN_RETURN_FAIL"));

			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			CString strMsg;
			strMsg.Format(_T("Zoom %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}

#ifdef USE_ZMP
	if (GetState(ZOOM_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(ZOOM_AXIS) == 0.0)
		{

			double dActPos = GetActualPosition(ZOOM_AXIS);
			double dCmdPos = GetCommandPosition(ZOOM_AXIS);

			long nActPulse = LengthToPulse(ZOOM_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(ZOOM_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(ZOOM_AXIS, 0.1);
				Clear(ZOOM_AXIS);

				pMotionNew->SetCommandPosition(ZOOM_AXIS, dActPos);
				pMotionNew->SetActualPosition(ZOOM_AXIS, dActPos);
				Sleep(100);
				Clear(ZOOM_AXIS);
			}

		}
	}
	else
	{

	}
#endif


	if (!bAbs)
		fTgtPos += GetActualPosition(ZOOM_AXIS);
	fTgtPos = min(max(fTgtPos, m_pParamMotor[ZOOM_AXIS].fNegLimit + 0.1), m_pParamMotor[ZOOM_AXIS].fPosLimit - 0.1);
	short ret;
	if (fTgtPos >= m_pParamMotor[ZOOM_AXIS].fNegLimit && fTgtPos <= m_pParamMotor[ZOOM_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(ZOOM_AXIS, fTgtPos, 100.0, ABS, bWait, NO_OPTIMIZED);
	else
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (ZOOM_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (ZOOM_AXIS)"));

		}

	}
#endif
#endif
	return TRUE;
}

BOOL CMotionNew::MoveInspZoomMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}
#ifdef INSP_ZOOM_AXIS
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[INSP_ZOOM_AXIS])
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Lens %s"), szData);
			else
				strMsg.Format(_T("Lens ORIGIN_RETURN_FAIL"));

			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			CString strMsg;
			strMsg.Format(_T("Lens %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}


#ifdef USE_ZMP
	if (GetState(INSP_ZOOM_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(INSP_ZOOM_AXIS) == 0.0)
		{
			double dActPos = GetActualPosition(INSP_ZOOM_AXIS);
			double dCmdPos = GetCommandPosition(INSP_ZOOM_AXIS);

			long nActPulse = LengthToPulse(INSP_ZOOM_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(INSP_ZOOM_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(INSP_ZOOM_AXIS, 0.1);
				Clear(INSP_ZOOM_AXIS);

				pMotionNew->SetCommandPosition(INSP_ZOOM_AXIS, dActPos);
				pMotionNew->SetActualPosition(INSP_ZOOM_AXIS, dActPos);
				Sleep(100);
				Clear(INSP_ZOOM_AXIS);
			}

		}
	}
	else
	{

	}
#endif


	if (!bAbs)
		fTgtPos += GetActualPosition(INSP_ZOOM_AXIS);

	fTgtPos = min(max(fTgtPos, m_pParamMotor[INSP_ZOOM_AXIS].fNegLimit + 0.005), m_pParamMotor[INSP_ZOOM_AXIS].fPosLimit - 0.005);
	short ret;
	if (fTgtPos != m_pParamMotor[INSP_ZOOM_AXIS].fNegLimit && fTgtPos != m_pParamMotor[INSP_ZOOM_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(INSP_ZOOM_AXIS, fTgtPos, 50.0, ABS, bWait, NO_OPTIMIZED);
	else
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (INSP_ZOOM_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (INSP_ZOOM_AXIS)"));

		}

	}
#endif
#endif

	return TRUE;
}









BOOL CMotionNew::MoveFocusMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit || pGlobalView->m_bSafetySensorBuzzerRestored)
	{
		return FALSE;
	}
#ifdef USE_ATF_MODULE
	if (pGlobalView->m_bATFConnect)
	{
		double fCurrPos, fPosDiff;
		int nMicroStep;
		int err;

		fCurrPos = GetActualPosition(FOCUS_AXIS);
		fPosDiff = fTgtPos - fCurrPos;

		nMicroStep = (int)(fPosDiff * 1000.0) * m_nAtfMicroStepNum * m_nAtfFullStepPerMM / 1000;
		err = atf_MoveZ(nMicroStep);

		if (err == ErrOK)
		{
			if (bWait)
			{
				while (!pMotionNew->CheckMotionDone(FOCUS_AXIS))
					Sleep(1);
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("[MSG294] ATF Connect Error!"));
		return FALSE;
	}
	return TRUE;
#endif
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit || pGlobalView->m_bSafetySensorBuzzerRestored)
	{
		return FALSE;
	}
	if (!m_bOrigin[FOCUS_AXIS])
	{
		CString strMsg;
		strMsg.Format(_T("Focus %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);

			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}

		return FALSE;
	}

#ifdef USE_ZMP
	if (GetState(FOCUS_AXIS) == MPIStateMOVING)
	{
		if (GetVelocity(FOCUS_AXIS) == 0.0)
		{
			double dActPos = GetActualPosition(FOCUS_AXIS);
			double dCmdPos = GetCommandPosition(FOCUS_AXIS);

			long nActPulse = LengthToPulse(FOCUS_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(FOCUS_AXIS, dCmdPos);

			if (fabs(nActPulse - nCmdPulse) > 200)
			{
				Stop(FOCUS_AXIS, 0.1);
				Clear(FOCUS_AXIS);

				pMotionNew->SetCommandPosition(FOCUS_AXIS, dActPos);
				pMotionNew->SetActualPosition(FOCUS_AXIS, dActPos);
				Sleep(100);
				Clear(FOCUS_AXIS);
			}
		}
	}
	else if (GetState(FOCUS_AXIS) == MPIStateIDLE)
	{
		double dActPos = GetActualPosition(FOCUS_AXIS);
		double dCmdPos = GetCommandPosition(FOCUS_AXIS);

		long nActPulse = LengthToPulse(FOCUS_AXIS, dActPos);
		long nCmdPulse = LengthToPulse(FOCUS_AXIS, dCmdPos);
		long nInsp = LengthToPulse(FOCUS_AXIS, m_pParamAxis[FOCUS_AXIS].fInpRange);

		if (fabs(nActPulse - nCmdPulse) > nInsp)
		{
			Clear(FOCUS_AXIS);
			pMotionNew->SetCommandPosition(FOCUS_AXIS, dActPos);
			pMotionNew->SetActualPosition(FOCUS_AXIS, dActPos);
			Sleep(30);
			Clear(FOCUS_AXIS);
		}
	}
#endif

	if (!bAbs)
		fTgtPos += GetActualPosition(FOCUS_AXIS);
	fTgtPos = min(max(fTgtPos, m_pParamMotor[FOCUS_AXIS].fNegLimit + 0.01), m_pParamMotor[FOCUS_AXIS].fPosLimit - 0.01);
	short ret;
	if (fTgtPos != m_pParamMotor[FOCUS_AXIS].fNegLimit && fTgtPos != m_pParamMotor[FOCUS_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(FOCUS_AXIS, fTgtPos, 100.0, ABS, bWait, NO_OPTIMIZED);
	else
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (FOCUS_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (FOCUS_AXIS)"));
		}
	}
#endif
	return TRUE;
}

BOOL CMotionNew::MoveFocus2Motor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef FOCUS2_AXIS

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[FOCUS2_AXIS])
	{
		CString strMsg;
		strMsg.Format(_T("Focus2 %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}

	if (!bAbs)
		fTgtPos += GetActualPosition(FOCUS2_AXIS);
	fTgtPos = min(max(fTgtPos, m_pParamMotor[FOCUS2_AXIS].fNegLimit), m_pParamMotor[FOCUS2_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[FOCUS2_AXIS].fNegLimit && fTgtPos != m_pParamMotor[FOCUS2_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(FOCUS2_AXIS, fTgtPos, 100.0, ABS, bWait, NO_OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (FOCUS2_AXIS)"));

#endif
#endif
	return TRUE;
}

BOOL CMotionNew::MoveLightMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef LIGHT_AXIS
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (!m_bOrigin[LIGHT_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		CString strMsg;
		strMsg.Format(_T("Light %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}
	if (!bAbs)
		fTgtPos += GetActualPosition(LIGHT_AXIS);

	fTgtPos = min(max(fTgtPos, m_pParamMotor[LIGHT_AXIS].fNegLimit), m_pParamMotor[LIGHT_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[LIGHT_AXIS].fNegLimit && fTgtPos != m_pParamMotor[LIGHT_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(LIGHT_AXIS, fTgtPos, 50.0, ABS, bWait, NO_OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (LIGHT_AXIS)"));
#endif
#endif
	return TRUE;
}

BOOL CMotionNew::MoveCamMotor(double fTgtPos, double fVelRatio, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return FALSE;
		}
	}

	if (m_pMotionCard->m_nInterlockStatus || AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return FALSE;
	}
	if (!m_bOrigin[CAM_AXIS])
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			TCHAR szData[200];
			CString strMsg;
			if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
				strMsg.Format(_T("Camera %s"), szData);
			else
				strMsg.Format(_T("Camera ORIGIN_RETURN_FAIL"));

			CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			SetMainMessage(strMsg);
		}
		else
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			CString strMsg;
			strMsg.Format(_T("Camera %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		return FALSE;
	}
	fTgtPos = min(max(fTgtPos, m_pParamMotor[CAM_AXIS].fNegLimit), m_pParamMotor[CAM_AXIS].fPosLimit);
	short ret;
#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);
#endif
	if (fTgtPos != m_pParamMotor[CAM_AXIS].fNegLimit + 0.01 && fTgtPos != m_pParamMotor[CAM_AXIS].fPosLimit - 0.01) // 20160204-syd
		ret = StartPosMove(CAM_AXIS, fTgtPos, fVelRatio, ABS, bWait, OPTIMIZED);
	else
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			SetMainMessage(_T("Can't move position to limit. (CAM_AXIS)"));
		}
		else
		{
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(_T("Can't move position to limit. (CAM_AXIS)"));
		}
	}
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	double dTgtPos = fTgtPos + pGlobalDoc->m_fLightAxisOffset;
	if (dTgtPos < 0.0)
		dTgtPos = 0.0;
	if (StartPosMove(LIGHT_AXIS, dTgtPos, fVelRatio, ABS, bWait, OPTIMIZED) != SUCCESS)
		return FALSE;
#endif

#endif
	return TRUE;
}

BOOL CMotionNew::MoveScanPos(double fTgtPosX, double fTgtPosY, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	if (pGlobalDoc->m_bInhibitMotion)
		return FALSE;

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (pIO->CheckShinkoDoorOpen())
		{
			return OPERATE_INTERLOCK;
		}
	}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strMsg;
	if (m_pMotionCard->m_nInterlockStatus || AoiParam()->m_bEmgStatusRestored == 0 || pGlobalDoc->m_bEmergencyStatus || pGlobalDoc->m_bOperateInterlock || pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
	{
		return OPERATE_INTERLOCK;
	}

	if (pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
	{
		strMsg.Format(_T("Glass cover is open."));
		pMainFrame->MessageBox(strMsg);
		return OPERATE_INTERLOCK;
	}

	if (!m_bOrigin[CAM_AXIS])
	{
		if (!pGlobalDoc->m_bUseInlineAutomation)
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			strMsg.Format(_T("Camera %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		else
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			CString strError;

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(CAM_AXIS));
			else
				strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(CAM_AXIS));


			SetMainMessage(strError);
		}
		return NOT_ORIGIN;
	}

	if (!m_bOrigin[SCAN_AXIS])
	{
		if (!pGlobalDoc->m_bUseInlineAutomation)
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
			strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));

			if (!m_pMotionCard->m_nInterlockStatus)
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		}
		else
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
			CString strError;

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(SCAN_AXIS));
			else
				strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(SCAN_AXIS));


			SetMainMessage(strError);
		}
		return NOT_ORIGIN;
	}

	if (fTgtPosX < GetNegSWLimitValue(CAM_AXIS) || fTgtPosX > GetPosSWLimitValue(CAM_AXIS))
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			CString strError;


			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));
			else
				strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));


			SetMainMessage(strError);
		}

		return MOTION_LIMIT_OVER;
	}

	if (fTgtPosY < GetNegSWLimitValue(SCAN_AXIS) || fTgtPosY > GetPosSWLimitValue(SCAN_AXIS))
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			CString strError;
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));
			else
				strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));


			SetMainMessage(strError);
		}

		return MOTION_LIMIT_OVER;
	}

#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);
#endif

	int nRet = MoveXY(fTgtPosX, fTgtPosY, 100.0, NO_WAIT);
	/* 이미 MOVE XY에서 한다.
	#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	double dTgtPos = fTgtPosX+pGlobalDoc->m_fLightAxisOffset;
	if(dTgtPos < 0.0)
	dTgtPos = 0.0;
	if(StartPosMove(LIGHT_AXIS, dTgtPos, 50.0, ABS, NO_WAIT, OPTIMIZED) != SUCCESS)
	return FALSE;
	#endif
	*/
	if (bWait == WAIT)
		return WaitScanEnd();
	else
	{
		return nRet;
	}

#endif
	return SUCCESS;
}

int CMotionNew::MoveXY(double fTgtPosX, double fTgtPosY, BOOL bSpeed, BOOL bWait, BOOL bCheckClamp)
{
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
	#endif
	
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		CString strMsg;
		if(
			m_pMotionCard->m_nInterlockStatus ||
			AoiParam()->m_bEmgStatusRestored ==0 ||
			pGlobalDoc->m_bEmergencyStatus ||
			pGlobalDoc->m_bOperateInterlock ||
			pGlobalDoc->m_bSaftyAreaSensorStatus ||
			pGlobalView->m_bSwSafetyArea ||
			pGlobalView->m_bCheckSafetyOnlyBit
			)
		{
			return OPERATE_INTERLOCK;
		}
	
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pIO->CheckShinkoDoorOpen())
			{
				return OPERATE_INTERLOCK;
			}
		}
	
	
	#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
		if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		{
			pGlobalView->ClearDispMessage();
			pGlobalView->m_bPreventDispMsg = TRUE;
			strMsg.Format(_T("Safty Area Sensor Detected."));
			pMainFrame->MessageBox(strMsg);
			pGlobalView->m_bPreventDispMsg = FALSE;
			return OPERATE_INTERLOCK;
		}
	#endif
		
		// 20100507-syd
	#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
		if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		{
			pGlobalView->ClearDispMessage();	// 20110224 syd
			pGlobalView->m_bPreventDispMsg = TRUE;	// 20110224 syd
			strMsg.Format(_T("Safty Area Sensor Detected."));
			pMainFrame->MessageBox(strMsg);
			pGlobalView->m_bPreventDispMsg = FALSE;	// 20110224 syd
			return OPERATE_INTERLOCK;
		}
	#endif
	
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (g_bUseTableLock && pIO->ReadBit(LD_SHK_TABLE_LOCK))
			{
				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
				else
					SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));
	
				return MEI_MOTION_ERROR;
			}
		}
	
		if (AoiParam()->m_bUseTableClamp)
		{
			if (GetClampObject()->CheckClampOperating() != CLAMP_OK)
			{
				if(CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_CLAMP_ERROR, 1);
	
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("클램프 동작중 모션 이동 감지!!, 클램프 동작 완료후 다시 시도하십시오"));
				else
					SetMainMessage(_T("Motion movement detected during clamp operation !!, Try again after clamp operation completed"));
	
				return OPERATE_INTERLOCK;
			}
		}
	
	
		//170105 lgh
		if (AoiParam()->m_bUseTableClamp && AoiParam()->m_bUseAutoClamp && pGlobalDoc->m_bUseInlineAutomation==0 && bCheckClamp)
		{
			//180511 LGH ADD
			if (AoiParam()->m_bClampSafetyModeForSemco)
			{
				if (GetPlatform()->m_ClampManager.CheckClampOn() != CLAMP_OK)
				{
					GetInterlockObject()->AddInterlockMessage(GetClampObject()->GetMsg(CLAMP_IS_NOT_READY, pGlobalDoc->m_nSelectedLanguage));
					pMainFrame->DispStatusBar("Clamp is not ready..."); 	//20100121-ndy for debugging
					return (OPERATE_INTERLOCK);
				}
			}
	
			if (!GetPlatform()->CheckClampBeforeMotionMove(pGlobalDoc->m_nSelectedLanguage))
			{
				pMainFrame->DispStatusBar("Clamp is not ready..."); 	//20100121-ndy for debugging
				return (OPERATE_INTERLOCK);
			}
		}
	
		if(pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
		{
			strMsg.Format(_T("Glass cover is open."));
			pMainFrame->MessageBox(strMsg);
			return OPERATE_INTERLOCK;
		}
		
		if(!m_bOrigin[CAM_AXIS])
		{
			if (pGlobalDoc->m_bUseInlineAutomation == 0)
			{
				pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
	
				strMsg.Format(_T("Camera %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
				if (!m_pMotionCard->m_nInterlockStatus)
					AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
	
				pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
			}
			else
			{
				CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
				CString strError;
	
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(CAM_AXIS));
				else
					strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(CAM_AXIS));
			
				SetMainMessage(strError);
			}
			return NOT_ORIGIN;
		}
		
		if(!m_bOrigin[SCAN_AXIS])
		{
			if (pGlobalDoc->m_bUseInlineAutomation == 0)
			{
				pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
				strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
	
				if (!m_pMotionCard->m_nInterlockStatus)
					AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
				pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
			}
			else
			{
				CController::m_pController->AlarmCall(ALARM_MOTION_ORIGIN_FAIL, 1);
				CString strError;
	
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					strError.Format(_T("%s, 원점 복귀가 이뤄지지 않아 이동 할 수 없습니다"), GetMotorName(SCAN_AXIS));
				else
					strError.Format(_T("%s, Can not move because homing is not done"), GetMotorName(SCAN_AXIS));
	
	
	
				SetMainMessage(strError);
			}
			return NOT_ORIGIN;
		}
	
		if(fTgtPosX < GetNegSWLimitValue(CAM_AXIS) || fTgtPosX > GetPosSWLimitValue(CAM_AXIS))
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if(CController::m_pController)	
				CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
				CString strError;
	
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));
				else
					strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));
	
						
				
				SetMainMessage(strError);
			}
			return MOTION_LIMIT_OVER;
		}
	
		if(fTgtPosY < GetNegSWLimitValue(SCAN_AXIS) || fTgtPosY > GetPosSWLimitValue(SCAN_AXIS))
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
				CString strError;
	
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));
				else
					strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));
	
	
				SetMainMessage(strError);
			}
			return MOTION_LIMIT_OVER;
		}
	
	#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
		pGlobalDoc->m_bTowerLampFlag = TRUE;
		pGlobalView->TowerLampControl(TOWER_LAMP_RED,ON);
	#endif

#ifdef USE_INTERPOLATION_MOVE
		TwoStartPosMove(SCAN_AXIS, CAM_AXIS, fTgtPosY, fTgtPosX, 75.0, ABS, NO_WAIT, OPTIMIZED);
#else
		StartPosMove(CAM_AXIS,fTgtPosX,75.0,ABS,NO_WAIT,OPTIMIZED);
		StartPosMove(SCAN_AXIS,fTgtPosY,75.0,ABS,NO_WAIT,OPTIMIZED);
#endif

	#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
		double dTgtPos = fTgtPosX+pGlobalDoc->m_fLightAxisOffset;
		if(dTgtPos < 0.0)
			dTgtPos = 0.0;
		StartPosMove(LIGHT_AXIS, dTgtPos, 75.0, ABS, NO_WAIT, OPTIMIZED);
	#endif
		
		if(bWait)
		{
//#ifdef USE_INTERPOLATION_MOVE
//			BOOL bSuccess = WaitXYPosEnd(CHECK_MOTION_DONE);
//			int mc = m_pMotionCard->UnGroup2Ax(NMC_GROUPID_INTERPOLRATION);
//			return bSuccess;
//#else
			return WaitXYPosEnd(CHECK_MOTION_DONE);
//#endif
		}
//		else
//		{
//#ifdef USE_INTERPOLATION_MOVE
//			int mc = m_pMotionCard->UnGroup2Ax(NMC_GROUPID_INTERPOLRATION);
//#endif
//		}
		
	#endif

	return SUCCESS;
}

BOOL CMotionNew::WaitScanEnd(DWORD dwTimeOver, BOOL bPumpMsg)
{
	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh) // while(1) // syd-20100629
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (g_bUseTableLock && pIO->ReadBit(LD_SHK_TABLE_LOCK))
			{

#ifdef MOTION_ACTION
				StopMove(SCAN_AXIS, 0.01, 0);
				//		m_pMotionNew->m_pMotionCard->m_nInterlockStatus = TRUE;
#endif
				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
				else
					SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

				return MEI_MOTION_ERROR;
			}
		}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}

		if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
		{
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				if (pGlobalView->m_bCheckSafetyOnlyBit)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
					}
					nRet = OPERATE_INTERLOCK;
					break;
				}
				else
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
					}
				}
				nRet = MEI_MOTION_ERROR;
				break;
			}
			else
			{
				if (pIO->CheckEmgSwitch())
				{
					nRet = EMERGENCY_STOP;
					break;
				}
			}
		}
#endif



#if MACHINE_MODEL == INSPRO10S_PLUS || MACHINE_MODEL == INSPRO20S || MACHINE_MODEL == INSPRO20S_PLUS || MACHINE_MODEL == INSPRO30S || MACHINE_MODEL == INSPRO30S_PLUS || MACHINE_MODEL == INSPRO40S || MACHINE_MODEL == INSPRO40S_PLUS_30S || MACHINE_MODEL == INSPRO40S_1IPU || MACHINE_MODEL == INSPRO30S_PLUS_AF || MACHINE_MODEL == INSPRO30S_PLUS_PRIMIUM
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(THETA_AXIS))
#else
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH  || MACHINE_MODEL == INSPRO_R2R_MDS || MACHINE_MODEL == INSPRO_R2R_EG || MACHINE_MODEL == INSMART_R2R_SUMITOMO
#ifdef LIGHT_AXIS		
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(SCAN_S_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(LIGHT_AXIS))
#else
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(SCAN_S_AXIS) && CheckMotionDone(CAM_AXIS))
#endif
#else
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS))
#endif
#endif
		{
			if (pGlobalDoc->m_bOperateInterlock)
				nRet = (OPERATE_INTERLOCK);
			else
			{
				nRet = SUCCESS;
			}
			break;
		}
		else if (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
			}

			nRet = OPERATE_INTERLOCK;
			break;
		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
		{
			nRet = USER_STOP;
			break;
		}
		else if (pIO->CheckEmgSwitch() || AoiParam()->m_bEmgStatusRestored == FALSE)
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_EMG_CALL, 1);
			}

			nRet = EMERGENCY_STOP;
			break;
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			nRet = (OPERATE_INTERLOCK);
			break;
		}
#if MACHINE_MODEL == INSPRO10S_PLUS || MACHINE_MODEL == INSPRO20S || MACHINE_MODEL == INSPRO20S_PLUS || MACHINE_MODEL == INSPRO30S || MACHINE_MODEL == INSPRO30S_PLUS || MACHINE_MODEL == INSPRO40S || MACHINE_MODEL == INSPRO40S_PLUS_30S || MACHINE_MODEL == INSPRO40S_1IPU || MACHINE_MODEL == INSPRO30S_PLUS_AF || MACHINE_MODEL == INSPRO30S_PLUS_PRIMIUM
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR || GetAxisState(THETA_AXIS) == MPIStateERROR)
#else
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSPRO_R2R_MDS || MACHINE_MODEL == INSPRO_R2R_EG || MACHINE_MODEL == INSMART_R2R_SUMITOMO
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(SCAN_S_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
#else
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
#endif
#endif
		{
			if (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit || pGlobalView->m_bSafetyInspection)
			{
				nRet = USER_STOP;
				break;
			}

			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
				}

				nRet = MEI_MOTION_ERROR;
				break;
			}
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
				}
				nRet = WAIT_TIME_OVER;
				break;
			}
		}
		if (bPumpMsg)
			pMainFrame->RefreshWindows(1);
	}

	// syd - 20101202 del, 20101206 ljg & syd del conform
	//	 pIO->ExtTriggerPulseOnOff(OFF); 

#endif	


	//20150507 - syd
	//if(pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor)
	if (pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bUseAutoFocusSol)
	{
		CString strMsg;

		int nScanDir;
		if (pGlobalDoc->m_nCurrSwathNum % 2)
			nScanDir = BACKWARD;
		else
			nScanDir = FORWARD;

		//		if(pGlobalDoc->m_bScanMode == BIDIRECTION && nScanDir == FORWARD && pGlobalDoc->m_bUseReverseDirAutoFocusing && pGlobalDoc->m_bLaserSensorOption)

		//20150507 - syd
		//if(pGlobalDoc->m_bLaserSensorOption) 	//20121125-ndy for 2Head Laser Control
		if (pGlobalDoc->m_bLaserSensorOption || pGlobalDoc->m_bUseAutoFocusSol)
		{
			ResetAutoFocusingSeq();
			StopLMI();
			//160116 lgh del.. 포커스 값이저장되푳E있햨E않은 상태에서 움직이기때문에 문제가 발생.
			//MoveFocusMotor(m_dCurrFocusPos,ABS,WAIT);	//20101102 mod	//m_dCurrFocusPos -> 광학 세팅된 포커스값
		}

		strMsg.Format(_T("af.n:%d"), pGlobalDoc->m_nInspAutoFocusCnt);
		pMainFrame->DispStatusBar(strMsg, 1);

		pGlobalDoc->m_nInspAutoFocusCnt = 0;
	}

	//170117 lgh
	if (GetAirCleaner()->m_bEnableCleanerAction)
	{
		GetAirCleaner()->ClearAirCleanerTriggerEvent();
	}

	return nRet;
}
BOOL CMotionNew::WaitGrabEnd(DWORD dwTimeOver, BOOL bPumpMsg)
{
	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	g_bLoopWh = TRUE; // syd-20100629
	int i = 0;
	while (g_bLoopWh) // while(1) // syd-20100629
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (g_bUseTableLock && pIO->ReadBit(LD_SHK_TABLE_LOCK))
			{

#ifdef MOTION_ACTION
				StopMove(SCAN_AXIS, 0.01, 0);
				//	m_pMotionNew->m_pMotionCard->m_nInterlockStatus = TRUE;
#endif
				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
				else
					SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

				return MEI_MOTION_ERROR;
			}
		}

		//160212 lgh add for async grab end
		int nRetCnt = 0;
		for (i = 0; i<pGlobalDoc->m_nNumOfIPU; i++)
		{
			if (pGlobalDoc->m_nAsyncEnd[i])
			{
				nRetCnt++;
			}
		}

		if (nRetCnt == pGlobalDoc->m_nNumOfIPU)
		{
			nRet = SUCCESS;
			break;
		}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif

#if MACHINE_MODEL == INSPRO10S_PLUS || MACHINE_MODEL == INSPRO20S || MACHINE_MODEL == INSPRO20S_PLUS || MACHINE_MODEL == INSPRO30S || MACHINE_MODEL == INSPRO30S_PLUS || MACHINE_MODEL == INSPRO40S || MACHINE_MODEL == INSPRO40S_PLUS_30S || MACHINE_MODEL == INSPRO40S_1IPU || MACHINE_MODEL == INSPRO30S_PLUS_AF || MACHINE_MODEL == INSPRO30S_PLUS_PRIMIUM
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(THETA_AXIS))
#else
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH
#ifdef LIGHT_AXIS
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(SCAN_S_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(LIGHT_AXIS))
#else
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(SCAN_S_AXIS) && CheckMotionDone(CAM_AXIS))
#endif
#else
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS))
#endif
#endif
		{
			if (pGlobalDoc->m_bOperateInterlock)
				nRet = (OPERATE_INTERLOCK);
			else
			{
				nRet = SUCCESS;
			}
			break;
		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
		{
			nRet = USER_STOP;
			break;
		}
		else if (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				if (CController::m_pController)
					CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
			}

			nRet = OPERATE_INTERLOCK;
			break;
		}
		else if (pIO->CheckEmgSwitch())
		{
			nRet = EMERGENCY_STOP;
			break;
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			nRet = (OPERATE_INTERLOCK);
			break;
		}
#if MACHINE_MODEL == INSPRO10S_PLUS || MACHINE_MODEL == INSPRO20S || MACHINE_MODEL == INSPRO20S_PLUS || MACHINE_MODEL == INSPRO30S || MACHINE_MODEL == INSPRO30S_PLUS || MACHINE_MODEL == INSPRO40S || MACHINE_MODEL == INSPRO40S_PLUS_30S || MACHINE_MODEL == INSPRO40S_1IPU || MACHINE_MODEL == INSPRO30S_PLUS_AF || MACHINE_MODEL == INSPRO30S_PLUS_PRIMIUM
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR || GetAxisState(THETA_AXIS) == MPIStateERROR)
#else
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(SCAN_S_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
#else
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
#endif
#endif
		{
			if (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit || pGlobalView->m_bSafetyInspection)
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
				}

				nRet = USER_STOP;
				break;
			}
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				nRet = MEI_MOTION_ERROR;
				break;
			}
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
				}
				nRet = WAIT_TIME_OVER;
				break;
			}
		}
		if (bPumpMsg)
			pMainFrame->RefreshWindows(1);
	}

	// syd - 20101202 del, 20101206 ljg & syd del conform
	//	 pIO->ExtTriggerPulseOnOff(OFF); 

#endif	


	//20150507 - syd
	//if(pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor)
	if (pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bUseAutoFocusSol)
	{
		CString strMsg;

		int nScanDir;
		if (pGlobalDoc->m_nCurrSwathNum % 2)
			nScanDir = BACKWARD;
		else
			nScanDir = FORWARD;

		//		if(pGlobalDoc->m_bScanMode == BIDIRECTION && nScanDir == FORWARD && pGlobalDoc->m_bUseReverseDirAutoFocusing && pGlobalDoc->m_bLaserSensorOption)

		//20150507 - syd
		//if(pGlobalDoc->m_bLaserSensorOption) 	//20121125-ndy for 2Head Laser Control
		if (pGlobalDoc->m_bLaserSensorOption || pGlobalDoc->m_bUseAutoFocusSol)
		{
			ResetAutoFocusingSeq();
			StopLMI();
			//160116 lgh del.. 포커스 값이저장되푳E있햨E않은 상태에서 움직이기때문에 문제가 발생.
			//MoveFocusMotor(m_dCurrFocusPos,ABS,WAIT);	//20101102 mod	//m_dCurrFocusPos -> 광학 세팅된 포커스값
		}

		strMsg.Format(_T("af.n:%d"), pGlobalDoc->m_nInspAutoFocusCnt);
		pMainFrame->DispStatusBar(strMsg, 1);

		pGlobalDoc->m_nInspAutoFocusCnt = 0;
	}

	//170117 lgh
	if (GetAirCleaner()->m_bEnableCleanerAction)
	{
		GetAirCleaner()->ClearAirCleanerTriggerEvent();
	}

	return nRet;
}
double CMotionNew::GetScanEndPosition(double fLineRate, double fOptRes, double fCurPos, double fScanSize, int nScanDir, long nStartDummy, long nEndDummy)
{
	double fScanVel = fLineRate * fOptRes;
	double fScanAcc = fScanVel*10.; // [mm/sec^2], 속도 추종성 확보를 위하여 급격한 가속 금햨E
	double fScanDec = fScanVel*10.; // [mm/sec^2]

	double fMaxVel = 1000.0; // [mm/s]
	double fMaxAcc = m_pParamAxis[SCAN_AXIS].fMaxAccel*9.8*1000.0;
	fScanVel = min(fScanVel, fMaxVel);
	fScanAcc = min(fScanAcc, fMaxAcc);
	fScanDec = min(fScanDec, fMaxAcc);

	// 1. 시작 더미 구간을 구한다.
	double fStartDummyLength = (nStartDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
																// 2. 감속 더미 구간을 구한다.
	double fEndDummyLength = (nEndDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
															// 3. 가속 구간의 거리 [mm]
	double fAccLength = ((fScanVel*fScanVel) / fScanAcc) / 2.0;
	double fDecLength = ((fScanVel*fScanVel) / fScanDec) / 2.0; // 감속 구간의 거리 [mm]
	double fCurPosition = fCurPos;
	double fDBNStartPos, fDBNEndPos, fTgtPos;
	if (fStartDummyLength < fAccLength)
	{
		fDBNStartPos = fCurPosition + pGlobalDoc->m_fSettleLen*nScanDir + (fAccLength - fStartDummyLength)*nScanDir;
	}
	else
	{
		fDBNStartPos = fCurPosition + pGlobalDoc->m_fSettleLen*nScanDir;
	}
	//fDBNEndPos = fDBNStartPos + ((fScanSize-(fOptRes/1000.))*nScanDir)+(fStartDummyLength*nScanDir);	// 20110404 ljh 
	fDBNEndPos = fDBNStartPos + ((fScanSize - (fOptRes / 1000.))*nScanDir) + (fEndDummyLength*nScanDir);// 110920 jsy mod
	fTgtPos = fDBNEndPos + (fDecLength*nScanDir);

	return fTgtPos;
}

// fOptRes: um단위의 광학 해상도 
// fImageSize : dummy를 포함한 mm단위의 종퉩EEswath 크콅E
// nScanDir : Scanning 퉩EE
// nStartDummy : fImageSize에서 시작부 dummy pulse 펯E
// nEndDummy : 종료부 dummy pulse 펯E
//161010 lgh scan 함수 정리 
BOOL CMotionNew::StartScanForBidirection(BOOL bPreAlign, BOOL bHoldingMotion, double dNextSwathXPos, double dNextSwathStartPos, BOOL& bScanPosMove, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir, long nStartDummy, long nEndDummy, double fCurrThicknessByLaserSensor)
{
	m_stScanParam[1].bHoldAction = bHoldingMotion;
	m_stScanParam[1].nScanMode = SCAN_MODE_ASYNC;
	m_stScanParam[1].bScanPosMove = 0;
	m_stScanParam[1].dHoldingPosX = dNextSwathXPos;
	m_stScanParam[1].dNextSwathStartPosY = dNextSwathStartPos;

	BOOL bRet = StartScan(bPreAlign, fLineRate, fOptRes, fScanSize, fImageSize, nScanDir, nStartDummy, nEndDummy, fCurrThicknessByLaserSensor, &m_stScanParam[1]);
	bScanPosMove = m_stScanParam[1].bScanPosMove;
	return bRet;
}

BOOL CMotionNew::StartScan(BOOL bPreAlign, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir, long nStartDummy, long nEndDummy, double fCurrThicknessByLaserSensor, SCAN_STRUCT* pScanParam)
{
	//return TRUE;
	if(pScanParam == nullptr)
	{
		pScanParam = &m_stScanParam[COMMON_PARAMETER];
	}

	int nLaserID = nScanDir == FORWARD ? 0 : 1;

	OpticalSysInfo* pOptics=GetOptic();
	
	CString strMsg;
	int nGroupID = 0;
	int nBlockID = 0;
		m_dCurrentDBNStartPos=0;
		m_dCurrentDBNEndPos=0;
	pGlobalDoc->ClearAsyncSignal();
	
	if(pGlobalDoc->m_bInhibitMotion)
		return FALSE;
		
#ifdef RI_SYSTEM
	return TRUE;
#endif
	if(m_pMotionCard->m_nInterlockStatus)
		return FALSE;
	
	if(!m_bOrigin[SCAN_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN,EES_ACTIVATE_ALARM);
		strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		if(!m_pMotionCard->m_nInterlockStatus)
			AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN,EES_DEACTIVATE_ALARM);
		return FALSE;
	}
	
	if(pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
	{
		strMsg.Format(_T("Glass cover is open."));
		pMainFrame->MessageBox(strMsg);
		return FALSE;
	}
	
	double fScanVel;
#ifdef USE_FRAME_TRIGGER   // 20120114 ljg add, check !!!
	double fScanVelRatio;
	
	fScanVelRatio = 1.00094;
	fScanVel = fLineRate * fScanVelRatio* fOptRes;
#else
	fScanVel = fLineRate * fOptRes;
#endif
	if(pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor)
	{
		pOptics->CalcOpticInfo(
		pGlobalDoc->m_nCamPixels,
		AoiParam()->m_dInspectionCamCCDPixelSize,
		AoiParam()->m_nCamStage,
		fLineRate,
		AoiParam()->m_nLightSpectrum,
		AoiParam()->m_dLensFnumber,
		fOptRes);
	
		pOptics->CalcWarpageReference(
			fScanVel,
			fOptRes*pGlobalDoc->m_fTriggerPulseStep,
			pGlobalDoc->m_fTriggerPulseStep,
			AoiParam()->m_dLaserSamplingTime,
			AoiParam()->m_dLaserSpotHeight,
			AoiParam()->m_nLaserSampleCount);
	
		if (AoiParam()->m_bUseLaserSpotAdjust)
		{
			if(nScanDir == FORWARD)
				AoiParam()->m_dScanPosShiftForCommThread = +fabs(pOptics->m_dTriggerShiftLength) / 1000.0;
			else
				AoiParam()->m_dScanPosShiftForCommThread = -fabs(pOptics->m_dTriggerShiftLength) / 1000.0;
	
		}
		else
			AoiParam()->m_dScanPosShiftForCommThread = 0;
	
		AoiParam()->m_dAdativeAFPositionLimit = pOptics->m_dWarpageAdjustMaxVal / 1000.0;
	}
	
	double fScanAcc = fScanVel*10.; // [mm/sec^2], 속도 추종성 확보를 위하여 급격한 가속 금햨E
	double fScanDec = fScanVel*10.; // [mm/sec^2]
	
	double fMaxVel = 1000.0; // [mm/s]	
	double fMaxAcc = m_pParamAxis[SCAN_AXIS].fMaxAccel*9.8*1000.0;
	fScanVel = min(fScanVel,fMaxVel);
	fScanAcc = min(fScanAcc,fMaxAcc);
	fScanDec = min(fScanDec,fMaxAcc);
	
	// 1. 시작 더미 구간을 구한다.
	double fStartDummyLength = (nStartDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
		// 가속 구간의 거리 [mm]
	double fAccLength = ((fScanVel*fScanVel)/fScanAcc)/2.0;
	double fDecLength = ((fScanVel*fScanVel)/fScanDec)/2.0; // 감속 구간의 거리 [mm]
	double fCurPositionX = GetCommandPosition(CAM_AXIS);
	double fCurPositionY = GetCommandPosition(SCAN_AXIS);
	double fDBNStartPos,fDBNEndPos,fTgtPos;
		
	// 2009,02,10 by khc
	double fYDistOfLaserSensorAndInspectCamera = 0;
	double fYDistOfAirSolAndInspectCamera = 0;
	
#if MACHINE_MODEL == INSMART_EXTREME_PLUS
	if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
	{
		if (nScanDir == BACKWARD)	//20121125-ndy for 2Head Laser Control
			fYDistOfAirSolAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_dAirCleanerScanOffset[1];
		else
			fYDistOfAirSolAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_dAirCleanerScanOffset[0];
	}
#endif
	if(pGlobalDoc->m_bUseAutoFocusSol) //20150507 - syd
	{
	
	}
	else
	{
		if(nScanDir == BACKWARD)	//20121125-ndy for 2Head Laser Control
			fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y+pGlobalDoc->m_fLaserSensorOffsetY[1];
		else
			fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y+pGlobalDoc->m_fLaserSensorOffsetY[0];
	
		//20120713-ndy
		if(nScanDir == BACKWARD)
		{
			if(!pGlobalDoc->m_bUseUniHeaderLaserBiInsp)//20150119-ndy for UniLaser BiInspection
				fYDistOfLaserSensorAndInspectCamera += pGlobalDoc->m_fLaserStartScanPosOffset;
		}
		else
			fYDistOfLaserSensorAndInspectCamera -= pGlobalDoc->m_fLaserStartScanPosOffset;
	
	}
	
	if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD)
	{
		fYDistOfLaserSensorAndInspectCamera = 0;
	}
	
#ifdef USE_LMI_SENSOR
	if (pGlobalDoc->m_bUseUniHeaderLaserBiInsp && g_SequenceAF.m_bUseReverseDirAFby1head && nScanDir == BACKWARD)
	{
		fYDistOfLaserSensorAndInspectCamera = 0;
	}
#endif
	
	pGlobalDoc->m_fLaserStartScanPosOffset = 0.0;
#if MACHINE_MODEL == INSMART_EXTREME_PLUS
	if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
	{
		//무조건 가속 구간을 따라간다
	
		double dDelayLength = 0;
	
		if (GetAirCleaner()->m_nAirDelayTimeMs != 0)
		{
			double dScanVel = pGlobalDoc->m_fScanRate * SizeData[COMMON_PARAMETER].fPixelSizeY;
	
			if (dScanVel != 0.0)
			{
				dDelayLength = fabs(((double)GetAirCleaner()->m_nAirDelayTimeMs*0.001) * dScanVel);
			}
		}
	
		if (nScanDir == BACKWARD)
			fDBNStartPos = fCurPositionY + ((fStartDummyLength + fYDistOfAirSolAndInspectCamera - fAccLength - dDelayLength));
		else
			fDBNStartPos = fCurPositionY + ((dDelayLength + fAccLength + pGlobalDoc->m_fSettleLen + fYDistOfAirSolAndInspectCamera - fStartDummyLength)*nScanDir);
	}
	else if((pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bMeasureThicknessAction || pGlobalDoc->m_bUseAutoFocusSol) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm) //20150507 - syd
#else
		if ((pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bMeasureThicknessAction || pGlobalDoc->m_bUseAutoFocusSol) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm) //20150507 - syd
#endif
	{		
		if( fAccLength > fStartDummyLength)
		{
			if(fabs(fYDistOfLaserSensorAndInspectCamera) > fAccLength)
			{
				if(nScanDir == BACKWARD)
					fDBNStartPos = fCurPositionY - ((fStartDummyLength + fYDistOfLaserSensorAndInspectCamera)*nScanDir);
				else
					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fYDistOfLaserSensorAndInspectCamera-fStartDummyLength)*nScanDir);
			}
			else
				fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fAccLength-fStartDummyLength)*nScanDir);
		}
		else // if( fStartDummyLength > fAccLength)
		{
			if(fabs(fYDistOfLaserSensorAndInspectCamera) > fStartDummyLength)
			{
				if(nScanDir == BACKWARD)
					fDBNStartPos = fCurPositionY - ((fStartDummyLength + fYDistOfLaserSensorAndInspectCamera)*nScanDir);
				else
					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fYDistOfLaserSensorAndInspectCamera-fStartDummyLength)*nScanDir);
			}
			else
				fDBNStartPos = fCurPositionY + pGlobalDoc->m_fSettleLen*nScanDir;
		}
#ifdef USE_FRAME_TRIGGER
	#if MACHINE_MODEL == INSPRO_GM_PLUS
		fDBNStartPos += 82 * SizeData[COMMON_PARAMETER].fPixelSizeY / 1000.0;		//0.060844	//20140725-ndy only Test. Must modeling for AF Distance!!.
	#endif
#endif
	}
	else
	{
		//20150507 - syd
		if( fAccLength > fStartDummyLength)
			fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fAccLength-fStartDummyLength)*nScanDir);
		else
			fDBNStartPos = fCurPositionY + pGlobalDoc->m_fSettleLen*nScanDir;
	}
	fDBNEndPos = fDBNStartPos + ((fScanSize-(fOptRes/1000.))*nScanDir)+ ((fOptRes*(double)nEndDummy/1000.)*nScanDir);
	
//	pGlobalView->SetAutoFocusEndPos(fDBNEndPos);
				
	fTgtPos = fDBNEndPos + (fDecLength*nScanDir);
	//160408 lgh
	m_dCurrentDBNStartPos = fDBNStartPos;
	m_dCurrentDBNEndPos = fDBNEndPos;	
	//161010 LGH
	if(pScanParam->nScanMode == SCAN_MODE_ASYNC)
	{	
		//양방향 액션시 스캔 종료 지점을 동일하게 만들어야한다. dummy 구간이 크다면 두번 움직이는게 나은데 반대라면 한번 움직이는게 좋다.
		if((nScanDir == BACKWARD && pScanParam->dNextSwathStartPosY <= fDBNEndPos) || (nScanDir == FORWARD && pScanParam->dNextSwathStartPosY >= fDBNEndPos)  )
		{
			TRACE("Start Scan Y Pos Changed = %f -> %f\n", fTgtPos,pScanParam->dNextSwathStartPosY);	//150602 jsy Debug
			fTgtPos = pScanParam->dNextSwathStartPosY;
			pScanParam->bScanPosMove = FALSE;
		}
		else
		{
			//다음 
			pScanParam->bScanPosMove = TRUE;
			TRACE("Start Scan Y Pos No changed = %f, next %f\n", fTgtPos,pScanParam->dNextSwathStartPosY);	//150602 jsy Debug
		}
	}
	
	double fPeriod = 1/(fLineRate*1000.)*1000000.; // [us]
	long   nPulseWidth = int(fPeriod/2.0);// duty 50% [us]
	
	// 일단 정방향에
	if(fTgtPos >= m_pParamMotor[SCAN_AXIS].fPosLimit && nScanDir == PLUS)
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_SCANLIMIT,EES_ACTIVATE_ALARM);
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "POS_SCAN_RANGE_OVER"));
		AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_SCANLIMIT,EES_DEACTIVATE_ALARM);
		return FALSE;
	}
	
	if(fTgtPos <= m_pParamMotor[SCAN_AXIS].fNegLimit && nScanDir == MINUS)
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_SCANLIMIT,EES_ACTIVATE_ALARM);
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "NEG_SCAN_RANGE_OVER"));
		AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_SCANLIMIT,EES_DEACTIVATE_ALARM);
		return FALSE;
	}
	
	//20120601-ndy for ConvexDriver
	double ffDBNStartPos = (double)LengthToPulse(SCAN_AXIS,fDBNStartPos);
	double ffDBNEndPos = (double)LengthToPulse(SCAN_AXIS,fDBNEndPos);
	double ffCurPosition = (double)LengthToPulse(SCAN_AXIS,fCurPositionY);
// fPeriod : uSec	
#if TRIGGER_TYPE == RSA_TRIGGER
//	TriggerStop(TRIGGER_ETHERCAT_ADDR, TRIGGER_CAMERA_VX); //2021.05.06
	TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_CAMERA_VX, fDBNStartPos, fDBNEndPos, (double)nPulseWidth, fOptRes);// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec , fOptRes : um
#else
	
#if TRIGGER_TYPE == GVIS_TRIGGER	//20120601-ndy for ConvexDriver
#ifndef RI_SYSTEM
	BOOL bRet;
	int nCnt = 0;
	do{
		bRet = m_pGtkDBN->DBNModify(nCnt,nScanDir,fCurPositionY, ffDBNStartPos,ffDBNEndPos,fOptRes,1);
		if(!bRet)
			break;
		nCnt++;
	}while(nCnt < 10);
		
	if(nCnt >= 10)
	{
		AfxMessageBox(_T("[MSG304] Error! at DBNModify() at StartScan(): Call GigaVis, please!!!"));
	}
#endif
#if MACHINE_MODEL == INSPRO10S
	SetTriggerStartPos((ffDBNStartPos - ffCurPosition) / (2.0 * fOptRes));
#endif
	
#elif TRIGGER_TYPE == CONVEX_TRIGGER || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
	if(nScanDir != BACKWARD)
	{
		double dSpace = fOptRes/1000.0;//fPeriod, fOptRes/1000.0
		double dStart = fDBNStartPos;
		double dEnd = (int((fDBNEndPos - fDBNStartPos) / dSpace ) + 1) * dSpace + fDBNStartPos;
	 	SetConvexDriveTrigOriginPosition(SCAN_AXIS, GetActualPosition(SCAN_AXIS));
//		SetConvexDriveTrigDirection(SCAN_AXIS, 0);	//111223 hjc test
//		SetConvexDriveTrigPulseWidth(SCAN_AXIS, 0.002); //dSpace/2.0 mSec
		SetConvexDriveTrigPulseWidth(SCAN_AXIS, dSpace/2.0);// mSec
		SetConvexDriveTrigSpace(SCAN_AXIS, dSpace);
		SetConvexDriveTrigStartPosition(SCAN_AXIS, dStart);
		SetConvexDriveTrigEndPosition(SCAN_AXIS, dEnd);
	
	}
	else
	{
		double dSpace = fOptRes/1000.0;//fPeriod, fOptRes/1000.0
		double dEnd = fDBNStartPos;
		double dStart = (int((fDBNEndPos - fDBNStartPos) / dSpace ) - 1) * dSpace + fDBNStartPos;
		SetConvexDriveTrigOriginPosition(SCAN_AXIS, GetActualPosition(SCAN_AXIS));
//		SetConvexDriveTrigDirection(SCAN_AXIS, 1);	//111223 hjc test
//		SetConvexDriveTrigDirection(SCAN_AXIS, 0);
//		SetConvexDriveTrigPulseWidth(SCAN_AXIS, 0.002); //dSpace/2.0 mSec
		SetConvexDriveTrigPulseWidth(SCAN_AXIS, dSpace/2.0);// mSec
		SetConvexDriveTrigSpace(SCAN_AXIS, dSpace);
		SetConvexDriveTrigStartPosition(SCAN_AXIS, dStart);
		SetConvexDriveTrigEndPosition(SCAN_AXIS, dEnd);
	}
	
#elif TRIGGER_TYPE == JUSTEK_TRIGGER	//20120601-ndy for ConvexDriver
	// syd - 20101202
	if(!pGlobalDoc->m_bStopJtedSetting)
	{
		double dMotionRes = GetPositionResolution(SCAN_AXIS); // mm
		double dJtedRes = 50.0;	   // nm
		double dOptMotionResRate = fOptRes / (dMotionRes * 1000.0) ; 
		double dMotionJtedResRate = (dMotionRes * 1000000.0) / dJtedRes ; 
	
		int nWinMin = (/*4.0 * */ffDBNStartPos * dMotionJtedResRate);
		int nWinMax = (/*4.0 * */ffDBNEndPos * dMotionJtedResRate);
	
		double plsOptRes = (double)LengthToPulse(SCAN_AXIS,fOptRes/1000.0);			// syd-20120104
		int nWinMaxSp = (/*4.0 * */(ffDBNStartPos+plsOptRes) * dMotionJtedResRate);		// syd-20120104
	
	
		int nDenominator = int(fOptRes * 1000000.0); // um -> pm : 광학해상도.
		int nNumerator = int(/*4.0 * */ dJtedRes * 1000.0); // nm -> pm : JTED 해상도 * 4.0 (A+상만 사퓖E .
		double dJtedMulCn3 = (double)nNumerator / (double)nDenominator;
	  	double dJtedMulCn4 = (double)nNumerator / (double)nDenominator / pGlobalDoc->m_fTriggerPulseStep;	//20120127-ndy for Variable Trigger Pulse
	
		int nWinMinCn3 = int(dJtedMulCn3 * (double)nWinMin);
	
#ifdef USE_FRAME_TRIGGER
		int nWinMaxCn3 = int(dJtedMulCn3 * (double)nWinMaxSp);			// syd-20120104
#else
		int nWinMaxCn3 = int(dJtedMulCn3 * (double)nWinMax);
#endif
	
	 	double ffDBNEndPosOffset = ((double)LengthToPulse(SCAN_AXIS,fOptRes*(pGlobalDoc->m_fTriggerPulseStep/1000.0)))*dMotionJtedResRate;			// syd-20120104	//20120127-ndy for Variable Trigger Pulse
	
	 	int nWinMinCn4 = int(dJtedMulCn4 * (double)nWinMin);
	 	int nWinMaxCn4 = int(dJtedMulCn4 * (double)(nWinMax+(ffDBNEndPosOffset*5.5)));
	
	//	if(	pGlobalDoc->m_dJtedWinMin != nWinMin || pGlobalDoc->m_dJtedWinMax != nWinMax )
		{
			pGlobalDoc->m_dJtedWinMin = nWinMin;
			pGlobalDoc->m_dJtedWinMax = nWinMax;
			pGlobalView->SetJtedWindowRange(CN3, (int)nWinMinCn3, (int)nWinMaxCn3);
	 		if(pGlobalDoc->m_bLaserSensorOption)
			{
	 			pGlobalView->SetJtedWindowRange(CN4, (int)nWinMinCn4, (int)nWinMaxCn4);
				int nOffset = int((double)( (pGlobalDoc->m_fTriggerPulseStep*fOptRes*1000.0) - (((double)nWinMin*dJtedRes) - (double)(int(((double)nWinMin*dJtedRes)/(pGlobalDoc->m_fTriggerPulseStep*fOptRes*1000.0))*(pGlobalDoc->m_fTriggerPulseStep*fOptRes*1000.0)) ) ) / dJtedRes);	//20120127-ndy for Variable Trigger Pulse
				int nMulRate = 1;
				pGlobalView->SetTriggerOffset(CN4, nOffset, nMulRate);
			}
			Sleep(10);
		}
	}
		
#elif TRIGGER_TYPE == ADLINK_8124	//20120601-ndy for ConvexDriver
	pGlobalView->m_pTrig8124->SetWndRngCamera(0, fOptRes, fDBNStartPos, fDBNEndPos);	// 20120315 ljh
#if MACHINE_MODEL == INSMART_GM4000
//	pGlobalView->m_pATFLaserControl->EnableLaserPowerControl(fDBNStartPos,fDBNEndPos,fOptRes,nScanDir,fAccLength,nStartDummy,nEndDummy);
	if(AoiParam()->m_bUseATFCutOff)
	{
//		Sleep(50);
		int nAtfHWStat =0;
//			pGlobalView->m_pTrig8124->SetManualTrig(3);		
		atf_ReadHwStat(&nAtfHWStat);
	
		if((nAtfHWStat & 0x00000100) ==0)
		{
			pGlobalView->m_pTrig8124->SetManualTrig(3);		
		}
		double dDummyextra = 0;
		if( fAccLength > fStartDummyLength)
			dDummyextra = (((pGlobalDoc->m_fSettleLen + fAccLength-fStartDummyLength)*nScanDir)) * 0.5;
		else
			dDummyextra = (pGlobalDoc->m_fSettleLen*nScanDir) * 0.5;	
	
		pGlobalView->m_pTrig8124->SetWndRngTrigger(3, fDBNStartPos+dDummyextra, fDBNEndPos-((fOptRes*(double)nEndDummy/1000.)*nScanDir),1);// 151216KSH
	}
	
//		pGlobalView->m_pTrig8124->SetWndRngLaser(3,0.4,fDBNStartPos,fDBNEndPos);
#endif
#endif // #if TRIGGER_TYPE == GVIS_TRIGGER
#endif // TRIGGER_TYPE == RSA_TRIGGER
	
	//161010 LGH
	if(pScanParam->bHoldAction)
	{	
		// Setting Camera Axis Start Position for Holding Motion
		int nMotionRet;
		if((nMotionRet = pMotionNew->WaitScanEnd(TIME_OUT_MS,0)) != SUCCESS)
		{
			if(nMotionRet == MEI_MOTION_ERROR)
				pGlobalDoc->CallService(21);
		}
		Sleep(30);
	
		//160305 lgh add for holding motion?
		double fHoldingReleasePos = fDBNEndPos;// - ((fOptRes*(double)nEndDummy/1000.)*nScanDir) *0.5;	// For Holding Motion;
			//	fHoldingReleasePos = fDBNEndPos;
	
		pMotionNew->HoldingMoveEx(SCAN_AXIS, fHoldingReleasePos, CAM_AXIS, pScanParam->dHoldingPosX, nScanDir);
		TRACE("Event Axis Scan Y Pos = %f, Release Pos %f\n", fTgtPos,fHoldingReleasePos);	//150602 jsy Debug
	}
	
	pIO->ExtTriggerPulseDir(nScanDir);  // 7--
	pIO->ExtTriggerPulseOnOff(ON);		// 6
	
	Sleep(10);   // 20101206 ljg & syd chg
	
	double fDevideLength; // [mm]
	fDevideLength=fOptRes*pGlobalDoc->m_fTriggerPulseStep/1000.0; // [mm] Laser Triger Signal is outted by 8000 lines.	//20120127-ndy for Variable Trigger Pulse
	double fMeasureLength;
		
	if(pGlobalDoc->m_bLaserSensorOption && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm)	//090311-ndy
	{
#if TRIGGER_TYPE == ADLINK_8124 || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
			double d8124StartPos=0, d8124EndPos=0;
			int nTargetCh=0;
			double fLaserPosOFfset=0;
	
			fLaserPosOFfset = 0.5;
	
			//180817 lgh fix
			if (nScanDir == FORWARD)
			{
				fLaserPosOFfset = +fDevideLength;
			}
			else
			{
				fLaserPosOFfset = -fDevideLength;
			}
	
			nTargetCh = 1;
	
			if(pGlobalDoc->m_bUseReverseDirAutoFocusing && nScanDir == BACKWARD && !pGlobalDoc->m_bUseUniHeaderLaserBiInsp)	//20121125-ndy for 2Head Laser Control
			{
				nTargetCh = 2;
			}
	
			if(pGlobalDoc->m_bUseReverseDirAutoFocusing && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)	//20150119-ndy for UniLaser BiInspection
			{
				nTargetCh = 1;
			}
#if MACHINE_MODEL == INSMART_EXTREME_PLUS
			if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
			{
				if (nScanDir == FORWARD)
				{
					d8124StartPos = fDBNStartPos  - fabs(fYDistOfLaserSensorAndInspectCamera) + fLaserPosOFfset; // 20160203 - syd
				}
				else
				{
					d8124StartPos = fDBNStartPos  + fabs(fYDistOfLaserSensorAndInspectCamera) + fLaserPosOFfset; // 20160203 - syd
				}
			}
			else
#endif
				d8124StartPos = fCurPositionY + fLaserPosOFfset; // 20160203 - syd
	
			if(nScanDir==FORWARD)
			{
				if(fDBNEndPos - fYDistOfLaserSensorAndInspectCamera > d8124StartPos + fDevideLength)
					d8124EndPos   = fDBNEndPos - fYDistOfLaserSensorAndInspectCamera; // 20160203 - syd
				else
					d8124EndPos = fDBNEndPos + fDevideLength;
			}
			else
			{
				d8124EndPos   = fDBNEndPos + fYDistOfLaserSensorAndInspectCamera; // 20160203 - syd
			}
	
			if(AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)
			{
				InitInspectCamAutoFocusingWithLaserSensor(fScanVel, 
					fDevideLength,
					(pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0]), 
					//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
					pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
					pGlobalDoc->m_bSaveFocusInfoToROI, 
					pGlobalDoc->m_dFocusAdjustingLimitInUm/1000.0);
			}
			else
			{
				double dReal8124StartPos = d8124StartPos;
				double dReal8124EndPos = d8124EndPos;
	
				CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
	
				int nAFDir = FORWARD;
	
				if (nScanDir != FORWARD)
				{
					nAFDir = BACKWARD;
				}
	
				if (m_bEnableRTAF)
				{
					pAFModule->m_nSeqAddCount[0] = 0;
					pAFModule->m_nSeqAddReal = 0;
	
					m_dScanSpeed = fOptRes*fLineRate;
	
					pAFModule->m_pRTAF[0]->ApplyScanDir(nScanDir, m_nCurMotorSwath, fLineRate);
					SetRTAFTrigger(m_nCurMotorSwath, nAFDir, nTargetCh, fDBNStartPos, d8124EndPos);
	
					pAFModule->SetMotorSwath(m_nCurMotorSwath);
					pAFModule->EnableRTAF(nAFDir, TRUE,0);
				}
				else
				{
					pAFModule->SetMotorSwath(m_nCurMotorSwath);
					pAFModule->EnableRTAF(nAFDir, FALSE,1);
	
					//20200422 Edit by LHY
					pGlobalView->m_pTrig8124->SetWndRngLaser(nTargetCh, g_SequenceAF.m_dEncoderSpacing * 1000,
						dReal8124StartPos,
						dReal8124EndPos);

					g_SequenceAF.m_dReal8124StartPos = dReal8124StartPos;
				}
			}
#endif	// #if TRIGGER_TYPE == ADLINK_8124 || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER

#if TRIGGER_TYPE == RSA_TRIGGER
			double dStartPos = 0, dEndPos = 0;
			int nTargetCh = 0;
			double fLaserPosOFfset = 0;

			fLaserPosOFfset = 0.5;

			//180817 lgh fix
			if (nScanDir == FORWARD)
			{
				fLaserPosOFfset = +fDevideLength;
			}
			else
			{
				fLaserPosOFfset = -fDevideLength;
			}

			nTargetCh = 1;

			if (pGlobalDoc->m_bUseReverseDirAutoFocusing && nScanDir == BACKWARD && !pGlobalDoc->m_bUseUniHeaderLaserBiInsp)	//20121125-ndy for 2Head Laser Control
			{
				nTargetCh = 2;
			}

			if (pGlobalDoc->m_bUseReverseDirAutoFocusing && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)	//20150119-ndy for UniLaser BiInspection
			{
				nTargetCh = 1;
			}
#if MACHINE_MODEL == INSMART_EXTREME_PLUS
			if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
			{
				if (nScanDir == FORWARD)
				{
					d8124StartPos = fDBNStartPos - fabs(fYDistOfLaserSensorAndInspectCamera) + fLaserPosOFfset; // 20160203 - syd
				}
				else
				{
					d8124StartPos = fDBNStartPos + fabs(fYDistOfLaserSensorAndInspectCamera) + fLaserPosOFfset; // 20160203 - syd
				}
			}
			else
#endif
				dStartPos = fCurPositionY + fLaserPosOFfset; // 20160203 - syd

			if (nScanDir == FORWARD)
			{
				if (fDBNEndPos - fYDistOfLaserSensorAndInspectCamera > dStartPos + fDevideLength)
					dEndPos = fDBNEndPos - fYDistOfLaserSensorAndInspectCamera; // 20160203 - syd
				else
					dEndPos = fDBNEndPos + fDevideLength;
			}
			else
			{
				dEndPos = fDBNEndPos + fYDistOfLaserSensorAndInspectCamera; // 20160203 - syd
			}

			if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)
			{
				InitInspectCamAutoFocusingWithLaserSensor(fScanVel,
					fDevideLength,
					(pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0]),
					//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
					pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
					pGlobalDoc->m_bSaveFocusInfoToROI,
					pGlobalDoc->m_dFocusAdjustingLimitInUm / 1000.0);
			}
			else
			{
				double dRealStartPos = dStartPos;
				double dRealEndPos = dEndPos;

				CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();

				int nAFDir = FORWARD;

				if (nScanDir != FORWARD)
				{
					nAFDir = BACKWARD;
				}

				if (m_bEnableRTAF)
				{
					pAFModule->m_nSeqAddCount[0] = 0;
					pAFModule->m_nSeqAddReal = 0;

					m_dScanSpeed = fOptRes*fLineRate;

					pAFModule->m_pRTAF[0]->ApplyScanDir(nScanDir, m_nCurMotorSwath, fLineRate);
//					SetRTAFTrigger(m_nCurMotorSwath, nAFDir, nTargetCh, fDBNStartPos, dEndPos); // ? dEndPos ---> fDBNEndPos
					SetRTAFTrigger(m_nCurMotorSwath, nAFDir, nTargetCh, dRealStartPos, fTgtPos); // ? dEndPos ---> fTgtPos
					g_SequenceAF.m_dReal8124StartPos = dRealStartPos;

					pAFModule->SetMotorSwath(m_nCurMotorSwath);
					pAFModule->EnableRTAF(nAFDir, TRUE, 0);
				}
				else
				{
					pAFModule->SetMotorSwath(m_nCurMotorSwath);
					pAFModule->EnableRTAF(nAFDir, FALSE, 1);

					//pGlobalView->m_pTrig8124->SetWndRngLaser(nTargetCh, pGlobalDoc->m_fTriggerPulseStep * fOptRes,
					//	dReal8124StartPos,
					//	dReal8124EndPos);

					// fPeriod : uSec
					//TriggerStop(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX); //2021.05.06


					TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dRealStartPos, fTgtPos, (double)nPulseWidth * g_SequenceAF.m_dEncoderSpacing * 1000.0 / fOptRes, g_SequenceAF.m_dEncoderSpacing * 1000.0);	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//					TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dRealStartPos, dRealEndPos, (double)nPulseWidth * g_SequenceAF.m_dEncoderSpacing * 1000.0 / fOptRes, g_SequenceAF.m_dEncoderSpacing * 1000.0);	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
					g_SequenceAF.m_dReal8124StartPos = dRealStartPos;
				}
			}
#endif	// TRIGGER_TYPE == RSA_TRIGGER

			//170115 lgh
			if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
			{
				double d8124AirStart = 0;
				double d8124AirEnd = 0;
	
				double dNewPosY = d8124AirStart - fYDistOfAirSolAndInspectCamera;
				CLAMP_MESSAGE clamp = GetClampObject()->CheckClampOn();
	
				if (clamp == CLAMP_OK)
				{
					if (nScanDir == BACKWARD)
					{
						d8124AirStart = fDBNStartPos - fStartDummyLength - 0.5;
						d8124AirEnd = fDBNEndPos - fStartDummyLength - 0.5;
					}
					else
					{
						d8124AirStart = fDBNStartPos + fStartDummyLength + 0.5;
						d8124AirEnd = fDBNEndPos + fStartDummyLength + 0.5;
					}
				}
				else
				{
					if (nScanDir == BACKWARD)
					{
						d8124AirStart = fDBNStartPos - fStartDummyLength - 0.5 - 15;
						d8124AirEnd = fDBNEndPos - fStartDummyLength - 0.5 + 15;
					}
					else
					{
						d8124AirStart = fDBNStartPos + fStartDummyLength + 0.5 + 15;
						d8124AirEnd = fDBNEndPos + fStartDummyLength + 0.5 - 15;
					}
				}
	
	
				GetAirCleaner()->SetAirCleanerTriggerEvent(nScanDir, fCurPositionY, d8124AirStart, fTgtPos, d8124AirEnd, fScanVel);
			}
	
	
		double fPanelStartPosY = EntireRegion.fStartY 
			        - pGlobalDoc->PixelToLength(SCAN_AXIS, SizeData[nBlockID].GrabMarginY)
					+ SizeData[COMMON_PARAMETER].fPinPosY 
					+ pGlobalDoc->m_fFidPosY 
					+ pGlobalDoc->m_TstCamOffset_Y;
		double fPanelEndPosY = EntireRegion.fEndY 
			        - pGlobalDoc->PixelToLength(SCAN_AXIS, SizeData[nBlockID].GrabMarginY)
					+ SizeData[COMMON_PARAMETER].fPinPosY 
					+ pGlobalDoc->m_fFidPosY 
					+ pGlobalDoc->m_TstCamOffset_Y;
	
		if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)
		{
			double dTempOffset = pGlobalDoc->m_fLaserSensorOffsetY[0]*2;
	
			if (fDBNEndPos > fPanelStartPosY - (pGlobalDoc->m_TstCamOffset_Y + dTempOffset))
				fMeasureLength = fCurPositionY - fDBNEndPos;
			else
				fMeasureLength = fCurPositionY - (fPanelStartPosY - (pGlobalDoc->m_TstCamOffset_Y + dTempOffset));
		}
		else
		{
			if (nScanDir == BACKWARD)	//20121125-ndy for 2Head Laser Control
			{
				if (fDBNEndPos > fPanelStartPosY - (pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1]))
					fMeasureLength = fCurPositionY - fDBNEndPos;
				else
					fMeasureLength = fCurPositionY - (fPanelStartPosY - (pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1]));
			}
			else
			{
				if (fDBNEndPos < fPanelEndPosY - (pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0]))
					fMeasureLength = fDBNEndPos - fCurPositionY;
				else
					fMeasureLength = (fPanelEndPosY - (pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0])) - fCurPositionY;
			}
		}
	
		double fnMaxRecvCount;
		if(fMeasureLength > fDevideLength)
			fnMaxRecvCount= (fMeasureLength / fDevideLength) + 1; //modifiy by khc 2009,07,17
		else
			fnMaxRecvCount = 0;
	
		pGlobalView->m_nMaxRecvCount = int(fnMaxRecvCount);
	
		if(pGlobalDoc->m_bUseUniHeaderLaserBiInsp && nScanDir != FORWARD)		//20150119-ndy for UniLaser BiInspection
			pGlobalView->m_nUniAFTrigCnt = pGlobalView->m_nRecvCntByExtTrig;
		else
			pGlobalView->m_nUniAFTrigCnt = 0;
	
		pGlobalView->m_nRecvCntByExtTrig=0;
	
		//171018 lgh
		if (m_bEnableRTAF)
		{
			int nAFDir = FORWARD;
			if (nScanDir != FORWARD)
				nAFDir = BACKWARD;
	
			CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
	
			CArPanelRTAFInfo* pAFInfo = pAFModule->m_pRTAF[0]->GetRTAFInfo();
	
			structPanelRTAFInfo stSwathArray = pAFInfo->GetAt(m_nCurMotorSwath);
	
			int nPosCnt = stSwathArray.pArRTAFInfo->GetCount();
	
			if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD)
			{
	
			}
			else
			{
				pGlobalView->m_nMaxRecvCount = nPosCnt;
	
	
				pGlobalView->m_arHeight.RemoveAll();
	
				double dTriggerStartPos = 0;
				double dTriggerEndPos = 0;
	
#ifdef _DEBUG
				CString strTitle, strPath, strIdx;
				strPath.Format(_T("%sAFStructure.txt"), pGlobalDoc->m_strSharedDir);
				if (nScanDir == FORWARD)
					strTitle.Format(_T("Forward"));
				else
					strTitle.Format(_T("BackWard"));
#endif
				structHeightInfo sttHeight;
				int nCount;
				BOOL bValid;
				int n;
				int nBreakPoint;
				double fZoomPos = 0.0;
	
#ifdef ZOOM_AXIS
				fZoomPos = GetActualPosition(ZOOM_AXIS);
#endif
	
				for (n = 0; n < nPosCnt; n++)
				{
					structRTAFInfo info = stSwathArray.pArRTAFInfo->GetAt(n);
	
					sttHeight.m = fZoomPos; // modify by khc 2008,03,23
					sttHeight.x = fCurPositionX;// +pGlobalDoc->m_TstCamOffset_X;
	
					sttHeight.y = info.dPosToMeasureAtMotionPosByScanAxis;// -pGlobalDoc->m_TstCamOffset_Y;	//20160107-syd for AF Modify
	
					sttHeight.valid = TRUE;
					sttHeight.f = pGlobalDoc->m_fPanelThicknessByLaser - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];//pGlobalDoc->m_fPanelThicknessByLaser; // default panel thickness
					sttHeight.h = 0.0;
					sttHeight.d = info.dDistance;
	
					pGlobalView->m_arHeight.Add(sttHeight);
				}
			}
		}
		else
		{
			structHeightInfo sttHeight;	
			int nCount;
			BOOL bValid;
			int n;
			int nBreakPoint;
			double fZoomPos = 0.0;
	#ifdef ZOOM_AXIS
			fZoomPos = GetActualPosition(ZOOM_AXIS);
	#endif
	
			if(AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp && pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor)
			{
					InitInspectCamAutoFocusingWithLaserSensor(fScanVel, 
						fDevideLength,
						(pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0]), 
						//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
						pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
						pGlobalDoc->m_bSaveFocusInfoToROI, 
						pGlobalDoc->m_dFocusAdjustingLimitInUm/1000.0);
	
				if(AoiParam()->m_bGlassAF)
				{
					//레이저 시작위치가 올바르지 않다.
					if(fabs(fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[0]) > AoiParam()->m_dGlassValidRange)
					{
						fCurrThicknessByLaserSensor = pGlobalDoc->m_fRefThicknessAtTestPoint[0];
					}
				}
	
				pGlobalView->m_arHeight.RemoveAll();
			}
	
			if (nScanDir == FORWARD || (pGlobalDoc->m_bUseReverseDirAutoFocusing && !pGlobalDoc->m_bUseUniHeaderLaserBiInsp)) // syd-20110525	//20121125-ndy for 2Head Laser Control
			{
				pGlobalView->m_arHeight.RemoveAll();
	
				nCount = (fabs(fDBNEndPos - fCurPositionY) / (fOptRes*pGlobalDoc->m_fTriggerPulseStep / 1000.0)) + 2.0; //modifiy by khc 2009,07,17	//20120127-ndy for Variable Trigger Pulse
				pGlobalView->m_nMaxRecvCount = min(pGlobalView->m_nMaxRecvCount, nCount);
	
#ifdef _DEBUG
				CString strTitle, strPath, strIdx;
				strPath.Format(_T("%sAFStructure.txt"), pGlobalDoc->m_strSharedDir);
				if (nScanDir == FORWARD)
					strTitle.Format(_T("Forward"));
				else
					strTitle.Format(_T("BackWard"));
#endif
	
				for (n = 0; n < nCount; n++)
				{
					sttHeight.m = fZoomPos; // modify by khc 2008,03,23
					sttHeight.x = fCurPositionX;
	
#if TRIGGER_TYPE == ADLINK_8124 || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
					if (nScanDir == FORWARD)	//20121125-ndy for 2Head Laser Control
						sttHeight.y = d8124StartPos + (n*fDevideLength);	//20160107-syd for AF Modify
					else
						sttHeight.y = d8124StartPos - (n*fDevideLength);	//20160107-syd for AF Modify
#elif TRIGGER_TYPE == NONE_TRIGGER
					;
#else
					if (nScanDir == FORWARD)	//20121125-ndy for 2Head Laser Control
						sttHeight.y = fCurPositionY + (n*fDevideLength);	//20120210-ndy for AF Modify
					else
						sttHeight.y = fCurPositionY - (n*fDevideLength);	//20120210-ndy for AF Modify
#endif
	
					sttHeight.valid = TRUE;
					if (pGlobalDoc->m_bIncludePieceBoundary)
					{
						if (pGlobalView->CheckPieceAreaSpecfied())
						{
							if (n > 0)
							{
								bValid = pGlobalView->CheckLaserSpotInPieceArea(0, sttHeight.x, sttHeight.y);	//20121125-ndy for 2Head Laser Control
								if (!bValid)
									nBreakPoint = 1; // for debug
								sttHeight.valid = bValid;
							}
						}
					}
					sttHeight.f = pGlobalDoc->m_fPanelThicknessByLaser - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];//pGlobalDoc->m_fPanelThicknessByLaser; // default panel thickness
					sttHeight.h = 0.0;
					sttHeight.d = fDevideLength;
					if (nScanDir == FORWARD || (pGlobalDoc->m_bUseReverseDirAutoFocusing && !pGlobalDoc->m_bUseUniHeaderLaserBiInsp)) // syd-20110525	//20121125-ndy for 2Head Laser Control
					{
						pGlobalView->m_arHeight.Add(sttHeight);
					}
	
#ifdef _DEBUG
					CString strData;
					strIdx.Format(_T("Index%03d"), n);
					strData.Format(_T("%.3f, %d"), sttHeight.y, sttHeight.valid);
					::WritePrivateProfileString(strTitle, strIdx, strData, strPath);
#endif
				}
			}
		}
	
	}
	else
	{
		pGlobalView->m_arHeight.RemoveAll();
		pGlobalView->m_nMaxRecvCount = pGlobalView->m_nRecvCntByExtTrig=0;
	}
	
#if TRIGGER_TYPE != ADLINK_8124
	if (GetAirCleaner()->m_bEnableCleanerAction && AoiParam()->m_bUseAirCleanerWhenInspectingPanel)
	{
	
		double d8124AirStart = 0;
		double d8124AirEnd = 0;
	
		double dNewPosY = d8124AirStart - fYDistOfAirSolAndInspectCamera;
		CLAMP_MESSAGE clamp = GetClampObject()->CheckClampOn();
	
		if (clamp == CLAMP_OK)
		{
			if (nScanDir == BACKWARD)
			{
				d8124AirStart = fDBNStartPos - fStartDummyLength - 0.5;
				d8124AirEnd = fDBNEndPos - fStartDummyLength - 0.5;
			}
			else
			{
				d8124AirStart = fDBNStartPos + fStartDummyLength + 0.5;
				d8124AirEnd = fDBNEndPos + fStartDummyLength + 0.5;
			}
		}
		else
		{
			if (nScanDir == BACKWARD)
			{
				d8124AirStart = fDBNStartPos - fStartDummyLength - 0.5 - 15;
				d8124AirEnd = fDBNEndPos - fStartDummyLength - 0.5 + 15;
			}
			else
			{
				d8124AirStart = fDBNStartPos + fStartDummyLength + 0.5 + 15;
				d8124AirEnd = fDBNEndPos + fStartDummyLength + 0.5 - 15;
			}
		}
	
	
		GetAirCleaner()->SetAirCleanerTriggerEvent(nScanDir, fCurPositionY, d8124AirStart, fTgtPos, d8124AirEnd, fScanVel);
	}
#endif
	
	pGlobalView->m_bStWriteTraceAF = TRUE; // 20160202-syd
#ifdef USE_LMI_SENSOR
	if((g_SequenceAF.m_bThicknessMeaurementMode || pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bUseAutoFocusSol) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm)	//090311-ndy
	{
#else
	if ((pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bUseAutoFocusSol) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm)	//090311-ndy
	{
#endif
		BOOL bOk;
//		CString strMsg;
		double fFocusPos;	
		double fLaserFocusOffset = 0;
		double fFocus;
		double dFocusOffset;
		structHeightInfo sttHeight;
	
		if(!pGlobalDoc->m_bUseAutoFocusSol)	//20150507 - syd
		{
			ULONGLONG nStartTick = GetTickCount64();
			if(fCurrThicknessByLaserSensor < -5.0)  // 20100714 ljg del
			{
				if(!pGlobalDoc->m_bUseUniHeaderLaserBiInsp || nScanDir != BACKWARD)	//20150119-ndy for UniLaser BiInspection
				{
					//170313 LGH MOD
					if (nScanDir == FORWARD)	//20121125-ndy for 2Head Laser Control
					{
						if (AoiParam()->m_bUse8124ManualTriggerForVerifyAF && g_bLaserMsgFirst[0] == 1)
						{
							if (!pGlobalView->GetLaserSensorHeightUseTrigger(1, 0, fCurrThicknessByLaserSensor))
							{
								bOk = pGlobalView->GetHeightOfPanelByLaserSensor(1, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime, TRUE);
							}
							else
								bOk = TRUE;
						}
						else
							bOk = pGlobalView->GetHeightOfPanelByLaserSensor(1, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime, TRUE);
	
	
						//bOk = pGlobalView->GetHeightOfPanelByLaserSensor(1, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime);
					}
					else
					{
						if (AoiParam()->m_bUse8124ManualTriggerForVerifyAF && g_bLaserMsgFirst[0] == 1)
						{
							if (!pGlobalView->GetLaserSensorHeightUseTrigger(2, 1, fCurrThicknessByLaserSensor))
							{
								bOk = pGlobalView->GetHeightOfPanelByLaserSensor(2, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime, TRUE);
							}
							else
								bOk = TRUE;
						}
						else
							bOk = pGlobalView->GetHeightOfPanelByLaserSensor(2, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime, TRUE);
	
						//bOk = pGlobalView->GetHeightOfPanelByLaserSensor(2, fCurrThicknessByLaserSensor, pGlobalDoc->m_nLaserSensorAverTime);
					}
				}
	
				if(CFCMap.pData!=NULL && CFCMap.nCnt<CFCMap.EntireNum && CFCMap.bInspecting==TRUE) //똑같은 포인트가 두퉩E나올경퓖E이 구문을 지워야함 
				{
					CFCMap.pData[CFCMap.nCnt].bValid = TRUE;
					CFCMap.pHeightNum[pGlobalDoc->m_nCurrSwathNum] += 1;
					CFCMap.pHeightDir[pGlobalDoc->m_nCurrSwathNum] = nScanDir;
	
					CFCMap.pData[CFCMap.nCnt].PosX = GetActualPosition(CAM_AXIS) + pGlobalDoc->m_fLaserSensorOffsetX[0];
					CFCMap.pData[CFCMap.nCnt].PosY = GetActualPosition(SCAN_AXIS) + pGlobalDoc->m_fLaserSensorOffsetY[0];//131015 LGH 레이픸E오프셋 적퓖E필퓖E
					CFCMap.pData[CFCMap.nCnt].PosZ = fCurrThicknessByLaserSensor;
	#ifdef _DEBUG
					CString strData;
					strData.Format(_T("S0:%d h:%d,%d,%d,%.2f,%.2f,%.2f"),pGlobalDoc->m_nCurrSwathNum,CFCMap.pHeightNum[pGlobalDoc->m_nCurrSwathNum],CFCMap.nCnt,CFCMap.pData[CFCMap.nCnt].bValid ,CFCMap.pData[CFCMap.nCnt].PosX,CFCMap.pData[CFCMap.nCnt].PosY,CFCMap.pData[CFCMap.nCnt].PosZ);
					pGlobalView->ResultDisplayStaticEX(strData,RGB_BLACK,RGB_GREEN);
	#endif
					CFCMap.nCnt++;
				}
	
			}
			ULONGLONG nTick2 = GetTickCount64() - nStartTick;
			if( fCurrThicknessByLaserSensor > -20.0 && fCurrThicknessByLaserSensor < 20.0)
				m_fPrevThicknessByLaserSensor = fCurrThicknessByLaserSensor;
			else
			{
				if(m_fPrevThicknessByLaserSensor > -20.0 && m_fPrevThicknessByLaserSensor < 20.0)
					fCurrThicknessByLaserSensor = m_fPrevThicknessByLaserSensor;
			}
	
			if(AoiParam()->m_bGlassAF)
			{
				//레이저 시작위치가 올바르지 않다.
				if(fabs(fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID]) > AoiParam()->m_dGlassValidRange)
				{
					fCurrThicknessByLaserSensor = pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
				}
			}
	
			if(pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID])
				fLaserFocusOffset = fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
			else
				fLaserFocusOffset = 0.0f;
		}
	
		if(pGlobalDoc->m_bFocusAdjustByTemperature && pGlobalDoc->m_bTempSensorOption)	//090324-ndy
		{
			if(pGlobalDoc->m_bUseAutoFocusSol)	//20150507 - syd
				fFocus = pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature;
			else
				fFocus = pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + fLaserFocusOffset;
	
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S		//20120321-ndy must check because of the focus axis origin position of 50S is not minus position	//20140708-ndy GSM Edit
			if(pGlobalDoc->m_fRefFocusPosAtTestPoint > 0)
#else
			if(pGlobalDoc->m_fRefFocusPosAtTestPoint < 0)
#endif
				pMotionNew->MoveFocusMotor(fFocus);
			else
				pGlobalDoc->m_fRefFocusPosAtTestPoint = SpecData.fFocusPos;
		}
		else
		{
			ULONGLONG nTickTest = GetTickCount64();
	
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S		//20120321-ndy must check because of the focus axis origin position of 50S is not minus position	//20140708-ndy GSM Edit
			if(pGlobalDoc->m_fRefFocusPosAtTestPoint > 0)
#else
			if(pGlobalDoc->m_fRefFocusPosAtTestPoint < 0)
#endif
			{
			//	pMotionNew->CheckAxisDone(FOCUS_AXIS);
	
				double dCurPos = GetActualPosition(FOCUS_AXIS);
				double dDiff = fabs(pGlobalDoc->m_fRefFocusPosAtTestPoint - dCurPos);
	
				if(pGlobalDoc->m_bTesting && (pGlobalDoc->m_nCurrSwathNum % 2) && pGlobalDoc->m_bScanMode==BIDIRECTION && !pGlobalView->m_bPartialInspect)
				{
					// BACKWARD
					if(pGlobalDoc->m_bUseAutoFocusSol)	//20150507 - syd
					{
							pMotionNew->MoveFocusMotor(pGlobalDoc->m_fRefFocusPosAtTestPoint);	//20121125-ndy for 2Head Laser Control
					}
					else
					{
							pMotionNew->MoveFocusMotor(pGlobalDoc->m_fRefFocusPosAtTestPoint + fLaserFocusOffset);
					}//20121125-ndy for 2Head Laser Control
				}
				else
				{
					if(pGlobalDoc->m_bUseAutoFocusSol)	//20150507 - syd
					{
							pMotionNew->MoveFocusMotor(pGlobalDoc->m_fRefFocusPosAtTestPoint);
					}
					else
					{
							pMotionNew->MoveFocusMotor(pGlobalDoc->m_fRefFocusPosAtTestPoint + fLaserFocusOffset);
					}
				}
			}
			else
			{
				pGlobalDoc->m_fRefFocusPosAtTestPoint = SpecData.fFocusPos;
				if(pGlobalDoc->m_bUseAutoFocusSol)	//20150507 - syd
					pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID] = 0.0;
				//else
				//	pGlobalDoc->m_fRefThicknessAtTestPoint = fCurrThicknessByLaserSensor;// 110712 jsy add
			}
	
			ULONGLONG nTickTest2 = GetTickCount64()-nTickTest;
			CString strMsg;
	
			strMsg.Format(_T("FocusMove:%dms"), nTickTest2);
			pMainFrame->DispStatusBar(strMsg);
	
			TRACE(_T("%s\n"), strMsg);
	
			int asd = 0;
		}
	
		fFocusPos = GetCommandPosition(FOCUS_AXIS);
				
m_pMotionCard->InitListMotion();
#ifdef USE_LMI_SENSOR
		if (AoiParam()->m_bUseLMI)
		{
			CString strTime;
			ULONGLONG nPrepare = GetTickCount64();
			ULONGLONG nPrepare2 = GetTickCount64() - nPrepare;
				CGocator* pGoCator = g_lmi.GetHead(0);

				pGoCator->CheckStautsAndReConnect();
				pGoCator->Stop();

				nPrepare2 = GetTickCount64() - nPrepare;
				strTime.Format(_T("Prepare Scan LMI 2 %dms"), nPrepare2);
				SaveLog(strTime);

				g_nCurrentActorType = ACTOR_MEI_SEQUENCE;

				if (nScanDir == FORWARD)
				{
					g_SequenceAF.m_bGrabForward = TRUE;

					//2020824 lgh
					if (pGlobalDoc->m_bUseReverseDirAutoFocusing)
					{
						CGocator* pGoCator2 = g_lmi.GetHead(1);

						if (pGoCator2)
				{
							pGoCator2->CheckStautsAndReConnect();
							pGoCator2->Stop();
				}
				}
					nPrepare2 = GetTickCount64() - nPrepare;
					strTime.Format(_T("Prepare Scan LMI3 %dms"), nPrepare2);
					SaveLog(strTime);
			}
			else
			{
					g_SequenceAF.m_bGrabForward = 0;

					if (pGlobalDoc->m_bUseReverseDirAutoFocusing)
					{
						CGocator* pGoCator2 = g_lmi.GetHead(1);

						if (pGoCator2)
						{
							pGoCator2->CheckStautsAndReConnect();
							pGoCator2->Stop();

							if (g_bLMISurfaceMode == 0)
							{					
								pGoCator2->SyncSensorData();
								pGoCator2->SetEncoderSpacing(g_SequenceAF.m_dEncoderSpacing);
								pGoCator2->SetTriggerSource(GO_TRIGGER_INPUT);        //20200422 add by lhy
								pGoCator2->SetEnableInputTrigger(TRUE);               //20200422 add by lhy
								pGoCator2->SetScanMode(GO_MODE_SURFACE);
								pGoCator2->SetSurfaceGenMode(GO_SURFACE_GENERATION_TYPE_FIXED_LENGTH);
								pGoCator2->SetSurfaceLength(g_SequenceAF.m_dSurfaceLength);
								pGoCator2->SetTravelSpeed(fLineRate*fOptRes);
								pGoCator2->ResetEncoderValue();
	
								g_bLMISurfaceMode = 1;
							}

							pGoCator2->m_pActor = &g_SequenceAF;
						}
					}
				}

				g_SequenceAF.EnableAFThread(0);

				if (nScanDir == FORWARD)
				{
					if (g_bLMISurfaceMode == 0)
					{
						pGoCator->SyncSensorData();

						//20200422 Edit by LHY 
				pGoCator->SetEncoderSpacing(g_SequenceAF.m_dEncoderSpacing);
						//pGoCator->SetTriggerUnit(GO_TRIGGER_UNIT_ENCODER); //20200422 delete by lhy
						//pGoCator->SetTriggerSource(GO_TRIGGER_ENCODER);    //20200422 delete by lhy
						pGoCator->SetTriggerSource(GO_TRIGGER_INPUT);        //20200422 add by lhy
						pGoCator->SetEnableInputTrigger(TRUE);               //20200422 add by lhy
				pGoCator->SetScanMode(GO_MODE_SURFACE);
				pGoCator->SetSurfaceGenMode(GO_SURFACE_GENERATION_TYPE_FIXED_LENGTH);
				pGoCator->SetSurfaceLength(g_SequenceAF.m_dSurfaceLength);
				pGoCator->SetTravelSpeed(fLineRate*fOptRes);
				pGoCator->ResetEncoderValue();
						g_bLMISurfaceMode = 1;
					}

				pGoCator->m_pActor = &g_SequenceAF;
				}

				g_SequenceAF.SetEncoderResetPosition(GetActualPosition(SCAN_AXIS));

				g_SequenceAF.m_dSwathCenterPosX = GetActualPosition(CAM_AXIS);
				g_SequenceAF.m_nSeqIdx = 0;
				g_SequenceAF.m_dBaseFocus = GetActualPosition(FOCUS_AXIS);
				if (nScanDir == FORWARD)
					g_SequenceAF.m_dSequenceStartPosY = fDBNStartPos - 10;
				else
					g_SequenceAF.m_dSequenceStartPosY = fDBNStartPos + 10;


				int nLaserID = nScanDir == FORWARD ? 0 : 1;


				g_SequenceAF.m_dSequenceEndPosY = fDBNEndPos;
				g_SequenceAF.m_dScanSpeed = fLineRate * fOptRes;
#if TRIGGER_TYPE == ADLINK_8124 || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER
				g_SequenceAF.m_fPulseResoltion = pGlobalView->m_pTrig8124->m_dInPlsRes[0] / 1000;
#elif TRIGGER_TYPE == RSA_TRIGGER
				g_SequenceAF.m_fPulseResoltion = GetPositionResolution(SCAN_AXIS);
#endif
				if (!bPreAlign)
				{
					g_SequenceAF.m_bCenterMode = 0;
					g_SequenceAF.m_dCellSizeX = SizeData[0].ProcSizeX*SizeData[0].fPixelSize*0.001;
				}
				else
				{
					g_SequenceAF.m_bCenterMode = 1;
					g_SequenceAF.m_dCellSizeX = g_SequenceAF.m_dProfleWidthForAlignment;
				}


				g_SequenceAF.m_dLaserDistanceFromCamera = (fYDistOfLaserSensorAndInspectCamera);
				g_SequenceAF.m_nReceivedPointNum = 0;
				g_SequenceAF.m_nReceivedPointNumUniHeader = 0;
				g_SequenceAF.m_nSeqNumRevDir = 0;
				g_SequenceAF.m_nSeqNum = 0;
				g_SequenceAF.m_nBufferIndex = 0;
				g_SequenceAF.m_nBufferIndexUniHeader = 0;
				g_SequenceAF.m_dDOF = 0.02;
				g_SequenceAF.ClearQueue();
				g_SequenceAF.EnableAFThread(1);

				g_SequenceAF.m_dReferenceThickness = pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];// = fRefThicknessAtTestPoint;
				g_SequenceAF.m_dRefFocus = pGlobalDoc->m_fRefFocusPosAtTestPoint;// = fRefFocusPosAtTestPoint;

				g_SequenceAF.m_cs.Lock();
				while (!g_SequenceAF.m_Queue.empty()) // initialize stack
					g_SequenceAF.m_Queue.pop();
				g_SequenceAF.m_cs.Unlock();

				g_SequenceAF.m_bAFSequenceEnabled = 1;

				if (nScanDir == FORWARD)
				pGoCator->Start();
				else
				{
					if (g_lmi.m_nHeadCnt > 1)
					{
						CGocator* pGoCator2 = g_lmi.GetHead(1);
						if (pGoCator2)
							pGoCator2->Start();
			}
		}
	
		}

#endif
		if (AoiParam()->m_bUseLMI == 0)
		{
			TRACE("Scan X%f Y%f\n", GetActualPosition(CAM_AXIS), GetActualPosition(SCAN_AXIS));
			if (!pGlobalDoc->m_bUseReverseDirAutoFocusing)
			{
				pGlobalDoc->m_nInspAutoFocusCnt = 1; // 2009,02,13 by khc
	
				dFocusOffset = fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];			//20101102 mod syd
	
				m_dCurrFocusPos = pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature;
	
				// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Write Trace Data, 6. Limit Change Value of Interval Focus Pos [mm]
				InitInspectCamAutoFocusingWithLaserSensor(fScanVel,
					fDevideLength,
					(pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0]),
					//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
					pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
					pGlobalDoc->m_bSaveFocusInfoToROI,
					pGlobalDoc->m_dFocusAdjustingLimitInUm / 1000.0);
	
				sttHeight = pGlobalView->m_arHeight.GetAt(0);
				sttHeight.h = fCurrThicknessByLaserSensor; // Panel thickness...
				pGlobalView->m_arHeight.SetAt(0, sttHeight);
	
	
				strMsg.Format(_T("s.f:%.3f,rf:%.3f,tpf:%.3f,rlh:%.3f,lh:%.3f"), fFocusPos,
					pGlobalDoc->m_fRefFocusPosAtTestPoint,
					pGlobalDoc->m_fFocusPosOffsetByTemperature,
					pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID],
					fCurrThicknessByLaserSensor);
				pMainFrame->DispStatusBar(strMsg, 4);
				if (pGlobalDoc->m_bFocusAdjustByTemperature && pGlobalDoc->m_bTempSensorOption)
				{
					strMsg.Format(_T("rtp:%.1f,tp:%.1f"), pGlobalDoc->m_fRefTemperature, pGlobalDoc->m_fCurrTemperature);
					pMainFrame->DispStatusBar(strMsg, 3);
				}
				strMsg.Format(_T("af.n:%d"), pGlobalDoc->m_nInspAutoFocusCnt);
				pMainFrame->DispStatusBar(strMsg, 1);
	
				if (!m_bEnableRTAF)
				{
					CreateAutoFocusingSeq(TRUE, sttHeight.y,
						dFocusOffset,
						nScanDir,
						fCurrThicknessByLaserSensor,
						pGlobalDoc->m_nCurrSwathNum,0
						); // [mm], [mm] 
	
					//180717 lgh fix 트리거 오프셋을 밀었기 때문에 0번부터 가야된다
					pGlobalView->m_nRecvCntByExtTrig = 0;
					pGlobalDoc->m_nInspAutoFocusCnt = 0;
	
				}
				else
				{
					pGlobalView->m_nRecvCntByExtTrig = 0;
					pGlobalDoc->m_nInspAutoFocusCnt = 0;
	
					int nAFDir = FORWARD;
	
					if (nScanDir != FORWARD)
						nAFDir = BACKWARD;
					CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
	
					pAFModule->m_pRTAF[0]->ModifyRTAFData(m_nCurMotorSwath, pAFModule->m_nRTAFStartIndex, fCurrThicknessByLaserSensor);
				}
	
				TRACE("Initial Data Recv %d InspCnt %d\n", pGlobalView->m_nRecvCntByExtTrig, pGlobalDoc->m_nInspAutoFocusCnt);
			}
			else
			{
				if (m_bEnableRTAF)
				{
					// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Write Trace Data, 6. Limit Change Value of Interval Focus Pos [mm]
					double dOftInspCamLaser;
	
					if (nScanDir == FORWARD)	//20121125-ndy for 2Head Laser Control
						dOftInspCamLaser = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0];
					else
					{
						if (pGlobalDoc->m_bUseUniHeaderLaserBiInsp && nScanDir == BACKWARD)
							dOftInspCamLaser = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0];
						else
							dOftInspCamLaser = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1];
					}
					InitInspectCamAutoFocusingWithLaserSensor(fScanVel,
						fDevideLength,
						dOftInspCamLaser, 	//20121125-ndy for 2Head Laser Control
											//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
						pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
						pGlobalDoc->m_bSaveFocusInfoToROI,
						pGlobalDoc->m_dFocusAdjustingLimitInUm / 1000.0);
	
					strMsg.Format(_T("s.f:%.3f,rf:%.3f,tpf:%.3f,rlh:%.3f,lh:%.3f"), fFocusPos,
						pGlobalDoc->m_fRefFocusPosAtTestPoint,
						pGlobalDoc->m_fFocusPosOffsetByTemperature,
						pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID],
						fCurrThicknessByLaserSensor);
					pMainFrame->DispStatusBar(strMsg, 4);
	
					int nAFDir = FORWARD;
	
					if (nScanDir != FORWARD)
						nAFDir = BACKWARD;
	
					CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
						
					if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)
					{
						if (!CreateBackwardSequenceDataForUniHeader(fCurrThicknessByLaserSensor))
						{
							int jj = 0;
						}
					}
					else
						pAFModule->m_pRTAF[0]->ModifyRTAFData(m_nCurMotorSwath, pAFModule->m_nRTAFStartIndex, fCurrThicknessByLaserSensor);
				}
				else
				{
					pGlobalDoc->m_nInspAutoFocusCnt = 0; // 2009,02,13 by khc
	
					if (AoiParam()->m_bUseUniHeaderReverseAF && nScanDir == BACKWARD && pGlobalDoc->m_bUseUniHeaderLaserBiInsp)
					{
						if (!CreateBackwardSequenceDataForUniHeader(fCurrThicknessByLaserSensor))
						{
							int jj = 0;
						}
					}
					else
					{
						dFocusOffset = fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
	
						m_dCurrFocusPos = pGlobalDoc->m_fRefFocusPosAtTestPoint + fLaserFocusOffset;
	
						// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Write Trace Data, 6. Limit Change Value of Interval Focus Pos [mm]
						double dOftInspCamLaser;
	
						if (nScanDir == FORWARD)	//20121125-ndy for 2Head Laser Control
							dOftInspCamLaser = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0];
						else
						{
							dOftInspCamLaser = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1];
						}
	
						sttHeight = pGlobalView->m_arHeight.GetAt(0);
						sttHeight.h = fCurrThicknessByLaserSensor; // Panel thickness...
						pGlobalView->m_arHeight.SetAt(0, sttHeight);
							
						InitInspectCamAutoFocusingWithLaserSensor(fScanVel,
							fDevideLength,
							dOftInspCamLaser, 	//20121125-ndy for 2Head Laser Control
							//pGlobalDoc->m_fRefFocusPosAtTestPoint + pGlobalDoc->m_fFocusPosOffsetByTemperature + dFocusOffset,// + fLaserFocusOffset,	//090317-ndy
							pGlobalDoc->m_fRefFocusPosAtTestPoint, // 20160211-syd//20101102 mod syd
							pGlobalDoc->m_bSaveFocusInfoToROI,
							pGlobalDoc->m_dFocusAdjustingLimitInUm / 1000.0);
	
						strMsg.Format(_T("s.f:%.3f,rf:%.3f,tpf:%.3f,rlh:%.3f,lh:%.3f"), fFocusPos,
							pGlobalDoc->m_fRefFocusPosAtTestPoint,
							pGlobalDoc->m_fFocusPosOffsetByTemperature,
							pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID],
							fCurrThicknessByLaserSensor);
						pMainFrame->DispStatusBar(strMsg, 4);
						if (pGlobalDoc->m_bFocusAdjustByTemperature && pGlobalDoc->m_bTempSensorOption)
						{
							strMsg.Format(_T("rtp:%.1f,tp:%.1f"), pGlobalDoc->m_fRefTemperature, pGlobalDoc->m_fCurrTemperature);
							pMainFrame->DispStatusBar(strMsg, 3);
						}
						strMsg.Format(_T("af.n:%d"), pGlobalDoc->m_nInspAutoFocusCnt);
						pMainFrame->DispStatusBar(strMsg, 1);
	
						double fCurPos;
						if (nScanDir == BACKWARD)
							fCurPos = sttHeight.y;
						else
							fCurPos = sttHeight.y;
	
						//160727 lgh fix 처음 위치에서는 포커스 오프셋이 무조건 0일텐데..?
						CreateAutoFocusingSeq(TRUE, fCurPos,
							dFocusOffset,
							nScanDir,
							fCurrThicknessByLaserSensor,
							pGlobalDoc->m_nCurrSwathNum,0
							); // [mm], [mm]
	
						//180717 lgh fix
						pGlobalView->m_nRecvCntByExtTrig = 0;//160727 LGH TEST
						pGlobalDoc->m_nInspAutoFocusCnt = 0;
					}
				}
			}
		}
	}
	else
	{
//		CString strMsg;
		double fFocusPos;
		fFocusPos = GetActualPosition(FOCUS_AXIS);
		strMsg.Format(_T("s.f:%.3f"),fFocusPos);
		pMainFrame->DispStatusBar(strMsg, 4);
		//m_pMotionCard->m_bStartAfSeq = FALSE;
	}
	
	if(pGlobalDoc->m_bLaserSensorOption && pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor && pGlobalDoc->m_bSaveFocusInfoToROI)
	{
		for(int nLaserChkNum = 0; nLaserChkNum<MAX_LASER_SENSOR_CHECK_NUM; nLaserChkNum++)
		{
			pGlobalDoc->m_fTraceActualPosScanAxis[nLaserChkNum] = 0.0;
			pGlobalDoc->m_fTraceActualPosFocusAxis[nLaserChkNum] = 0.0;
		}
		pGlobalDoc->m_nTraceActualPosCnt = 0;
		pGlobalDoc->m_bTraceActualTimerAct = TRUE;
		pGlobalView->SetTimer(FOCUS_MOTION_CHECK_TIMER, 1, NULL);
	}
	
	pGlobalView->m_bLaserExtTrigger=TRUE;	
		
#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED,ON);
#endif
	BOOL bError = SUCCESS;

	if (m_pMotionCard->IsListMotion())
	{
		double dStartScanPos = GetCommandPosition(SCAN_AXIS);
		double dStartFocusPos = GetCommandPosition(FOCUS_AXIS);
		m_pMotionCard->RestoreStartListMotionPos(dStartScanPos, dStartFocusPos);

		dStartScanPos += fabs(fYDistOfLaserSensorAndInspectCamera) - fDevideLength - pGlobalDoc->m_fDummyLine - pGlobalDoc->m_fSettleLen;
		dStartFocusPos += fCurrThicknessByLaserSensor - pGlobalDoc->m_fRefThicknessAtTestPoint[nLaserID];
		m_pMotionCard->AddList(dStartScanPos, dStartFocusPos, fScanVel);	// for Moving Scan Axis to dStartScanPos...
		m_pMotionCard->StartListMotion();									

#ifdef _DEBUG
		CString strPath;
		strPath.Format(_T("%sAddList.txt"), pGlobalDoc->m_strSharedDir);
		if (nScanDir == FORWARD)
			m_pMotionCard->m_sAddListLogTitle.Format(_T("Forward"));
		else
			m_pMotionCard->m_sAddListLogTitle.Format(_T("BackWard"));

		CFileFind finder;
		CFile file;
		if (finder.FindFile(strPath))
		{
			DeleteFile(strPath);
		}

		if (!file.Open(strPath, CFile::modeCreate | CFile::modeWrite, nullptr))
		{
			m_pMotionCard->m_bLogAddList = FALSE;
			AfxMessageBox(_T("Fail to create file."));
		}
		else
		{
			m_pMotionCard->m_bLogAddList = TRUE;
			file.Close();
		}

		if (m_pMotionCard->m_bLogAddList)
		{
			CString strData, strIdx;
			strIdx.Format(_T("Index%03d"), m_pMotionCard->GetAddListNum() - 1);
			strData.Format(_T("%.3f, %.3f, %.3f"), dStartScanPos, dStartFocusPos, fScanVel);
			::WritePrivateProfileString(m_pMotionCard->m_sAddListLogTitle, strIdx, strData, strPath);
		}
#endif
	}
	else
	{
		BOOL bError = StartPosMove(SCAN_AXIS,
									fTgtPos, // syd-test +100.0
									fScanVel,
									fScanAcc,
									fScanDec,
									ABS,
									NO_WAIT,
									S_CURVE,//MPIMotionTypeS_CURVE
									NO_OPTIMIZED);
	}

	if (bError == SUCCESS)
		return TRUE;
	else
		return FALSE;
}

void CMotionNew::RestoreStartListMotionPos(double dStartScanPos, double dStartFocusPos)
{
	m_pMotionCard->RestoreStartListMotionPos(dStartScanPos, dStartFocusPos);
}

void CMotionNew::StartListMotion()
{
	m_pMotionCard->StartListMotion();
	return;
}

void CMotionNew::StopListMotion()
{
	m_pMotionCard->StopListMotion();
	return;
}

void CMotionNew::LoggingAddList(double dStartScanPos, double dStartFocusPos, double fScanVel, int nScanDir, CString sPath)
{
	m_pMotionCard->LoggingAddList(dStartScanPos, dStartFocusPos, fScanVel, nScanDir, sPath);
}

BOOL CMotionNew::HoldingStartScan(double fHoldingPosX, double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir, long nStartDummy, long nEndDummy, double fCurrThicknessByLaserSensor)
{
	m_stScanParam[1].bHoldAction = TRUE;
	m_stScanParam[1].nScanMode = SCAN_MODE_HOLD;
	m_stScanParam[1].bScanPosMove = 0;
	m_stScanParam[1].dHoldingPosX = fHoldingPosX;
	m_stScanParam[1].dNextSwathStartPosY = 0;

	return StartScan(0, fLineRate, fOptRes, fScanSize, fImageSize, nScanDir, nStartDummy, nEndDummy, fCurrThicknessByLaserSensor, &m_stScanParam[1]);
}

void CMotionNew::ChangeMagMotorNegSWLimit(double fTgtFocusPos)
{
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	#ifdef MAG_AXIS
		// 변경된 포커스 모터의 위치를 반영하여 배율모터의 negative software limit위치를 수정한다.
		double fOffset = fTgtFocusPos - m_pParamMotor[FOCUS_AXIS].fNegLimit;
		double fNewSWNegLimit = m_pParamMotor[MAG_AXIS].fNegLimit - fOffset;
		
		m_pParamMotor[MAG_AXIS].fNegLimit = fNewSWNegLimit;
		//m_pMotionCard->ChangeNegSWLimitValue(MAG_AXIS, fNewSWNegLimit);
		INT nAct = (INT)E_STOP_EVENT;
		m_fNegLimit[MAG_AXIS] = fNewSWNegLimit;
		m_pMotionCard->GetAxis(MAG_AXIS)->SetNegSoftwareLimit(fNewSWNegLimit, nAct);
	#endif
	#endif
}

BOOL CMotionNew::WaitXYPosEnd(BOOL bMotionDone, DWORD dwTimeOver, BOOL bPumpMsg)
{
	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh)
	{

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (g_bUseTableLock && pIO->ReadBit(LD_SHK_TABLE_LOCK))
			{
#ifdef MOTION_ACTION
				StopMove(SCAN_AXIS, 0.01, 0);
#endif
				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
				if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
					SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
				else if (pGlobalDoc->m_nSelectedLanguage == JAPANESE)
					SetMainMessage(_T("テ?ブルロック?態です。ロ?ダ?/アンロ?ダ?移動した後、再試行してください"));
				else
					SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

				return MEI_MOTION_ERROR;
			}
		}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif

		if (bMotionDone == CHECK_MOTION_DONE)
		{
#ifdef LIGHT_AXIS
			if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS) && CheckMotionDone(LIGHT_AXIS))
			{
				nRet = SUCCESS;
				break;
			}
#else
			if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(CAM_AXIS))
			{
				nRet = SUCCESS;
				break;
			}
#endif

			if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
			{
				if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
				{
					if (pGlobalView->m_bCheckSafetyOnlyBit)
					{
						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
						}
						nRet = OPERATE_INTERLOCK;
					}
					else
					{
						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
						}
						nRet = MEI_MOTION_ERROR;
					}
					break;
				}
			}
			else
			{
				if ((GetTickCount() - dwStartTick) >= dwTimeOver)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
					}

					nRet = WAIT_TIME_OVER;
					break;
				}
			}

		}
		else if (bMotionDone == CHECK_AXIS_DONE)
		{
#ifdef LIGHT_AXIS
			if (CheckAxisDone(SCAN_AXIS) && CheckAxisDone(CAM_AXIS) && CheckAxisDone(LIGHT_AXIS))
			{
				nRet = SUCCESS;
				break;
			}
#else
			if (CheckAxisDone(SCAN_AXIS) && CheckAxisDone(CAM_AXIS))
			{
				nRet = SUCCESS;
				break;
			}
#endif

			if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
			{
				if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
				{
					if (pGlobalView->m_bCheckSafetyOnlyBit)
					{
						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
						}
						nRet = OPERATE_INTERLOCK;
					}
					else
					{
						if (pGlobalDoc->m_bUseInlineAutomation)
						{
							if (CController::m_pController)
								CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
						}
						nRet = MEI_MOTION_ERROR;
					}
					break;
				}
			}
			else
			{
				if ((GetTickCount() - dwStartTick) >= dwTimeOver)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
					}

					nRet = WAIT_TIME_OVER;
					break;
				}
			}

		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
		{
			nRet = USER_STOP;
			break;
		}
		else if (pIO->CheckEmgSwitch())
		{
			nRet = EMERGENCY_STOP;
			break;
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			nRet = (OPERATE_INTERLOCK);
			break;
		}
		else if (GetAxisState(SCAN_AXIS) == MPIStateERROR || GetAxisState(CAM_AXIS) == MPIStateERROR)
		{
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				if (pGlobalView->m_bCheckSafetyOnlyBit)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
					}
					nRet = OPERATE_INTERLOCK;
				}
				else
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						if (CController::m_pController)
							CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
					}
					nRet = MEI_MOTION_ERROR;
				}
				break;
			}
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					if (CController::m_pController)
						CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
				}

				nRet = WAIT_TIME_OVER;
				break;
			}
		}

		if (bPumpMsg == 1)
			pMainFrame->RefreshWindows(1);  // 20100506 ljg chg

		Sleep(1);
	}

#endif	
	return nRet;
}

//160212 LGH ADD WAIT FLAG
BOOL CMotionNew::ThetaAdjust(double fAdjAngle, double fMaxAngle, double *fRetThetaPos, BOOL bWait)
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI

#ifdef THETA_AXIS
	if (!m_bOrigin[THETA_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		TCHAR szData[200];
		CString strMsg;
		if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
			strMsg.Format(_T("Theta %s"), szData);
		else
			strMsg.Format(_T("[MSG305] Theta ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}

	double fCurPos = GetActualPosition(THETA_AXIS);
	double fAdjLength = pGlobalDoc->m_fDistanceTableCenterAndThetaMotor*sin(fAdjAngle*((double)atan(1.0)*4.0 / 180.0)); // modify by khc 20080909
	double fTgtPos = fCurPos - fAdjLength;
	fTgtPos = min(max(fTgtPos, m_pParamMotor[THETA_AXIS].fNegLimit), m_pParamMotor[THETA_AXIS].fPosLimit);
	double fModifiedAngle = asin((fCurPos - fTgtPos) / pGlobalDoc->m_fDistanceTableCenterAndThetaMotor) / ((double)atan(1.0)*4.0 / 180.0);
	if (fabs(fAdjAngle - fModifiedAngle) > fabs(fMaxAngle))
	{
		CString strMsgL;
		strMsgL.Format(_T("[MSG315] ThetaAdjust() Failed! Angle:%.3f,ModAngle:%.3f,RefAngle:%.3f"), fAdjAngle, fModifiedAngle, fMaxAngle);
		pMainFrame->DispStatusBar(strMsgL);
		//AfxMessageBox(strMsgL);
		SetMainMessage(strMsgL);
		SaveLog(strMsgL);
		return FALSE;
	}

	*fRetThetaPos = fTgtPos; // 불량위치 확인시 Theta 위치 설정
	short ret;
	if (fTgtPos != m_pParamMotor[THETA_AXIS].fNegLimit && fTgtPos != m_pParamMotor[THETA_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(THETA_AXIS, fTgtPos, 50.0, ABS, bWait, OPTIMIZED);//161004lgh fix no_optimized->optimized
	else
		AfxMessageBox(_T("Can't move position to limit. (THETA_AXIS)"));
#endif


#ifdef SCAN_S_AXIS
	if (!m_bOrigin[SCAN_S_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		TCHAR szData[200];
		CString strMsg;
		if (0 < ::GetPrivateProfileString(_T("MOTION"), _T("ORIGIN_RETURN_FAIL"), NULL, szData, sizeof(szData), pGlobalDoc->m_strLanguageTextPath))  // 20080429 ljg
			strMsg.Format(_T("Theta %s"), szData);
		else
			strMsg.Format(_T("[MSG305] Theta ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}

	double fCurPos = GetActualPosition(SCAN_S_AXIS);
	double fAdjLength = pGlobalDoc->m_fDistanceScanAndScanS*sin((fAdjAngle / 2.0)*(atan(1.0) * 4 / 180.0)); // modify by khc 20080909
	double fTgtPos = fCurPos - fAdjLength;
	fTgtPos = min(max(fTgtPos, m_pParamMotor[SCAN_S_AXIS].fNegLimit), m_pParamMotor[SCAN_S_AXIS].fPosLimit);
	double fModifiedAngle = asin((fCurPos - fTgtPos) / (pGlobalDoc->m_fDistanceScanAndScanS / 2.0)) / (atan(1.0) * 4 / 180.0);
	if (fabs(fAdjAngle - fModifiedAngle) > fabs(fMaxAngle))
	{
		CString strMsgL;
		strMsgL.Format(_T("[MSG315] ThetaAdjust() Failed! Angle:%.3f,ModAngle:%.3f,RefAngle:%.3f"), fAdjAngle, fModifiedAngle, fMaxAngle);
		pMainFrame->DispStatusBar(strMsgL);
		AfxMessageBox(strMsgL);
		return FALSE;
	}

	*fRetThetaPos = fTgtPos - pGlobalDoc->m_dReferenceThetaPosition; // 불량위치 확인시 Theta 위치 설정
	short ret;
	if (fTgtPos != m_pParamMotor[SCAN_S_AXIS].fNegLimit && fTgtPos != m_pParamMotor[SCAN_S_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(SCAN_S_AXIS, fTgtPos, 50.0, ABS, bWait, OPTIMIZED);//161004lgh fix no_optimized->optimized
	else
		AfxMessageBox(_T("Can't move position to limit. (SCAN_S_AXIS)"));
#endif

#endif

	return TRUE;
}

BOOL CMotionNew::WaitMotorPosEnd(int nAxisId, BOOL bMotionDone, DWORD dwTimeOver, BOOL bMsgPump)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	g_bLoopWh = TRUE; // syd-20100629

	double dActPrev = 0;
	ULONGLONG nTimeTick = GetTickCount64();
	while (g_bLoopWh)
	{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pGlobalView->m_bCallFaultRestore)
			{
				return MEI_MOTION_ERROR;
			}
		}

		if (bMotionDone == CHECK_MOTION_DONE)
		{
			if (CheckMotionDone(nAxisId))
			{
				nRet = SUCCESS;
				break;
			}
			else
			{
				if (GetAxisState(nAxisId) == MPIStateERROR)
				{
					if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
					{
						if (pGlobalView->m_bCheckSafetyOnlyBit)
						{
							if (pGlobalDoc->m_bUseInlineAutomation)
							{
								if (CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
							}
							nRet = OPERATE_INTERLOCK;
						}
						else
						{
							if (pGlobalDoc->m_bUseInlineAutomation)
							{
								if (CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
							}
							nRet = MEI_MOTION_ERROR;
						}
						break;
					}
					else
					{

					}
				}

			}
		}
		else if (bMotionDone == CHECK_AXIS_DONE)
		{
			if (CheckAxisDone(nAxisId))
			{
				nRet = SUCCESS;
				break;
			}
			else
			{
				if (GetAxisState(nAxisId) == MPIStateERROR)
				{
					if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
					{
						if (pGlobalView->m_bCheckSafetyOnlyBit)
						{
							if (pGlobalDoc->m_bUseInlineAutomation)
							{
								if (CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
							}
							nRet = OPERATE_INTERLOCK;
						}
						else
						{
							if (pGlobalDoc->m_bUseInlineAutomation)
							{
								if (CController::m_pController)
									CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
							}
							nRet = MEI_MOTION_ERROR;
						}
						break;
					}
				}
			}
		}

		if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
		{
			nRet = USER_STOP;
			break;
		}

		else if (pIO->CheckSaftyOnlyBit())
		{
			nRet = OPERATE_INTERLOCK;
			break;
		}
		else if (pIO->CheckIBIDoorStatusNoMessage() == 0)
		{
			SetMainMessage(_T("Door open detection"));
			nRet = OPERATE_INTERLOCK;
			break;
		}
		else if (pIO->CheckEmgSwitch())
		{
			nRet = EMERGENCY_STOP;
			break;
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			nRet = (OPERATE_INTERLOCK);
			break;
		}
		else if (GetAxisState(nAxisId) == MPIStateERROR)
		{
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				nRet = MEI_MOTION_ERROR;
				break;
			}
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				nRet = WAIT_TIME_OVER;
				break;
			}

#ifdef USE_ZMP

			if (nAxisId != CAM_AXIS && nAxisId != SCAN_AXIS)
			{
				double dActPos = GetActualPosition(nAxisId);
				double dCmdPos = GetCommandPosition(nAxisId);

				double dActPulse = LengthToPulse(nAxisId, dActPos);//  
				double dCmdPulse = LengthToPulse(nAxisId, dCmdPos);//

				double dInspRangePulse = LengthToPulse(nAxisId, m_pParamAxis[nAxisId].fInpRange);

				if (fabs(dActPulse - dActPrev) >= dInspRangePulse)
				{
					dActPrev = dActPulse;
					nTimeTick = GetTickCount64();
				}
				else
				{
					if (GetTickCount64() - nTimeTick >= 10000)
					{
						if (GetAxisState(nAxisId) == MPIStateMOVING && GetVelocity(nAxisId) == 0.0)
						{
							double dActPulse = LengthToPulse(nAxisId, dActPos); // [mm/sec] -> [counts/sec]
							double dCmdPulse = LengthToPulse(nAxisId, dCmdPos); // [mm/sec] -> [counts/sec]

							Clear(nAxisId);

							SetCommandPosition(nAxisId, dActPos);
							SetActualPosition(nAxisId, dCmdPos);

							Clear(nAxisId);

							//포지션 에러가 나는 경우 일단 위치 동기화를  시켜 놓는다.

							if (pGlobalDoc->m_bUseInlineAutomation)
								CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);

							CString strError;

							strError.Format(_T("Position Error Axis %s A:%3f C:%.3f"), GetMotorName(nAxisId), dActPos, dCmdPos);
							SetMainMessage(strError);

							return FALSE;

						}
					}
				}
			}
#endif
		}

		if (bMsgPump)
			pMainFrame->RefreshWindows(10);
	}
#endif	
	return nRet;
}

long CMotionNew::LengthToPulse(int nAxisId, double fLength)
{
	if (nAxisId >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	//return m_pMotionCard->Len2Pulse(nAxisId, fLength);
	return m_pMotionCard->GetAxis(nAxisId)->LenToPulse(fLength);
}

double CMotionNew::PulseToLength(int nAxisId, long nPulse)
{
	if (nAxisId >= MAX_AXIS)
		return 0.0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	double fPulse = (double)nPulse;
	//return m_pMotionCard->Pulse2Len(nAxisId, nPulse);
	return m_pMotionCard->GetAxis(nAxisId)->PulseToLen(fPulse);
}

void CMotionNew::ShutdownHomeThreadAll()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif

	if (m_pMotionCard)
		m_pMotionCard->ShutdownHomeThreadAll();
}

BOOL CMotionNew::SearchHomePos(int nAxisId, BOOL bThread)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return 0;
		}
	#endif
	
	#ifdef NO_MAG_AXIS	//20120308-ndy add
		#ifdef MAG_AXIS
		if(nAxisId == MAG_AXIS)
			return TRUE;
		#endif
	#endif
		
	#ifdef USE_ATF_MODULE
		if(nAxisId == FOCUS_AXIS)
		{
			if(pGlobalView->m_bATFConnect)
			{
				m_nAtfHomingError = 0;
				m_nAtfHomingError = atf_RunHomingZ(0);
	
				if(!bThread)
				{
					if(WaitHomeEndFocus()==SUCCESS)
						return TRUE;
					else
						return FALSE;
				}
			}
			else
			{
				AfxMessageBox(_T("[MSG294] ATF Connect Error!"));
			}
			return TRUE;
		}
	#endif
	//20200729_KSJ_2in1_추가
#if VERIFY_OPTICAL == USE_2_IN_1_MODULE
	if (nAxisId == VERIFY_AXIS_ATF)
	{
		if (pGlobalView->m_bATFConnect)
		{
			m_nAtfHomingError = 0;
			m_nAtfHomingError = atf_RunHomingZ(0);

			//20201228 KSJ MOD - atf_MoveZ(-36200)추가
			if (!bThread)
			{
				if (WaitHomeEndFocus() == SUCCESS)
				{
					atf_MoveZ(-36200);
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("ATF Connect Error!"));
		}
		return TRUE;
	}
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		
		if(!m_pParamMotion[nAxisId].Home.bAct)
			return TRUE;
		
	//if(!IsServoOn(nAxisId)) // 20180413-syd : Modified for Fastech synqnet servo pack.
	//	return FALSE;
		
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSMART_ULTRA || MACHINE_MODEL == INSPRO_PTH
		if(nAxisId == SCAN_AXIS)
		{
#ifdef USE_ZMP
			SetCommandPosition(SCAN_AXIS, 0.0);
			SetActualPosition(SCAN_AXIS, 0.0);
#endif
			
			if(!SearchThetaReadyPosOfGm())
				return FALSE;
		}
	#endif
		
		m_bOrigin[nAxisId] = FALSE;

	BOOL bRtn = FALSE;
	m_bOrigin[nAxisId] = FALSE;

#ifndef USE_NMC_HOME
	if (bThread)
	{
		m_bOrigin[nAxisId] = FALSE;
		m_pMotionCard->m_bOrigin[nAxisId] = FALSE;
		bRtn = m_pMotionCard->GetAxis(nAxisId)->StartHomeThread();
	}
	else
	{
		bRtn = m_pMotionCard->GetAxis(nAxisId)->StartHomming();
		if (bRtn)
		{
			m_bOrigin[nAxisId] = TRUE;
			m_pMotionCard->GetAxis(nAxisId)->SetOriginStatus(TRUE);
		}
		else
			m_pMotionCard->GetAxis(nAxisId)->SetOriginStatus(FALSE);
	}
#else
	int nDir = m_pParamMotion[nAxisId].Home.nDir == HOMING_DIR_CW ? HOMING_DIR_CW : HOMING_DIR_CCW;
	int mc = m_pMotionCard->StartRsaHoming(nAxisId, !bThread, HOMING_LIMIT_SWITCH, nDir);
	bRtn = (mc == MC_OK) ? TRUE : FALSE;

	if(!bThread && bRtn)
		m_bOrigin[nAxisId] = TRUE;
#endif

	return bRtn;

#endif
	return TRUE;
}

BOOL CMotionNew::GetThetaLimitInputForGm(BOOL bDir)
{
	BOOL bOnOff = TRUE;

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO_PTH
	if (bDir == PLUS)
	{
		bOnOff = !pIO->ReadBit(THETA_LIMIT_POS);
	}
	else
	{
		bOnOff = !pIO->ReadBit(THETA_LIMIT_NEG);
	}
#endif	

#if MACHINE_MODEL == INSPRO50S	|| MACHINE_MODEL == INSMART_ULTRA
	if(bDir==PLUS)
	{
		bOnOff = !pIO->ReadBit(THETA_LIMIT_POS);
	}
	else
	{
		bOnOff = !pIO->ReadBit(THETA_LIMIT_NEG);
	}

	//if (bDir == PLUS)
	//{
	//	if(m_pParamMotor[SCAN_S_AXIS].bPosLimitLevel)
	//		bOnOff = pIO->ReadBit(THETA_LIMIT_POS);
	//	else
	//		bOnOff = !pIO->ReadBit(THETA_LIMIT_POS);
	//}
	//else
	//{
	//	if (m_pParamMotor[SCAN_S_AXIS].bNegLimitLevel)
	//		bOnOff = pIO->ReadBit(THETA_LIMIT_NEG);
	//	else
	//		bOnOff = !pIO->ReadBit(THETA_LIMIT_NEG);
	//}
#endif

	return bOnOff;
}

BOOL CMotionNew::SearchThetaReadyPosOfGm()
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH
#ifdef SCAN_S_AXIS

	if (!IsServoOn(SCAN_S_AXIS))
		return FALSE;

	short val = MPIMessageOK;

	long lOnOff;
	CString strMsg;

	if (!GetGantry(SCAN_AXIS, SCAN_S_AXIS, &lOnOff))
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Get. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		pGlobalView->DispMessage(_T("Initialize Gantry Motion Failed"), strMsg, RGB(255, 0, 0));
		pGlobalView->WaitDispMsgClear();
		return FALSE;
	}

	if (!lOnOff) // Gantry Mode
	{
		if (!SetGantry(SCAN_AXIS, SCAN_S_AXIS, TRUE))
		{
			strMsg.Format(_T("[MSG94] Failed Gantry Set. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
			pGlobalView->DispMessage(_T("Initialize Gantry Motion Failed"), strMsg, RGB(255, 0, 0));
			pGlobalView->WaitDispMsgClear();
			return FALSE;
		}
	}

	if (!GetGantry(SCAN_AXIS, SCAN_S_AXIS, &lOnOff))
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Get. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		pGlobalView->DispMessage(_T("Initialize Gantry Motion Failed"), strMsg, RGB(255, 0, 0));
		pGlobalView->WaitDispMsgClear();
		return FALSE;
	}

	if (!lOnOff)
	{
		strMsg.Format(_T("[MSG94] Failed Gantry Set. - %s axis(master) and %s axis(slaver)."), GetMotorName(SCAN_AXIS), GetMotorName(SCAN_S_AXIS));
		pGlobalView->DispMessage(_T("Initialize Gantry Motion Failed"), strMsg, RGB(255, 0, 0));
		pGlobalView->WaitDispMsgClear();
		return FALSE;
	}

	if (GetLimitInput(SCAN_AXIS, FALSE) || GetLimitInput(SCAN_S_AXIS, FALSE)
		|| GetOrgInput(SCAN_AXIS) || GetOrgInput(SCAN_S_AXIS))
	{
		Sleep(100);
		StartPosMove(SCAN_AXIS, 50.0, 10.0, ABS, WAIT, OPTIMIZED);
		Sleep(100);
	}

	if (!_SearchThetaReadyPosOfGm())
		return FALSE;

#endif
#endif

	return TRUE;
}

int CMotionNew::_SearchThetaReadyPosOfGm()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef SCAN_S_AXIS

	int errCode = 0;

	BOOL bLimitEvent = FALSE; //2007,08,12 khc

	if (!IsServoOn(SCAN_S_AXIS))
		return FALSE;

	CString strTitleMsg, strMsg;

	strTitleMsg.Format(_T("%s %s"), GetMotorName(SCAN_S_AXIS), pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "MOTOR_POSITION_INITIALIZE"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	ClearMotionFault(SCAN_S_AXIS);
	while (!IsMotionDone(SCAN_S_AXIS))
	{
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}

	// Software Limit값 임시 퇴피.
	double fPrevPosLimit = m_pParamMotor[SCAN_S_AXIS].fPosLimit;
	double fPrevNegLimit = m_pParamMotor[SCAN_S_AXIS].fNegLimit;

	// software limit value 최큱Eだ막?변컖E
	ChangePosSWLimitValue(SCAN_S_AXIS, 10000.0);
	ChangeNegSWLimitValue(SCAN_S_AXIS, -10000.0);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionNONE);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionNONE);
	INT nAct = (INT)NO_EVENT;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);

	//0. -축을 제외한 다른위치에 모타가 있을때 
	// -방향으로 움직이면서 일단 -Limit를 찾음 
	// Move to the negative limit : 
	// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
	// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
	//	float fEStopTime = m_pMotionCard->GetEStopTime(SCAN_S_AXIS);
	double fSpeed, fMachineSpeed, fENewStopTime;
	double fTemp = (double)m_pParamMotor[SCAN_S_AXIS].fLeadPitch / (double)m_pParamMotor[SCAN_S_AXIS].nEncPulse;
	if (fTemp <= 0.001)
	{
		//		fSpeed = (1.0*1000.0)/(m_fPosRes*1000); // [mm/s]
		fSpeed = (1.0*1000.0) / (GetPositionResolution(SCAN_S_AXIS)*1000.0); // [mm/s]
		fMachineSpeed = PulseToLength(SCAN_S_AXIS, (long)fSpeed) / 2.0;
		fENewStopTime = float(fMachineSpeed / 1000.0);
	}
	else
	{
		fMachineSpeed = (GetPositionResolution(SCAN_S_AXIS)*1000.0) / 2.0; // [mm/s]
		fENewStopTime = float(fMachineSpeed / 1000.0);
	}


	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fENewStopTime);

	//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉救다.
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP1_ESCAPE_ORIGIN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	if (GetThetaLimitInputForGm(m_pParamMotion[SCAN_S_AXIS].Home.nDir))
	{
		ClearMotionFault(SCAN_S_AXIS);
		StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);
		do {
			if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == PLUS)
				bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
			else
				bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

			if (!bLimitEvent)
			{
				//Stop(SCAN_AXIS);
				EStop(SCAN_AXIS);
				Sleep(200);
				//StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, GetCommandPosition(SCAN_S_AXIS));
				GantryEnable(SCAN_AXIS, SCAN_S_AXIS, FALSE);
				Sleep(200);
				GantryEnable(SCAN_AXIS, SCAN_S_AXIS, TRUE);
				Sleep(200);
			}

			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
			Sleep(1);
		} while (bLimitEvent);

		ClearMotionFault(SCAN_S_AXIS);
		Sleep(50);
	}

	//2. -축을 제외한 다른위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP2_MOVE_ORIGIN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);

	bLimitEvent = FALSE;
	while (!bLimitEvent)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_AXIS);
			//MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(200);
			//Sleep(1000);
			//StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, GetCommandPosition(SCAN_S_AXIS));
			GantryEnable(SCAN_AXIS, SCAN_S_AXIS, FALSE);
			Sleep(200);
			GantryEnable(SCAN_AXIS, SCAN_S_AXIS, TRUE);
			Sleep(200);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}

	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	ChangePosSWLimitValue(SCAN_S_AXIS, fPrevPosLimit);
	ChangeNegSWLimitValue(SCAN_S_AXIS, fPrevNegLimit);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	//m_pMotionCard->SetMsAction(SCAN_S_AXIS, MPIActionRESET);
	nAct = (INT)E_STOP_EVENT;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);

#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
	//m_pMotionCard->SetPosSWLimitDirection(SCAN_S_AXIS, TRUE);
	//m_pMotionCard->SetNegSWLimitDirection(SCAN_S_AXIS, TRUE);
#endif

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_ORIGIN_RETURN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

#endif

	return TRUE;
}

BOOL CMotionNew::IsServoOn(int nMotorID)
{
	if (nMotorID >= MAX_AXIS)
		return FALSE;

	//return TRUE;

	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return 0;
		}
	#endif
	
		//return (m_pMotionCard->IsServoOn(nMotorID));
		return m_pMotionCard->GetAxis(nMotorID)->GetAmpEnable();
}

BOOL CMotionNew::ThetaOriginForGm()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION
#ifdef SCAN_S_AXIS
	int errCode = 0;
	INT nAct;

	BOOL bLimitEvent = FALSE; //2007,08,12 khc

	if (!IsServoOn(SCAN_S_AXIS))
		return FALSE;

	m_bOrigin[SCAN_S_AXIS] = FALSE;

	CString strTitleMsg, strMsg;

	strTitleMsg.Format(_T("%s %s"), GetMotorName(SCAN_S_AXIS), pGlobalView->GetLanguageString("MOTION", "ORIGIN_SEARCH"));
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "MOTOR_POSITION_INITIALIZE"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	ClearMotionFault(SCAN_S_AXIS);
	while (!IsMotionDone(SCAN_S_AXIS))
	{
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}

	// Software Limit값 임시 퇴피.
	double fPrevPosLimit = m_pParamMotor[SCAN_S_AXIS].fPosLimit;
	double fPrevNegLimit = m_pParamMotor[SCAN_S_AXIS].fNegLimit;

	// software limit value 최큱Eだ막?변컖E
	ChangePosSWLimitValue(SCAN_S_AXIS, 10000.0);
	ChangeNegSWLimitValue(SCAN_S_AXIS, -10000.0);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionNONE);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionNONE);

	nAct = (INT)NO_EVENT;

	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);

	//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉救다.
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP1_ESCAPE_ORIGIN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	if (GetThetaLimitInputForGm((BOOL)m_pParamMotion[SCAN_S_AXIS].Home.nDir))
	{
		ClearMotionFault(SCAN_S_AXIS);
		StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);
		do {
			Sleep(1);
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		} while (GetThetaLimitInputForGm((BOOL)m_pParamMotion[SCAN_S_AXIS].Home.nDir));
		//Stop(SCAN_AXIS);
		EStop(SCAN_AXIS);
		Sleep(50);

		while (!IsMotionDone(SCAN_S_AXIS))
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
			Sleep(1);
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		ClearMotionFault(SCAN_S_AXIS);
		Sleep(50);
	}

	//2. -축을 제외한 다른위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP2_MOVE_ORIGIN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);

	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}


	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP3_ESCAPE_ORIGIN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	StartPosMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fEscLen*(-m_pParamMotion[SCAN_S_AXIS].Home.nDir), m_pParamMotion[SCAN_S_AXIS].Home.f1stSpd, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, INC, NO_WAIT);
	Sleep(10);
	while (!IsMotionDone(SCAN_S_AXIS) || !IsInPosition(SCAN_S_AXIS))
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP4_ORIGIN_RETURN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	//4. -축을 제외한 다른위치에 모타가 있을때 
	// -방향으로 움직이면서 일단 -Limit를 찾음 
	// Move to the negative limit : 
	// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
	// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
	//	float fEStopTime = m_pMotionCard->GetEStopTime(SCAN_S_AXIS);
	double fSpeed, fMachineSpeed, fENewStopTime;
	double fTemp = (double)m_pParamMotor[SCAN_S_AXIS].fLeadPitch / (double)m_pParamMotor[SCAN_S_AXIS].nEncPulse;
	if (fTemp <= 0.001)
	{
		fSpeed = (1.0*1000.0) / (m_fPosRes[SCAN_S_AXIS]*1000.0); // [mm/s]
		fMachineSpeed = PulseToLength(SCAN_S_AXIS, (long)fSpeed) / 2.0;
		fENewStopTime = float(fMachineSpeed / 1000.);
	}
	else
	{
		fMachineSpeed = (m_fPosRes[SCAN_S_AXIS]*1000.0) / 2.0; // [mm/s]
		fENewStopTime = float(fMachineSpeed / 1000.0);
	}
	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fENewStopTime);
	StartVelMove(SCAN_S_AXIS, fMachineSpeed*m_pParamMotion[SCAN_S_AXIS].Home.nDir, 10.0*fMachineSpeed);

	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}

	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fEStopTime);

	// 센서 기준 원점 복귀 완톩E
#ifdef USE_ZMP
	SetCommandPosition(SCAN_S_AXIS, 0.0);
	SetActualPosition(SCAN_S_AXIS, 0.0);
#else
	SetActualPosition(SCAN_S_AXIS, 0.0);
	SetCommandPosition(SCAN_S_AXIS, 0.0);
#endif

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP5_ORIGIN_SHIFT"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	if (m_pParamMotion[SCAN_S_AXIS].Home.fShift != 0.0)
	{
		if (StartPosMove(SCAN_S_AXIS, -1.0 * m_pParamMotion[SCAN_S_AXIS].Home.nDir * m_pParamMotion[SCAN_S_AXIS].Home.fShift, m_pParamMotion[SCAN_S_AXIS].Home.f1stSpd, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, INC, NO_WAIT) != SUCCESS)
		{
			ChangePosSWLimitValue(SCAN_S_AXIS, fPrevPosLimit);
			ChangeNegSWLimitValue(SCAN_S_AXIS, fPrevNegLimit);
			return FALSE;
		}
		Sleep(100);
		while (!IsMotionDone(SCAN_S_AXIS) || !IsInPosition(SCAN_S_AXIS))
		{
			if (GetAxisState(SCAN_S_AXIS) == MPIStateERROR)
			{
				ChangePosSWLimitValue(SCAN_S_AXIS, fPrevPosLimit);
				ChangeNegSWLimitValue(SCAN_S_AXIS, fPrevNegLimit);
				pGlobalView->m_strFormat = _T("[MSG873] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "STR_FORMAT", "OriginReturn_P1_M7");
				strMsg.Format(_T("%s %s"), GetMotorName(SCAN_S_AXIS), pGlobalView->m_strFormat);
				AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
				return FALSE;
			}
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(SCAN_S_AXIS);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
			Sleep(1);
		}
	}

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "STEP6_ORIGIN_OFFSET"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

#ifdef USE_ZMP
	SetCommandPosition(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fOffset);
	SetActualPosition(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fOffset);
#else
	SetActualPosition(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fOffset);
	SetCommandPosition(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fOffset);
#endif

	// previous software limit value 복퓖E
	ChangePosSWLimitValue(SCAN_S_AXIS, fPrevPosLimit);
	ChangeNegSWLimitValue(SCAN_S_AXIS, fPrevNegLimit);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	//m_pMotionCard->SetMsAction(SCAN_S_AXIS, MPIActionRESET);
	//m_pMotionCard->m_pAxis[SCAN_S_AXIS].SetOriginPos();
	nAct = (INT)E_STOP_EVENT;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);

	m_bOrigin[SCAN_S_AXIS] = TRUE;

#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
	m_pMotionCard->SetPosSWLimitDirection(SCAN_S_AXIS, TRUE);
	m_pMotionCard->SetNegSWLimitDirection(SCAN_S_AXIS, TRUE);
#endif

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_ORIGIN_RETURN"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);


	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);
	// Homming end
#endif
#endif
	return TRUE;
}

// 지정된 축의 리미트센서 위치를 파악하여 소프트웨푳E리미트 값, 햨E최큱E스트로크 값을 결정하여 리턴한다.
int CMotionNew::GetThetaStrokeForGm(double fPlusMargin, double fMinusMargin)
{
#ifdef SCAN_S_AXIS
	BOOL bLimitEvent = FALSE;
	int errCode = 0;

	if (!IsServoOn(SCAN_S_AXIS))
		return FALSE;

	double fPosLimit, fNegLimit;
	CString strTitleMsg, strMsg, strDir;

	strTitleMsg.Format(_T("%s %s"), GetMotorName(SCAN_S_AXIS), pGlobalView->GetLanguageString("MOTION", "STROKE_CHECK"));
	if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == PLUS)
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "POS_STROKE_TEST"));
	else
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "NEG_STROKE_TEST"));

	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);

	ClearMotionFault(SCAN_S_AXIS);
	while (!IsMotionDone(SCAN_S_AXIS));

	// software limit value 최큱Eだ막?변컖E
	ChangePosSWLimitValue(SCAN_S_AXIS, 10000.0);
	ChangeNegSWLimitValue(SCAN_S_AXIS, -10000.0);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionNONE);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionNONE);
	INT nAct = (INT)NO_EVENT;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);

	//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉救다.
	if (GetThetaLimitInputForGm(m_pParamMotion[SCAN_S_AXIS].Home.nDir))
	{
		ClearMotionFault(SCAN_S_AXIS);
		StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f1stSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);
		do {
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		} while (GetThetaLimitInputForGm(m_pParamMotion[SCAN_S_AXIS].Home.nDir));
		//Stop(SCAN_AXIS);
		EStop(SCAN_AXIS);
		Sleep(50);

		while (!IsMotionDone(SCAN_S_AXIS))
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
		ClearMotionFault(SCAN_S_AXIS);
		Sleep(50);
	}

	//2. 원점 limit센서가 Check되햨E않은 위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 
	StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);
	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}
	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
	StartPosMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fEscLen*(-m_pParamMotion[SCAN_S_AXIS].Home.nDir), m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(SCAN_S_AXIS) || !IsInPosition(SCAN_S_AXIS))
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
	}

	//4. -축을 제외한 다른위치에 모타가 있을때 
	// -방향으로 움직이면서 일단 -Limit를 찾음 
	// Move to the negative limit : 
	// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
	// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
	//2008,08,13 modify by khc
	//	float fEStopTime = m_pMotionCard->GetEStopTime(SCAN_S_AXIS);
	double fSpeed, fMachineSpeed, fENewStopTime;
	double fTemp = (double)m_pParamMotor[SCAN_S_AXIS].fLeadPitch / (double)m_pParamMotor[SCAN_S_AXIS].nEncPulse;
	if (fTemp <= 0.001)
	{
		fSpeed = (1.0*1000.0) / (m_fPosRes[SCAN_S_AXIS] * 1000); // [mm/s]
		fMachineSpeed = PulseToLength(SCAN_S_AXIS, (long)fSpeed) / 2.0;
		fENewStopTime = float(fMachineSpeed / 1000.);
	}
	else
	{
		fMachineSpeed = (m_fPosRes[SCAN_S_AXIS] * 1000) / 2.0; // [mm/s]
		fENewStopTime = float(fMachineSpeed / 1000.);
	}
	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fENewStopTime);

	// 홈방향으로 정밀 센싱을 함.
	StartVelMove(SCAN_S_AXIS, fMachineSpeed*m_pParamMotion[SCAN_S_AXIS].Home.nDir, 10.0*fMachineSpeed);

	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}
	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	// 홈퉩EE위치를 체크.
	if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == PLUS)
		fPosLimit = GetActualPosition(SCAN_S_AXIS);
	else
		fNegLimit = GetActualPosition(SCAN_S_AXIS);

	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fEStopTime);

	if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == PLUS)
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "NEG_STROKE_TEST"));
	else
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "POS_STROKE_TEST"));

	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);


	//5. 원점 limit센서가 Check되햨E않은 위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점반큱E센서를 찾음 

	StartVelMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, m_pParamMotion[SCAN_S_AXIS].Home.fAcc);

	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}
	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	// 원점방향으로 fEscLen만큼 벗엉毆.
	StartPosMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fEscLen*(m_pParamMotion[SCAN_S_AXIS].Home.nDir), m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(SCAN_S_AXIS) || !IsInPosition(SCAN_S_AXIS))
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
	}

	//4. -축을 제외한 다른위치에 모타가 있을때 
	// -방향으로 움직이면서 일단 -Limit를 찾음 
	// Move to the negative limit : 
	// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
	// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)

	// 원점 반큱E센싱를 함.
	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fENewStopTime);	
	StartVelMove(SCAN_S_AXIS, fMachineSpeed*-m_pParamMotion[SCAN_S_AXIS].Home.nDir, 10.0*fMachineSpeed);

	bLimitEvent = FALSE;
	while (bLimitEvent == FALSE)
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == 1)
			bLimitEvent = GetThetaLimitInputForGm(MINUS);//IsPosHWLimit();
		else
			bLimitEvent = GetThetaLimitInputForGm(PLUS);//IsNegHWLimit();

		if (bLimitEvent)
		{
			//Stop(SCAN_AXIS);
			EStop(SCAN_AXIS);
			Sleep(50);
		}

		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
		Sleep(1);
	}
	ClearMotionFault(SCAN_S_AXIS);
	Sleep(50);

	// 	while(!GetThetaLimitInputForGm(-m_MotionParam.Home.nDir))
	// 	{
	// 		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
	// 		{
	// 			MotionAbort();
	// 			return FALSE;
	// 		}
	// 	}

	// 원점 반큱E씔?위치를 체크함.
	if (m_pParamMotion[SCAN_S_AXIS].Home.nDir == PLUS)
		fNegLimit = GetActualPosition(SCAN_S_AXIS);
	else
		fPosLimit = GetActualPosition(SCAN_S_AXIS);

	//	m_pMotionCard->SetEStopTime(SCAN_S_AXIS, fEStopTime);

	//원점 방향으로 fEscLen만큼 벗엉毆.
	StartPosMove(SCAN_S_AXIS, m_pParamMotion[SCAN_S_AXIS].Home.fEscLen*(m_pParamMotion[SCAN_S_AXIS].Home.nDir), m_pParamMotion[SCAN_S_AXIS].Home.f2ndSpd, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, m_pParamMotion[SCAN_S_AXIS].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(SCAN_S_AXIS) || !IsInPosition(SCAN_S_AXIS))
	{
		if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
		{
			MotionAbort(SCAN_S_AXIS);
			return FALSE;
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
	}

	m_pParamMotor[SCAN_S_AXIS].fPosLimit = fPosLimit - fabs(fPlusMargin);
	m_pParamMotor[SCAN_S_AXIS].fNegLimit = fNegLimit + fabs(fMinusMargin);

	//m_pMotionCard->SetPosSWLimitValue(SCAN_S_AXIS, m_pParamMotor[SCAN_S_AXIS].fPosLimit);
	//m_pMotionCard->SetNegSWLimitValue(SCAN_S_AXIS, m_pParamMotor[SCAN_S_AXIS].fNegLimit);

	//m_pMotionCard->SetPosSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	//m_pMotionCard->SetNegSWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
	nAct = (INT)E_STOP_EVENT;
	m_fPosLimit[SCAN_S_AXIS] = m_pParamMotor[SCAN_S_AXIS].fPosLimit;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosSoftwareLimit(m_fPosLimit[SCAN_S_AXIS], nAct);
	m_fNegLimit[SCAN_S_AXIS] = m_pParamMotor[SCAN_S_AXIS].fNegLimit;
	m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegSoftwareLimit(m_fNegLimit[SCAN_S_AXIS], nAct);

#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	//m_pMotionCard->SetPosSWLimitDirection(SCAN_S_AXIS, TRUE);
	//m_pMotionCard->SetNegSWLimitDirection(SCAN_S_AXIS, TRUE);
#endif

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_STROKE_TEST"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN, 200);


	ClearMotionFault(SCAN_S_AXIS);
	Sleep(300);
#endif
	return TRUE;
}

BOOL CMotionNew::SearchThetaHomePosOfGm()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
#ifdef SCAN_S_AXIS
#if MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSMART_GM4000

	if (!m_pParamMotion[SCAN_S_AXIS].Home.bAct)
		return TRUE;

	if (!IsServoOn(SCAN_S_AXIS))
		return FALSE;

	m_bOrigin[SCAN_S_AXIS] = FALSE;

	if (!ThetaOriginForGm())
		return FALSE;
#endif
#endif
#endif

	return TRUE;
}

void CMotionNew::SetSWLimitDirection(BOOL bDir)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif

#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver

	int nAxisId;

	for (nAxisId = 0; nAxisId < MAX_AXIS; nAxisId++)
	{
#ifdef USE_ATF_MODULE
		if (nAxisId == FOCUS_AXIS)
			continue;
#endif
		//m_pMotionCard->SetPosSWLimitDirection(nAxisId, bDir);
		//m_pMotionCard->SetNegSWLimitDirection(nAxisId, bDir);
	}

#endif	
}

int CMotionNew::SearchMachineStroke(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	
	// 현재위치를 파악
	double fCurPosition, dTempV; // 20160104-syd
#ifdef MAG_AXIS
	if(nAxisId==MAG_AXIS)
	{
		if(m_pParamMotion[MAG_AXIS].Home.fShift < 0.0)
			dTempV = -m_pParamMotion[MAG_AXIS].Home.fShift;
		else
			dTempV = m_pParamMotion[MAG_AXIS].Home.fShift;
		fCurPosition = dTempV * (double)(-m_pParamMotion[MAG_AXIS].Home.nDir);
	}
	else
		fCurPosition = GetCommandPosition(nAxisId);
#else
		fCurPosition = GetCommandPosition(nAxisId);
#endif
	// Check Origin Sensor and change Origin Position.
#ifdef SCAN_S_AXIS
	if(nAxisId==SCAN_S_AXIS)
		SearchThetaHomePosOfGm();
	else
		SearchHomePos(nAxisId,FALSE);
#else
	SearchHomePos(nAxisId,FALSE);// syd-20110627
#endif
		
	double fSoftwareLimitMargin=0.0;
		
	if(m_pParamMotor[nAxisId].bType==SERVO)
	{
		fSoftwareLimitMargin = 1.0;
			
#ifdef SCAN_S_AXIS
		if(nAxisId==SCAN_S_AXIS)
			fSoftwareLimitMargin = 0.005;
#endif
	
#ifdef THETA_AXIS
		if (nAxisId == THETA_AXIS)
			fSoftwareLimitMargin = 0.005;
#endif
	}
	else
	{
		fSoftwareLimitMargin = 0.5;
			
#ifdef THETA_AXIS
		if(nAxisId!=THETA_AXIS)
			fSoftwareLimitMargin = 0.5;
		else
			fSoftwareLimitMargin = 0.005;
#endif
		//190724 lgh
#ifdef ZOOM_AXIS
	
		if(ZOOM_AXIS == nAxisId)
			fSoftwareLimitMargin = 5;
#endif
			
#ifdef SCAN_S_AXIS
		if(nAxisId!=SCAN_S_AXIS)
			fSoftwareLimitMargin = 0.5;
		else
			fSoftwareLimitMargin = 0.005;
#endif
	}
		
		
#if MACHINE_MODEL != INSPRO_R2R_EG && MACHINE_MODEL != INSPRO_R2R_MDS && MACHINE_MODEL != INSMART_R2R_SUMITOMO && MACHINE_MODEL != INSPRO_GM_PLUS && MACHINE_MODEL != INSPRO_GM && MACHINE_MODEL != INSPRO50S && MACHINE_MODEL != INSPRO_PTH && MACHINE_MODEL != INSMART_ULTRA
	if(!GetAxisStroke(nAxisId, fSoftwareLimitMargin, fSoftwareLimitMargin))
		return FALSE;
#elif MACHINE_MODEL == INSPRO_R2R_EG || MACHINE_MODEL == INSPRO_R2R_MDS || MACHINE_MODEL == INSMART_R2R_SUMITOMO
	if(nAxisId != SCAN_S_AXIS)
	{
		if(!GetAxisStroke(nAxisId, fSoftwareLimitMargin,fSoftwareLimitMargin))
			return FALSE;
	}
	else
		return FALSE;
#else
	if(nAxisId != SCAN_S_AXIS)
	{
	#ifdef USE_ATF_MODULE
		if(nAxisId == FOCUS_AXIS)
		{
			int err;
			int nAtfMovePos;
				
			nAtfMovePos = -(m_nAtfHomingParam[0]);
			err = atf_MoveZ(nAtfMovePos);
	
			if(err == ErrOK)
			{
				while(!pMotionNew->CheckMotionDone(FOCUS_AXIS))
					Sleep(1);
			}
			else
			{
				return FALSE;
			}
	
			m_fAtfNegSWLimitPos = GetActualPosition(FOCUS_AXIS) + fSoftwareLimitMargin;
			m_fAtfPosSWLimitPos = -fSoftwareLimitMargin;
		}
		else
		{
			if(!GetAxisStroke(nAxisId, fSoftwareLimitMargin, fSoftwareLimitMargin))
				return FALSE;
		}
	#else
		if(!GetAxisStroke(nAxisId, fSoftwareLimitMargin, fSoftwareLimitMargin))
			return FALSE;
	#endif
	}
	else
	{
		if(!MoveScanMotor(200.0,25.0,WAIT))
			return FALSE;
			
		if(!GetThetaStrokeForGm(fSoftwareLimitMargin, fSoftwareLimitMargin))
			return FALSE;
	}
#endif
		
	m_pParamMotor[nAxisId].fPosLimit = GetPosSWLimitValue(nAxisId);
	m_pParamMotor[nAxisId].fNegLimit = GetNegSWLimitValue(nAxisId);
		
#ifdef MAG_AXIS
	if(nAxisId == MAG_AXIS)
		m_fMagMotorNegLimitPos = m_pParamMotor[MAG_AXIS].fNegLimit;
#endif
		
#ifdef USE_ATF_MODULE
	if(nAxisId == FOCUS_AXIS)
	{
		m_pParamMotor[nAxisId].fPosLimit = m_fAtfPosSWLimitPos;
		m_pParamMotor[nAxisId].fNegLimit = m_fAtfNegSWLimitPos;
	
		ChangePosSWLimitValue(FOCUS_AXIS, m_pParamMotor[nAxisId].fPosLimit);
		ChangeNegSWLimitValue(FOCUS_AXIS, m_pParamMotor[nAxisId].fNegLimit);
	
		if(pGlobalView->m_bATFConnect)
		{
			double fCurrPos, fPosDiff;
			int nMicroStep;
			int err;
	
			fCurrPos = GetActualPosition(FOCUS_AXIS);
			fPosDiff = fCurPosition - fCurrPos;
	
			nMicroStep = (int)(fPosDiff * 1000.0) * m_nAtfMicroStepNum * m_nAtfFullStepPerMM / 1000;
			err = atf_MoveZ(nMicroStep);
	
			if(err == ErrOK)
			{
				while(!pMotionNew->CheckMotionDone(FOCUS_AXIS))
				{
					//160104 lgh for ui freeze
					pGlobalView->RefreshWindows(1);
					pMainFrame->RefreshWindows(1);
					Sleep(1);
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox(_T("[MSG294] ATF Connect Error!"));
			return FALSE;
		}
	}
	else
	{
		if(m_pParamMotor[nAxisId].bType==SERVO)
			StartPosMove(nAxisId, fCurPosition, 25.0);
		else
			StartPosMove(nAxisId, fCurPosition, 100.0);
	}
#else
	if(m_pParamMotor[nAxisId].bType==SERVO)
		StartPosMove(nAxisId, fCurPosition, 25.0);
	else
		StartPosMove(nAxisId, fCurPosition, 100.0);
#endif	
	
#ifdef SCAN_S_AXIS
	if (nAxisId == SCAN_AXIS)
	{
		pMotionNew->ClearMotionFault(SCAN_S_AXIS);
		pMotionNew->MoveThetaMotor((GetPosSWLimitValue(SCAN_S_AXIS)+GetNegSWLimitValue(SCAN_S_AXIS))/2.0, TRUE);
	}
#endif
	
	// 20160104-syd
	//m_pMotionCard->SetPosSWLimitAction(nAxisId, MPIActionE_STOP);
	//m_pMotionCard->SetNegSWLimitAction(nAxisId, MPIActionE_STOP);
	INT nAct = (INT)E_STOP_EVENT;
	m_pMotionCard->GetAxis(nAxisId)->SetNegHWLimitAction(MPIActionE_STOP);
	m_pMotionCard->GetAxis(nAxisId)->SetPosHWLimitAction(MPIActionE_STOP);

	return TRUE;
}

double CMotionNew::GetMagAxisStrokeLen()
{
	return m_fMagAxisStrokeLen;
}

BOOL CMotionNew::SetHomeShiftVal(int nMsId, double fShift)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	m_pParamMotion[nMsId].Home.fShift = fShift;

	CString strData, strValue;
	int nAxisID;

	strData = "";
	for (nAxisID = 0; nAxisID < MAX_AXIS - 1; nAxisID++)
	{
		strValue.Format(_T("%.3f"), m_pParamMotion[nAxisID].Home.fShift);
		strData += strValue;
		strData += ",";
	}
	strValue.Format(_T("%.3f"), m_pParamMotion[nAxisID].Home.fShift);
	strData += strValue;
	strData += "	// [mm] or [deg]";

	WritePrivateProfileString(_T("ORIGIN PARAM"), _T("SHIFT"), strData, MOTOR_PARAM_PATH);

	return TRUE;
}

double CMotionNew::GetAccScanLength(double fLineRate, double fOptRes)
{
	double fScanVel = fLineRate * fOptRes;
	double fScanAcc = fScanVel*10.; // [mm/sec^2], 속도 추종성 확보를 위하여 급격한 가속 금햨E
	double fScanDec = fScanVel*10.; // [mm/sec^2]

	double fMaxVel = 1000.0; // [mm/s]
	double fMaxAcc = m_pParamAxis[SCAN_AXIS].fMaxAccel*9.8*1000.0;
	fScanVel = min(fScanVel, fMaxVel);
	fScanAcc = min(fScanAcc, fMaxAcc);
	fScanDec = min(fScanDec, fMaxAcc);

	double fAccLength = ((fScanVel*fScanVel) / fScanAcc) / 2.0; // 가속 구간의 거리 [mm]
	return fAccLength;
}

CString CMotionNew::CheckAllAxisErrorStatus()		// 20110812 ljh
{
	//if (m_pMotionCard == nullptr)
	//	return "";

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return "Need initiate motion board";
	}
#endif

	CString strAxesStatus = "";
	int nLine = 0;

#ifdef SCAN_AXIS
	if (m_pMotionCard->GetAxisState(SCAN_AXIS) == MPIStateERROR) {
		nLine++;
		strAxesStatus += "ScanAxisError";
	}
#endif

#ifdef CAM_AXIS
	if (m_pMotionCard->GetAxisState(CAM_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "CamAxisError";
	}
#endif

#ifdef FOCUS_AXIS
	if (m_pMotionCard->GetAxisState(FOCUS_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "FocusAxisError";
	}
#endif

#ifdef FOCUS2_AXIS
	if (m_pMotionCard->GetAxisState(FOCUS2_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "Focus2AxisError";
	}
#endif

#ifdef ZOOM_AXIS
	if (m_pMotionCard->GetAxisState(ZOOM_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "ZoomAxisError";
	}
#endif

#ifdef MAG_AXIS
	if (m_pMotionCard->GetAxisState(MAG_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "MagAxisError";
	}
#endif

#ifdef THETA_AXIS
	if (m_pMotionCard->GetAxisState(THETA_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "ThetaAxisError";
	}
#endif

#ifdef HEIGHT_AXIS
	if (m_pMotionCard->GetAxisState(HEIGHT_AXIS) == MPIStateERROR) {
		if (nLine > 0)
			strAxesStatus += ", ";
		nLine++;
		strAxesStatus += "HeightAxisError";
	}
#endif

	return strAxesStatus;
}

BOOL CMotionNew::GetGantry(long lMaster, long lSlave, long *lOnOff)
{
	if (lMaster >= MAX_AXIS || lSlave >= MAX_AXIS)
		return FALSE;

	//return  TRUE;
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return 0;
		}
	#endif


		if (!m_bGantryEnabled && m_bUseGantry && m_lGantryEnable)
		{
			*lOnOff = TRUE;
			return TRUE;
		}

	return m_pMotionCard->GetGantry(lMaster, lSlave, lOnOff);
}

BOOL CMotionNew::SetGantry(long lMaster, long lSlave, long lOnOff)
{
	if (lMaster >= MAX_AXIS || lSlave >= MAX_AXIS)
		return FALSE;

	m_bUseGantry = TRUE;
	m_lGantryMaster = lMaster;
	m_lGantrylSlave = lSlave;
	m_lGantryEnable = lOnOff;

	return m_pMotionCard->SetGantry(lMaster, lSlave, lOnOff);
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//	return m_pMotionCard->SetGantry(lMaster, lSlave, lOnOff);
}

BOOL CMotionNew::GantryEnable(long lMaster, long lSlave, long lOnOff)
{
	BOOL bRtn = m_pMotionCard->GantryEnable(lMaster, lSlave, lOnOff);

	if(bRtn)
		m_bGantryEnabled = TRUE;
	else
		m_bGantryEnabled = FALSE;

	return bRtn;
}


BOOL CMotionNew::CheckOriginRet(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
	{
		//m_bOrigin[nAxisId] = TRUE;
		return TRUE;
	}

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef USE_ATF_MODULE
	if(nAxisId == FOCUS_AXIS)
	{
		int err;
		bool bRet;

		if(m_nAtfHomingError == ErrOK)
		{
			err = 0;
			bRet = 0;

			err = atf_IsInHomePosition(&bRet);

			if(err == ErrOK)
			{
				if(bRet)
					return TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
#endif

	if(!m_pParamMotion[nAxisId].Home.bAct)
		return TRUE;
	
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if(nAxisId==MAG_AXIS)
		int nBreak=0;

	m_bOrigin[nAxisId] = m_pMotionCard->m_bOrigin[nAxisId];
	if(!m_bOrigin[nAxisId])
		return FALSE;
#endif
	return TRUE;
}

void CMotionNew::DBNSetUp()
{
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return;
		}
	#endif
	
	#if TRIGGER_TYPE == CONVEX_TRIGGER || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
		SetConvexDriveTrigOriginPosition(SCAN_AXIS, 0.0);
	#else
	
		
	#if TRIGGER_TYPE == GVIS_TRIGGER
	#ifndef RI_SYSTEM
		if(!m_pGtkDBN)
		{
			m_pGtkDBN = new CGtkDbnCfg;
	#ifdef USE_XMP
			m_pGtkDBN->DBNSetUp(m_pMotionCard->m_hControl,
								m_pMotionCard->m_pAxis[SCAN_AXIS].m_hAxis,
								m_pMotionCard->m_pMotor[SCAN_AXIS].m_hMotor,
								controlType,
								controlAddress);//,
	#else
			m_pGtkDBN->DBNSetUp(m_pMotionCard->m_hControl,
				m_pMotionCard->m_pAxis[SCAN_AXIS].m_hAxis,
				m_pMotionCard->m_pMotor[SCAN_AXIS].m_hMotor,
				controlType,
				controlAddress, m_pMotionCard->m_hSynqNet);
	
	#endif
		}
	#endif
	#endif
	#endif
}

BOOL CMotionNew::IsOriginDone(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	return m_bOrigin[nMsId];
}

BOOL CMotionNew::InitRecorder()
{
	return TRUE;
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//	return m_pMotionCard->InitRecorder();
}

CString CMotionNew::GetMotorName(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return _T("");

	return m_pParamAxis[nAxisId].sName;
}

BOOL CMotionNew::FreeAll()
{
	/* Delete user limit object */
	//if (m_UserLimitDoor != nullptr)
	//{
	//	mpiUserLimitDelete(m_UserLimitDoor);
	//	m_UserLimitDoor = NULL;
	//}
	//if (m_UserLimitDoor2 != nullptr)
	//{
	//		mpiUserLimitDelete(m_UserLimitDoor2);
	//	m_UserLimitDoor2 = NULL;
	//}

	if (m_pRevolverLensPos)
	{
		delete[] m_pRevolverLensPos;
		m_pRevolverLensPos = NULL;
	}

	if (m_pMyFileErrMap)
	{
		delete m_pMyFileErrMap;
		m_pMyFileErrMap = NULL;
	}
#if TRIGGER_TYPE == GVIS_TRIGGER	
#ifndef RI_SYSTEM
	if (m_pGtkDBN)
	{
		m_pGtkDBN->DBNExit();
		delete m_pGtkDBN;
		m_pGtkDBN = NULL;
	}
#endif
#endif	
	if (m_pParamMotor)
	{
		delete[] m_pParamMotor;
		m_pParamMotor = NULL;
	}

	if (m_pParamAxis)
	{
		delete[] m_pParamAxis;
		m_pParamAxis = NULL;
	}

	if (m_pParamMotion)
	{
		delete[] m_pParamMotion;
		m_pParamMotion = NULL;
	}

	if (m_pMotionCard)
	{
		delete m_pMotionCard;
		m_pMotionCard = NULL;
	}

	return TRUE;
}

void CMotionNew::Exit()
{
	if (m_pMonitorThread)
		StopMonitorThread();



#if TRIGGER_TYPE == CONVEX_TRIGGER || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
	if (m_bEnabelConvexDriveTrig[SCAN_AXIS])
		DisableConvexDriveTrig(SCAN_AXIS);
#endif
#if TRIGGER_TYPE == GVIS_TRIGGER	
#ifndef RI_SYSTEM
	if (m_pGtkDBN)
	{
		m_pGtkDBN->DBNExit();
		delete m_pGtkDBN;
		m_pGtkDBN = NULL;
	}
#endif
#endif
	for (int i = 0; i<MAX_AXIS; i++)
	{
		Abort(i);
	}
}

int CMotionNew::WaitHomeEnd(DWORD dwTimeOver)
{
#ifndef MOTION_ACTION	//20120306-ndy modified for RI
	return SUCCESS;
#endif
	DWORD dwStartTick = GetTickCount();
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh)
	{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif
		if (m_pMotionCard->m_nInterlockStatus)
		{
			return OPERATE_INTERLOCK;
		}

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pIO->CheckShinkoDoorOpen())
			{
				return OPERATE_INTERLOCK;
			}
		}

#if MACHINE_MODEL != INSPRO_R2R_EG && MACHINE_MODEL != INSMART_R2R_SUMITOMO && MACHINE_MODEL != INSPRO2HR2R_GANTRY && MACHINE_MODEL != INSPRO_R2R_MDS

#if MACHINE_MODEL != INSPRO_GM_PLUS && MACHINE_MODEL != INSMART_GM4000 && MACHINE_MODEL != INSPRO10S && MACHINE_MODEL != INSPRO_GM && MACHINE_MODEL != INSPRO10S_UpGrade && MACHINE_MODEL != INSPRO10S_UpGrade_MAG && MACHINE_MODEL != INSPRO50S && MACHINE_MODEL != INSMART_ULTRA && MACHINE_MODEL != INSMART_ULTRA2 && MACHINE_MODEL != INSMART_ULTRA3 && MACHINE_MODEL != INSPRO_PTH
			if(CheckOriginRet(SCAN_AXIS) 
				&& CheckOriginRet(CAM_AXIS) 
				&& CheckOriginRet(FOCUS_AXIS) 
#ifdef ZOOM_AXIS
				&& CheckOriginRet(ZOOM_AXIS) 
#endif
				&& CheckOriginRet(MAG_AXIS) 
			&& CheckOriginRet(THETA_AXIS)
#ifdef INSP_ZOOM_AXIS
				&& CheckOriginRet(INSP_ZOOM_AXIS)				
#endif
			)
			break;
#else
	#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO10S || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO10S_UpGrade_MAG || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSMART_ULTRA || MACHINE_MODEL == INSMART_ULTRA2 || MACHINE_MODEL == INSMART_ULTRA3
		// 120113 jsy NO_MAG_AXIS
		#ifdef NO_MAG_AXIS
			#ifdef USE_ATF_MODULE
			if(CheckOriginRet(SCAN_AXIS) 
				&& CheckOriginRet(CAM_AXIS) 
			&& CheckOriginRet(ZOOM_AXIS)
			)
			break;
			#else
			if(CheckOriginRet(SCAN_AXIS) 
				&& CheckOriginRet(CAM_AXIS) 
				&& CheckOriginRet(FOCUS_AXIS) 
#ifdef ZOOM_AXIS
			&& CheckOriginRet(ZOOM_AXIS)
#endif
			)
			break;
			#endif
		#else
			if(CheckOriginRet(SCAN_AXIS) 
				&& CheckOriginRet(CAM_AXIS) 
				&& CheckOriginRet(FOCUS_AXIS) 
				&& CheckOriginRet(ZOOM_AXIS) 
			&& CheckOriginRet(MAG_AXIS)
			)
			break;
		#endif
	#else
			if(CheckOriginRet(SCAN_AXIS) 
				&& CheckOriginRet(CAM_AXIS) 
				&& CheckOriginRet(FOCUS_AXIS) 
			&& CheckOriginRet(ZOOM_AXIS)
			)
			break;
	#endif
#endif
#elif MACHINE_MODEL == INSPRO_R2R_EG
		if(CheckOriginRet(SCAN_AXIS) && CheckOriginRet(CAM_AXIS) &&
			CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(ZOOM_AXIS)	)
			break;
#elif MACHINE_MODEL == INSPRO_R2R_MDS
		if(CheckOriginRet(SCAN_AXIS) && CheckOriginRet(CAM_AXIS) &&
			CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(ZOOM_AXIS) && CheckOriginRet(MAG_AXIS))
			break;
#elif MACHINE_MODEL == INSMART_R2R_SUMITOMO
		if(CheckOriginRet(SCAN_AXIS) && CheckOriginRet(CAM_AXIS) &&
			CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(ZOOM_AXIS) && CheckOriginRet(MAG_AXIS) && CheckOriginRet(INSP_ZOOM_AXIS))
			break;
#else
		if(CheckOriginRet(SCAN_AXIS) && CheckOriginRet(CAM_AXIS) &&
			CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(ZOOM_AXIS) &&
			CheckOriginRet(FOCUS2_AXIS) && CheckOriginRet(DIST_AXIS))
			break;
#endif
		else if(pIO->CheckEmgSwitch())
		{
			return (EMERGENCY_STOP);
		}

		else if(pIO->CheckSaftyOnlyBit())
		{
			return (OPERATE_INTERLOCK);
		}
		else if (pIO->CheckIBIDoorStatusNoMessage() == 0)
		{
			SetMainMessage(_T("Door open detection"));
			return  OPERATE_INTERLOCK;
		}
		else if(pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock=TRUE;
			return (OPERATE_INTERLOCK);
		}
		else if(pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
			return (USER_STOP);
		else
		{
			if((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				return (WAIT_TIME_OVER);
			}
		}
		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
	}

	return SUCCESS;			
}

int CMotionNew::WaitMagAdjustEnd(DWORD dwTimeOver, BOOL bThread)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
#ifdef MAG_AXIS
	DWORD dwStartTick = GetTickCount();
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh)
	{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pIO->CheckShinkoDoorOpen())
			{
				return OPERATE_INTERLOCK;
			}
		}

		if (GetAxisState(FOCUS_AXIS) == MPIStateERROR || GetAxisState(MAG_AXIS) == MPIStateERROR)
		{
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				if (pGlobalView->m_bCheckSafetyOnlyBit)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
					}
					return OPERATE_INTERLOCK;
				}
				else
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
					}
				}
				return MEI_MOTION_ERROR;
			}
			else
			{
				if (pIO->CheckEmgSwitch())
					return EMERGENCY_STOP;
			}
		}

		if (CheckMotionDone(FOCUS_AXIS) && CheckMotionDone(MAG_AXIS))
			return SUCCESS;

		if (pIO->CheckEmgSwitch())
			return (EMERGENCY_STOP);
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			return (OPERATE_INTERLOCK);
		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
			return (USER_STOP);
		else if (m_pMotionCard->m_nInterlockStatus)
		{
			return OPERATE_INTERLOCK;
		}

		else if (pIO->CheckSaftyOnlyBit())
		{
			//	nRet = OPERATE_INTERLOCK;
			return OPERATE_INTERLOCK;
		}
		else if (pIO->CheckIBIDoorStatusNoMessage() == 0)
		{
			return OPERATE_INTERLOCK;
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				return (WAIT_TIME_OVER);
			}
		}

		if (bThread == 0)
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	}
#endif
#endif
	return SUCCESS;
}

CString CMotionNew::GetSystemTime()
{

	time_t osBinaryTime;		// C run-time time (defined in <time.h>)
	time(&osBinaryTime);		// Get the current time from the 
								// operating system.
	CTime timeSystem(osBinaryTime);

	CString strTemp;
	strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), timeSystem.GetYear(), timeSystem.GetMonth(), timeSystem.GetDay(), timeSystem.GetHour(), timeSystem.GetMinute(), timeSystem.GetSecond());

	return strTemp;
}

BOOL CMotionNew::StopAll(BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	BOOL bCheckAxes[MAX_AXIS];
	int nCheckCount;
	int nAxisId;

	nCheckCount = 0;
	for (nAxisId = 0; nAxisId<MAX_AXIS; nAxisId++)
	{
		if (GetAxisState(nAxisId) == MPIStateMOVING)
		{
			double fVelocity = GetVelocity(nAxisId);
			double dAcc = m_pParamAxis[nAxisId].fMaxAccel*9.8 * 1000;
			double fStopTime = fabs(fVelocity) / dAcc;
			if (Stop(nAxisId, fStopTime))
			{
				nCheckCount++;
				bCheckAxes[nAxisId] = TRUE;
			}
			else
				bCheckAxes[nAxisId] = FALSE;
		}
	}
	if (nCheckCount)
	{
		int nAxisCount;
		do {
			nAxisCount = 0;
			for (nAxisId = 0; nAxisId<MAX_AXIS; nAxisId++)
			{
				if (bCheckAxes[nAxisId])
				{
					if (GetAxisState(nAxisId) == MPIStateSTOPPED && CheckMotionDone(nAxisId))
						nAxisCount++;
				}
			}
		} while (nAxisCount < nCheckCount);

		for (nAxisId = 0; nAxisId<MAX_AXIS; nAxisId++)
		{
			if (bCheckAxes[nAxisId])
				ClearMotionFault(nAxisId);
		}
	}
	return TRUE;
}

BOOL CMotionNew::WaitMoveEnd(int nAxisID, DWORD dwTimeOver, BOOL bThread)
{
	if (nAxisID >= MAX_AXIS)
		return FALSE;

	DWORD dwStartTick = GetTickCount();
	int nRet = 0; // 0 : Success
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh)
	{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return 1;
		}
#endif

		if (CheckAxisDone(nAxisID))
		{
			nRet = 0; // 0 : Success
			break;
		}
		else if (GetAxisState(nAxisID) == MPIStateERROR)
		{
			nRet = 1; // 1 : MEI_MOTION_ERROR
			break;
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				nRet = 2; // 2 : WAIT_TIME_OVER
				break;
			}
		}
		if (bThread == 0)
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
		else
		{
			Sleep(0);
		}
	}

	return nRet;
}

BOOL CMotionNew::GetRecordLastData(double &dScanPos, double &dThickPos, int bDir)
{
	return TRUE;
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//	return m_pMotionCard->GetRecordLastData(dScanPos, dThickPos, bDir);
}

double CMotionNew::GetRecordStPosScan()
{
	return m_dRecordStPosScan;
}

void CMotionNew::SetRecordStPosScan(double dRecordStPosScan)
{
	m_dRecordStPosScan = dRecordStPosScan;
}

BOOL CMotionNew::GetAutoFocusingData(double fScanCurrPos, double &fScanMotPos, double &dFocusOffset, int bDir)
{
	//	register MPIRecorderRecord  *recordPtr;
	//	double dScanPos, dThickPos;
	//	int nIdx;
	//	
	//	if(bDir==FORWARD)
	//	{
	//		GetRecords();
	//		
	//		long recordIndex = m_pMotionCard->m_pRecordPtr - m_pMotionCard->m_pRecord;
	//// 		recordPtr = &m_pMotionCard->m_Records[recordIndex-1];
	//// 		dScanPos = PulseToLength(SCAN_AXIS, (double)recordPtr->point[0]);
	//		dScanPos = m_pMotionCard->GetRecordData(SCAN_AXIS, 0, recordIndex);
	//		
	//		if(dScanPos < fScanCurrPos)
	//			return FALSE;
	//		
	//		for(nIdx=recordIndex-2; nIdx>=0; nIdx--)
	//		{
	//// 			recordPtr = &m_pMotionCard->m_Records[nIdx];
	//// 			dScanPos = PulseToLength(SCAN_AXIS, (double)recordPtr->point[0]);
	//			dScanPos = m_pMotionCard->GetRecordData(SCAN_AXIS, 0, nIdx);
	//			if(dScanPos < fScanCurrPos)
	//			{
	//// 				dScanPos = PulseToLength(SCAN_AXIS, (double)recordPtr->point[0]);
	//// 				dThickPos = PulseToLength(THICK_AXIS, (double)recordPtr->point[1]);
	//				dScanPos = m_pMotionCard->GetRecordData(SCAN_AXIS, 0, nIdx);
	//				fScanMotPos = dScanPos;
	//#ifdef THICK_AXIS
	//				dThickPos = m_pMotionCard->GetRecordData(THICK_AXIS, 1, nIdx);
	//				dFocusOffset = dThickPos - m_dOrgThickPos;
	//#endif
	//				break;
	//			}
	//		}
	//		
	//		if(nIdx<0)
	//			return FALSE;
	//	}
	//	
	return TRUE;
}

BOOL CMotionNew::CreateBackwardSequenceDataForUniHeader(double dOrgFocus)
{
	return TRUE;
	//	return m_pMotionCard->CreateBackwardSequenceDataForUniHeader(dOrgFocus);
}

long CMotionNew::CreateAutoFocusingSeq2(double fScanMotPos, double dFocusOffset, int bDir) // for THICK_AXIS (SolEnc)
{
	//return 0;
	//return m_pMotionCard->CreateAutoFocusingSeq2(fScanMotPos, dFocusOffset, bDir);
	double fSamplePosOfScanAxis = fScanMotPos + m_dOffsetInspCamLaser;
	double fTargetPosOfFocusAxis = dFocusOffset + m_dOrgFocusPos;

	m_pMotionCard->AddList(fSamplePosOfScanAxis, fTargetPosOfFocusAxis, m_dScanVel);
	return 0;
}

void CMotionNew::StartRecorder()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}

#endif
#ifdef MOTION_ACTION
#ifdef USE_ZMP
	long returnValue;

	m_dRecordStPosScan = GetActualPosition(SCAN_AXIS);
	m_dRecordStPosFocus = GetActualPosition(FOCUS_AXIS);

	memset(m_pRecordScanPos, 0, sizeof(double) * 16384);
	memset(m_pRecordFocusPos, 0, sizeof(double) * 16384);
	m_nRecordTotal = 0;

	/* Start recording the axis */
	returnValue = m_pMotionCard->StartRecorder();
	// //msgCHECK(returnValue);
#endif
#endif
}

void CMotionNew::StopRecorder()
{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif

#ifdef MOTION_ACTION
#ifdef USE_ZMP
	long returnValue;
	returnValue = m_pMotionCard->StopRecorder();



	/* In case the recorder has already stopped */
	if (returnValue == MPIRecorderMessageSTOPPED)
	{
		returnValue = MPIMessageOK;
	}
#endif
#endif
	////msgCHECK(returnValue);
}

BOOL CMotionNew::InitRecords()
{
	return TRUE;
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//
	//	return m_pMotionCard->InitRecords();
}

BOOL CMotionNew::GetRecords()
{
	return TRUE;
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//	}
	//#endif
	//
	//
	//	return m_pMotionCard->GetRecords();
}

CString CMotionNew::DispRecords()
{
	CString sMsg, sMsg0, sMsg1;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return "";
	}
#endif

#ifdef USE_ZMP
	sMsg = "";

	GetRecords();
	//m_pRecordScanPos
	//	long numRecords = MAX_RECORDS;
	long numRecords = m_pMotionCard->m_pRecordPtr - m_pMotionCard->m_pRecord;
	//MEIRecorderRecord   *recordPtr;
	register MPIRecorderRecord  *recordPtr;

	long recordIndex;
	long axisIndex;

	if (numRecords > MAX_RECORDS)
		numRecords = MAX_RECORDS;

	m_nRecordTotal = 0;
	/* Output record buffer to screen */
	for (recordIndex = 0; recordIndex < numRecords; recordIndex++)
	{
		m_pRecordScanPos[recordIndex] = m_pMotionCard->GetRecordData(SCAN_AXIS, 0, recordIndex);
		m_pRecordFocusPos[recordIndex] = m_pMotionCard->GetRecordData(FOCUS_AXIS, 1, recordIndex);
		m_nRecordTotal++;
	}


#ifdef THICK_AXIS

	//	long numRecords = MAX_RECORDS;
	long numRecords = m_pMotionCard->m_pRecordPtr - m_pMotionCard->m_pRecord;
	//MEIRecorderRecord   *recordPtr;
	register MPIRecorderRecord  *recordPtr;

	long recordIndex;
	long axisIndex;


	if (numRecords > MAX_RECORDS)
		numRecords = MAX_RECORDS;

	/* Output record buffer to screen */
	for (recordIndex = 0; recordIndex < numRecords; recordIndex++)
	{
		/* Cast records to MEIRecorderRecord pointer */
		//recordPtr = &m_Records[recordIndex];
		sMsg1.Format(_T("[%d]Actual %.3f , %.3f\n"), recordIndex,
			m_pMotionCard->GetRecordData(SCAN_AXIS, 0, recordIndex),
			m_pMotionCard->GetRecordData(THICK_AXIS, 1, recordIndex));

		sMsg += sMsg1;
	}
#else
	return "";
#endif
#endif
	return sMsg;
}

BOOL CMotionNew::MoveScanPosModifiedSpeed(double fTgtPosX, double fTgtPosY, BOOL bWait, BOOL bClrSol) // goto scan start pos
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	if (pGlobalDoc->m_bInhibitMotion)
		return FALSE;

#ifdef RI_SYSTEM
	return 1;
#endif

	if (pIO->CheckSaftyOnlyBit())
	{
		//nRet = OPERATE_INTERLOCK;
		return FALSE;
	}
	else if (pIO->CheckIBIDoorStatusNoMessage() == 0)
	{
		return FALSE;
	}

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (g_bUseTableLock && pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

			CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else if (pGlobalDoc->m_nSelectedLanguage == JAPANESE)
				SetMainMessage(_T("テ?ブルロック?態です。ロ?ダ?/アンロ?ダ?移動した後、再試行してください"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return FALSE;
		}
	}


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strMsg;

	if (fTgtPosX < GetNegSWLimitValue(CAM_AXIS) || fTgtPosX > GetPosSWLimitValue(CAM_AXIS))
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			CString strError;
			//		strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));


			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));
			else if (pGlobalDoc->m_nSelectedLanguage == JAPANESE)
				strError.Format(_T("%s, 移動範?を超え neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));
			else
				strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(CAM_AXIS), GetNegSWLimitValue(CAM_AXIS), fTgtPosX, GetPosSWLimitValue(CAM_AXIS));



			SetMainMessage(strError);
		}

		return FALSE;
	}

	if (fTgtPosY < GetNegSWLimitValue(SCAN_AXIS) || fTgtPosY > GetPosSWLimitValue(SCAN_AXIS))
	{
		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			CController::m_pController->AlarmCall(ALARM_MOTION_LIMIT_OVER, 1);
			CString strError;

			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				strError.Format(_T("%s, 이동 범위 초과 neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));
			else if (pGlobalDoc->m_nSelectedLanguage == JAPANESE)
				strError.Format(_T("%s, 移動範?を超え neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));
			else
				strError.Format(_T("%s, Move range over neg:%.3f pos:%.3f pos:%.3f"), GetMotorName(SCAN_AXIS), GetNegSWLimitValue(SCAN_AXIS), fTgtPosX, GetPosSWLimitValue(SCAN_AXIS));

			SetMainMessage(strError);
		}

		return FALSE;
	}

	if (pGlobalDoc->m_bUseGlassCover && !pIO->ReadBit(GLASS_COVER_OPEN_SENSOR))
	{
		strMsg.Format(_T("Glass cover is open."));
		pMainFrame->MessageBox(strMsg);
		return FALSE;
	}

	if (!m_bOrigin[CAM_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		strMsg.Format(_T("Camera %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		if (!m_pMotionCard->m_nInterlockStatus)
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}

	if (!m_bOrigin[SCAN_AXIS])
	{
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_ACTIVATE_ALARM);
		strMsg.Format(_T("Scan %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		if (!m_pMotionCard->m_nInterlockStatus)
			AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pMainFrame->SetEESAlarm(ALARM_MOTION_ORIGIN, EES_DEACTIVATE_ALARM);
		return FALSE;
	}
#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED, ON);
#endif
	//20140714 - syd
	if (bClrSol)
	{
		if (pGlobalDoc->m_bUseInspClrSol)
		{
			if (pGlobalDoc->m_nInspClrSolType == 1)	// Outside
			{

				pMotionNew->SetInspClrSol(pGlobalDoc->m_fStartingSpeed,
					fTgtPosY,
					SizeData[COMMON_PARAMETER].fPixelSizeY,
					(long)pGlobalDoc->m_nDummyLine);
			}
		}
	}

	MoveXY(fTgtPosX, fTgtPosY, pGlobalDoc->m_fStartingSpeed, NO_WAIT);
#if MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSMART_GM4000 || MACHINE_MODEL == INSPRO_PTH
	double dTgtPos = fTgtPosX + pGlobalDoc->m_fLightAxisOffset;
	if (dTgtPos < 0.0)
		dTgtPos = 0.0;
	if (StartPosMove(LIGHT_AXIS, dTgtPos, pGlobalDoc->m_fStartingSpeed / 2, ABS, NO_WAIT, OPTIMIZED) != SUCCESS)
		return FALSE;
#endif
	if (bWait == WAIT)
		WaitScanEnd(TIME_OUT_MS, 0);
#endif
	return TRUE;
}

double CMotionNew::GetImageEndPosition(double fLineRate, double fOptRes, double fCurPos, double fImageSize, int nScanDir, long nStartDummy, long nEndDummy)
{
	double fScanVel = fLineRate * fOptRes;
	double fScanAcc = fScanVel*10.; // [mm/sec^2], 속도 추종성 확보를 위하여 급격한 가속 금햨E
	double fScanDec = fScanVel*10.; // [mm/sec^2]

	double fMaxVel = 1000.0; // [mm/s]
	double fMaxAcc = m_pParamAxis[SCAN_AXIS].fMaxAccel*9.8*1000.0;
	fScanVel = min(fScanVel, fMaxVel);
	fScanAcc = min(fScanAcc, fMaxAcc);
	fScanDec = min(fScanDec, fMaxAcc);

	// 1. 시작 더미 구간을 구한다.
	double fStartDummyLength = (nStartDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
																// 가속 구간의 거리 [mm]
	double fAccLength = ((fScanVel*fScanVel) / fScanAcc) / 2.0;
	double fDecLength = ((fScanVel*fScanVel) / fScanDec) / 2.0; // 감속 구간의 거리 [mm]
	double fCurPosition = fCurPos;
	double fDBNStartPos, fDBNEndPos;
	if (fStartDummyLength < fAccLength)
	{
		fDBNStartPos = fCurPosition + ((pGlobalDoc->m_fSettleLen + fAccLength)*nScanDir) - (fStartDummyLength*nScanDir);
		fDBNEndPos = fDBNStartPos + ((fImageSize - (fOptRes / 1000.))*nScanDir);
	}
	else
	{
		fDBNStartPos = fCurPosition + pGlobalDoc->m_fSettleLen*nScanDir;
		fDBNEndPos = fDBNStartPos + ((fImageSize - (fOptRes / 1000.))*nScanDir);
	}
	return fDBNEndPos;
}

void CMotionNew::SetInspClrSol(double dSpdRatio, double dStPosY, double fOptRes, long nStartDummy)
{
	//	// 1. 시작 더미 구간을 구한다.
	//	double fStartDummyLength = (nStartDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
	//	
	//	// 가속 구간의 거리 [mm]
	//	double fCurPositionX = GetCommandPosition(CAM_AXIS);
	//	double fCurPositionY = GetCommandPosition(SCAN_AXIS);
	//	double fTgtPositionY = dStPosY;
	//	double fDBNStartPos;
	//	
	//	double fYDistOfLaserSensorAndInspectCamera;
	//	
	//// 	if(pGlobalDoc->m_bUseAutoFocusSol) //20150507 - syd
	//// 	{
	//// 		fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_dAutoFocusSolOffset;
	//// 		fYDistOfLaserSensorAndInspectCamera -= pGlobalDoc->m_fLaserStartScanPosOffset;
	//// 	}
	//// 	else
	//	{
	//		fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y+pGlobalDoc->m_fLaserSensorOffsetY[0];
	//	}
	//	
	//	fYDistOfLaserSensorAndInspectCamera -= pGlobalDoc->m_fLaserStartScanPosOffset;
	//	
	//	if((pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bMeasureThicknessAction || pGlobalDoc->m_bUseAutoFocusSol) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm) //20150507 - syd
	//	{		
	//		if(fabs(fYDistOfLaserSensorAndInspectCamera) > fStartDummyLength)
	//		{
	//			fDBNStartPos = fTgtPositionY + ((pGlobalDoc->m_fSettleLen + fYDistOfLaserSensorAndInspectCamera-fStartDummyLength));
	//		}
	//		else
	//			fDBNStartPos = fTgtPositionY + pGlobalDoc->m_fSettleLen;
	//	}
	//	else
	//	{
	//		fDBNStartPos = fTgtPositionY + fStartDummyLength + pGlobalDoc->m_fSettleLen;
	//	}
	//	
	//	double fSolOnPos = fDBNStartPos - pGlobalDoc->m_dOffsetClrSolPosSt;
	//	
	//	double dSpd, dAcc;
	//	double fLength = fTgtPositionY - fCurPositionY;
	//	if(dSpdRatio>100.0) 
	//		dSpdRatio = 100.0;
	//	if(dSpdRatio<1.0) 
	//		dSpdRatio = 1.0;
	//	
	//	dSpd=(m_pParamMotion[SCAN_AXIS].Speed.fSpd*dSpdRatio)/100.0;
	//	dAcc=m_pParamMotion[SCAN_AXIS].Speed.fAcc;
	//	
	//	m_pMotionCard->m_pMotion[SCAN_AXIS].CalSpeedProfile(fLength, dSpd, dAcc, dAcc);
	//	
	//	// t = d/v , t = v/a
	//	double dAccTime = dSpd / dAcc;
	//	double fAccLength = ((dSpd*dSpd)/dAcc)/2.0;
	//	double dSpdDist = fSolOnPos - fCurPositionY - fAccLength;
	//	double dSpdTime = dSpdDist / dSpd;
	//	double dTimeSt = dAccTime + dSpdTime; 	
	//	
	//	DWORD dwSt = int(dTimeSt * 1000.0);
	//	DWORD dwCur = GetTickCount();
	//	pGlobalView->SetInspClrSol(dwSt+dwCur);

}


void CMotionNew::SetInspClrSol(double fLineRate, double fOptRes, double fScanSize, double fImageSize, int nScanDir, long nStartDummy, long nEndDummy, double fCurrThicknessByLaserSensor)
{
	return;
	//	DWORD dwSt, dwEd;
	//	double fSolOnPos,fSolOffPos;
	//	
	//	double fScanVel = fLineRate * fOptRes;
	//	double fScanAcc = fScanVel*10.; // [mm/sec^2], 속도 추종성 확보를 위하여 급격한 가속 금햨E
	//	double fScanDec = fScanVel*10.; // [mm/sec^2]
	//	
	//	double fMaxVel = 1000.0; // [mm/s]	
	//	double fMaxAcc = m_pParamAxis[SCAN_AXIS].fMaxAccel*9.8*1000.0;
	//	fScanVel = min(fScanVel,fMaxVel);
	//	fScanAcc = min(fScanAcc,fMaxAcc);
	//	fScanDec = min(fScanDec,fMaxAcc);
	//	
	//	// 1. 시작 더미 구간을 구한다.
	//	double fStartDummyLength = (nStartDummy * fOptRes / 1000.); // 더미펄스 구간의 거리 [mm]
	//	// 가속 구간의 거리 [mm]
	//	double fAccLength = ((fScanVel*fScanVel)/fScanAcc)/2.0;
	//	double fDecLength = ((fScanVel*fScanVel)/fScanDec)/2.0; // 감속 구간의 거리 [mm]
	//	double fCurPositionX = GetCommandPosition(CAM_AXIS);
	//	double fCurPositionY = GetCommandPosition(SCAN_AXIS);
	//	double fDBNStartPos,fDBNEndPos;
	//	
	//	double fYDistOfLaserSensorAndInspectCamera;
	//	
	//// 	if(pGlobalDoc->m_bUseAutoFocusSol) //20150507 - syd
	//// 	{
	//// 		fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_dAutoFocusSolOffset;
	//// 	}
	//// 	else
	//	{
	//		if(nScanDir == BACKWARD)
	//			fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y+pGlobalDoc->m_fLaserSensorOffsetY[1];
	//		else
	//			fYDistOfLaserSensorAndInspectCamera = pGlobalDoc->m_TstCamOffset_Y+pGlobalDoc->m_fLaserSensorOffsetY[0];
	//	}
	//	
	//	
	//	fYDistOfLaserSensorAndInspectCamera -= pGlobalDoc->m_fLaserStartScanPosOffset;
	//	
	//	if((pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor || pGlobalDoc->m_bMeasureThicknessAction) && !pGlobalView->m_pDlgAlignSub->m_bAlignPosConfirm)	//090311-ndy	//20120429-ndy add	//20121125-ndy for 2Head Laser Control
	//	{		
	//		if( fAccLength > fStartDummyLength)
	//		{
	//			if(fabs(fYDistOfLaserSensorAndInspectCamera) > fAccLength)
	//			{
	//				if(nScanDir == BACKWARD)
	//					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen+fAccLength-fStartDummyLength - fYDistOfLaserSensorAndInspectCamera)*nScanDir);
	//				else
	//					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fYDistOfLaserSensorAndInspectCamera-fStartDummyLength)*nScanDir);
	//			}
	//			else
	//				fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fAccLength-fStartDummyLength)*nScanDir);
	//		}
	//		else // if( fStartDummyLength > fAccLength)
	//		{
	//			if(fabs(fYDistOfLaserSensorAndInspectCamera) > fStartDummyLength)
	//			{
	//				if(nScanDir == BACKWARD)
	//					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen-fYDistOfLaserSensorAndInspectCamera)*nScanDir);
	//				else
	//					fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fYDistOfLaserSensorAndInspectCamera-fStartDummyLength)*nScanDir);
	//			}
	//			else
	//				fDBNStartPos = fCurPositionY + pGlobalDoc->m_fSettleLen*nScanDir;
	//		}
	//	}
	//	else
	//	{
	//		if( fAccLength > fStartDummyLength)
	//			fDBNStartPos = fCurPositionY + ((pGlobalDoc->m_fSettleLen + fAccLength-fStartDummyLength)*nScanDir);
	//		else
	//			fDBNStartPos = fCurPositionY + pGlobalDoc->m_fSettleLen*nScanDir;
	//	}
	//	fDBNEndPos = fDBNStartPos + ((fScanSize-(fOptRes/1000.))*nScanDir)+ ((fOptRes*(double)nEndDummy/1000.)*nScanDir);
	//	
	//	fSolOnPos = fDBNStartPos;
	//	fSolOffPos = fDBNStartPos + ((fScanSize-(fOptRes/1000.))*nScanDir);
	//	
	//	double dDist, dTmp, dTimeSt;
	//	dTimeSt = 2.0 * fAccLength / fScanVel;
	//	dwSt = int(dTimeSt * 1000.0) + int( (pGlobalDoc->m_dOffsetClrSolPosSt / fScanVel) * 1000.0 );
	//	
	//	dDist = (dTmp = fSolOnPos - fSolOffPos) < 0 ? -dTmp : dTmp;	
	//	dwEd = dwSt + int( ((dDist - pGlobalDoc->m_dOffsetClrSolPosEd) / fScanVel) * 1000.0);
	//	
	//	DWORD dwCur = GetTickCount();
	//	pGlobalView->SetInspClrSol(dwSt+dwCur, dwEd+dwCur, nScanDir);
	//	
	//}
	//
	//BOOL CMotionNew::StartVelMove(int nAxisId,double fVel,double fAcc)
	//{
	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 0;
	//}
	//#endif
	//
	//	if (pGlobalDoc->m_bUseInlineAutomation)
	//	{
	//		if (g_bUseTableLock && nAxisId == SCAN_AXIS && pIO->ReadBit(LD_SHK_TABLE_LOCK))
	//		{
	//
	//#ifdef MOTION_ACTION
	//			StopMove(SCAN_AXIS, 0.01, 0);
	//#endif
	//			if(CController::m_pController)
	//				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
	//			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
	//				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
	//			else
	//				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));
	//
	//			return 0;
	//		}
	//	}
	//
	//#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	//	pGlobalDoc->m_bTowerLampFlag = TRUE;
	//	pGlobalView->TowerLampControl(TOWER_LAMP_RED,ON);
	//#endif
	//	
	//	pGlobalDoc->m_bRestedMotion = FALSE;
	//	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();
	//	
	//#if MACHINE_MODEL != INSPRO_R2R_EG && MACHINE_MODEL != INSPRO_R2R_MDS && MACHINE_MODEL != INSMART_R2R_SUMITOMO && MACHINE_MODEL != INSPRO_GM_PLUS && MACHINE_MODEL != INSPRO_GM && MACHINE_MODEL != INSPRO50S && MACHINE_MODEL != INSPRO_PTH
	//	if(!m_pParamMotor[nAxisId].bPosLimitLevel)
	//	{
	//		if(!IsLimit(nAxisId, PLUS) && fVel>0.0)
	//			return FALSE;
	//	}
	//	else
	//	{
	//		if(IsLimit(nAxisId, PLUS) && fVel>0.0)
	//			return FALSE;
	//	}
	//
	//	if (!m_pParamMotor[nAxisId].bNegLimitLevel)
	//	{
	//		if (!IsLimit(nAxisId, MINUS) && fVel < 0.0)
	//			return FALSE;
	//	}
	//	else
	//	{
	//		if (IsLimit(nAxisId, MINUS) && fVel < 0.0)
	//			return FALSE;
	//	}
	//
	//#else
	//	if(nAxisId != SCAN_S_AXIS)
	//	{
	//		if(!m_pParamMotor[nAxisId].bPosLimitLevel)
	//		{
	//			if((!IsLimit(nAxisId, PLUS) && fVel>0.0) || (!IsLimit(nAxisId, MINUS) && fVel<0.0))
	//				return FALSE;
	//		}
	//		else
	//		{
	//			if((IsLimit(nAxisId, PLUS) && fVel>0.0) || (IsLimit(nAxisId, MINUS) && fVel<0.0))
	//				return FALSE;
	//		}
	//	}
	//#endif
	//	
	//	if(GetAxisState(nAxisId) == MPIStateERROR)
	//	{
	//		if (IsAmpFault(nAxisId))
	//		{
	//			if (m_pParamMotor[nAxisId].bType == STEPPER) // servo
	//			{
	//				m_pMotionCard->m_pMotor[nAxisId].ResetEasyServo();
	//				Sleep(100);
	//			}
	//
	//			ClearMotionFault(nAxisId);
	//		}
	//
	//		if (GetAxisState(nAxisId) == MPIStateERROR)
	//		{
	//			if ((IsLimit(nAxisId, PLUS) && fVel > 0.0) || (IsLimit(nAxisId, MINUS) && fVel < 0.0))
	//				return FALSE;
	//
	//			if (((IsPosSWLimit(nAxisId) || IsLimit(nAxisId, PLUS)) && fVel < 0.0) || ((IsNegSWLimit(nAxisId) || IsLimit(nAxisId, MINUS)) && fVel > 0.0))
	//			{
	//				MotionAbort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
	//				GetActualPosition(nAxisId);
	//				ClearMotionFault(nAxisId);	// AMP를 idle상태로 픸E?磯?
	//				ServoOnOff(nAxisId, ON);	// AMP를 enable시킨다.
	//				Sleep(30);
	//			}
	//			else
	//			{
	//				//if(/*IsEncoderFault(nAxisId) &&*/ m_pParamMotor[nAxisId].bType == SERVO)	//ZMP에서 IsEncoderFault 함수가 존재하지 않아 임시 대체	2017.07.20 hyk
	//				if (IsLimitError(nAxisId) && m_pParamMotor[nAxisId].bType == SERVO)
	//				{
	//					BOOL bPosHW = 0, bPosSW = 0, bNegHW = 0, bNegSW = 0, bDir = 0, bPosErr = 0, bAmpFault = 0;
	//					CString strMsg;
	//					bPosHW = IsLimit(nAxisId, PLUS);
	//					bNegHW = IsLimit(nAxisId, MINUS);
	//					bPosSW = IsPosSWLimit(nAxisId);
	//					bNegSW = IsNegSWLimit(nAxisId);
	//					if (fVel > 0)
	//						bDir = 1;
	//					else
	//						bDir = 0;
	//
	//					bPosErr = IsLimitError(nAxisId);
	//					bAmpFault = IsAmpFault(nAxisId);
	//
	//					strMsg.Format(_T("PosHW: %d NegHW: %d PosSW: %d NegSW: %d bDir: %d\nPosErr = %d AmpFault = %d"), bPosHW, bNegHW, bPosSW, bNegSW, bDir, bPosErr, bAmpFault);
	//					AfxMessageBox(strMsg);
	//
	//					pMainFrame->SetEESAlarm(ALARM_MOTION_ENCODER_ERROR, EES_ACTIVATE_ALARM);
	//					pGlobalView->m_strAfxMsg = _T("[MSG893] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartVelMove_P3_M1");
	//					AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
	//					pMainFrame->SetEESAlarm(ALARM_MOTION_ENCODER_ERROR, EES_DEACTIVATE_ALARM);
	//					return FALSE;
	//				}
	//				MotionAbort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
	//				ClearMotionFault(nAxisId); // AMP를 idle상태로 픸E?磯?
	//				ServoOnOff(nAxisId, ON);		// AMP를 enable시킨다.
	//			}
	//		}
	//	}
	//
	//	m_pMotionCard->SetVMove(nAxisId, fVel, fAcc);
	//	return m_pMotionCard->VMove(nAxisId, fVel>0.0?PLUS:MINUS);
}
BOOL CMotionNew::StartVelMove(int nAxisId, double fVel, double fAcc, double dPos)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	if(m_pMotionCard->IsListMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);

	if (m_pMotionCard->IsInterpolationMotion())
		m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation);

	if (pGlobalDoc->m_bUseInlineAutomation)
	{
		if (g_bUseTableLock && nAxisId == SCAN_AXIS && pIO->ReadBit(LD_SHK_TABLE_LOCK))
		{

#ifdef MOTION_ACTION
			StopMove(SCAN_AXIS, 0.01, 0);
#endif
			if(CController::m_pController)
				CController::m_pController->AlarmCall(ALARM_TABLE_LOCK, 1);
			if (pGlobalDoc->m_nSelectedLanguage == KOREAN)
				SetMainMessage(_T("테이블 잠금 상태입니다. 로더/언로더 이동 후 다시 시도하십시오"));
			else
				SetMainMessage(_T("Table lock status. Please try again after moving the loader / unloader"));

			return 0;
		}
	}

#if CUSTOMER_COMPANY == SUMITOMO	//120923 hjc add
	pGlobalDoc->m_bTowerLampFlag = TRUE;
	pGlobalView->TowerLampControl(TOWER_LAMP_RED,ON);
#endif
	
	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();
	
#if MACHINE_MODEL != INSPRO_R2R_EG && MACHINE_MODEL != INSPRO_R2R_MDS && MACHINE_MODEL != INSMART_R2R_SUMITOMO && MACHINE_MODEL != INSPRO_GM_PLUS && MACHINE_MODEL != INSPRO_GM && MACHINE_MODEL != INSPRO50S && MACHINE_MODEL != INSPRO_PTH
	if(!m_pParamMotor[nAxisId].bPosLimitLevel)
	{
		if(!IsLimit(nAxisId, PLUS) && fVel>0.0)
			return FALSE;
	}
	else
	{
		if(IsLimit(nAxisId, PLUS) && fVel>0.0)
			return FALSE;
	}

	if (!m_pParamMotor[nAxisId].bNegLimitLevel)
	{
		if (!IsLimit(nAxisId, MINUS) && fVel < 0.0)
			return FALSE;
	}
	else
	{
		if (IsLimit(nAxisId, MINUS) && fVel < 0.0)
			return FALSE;
	}

#else
	if(nAxisId != SCAN_S_AXIS)
	{
		if(!m_pParamMotor[nAxisId].bPosLimitLevel)
		{
			if((!IsLimit(nAxisId, PLUS) && fVel>0.0) || (!IsLimit(nAxisId, MINUS) && fVel<0.0))
				return FALSE;
		}
		else
		{
			if((IsLimit(nAxisId, PLUS) && fVel>0.0) || (IsLimit(nAxisId, MINUS) && fVel<0.0))
				return FALSE;
		}
	}
#endif
	
	if(GetAxisState(nAxisId) == MPIStateERROR)
	{
		if (IsAmpFault(nAxisId))
		{
			if (m_pParamMotor[nAxisId].bType == STEPPER) // servo
			{
				m_pMotionCard->GetAxis(nAxisId)->ResetEasyServo();
				Sleep(100);
			}

			ClearMotionFault(nAxisId);
		}

		if (GetAxisState(nAxisId) == MPIStateERROR)
		{
			if ((IsLimit(nAxisId, PLUS) && fVel > 0.0) || (IsLimit(nAxisId, MINUS) && fVel < 0.0))
				return FALSE;

			if (((IsPosSWLimit(nAxisId) || IsLimit(nAxisId, PLUS)) && fVel < 0.0) || ((IsNegSWLimit(nAxisId) || IsLimit(nAxisId, MINUS)) && fVel > 0.0))
			{
				MotionAbort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
				GetActualPosition(nAxisId);
				ClearMotionFault(nAxisId);	// AMP를 idle상태로 픸E?磯?
				ServoOnOff(nAxisId, ON);	// AMP를 enable시킨다.
				Sleep(30);
			}
			else
			{
				//if(/*IsEncoderFault(nAxisId) &&*/ m_pParamMotor[nAxisId].bType == SERVO)	//ZMP에서 IsEncoderFault 함수가 존재하지 않아 임시 대체	2017.07.20 hyk
				if (IsLimitError(nAxisId) && m_pParamMotor[nAxisId].bType == SERVO)
				{
					BOOL bPosHW = 0, bPosSW = 0, bNegHW = 0, bNegSW = 0, bDir = 0, bPosErr = 0, bAmpFault = 0;
					CString strMsg;
					bPosHW = IsLimit(nAxisId, PLUS);
					bNegHW = IsLimit(nAxisId, MINUS);
					bPosSW = IsPosSWLimit(nAxisId);
					bNegSW = IsNegSWLimit(nAxisId);
					if (fVel > 0)
						bDir = 1;
					else
						bDir = 0;

					bPosErr = IsLimitError(nAxisId);
					bAmpFault = IsAmpFault(nAxisId);

					strMsg.Format(_T("PosHW: %d NegHW: %d PosSW: %d NegSW: %d bDir: %d\nPosErr = %d AmpFault = %d"), bPosHW, bNegHW, bPosSW, bNegSW, bDir, bPosErr, bAmpFault);
					AfxMessageBox(strMsg);

					pMainFrame->SetEESAlarm(ALARM_MOTION_ENCODER_ERROR, EES_ACTIVATE_ALARM);
					pGlobalView->m_strAfxMsg = _T("[MSG893] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartVelMove_P3_M1");
					AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
					pMainFrame->SetEESAlarm(ALARM_MOTION_ENCODER_ERROR, EES_DEACTIVATE_ALARM);
					return FALSE;
				}
				MotionAbort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
				ClearMotionFault(nAxisId); // AMP를 idle상태로 픸E?磯?
				ServoOnOff(nAxisId, ON);		// AMP를 enable시킨다.
			}
		}
	}

	m_pMotionCard->SetVMove(nAxisId, fVel, fAcc);

	BOOL bRtn;
	double fPos;

	if (dPos == 0.0)
	{
		if (fVel > 0.0)
			fPos = 100.0;
		else
			fPos = -100.0;
	}
	else
	{
		fPos = dPos;
		fVel = 1.0;
	}

#ifdef SCAN_S_AXIS
	if(nAxisId == SCAN_S_AXIS)
		bRtn = m_pMotionCard->StartGantrySlaveMove(SCAN_S_AXIS, INC, fPos, fVel, fAcc, fAcc, 0 , 3);
	else
		bRtn = m_pMotionCard->VMove(nAxisId, fVel > 0.0 ? PLUS : MINUS);
#else
	bRtn = m_pMotionCard->VMove(nAxisId, fVel > 0.0 ? PLUS : MINUS);

#endif

	return bRtn;
}
BOOL CMotionNew::IsEncoderFault(int nAxisId)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

	//#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//	if (m_bMotionCreated == 0)
	//	{
	//		return 1;
	//	}
	//#endif
	//
	//	return m_pMotionCard->IsEncoderFault(nAxisId);
	return FALSE;
}

double CMotionNew::GetVelocity(int nAxisId) // [mm/sec]
{
	if (nAxisId >= MAX_AXIS)
		return 0.0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	
	double dVel = 0.0;
	m_csGetActualVelocity.Lock();
	dVel = m_pMotionCard->GetActualVelocity(nAxisId);
	m_csGetActualVelocity.Unlock();
	return(dVel);
	//return 0.0;
}

void CMotionNew::ResetAutoFocusingSeq()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif
	if (m_pMotionCard)
	{
		if (IsListMotion())
		{
			//m_pMotionCard->ResetAutoFocusingSeq();
			m_pMotionCard->StopListMotion();
			Sleep(30);
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_RTAF);
			Sleep(30);
			MoveFocusMotor(m_pMotionCard->GetStartListMotionPos(1)); // 0: ScanAxis, 1:FocusAxis
		}
	}
	
	pGlobalView->m_bLaserExtTrigger = 0;
	StopLMI();
}

void CMotionNew::StopLMI()
{
#ifdef USE_LMI_SENSOR
	if (AoiParam()->m_bUseLMI)
	{
		//g_SequenceAF.m_bThicknessMeaurementMode = 0;
		StopRecorder();
		g_SequenceAF.StopAndReset();
		g_SequenceAF.EnableAFThread(0);
		g_SequenceAF.ClearQueue();

		CGocator* pGoCator = g_lmi.GetHead(0);
		pGoCator->Stop();
	}
#endif
}

BOOL CMotionNew::IsMonitorThreadAlive()
{
	return m_bMonitorThreadAlive;
}

void CMotionNew::SetMonitorThreadAlive(BOOL bAlive)
{
	m_bMonitorThreadAlive = bAlive;
}


// Slice IO function...

unsigned long CMotionNew::GetSliceIo(int nSegment)
{
#ifdef RI_SYSTEM
	return 0;
#endif
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	long value = 0;
	long rtn = 0;

	m_pMotionCard->In32(&value);

	if(nSegment == 0)
		rtn |= (value << 0) & 0x0000FFFF;
	else if(nSegment == 1)
		rtn |= (value >> 16) & 0x0000FFFF;

	return ((unsigned long)rtn);
}

BOOL CMotionNew::SetSliceIo(int nSegment, int nBit, BOOL bOn)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return MEI_MOTION_ERROR;
	}
#endif

	if (nSegment == 3)
		nBit += 16;
	else if (nSegment != 2)
		return FALSE;

	m_pMotionCard->OutBit(nBit, bOn);
	return TRUE;
}

BOOL CMotionNew::SetSliceIo(int nSegment, unsigned long ulOut)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	long OutData = m_pMotionCard->ReadOut();
	long nOut = 0;

	if (nSegment == 2)
	{
		nOut = ((ulOut << 0) & 0x0000FFFF) | (OutData & 0xFFFF0000);
	}
	else if (nSegment == 3)
	{
		nOut = ((ulOut << 16) & 0xFFFF0000) | (OutData & 0x0000FFFF);
	}
	else
		return FALSE;
	
	m_pMotionCard->Out32(nOut);
	return TRUE;
}

BOOL CMotionNew::IsOutSliceIo(int nSegment, int nBit)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	
#ifdef RI_SYSTEM
	return FALSE;
#endif

	if (nSegment == 3)
		nBit += 16;
	else if (nSegment != 2)
		return FALSE;

	return (m_pMotionCard->ReadOut(nBit));
}

//160512 lgh add
unsigned long CMotionNew::GetSliceOut(int nSegment)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	long OutData = m_pMotionCard->ReadOut();
	long nOut = 0;

	if (nSegment == 2)
	{
		nOut = ((OutData & 0x0000FFFF) >> 0);
	}
	else if (nSegment == 3)
	{
		nOut = ((OutData & 0xFFFF0000) >> 16);
	}
	else
		return FALSE;

	return ((unsigned long)nOut);
}




BOOL CMotionNew::CheckScanEnd()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
#ifdef LIGHT_AXIS
	if (CheckAxisDone(SCAN_AXIS) && CheckAxisDone(CAM_AXIS) && CheckAxisDone(LIGHT_AXIS))
	{
#else
	if (CheckAxisDone(SCAN_AXIS) && CheckAxisDone(CAM_AXIS))
	{
#endif
		// pIO->ExtTriggerPulseOnOff(OFF);  // 20101207 ljg delete
		return TRUE;
	}
	pMainFrame->RefreshWindows(1);
#endif
	return FALSE;
}

BOOL CMotionNew::WaitHomeEndFocus(DWORD dwTimeOver)
{
#ifdef MOTION_ACTION	
	DWORD dwStartTick = GetTickCount();
	g_bLoopWh = TRUE;
	while (g_bLoopWh)
	{


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif

		if (m_pMotionCard->m_nInterlockStatus)
		{
			return OPERATE_INTERLOCK;
		}
#if MACHINE_MODEL == INSMART_R2R_SUMITOMO
		if (pIO->CheckSaftyOnlyBit())
		{
			return OPERATE_INTERLOCK;
		}
#endif
#ifdef FOCUS2_AXIS
		if (CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(FOCUS2_AXIS))
			break;
#else
#if MACHINE_MODEL == INSPRO10S_UpGrade
		if (CheckOriginRet(FOCUS_AXIS))
			break;
#else
		// 120113 jsy NO_MAG_AXIS
#ifdef NO_MAG_AXIS
		if (CheckOriginRet(FOCUS_AXIS))
			break;
#else
#ifdef INSP_ZOOM_AXIS
		if (CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(MAG_AXIS) && CheckOriginRet(INSP_ZOOM_AXIS))
			break;
#else
		if (CheckOriginRet(FOCUS_AXIS) && CheckOriginRet(MAG_AXIS))
			break;
#endif
#endif
#endif
#endif
		else if (pIO->CheckEmgSwitch())
		{
			return (EMERGENCY_STOP);
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			return (OPERATE_INTERLOCK);
		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
			return (USER_STOP);
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				return (WAIT_TIME_OVER);
			}
		}



		//160104 lgh for ui freeze
		pGlobalView->RefreshWindows(1);
		pMainFrame->RefreshWindows(1);
	}
#endif
	return SUCCESS;
}


extern BOOL g_bExit;
BOOL CMotionNew::InitServoMotor()
{
	//160128 LGH 
	AoiParam()->m_bInitServoMotor=FALSE;
	/////////////////////////////////////////////
	// 2. Motion Initialize
	/////////////////////////////////////////////

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strTitleMsg,strMsg;
	/////////////////////////////////////////////
	// 2. Motion Initialize
	/////////////////////////////////////////////

	int nAxisId;
	INT nAct;

	strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "SERVO_DRIVER_INITIALIZE"));

	//for(nAxisId=0; nAxisId<MAX_AXIS ;nAxisId++)
	for(nAxisId=MAX_AXIS-1; nAxisId>=0 ;nAxisId--)
	{
		if(g_bExit)
			return FALSE;

#ifdef USE_ATF_MODULE
		if(nAxisId == FOCUS_AXIS || nAxisId == ZOOM_AXIS)
			continue;
#endif
		MotionAbort(nAxisId);		// Command position??Actual Position?? ??????????.

		nAct = (INT)E_STOP_EVENT;
		m_pMotionCard->GetAxis(nAxisId)->SetNegHWLimitAction(nAct);
		m_pMotionCard->GetAxis(nAxisId)->SetPosHWLimitAction(nAct);
		//m_pMotionCard->SetPosHWLimitAction(nAxisId, MPIActionE_STOP);
		//m_pMotionCard->SetNegHWLimitAction(nAxisId, MPIActionE_STOP);
		//m_pMotionCard->SetPosHWLimitAction(nAxisId, MPIActionNONE);
		//m_pMotionCard->SetNegHWLimitAction(nAxisId, MPIActionNONE);
		Sleep(100);

		ClearMotionFault(nAxisId); // AMP?? idle????? ??????
		strMsg.Format(_T("%s[%d] %s"), GetMotorName(nAxisId), nAxisId, pGlobalView->GetLanguageString("MOTION", "AXIS_SERVO_ON"));
#ifdef TENSION_AXIS
		if(nAxisId != TENSION_AXIS)
			ServoOnOff(nAxisId,ON);		// Amp Enable
		else
			ServoOnOff(nAxisId,OFF);	// Amp Disable
#else
		ServoOnOff(nAxisId,ON);		// Amp Enable
#endif
		pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_LTGREEN);
		Sleep(200);
		

		BOOL bForceInPlug = 0;

#ifdef IBIDEN_MANUAL_INLINE
		if (!pGlobalDoc->m_bUseInlineAutomation)
		{
			bForceInPlug = !pIO->ReadBit(15);
		}
#endif

		//초기화 하는 도중에 이머픸E?걸툈E뒤져야한다.
		if(bForceInPlug || pIO->ReadBit(EMERGENCY_SWITCH) || pIO->CheckSaftyOnlyBit())
		{
			BOOL bSafety = pIO->CheckSaftyOnlyBit();

			pGlobalView->m_bDestroy=TRUE;
			pGlobalView->ClearDispMessage();

#if MACHINE_MODEL == INSMART_R2R_SUMITOMO
			if(!bSafety)
			{				
				strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "WARNING"));
				if(AoiParam()->m_bRTRDoorInterlock)
					strMsg = _T("[MSG536] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG3");
				else
					strMsg = _T("[MSG559] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG9");
			}
			else
			{
				strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "WARNING"));
				strMsg = _T("[MSG568] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG14");
			}

			AfxMessageBox(strMsg,MB_SETFOREGROUND | MB_ICONSTOP);
#else
			strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "FATAL_ERROR"));
			strMsg.Format(_T("%s %s\r\n %s"),
						GetMotorName(nAxisId),
						pGlobalView->GetLanguageString("MOTION", "AMP_FAULT_DETECT"),
						pGlobalView->GetLanguageString("MOTION", "CHECK_SERVO_DRIVER"));
			AfxMessageBox(strMsg);

			strMsg.Format(_T("%s"),	pGlobalView->GetLanguageString("SYSTEM", "PROGRAM_SHUTDOWN"));
			AfxMessageBox(strMsg);
#endif
			//DispMessage(strTitleMsg,strMsg,RGB_RED,3000);
			//WaitDispMsgClear();
			//if(pGlobalView->GetSafeHwnd())
			if(this->GetSafeHwnd() && m_bExit==0)
			{
				DestroyWindow();
				m_bExit =TRUE;
			}

			pGlobalView->ExitProgram();

			return FALSE;			
		}
	}

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "CHECK_SERVO_STATUS"));
	pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_LTGREEN);
	for(int nAxisID = 0; nAxisID < MAX_AXIS; nAxisID++)
	{
#ifdef USE_ATF_MODULE
		if(nAxisID == FOCUS_AXIS || nAxisID == ZOOM_AXIS)
			continue;
#endif
		// note servo driver?
		if(IsAmpFault(nAxisID))
		{
			BOOL bForceInPlug = 0;

#ifdef IBIDEN_MANUAL_INLINE
			if (!pGlobalDoc->m_bUseInlineAutomation)
			{
				bForceInPlug = !pIO->ReadBit(15);
			}
#endif

			//초기화 하는 도중에 이머픸E?걸툈E뒤져야한다.
			if(bForceInPlug || pIO->ReadBit(EMERGENCY_SWITCH) || pIO->CheckSaftyOnlyBit() || pIO->CheckIBIDoorStatusNoMessage() == 0)
			{
				BOOL bSafety = pIO->CheckSaftyOnlyBit();

				pGlobalView->m_bDestroy=TRUE;
				pGlobalView->ClearDispMessage();

#if MACHINE_MODEL == INSMART_R2R_SUMITOMO
				if(!bSafety)
				{				
					strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "WARNING"));
					if(AoiParam()->m_bRTRDoorInterlock)
						strMsg = _T("[MSG536] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG3");
					else
						strMsg = _T("[MSG559] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG9");
				}
				else
				{
					strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "WARNING"));
					strMsg = _T("[MSG568] ") + pGlobalView->GetLanguageString("SAFETY SENSOR", "MSG14");
				}

				AfxMessageBox(strMsg,MB_SETFOREGROUND | MB_ICONSTOP);
#else
				strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "FATAL_ERROR"));
				strMsg.Format(_T("%s %s\r\n %s"),
					GetMotorName(nAxisId),
					pGlobalView->GetLanguageString("MOTION", "AMP_FAULT_DETECT"),
					pGlobalView->GetLanguageString("MOTION", "CHECK_SERVO_DRIVER"));
				AfxMessageBox(strMsg);

				strMsg.Format(_T("%s"),	pGlobalView->GetLanguageString("SYSTEM", "PROGRAM_SHUTDOWN"));
				AfxMessageBox(strMsg);
#endif
				//DispMessage(strTitleMsg,strMsg,RGB_RED,3000);
				//WaitDispMsgClear();
				//if(pGlobalView->GetSafeHwnd())
				if(this->GetSafeHwnd() && m_bExit==0)
				{
					DestroyWindow();
					m_bExit =TRUE;
				}

				pGlobalView->ExitProgram();

				return FALSE;			
			}

				//Motion: Amp Fault 3
			pMainFrame->SetEESAlarm(ALARM_MOTION_FAULT,EES_ACTIVATE_ALARM);

				//Motion: 서보 드라이퉩E체크 8
			pMainFrame->SetEESAlarm(ALARM_MOTION_SERVODRIVE,EES_ACTIVATE_ALARM);

			strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("SYSTEM", "FATAL_ERROR"));
			strMsg.Format(_T("[MSG153] %s %s\r\n %s"),
						GetMotorName(nAxisID),
						pGlobalView->GetLanguageString("MOTION", "AMP_FAULT_DETECT"),
						pGlobalView->GetLanguageString("MOTION", "CHECK_SERVO_DRIVER"));
			pGlobalView->ClearDispMessage();
			AfxMessageBox(strMsg,MB_SETFOREGROUND);
//			pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_RED);
			//pGlobalView->WaitDispMsgClear();

			pMainFrame->SetEESAlarm(ALARM_MOTION_FAULT,EES_DEACTIVATE_ALARM);

			//Motion: 서보 드라이퉩E체크 8
			pMainFrame->SetEESAlarm(ALARM_MOTION_SERVODRIVE,EES_DEACTIVATE_ALARM);
				pGlobalView->ClearDispMessage();
			strMsg.Format(_T("%s"),	pGlobalView->GetLanguageString("SYSTEM", "PROGRAM_SHUTDOWN"));
			//pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_RED,3000);
			//pGlobalView->WaitDispMsgClear();
			AfxMessageBox(strMsg,MB_SETFOREGROUND);


			if(this->GetSafeHwnd() && m_bExit==0)
			{
				DestroyWindow();
				m_bExit =TRUE;
			}
			if(::IsWindow(pGlobalView->GetSafeHwnd()))
			{
				pGlobalView->ExitProgram();
				//if(pGlobalView->GetSafeHwnd())
				//pGlobalView->DestroyWindow();
			}

//			pMainFrame->PostMessage(WM_CLOSE,0,0);			
			return FALSE;
		}
	}

	if (m_bUseGantry)
	{
		GantryEnable(m_lGantryMaster, m_lGantrylSlave, m_lGantryEnable);
	}


	pGlobalView->ClearDispMessage();
#endif // MOTION_ACTION		

	DebugStringMessage(_T("InitServoMotor End"),__FILE__,__LINE__);
	AoiParam()->m_bEmgStatusRestored=TRUE;
	AoiParam()->m_bInitServoMotor=TRUE;

	return TRUE;
}


BOOL CMotionNew::InitServoMotorNoDestroy()
{

	//160128 LGH 
	AoiParam()->m_bInitServoMotor = FALSE;
	/////////////////////////////////////////////
	// 2. Motion Initialize
	/////////////////////////////////////////////

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	CString strTitleMsg, strMsg;
	/////////////////////////////////////////////
	// 2. Motion Initialize
	/////////////////////////////////////////////

	int nAxisId;
	INT nAct;

	strTitleMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "SERVO_DRIVER_INITIALIZE"));

	//for(nAxisId=0; nAxisId<MAX_AXIS ;nAxisId++)
	for (nAxisId = MAX_AXIS - 1; nAxisId >= 0; nAxisId--)
	{
		if (g_bExit)
			return FALSE;

#ifdef USE_ATF_MODULE
		if (nAxisId == FOCUS_AXIS || nAxisId == ZOOM_AXIS)
			continue;
#endif
		MotionAbort(nAxisId);		// Command position??Actual Position?? ??????????.

		//m_pMotionCard->SetPosHWLimitAction(nAxisId, MPIActionE_STOP);
		//m_pMotionCard->SetNegHWLimitAction(nAxisId, MPIActionE_STOP);
		nAct = (INT)E_STOP_EVENT;
		m_pMotionCard->GetAxis(nAxisId)->SetNegHWLimitAction(nAct);
		m_pMotionCard->GetAxis(nAxisId)->SetPosHWLimitAction(nAct);

		Sleep(100);

		ClearMotionFault(nAxisId); // AMP?? idle????? ??????
		strMsg.Format(_T("%s[%d] %s"), GetMotorName(nAxisId), nAxisId, pGlobalView->GetLanguageString("MOTION", "AXIS_SERVO_ON"));
#ifdef TENSION_AXIS
		if (nAxisId != TENSION_AXIS)
			ServoOnOff(nAxisId, ON);		// Amp Enable
		else
			ServoOnOff(nAxisId, OFF);	// Amp Disable
#else
		ServoOnOff(nAxisId, ON);		// Amp Enable
#endif
		pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
		Sleep(200);

		BOOL bForceInPlug = 0;

#ifdef IBIDEN_MANUAL_INLINE
		if (!pGlobalDoc->m_bUseInlineAutomation)
		{
			bForceInPlug = !pIO->ReadBit(15);
		}
#endif

		//초기화 하는 도중에 이머픸E?걸툈E뒤져야한다.
		if (bForceInPlug || pIO->ReadBit(EMERGENCY_SWITCH) || pIO->CheckSaftyOnlyBit())
		{
			return FALSE;
		}

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pIO->CheckShinkoDoorOpen())
			{
				return FALSE;
			}
		}
	}

	strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "CHECK_SERVO_STATUS"));
	pGlobalView->DispMessage(strTitleMsg, strMsg, RGB_LTGREEN);
	for (int nAxisID = 0; nAxisID < MAX_AXIS; nAxisID++)
	{
#ifdef USE_ATF_MODULE
		if (nAxisID == FOCUS_AXIS || nAxisID == ZOOM_AXIS)
			continue;
#endif
		// note servo driver?
		if (IsAmpFault(nAxisID))
		{
			BOOL bForceInPlug = 0;

#ifdef IBIDEN_MANUAL_INLINE
			if (!pGlobalDoc->m_bUseInlineAutomation)
			{
				bForceInPlug = !pIO->ReadBit(15);
			}
#endif

			//초기화 하는 도중에 이머픸E?걸툈E뒤져야한다.
			if (bForceInPlug || pIO->ReadBit(EMERGENCY_SWITCH) || pIO->CheckSaftyOnlyBit())
			{
				return FALSE;
			}	
			return FALSE;
		}

		if (pGlobalDoc->m_bUseInlineAutomation)
		{
			if (pIO->CheckShinkoDoorOpen())
			{
				return FALSE;
			}
		}
	}

	pGlobalView->ClearDispMessage();
#endif // MOTION_ACTION		


	DebugStringMessage(_T("InitServoMotor End"), __FILE__, __LINE__);
	AoiParam()->m_bEmgStatusRestored = TRUE;
	AoiParam()->m_bInitServoMotor = TRUE;

	return TRUE;
}


BOOL CMotionNew::StopMove(int nAxisId, double fStopTime, BOOL bWait)
{
	if (nAxisId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	//return Stop(nAxisId, fStopTime);

	//float fEStopTime = m_pMotionCard->GetEStopTime(nAxisId);
	//m_pMotionCard->SetEStopTime(nAxisId, fEStopTime);
	EStop(nAxisId);
	if (GetAxisState(nAxisId) == MPIStateERROR)
	{
		Clear(nAxisId);
		Sleep(30);
	}
	if (!IsServoOn(nAxisId))
	{
		ServoOnOff(nAxisId, TRUE);
		Sleep(30);
	}
	if (GetAxisState(nAxisId) == MPIStateERROR)
	{
		Clear(nAxisId);
		Sleep(30);
	}
	//m_pMotionCard->SetEStopTime(nAxisId, fEStopTime);
	return TRUE;
}

double CMotionNew::CheckThetaLimit(double fAdjAngle)
{
#if MACHINE_MODEL == INSPRO10S_PLUS || MACHINE_MODEL == INSPRO20S || MACHINE_MODEL == INSPRO20S_PLUS || MACHINE_MODEL == INSPRO30S || MACHINE_MODEL == INSPRO30S_PLUS || MACHINE_MODEL == INSPRO40S || MACHINE_MODEL == INSPRO40S_PLUS_30S || MACHINE_MODEL == INSPRO40S_1IPU || MACHINE_MODEL == INSPRO30S_PLUS_AF || MACHINE_MODEL == INSPRO30S_PLUS_PRIMIUM 
	double fAdjCoeff = 100.0 / (pGlobalDoc->m_fThetaCoeff*1000.);
	double fTgtPos = -(fAdjAngle*fAdjCoeff);

	// 위치 데이터가 -퉩EE한계를 초과하는지를 조퍊E
	if (fTgtPos <= pMotionNew->m_pParamMotor[THETA_AXIS].fNegLimit)
	{
		fTgtPos = m_pParamMotor[THETA_AXIS].fNegLimit + 0.01;
		double fModifiedAngle = -(fTgtPos / fAdjCoeff);
		return (fModifiedAngle);

	}
	// 위치 데이터가 +퉩EE한계를 초과하는지를 조퍊E
	if (fTgtPos >= m_pParamMotor[THETA_AXIS].fPosLimit)
	{
		fTgtPos = m_pParamMotor[THETA_AXIS].fPosLimit - 0.01;
		double fModifiedAngle = -(fTgtPos / fAdjCoeff);
		return (fModifiedAngle);
	}
	return (0.0);
#elif MACHINE_MODEL == INSPRO_GM_PLUS || MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSMART_ULTRA
	double fAdjCoeff = 100.0/(pGlobalDoc->m_fThetaCoeff*1000.);
	double fTgtPos = -(fAdjAngle*fAdjCoeff);

	// 위치 데이터가 -퉩EE한계를 초과하는지를 조퍊E
	if (fTgtPos <= m_pParamMotor[SCAN_S_AXIS].fNegLimit)
	{
		fTgtPos = m_pParamMotor[SCAN_S_AXIS].fNegLimit + 0.01;
		double fModifiedAngle = -(fTgtPos / fAdjCoeff);
		return (fModifiedAngle);

	}
	// 위치 데이터가 +퉩EE한계를 초과하는지를 조퍊E
	if (fTgtPos >= m_pParamMotor[SCAN_S_AXIS].fPosLimit)
	{
		fTgtPos = m_pParamMotor[SCAN_S_AXIS].fPosLimit - 0.01;
		double fModifiedAngle = -(fTgtPos / fAdjCoeff);
		return (fModifiedAngle);
	}
	return (0.0);
#else
	return (0.0);
#endif
}

BOOL CMotionNew::IsLimitError(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	return 0;

	//return m_pMotionCard->IsLimitError(nMotorId);
}

BOOL CMotionNew::IsNegSWLimit(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	return (GetActualPosition(nMotorId) <= m_pMotionCard->GetAxis(nMotorId)->GetNegSoftwareLimit()) ? TRUE : FALSE;
}

BOOL CMotionNew::IsPosSWLimit(int nMotorId)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	return (GetActualPosition(nMotorId) >= m_pMotionCard->GetAxis(nMotorId)->GetPosSoftwareLimit()) ? TRUE : FALSE;
	//return m_pMotionCard->IsPosSWLimit(nMotorId);
}

long CMotionNew::CheckError(int nMotorId, long errNo)
{
	if (nMotorId >= MAX_AXIS)
		return FALSE;

	switch (errNo)
	{
	//case MPIMessageOK:
	//case MPIMotionMessageAUTO_START:
	//	errNo = 0;
	//	break;
	default:
		if (!pIO->CheckEmgSwitch() && !pGlobalDoc->m_bOperateInterlock)
		{
			CString strError;
			pMainFrame->SetEESAlarm(ALARM_MOTION_SYSTEM, EES_ACTIVATE_ALARM);
			strError = _T("[MSG874]") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "STR_FORMAT", "CheckError_P1_M1");
			//m_strError.Format(_T("%s %s(0x%x): %s"), GetMotorName(nMotorId), strError, errNo, CharToString(mpiMessage(errNo, NULL)));
			m_strError.Format(_T("%s %s(0x%x)"), GetMotorName(nMotorId), strError, errNo);
			AfxMessageBox(m_strError, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			m_strError = "";
			pMainFrame->SetEESAlarm(ALARM_MOTION_SYSTEM, EES_DEACTIVATE_ALARM);
			break;
		}
		else
			errNo = 0;
	}

	return errNo;
}


// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Distance From Laser Point To Inspect Cam [mm], 4. Current Focus Pos [mm], 5. Write Trace Data, 6. Limit Change Value of Interval Focus Pos [mm]
BOOL CMotionNew::InitInspectCamAutoFocusingWithLaserSensor(double dScanVel, double dSampleInterval, double dOffsetInspCamLaser, double dOrgFocusPos, BOOL bWriteTrace, double dErrRange) 	//20121125-ndy for 2Head Laser Control
{
	if(pGlobalDoc->m_bUseAutoFocusSmoothing)
		m_pMotionCard->ClearAutoFocusSmoothingData();

	m_pMotionCard->m_bStartAfSeq = TRUE;
	m_pMotionCard->m_bWriteTrace = bWriteTrace;
	
	m_pMotionCard->m_nSeqIdx = 0;
	m_pMotionCard->m_dCmdTime = dSampleInterval / dScanVel; // Scan Axis Interval Time.
	m_pMotionCard->m_dSampleInterval = dSampleInterval;
	m_dOffsetInspCamLaser = dOffsetInspCamLaser;
	m_dOrgFocusPos = dOrgFocusPos;
	m_pMotionCard->m_dErrRange = dErrRange;
	m_dScanVel = dScanVel;

	//m_pMotionCard->StopSequence();
	//m_pMotionCard->ClearUserBuffer();

	pGlobalDoc->m_nLSTNum = 0;

	for (int i = 0; i<MAX_LASER_SENSOR_PULSE_NUM; i++)
	{
		pGlobalDoc->m_nLSTSeqIdx[i] = 0;
		pGlobalDoc->m_nLSTScanMotPos[i] = 0;
		pGlobalDoc->m_fLSTSamplePosOfScanAxis[i] = 0.0;
		pGlobalDoc->m_fLSTTargetPosOfFocusAxis[i] = 0.0;
		pGlobalDoc->m_fLSTActualPosScanAxis[i] = 0.0;
		pGlobalDoc->m_fLSTActualPosFocusAxis[i] = 0.0;
		pGlobalDoc->m_nLSTValid[i] = 0;
	}

	//m_pMotionCard->Grouping2Ax(m_nBoardId, m_nGroupID, SCAN_AXIS, FOCUS_AXIS);
	//return m_pMotionCard->BufferInit();
	return m_pMotionCard->InitListMotion();

//	return TRUE;
}

// 1. Scan Velocity [mm/sec], 2. Interval Distance [mm], 3. Current Focus Pos [mm], 4. Limit Change Value of Interval Focus Pos [mm]
BOOL CMotionNew::InitInspectCamAutoFocusingWithLaserSensor2(double dScanVel, double dSampleInterval, double dOffsetInspCamSol, double dOrgFocusPos, double dErrRange, BOOL bBiDir)
{
	if(pGlobalDoc->m_bUseAutoFocusSmoothing)
		m_pMotionCard->ClearAutoFocusSmoothingData();
	
	m_pMotionCard->m_bStartAfSeq = TRUE;
		
	m_pMotionCard->m_nSeqIdx = 0;
	m_pMotionCard->m_dCmdTime = dSampleInterval / dScanVel; // Scan Axis Interval Time.
	m_pMotionCard->m_dSampleInterval = dSampleInterval;
	m_pMotionCard->m_dOffsetInspCamSol = dOffsetInspCamSol;
	m_dOrgFocusPos = dOrgFocusPos;
	m_pMotionCard->m_dErrRange = dErrRange;
	m_dScanVel = dScanVel;
	
	//m_dOrgThickPos = pGlobalDoc->m_dThickPosOnSolAxisZero;
		
	//m_pMotionCard->StopSequence();
	//m_pMotionCard->ClearUserBuffer();
		
	//m_pMotionCard->Grouping2Ax(m_nBoardId, m_nGroupID, SCAN_AXIS, FOCUS_AXIS);
	//return m_pMotionCard->BufferInit();
	return m_pMotionCard->InitListMotion();

//	return TRUE;
}


// 지정된 축의 리미트센서 위치를 파악하여 소프트웨푳E리미트 값, 햨E최큱E스트로크 값을 결정하여 리턴한다.
int CMotionNew::GetAxisStroke(int nMotorId, double fPlusMargin, double fMinusMargin)
{
	if (nMotorId >= MAX_AXIS)
		return 0;

		int errCode=0;
		INT nAct = (INT)E_STOP_EVENT;
		//INT nAct = (INT)NO_EVENT;
	
	#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return 0;
		}
	#endif
	
		if(!IsServoOn(nMotorId))
			return FALSE;
	
		double fPosLimit,fNegLimit;
		CString strTitleMsg,strMsg,strDir;
	
		strTitleMsg.Format(_T("%s %s"), GetMotorName(nMotorId), pGlobalView->GetLanguageString("MOTION", "STROKE_CHECK"));
		if(m_pParamMotion[nMotorId].Home.nDir==PLUS)
			strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "POS_STROKE_TEST"));
		else
			strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "NEG_STROKE_TEST"));
	
		pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_LTGREEN);
	
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		//m_pMotionCard->SetPosHWLimitDirection(nMotorId, TRUE);
		//m_pMotionCard->SetNegHWLimitDirection(nMotorId, TRUE);
	#endif
		ClearMotionFault(nMotorId);
		while (!IsMotionDone(nMotorId))
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
		// software limit value
		ChangePosSWLimitValue(nMotorId, 10000.0);
		ChangeNegSWLimitValue(nMotorId, -10000.0);
	
		//m_pMotionCard->SetPosSWLimitAction(nMotorId, MPIActionNONE);
		//m_pMotionCard->SetNegSWLimitAction(nMotorId, MPIActionNONE);
	
		double dTempV;
		if(m_pParamMotion[nMotorId].Home.fEscLen < 0.0)
			dTempV = -m_pParamMotion[nMotorId].Home.fEscLen;
		else
			dTempV = m_pParamMotion[nMotorId].Home.fEscLen;
	
	
		//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉救다.
//		nAct = (INT)NO_EVENT;
		//nAct = (INT)E_STOP_EVENT;
		//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
		//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		//Sleep(100);

		if(GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
		{
			ClearMotionFault(nMotorId);
			StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f2ndSpd*-m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
			do{
				//160104 lgh for ui freeze
				pGlobalView->RefreshWindows(1);
				pMainFrame->RefreshWindows(1);
			}while (GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir));
			Stop(nMotorId, 0.1);
			while(!IsMotionDone(nMotorId))
			{
				//160104 lgh for ui freeze
				pGlobalView->RefreshWindows(1);
				pMainFrame->RefreshWindows(1);
			}
			ClearMotionFault(nMotorId);
		}
	
		//2. 원점 limit센서가 Check
		// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 
		//nAct = (INT)E_STOP_EVENT;
		//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
		//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		//Sleep(100);

		StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
		while(!GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
		while (!IsMotionDone(nMotorId))
		{
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionNONE);
			//nAct = (INT)NO_EVENT;
			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#ifdef CHUCK_AXIS
		if (nMotorId == CHUCK_AXIS)
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionNONE);
			//nAct = (INT)NO_EVENT;
			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	#endif
	
		ClearMotionFault(nMotorId);
	
	#ifdef SCAN_S_AXIS
		if(nMotorId == SCAN_AXIS)
			ClearMotionFault(SCAN_S_AXIS);
	#endif
	
		//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
		StartPosMove(nMotorId, dTempV*(-m_pParamMotion[nMotorId].Home.nDir),m_pParamMotion[nMotorId].Home.f2ndSpd,m_pParamMotion[nMotorId].Home.fAcc,m_pParamMotion[nMotorId].Home.fAcc,INC,NO_WAIT);
		while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionE_STOP);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionE_STOP);
			//nAct = (INT)E_STOP_EVENT;
			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#ifdef CHUCK_AXIS
		if (nMotorId == CHUCK_AXIS)
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionE_STOP);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionE_STOP);
			//nAct = (INT)E_STOP_EVENT;
			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	#endif
	
		//4. -축을 제외한 다른위치에 모타가 있을때 
		// -방향으로 움직이면서 일단 -Limit를 찾음 
		// Move to the negative limit : 
		// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
		// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
		//2008,08,13 modify by khc
		//float fEStopTime = m_pMotionCard->GetEStopTime(nMotorId);
		double fSpeed,fMachineSpeed,fENewStopTime;
		double fTemp= (double)m_pParamMotor[nMotorId].fLeadPitch/ (double)m_pParamMotor[nMotorId].nEncPulse;
		if(fTemp<=0.001)
		{
			fSpeed = (1.0*1000.0)/(m_fPosRes[nMotorId]*1000); // [mm/s]
			fMachineSpeed = PulseToLength(nMotorId, fSpeed)/2.0;
			fENewStopTime = float(fMachineSpeed/1000.);
	
			if(fMachineSpeed * fENewStopTime < m_fPosRes[nMotorId])	//20120601-ndy for ConvexDriver : Velocity * Time = Distance & Distance > LinearScaler Position Resolution
				fENewStopTime = fENewStopTime * 100;
		}
		else
		{
			fMachineSpeed = (m_fPosRes[nMotorId]*1000)/2.0; // [mm/s]
			fENewStopTime = float(fMachineSpeed/1000.);
	
			if(fMachineSpeed * fENewStopTime < m_fPosRes[nMotorId])	//20120601-ndy for ConvexDriver
				fENewStopTime = fENewStopTime * 100;
		}
		//m_pMotionCard->SetEStopTime(nMotorId, fENewStopTime);	
	
	
		double dPos = GetActualPosition(nMotorId);
		// 홈방향으로 정밀 센싱을 함.
		BOOL bMove = 0;
		StartVelMove(nMotorId, fMachineSpeed*m_pParamMotion[nMotorId].Home.nDir,10.0*fMachineSpeed);
		while(!GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
	
		}
	
		// 홈퉩EE위치를 체크.
		if(m_pParamMotion[nMotorId].Home.nDir==PLUS)
			fPosLimit = GetActualPosition(nMotorId);
		else
			fNegLimit = GetActualPosition(nMotorId);
	
		Sleep(100);
	
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		//m_pMotionCard->SetPosHWLimitDirection(nMotorId, TRUE);
		//m_pMotionCard->SetNegHWLimitDirection(nMotorId, TRUE);
	
	#ifdef SCAN_S_AXIS
		if (nMotorId == SCAN_AXIS)
		{
			//m_pMotionCard->SetPosHWLimitDirection(SCAN_S_AXIS, TRUE);
			//m_pMotionCard->SetNegHWLimitDirection(SCAN_S_AXIS, TRUE);
		}
	#endif
	#endif
	
		ClearMotionFault(nMotorId);
	
	#ifdef SCAN_S_AXIS
		if (nMotorId == SCAN_AXIS)
		{
			if (!MoveScanMotor(200.0, 25.0, WAIT))
				return FALSE;

			while (!IsMotionDone(SCAN_AXIS));
			Sleep(300);

			if (!GetThetaStrokeForGm(0.005, 0.005))
				return FALSE;
		}
	#endif
	
		ClearMotionFault(nMotorId);
		//m_pMotionCard->SetEStopTime(nMotorId, fEStopTime);
	
		if(m_pParamMotion[nMotorId].Home.nDir==PLUS)
			strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "NEG_STROKE_TEST"));
		else
			strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "POS_STROKE_TEST"));
	
		pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_LTGREEN);
	
		//5. 원점 limit센서가 Check되햨E않은 위치에 모타가 있을때 
		// 원점 복귀 방향으로 움직이면서 일단 원점반큱E센서를 찾음 
	
		StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*-m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
		while(!GetLimitInput(nMotorId, -m_pParamMotion[nMotorId].Home.nDir))
		{
	
	#ifdef SCAN_S_AXIS
			if (nMotorId == SCAN_AXIS)
			{
				if (m_pParamMotion[nMotorId].Home.nDir == MINUS)
				{
					if (bMove == 0 && GetActualPosition(nMotorId) - dPos >= 100)
					{
						bMove = 1;
						//m_pMotionCard->SetPosHWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
						//m_pMotionCard->SetNegHWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
						//nAct = (INT)E_STOP_EVENT;
						//m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
						//m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);
	
						ClearMotionFault(SCAN_S_AXIS);
	
						if (m_pParamMotion[nMotorId].Home.nDir == PLUS)
						{
							MoveThetaMotor(GetPosSWLimitValue(SCAN_S_AXIS) - 1, 1, NO_WAIT);
						}
						else
						{
							MoveThetaMotor(GetPosSWLimitValue(SCAN_S_AXIS) - 1, 1, NO_WAIT);
						}
	
						ClearMotionFault(SCAN_S_AXIS);
					}
				}
				else
				{
					if (bMove == 0 && dPos - GetActualPosition(nMotorId)>= 100)
					{
						bMove = 1;
						//m_pMotionCard->SetPosHWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
						//m_pMotionCard->SetNegHWLimitAction(SCAN_S_AXIS, MPIActionE_STOP);
						//nAct = (INT)E_STOP_EVENT;
						//m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetNegHWLimitAction(nAct);
						//m_pMotionCard->GetAxis(SCAN_S_AXIS)->SetPosHWLimitAction(nAct);
	
						ClearMotionFault(SCAN_S_AXIS);
	
						if (m_pParamMotion[nMotorId].Home.nDir == PLUS)
						{
							MoveThetaMotor(GetNegSWLimitValue(SCAN_S_AXIS) + 1, 1, NO_WAIT);
						}
						else
						{
							MoveThetaMotor(GetNegSWLimitValue(SCAN_S_AXIS) +1, 1, NO_WAIT);
						}
	
						ClearMotionFault(SCAN_S_AXIS);
					}
				}
			}
	#endif
	
	#ifdef SCAN_AXIS
	#ifdef SCAN_S_AXIS
			if (nMotorId == SCAN_AXIS)
			{
				if(GetLimitInput(SCAN_S_AXIS, -m_pParamMotion[nMotorId].Home.nDir))
				
					break;
			}
	#endif
	#endif
	
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
		while (!IsMotionDone(nMotorId));
	
		//nAct = (INT)NO_EVENT;
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetPosHWLimitDirection(nMotorId, TRUE);
			//m_pMotionCard->SetNegHWLimitDirection(nMotorId, TRUE);

			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	
		ClearMotionFault(nMotorId);
	
		// 원점방향으로 fEscLen만큼 벗엉毆.
		StartPosMove(nMotorId, dTempV*(m_pParamMotion[nMotorId].Home.nDir),m_pParamMotion[nMotorId].Home.f2ndSpd,m_pParamMotion[nMotorId].Home.fAcc,m_pParamMotion[nMotorId].Home.fAcc,INC,NO_WAIT);
		while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
		//nAct = (INT)E_STOP_EVENT;
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionE_STOP);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionE_STOP);

			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	
		//4. -축을 제외한 다른위치에 모타가 있을때 
		// -방향으로 움직이면서 일단 -Limit를 찾음 
		// Move to the negative limit : 
		// 위치 정도 5um , 센서 응큱E주파펯E1kHz , 엔코큱E해상도 0.001mm
		// 이동속도 = 위치 정도 * 센서 반응시간 * 엔코큱E위치)정도(단위mm)
	
		// 원점 반큱E센싱를 함.
		//	m_pMotionCard->SetEStopTime(nMotorId, fENewStopTime);	
		//	StartVelMove(nMotorId, fMachineSpeed*-m_pParamMotion[nMotorId].Home.nDir,10.0*fMachineSpeed);
		StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*-m_pParamMotion[nMotorId].Home.nDir,10.0*fMachineSpeed);
		while(!GetLimitInput(nMotorId, -m_pParamMotion[nMotorId].Home.nDir))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
		// 원점 반큱E씔?위치를 체크함.
		if(m_pParamMotion[nMotorId].Home.nDir==PLUS)
			fNegLimit = GetActualPosition(nMotorId);
		else
			fPosLimit = GetActualPosition(nMotorId);
	
		Sleep(100);
	
		//nAct = (INT)NO_EVENT;
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionNONE);
			//m_pMotionCard->SetPosHWLimitDirection(nMotorId, TRUE);
			//m_pMotionCard->SetNegHWLimitDirection(nMotorId, TRUE);

			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	
		ClearMotionFault(nMotorId);
		//m_pMotionCard->SetEStopTime(nMotorId, fEStopTime);
	
		//원점방향으로 fEscLen만큼 벗엉毆.
		StartPosMove(nMotorId, dTempV*(m_pParamMotion[nMotorId].Home.nDir),m_pParamMotion[nMotorId].Home.f2ndSpd,m_pParamMotion[nMotorId].Home.fAcc,m_pParamMotion[nMotorId].Home.fAcc,INC,NO_WAIT);
		while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
		{
			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
			{
				MotionAbort(nMotorId);
				return FALSE;
			}
			//160104 lgh for ui freeze
			pGlobalView->RefreshWindows(1);
			pMainFrame->RefreshWindows(1);
		}
	
		//nAct = (INT)E_STOP_EVENT;
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		if(GetMotorName(nMotorId)=="SCAN")
		{
			//m_pMotionCard->SetPosHWLimitAction(nMotorId, MPIActionE_STOP);
			//m_pMotionCard->SetNegHWLimitAction(nMotorId, MPIActionE_STOP);

			//m_pMotionCard->GetAxis(nMotorId)->SetNegHWLimitAction(nAct);
			//m_pMotionCard->GetAxis(nMotorId)->SetPosHWLimitAction(nAct);
		}
	#endif
	
		m_fPosLimit[nMotorId] = m_pParamMotor[nMotorId].fPosLimit = fPosLimit-fabs(fPlusMargin);
		m_fNegLimit[nMotorId] = m_pParamMotor[nMotorId].fNegLimit = fNegLimit+fabs(fMinusMargin);
	
		//m_pMotionCard->SetPosSWLimitValue(nMotorId, m_pParamMotor[nMotorId].fPosLimit);
		//m_pMotionCard->SetNegSWLimitValue(nMotorId, m_pParamMotor[nMotorId].fNegLimit);	
		//m_pMotionCard->SetPosSWLimitAction(nMotorId, MPIActionE_STOP);
		//m_pMotionCard->SetNegSWLimitAction(nMotorId, MPIActionE_STOP);
		m_pMotionCard->GetAxis(nMotorId)->SetNegSoftwareLimit(m_pParamMotor[nMotorId].fNegLimit, (INT)MPIActionE_STOP);
		m_pMotionCard->GetAxis(nMotorId)->SetPosSoftwareLimit(m_pParamMotor[nMotorId].fPosLimit, (INT)MPIActionE_STOP);
	
	#if MPI_VERSION_AOI == 030420	//20120601-ndy for ConvexDriver
		//m_pMotionCard->SetPosSWLimitDirection(nMotorId, TRUE);
		//m_pMotionCard->SetNegSWLimitDirection(nMotorId, TRUE);
	#endif
	
		strMsg.Format(_T("%s"), pGlobalView->GetLanguageString("MOTION", "FINISH_STROKE_TEST"));
		pGlobalView->DispMessage(strTitleMsg,strMsg,RGB_LTGREEN,200);

	return TRUE;
}




// MonitorThread ========================================================
UINT CMotionNew::MonitorThread(LPVOID pParam)
{
	CMotionNew* pMotionNewPtr = (CMotionNew*)pParam;
	DWORD dwMilliseconds = 10; // 10ms sec sleep
	pMotionNewPtr->m_bMonitorThreadAlive = TRUE;

	int nEmgCheckStatus = 0;
	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pMotionNewPtr->m_evtMonitorThread, dwMilliseconds))
	{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			continue;
		}
#endif

		switch (nEmgCheckStatus)
		{
		case 0:
			if (pIO->CheckEmgSwitch())
			{
				for (int nAxisId = 0; nAxisId < MAX_AXIS; nAxisId++)
					pMotionNewPtr->MotionAbort(nAxisId);
				nEmgCheckStatus = 1;
			}
			break;
		case 1:
			if (!pIO->CheckEmgSwitch())
				nEmgCheckStatus = 0;
			break;
		}
	}
	pMotionNew->m_bMonitorThreadAlive = FALSE;
	return 0;
}

void CMotionNew::WaitUntilMonitorThreadEnd(HANDLE hThread)
{
	TRACE("WaitUntilMonitorThreadEnd(0x%08x:MonitorThread) Entering...\n", hThread);
	//	MSG message;
	const DWORD dwTimeOut = 10000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while ((GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE) || m_bMonitorThreadAlive)
	{
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut)) {
			// 2009-04-02 jsy
			/*
			AfxMessageBox(_T("WaitUntilMonitorThreadEnd() Time Out!!!",MB_ICONSTOP|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST));
			*/
			pMainFrame->SetEESAlarm(ALARM_MOTION_MONITOR_THREAD, EES_ACTIVATE_ALARM);
			pGlobalView->m_strAfxMsg = pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "WaitUntilMonitorThreadEnd_P1_M1");
			AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_MONITOR_THREAD, EES_DEACTIVATE_ALARM);
			return;
		}
		/*
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
		if(!IsDialogMessage(&message))
		{
		TranslateMessage(&message);
		DispatchMessage(&message);
		}
		}
		*/
	}
	TRACE("WaitUntilMonitorThreadEnd(0x%08x:MonitorThread) Exit...\n", hThread);
}

void CMotionNew::StartMonitorThread()
{
	if (m_pMonitorThread == NULL)
	{
		m_evtMonitorThread.ResetEvent();
		m_pMonitorThread = AfxBeginThread(MonitorThread, this);
		if (m_pMonitorThread)
		{
			m_hMonitorThread = m_pMonitorThread->m_hThread;
		}
	}
}

void CMotionNew::StopMonitorThread()
{
	if (m_pMonitorThread != NULL)
	{
		DWORD dwSuspendCount = 0;
		do {
			dwSuspendCount = m_pMonitorThread->ResumeThread();
		} while (dwSuspendCount != 0 && dwSuspendCount != 0xffffffff);

		m_evtMonitorThread.SetEvent();
		WaitUntilMonitorThreadEnd(m_pMonitorThread->m_hThread);
	}
	m_pMonitorThread = NULL;
}
// MonitorThread ========================================================


BOOL CMotionNew::SetErrMap(CMyFileErrMap *pMyFile)
{
	return m_pMotionCard->ApplyErrorCompensateionTable(NMC_EC_AXISID, TRUE);

	//#ifndef RI_SYSTEM
	//	if(!pMyFile)
	//		pMyFile = m_pMyFileErrMap;
	//	return m_pMotionCard->SetErrMap(pMyFile);
	//
	//#else
	//	return TRUE;
	//#endif
}

BOOL CMotionNew::ResetErrMap()
{
	return m_pMotionCard->ApplyErrorCompensateionTable(NMC_EC_AXISID, FALSE);

	//#ifndef RI_SYSTEM
	//	return m_pMotionCard->ResetErrMap();
	//#else
	//	return TRUE;
	//#endif
}

BOOL CMotionNew::AdjustErrMapData(double dStdPosX, double dStdPosY)
{
	//if (m_pMyFileErrMap)
	//{
	//	double dAdjVal = GetAdjValErrMapData(dStdPosX, dStdPosY);
	//	if (!m_pMyFileErrMap->AdjustErrMapData(dAdjVal))
	//	{
	//		return FALSE;
	//	}
	//}
	//else
	//	return FALSE;

	return TRUE;
}

BOOL CMotionNew::LoadErrMapFile(CString sPath)
{
#ifdef UNICODE
	TCHAR sEcPath[MAX_PATH];
	_tcscpy(sEcPath, sPath);
	char *pText = TCHARToChar(sEcPath);
#else
	TCHAR pText[MAX_PATH];
	strcpy(pText, sPath);
#endif
	if (!m_pMotionCard)
		return FALSE;

	return m_pMotionCard->LoadErrorCompensationTable(NMC_EC_TYPE_FILE, NMC_EC_MAPID, pText);

	//if (!m_pMyFileErrMap)
	//	m_pMyFileErrMap = new CMyFileErrMap();

	//if (m_pMyFileErrMap)
	//{
	//	if (!m_pMyFileErrMap->Open(sPath))
	//	{
	//		delete m_pMyFileErrMap;
	//		m_pMyFileErrMap = NULL;
	//		return FALSE;
	//	}
	//}

	//else
	//	return FALSE;


	//CFileFind finder;
	//TCHAR szFile[MAX_PATH] = { 0, };
	//_stprintf(szFile, _T("%s"), sPath);

	//if (!finder.FindFile(szFile))
	//{
	//	m_sErrorMappingFilePath = _T("");
	//	return FALSE;
	//}
	//
	//m_sErrorMappingFilePath = sPath;
	//
	//return TRUE;
}

double CMotionNew::GetAdjValErrMapData(double dStdPosX, double dStdPosY)
{
	double dAdjVal = 0.0;

	if (m_pMyFileErrMap)
		dAdjVal = m_pMyFileErrMap->GetAdjValErrMapData(dStdPosX, dStdPosY);

	return dAdjVal;
}


double CMotionNew::GetThetaAngle(double dPos)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	double dAngle = 0.0;
#ifdef THETA_AXIS
	dAngle = asin(dPos / pGlobalDoc->m_fDistanceTableCenterAndThetaMotor) * 45.0 / atan(1.0);
#endif

#ifdef SCAN_S_AXIS
	dAngle = asin(dPos / pGlobalDoc->m_fDistanceScanAndScanS * 2.0) * 45.0 / atan(1.0);
#endif
	return dAngle;
}

BOOL CMotionNew::IsHomming(int nMotor)
{
	if (nMotor >= MAX_AXIS)
		return FALSE;

	return !m_bOrigin[nMotor];
}

BOOL CMotionNew::CheckThetaAxisRange(float fAdjAngle, float fMaxAngle)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef MOTION_ACTION	//20120306-ndy modified for RI

#ifdef THETA_AXIS
	if (!m_bOrigin[THETA_AXIS])
	{
		return FALSE;
	}

	// 회픸E각도를 위치 데이터로 변컖E
	double fCurPos = GetActualPosition(THETA_AXIS);
	double fAdjLength = pGlobalDoc->m_fDistanceTableCenterAndThetaMotor*sin(fAdjAngle*((double)atan(1.0)*4.0 / 180.0)); // modify by khc 20080909
	double fTgtPos = fCurPos - fAdjLength;
	fTgtPos = min(max(fTgtPos, m_pParamMotor[THETA_AXIS].fNegLimit), m_pParamMotor[THETA_AXIS].fPosLimit);
	double fModifiedAngle = asin((fCurPos - fTgtPos) / pGlobalDoc->m_fDistanceTableCenterAndThetaMotor) / ((double)atan(1.0)*4.0 / 180.0);
	if (fabs(fAdjAngle - fModifiedAngle) > fabs(fMaxAngle))
	{
		return FALSE;
	}

#endif


#ifdef SCAN_S_AXIS
	if (!m_bOrigin[SCAN_S_AXIS])
	{
		return FALSE;
	}

	// 회픸E각도를 위치 데이터로 변컖E
	double fCurPos = GetActualPosition(SCAN_S_AXIS);

	double fAdjLength = (pGlobalDoc->m_fDistanceScanAndScanS / 2.0)*sin(fAdjAngle*(atan(1.0) * 4 / 180.0)); 	// 20150212 - syd
																												//	double fAdjLength = pGlobalDoc->m_fDistanceScanAndScanS*sin((fAdjAngle/2.0)*(atan(1)*4/180.0)); // modify by khc 20080909
																												//	double fAdjLength = pGlobalDoc->m_fDistanceScanAndScanS*sin((fAdjAngle)*(atan(1)*4/180.0)); // modify by khc 20080909
	double fTgtPos = fCurPos - fAdjLength;
	fTgtPos = min(max(fTgtPos, m_pParamMotor[SCAN_S_AXIS].fNegLimit), m_pParamMotor[SCAN_S_AXIS].fPosLimit);
	double fModifiedAngle = asin((fCurPos - fTgtPos) / (pGlobalDoc->m_fDistanceScanAndScanS / 2.0)) / (atan(1.0) * 4 / 180.0);
	if (fabs(fAdjAngle - fModifiedAngle) > fabs(fMaxAngle))
	{
		return FALSE;
	}
#endif

#endif

	return TRUE;
}



// Setting Axis Start Position for Holding Motion
BOOL CMotionNew::HoldingMove(int nEventAxis, double dEventPos, int nHoldAxis, double dTagetPos, int nDir) // 20150122 - syd
{
	if (nEventAxis >= MAX_AXIS || nHoldAxis >= MAX_AXIS)
		return FALSE;

	long nEventPls = LengthToPulse(nEventAxis, dEventPos);
	BOOL bRtn = StartHoldingMove(nHoldAxis, nEventAxis, (double)nEventPls, dTagetPos, nDir, 50.0, TRUE);//, FALSE);
	if (!bRtn)
		return FALSE;

	return TRUE;
}

// 기존 홀딩 무브를 건드리지 않는다.
BOOL CMotionNew::HoldingMoveEx(int nEventAxis, double dEventPos, int nHoldAxis, double dTagetPos, int nDir) // 20150122 - syd
{
	if (nEventAxis >= MAX_AXIS || nHoldAxis >= MAX_AXIS)
		return FALSE;

	long nEventPls = LengthToPulse(nEventAxis, dEventPos);
	BOOL bRtn = StartHoldingMoveEx(nHoldAxis, nEventAxis, (double)nEventPls, dTagetPos, nDir, 50.0, TRUE);//, FALSE);
	if (!bRtn)
		return FALSE;

	return TRUE;
}



BOOL CMotionNew::SetTriggerStartPos(double fStartPos)  // fStartPos [Pls]
{
	// 이픸E?설정된 내퓖E갋동일할 경퓖E바로 리턴
	if (m_fStartPos == fStartPos || fStartPos < 0)
	{
		return TRUE;
	}

	// 새로퓖E설정 값을 멤퉩E변수에 저픸E
	m_fStartPos = fStartPos;

	int nStartPos = int(fStartPos + 0.5);

	int nPortValLSB, nPortValMSB;
	nPortValMSB = int((nStartPos) / 256);
	nPortValLSB = int((nStartPos) % 256);
	nPortValMSB = max(min(255, nPortValMSB), 0);
	nPortValLSB = max(min(255, nPortValLSB), 0);

	pGlobalView->TriggerPosSetByLightController(pGlobalDoc->m_LIGHT_TRIGGER_LSB, 255 - nPortValLSB);
	pGlobalView->TriggerPosSetByLightController(pGlobalDoc->m_LIGHT_TRIGGER_MSB, 255 - nPortValMSB);

	Sleep(100);

	return TRUE;
}

BOOL CMotionNew::StartHoldingMoveEx(int nHoldAxis, int nEventAxis, double dEventPos, double fPos, int nDir, double fVelRatio, BOOL bAbs, BOOL bWait, BOOL bOptimize, int bMotionType)
{
	if (nEventAxis >= MAX_AXIS || nHoldAxis >= MAX_AXIS)
		return FALSE;

	// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif
	
#if MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO_PTH
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif
	
	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();
	
	double fLength; // 이동 거리
	double fDelay = 0.0;
	double fTgtPos;
	double fCurPos;
	
	//fpos는 홀드 축의 이벤트 포지션에서 이동할 이동 위치이다.
	
	if(m_pParamMotor[nHoldAxis].bType==SERVO)
		fCurPos = GetCommandPosition(nHoldAxis);
	else
		fCurPos = GetActualPosition(nHoldAxis);
	
	if(bAbs)
	{
		fTgtPos = fPos;
		if(fTgtPos >=fCurPos)
			m_nMoveDir[nHoldAxis] = POSITIVE_DIR;
		else
			m_nMoveDir[nHoldAxis] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}
	else
	{
		fTgtPos = fCurPos + fPos;
		if(fTgtPos >= fCurPos)
			m_nMoveDir[nHoldAxis] = POSITIVE_DIR;
		else
			m_nMoveDir[nHoldAxis] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}
	
	// 이동량이 최소 이동량보다 작을 경퓖E리턴
	if(fLength < m_fPosRes[nHoldAxis])
		return TRUE;
	
	// 이동위치가 software limit을 초과하는 경퓖E리턴
	double dNegSWLimitValue;
	//160305 LGH BUF FIX
	if(!m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("MAGNIFIER"))) // 0 : Same String.
		dNegSWLimitValue = GetNegSWLimitValue(nHoldAxis) + GetCommandPosition(FOCUS_AXIS);
	else
		dNegSWLimitValue = GetNegSWLimitValue(nHoldAxis); //160305 lgh mod
	
	//160305 LGH BUG FIX FOCUS AXIS-> HOLD AXIS
	if((m_nMoveDir[nHoldAxis] == NEGATIVE_DIR && fTgtPos < dNegSWLimitValue) || (m_nMoveDir[nHoldAxis] == POSITIVE_DIR && fTgtPos > GetPosSWLimitValue(nHoldAxis)))
	{
		CString strMsg;
	
		//20080820 modify by khc
		if(m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("SCAN")) || m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("CAMERA")))
			strMsg.Format(_T("%s %s\n"),
			m_pParamAxis[nHoldAxis].sName,
			pGlobalView->GetLanguageString("MOTION", "SW_LIMIT_OVER"));
		else
			strMsg.Format(_T("%s %s\n%s"),
			m_pParamAxis[nHoldAxis].sName,
			pGlobalView->GetLanguageString("MOTION", "SW_LIMIT_OVER"),
			pGlobalView->GetLanguageString("MOTION", "CHECK_PIN_POS"));
	
		AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			
		return FALSE;
	}
	
	if(fVelRatio>100.0) 
		fVelRatio = 100.0;
	if(fVelRatio<1.0) 
		fVelRatio = 1.0;
	
	double fVel=(m_fVel[nHoldAxis]*fVelRatio)/100.0;
	double fAcc= m_fAcc[nHoldAxis];
	double fDec= m_fDec[nHoldAxis];
	
	if(bOptimize)
		CalSpeedProfile(nHoldAxis, fLength,fVel,fAcc,fDec);
#ifdef USE_XMP
	MPITrajectory	Traj;
	Traj.velocity = LengthToPulse(nHoldAxis, fVel); // [mm/sec] -> [counts/sec]
	Traj.acceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2]
	Traj.deceleration = LengthToPulse(nHoldAxis, fDec); // [m/sec^2] -> [counts/sec^2]
	// 20080429 khc
#if MACHINE_MODEL == INSPRO2HR2R || MACHINE_MODEL == INSPRO2HR2R_GANTRY
	Traj.jerkPercent = 100.0;
#else
	Traj.jerkPercent = 30.0;
	// 	m_Traj.jerkPercent = 50.0;
#endif
	Traj.accelerationJerk = 0.0;
	Traj.decelerationJerk = 0.0;
#endif
	double fTempTgtPos = LengthToPulse(nHoldAxis, fTgtPos);
	INT_PTR nTgtPos = (INT_PTR)(fTempTgtPos);
	if(fTempTgtPos - (double)(nTgtPos) <= -0.5)
		nTgtPos--;
	else if(fTempTgtPos - (double)(nTgtPos) >= 0.5)
		nTgtPos++;
	fTempTgtPos = (double)nTgtPos;
	
#ifdef USE_XMP
	//=======================================================
	MEIMotionAttributes attributes;     /* motion attributes */
	MEIMotionAttrHold   hold;           /* hold attribute configuration */
	MPIMotionParams param;
	// 	int nEventPos = (int)LengthToPulse(dEventPos);
	int nEventPos = int(dEventPos);
	
	//     hold.type = MEIMotionAttrHoldTypeAXIS_POSITION_COMMAND;  /* software motion Gate control */
	hold.type = MEIMotionAttrHoldTypeAXIS_POSITION_ACTUAL;  /* software motion Gate control */
	hold.source.axis.number   = nEventAxis;
	hold.source.axis.position = nEventPos;
	hold.timeout = 0;
	
	attributes.hold = &hold;
	
	param.external = &attributes;
	
	param.sCurve.trajectory = &Traj;
	param.sCurve.position	= &fTempTgtPos;
	
	param.trapezoidal.trajectory = &Traj;
	param.trapezoidal.position = &fTempTgtPos;
	
	param.velocity.trajectory = &Traj;
	param.attributes.delay = &fDelay;
	
	//motion type을 지정한다.
	MPIMotionType type;
	if(nDir != BACKWARD)
	{
		if(m_pParamMotor[nHoldAxis].bType==SERVO && bMotionType==MPIMotionTypeS_CURVE)
			type = (MPIMotionType)(MPIMotionTypeS_CURVE | MEIMotionAttrMaskHOLD_GREATER);
		else
			type = (MPIMotionType)(MPIMotionTypeTRAPEZOIDAL | MEIMotionAttrMaskHOLD_GREATER);
	}
	else
	{
		if(m_pParamMotor[nHoldAxis].bType==SERVO && bMotionType==MPIMotionTypeS_CURVE)
			type = (MPIMotionType)(MPIMotionTypeS_CURVE | MEIMotionAttrMaskHOLD_LESS);
		else
			type = (MPIMotionType)(MPIMotionTypeTRAPEZOIDAL | MEIMotionAttrMaskHOLD_LESS);
	}
	
	int nAxisState = GetAxisState(nHoldAxis);
	
	while(nAxisState != MPIStateIDLE && nAxisState != MPIStateSTOPPED)
	{
		nAxisState = GetAxisState(nHoldAxis);
		Sleep(30);
	}
	
	if(m_pParamMotor[nHoldAxis].bType==STEPPER)
	{
		double fCmdPos = GetCommandPosition(nHoldAxis);
		double fActPos = GetActualPosition(nHoldAxis);
		if(fCmdPos!=fActPos)
			SetCommandPosition(nHoldAxis, fActPos);
	}
	
	if(CheckError(nHoldAxis, m_pMotionCard->MotionStart(nHoldAxis, type, param)))
	{
		// 2009-04-02 jsy
		//AfxMessageBox(_T("mpiMotionStart Error.-4",MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		pGlobalView->m_strAfxMsg = pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartPosMove_P6_M5");
		AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}
#else
	//long val;
	////double dEvent = LengthToPulse(nEventAxis, dEventPos);
	//double dEvent = dEventPos; // 이미 펄스변환이 되어 있다. 160305 LGH BUG FIX
	//double dEventCmdPos = LengthToPulse(nEventAxis,GetCommandPosition(nEventAxis));
	//	double dActPosofEvt= GetActualPosition(nEventAxis);
	//	double dActPosofEvtPls= LengthToPulse(nEventAxis,GetActualPosition(nEventAxis));
	//MPIMotionPointToPointAttributes attribute;
	//int32_t							gateNumber  = 0;
	//double							gateTimeout = 0.0;	/* Wait forever */
	//MPIMotionPointToPointAttrMask	attr;
	//
	//attribute.hold.data.axisActualPosition.axisNumber = nEventAxis;
	//attribute.hold.data.axisActualPosition.position = dEvent;
	////attribute.hold.data.axisCommandPosition.axisNumber = nEventAxis;
	////attribute.hold.data.axisCommandPosition.position = dEventCmdPos;
	////160305 lgh mod
	//if(nDir==FORWARD)
	//	attribute.hold.data.axisActualPosition.logic = MPIUserLimitLogicGE;
	//else
	//	attribute.hold.data.axisActualPosition.logic = MPIUserLimitLogicLE;
	//
	//attribute.hold.timeout = gateTimeout;
	//attribute.hold.type = MPIMotionHoldTypeAXIS_ACTUAL_POSITION;
	//
	///* Load motion profile with HOLD attribute */
	//double position = fTempTgtPos;
	//double velocity = LengthToPulse(nHoldAxis, fVel); // [mm/sec] -> [counts/sec]
	//double acceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2];
	//double deceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2];
	//
	//val = mpiMotionTrapezoidalMove(m_pMotionCard->m_pMotionNew[nHoldAxis].m_hMotion, position,
	//	velocity, acceleration, deceleration, 0, MPIMotionPointToPointAttrMaskHOLD, &attribute);
	//if(CheckError(nHoldAxis, val))
	//	return FALSE;
	
#endif
	return TRUE;
}

BOOL CMotionNew::StartHoldingMove(int nHoldAxis, int nEventAxis, double dEventPos, double fPos, int nDir, double fVelRatio, BOOL bAbs, BOOL bWait, BOOL bOptimize, int bMotionType)
{
	if (nEventAxis >= MAX_AXIS || nHoldAxis >= MAX_AXIS)
		return FALSE;

// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif

#if MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO_PTH
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif

	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();

	double fLength; // 이동 거리
	double fDelay = 0.0;
	double fTgtPos;
	double fCurPos;

	if(m_pParamMotor[nHoldAxis].bType==SERVO)
		fCurPos = GetCommandPosition(nHoldAxis);
	else
		fCurPos = GetActualPosition(nHoldAxis);

	if(bAbs)
	{
		fTgtPos = fPos;
		if(fTgtPos >=fCurPos)
			m_nMoveDir[nHoldAxis] = POSITIVE_DIR;
		else
			m_nMoveDir[nHoldAxis] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}
	else
	{
		fTgtPos = fCurPos + fPos;
		if(fTgtPos >= fCurPos)
			m_nMoveDir[nHoldAxis] = POSITIVE_DIR;
		else
			m_nMoveDir[nHoldAxis] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}
	
	// 이동량이 최소 이동량보다 작을 경퓖E리턴
	if(fLength < m_fPosRes[nHoldAxis])
		return TRUE;

	
	// 이동위치가 software limit을 초과하는 경퓖E리턴
	double dNegSWLimitValue;
 	if(!m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("MAGNIFIER"))) // 0 : Same String.
 		dNegSWLimitValue = GetNegSWLimitValue(FOCUS_AXIS) + GetCommandPosition(FOCUS_AXIS);
	else
		dNegSWLimitValue = GetNegSWLimitValue(FOCUS_AXIS);

	if((m_nMoveDir[nHoldAxis] == NEGATIVE_DIR && fTgtPos < dNegSWLimitValue) || (m_nMoveDir[nHoldAxis] == POSITIVE_DIR && fTgtPos > GetPosSWLimitValue(FOCUS_AXIS)))
	{
		CString strMsg;

// 		if(pGlobalDoc->m_clsEES.m_pEESInfo!=NULL) //Motion: Limit 초컖E6//131029 lgh
// 		{
// 			pGlobalDoc->m_clsEES.m_pEESInfo->SetAlarm("AOI",pGlobalDoc->m_strEquipCode,6,EES_ACTIVATE_ALARM,pGlobalDoc->m_strLotNum,pGlobalDoc->m_nSerialNum);
// 		}

		//20080820 modify by khc
		if(m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("SCAN")) || m_pParamAxis[nHoldAxis].sName.CompareNoCase(_T("CAMERA")))
			strMsg.Format(_T("%s %s\n"),
							m_pParamAxis[nHoldAxis].sName,
							pGlobalView->GetLanguageString("MOTION", "SW_LIMIT_OVER"));
		else
			strMsg.Format(_T("%s %s\n%s"),
							m_pParamAxis[nHoldAxis].sName,
							pGlobalView->GetLanguageString("MOTION", "SW_LIMIT_OVER"),
							pGlobalView->GetLanguageString("MOTION", "CHECK_PIN_POS"));

		AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);

// 		if(pGlobalDoc->m_clsEES.m_pEESInfo!=NULL) //Motion: Limit 초컖E6//131029 lgh
// 		{
// 			pGlobalDoc->m_clsEES.m_pEESInfo->SetAlarm("AOI",pGlobalDoc->m_strEquipCode,6,EES_DEACTIVATE_ALARM,pGlobalDoc->m_strLotNum,pGlobalDoc->m_nSerialNum);
// 		}		
		return FALSE;
	}

	if(fVelRatio>100.0) 
		fVelRatio = 100.0;
	if(fVelRatio<1.0) 
		fVelRatio = 1.0;
		
	double fVel=(m_fVel[nHoldAxis]*fVelRatio)/100.0;
	double fAcc= m_fAcc[nHoldAxis];
	double fDec= m_fDec[nHoldAxis];

	if(bOptimize)
		CalSpeedProfile(nHoldAxis,fLength,fVel,fAcc,fDec);
#ifdef USE_XMP
	MPITrajectory	Traj;
	Traj.velocity = LengthToPulse(nHoldAxis, fVel); // [mm/sec] -> [counts/sec]
	Traj.acceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2]
	Traj.deceleration = LengthToPulse(nHoldAxis, fDec); // [m/sec^2] -> [counts/sec^2]
	// 20080429 khc
#if MACHINE_MODEL == INSPRO2HR2R || MACHINE_MODEL == INSPRO2HR2R_GANTRY
	Traj.jerkPercent = 100.0;
#else
	Traj.jerkPercent = 30.0;
// 	m_Traj.jerkPercent = 50.0;
#endif
	Traj.accelerationJerk = 0.0;
	Traj.decelerationJerk = 0.0;
#endif
	double fTempTgtPos = LengthToPulse(nHoldAxis, fTgtPos);
	int nTgtPos = (int)(fTempTgtPos);
	if(fTempTgtPos - (double)(nTgtPos) <= -0.5)
		nTgtPos--;
	else if(fTempTgtPos - (double)(nTgtPos) >= 0.5)
		nTgtPos++;
	fTempTgtPos = (double)nTgtPos;

#ifdef USE_XMP
	//=======================================================
	MEIMotionAttributes attributes;     /* motion attributes */
    MEIMotionAttrHold   hold;           /* hold attribute configuration */
	MPIMotionParams param;
// 	int nEventPos = (int)LengthToPulse(dEventPos);
	int nEventPos = int(dEventPos);

//     hold.type = MEIMotionAttrHoldTypeAXIS_POSITION_COMMAND;  /* software motion Gate control */
    hold.type = MEIMotionAttrHoldTypeAXIS_POSITION_ACTUAL;  /* software motion Gate control */
	hold.source.axis.number   = nEventAxis;
	hold.source.axis.position = nEventPos;
    hold.timeout = 0;

    attributes.hold = &hold;

	param.external = &attributes;

	param.sCurve.trajectory = &Traj;
	param.sCurve.position	= &fTempTgtPos;

	param.trapezoidal.trajectory = &Traj;
	param.trapezoidal.position = &fTempTgtPos;

	param.velocity.trajectory = &Traj;
	param.attributes.delay = &fDelay;

	//motion type을 지정한다.
	MPIMotionType type;
	if(nDir != BACKWARD)
	{
		if(m_pParamMotor[nHoldAxis].bType==SERVO && bMotionType==MPIMotionTypeS_CURVE)
			type = (MPIMotionType)(MPIMotionTypeS_CURVE | MEIMotionAttrMaskHOLD_GREATER);
		else
			type = (MPIMotionType)(MPIMotionTypeTRAPEZOIDAL | MEIMotionAttrMaskHOLD_GREATER);
	}
	else
	{
		if(m_pParamMotor[nHoldAxis].bType==SERVO && bMotionType==MPIMotionTypeS_CURVE)
			type = (MPIMotionType)(MPIMotionTypeS_CURVE | MEIMotionAttrMaskHOLD_LESS);
		else
			type = (MPIMotionType)(MPIMotionTypeTRAPEZOIDAL | MEIMotionAttrMaskHOLD_LESS);
	}

	int nAxisState = GetAxisState(nHoldAxis);

	while(nAxisState != MPIStateIDLE && nAxisState != MPIStateSTOPPED)
	{
		nAxisState = GetAxisState(nHoldAxis);
		Sleep(30);
	}

	if(m_pParamMotor[nHoldAxis].bType==STEPPER)
	{
		double fCmdPos = GetCommandPosition(nHoldAxis);
		double fActPos = GetActualPosition(nHoldAxis);
		if(fCmdPos!=fActPos)
			SetCommandPosition(nHoldAxis, fActPos);
	}

	if(CheckError(nHoldAxis, m_pMotionCard->MotionStart(nHoldAxis, type, param)))
	{
		// 2009-04-02 jsy
		//AfxMessageBox(_T("mpiMotionStart Error.-4",MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST));
		pGlobalView->m_strAfxMsg = pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartPosMove_P6_M5");
		AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}
#else
	//long val;
	//double dEvent = LengthToPulse(nEventAxis, dEventPos);
	//double dActPosofEvt= GetActualPosition(nEventAxis);
	//MPIMotionPointToPointAttributes attribute;
	//int32_t							gateNumber  = 0;
	//double							gateTimeout = 0.0;	/* Wait forever */
	//MPIMotionPointToPointAttrMask	attr;

	//attribute.hold.data.axisActualPosition.axisNumber = nEventAxis;
	//attribute.hold.data.axisActualPosition.position = dEvent;

	////160305 lgh mod
	//if(nDir==FORWARD)
	//	attribute.hold.data.axisActualPosition.logic = MPIUserLimitLogicGE;
	//else
	//	attribute.hold.data.axisActualPosition.logic = MPIUserLimitLogicLE;

	//attribute.hold.timeout = gateTimeout;
	//attribute.hold.type = MPIMotionHoldTypeAXIS_ACTUAL_POSITION;


	///* Load motion profile with HOLD attribute */
	//double position = fTempTgtPos;
	//double velocity = LengthToPulse(nHoldAxis, fVel); // [mm/sec] -> [counts/sec]
	//double acceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2];
	//double deceleration = LengthToPulse(nHoldAxis, fAcc); // [m/sec^2] -> [counts/sec^2];

	//val = mpiMotionTrapezoidalMove(m_pMotionCard->m_pMotionNew[nHoldAxis].m_hMotion, position,
	//								velocity, acceleration, deceleration, 0, MPIMotionPointToPointAttrMaskHOLD, &attribute);
	//if(CheckError(nHoldAxis, val))
	//	return FALSE;

#endif
	return TRUE;
}

BOOL CMotionNew::ExchangeVelMove(int nMotor, double fPos, double fVel, double fAcc, double fDec, double fEventPos, double fExchangeVel, BOOL bAbs, BOOL bWait, int bMotionType)
{
	if (nMotor >= MAX_AXIS)
		return FALSE;

// 20100507-syd
#ifdef PANEL_INTERLOCK_SENSER	// 20110224 syd
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif

#if MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO_PTH
	if(pGlobalDoc->m_bSaftyAreaSensorStatus)
		return FALSE;
#endif

	pGlobalDoc->m_bRestedMotion = FALSE;
	pGlobalDoc->m_dCurrSystemTime = pGlobalDoc->m_dRestedSystemTime = GetTickCount(); //pGlobalView->GetSystemTimeVal();
	
	if(GetAxisState(nMotor) == MPIStateERROR)
	{
		if(IsEncoderFault(nMotor) && m_pParamMotor[nMotor].bType==SERVO)
		{
			pGlobalView->m_strAfxMsg = _T("[MSG894] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartVelMove_P2_M1");
			AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONSTOP|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			return FALSE;
		}

		if(IsLimitError(nMotor) && m_pParamMotor[nMotor].bType==SERVO) // 06,04,30 khc
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_MOTION_LIMIT,EES_ACTIVATE_ALARM);
			pGlobalView->m_strAfxMsg = _T("[MSG895] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartVelMove_P2_M2");
			AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONSTOP|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			pMainFrame->SetEESAlarm(ALARM_MOTION_MOTION_LIMIT,EES_DEACTIVATE_ALARM);
			return FALSE;
		}

		if(IsNegSWLimit(nMotor) || IsPosSWLimit(nMotor))
		{
			pMainFrame->SetEESAlarm(ALARM_MOTION_MOTION_LIMIT,EES_ACTIVATE_ALARM);
			MotionAbort(nMotor); // Command position컖EActual Position을 일치시킨다.
			pMainFrame->SetEESAlarm(ALARM_MOTION_MOTION_LIMIT,EES_DEACTIVATE_ALARM);
		}
		ClearMotionFault(nMotor);
		ServoOnOff(nMotor, ON);
	}
	
	double fDelay = 0.0, fTgtPos,fLength;
	double fCurPos;
	if(m_pParamMotor[nMotor].bType != SERVO )
		fCurPos = GetCommandPosition(nMotor);
	else
		fCurPos = GetActualPosition(nMotor);
	if(bAbs)
	{
		fTgtPos = fPos;
		if(fTgtPos >=fCurPos)
			m_nMoveDir[nMotor] = POSITIVE_DIR;
		else
			m_nMoveDir[nMotor] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}
	else
	{
		fTgtPos = fCurPos + fPos;
		if(fTgtPos >= fCurPos)
			m_nMoveDir[nMotor] = POSITIVE_DIR;
		else
			m_nMoveDir[nMotor] = NEGATIVE_DIR;
		fLength = fabs(fTgtPos-fCurPos);
	}

	// 이동량이 최소 이동량보다 작을 경퓖E리턴
	if(fLength < m_fPosRes[nMotor])
		return TRUE;
	
#if MACHINE_MODEL == INSPRO2HR2R  || MACHINE_MODEL == INSPRO2HR2R_GANTRY// 20080820 modify by khc
	// 이동위치가 software limit을 초과하는 경퓖E리턴
	if(m_pParamAxis[nMotor].sName != "FEEDING")
	{
		double dNegSWLimitValue;
 		if(!m_pParamAxis[nMotor].sName.CompareNoCase("MAGNIFIER")) // 0 : Same String.
 			dNegSWLimitValue = GetNegSWLimitValue(nMotor) + GetCommandPosition(FOCUS_AXIS);
		else
			dNegSWLimitValue = GetNegSWLimitValue(nMotor);
		if((m_nMoveDir == NEGATIVE_DIR && fTgtPos <= dNegSWLimitValue) || (m_nMoveDir == POSITIVE_DIR && fTgtPos >= GetPosSWLimitValue(nMotor)))
		{
			CString strMsg;

			strMsg.Format(_T("%s %s\n%s"),
							m_pParamAxis[nMotor].sName,
							pGlobalView->GetLanguageString("MOTION", "SW_LIMIT_OVER"),
							pGlobalView->GetLanguageString("MOTION", "CHECK_PIN_POS"));
			AfxMessageBox(strMsg,MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
			return FALSE;
		}
	}
#endif	
	double fModifyVel = LengthToPulse(nMotor, fVel); // [mm/sec] -> [counts/sec] fVel -> fExchangeVel

//	MPITrajectory	Traj;
//	Traj.velocity = LengthToPulse(nMotor, fVel); // [mm/sec] -> [counts/sec]
//	Traj.acceleration = LengthToPulse(nMotor, fAcc); // [m/sec^2] -> [counts/sec^2]
//	Traj.deceleration = LengthToPulse(nMotor, fDec); // [m/sec^2] -> [counts/sec^2]
//	// 20080429 khc
//#if MACHINE_MODEL == INSPRO2HR2R || MACHINE_MODEL == INSPRO2HR2R_GANTRY
//	Traj.jerkPercent = 100.0;
//#else
//	Traj.jerkPercent = 50.0;
//#endif
//	Traj.accelerationJerk = .0;
//	Traj.decelerationJerk = .0;

	double fTmpTgtPos = LengthToPulse(nMotor, fTgtPos);
	double fModifyPos = LengthToPulse(nMotor, fEventPos);
	// 반올림 공식 적퓖E=====================================
	int nTgtPos = (int)(fTmpTgtPos);
	if(fTmpTgtPos - (double)(nTgtPos) <= -0.5) // 반올림.
		nTgtPos--;
	else if(fTmpTgtPos - (double)(nTgtPos) >= 0.5) // 반올림.
		nTgtPos++;
	fTmpTgtPos = (double)nTgtPos;
	//========================================================


//	MPI_RESULT returnValue;
//
//	//motion type을 지정한다.
//	MPIMotionType type;
//
//	if(m_pParamMotor[nMotor].bType==SERVO && bMotionType==MPIMotionTypeS_CURVE_JERK)
//		type = (MPIMotionType)(MPIMotionTypeS_CURVE_JERK);
//	else
//		type = (MPIMotionType)(MPIMotionTypeTRAPEZOIDAL);
//
//
//	MPIMoveParams ParamVMove;
//
//	ParamVMove.trajectory.velocity = LengthToPulse(nMotor, fVel); // [mm/sec] -> [counts/sec]
//	ParamVMove.trajectory.acceleration = LengthToPulse(nMotor, fAcc); // [m/sec^2] -> [counts/sec^2]
//	ParamVMove.trajectory.deceleration = LengthToPulse(nMotor, fDec); // [m/sec^2] -> [counts/sec^2]
//#if MACHINE_MODEL == INSPRO2HR2R || MACHINE_MODEL == INSPRO2HR2R_GANTRY
//	ParamVMove.trajectory.jerkPercent = 100.0;
//#else
//	ParamVMove.trajectory.jerkPercent = 50.0;
//#endif	
//	ParamVMove.trajectory.accelerationJerk = .0;
//	ParamVMove.trajectory.decelerationJerk = .0;
//	ParamVMove.delay = fDelay;
//
//	MPIMotionCoordinationType coord_type;
//	MPIMotionType motionType;
//	double dVel;
//	MPIMotionPointToPointAttrMask mask;
//	MPIMotionPointToPointAttributes attributes;
//
//	int nAxisState = GetAxisState(nMotor);
//	if( nAxisState == MPIStateMOVING)
//	{
//		type = (MPIMotionType)(type) ;
//
// 		double fCurVel = fabs(GetVelocity(nMotor));
//		double fDecLength =((fCurVel*fCurVel)/fDec)/2.0;
//		if(m_nMoveDir[nMotor] == POSITIVE_DIR && (fTgtPos + fDecLength) > GetPosSWLimitValue(nMotor))
//		{
//			fDecLength = 0.0;
//			return FALSE;
//		}
//
//		double dNegSWLimitValue;
// 		if(!m_pParamAxis[nMotor].sName.CompareNoCase(_T("MAGNIFIER"))) // 0 : Same String.
// 			dNegSWLimitValue = GetNegSWLimitValue(nMotor) + GetCommandPosition(FOCUS_AXIS);
//		else
//			dNegSWLimitValue = GetNegSWLimitValue(nMotor);
//
//		if(m_nMoveDir[nMotor] == NEGATIVE_DIR && (fTgtPos - fDecLength) < dNegSWLimitValue)
//		{
//			fDecLength = 0.0;
//			return FALSE;
//		}		
//		
//
//		coord_type = MPIMotionCoordinationTypeSTART_AND_FINISH;
//		motionType = type; 
//		dVel = 0.0;
//
//		mask = (MPIMotionPointToPointAttrMask)(/*MPIMotionPointToPointAttrMaskDELAY | */MPIMotionPointToPointAttrMaskBEHAVIOR);
//		attributes.delay = ParamVMove.delay;
//		attributes.behavior = MPIMotionPointToPointBehaviorTypeMODIFY;
//
//		returnValue =
//			mpiMotionSCurveJerkCoordinatedMove(m_pMotionCard->m_pMotionNew[nMotor].m_hMotion, 
//			&fTmpTgtPos, 
//			&ParamVMove.trajectory.velocity, 
//			&ParamVMove.trajectory.acceleration, 
//			&ParamVMove.trajectory.deceleration, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&dVel, 
//			coord_type, mask, 
//			&attributes);
//
//		if(CheckError(nMotor, (long)returnValue))
//			return FALSE;
//
//
//
//
//		ParamVMove.trajectory.velocity = LengthToPulse(nMotor, fExchangeVel); // [mm/sec] -> [counts/sec]
//		ParamVMove.position = fTmpTgtPos;
//
//		attributes.behavior = (MPIMotionPointToPointBehaviorType)(MPIMotionPointToPointBehaviorTypeMODIFY | MPIMotionPointToPointBehaviorTypeAPPEND_WITHOUT_MOTION_DONE);
//
//		returnValue =
//			mpiMotionSCurveJerkCoordinatedMove(m_pMotionCard->m_pMotionNew[nMotor].m_hMotion, 
//			&fTmpTgtPos, 
//			&ParamVMove.trajectory.velocity, 
//			&ParamVMove.trajectory.acceleration, 
//			&ParamVMove.trajectory.deceleration, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&dVel, 
//			coord_type, mask, 
//			&attributes);
//
//		if(CheckError(nMotor, (long)returnValue))
//			return FALSE;
//
//	}
//	else if(nAxisState == MPIStateIDLE || nAxisState == MPIStateSTOPPED)
//	{
//		if(m_pParamMotor[nMotor].bType==STEPPER)
//		{
//			double fCmdPos = GetCommandPosition(nMotor);
//			double fActPos = GetActualPosition(nMotor);
//			if(fCmdPos!=fActPos)
//				SetCommandPosition(nMotor, fActPos);
//		}
//		
//		coord_type = MPIMotionCoordinationTypeSTART_AND_FINISH;
//		motionType = type; 
//		dVel = 0.0;
//
//		mask = (MPIMotionPointToPointAttrMask)(/*MPIMotionPointToPointAttrMaskDELAY | */MPIMotionPointToPointAttrMaskBEHAVIOR);
//		attributes.delay = ParamVMove.delay;
//		attributes.behavior = MPIMotionPointToPointBehaviorTypeAPPEND_WITHOUT_MOTION_DONE;
//
//		returnValue =
//			mpiMotionSCurveJerkCoordinatedMove(m_pMotionCard->m_pMotionNew[nMotor].m_hMotion, 
//			&fTmpTgtPos, 
//			&ParamVMove.trajectory.velocity, 
//			&ParamVMove.trajectory.acceleration, 
//			&ParamVMove.trajectory.deceleration, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&dVel, 
//			coord_type, mask, 
//			&attributes);
//
//		if(CheckError(nMotor, (long)returnValue))
//			return FALSE;
//
//
//		ParamVMove.trajectory.velocity = LengthToPulse(nMotor, fExchangeVel); // [mm/sec] -> [counts/sec]
//		ParamVMove.position = fTmpTgtPos;
//
//		attributes.behavior = (MPIMotionPointToPointBehaviorType)(MPIMotionPointToPointBehaviorTypeMODIFY | MPIMotionPointToPointBehaviorTypeAPPEND_WITHOUT_MOTION_DONE);
//
//		returnValue =
//			mpiMotionSCurveJerkCoordinatedMove(m_pMotionCard->m_pMotionNew[nMotor].m_hMotion, 
//			&fTmpTgtPos, 
//			&ParamVMove.trajectory.velocity, 
//			&ParamVMove.trajectory.acceleration, 
//			&ParamVMove.trajectory.deceleration, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&ParamVMove.trajectory.jerkPercent, 
//			&dVel, 
//			coord_type, mask, 
//			&attributes);
//
//		if(CheckError(nMotor, (long)returnValue))
//			return FALSE;
//
//	}
//	else if(nAxisState == MPIStateSTOPPING)
//	{
//		pGlobalView->m_strAfxMsg = _T("[MSG902] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartPosMove_P7_M6");
//		AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
//	}
//	else if(nAxisState == MPIStateSTOPPING_ERROR)
//	{
//		pGlobalView->m_strAfxMsg = _T("[MSG902] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartPosMove_P7_M7");
//		AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
//	}
//	else if(nAxisState == MPIStateERROR)
//	{
//		Clear(nMotor);
//	}
//
//
//	if(bWait)
//	{
//		while (!IsMotionDone(nMotor) || !IsInPosition(nMotor))
//		{
//			if (pIO->CheckEmgSwitch())   // IO 0 bit Emergency
//			{
//				MotionAbort(nMotor);
//				return FALSE;
//			}
//
//			if (GetAxisState(nMotor) == MPIStateERROR )   // IO 0 bit Emergency
//			{
//				pGlobalView->m_strAfxMsg = _T("[MSG901] ") + pGlobalView->GetMultiLangString(pGlobalDoc->m_nSelectedLanguage, "AFX_MSG", "StartPosMove_P7_M8");
//				AfxMessageBox(pGlobalView->m_strAfxMsg, MB_ICONWARNING|MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
//				return FALSE;
//			}
//
//			if(GetAxisState(nMotor) == MPIStateSTOPPED)
//				break;
//		}
//	}

	return TRUE;
}

int CMotionNew::WaitScanAxisEnd(DWORD dwTimeOver, BOOL bPump) // 20150122 - syd
{
	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;


#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	g_bLoopWh = TRUE; // syd-20100629
	while (g_bLoopWh) // while(1) // syd-20100629
	{

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
		if (m_bMotionCreated == 0)
		{
			return MEI_MOTION_ERROR;
		}
#endif



		if (GetAxisState(SCAN_AXIS) == MPIStateERROR)
		{
			if (!pIO->CheckEmgSwitch() && !pIO->CheckOperateInterlock())
			{
				if (pGlobalView->m_bCheckSafetyOnlyBit)
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
					}
					nRet = OPERATE_INTERLOCK;
					break;
				}
				else
				{
					if (pGlobalDoc->m_bUseInlineAutomation)
					{
						CController::m_pController->AlarmCall(ALARM_MOTION_ERROR, 1);
					}
				}
				nRet = MEI_MOTION_ERROR;
				break;
			}
			else
			{
				if (pIO->CheckEmgSwitch())
				{
					nRet = EMERGENCY_STOP;
					break;
				}
			}
		}

#if MACHINE_MODEL == INSPRO_GM || MACHINE_MODEL == INSPRO50S || MACHINE_MODEL == INSPRO_PTH || MACHINE_MODEL == INSPRO_R2R_MDS || MACHINE_MODEL == INSMART_R2R_SUMITOMO  || MACHINE_MODEL == INSPRO_R2R_EG
		if (CheckMotionDone(SCAN_AXIS) && CheckMotionDone(SCAN_S_AXIS))
#else
		if (CheckMotionDone(SCAN_AXIS))
#endif
		{
			if (pGlobalDoc->m_bOperateInterlock)
				nRet = (OPERATE_INTERLOCK);
			else
			{
				nRet = SUCCESS;
			}
			break;
		}

		if (pGlobalDoc->m_bSaftyAreaSensorStatus || pGlobalView->m_bSwSafetyArea || pGlobalView->m_bCheckSafetyOnlyBit)
		{
			if (pGlobalDoc->m_bUseInlineAutomation)
			{
				CController::m_pController->AlarmCall(ALARM_DETECT_SAFETY, 1);
			}

			nRet = OPERATE_INTERLOCK;
			break;
		}
		else if (pGlobalDoc->m_bUserStop || pGlobalDoc->m_bExit)
		{
			nRet = USER_STOP;
			break;
		}
		else if (pIO->CheckEmgSwitch())
		{
			nRet = EMERGENCY_STOP;
			break;
		}
		else if (pIO->CheckOperateInterlock())
		{
			pGlobalDoc->m_bOperateInterlock = TRUE;
			nRet = (OPERATE_INTERLOCK);
			break;
		}
		else
		{
			if ((GetTickCount() - dwStartTick) >= dwTimeOver)
			{
				if (pGlobalDoc->m_bUseInlineAutomation)
				{
					CController::m_pController->AlarmCall(ALARM_MOTION_DONE_TIME, 1);
				}
				nRet = WAIT_TIME_OVER;
				break;
			}
		}
		if (bPump)
			pMainFrame->RefreshWindows(1);
	}

	// syd - 20101202 del, 20101206 ljg & syd del conform
	//	 pIO->ExtTriggerPulseOnOff(OFF); 

#endif	

	if (pGlobalDoc->m_bUseInspectCamAutoFocusingWithLaserSensor)
	{
		CString strMsg;

		int nScanDir;
		if (pGlobalDoc->m_nCurrSwathNum % 2)
			nScanDir = BACKWARD;
		else
			nScanDir = FORWARD;

		//		if(pGlobalDoc->m_bScanMode == BIDIRECTION && nScanDir == FORWARD && pGlobalDoc->m_bUseReverseDirAutoFocusing && pGlobalDoc->m_bLaserSensorOption)
		if (pGlobalDoc->m_bLaserSensorOption) 	//20121125-ndy for 2Head Laser Control
		{
			ResetAutoFocusingSeq();
			StopLMI();
		}
		strMsg.Format(_T("af.n:%d"), pGlobalDoc->m_nInspAutoFocusCnt);
		pMainFrame->DispStatusBar(strMsg, 1);

		pGlobalDoc->m_nInspAutoFocusCnt = 0;
	}

	return nRet;
}


BOOL CMotionNew::InitAtf()
{

// 20200729_KSJ_2In1 광학계 조건 변수 추가
#if defined USE_ATF_MODULE || VERIFY_OPTICAL == USE_2_IN_1_MODULE
	int err = 0;
	char chch[10];
	unsigned short StepNum;
	int nHomingParam[8];
	
	sprintf(chch, "\\\\.\\COM6");
	err = atf_OpenConnection(chch, 9600);

	if (err == ErrOK)
	{
		pGlobalView->m_bATFConnect = TRUE;
		pGlobalView->AtfAFOnOff(FALSE);
	}
	else
	{
		pGlobalView->m_bATFConnect = FALSE;
	}

	if (pGlobalView->m_bATFConnect)
	{
		err = atf_ReadMicrostep(&StepNum);
		if (err != ErrOK)
			AfxMessageBox(_T("[MSG325] Can't get MicroStepping Number!"));
		else
		{
			if (m_nAtfMicroStepNum != StepNum)
			{
				atf_WriteMicrostep(m_nAtfMicroStepNum);
				atf_ReadMicrostep(&StepNum);
			}
		}

		err = atf_ReadStepPerMmConversion(&StepNum);
		if (err != ErrOK)
			AfxMessageBox(_T("[MSG328] Can't get FullStep Per Milimeter Number!"));
		else
		{
			if (m_nAtfFullStepPerMM != StepNum)
			{
				atf_WriteStepPerMmConversion(m_nAtfFullStepPerMM);
				atf_ReadStepPerMmConversion(&StepNum);
			}
		}

		err = atf_ReadHomingZ_Parameters(&(nHomingParam[0]));
		if (err != ErrOK)
			AfxMessageBox(_T("[MSG329] Can't get Homing Parameter!"));
		else
		{
			if (m_nAtfHomingParam[0] != nHomingParam[0] ||
				m_nAtfHomingParam[1] != nHomingParam[1] ||
				m_nAtfHomingParam[2] != nHomingParam[2] ||
				m_nAtfHomingParam[3] != nHomingParam[3] ||
				m_nAtfHomingParam[4] != nHomingParam[4] ||
				m_nAtfHomingParam[5] != nHomingParam[5] ||
				m_nAtfHomingParam[6] != nHomingParam[6] ||
				m_nAtfHomingParam[7] != nHomingParam[7])
				atf_WriteHomingZ_Parameters(&(m_nAtfHomingParam[0]));
		}
	}
#endif

	return TRUE;
}


BOOL CMotionNew::MoveRevolverMotor(double fTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef REVOLVER_AXIS
#ifdef MOTION_ACTION	
	if (!m_bOrigin[REVOLVER_AXIS])
	{
		CString strMsg;
		strMsg.Format(_T("Revolver %s"), pGlobalView->GetLanguageString("MOTION", "ORIGIN_RETURN_FAIL"));
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK | MB_SYSTEMMODAL | MB_TOPMOST);
		return FALSE;
	}
	if (!bAbs)
		fTgtPos += GetActualPosition(REVOLVER_AXIS);

	fTgtPos = min(max(fTgtPos, m_pParamMotor[REVOLVER_AXIS].fNegLimit), m_pParamMotor[REVOLVER_AXIS].fPosLimit);
	short ret;
	if (fTgtPos != m_pParamMotor[REVOLVER_AXIS].fNegLimit && fTgtPos != m_pParamMotor[REVOLVER_AXIS].fPosLimit) // 20160204-syd
		ret = StartPosMove(REVOLVER_AXIS, fTgtPos, 50.0, ABS, bWait, NO_OPTIMIZED);
	else
		AfxMessageBox(_T("Can't move position to limit. (REVOLVER_AXIS)"));
#endif
#endif

	return TRUE;
}

void CMotionNew::SetConfigure()
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return;
	}
#endif

	m_pMotionCard->SetConfigure(m_nBoardId, m_nDevIdIoIn, m_nDevIdIoOut, m_nOffsetAxisID);
	SetMotionParam();
}

#if TRIGGER_TYPE == CONVEX_TRIGGER || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver

//  Convex Driver =========================================================================

long CMotionNew::SetConvexDriveTrigOriginPosition(int nAxisID, double fPos)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	//return m_pMotionCard->SetConvexDriveTrigOriginPosition(nAxisID, fPos);
}

long CMotionNew::SetConvexDriveTrigPulseWidth(int nAxisID, double fPulseWidth)	//pulsewidth : ms		
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

	//return m_pMotionCard->SetConvexDriveTrigPulseWidth(nAxisID, fPulseWidth);
}

long CMotionNew::SetConvexDriveTrigStartPosition(int nAxisID, double fPos)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->SetConvexDriveTrigStartPosition(nAxisID, fPos);
}

long CMotionNew::SetConvexDriveTrigEndPosition(int nAxisID, double fPos)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->SetConvexDriveTrigEndPosition(nAxisID, fPos);
}

long CMotionNew::SetConvexDriveTrigSpace(int nAxisID, double fTrigSpace)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->SetConvexDriveTrigSpace(nAxisID, fTrigSpace);
}

long CMotionNew::SetConvexDriveTrigOneshot(int nAxisID)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//	return m_pMotionCard->SetConvexDriveTrigOneshot(nAxisID);
}

long CMotionNew::SetConvexDriveTrigDirection(int nAxisID, long dir)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->SetConvexDriveTrigDirection(nAxisID, dir);
}

BOOL CMotionNew::IsEnableConvexDriveTrig(int nAxisId)
{
	if (nAxisID >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->IsEnableConvexDriveTrig(nAxisId);
}

long CMotionNew::EnableConvexDriveTrig(int nAxisID)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->EnableConvexDriveTrig(nAxisID);
}

long CMotionNew::DisableConvexDriveTrig(int nAxisID)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->DisableConvexDriveTrig(nAxisID);
}

long CMotionNew::SetConvexDriveTrigMode(int nAxisID)
{
	if (nAxisID >= MAX_AXIS)
		return 0;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif
	//return m_pMotionCard->SetConvexDriveTrigMode(nAxisID);
}

#endif


long CMotionNew::CreateAutoFocusingSeq3_31_RTAF(structRTAFInfo* pPointArr, double dRefHeightAtTestPoint, int nTotal, int nDir)
{
	#ifdef MOTION_ACTION
	//return m_pMotionCard->CreateAutoFocusingSeq3_31_RTAF(pPointArr, dRefHeightAtTestPoint, nTotal, nDir);
	// m_pMotionCard->AddList(fScanMotPos, m_dOrgFocusPos+dFocusOffset, m_dScanVel);

	int i;
	double fSamplePosOfScanAxis, fTargetPosOfFocusAxis;

	for (i = 0; i < nTotal; i++)
	{
		//fScanMotPos = pPointArr[i].dPosToTriggerAtMotionPosByScanAxis ;
		//fFocusMotPos = m_dOrgFocusPos + dFocusOffset;

		fSamplePosOfScanAxis = pPointArr[i].dPosToTriggerAtMotionPosByScanAxis;
		fTargetPosOfFocusAxis = pPointArr[i].dHeight + m_dOrgFocusPos;


		m_pMotionCard->AddList(fSamplePosOfScanAxis, fTargetPosOfFocusAxis, m_dScanVel);
	}

	#endif



	return 0;
}

int CMotionNew::SetRTAFTrigger(int nMotorSwath, int nDir, int nTrgrCh, double dStPos, double dEdPos)	// 20120315 ljh
{
#if TRIGGER_TYPE == ADLINK_8124 || TRIGGER_TYPE == CONVEX_AND_ADLINK8124_TRIGGER	//20120601-ndy for ConvexDriver
	CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();

	CAdlink_8124* p8124 = CAdlink_8124::GetTrigger();

	if (p8124->m_dInPlsRes[nTrgrCh] < 0)
	{
		//AfxMessageBox(_T("Invalid InPlsRes!!!"));
		return ALARM_8124_PULSEWITDH_INVALID;
	}

	//index를 1로 세팅
	pAFModule->m_nRTAFStartIndex = 0;

	CRTAFControl* pControl = pAFModule->m_pRTAF[0];
	CArPanelRTAFInfo* pAFInfo = pControl->GetRTAFInfo();

	CString strRTAFFilePath;
	/*
	if (pControl->GetRTAFData() != TRUE)
	{
	strRTAFFilePath.Format(_T("%sRTAF_Data-%d-%d.aff"), pGlobalDoc->m_strAoiSetDir, nDir, 0);

	pControl->LoadPanelRTAFData(strRTAFFilePath);
	}

	pControl->GetRTAFData();
	*/
	pControl->CalculateAFControlParameters();

	structPanelRTAFInfo stSwathArray = pAFInfo->GetAt(nMotorSwath);

	int nPosCnt = stSwathArray.pArRTAFInfo->GetCount();

	int i = 0;

	I16 setNum = nTrgrCh;
	I16 cntNum = 0;//nTrgrCh;
	I16 ret;
	I32 Finterval;
	I32 startPoint;

	double dLaserOffset = 0;

	if (nDir != FORWARD)	//20121125-ndy for 2Head Laser Control
		dLaserOffset = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1];
	else
		dLaserOffset = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0];

	dLaserOffset = fabs(dLaserOffset);

	//Position Setting

	for (i = 0; i < nPosCnt; i++)
	{
		structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);

		pPoint.dPosToCompareAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToCompare - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fPreAlignRotateTableOffsetY;
		pPoint.dPosToMeasureAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToMeasure - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fPreAlignRotateTableOffsetY;

		if (nDir == FORWARD)
			pPoint.dPosToTriggerAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToTrigger - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y - dLaserOffset + pGlobalDoc->m_fPreAlignRotateTableOffsetY + pGlobalDoc->m_fSwath0RotateTableOffsetY;
		else
			pPoint.dPosToTriggerAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToTrigger - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + dLaserOffset + pGlobalDoc->m_fPreAlignRotateTableOffsetY + pGlobalDoc->m_fSwath0RotateTableOffsetY;

		stSwathArray.pArRTAFInfo->SetAt(i, pPoint);
	}


	if (nDir == FORWARD)
	{
		pAFModule->m_nRTAFStartIndex = 0;
		startPoint = (I32)(int(dStPos * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

		const I16 ArraySize = 1023;
		I32 DataArr[ArraySize];
		int nDataID = 0;
		for (i = 0; i < nPosCnt; i++)
		{
			structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);

			if ((pPoint.dPosToCompareAtMotionPosByScanAxis) > dStPos)
			{
				if (nDataID == 0)
				{
					startPoint = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

					ret = _8124_set_comparator_data(p8124->m_CardId, setNum, startPoint);

					if (ret != ERR_NoError)
					{
						return p8124->GetErrorCode(ret);
					}
					nDataID++;

					pAFModule->m_nRTAFStartIndex = i;

					continue;
				}

				I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

				DataArr[nDataID - 1] = nTrgPos;
				nDataID++;
			}
		}

		if (nDataID > 0)
		{
			ret = _8124_set_fifo_array(p8124->m_CardId, setNum, DataArr, nDataID - 1);
			if (ret != ERR_NoError)
			{
				return p8124->GetErrorCode(ret);
			}
		}

		return ALARM_NONE;
	}
	else
	{

		pAFModule->m_nRTAFStartIndex = 0;
		startPoint = (I32)(int(dStPos * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

		const I16 ArraySize = 1023;
		I32 DataArr[ArraySize];
		int nDataID = 0;

		double dClampMargin = 0;

		for (i = nPosCnt - 1; i >= 0; i--)
		{
			structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);

			if ((pPoint.dPosToCompareAtMotionPosByScanAxis) < dStPos - dClampMargin)
			{
				if (nDataID == 0)
				{
					startPoint = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

					ret = _8124_set_comparator_data(p8124->m_CardId, setNum, startPoint);

					if (ret != ERR_NoError)
					{
						return p8124->GetErrorCode(ret);
					}
					nDataID++;

					pAFModule->m_nRTAFStartIndex = i;

					continue;
				}

				I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]

				DataArr[nDataID - 1] = nTrgPos;
				nDataID++;
			}
		}

		if (nDataID > 0)
		{
			ret = _8124_set_fifo_array(p8124->m_CardId, setNum, DataArr, nDataID - 1);
			if (ret != ERR_NoError)
			{
				return p8124->GetErrorCode(ret);
			}
		}
	}
#endif

#if TRIGGER_TYPE == RSA_TRIGGER
	CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
	CRTAFControl* pControl = pAFModule->m_pRTAF[0];

	/*
	double fPeriod = 1/(fLineRate*1000.)*1000000.; // [us]
	long   nPulseWidth = int(fPeriod/2.0);// duty 50% [us]
	*/
	// TRIGGER_LASER_F_VX
	double fPeriod = pControl->GetMeasuringTimingPeriod() * 1000.0;						// [mSec] -> [uSec]
	long   nPulseWidth = int(fPeriod / 2.0);											// duty 50% [us]
	//double dTrigRes = pControl->GetRTAFInterval() * 1000.0;							// [mm] -> [um]
	double dTrigRes = g_SequenceAF.m_dEncoderSpacing * 1000.0;							// [mm] -> [um]
	double fOptRes = pControl->GetOpticalResolution();

	CArPanelRTAFInfo* pAFInfo = pControl->GetRTAFInfo();
	structPanelRTAFInfo stSwathArray = pAFInfo->GetAt(nMotorSwath);
	int nPosCnt = stSwathArray.pArRTAFInfo->GetCount();

	if (nDir != FORWARD)
		pAFModule->m_nRTAFStartIndex = nPosCnt - 1;
	else
		pAFModule->m_nRTAFStartIndex = 0;

//	TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)100.0, dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//	TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)nPulseWidth * dTrigRes / fOptRes, g_SequenceAF.m_dEncoderSpacing * 1000.0);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
	TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)nPulseWidth, g_SequenceAF.m_dEncoderSpacing * 1000.0);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
#endif

//#if TRIGGER_TYPE == RSA_TRIGGER
//	CInspectionAutoFocus* pAFModule = CInspectionAutoFocus::GetAFInstance();
//
//	//CAdlink_8124* p8124 = CAdlink_8124::GetTrigger();
//
//	//if (p8124->m_dInPlsRes[nTrgrCh] < 0)
//	//{
//	//	//AfxMessageBox(_T("Invalid InPlsRes!!!"));
//	//	return ALARM_8124_PULSEWITDH_INVALID;
//	//}
//
//	//index를 1로 세팅
//	pAFModule->m_nRTAFStartIndex = 0;
//
//	CRTAFControl* pControl = pAFModule->m_pRTAF[0];
//	CArPanelRTAFInfo* pAFInfo = pControl->GetRTAFInfo();
//
//	CString strRTAFFilePath;
//	/*
//	if (pControl->GetRTAFData() != TRUE)
//	{
//	strRTAFFilePath.Format(_T("%sRTAF_Data-%d-%d.aff"), pGlobalDoc->m_strAoiSetDir, nDir, 0);
//
//	pControl->LoadPanelRTAFData(strRTAFFilePath);
//	}
//
//	pControl->GetRTAFData();
//	*/
//	pControl->CalculateAFControlParameters();
//
//	structPanelRTAFInfo stSwathArray = pAFInfo->GetAt(nMotorSwath);
//
//	int nPosCnt = stSwathArray.pArRTAFInfo->GetCount();
//
//	int i = 0;
//
//	I16 setNum = nTrgrCh;
//	I16 cntNum = 0;//nTrgrCh;
//	I16 ret;
//	I32 Finterval;
//	I32 startPoint;
//
//	double dLaserOffset = 0;
//
//	if (nDir != FORWARD)	//20121125-ndy for 2Head Laser Control
//		dLaserOffset = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[1];
//	else
//		dLaserOffset = pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fLaserSensorOffsetY[0];
//
//	dLaserOffset = fabs(dLaserOffset);
//
//	//Position Setting
//
//	for (i = 0; i < nPosCnt; i++)
//	{
//		structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);
//
//		pPoint.dPosToCompareAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToCompare - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fPreAlignRotateTableOffsetY;
//		pPoint.dPosToMeasureAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToMeasure - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + pGlobalDoc->m_fPreAlignRotateTableOffsetY;
//
//		if (nDir == FORWARD)
//			pPoint.dPosToTriggerAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToTrigger - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y - dLaserOffset + pGlobalDoc->m_fPreAlignRotateTableOffsetY + pGlobalDoc->m_fSwath0RotateTableOffsetY;
//		else
//			pPoint.dPosToTriggerAtMotionPosByScanAxis = (pGlobalDoc->m_fFidPosY + pGlobalDoc->m_fPinPosY) - (pGlobalView->m_DrawOffset.y - pPoint.dPosToTrigger - pGlobalView->m_PinPos.y) + pGlobalDoc->m_TstCamOffset_Y + dLaserOffset + pGlobalDoc->m_fPreAlignRotateTableOffsetY + pGlobalDoc->m_fSwath0RotateTableOffsetY;
//
//		stSwathArray.pArRTAFInfo->SetAt(i, pPoint);
//	}
//
//	// TRIGGER_LASER_F_VX
//	double fPeriod = pControl->GetMeasuringTimingPeriod() * 1000.0;						// [mSec] -> [uSec]
//	long   nPulseWidth = int(fPeriod / 2.0);											// duty 50% [us]
//	double dTrigRes = pControl->GetRTAFInterval() * 1000.0;								// [mm] -> [um]
//
//	if (nDir == FORWARD)
//	{
//		//pAFModule->m_nRTAFStartIndex = 0;
//		////startPoint = (I32)(int(dStPos * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//
//		//const I16 ArraySize = 1023;
//		//I32 DataArr[ArraySize];
//		//int nDataID = 0;
//		//for (i = 0; i < nPosCnt; i++)
//		//{
//		//	structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);
//
//		//	if ((pPoint.dPosToCompareAtMotionPosByScanAxis) > dStPos)
//		//	{
//		//		if (nDataID == 0)
//		//		{
//		//			//startPoint = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//
//		//			//ret = _8124_set_comparator_data(p8124->m_CardId, setNum, startPoint);
//
//		//			//if (ret != ERR_NoError)
//		//			//{
//		//			//	return p8124->GetErrorCode(ret);
//		//			//}
//
//
//		//			nDataID++;
//
//		//			pAFModule->m_nRTAFStartIndex = i;
//
//		//			continue;
//		//		}
//
//		//		//I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//		//		I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / dTrigRes)); // dTrigRes [um]
//
//		//		DataArr[nDataID - 1] = nTrgPos;
//		//		nDataID++;
//
//		//		// fPeriod : uSec
//		//		//TriggerStop(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX); //2021.05.06
//		//		TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)100.0, dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//		//	}
//		//}
//
//		//if (nDataID > 0)
//		//{
//		//	ret = _8124_set_fifo_array(p8124->m_CardId, setNum, DataArr, nDataID - 1);
//		//	if (ret != ERR_NoError)
//		//	{
//		//		return p8124->GetErrorCode(ret);
//		//	}
//		//}
//
//		TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)100.0, dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//		return ALARM_NONE;
//	}
//	else
//	{
//
//		//pAFModule->m_nRTAFStartIndex = 0;
//		////startPoint = (I32)(int(dStPos * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//
//		//const I16 ArraySize = 1023;
//		//I32 DataArr[ArraySize];
//		//int nDataID = 0;
//
//		//double dClampMargin = 0;
//
//		//for (i = nPosCnt - 1; i >= 0; i--)
//		//{
//		//	structRTAFInfo pPoint = stSwathArray.pArRTAFInfo->GetAt(i);
//
//		//	if ((pPoint.dPosToCompareAtMotionPosByScanAxis) < dStPos - dClampMargin)
//		//	{
//		//		if (nDataID == 0)
//		//		{
//		//			//startPoint = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//
//		//			//ret = _8124_set_comparator_data(p8124->m_CardId, setNum, startPoint);
//
//		//			//if (ret != ERR_NoError)
//		//			//{
//		//			//	return p8124->GetErrorCode(ret);
//		//			//}
//
//		//			nDataID++;
//
//		//			pAFModule->m_nRTAFStartIndex = i;
//
//		//			continue;
//		//		}
//
//		//		//I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / p8124->m_dInPlsRes[nTrgrCh])); // dInPlsRes [um]
//		//		I32 nTrgPos = (I32)(int((pPoint.dPosToTriggerAtMotionPosByScanAxis) * 1000.0 / dTrigRes)); // dTrigRes [um]
//
//		//		DataArr[nDataID - 1] = nTrgPos;
//		//		nDataID++;
//
//		//		//TriggerStop(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX); //2021.05.06
//		//		TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)100.0, dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//
//		//	}
//		//}
//
//		//if (nDataID > 0)
//		//{
//		//	ret = _8124_set_fifo_array(p8124->m_CardId, setNum, DataArr, nDataID - 1);
//		//	if (ret != ERR_NoError)
//		//	{
//		//		return p8124->GetErrorCode(ret);
//		//	}
//		//}
//
//		TriggerSetRange(TRIGGER_ETHERCAT_ADDR, TRIGGER_LASER_F_VX, dStPos, dEdPos, (double)100.0, dTrigRes);		// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
//	}
//#endif

	return ALARM_NONE;
}

void CMotionNew::EnableRTAF(BOOL bEnable, BOOL bPanelSync, BOOL bPartialMode)
{
	m_bPanelSyncMode = bPanelSync;
	m_bEnableRTAF = bEnable;
	m_bPartialMode = bPartialMode;
	if (m_bEnableRTAF)
	{
		if (CInspectionAutoFocus::GetAFInstance()->m_bAFDataCreated[0] == 0)
		{
			m_bEnableRTAF = 0;
		}
	}

	//20200825 lgh 
	g_SequenceAF.m_nSurfaceCnt = 0;

}

void CMotionNew::SetCurrentMotorSwath(int nSwath)
{
	m_nCurMotorSwath = nSwath;
}

void CMotionNew::SetOrgMotorStartPos(double dYPos)
{
	m_dOrgStartPos = dYPos;
}

BOOL CMotionNew::CheckMotionConfig()  // 20180417-syd
{
#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	if (m_bMotionCreated == 0)
	{
		return 0;
	}
#endif

#ifdef USE_ZMP
	return m_pMotionCard->CheckMotionConfig();
#else
	return TRUE;
#endif

	return TRUE;
}

BOOL CMotionNew::ClearStatus(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	return m_pMotionCard->GetAxis(nMsId)->ClearStatus();
}




BOOL CMotionNew::Clear(int nMotionId)
{
	if (nMotionId >= MAX_AXIS)
		return FALSE;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	ClearStatus(nMotionId);
#endif
	return TRUE;
}

void CMotionNew::SetStopTime(int nMotionId, float fStopTime)
{
	if (nMotionId >= MAX_AXIS)
		return;

#ifdef MOTION_ACTION	//20120306-ndy modified for RI
	//m_pMotionCard->SetStopTime(nMotionId, fStopTime);
	m_pMotionCard->GetAxis(nMotionId)->SetStopRate((int)fStopTime);
#endif
	return;
}



double CMotionNew::CalSpeedProfile(int nAxisID, double fLength, double &fVel, double &fAcc, double &fDec)
{
	if (nAxisID >= MAX_AXIS)
		return 0.0;

	double fAccTime, fVelocityTime, fTotalTime;

	double fVelocity = fVel;
	double fAccPeriod = m_fAccPeriod[nAxisID];
	double fAccLength;
	fAcc = m_fAcc[nAxisID];

	if (fAcc <= 0.0 || fAcc > m_fMaxAccel[nAxisID])
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
		if (fAccTime < m_fMinJerkTime[nAxisID])
			fAccTime = m_fMinJerkTime[nAxisID];
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


void CMotionNew::TriggerSetRange(int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
{
	m_pMotionCard->TriggerSetRange(nEcatAddr, vAxisId, dStPos, dEdPos, dPulseWd, dTrigRes);	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
}

BOOL CMotionNew::TriggerStop(int nEcatAddr, int vAxisId)
{
	return m_pMotionCard->TriggerStop(nEcatAddr, vAxisId);
}

BOOL CMotionNew::TriggerSetOriginPos(int nEcatAddr, int vAxisId, int nSdoIdx)
{
	m_pMotionCard->TriggerSetOriginPos(nEcatAddr, vAxisId, nSdoIdx);
	return TRUE;
}

CString CMotionNew::GetTriggerEncCnt(int nEcatAddr, int nSdoIdx)
{
	return m_pMotionCard->GetTriggerEncCnt(nEcatAddr, nSdoIdx);
}

void CMotionNew::EnableSensorStop(int nAxisId, int nSensorIndex, BOOL bEnable) // nSensorIndex : 0 ~ 4 , bEnable
{
	m_pMotionCard->EnableSensorStop(nAxisId, nSensorIndex, bEnable);
}

#endif	// #ifdef USE_NMC