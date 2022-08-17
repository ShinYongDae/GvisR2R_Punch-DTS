// Motion.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "../Global/GlobalDefine.h"
#include "Motion.h"
//#include "MyData.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CMotion

CMotion::CMotion(CWnd* pParent)
{
	int i=0;

	UINT16 nBoardID = NMC_DEVICE_BOARD;
	UINT16 nDevIdIoIn = NMC_DEVICE_IOIN;
	UINT16 nDevIdIoOut = NMC_DEVICE_IOOUT;

	m_nBoardId = nBoardID;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = 1;
	m_nGroupID_Interpolation[0] = NMC_GROUPID_INTERPOLRATION_X0Y0;
	m_nGroupID_Interpolation[1] = NMC_GROUPID_INTERPOLRATION_X1Y1;

	m_pParent = pParent;
	m_pParam = NULL; 

#ifdef USE_NMC
	m_pMotionCard = NULL;
#endif

	m_pParamMotion = NULL;
	m_pParamAxis = NULL;
	m_pParamMotor = NULL;

	m_pMyFileErrMap = NULL;

	m_bUseSaftyCover = TRUE;
	m_bUseVrsSkip = TRUE;

	m_bResetElementID = FALSE;
	m_bSetPinPos = FALSE;
	m_bSetPnlThickness = FALSE;
	m_dBaseFocusZ = 0.0;
	m_dPanelThicknessByUser = 0.0;
	m_nElementID = 0;
	m_InputFlag = 0;

	m_dTotMarkDist = 0.0;
	m_dTotJumpDist = 0.0;
	m_dTotMarkTime = 0.0;
	m_dTotJumpTime = 0.0;

	m_dPinPosX[0] = 0.0;
	m_dPinPosY[0] = 0.0;
	m_dPinPosX[1] = 0.0;
	m_dPinPosY[1] = 0.0;

	m_dAlignPosX[0][0] = 0.0;
	m_dAlignPosX[0][1] = 0.0;
	m_dAlignPosX[1][0] = 0.0;
	m_dAlignPosX[1][1] = 0.0;
	m_dAlignPosY[0][0] = 0.0;
	m_dAlignPosY[0][1] = 0.0;
	m_dAlignPosY[1][0] = 0.0;
	m_dAlignPosY[1][1] = 0.0;
	
	m_dStBufPos = 0.0;
	m_dScaleFdBuf = 0.0;

	m_dMarkOffsetX = 0.0;
	m_dMarkOffsetY = 0.0;
	m_bMarkOffset = FALSE;


	for(i=0; i<32; i++)
		m_bDO[i] = FALSE;

	for(i=0; i<4; i++)
		m_dFeedRate[i] = 1.0;

	RECT rt={0,0,0,0};
	if(!Create(NULL, NULL, WS_CHILD, rt, pParent, 0))
		AfxMessageBox(_T("CMotion::Create() Failed!!!"));

	m_sPathMotionParam = PATH_MOTION_PARAM;
	LoadParam();
}

CMotion::~CMotion()
{
	MotionAbortAll();
	Sleep(300);

	FreeAll();
}


BEGIN_MESSAGE_MAP(CMotion, CWnd)
	//{{AFX_MSG_MAP(CMotion)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BUF_THREAD_DONE, OnBufThreadDone)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMotion message handlers

void CMotion::FreeAll()
{
	if(m_pMyFileErrMap)
	{
		delete m_pMyFileErrMap;
		m_pMyFileErrMap = NULL;
	}
#ifdef USE_NMC
	if(m_pMotionCard)			
	{
		delete m_pMotionCard;
		m_pMotionCard = NULL;
	}
#endif
	if(m_pParamMotor)
	{
		delete[] m_pParamMotor;
		m_pParamMotor = NULL;
	}

	if(m_pParamAxis)
	{
		delete[] m_pParamAxis;
		m_pParamAxis = NULL;
	}

	if(m_pParamMotion)
	{
		delete[] m_pParamMotion;
		m_pParamMotion = NULL;
	}
}

void CMotion::LoadConfig()
{
	//CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	//CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
	//CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();

	// Initialize File Buffer...
	//m_pMyFileErrMap = new CMyFileErrMap(this);

// 	if(m_pMyFileErrMap)
// 	{
// 		if(!LoadErrMapFile(pDoc->m_strErrorMapping2dFilepath))
// 		{
// 			delete m_pMyFileErrMap;
// 			m_pMyFileErrMap = NULL;
// 		}
// 	}
}


BOOL CMotion::LoadErrMapFile(CString sPath)
{
	if(m_pMyFileErrMap)
	{
		if(!m_pMyFileErrMap->Open(sPath))
			return FALSE;
	}
	else
		return FALSE;

	return TRUE;
}

void CMotion::LoadParam()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();
	
	CString sVal, sIdx;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;

	int nID, nCol, i, nAxisID;


	// [CONTROL PARAM]
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotMotion = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotion = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL AXIS"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotAxis = _ttoi(szData);
	else
		m_ParamCtrl.nTotAxis = 0;
	m_nTotAxis = m_ParamCtrl.nTotAxis;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTOR"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotMotor = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotor = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL FILTER"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotFilter = _ttoi(szData);
	else
		m_ParamCtrl.nTotFilter = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL CAPTURE"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotCapture = _ttoi(szData);
	else
		m_ParamCtrl.nTotCapture = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL SEQUENCE"), NULL, szData, sizeof(szData), m_sPathMotionParam))
		m_ParamCtrl.nTotSequence = _ttoi(szData);
	else
		m_ParamCtrl.nTotSequence = 0;

	// CreateObject - DataParam
	if(!m_pParamMotion)
		m_pParamMotion = new MotionMotion[m_ParamCtrl.nTotMotion];
	if(!m_pParamAxis)
		m_pParamAxis = new MotionAxis[m_ParamCtrl.nTotAxis];
	if(!m_pParamMotor)
		m_pParamMotor = new MotionMotor[m_ParamCtrl.nTotMotor];


	 
	// [OBJECT MAPPING]
	for(nID=0; nID < m_ParamCtrl.nTotAxis; nID++)
	{
		sIdx.Format(_T("AXIS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), m_sPathMotionParam))
		{
			token = _tcstok(szData,sep);
			nCol = 0;
			if(token != NULL)
				m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);

			while(token != NULL)
			{
				nCol++;
				if(nCol >= 5)
					break;
				token = _tcstok(NULL,sep);
				if(token != NULL)
					m_pParamAxis[nID].stAxisMap.nMappingMotor[nCol] = _ttoi(token);
			}

			m_pParamAxis[nID].stAxisMap.nNumMotor = nCol;
		}
		else
			m_pParamAxis[nID].stAxisMap.nNumMotor = 0;
	}
	for(nID=0; nID < m_ParamCtrl.nTotMotion; nID++)
	{
		sIdx.Format(_T("MS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), m_sPathMotionParam))
		{
			token = _tcstok(szData,sep);
			nCol = 0;
			if(token != NULL)
			{
				m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
				m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
				for(i=0; i<m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
					m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
			}

			while(token != NULL)
			{
				nCol++;
				if(nCol >= 5)
					break;
				token = _tcstok(NULL,sep);
				if(token != NULL)
				{
					m_pParamMotion[nID].stMsMap.nMappingAxis[nCol] = nAxisID = _ttoi(token);
					m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor = m_pParamAxis[nAxisID].stAxisMap.nNumMotor;
					for(i=0; i<m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nNumMotor; i++)
						m_pParamMotion[nID].stMsMap.stMappingMotor[nCol].nMappingMotor[i] = m_pParamAxis[nAxisID].stAxisMap.nMappingMotor[i];
				}
			}

			m_pParamMotion[nID].stMsMap.nNumAxis = nCol;
		}
		else
			m_pParamMotion[nID].stMsMap.nNumAxis = 0;
	}

	 
	// [AXIS PARAM]
	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AXIS NAME"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].sName = CString(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].sName = CString(token);
		}
	}
	else
		m_pParamAxis[nID].sName = _T(""); // Axis Name

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamAxis[nID].fSpd = 0.0; // [mm/s]

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fAcc = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fAcc = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else 
		m_pParamAxis[nID].fAcc = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fDec = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fDec = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamAxis[nID].fDec = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fAccPeriod = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fAccPeriod = _tcstod(token, &stopstring); // [%]
		}
	}
	else
		m_pParamAxis[nID].fAccPeriod = 0.0;

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("MAX ACCELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fMaxAccel = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fMaxAccel = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamAxis[nID].fMaxAccel = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("MIN JERK TIME"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fMinJerkTime = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fMinJerkTime = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamAxis[nID].fMinJerkTime = 0.0; // [sec]

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("INPOSITION LENGTH"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotAxis)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamAxis[nID].fInpRange = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamAxis[nID].fInpRange = 0.0;	// Fine position tolerance of each motor

	 
	// [MOTOR PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR TYPE"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bType = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bType = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bType = 1; // 0: Servo Motor, 1: Step Motor

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR RATING SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].fRatingSpeed = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fRatingSpeed = 0.0; // 0: Servo Motor, 1: Step Motor

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR DIRECTION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].nDir = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].nDir = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nDir = 1; // -1: CCW, 1: CW

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("FEEDBACK SOURCE"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bEncoder = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bEncoder = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bEncoder = 0; // 1: External Encoder 0: Internal Pulse Out

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER PULSE"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].nEncPulse = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].nEncPulse = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nEncPulse = 0; // [pulse/rev]

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("LEAD PITCH"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].fLeadPitch = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fLeadPitch = 0.0; // Ball Screw Lead Pitch [mm]

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER MULTIPLIER"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].nEncMul = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].nEncMul = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].nEncMul = 0; // Encoder Multiplier

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("GEAR RATIO"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].fGearRatio = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fGearRatio = 0.0; // Gear Ratio

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("AMP ENABLE LEVEL"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bAmpEnableLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bAmpEnableLevel = 1; // Amp Enable Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("AMP FAULT LEVEL"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bAmpFaultLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bAmpFaultLevel = 1; // Amp Fault Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("POS LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bPosLimitLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bPosLimitLevel = 1; // Positive Limit sensor active Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("NEG LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bNegLimitLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bNegLimitLevel = 1; // Negative Limit sensor active Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("HOME SENSOR LEVEL"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].bHomeLevel = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].bHomeLevel = _ttoi(token);
		}
	}
	else
		m_pParamMotor[nID].bHomeLevel = 1; // Home sensor active Level 0: Low Active 1: High Active

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].fPosLimit = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fPosLimit = 0.0; // Sotftware positive motion limit

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotor)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotor[nID].fNegLimit = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotor[nID].fNegLimit = 0.0; // Sotftware negative motion limit


	// [MOTION PARAM]
	if (0 < ::GetPrivateProfileString(_T("MOTION PARAM"), _T("ESTOP TIME"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].dEStopTime = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].dEStopTime = 0.1; // [Sec]

	// [HOME PARAM]
	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ACTION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.bAct = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.bAct = _ttoi(token);
		}
	}
	else
		m_pParamMotion[nID].Home.bAct = 0; // 0: Don't 1:Do

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("SEARCH DIRECTION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.nDir = _ttoi(token);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.nDir = _ttoi(token);
		}
	}
	else
		m_pParamMotion[nID].Home.nDir = -1; // -1: Minus 1:Plus

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ESCAPE LENGTH"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.fEscLen = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fEscLen = 0.0; // Escape length from home sensor touch position

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("1'st SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.f1stSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.f1stSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed for home search 

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("2'nd SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.f2ndSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.f2ndSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed for home search 

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.fAcc = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fAcc = 0.0; // [mm/s^2] or [deg/s^2]

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("SHIFT"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.fShift = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fShift = 0.0; // [mm]

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("OFFSET"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Home.fOffset = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Home.fOffset = 0.0; // [mm] or [deg]


	// [SPEED PARAM]
	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fSpd = 0.0; // [mm/s]

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fAcc = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fAcc = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamMotion[nID].Speed.fAcc = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fDec = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fDec = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamMotion[nID].Speed.fDec = 0.0; // [G]=9.8*10^6[m/s^2]

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fAccPeriod = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fAccPeriod = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fAccPeriod = 0.0; // [%]


	// [JOG PARAM]
	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("FAST SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fJogFastSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogFastSpd = 0.0; // [mm/s] or [deg/s^2] Fast speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("MIDDLE SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fJogMidSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogMidSpd = 0.0; // [mm/s] or [deg/s^2] Middle speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("LOW SPEED"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fJogLowSpd = _tcstod(token, &stopstring);
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogLowSpd = 0.0; // [mm/s] or [deg/s^2] Middle speed

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_sPathMotionParam))
	{
		token = _tcstok(szData,sep);
		nID = 0;
		if(token != NULL)
			m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;

		while( token != NULL )
		{
			nID++;
			if(nID >= m_ParamCtrl.nTotMotion)
				break;
			token = _tcstok(NULL,sep);
			if(token != NULL)
				m_pParamMotion[nID].Speed.fJogAcc = _tcstod(token, &stopstring)*9.8*1000.0;
		}
	}
	else
		m_pParamMotion[nID].Speed.fJogAcc = 0.0; // [G]=9.8*10^6[m/s^2]
}

BOOL CMotion::InitBoard()
{
#ifdef USE_NMC
	if (!m_pMotionCard)
	{
		//m_pMotionCard = new CXmpControl(m_pMyFileErrMap, this);
		m_pMotionCard = new CNmcDevice(this);
		if (!m_pMotionCard)
		{
			delete m_pMotionCard;
			return FALSE;
		}
	}
	return InitNmcBoard();
#endif
	return TRUE;
}


BOOL CMotion::InitNmcBoard()
{
#ifdef USE_NMC
	if (!m_pMotionCard)
	{

		m_pMotionCard = new CNmcDevice((CWnd *)this);

		if (!m_pMotionCard)
		{
			//delete m_pMotionCard;
			return FALSE;
		}

	}

	if (m_pMotionCard)
	{
		m_pMotionCard->InitDevice(1); // 1 is Number Of NMMC Board.
		Sleep(100);
	}
	else
		return FALSE;

	if (!m_pMotionCard->CheckNmcConnection())
	{
		AfxMessageBox(_T("이더캣 통신이 불안정합니다. 노드 끊김 감지, 이더캣 장치의 상태 및 케이블 결속 상태를 재확인하고 프로그램을 다시 실행하십시오. 프로그램이 종료됩니다"), MB_ICONSTOP | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_TOPMOST);
		return FALSE;
	}

	if (CreateObject())
	{
		SetConfigure();
		//WriteData(0x01 << DO_MC);
		//Sleep(100);
		m_pMotionCard->RestoreSwEmergency();	// -1: Fault , 1: Emergency Signal Off complete, 2: Previous Emergency Signal Off-state, 3: Normal
	}
#endif


	return TRUE;
}

BOOL CMotion::CreateObject()
{
#ifdef USE_NMC
	for (int nAxis = 0; nAxis < m_nTotAxis; nAxis++)
	{
		if (m_pMotionCard)
			m_pMotionCard->CreateAxis(nAxis);
	}
#endif
	return TRUE;
}

BOOL CMotion::ReadBit(BYTE cBit, BOOL bInput)
{
	//return (m_pMotionCard->ReadBit(cBit, bInput));
#ifdef USE_NMC
	if (bInput)
	{
		return m_pMotionCard->ReadIn((long)cBit);
	}
	else
	{
		return m_pMotionCard->ReadOut((long)cBit);
	}

	return FALSE;
#else
	return TRUE;
#endif
}

unsigned long CMotion::ReadAllBit(BOOL bInput)
{
	//return (m_pMotionCard->ReadAllBit(bInput));

	long nData;

#ifdef USE_NMC

	if (bInput)
	{
		m_pMotionCard->In32(&nData);
		return ((unsigned long)nData);
	}
	else
	{
		nData = m_pMotionCard->ReadOut();
		return ((unsigned long)nData);
	}

#endif
	return 0L;
}
void CMotion::WriteData(long lData)
{
#ifdef USE_NMC
	m_pMotionCard->Out32(lData);
#endif
}

void CMotion::WriteBit(BYTE cBit, BOOL bOn)
{
#ifdef USE_NMC
	m_pMotionCard->OutBit((long)cBit, bOn);
#endif
}

void CMotion::SetConfigure()
{
#ifdef USE_NMC
	if (!m_pMotionCard)
		return;

	m_pMotionCard->SetConfigure(m_nBoardId, m_nDevIdIoIn, m_nDevIdIoOut, m_nOffsetAxisID);
	SetMotionParam();
#endif
}

void CMotion::SetMotionParam()
{
#ifdef USE_NMC
	if (m_pMotionCard)
		m_pMotionCard->SetParam();
#endif
}


BOOL CMotion::AmpReset(int nMsId)
{
#ifdef USE_NMC
	if (nMsId == MS_X0Y0)
	{
		return (m_pMotionCard->GetAxis(MS_X0)->AmpFaultReset() && m_pMotionCard->GetAxis(MS_Y0)->AmpFaultReset());
	}
	else if (nMsId == MS_X1Y1)
	{
		return (m_pMotionCard->GetAxis(MS_X1)->AmpFaultReset() && m_pMotionCard->GetAxis(MS_Y1)->AmpFaultReset());
	}
	else
		return m_pMotionCard->GetAxis(nMsId)->AmpFaultReset();


	//return m_pMotionCard->m_pMotion[nMsId].Clear();
#else
	return TRUE;
#endif
}

BOOL CMotion::ServoOnOff(int nAxisId,BOOL bOnOff)
{
#ifdef USE_NMC
	if (nAxisId == MS_X0Y0)
		return (m_pMotionCard->GetAxis(MS_X0)->SetAmpEnable(bOnOff) && m_pMotionCard->GetAxis(MS_Y0)->SetAmpEnable(bOnOff));
	else if(nAxisId == MS_X1Y1)
		return (m_pMotionCard->GetAxis(MS_X1)->SetAmpEnable(bOnOff) && m_pMotionCard->GetAxis(MS_Y1)->SetAmpEnable(bOnOff));
	else
		return m_pMotionCard->GetAxis(nAxisId)->SetAmpEnable(bOnOff);
#else
	return TRUE;
#endif
}

LRESULT CMotion::OnBufThreadDone(WPARAM wPara, LPARAM lPara)
{
	m_pParent->PostMessage(WM_BUF_THREAD_DONE, 0, 0);
	return 0L;
}

// BOOL CMotion::SearchHomeBuf(BOOL bInitPos)
// {
// 	if(!m_pMotionCard->SearchHomeBuf(bInitPos))
// 		return FALSE;
// 
// 	return TRUE;
// }

BOOL CMotion::SearchHome()
{
//	for(int nMsID=0; nMsID<1; nMsID++)
 	for(int nMsID=0; nMsID<m_ParamCtrl.nTotMotion; nMsID++)
	{
		SearchHomePos(nMsID);
		Sleep(10);
	}
	return TRUE;
}

BOOL CMotion::SearchHomePos(int nMotionId, BOOL bThread)
{

	if(!m_pParamMotion[nMotionId].Home.bAct)
		return TRUE;
#ifdef USE_NMC
	if(!m_pMotionCard->SearchHomePos(nMotionId, bThread))
		return FALSE;
#endif
	return TRUE;
}

BOOL CMotion::IsHome(int nMotionId)
{
#ifdef USE_NMC
	return (m_pMotionCard->IsHome(nMotionId));
#else
	return TRUE;
#endif
}

BOOL CMotion::IsHomeDone()
{
#ifdef USE_NMC
	return (m_pMotionCard->IsHomeDone());
#else
	return TRUE;
#endif
}

BOOL CMotion::IsHomeDone(int nMotionId)
{
#ifdef USE_NMC
	return (m_pMotionCard->IsHomeDone(nMotionId));
#else
	return TRUE;
#endif
}

BOOL CMotion::SetVMove(int nMotionId, double fVel, double fAcc)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->SetVMove(MS_X0, fVel, fAcc) && m_pMotionCard->SetVMove(MS_Y0, fVel, fAcc));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->SetVMove(MS_X1, fVel, fAcc) && m_pMotionCard->SetVMove(MS_Y1, fVel, fAcc));
	else
		return (m_pMotionCard->SetVMove(nMotionId, fVel, fAcc));
#else
	return TRUE;
#endif
}

BOOL CMotion::VMove(int nMotionId, int nDir)
{	
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->VMove(MS_X0, nDir) && m_pMotionCard->VMove(MS_Y0, nDir));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->VMove(MS_X1, nDir) && m_pMotionCard->VMove(MS_Y1, nDir));
	else
	{
		if (nMotionId == MS_X0 || nMotionId == MS_Y0)
		{
			if (m_pMotionCard->IsInterpolationMotion(0))
				m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		}
		if (nMotionId == MS_X1 || nMotionId == MS_Y1)
		{
			if (m_pMotionCard->IsInterpolationMotion(1))
				m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		}

		return (m_pMotionCard->VMove(nMotionId, nDir));
	}
#else
	return TRUE;
#endif
}

BOOL CMotion::Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait)
{	
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->Move(MS_X0, pTgtPos, bAbs, bWait) && m_pMotionCard->Move(MS_Y0, pTgtPos, bAbs, bWait));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->Move(MS_X1, pTgtPos, bAbs, bWait) && m_pMotionCard->Move(MS_Y1, pTgtPos, bAbs, bWait));
	else
	{
		if (nMotionId == MS_X0 || nMotionId == MS_Y0)
		{
			if (m_pMotionCard->IsInterpolationMotion(0))
				m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		}
		if (nMotionId == MS_X1 || nMotionId == MS_Y1)
		{
			if (m_pMotionCard->IsInterpolationMotion(1))
				m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		}

		return (m_pMotionCard->Move(nMotionId, pTgtPos, bAbs, bWait)); 
	}
#else
	return TRUE;
#endif
}

BOOL CMotion::Move(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait)
{	
#ifdef USE_NMC
	BOOL bOk = FALSE;
	if (nMotionId == MS_X0Y0)
	{
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		bOk = m_pMotionCard->TwoStartPosMove0(MS_X0, MS_Y0, pTgtPos[0], pTgtPos[1], dRatio, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		return bOk;
	}
	else if (nMotionId == MS_X1Y1)
	{
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		bOk = m_pMotionCard->TwoStartPosMove1(MS_X1, MS_Y1, pTgtPos[0], pTgtPos[1], dRatio, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		return bOk;
	}

	if (nMotionId == MS_X0 || nMotionId == MS_Y0)
	{
		if (m_pMotionCard->IsInterpolationMotion(0))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
	}
	if (nMotionId == MS_X1 || nMotionId == MS_Y1)
	{
		if (m_pMotionCard->IsInterpolationMotion(1))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
	}

	return (m_pMotionCard->Move(nMotionId, pTgtPos, dRatio, bAbs, bWait));
#else
	return TRUE;
#endif
}

BOOL CMotion::Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
#ifdef USE_NMC
	BOOL bOk = FALSE;
	if (nMotionId == MS_X0Y0)
	{
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		bOk = m_pMotionCard->TwoStartPosMove0(MS_X0, MS_Y0, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		return bOk;
	}
	else if (nMotionId == MS_X1Y1)
	{
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		bOk = m_pMotionCard->TwoStartPosMove1(MS_X1, MS_Y1, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		return bOk;
	}

	if (nMotionId == MS_X0 || nMotionId == MS_Y0)
	{
		if (m_pMotionCard->IsInterpolationMotion(0))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
	}
	if (nMotionId == MS_X1 || nMotionId == MS_Y1)
	{
		if (m_pMotionCard->IsInterpolationMotion(1))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
	}

	return (m_pMotionCard->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait));
#else
	return TRUE;
#endif
}

BOOL CMotion::Move0(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
#ifdef USE_NMC
	BOOL bOk = FALSE;
	if (nMotionId == MS_X0Y0)
	{
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		bOk = m_pMotionCard->TwoStartPosMove0(MS_X0, MS_Y0, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		return bOk;
	}
	else if (nMotionId == MS_X1Y1)
	{
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		bOk = m_pMotionCard->TwoStartPosMove1(MS_X1, MS_Y1, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		return bOk;
	}

	return (m_pMotionCard->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait));
#else
	return TRUE;
#endif
}

BOOL CMotion::Move1(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
#ifdef USE_NMC
	BOOL bOk = FALSE;
	if (nMotionId == MS_X0Y0)
	{
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		bOk = m_pMotionCard->TwoStartPosMove0(MS_X0, MS_Y0, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait);
		//if (m_pMotionCard->IsInterpolationMotion(0))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
		return bOk;
	}
	else if (nMotionId == MS_X1Y1)
	{
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		bOk = m_pMotionCard->TwoStartPosMove1(MS_X1, MS_Y1, pTgtPos[0], pTgtPos[1], dSpd, dAcc, dDec, bAbs, bWait); 
		//if (m_pMotionCard->IsInterpolationMotion(1))
		//	m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
		return bOk;
	}

	return (m_pMotionCard->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait));
#else
	return TRUE;
#endif
}

BOOL CMotion::Move(int nMotionId, double dTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0 || nMotionId == MS_Y0)
	{
		if (m_pMotionCard->IsInterpolationMotion(0))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[0]);
	}
	if (nMotionId == MS_X1 || nMotionId == MS_Y1)
	{
		if (m_pMotionCard->IsInterpolationMotion(1))
			m_pMotionCard->UnGroup2Ax(m_nBoardId, m_nGroupID_Interpolation[1]);
	}

	return (m_pMotionCard->Move(nMotionId, dTgtPos, dSpd, dAcc, dDec, bAbs, bWait));
#else
	return TRUE;
#endif
}

BOOL CMotion::IsMotionDone(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->IsMotionDone(MS_X0) && m_pMotionCard->IsMotionDone(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->IsMotionDone(MS_X1) && m_pMotionCard->IsMotionDone(MS_Y1));
	else
		return (m_pMotionCard->IsMotionDone(nMotionId));
#else
return TRUE;
#endif
}

BOOL CMotion::IsInPosition(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->IsInPosition(MS_X0) && m_pMotionCard->IsInPosition(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->IsInPosition(MS_X1) && m_pMotionCard->IsInPosition(MS_Y1));
	else
		return (m_pMotionCard->IsInPosition(nMotionId));
#else
return TRUE;
#endif
}

//BOOL CMotion::MovePath(int nMotionId)
//{
//	return (m_pMotionCard->MovePath(nMotionId));
//}
//
//BOOL CMotion::SetErrMap(CMyFileErrMap *pMyFile)
//{
//	return (m_pMotionCard->SetErrMap(pMyFile));
//}

double CMotion::GetActualPosition(int nAxisId)
{
	double dPos = -10000.0;
	m_csGetActualPosition.Lock();
#ifdef USE_NMC
	dPos = m_pMotionCard->GetActualPosition(nAxisId);
#endif
	m_csGetActualPosition.Unlock();
	return(dPos);
}

double CMotion::GetActualVelocity(int nAxisId)
{
	double dVel = 0.0;
	m_csGetActualVelocity.Lock();
#ifdef USE_NMC
	dVel = m_pMotionCard->GetActualVelocity(nAxisId);
#endif
	m_csGetActualVelocity.Unlock();
	return(dVel);
}

BOOL CMotion::ObjectMapping()
{
	//return (m_pMotionCard->ObjectMapping());
	return TRUE;
}

BOOL CMotion::Stop(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->Stop(MS_X0) && m_pMotionCard->Stop(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->Stop(MS_X1) && m_pMotionCard->Stop(MS_Y1));
	else
		return (m_pMotionCard->Stop(nMotionId));
#else
	return TRUE;
#endif
}

BOOL CMotion::EStop(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->EStop(MS_X0) && m_pMotionCard->EStop(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->EStop(MS_X1) && m_pMotionCard->EStop(MS_Y1));
	else
		return (m_pMotionCard->EStop(nMotionId));
#else
	return TRUE;
#endif
}

BOOL CMotion::VMoveStop(int nMotionId, int nDir)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->VMoveStop(MS_X0, nDir) && m_pMotionCard->VMoveStop(MS_Y0, nDir));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->VMoveStop(MS_X1, nDir) && m_pMotionCard->VMoveStop(MS_Y1, nDir));
	else
		return (m_pMotionCard->VMoveStop(nMotionId, nDir));
#else
	return TRUE;
#endif
}

BOOL CMotion::IsLimit(int nMotionId, int nDir)
{
#ifdef USE_NMC
	return (m_pMotionCard->IsLimit(nMotionId, nDir));
#else
	return TRUE;
#endif
}

BOOL CMotion::Clear(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->Clear(MS_X0) && m_pMotionCard->Clear(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->Clear(MS_X1) && m_pMotionCard->Clear(MS_Y1));
	else
		return (m_pMotionCard->Clear(nMotionId));
#else
	return TRUE;
#endif
}

//long CMotion::ReadAdc(int nSegment, int nCh)
//{
//	m_csReadAdc.Lock();
//	long lAdc = m_pMotionCard->ReadAdc(nSegment, nCh);
//	m_csReadAdc.Unlock();
//	return (lAdc);
//}

//BOOL CMotion::ReadBit(BYTE cBit, BOOL bInput)
//{
//	return (m_pMotionCard->ReadBit(cBit, bInput));
//}
//
//unsigned long CMotion::ReadAllBit(BOOL bInput)
//{
//	return (m_pMotionCard->ReadAllBit(bInput));
//}
//
//void CMotion::WriteBit(BYTE cBit, BOOL bOn)
//{
//	m_pMotionCard->WriteBit(cBit, bOn);
//}
//
//void CMotion::WriteData(long lData)
//{
//	m_pMotionCard->WriteData(lData);
//}
//
//
//unsigned short CMotion::Read(int nSegment)
//{
//	return (m_pMotionCard->Read(nSegment));
//}
//
//void CMotion::Write(int nSegment, unsigned short nOut)
//{
//	m_pMotionCard->Write(nSegment, nOut);
//}


//long CMotion::SetNotifyFlush()
//{
//	return(m_pMotionCard->SetNotifyFlush());
//}

void CMotion::GetCamPinPos(double &dX, double &dY)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();

// 	dX = pView->m_pToolPathData->m_fptPinPos.x;
// 	dY = pView->m_pToolPathData->m_fptPinPos.y;
}

void CMotion::GetCamStPos(double &dX, double &dY)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();

// 	dX = pView->m_pToolPathData->m_fptLeftTop.x;
// 	dY = pView->m_pToolPathData->m_fptLeftTop.y;
}

//int CMotion::GetElementID(int nMotionId)
//{
//	return (m_nElementID = (int)m_pMotionCard->GetElementID(nMotionId));
//}

void CMotion::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();
	
	CWnd::OnTimer(nIDEvent);
}


void CMotion::SetPinPos(int nCam, int nMotionId, double &dX, double &dY)
{	
	m_dPinPosX[nCam] = dX = GetActualPosition(AXIS_X0);
	m_dPinPosY[nCam] = dY = GetActualPosition(AXIS_Y0);
}

void CMotion::SetPinPos(int nCam, double dX, double dY)
{	
	m_dPinPosX[nCam] = dX;
	m_dPinPosY[nCam] = dY;
}

void CMotion::GetPinPos(int nCam, double &dX, double &dY)
{	
	dX = m_dPinPosX[nCam];
	dY = m_dPinPosY[nCam];
}

// void CMotion::SetStBufPos(double dPos)
// {	
// 	m_dStBufPos = dPos;
// }

void CMotion::GetMarkOffset(double &dX, double &dY)
{
	dX = m_dMarkOffsetX;
	dY = m_dMarkOffsetY;
}

void CMotion::SetMarkOffset(double dX, double dY)
{
	m_bMarkOffset = TRUE;
	m_dMarkOffsetX = dX;
	m_dMarkOffsetY = dY;
}

//BOOL CMotion::SetFeedRate(int nMotionId, double dFeedRate)
//{
//	m_dFeedRate[nMotionId] = dFeedRate;
//	return (m_pMotionCard->SetFeedRate(nMotionId, dFeedRate));
//}
//
//float CMotion::GetEStopTime(int nMotionId)
//{
//	return (m_pMotionCard->GetEStopTime(nMotionId));
//}
//
//void CMotion::SetEStopTime(int nMotionId, float fEStopTime)
//{
//	m_pMotionCard->SetEStopTime(nMotionId, fEStopTime);
//}
//
//void CMotion::ResetEStopTime(int nMotionId)
//{
//	m_pMotionCard->ResetEStopTime(nMotionId);
//}

long CMotion::GetState(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->GetState(MS_X0) | m_pMotionCard->GetState(MS_Y0));
	if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->GetState(MS_X1) | m_pMotionCard->GetState(MS_Y1));
	else
		return (m_pMotionCard->GetState(nMotionId));
#else
	return 0L;
#endif
}

BOOL CMotion::Abort(int nMotionId)
{
#ifdef USE_NMC
	if (nMotionId == MS_X0Y0)
		return (m_pMotionCard->Abort(MS_X0) && m_pMotionCard->Abort(MS_Y0));
	else if (nMotionId == MS_X1Y1)
		return (m_pMotionCard->Abort(MS_X1) && m_pMotionCard->Abort(MS_Y1));
	else
		return (m_pMotionCard->Abort(nMotionId));
#else
	return TRUE;
#endif
}

//long CMotion::SetInposition(int nAxis, double fInpRange)
//{
//	return (m_pMotionCard->SetInposition(nAxis, fInpRange));
//}

//long CMotion::SetOriginPos(int nAxisId)
//{
//	return (m_pMotionCard->SetOriginPos(nAxisId));
//}

double CMotion::GetSpeedProfile(int nMode,int nAxisID,double fLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType)
{
	if (nAxisID == MS_X0Y0)
		nAxisID = MS_X0;
	else if(nAxisID == MS_X1Y1)
		nAxisID = MS_X1;

	double fMotionTime,fAccTime,fVelocityTime,fSettleTime,fTotalTime;
	
	double fMaxVelocity = m_pParamMotor[nAxisID].fLeadPitch*(m_pParamMotor[nAxisID].fRatingSpeed/60.); // [mm/s]
	double fMaxAccel = m_pParamAxis[nAxisID].fMaxAccel; //[mm/s2]
	double fVelocity = m_pParamAxis[nAxisID].fSpd;
	double fMinJerkTime = m_pParamAxis[nAxisID].fMinJerkTime;
	double fMaxJerk = fMaxAccel/fMinJerkTime; //[mm/s3]
	double fAccPeriod = m_pParamAxis[nAxisID].fAccPeriod;
	double fAccLength = 0.0;

	if(nMode == S_CURVE)
	{

		if(nSpeedType == LOW_SPEED)
		{
			fVelocity /= 3.0;
			fMaxAccel /= 3.0;
			fMaxJerk /= 3.0;
			fMinJerkTime *= 3.0;
		}
		if(nSpeedType == MIDDLE_SPEED)
		{
			fVelocity /= 2.0;
			fMaxAccel /= 2.0;
			fMaxJerk /= 2.0;
			fMinJerkTime *=2.0;
		}
		
		GetSCurveVelocity(fLength,fVelocity,fMaxAccel,fMaxJerk,fMinJerkTime);
		fVel = fVelocity;
		fAcc = fMaxAccel;
		fJerk = fMaxJerk;

		fMotionTime = GetMotionTime(fLength,fVelocity,fAcc,fJerk);
		fSettleTime = 0.1;
		fTotalTime = fMotionTime + fSettleTime;
//		fJerkTime = GetJerkTime(fAcc,fJerk);
//		fAccTime = GetAccTime(fVelocity,fAcc,fJerk);
//		fVelocityTime = GetVelTime(fLength,fVelocity,fAcc,fJerk);
//		fSettleTime = 0.1;
//		fTotalTime = fJerkTime*4 + fAccTime*2 + fVelocityTime + fSettleTime;
	}
	else if(nMode == TRAPEZOIDAL)
	{		
		fAcc = m_pParamAxis[nAxisID].fAcc;

		// 가속도 값이 최대 가속도이내로 설정되도록 한다. 
		if(fAcc<=0.0 || fAcc > fMaxAccel) 
			fAcc = fMaxAccel;
		
		// 가속 구간 값이 50%를 초과할경우 50%로 규제한다.
		if(fAccPeriod > 50.0)
			fAccPeriod = 50.0;

		if(fAccPeriod < 50.0) // trapezoidal profile
		{
			// 전체 이동 거리중 가속구간의 거리 및 소요 시간 계산,
//			fAccLength = fLength - (fLength*fAccPeriod*2.0/100.0);
			fAccLength = (fLength*fAccPeriod*2.0/100.0);
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}
		else // triangular profile
		{
			// 전체 이동 거리의 1/2구간의 거리 및 소요 시간 계산,
			fAccLength = fLength/2.0;
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}

		// 최대속도 도달시간.
		double fAccTimeToMaxSpeed = fMaxVelocity / fAcc;
		if(fAccTime > fAccTimeToMaxSpeed)
		{
			fAccTime = fAccTimeToMaxSpeed;
			fVel = fMaxVelocity;
		}
		else
		{
			fVel = fAcc*fAccTime;
		}

		// 정속 구간의 이동시간을 구한다.
		fVelocityTime = (fLength-(fAccLength*2.0))/fVel;
		//위치 정정 시간을 설정한다.
		double fSettleTime = 0.1;

		fTotalTime = fAccTime*2.0 + fVelocityTime + fSettleTime;

		if(nSpeedType == LOW_SPEED)
		{
			fVel /= 3.0;
			fAcc /= 3.0;
		}
		else if(nSpeedType == MIDDLE_SPEED)
		{
			fVel /= 2.0;
			fAcc /= 2.0;
		}

	}
	return fTotalTime;
}

double CMotion::GetSpeedProfile0(int nMode,int nAxisID,double fLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType)
{
	if (nAxisID == MS_X0Y0)
		nAxisID = MS_X0;
	else if(nAxisID == MS_X1Y1)
		nAxisID = MS_X1;

	double fMotionTime, fAccTime, fVelocityTime, fSettleTime, fTotalTime;
	
	double fMaxVelocity = m_pParamMotor[nAxisID].fLeadPitch*(m_pParamMotor[nAxisID].fRatingSpeed/60.); // [mm/s]
	double fMaxAccel = m_pParamAxis[nAxisID].fMaxAccel; //[mm/s2]
	double fVelocity = m_pParamAxis[nAxisID].fSpd;
	double fMinJerkTime = m_pParamAxis[nAxisID].fMinJerkTime;
	double fMaxJerk = fMaxAccel/fMinJerkTime; //[mm/s3]
	double fAccPeriod = m_pParamAxis[nAxisID].fAccPeriod;
	double fAccLength = 0.0;

	if(nMode == S_CURVE)
	{

		if(nSpeedType == LOW_SPEED)
		{
			fVelocity /= 3.0;
			fMaxAccel /= 3.0;
			fMaxJerk /= 3.0;
			fMinJerkTime *= 3.0;
		}
		if(nSpeedType == MIDDLE_SPEED)
		{
			fVelocity /= 2.0;
			fMaxAccel /= 2.0;
			fMaxJerk /= 2.0;
			fMinJerkTime *=2.0;
		}
		
		GetSCurveVelocity(fLength,fVelocity,fMaxAccel,fMaxJerk,fMinJerkTime);
		fVel = fVelocity;
		fAcc = fMaxAccel;
		fJerk = fMaxJerk;

		fMotionTime = GetMotionTime(fLength,fVelocity,fAcc,fJerk);
		fSettleTime = 0.1;
		fTotalTime = fMotionTime + fSettleTime;
//		fJerkTime = GetJerkTime(fAcc,fJerk);
//		fAccTime = GetAccTime(fVelocity,fAcc,fJerk);
//		fVelocityTime = GetVelTime(fLength,fVelocity,fAcc,fJerk);
//		fSettleTime = 0.1;
//		fTotalTime = fJerkTime*4 + fAccTime*2 + fVelocityTime + fSettleTime;
	}
	else if(nMode == TRAPEZOIDAL)
	{		
		fAcc = m_pParamAxis[nAxisID].fAcc;

		// 가속도 값이 최대 가속도이내로 설정되도록 한다. 
		if(fAcc<=0.0 || fAcc > fMaxAccel) 
			fAcc = fMaxAccel;
		
		// 가속 구간 값이 50%를 초과할경우 50%로 규제한다.
		if(fAccPeriod > 50.0)
			fAccPeriod = 50.0;

		if(fAccPeriod < 50.0) // trapezoidal profile
		{
			// 전체 이동 거리중 가속구간의 거리 및 소요 시간 계산,
//			fAccLength = fLength - (fLength*fAccPeriod*2.0/100.0);
			fAccLength = (fLength*fAccPeriod*2.0/100.0);
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}
		else // triangular profile
		{
			// 전체 이동 거리의 1/2구간의 거리 및 소요 시간 계산,
			fAccLength = fLength/2.0;
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}

		// 최대속도 도달시간.
		double fAccTimeToMaxSpeed = fMaxVelocity / fAcc;
		if(fAccTime > fAccTimeToMaxSpeed)
		{
			fAccTime = fAccTimeToMaxSpeed;
			fVel = fMaxVelocity;
		}
		else
		{
			fVel = fAcc*fAccTime;
		}

		// 정속 구간의 이동시간을 구한다.
		fVelocityTime = (fLength-(fAccLength*2.0))/fVel;
		//위치 정정 시간을 설정한다.
		double fSettleTime = 0.1;

		fTotalTime = fAccTime*2.0 + fVelocityTime + fSettleTime;

		if(nSpeedType == LOW_SPEED)
		{
			fVel /= 3.0;
			fAcc /= 3.0;
		}
		else if(nSpeedType == MIDDLE_SPEED)
		{
			fVel /= 2.0;
			fAcc /= 2.0;
		}

	}
	return fTotalTime;
}

double CMotion::GetSpeedProfile1(int nMode,int nAxisID,double fLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType)
{
	if (nAxisID == MS_X0Y0)
		nAxisID = MS_X0;
	else if (nAxisID == MS_X1Y1)
		nAxisID = MS_X1;

	double fMotionTime,fAccTime,fVelocityTime,fSettleTime,fTotalTime;
	
	double fMaxVelocity = m_pParamMotor[nAxisID].fLeadPitch*(m_pParamMotor[nAxisID].fRatingSpeed/60.); // [mm/s]
	double fMaxAccel = m_pParamAxis[nAxisID].fMaxAccel; //[mm/s2]
	double fVelocity = m_pParamAxis[nAxisID].fSpd;
	double fMinJerkTime = m_pParamAxis[nAxisID].fMinJerkTime;
	double fMaxJerk = fMaxAccel/fMinJerkTime; //[mm/s3]
	double fAccPeriod = m_pParamAxis[nAxisID].fAccPeriod;
	double fAccLength = 0.0;

	if(nMode == S_CURVE)
	{

		if(nSpeedType == LOW_SPEED)
		{
			fVelocity /= 3.0;
			fMaxAccel /= 3.0;
			fMaxJerk /= 3.0;
			fMinJerkTime *= 3.0;
		}
		if(nSpeedType == MIDDLE_SPEED)
		{
			fVelocity /= 2.0;
			fMaxAccel /= 2.0;
			fMaxJerk /= 2.0;
			fMinJerkTime *=2.0;
		}
		
		GetSCurveVelocity(fLength,fVelocity,fMaxAccel,fMaxJerk,fMinJerkTime);
		fVel = fVelocity;
		fAcc = fMaxAccel;
		fJerk = fMaxJerk;

		fMotionTime = GetMotionTime(fLength,fVelocity,fAcc,fJerk);
		fSettleTime = 0.1;
		fTotalTime = fMotionTime + fSettleTime;
//		fJerkTime = GetJerkTime(fAcc,fJerk);
//		fAccTime = GetAccTime(fVelocity,fAcc,fJerk);
//		fVelocityTime = GetVelTime(fLength,fVelocity,fAcc,fJerk);
//		fSettleTime = 0.1;
//		fTotalTime = fJerkTime*4 + fAccTime*2 + fVelocityTime + fSettleTime;
	}
	else if(nMode == TRAPEZOIDAL)
	{		
		fAcc = m_pParamAxis[nAxisID].fAcc;

		// 가속도 값이 최대 가속도이내로 설정되도록 한다. 
		if(fAcc<=0.0 || fAcc > fMaxAccel) 
			fAcc = fMaxAccel;
		
		// 가속 구간 값이 50%를 초과할경우 50%로 규제한다.
		if(fAccPeriod > 50.0)
			fAccPeriod = 50.0;

		if(fAccPeriod < 50.0) // trapezoidal profile
		{
			// 전체 이동 거리중 가속구간의 거리 및 소요 시간 계산,
//			fAccLength = fLength - (fLength*fAccPeriod*2.0/100.0);
			fAccLength = (fLength*fAccPeriod*2.0/100.0);
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}
		else // triangular profile
		{
			// 전체 이동 거리의 1/2구간의 거리 및 소요 시간 계산,
			fAccLength = fLength/2.0;
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}

		// 최대속도 도달시간.
		double fAccTimeToMaxSpeed = fMaxVelocity / fAcc;
		if(fAccTime > fAccTimeToMaxSpeed)
		{
			fAccTime = fAccTimeToMaxSpeed;
			fVel = fMaxVelocity;
		}
		else
		{
			fVel = fAcc*fAccTime;
		}

		// 정속 구간의 이동시간을 구한다.
		fVelocityTime = (fLength-(fAccLength*2.0))/fVel;
		//위치 정정 시간을 설정한다.
		double fSettleTime = 0.1;

		fTotalTime = fAccTime*2.0 + fVelocityTime + fSettleTime;

		if(nSpeedType == LOW_SPEED)
		{
			fVel /= 3.0;
			fAcc /= 3.0;
		}
		else if(nSpeedType == MIDDLE_SPEED)
		{
			fVel /= 2.0;
			fAcc /= 2.0;
		}

	}
	return fTotalTime;
}

double CMotion::GetMotionTime(double dLen,double dVel,double dAcc,double dJerk)
{
	return dAcc/dJerk + dVel/dAcc + dLen/dVel;
}

double CMotion::GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk,double dMinJerkTime)
{
	do
	{
		if((dVel/dAcc) < (dAcc/dJerk))
			dVel = dAcc*dAcc/dJerk;
		double fTemp = (dLen/dVel - (dVel/dAcc + dAcc/dJerk));
		if(fTemp > 0)
			break;
		else if(fabs(fTemp) < 0.000000001)
			break;
		else
		{
			double a = 1/dAcc;
			double b = dAcc/dJerk;
			double c = -dLen;
			double r1 = (-b + sqrt(b*b-4*a*c)) / (2*a);
			double r2 = (-b - sqrt(b*b-4*a*c)) / (2*a);
			
			if(r1 > r2)
			{
				dVel = r1;
				if(dVel/dAcc < dAcc/dJerk)
					dAcc = sqrt(dJerk*dVel);
			}
			else
			{
				AfxMessageBox(_T("S-Curve 속도 프로파일 계산 에러 "));
//				AfxMessageBox(_T("Calculation Error at Speed profile of S-Curve motion"));
			}		
		}
	}while(1);

	return dVel;
}

BOOL CMotion::SetTorque(int nAxisId, double dKgf)
{
	//return(m_pMotionCard->SetTorque(nAxisId, dKgf));
	return TRUE;
}

void CMotion::SetR2RConf()
{
	//m_pMotionCard->SetR2RConf();
}

// void CMotion::SetLeadPitch(int nAxisId, double dPitch)
// {
// 	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
// 	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
// 	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();
// 
// 	CString strData, strTemp;
// 	strData.Format(_T(""));
// 	strTemp.Format(_T(""));
// 
// 	for(int nID=0; nID<m_ParamCtrl.nTotMotor; nID++)
// 	{
// 		if(nID == nAxisId)
// 			m_pParamMotor[nID].fLeadPitch = dPitch;
// 
// 		strTemp.Format(_T("%7.3f, "), m_pParamMotor[nID].fLeadPitch);
// 		
// 		if(!nID)
// 			strData.Insert(0, strTemp);
// 		else
// 			strData.Insert(strData.GetLength(), strTemp);
// 	}
// 
// 	int nPos = strData.ReverseFind(',');
// 	strData.Delete(nPos, 2);
// 	strTemp.Format(_T("					// Ball Screw Lead Pitch [mm]"));
// 	strData.Insert(strData.GetLength(), strTemp);
// 
// 	WritePrivateProfileString("MOTOR PARAM", "LEAD PITCH", (const char*)strData, m_sPathMotionParam);
// 
// 	m_pMotionCard->SetLeadPitch(nAxisId, dPitch);
// }
// 
// void CMotion::SetBufInitPos(double dPos)
// {
// 	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
// 	CGvisR2R_PunchView *pView = (CGvisR2R_PunchView *)pFrame->GetActiveView();
// 	CGvisR2R_PunchDoc *pDoc = (CGvisR2R_PunchDoc *)pFrame->GetActiveDocument();
// 
// 	m_dStBufPos = dPos;
// 	m_pMotionCard->SetBufInitPos(MS_MKFD, dPos);
// 
// 
// // 	CString strData, strTemp;
// // 	strData.Format(_T(""));
// // 	strTemp.Format(_T(""));
// // 
// // 	for(int nID=0; nID<m_ParamCtrl.nTotMotion; nID++)
// // 	{
// // 		if(nID == MS_MKFD)
// // 			m_pParamMotion[nID].Home.fShift = dPos;
// // 
// // 		strTemp.Format(_T("%.1f, "), m_pParamMotion[nID].Home.fShift);
// // 		
// // 		if(!nID)
// // 			strData.Insert(0, strTemp);
// // 		else
// // 			strData.Insert(strData.GetLength(), strTemp);
// // 	}
// // 
// // 	int nPos = strData.ReverseFind(',');
// // 	strData.Delete(nPos, 2);
// // 	strTemp.Format(_T("									// [mm] or [deg]"));
// // 	strData.Insert(strData.GetLength(), strTemp);
// // 
// // 	WritePrivateProfileString("HOME PARAM", "SHIFT", (const char*)strData, m_sPathMotionParam);
// // 
// // 	m_pMotionCard->SetShiftPos(MS_MKFD, dPos);
// }
// 
double CMotion::GetLeadPitch(int nAxisId)
{
	if (nAxisId == MS_X0Y0)
		nAxisId = MS_X0;
	else if (nAxisId == MS_X1Y1)
		nAxisId = MS_X1;

	return (m_pParamMotor[nAxisId].fLeadPitch);
}

BOOL CMotion::IsEnable(int nMsId)
{
#ifdef USE_NMC
	if (nMsId == MS_X0Y0)
		return (m_pMotionCard->IsEnable(MS_X0) && m_pMotionCard->IsEnable(MS_Y0));
	else if (nMsId == MS_X1Y1)
		return (m_pMotionCard->IsEnable(MS_X1) && m_pMotionCard->IsEnable(MS_Y1));
	else
		return (m_pMotionCard->IsEnable(nMsId));
#else
	return TRUE;
#endif
}

BOOL CMotion::IsServoOn(int nMotorID)
{
#ifdef USE_NMC
	if (nMotorID == MS_X0Y0)
		return (m_pMotionCard->IsServoOn(MS_X0) && m_pMotionCard->IsServoOn(MS_Y0));
	else if (nMotorID == MS_X1Y1)
		return (m_pMotionCard->IsServoOn(MS_X1) && m_pMotionCard->IsServoOn(MS_Y1));
	else
		return (m_pMotionCard->IsServoOn(nMotorID));
#else
	return TRUE;
#endif
}

BOOL CMotion::SetCollision(double dCollisionMargin)
{
	//return (m_pMotionCard->SetCollision(dCollisionMargin));
	return TRUE;
}

BOOL CMotion::ChkCollision()
{
	//return (m_pMotionCard->ChkXmpCollision());
	return ReadBit(0);
}

void CMotion::GetData(long *addressActPos1, long *addressActPos2, long *addressDifferenceStored)
{
	//m_pMotionCard->GetData(addressActPos1, addressActPos2, addressDifferenceStored);
	long lDiff = 0;
#ifdef USE_NMC
	*addressActPos1 = (long)m_pMotionCard->GetActualPosition(AXIS_X0);
	*addressActPos2 = (long)m_pMotionCard->GetActualPosition(AXIS_X1);
#endif
	*addressDifferenceStored = (lDiff = *addressActPos1 - *addressActPos2) > 0 ? lDiff : -lDiff;
}

void CMotion::MotionAbortAll()
{
	for (int nAxisId = 0; nAxisId < MAX_AXIS; nAxisId++)
	{
		MotionAbort(nAxisId);	// equalize Command position and Actual Position
	}
}

BOOL CMotion::MotionAbort(int nMsId)
{
	if (nMsId >= MAX_AXIS)
		return FALSE;

	return Abort(nMsId);
}