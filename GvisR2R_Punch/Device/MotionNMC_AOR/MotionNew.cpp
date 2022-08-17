// MotionNew.cpp : implementation file
//

#include "stdafx.h"

#ifdef USE_NMC

#include "MotionNew.h"
#include <math.h>
#include "stdafx.h"
#include "Controller.h"
#include "GvisAORDoc.h"
#include "GvisAORView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CMotionNew

CMotionNew::CMotionNew(CWnd* pParent,CString strFilePath)
{
	int i=0;
	UINT16 nBoardID = NMC_DEVICE_BOARD;
	UINT16 nDevIdIoIn = NMC_DEVICE_IOIN;
	UINT16 nDevIdIoOut = NMC_DEVICE_IOOUT;

	m_nBoardId = nBoardID;
	m_nDevIdIoIn = nDevIdIoIn;
	m_nDevIdIoOut = nDevIdIoOut;
	m_nOffsetAxisID = 1;
	//m_nGroupID_RTAF = NMC_GROUPID_RTAF;
	//m_nGroupID_ErrMapXY = NMC_GROUPID_ERRMAP;
	m_nGroupID_Interpolation = NMC_GROUPID_INTERPOLRATION;



	m_pbHomeActionAbort = NULL;
	m_pdVMoveVel = NULL;
	m_pdVMoveAcc = NULL;
	m_pParent = pParent;
	m_strMotionParamFilepath = strFilePath;



	m_pMotionCard = NULL;

	m_pParamMotion = NULL;
	m_pParamAxis = NULL;
	m_pParamMotor = NULL;


	for(i=0; i<32; i++)
		m_bDO[i] = FALSE;






	m_bPositiveOfLimitSensor = FALSE;
	m_nAxisOfLimitSensor = 0;

	LoadXmpParam();

#if USE_SSR_L_UL == USE;
	m_bMotionCreated = 0;
#endif

	if(m_ParamCtrl.nTotMotion > 0)
		for (i = 0; i < m_ParamCtrl.nTotMotion; i++)
			m_bOrigin[i] = FALSE;
	else
		for (i = 0; i < 4; i++)
			m_bOrigin[i] = FALSE;

	m_bAlarmCheck = FALSE;
}

CMotionNew::~CMotionNew()
{
#if USE_SSR_L_UL == USE
	m_bMotionCreated = 0;
#endif
	FreeAll();
}



/////////////////////////////////////////////////////////////////////////////
// CMotionNew message handlers

void CMotionNew::FreeAll()
{
	if(m_pMotionCard)			
	{
		WriteData(0);
		Sleep(100);

		m_pMotionCard->DestroyDevice();

		delete m_pMotionCard;
		m_pMotionCard = NULL;
		Sleep(500);
	}


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

	if (m_pdVMoveVel)
	{
		delete[] m_pdVMoveVel;
		m_pdVMoveVel = NULL;
	}

	if (m_pdVMoveAcc)
	{
		delete[] m_pdVMoveAcc;
		m_pdVMoveAcc = NULL;
	}

	if (m_pbHomeActionAbort)
	{
		delete[] m_pbHomeActionAbort;
		m_pbHomeActionAbort = NULL;
	}
}


void CMotionNew::LoadXmpParam()
{
	
	CString sVal, sIdx;

	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;

	int nID, nCol, i, nAxisID;


	// [CONTROL PARAM]
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotMotion = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotion = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL AXIS"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotAxis = _ttoi(szData);
	else
		m_ParamCtrl.nTotAxis = 0;

	m_nTotAxis = m_ParamCtrl.nTotAxis;

	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL MOTOR"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotMotor = _ttoi(szData);
	else
		m_ParamCtrl.nTotMotor = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL FILTER"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotFilter = _ttoi(szData);
	else
		m_ParamCtrl.nTotFilter = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL CAPTURE"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotCapture = _ttoi(szData);
	else
		m_ParamCtrl.nTotCapture = 0;
	if (0 < ::GetPrivateProfileString(_T("CONTROL PARAM"), _T("TOTAL SEQUENCE"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
		m_ParamCtrl.nTotSequence = _ttoi(szData);
	else
		m_ParamCtrl.nTotSequence = 0;

	// CreateObject - DataParam
	if (!m_pParamMotion)
		m_pParamMotion = new MotionMotion[m_ParamCtrl.nTotMotion];
	if (!m_pParamAxis)
		m_pParamAxis = new MotionAxis[m_ParamCtrl.nTotAxis];
	if (!m_pParamMotor)
		m_pParamMotor = new MotionMotor[m_ParamCtrl.nTotMotor];
	if (!m_pdVMoveVel)
		m_pdVMoveVel = new double[m_ParamCtrl.nTotAxis];
	if (!m_pdVMoveAcc)
		m_pdVMoveAcc = new double[m_ParamCtrl.nTotAxis];
	if(!m_pbHomeActionAbort)
		m_pbHomeActionAbort = new BOOL[m_ParamCtrl.nTotAxis];

	for (i = 0; i < m_ParamCtrl.nTotAxis; i++)
	{
		m_pbHomeActionAbort[i] = FALSE;
	}
	// [OBJECT MAPPING]
	for(nID=0; nID < m_ParamCtrl.nTotAxis; nID++)
	{
		sIdx.Format(_T("AXIS%d"), nID);
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
		if (0 < ::GetPrivateProfileString(_T("OBJECT MAPPING"), sIdx, NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("AXIS NAME"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("MAX ACCELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("MIN JERK TIME"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("AXIS PARAM"), _T("INPOSITION LENGTH"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR TYPE"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR RATING SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("MOTOR DIRECTION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("FEEDBACK SOURCE"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER PULSE"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("LEAD PITCH"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("ENCODER MULTIPLIER"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("GEAR RATIO"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("AMP ENABLE LEVEL"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("AMP FAULT LEVEL"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("POS LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("NEG LIMIT SENSOR LEVEL"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("HOME SENSOR LEVEL"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("MOTION PARAM"), _T("ESTOP TIME"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ACTION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("SEARCH DIRECTION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ESCAPE LENGTH"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("1'st SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("2'nd SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("SHIFT"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("HOME PARAM"), _T("OFFSET"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("DECELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("SPEED PARAM"), _T("ACCELERATION PERIOD"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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
	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("FAST SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("MIDDLE SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("LOW SPEED"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	if (0 < ::GetPrivateProfileString(_T("JOG PARAM"), _T("ACCELERATION"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

BOOL CMotionNew::InitNmcBoard()
{
	if(!m_pMotionCard)			
	{	

		m_pMotionCard = new CNmcDevice((CWnd *)this);

		if(!m_pMotionCard)			
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

	if (CreateObject())
	{
		SetConfigure();
		WriteData(0x01 << DO_MC);
		Sleep(100);
		m_pMotionCard->RestoreSwEmergency();	// -1: Fault , 1: Emergency Signal Off complete, 2: Previous Emergency Signal Off-state, 3: Normal
	}

#if USE_SSR_L_UL == USE
	m_bMotionCreated = 1;
#endif
	return TRUE;
}

BOOL CMotionNew::CreateObject()
{
	for (int nAxis = 0; nAxis < m_nTotAxis; nAxis++)
	{
		if (m_pMotionCard)
			m_pMotionCard->CreateAxis(nAxis);
	}

	return TRUE;
}

void CMotionNew::SetConfigure()
{
	if (!m_pMotionCard)
		return;

	m_pMotionCard->SetConfigure(m_nBoardId, m_nDevIdIoIn, m_nDevIdIoOut, m_nOffsetAxisID);
	SetMotionParam();
}

void CMotionNew::SetMotionParam()
{
	if (m_pMotionCard)
		m_pMotionCard->SetParam();
}

BOOL CMotionNew::AmpReset(int nAxisId)
{
	return Clear(nAxisId);
}

BOOL CMotionNew::ServoOnOff(int nAxisId,BOOL bOnOff)
{
	//return m_pMotionCard->m_pMotor[nAxisId].Enable(bOnOff);
	return m_pMotionCard->GetAxis(nAxisId)->SetAmpEnable(bOnOff);
}

BOOL CMotionNew::SearchHome()
{
#if USE_SSR_L_UL == USE
	g_nOriginStepStatus[ORG_STEP_X_AXIS] = 1;
	g_CurrentOrgStep = ORG_STEP_X_AXIS;
	g_nOriginStepStatus[ORG_STEP_Y_AXIS] = 1;
	g_CurrentOrgStep = ORG_STEP_Y_AXIS;
	g_nOriginStepStatus[ORG_STEP_FOCUS_AXIS] = 1;
	g_CurrentOrgStep = ORG_STEP_FOCUS_AXIS;
#endif
	for(int nMsID=0; nMsID<m_ParamCtrl.nTotMotion; nMsID++)
	{
		SearchHomePos(nMsID);
		Sleep(100);
	}
	return TRUE;
}

BOOL CMotionNew::SearchHomePos(int nMotionId, BOOL bThread)
{

	if (!m_pParamMotion[nMotionId].Home.bAct)
	{
#if USE_SSR_L_UL == USE
#ifdef SCAN_AXIS
		if (nMotionId == SCAN_AXIS)
			g_nOriginStepStatus[ORG_STEP_Y_AXIS] = 2;
#endif

#ifdef CAM_AXIS
		if (nMotionId == CAM_AXIS)
			g_nOriginStepStatus[ORG_STEP_X_AXIS] = 2;
#endif

#ifdef FOCUS_AXIS
		if (nMotionId == FOCUS_AXIS)
			g_nOriginStepStatus[ORG_STEP_FOCUS_AXIS] = 2;
#endif
#ifdef DUST_COVER_AXIS
		if (nMotionId == DUST_COVER_AXIS)
			g_nOriginStepStatus[ORG_STEP_COVER_AXIS] = 2;
#endif
#endif
		return TRUE;
	}

//	if (!m_pMotionCard->SearchHomePos(nMotionId, bThread))

	BOOL bRtn = FALSE;

#ifndef USE_NMC_HOME
	if (bThread)
	{
		m_bOrigin[nMotionId] = FALSE;
		m_pMotionCard->m_bOrigin[nMotionId] = FALSE;
		bRtn = m_pMotionCard->GetAxis(nMotionId)->StartHomeThread();
	}
	else
	{
		bRtn = m_pMotionCard->GetAxis(nMotionId)->StartHomming();
		if (bRtn)
		{
			m_bOrigin[nMotionId] = TRUE;
			m_pMotionCard->GetAxis(nMotionId)->SetOriginStatus(TRUE);
		}
		else
			m_pMotionCard->GetAxis(nMotionId)->SetOriginStatus(FALSE);
	}
#else
	int nDir = m_pParamMotion[nMotionId].Home.nDir == HOMING_DIR_CW ? HOMING_DIR_CW : HOMING_DIR_CCW;
	int mc = m_pMotionCard->StartRsaHoming(nMotionId, !bThread, HOMING_LIMIT_SWITCH, nDir);
	bRtn = (mc == MC_OK) ? TRUE : FALSE;

	if (!bThread && bRtn)
		m_bOrigin[nMotionId] = TRUE;
#endif

	if(!bRtn)
	{
#if USE_SSR_L_UL == USE
#ifdef SCAN_AXIS
		if (nMotionId == SCAN_AXIS)
			g_nOriginStepStatus[ORG_STEP_Y_AXIS] = 3;
#endif

#ifdef CAM_AXIS
		if (nMotionId == CAM_AXIS)
			g_nOriginStepStatus[ORG_STEP_X_AXIS] = 3;
#endif

#ifdef FOCUS_AXIS
		if (nMotionId == FOCUS_AXIS)
			g_nOriginStepStatus[ORG_STEP_FOCUS_AXIS] = 3;
#endif

#ifdef DUST_COVER_AXIS
			if (nMotionId == DUST_COVER_AXIS)
				g_nOriginStepStatus[ORG_STEP_COVER_AXIS] = 3;
#endif
#endif
		return FALSE;
	}

	return TRUE;
}


BOOL CMotionNew::IsHomeDone()
{
	return (m_pMotionCard->IsHomeDone());
}

BOOL CMotionNew::IsHomeDone(int nMotionId)
{
	return (m_pMotionCard->IsHomeDone(nMotionId));
}

BOOL CMotionNew::IsHomeSuccess()
{
	return (m_pMotionCard->IsHomeSuccess());
}

BOOL CMotionNew::IsHomeSuccess(int nMotionId)
{
	return (m_pMotionCard->IsHomeSuccess(nMotionId));
}

BOOL CMotionNew::SetVMove(int nMotionId, double fVel, double fAcc)
{
	return (m_pMotionCard->SetVMove(nMotionId, fVel, fAcc));
}

BOOL CMotionNew::VMove(int nMotionId, int nDir)
{	
	return (m_pMotionCard->VMove(nMotionId, nDir));
}

BOOL CMotionNew::Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait)
{	
	if (m_nTotAxis > 3)
	{
		if (nMotionId == DUST_COVER_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
		if (CGvisAORView::m_pAORMasterView)
		{
			if (CGvisAORView::m_pAORMasterView->m_pManager)
			{
				if (!CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor)
				{
					if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
					{
						SetHomeAction(nMotionId, MPIActionE_STOP);
						m_pbHomeActionAbort[nMotionId] = TRUE;
					}
				}
			}
		}
	}
	else
	{
		if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
	}
	//if (nMotionId == FOCUS_AXIS)
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	if (GetState(FOCUS_AXIS) == MPIStateMOVING)
	//	{
	//		if (GetVelocity(FOCUS_AXIS) == 0.0)
	//		{
	//			double dActPos = GetActualPosition(FOCUS_AXIS);
	//			double dCmdPos = GetCommandPosition(FOCUS_AXIS);

	//			long nActPulse = LengthToPulse(FOCUS_AXIS, dActPos);
	//			long nCmdPulse = LengthToPulse(FOCUS_AXIS, dCmdPos);
	//			long nInsp = LengthToPulse(FOCUS_AXIS, m_pParamAxis[FOCUS_AXIS].fInpRange);

	//			if (fabs(nActPulse - nCmdPulse) > nInsp)
	//			{
	//				Stop(FOCUS_AXIS);
	//				Clear(FOCUS_AXIS);

	//				SetCommandPos(FOCUS_AXIS, dActPos);
	//				Sleep(100);
	//				Clear(FOCUS_AXIS);
	//			}
	//		}
	//	}
	//	else if (GetState(FOCUS_AXIS) == MPIStateIDLE)
	//	{
	//		double dActPos = GetActualPosition(FOCUS_AXIS);
	//		double dCmdPos = GetCommandPosition(FOCUS_AXIS);

	//		long nActPulse = LengthToPulse(FOCUS_AXIS, dActPos);
	//		long nCmdPulse = LengthToPulse(FOCUS_AXIS, dCmdPos);
	//		long nInsp = LengthToPulse(FOCUS_AXIS, m_pParamAxis[FOCUS_AXIS].fInpRange);

	//		if (fabs(nActPulse - nCmdPulse) > nInsp)
	//		{
	//			Clear(FOCUS_AXIS);
	//			SetCommandPos(FOCUS_AXIS, dActPos);
	//			Sleep(30);
	//			Clear(FOCUS_AXIS);
	//		}
	//	}
	//	else if (GetState(FOCUS_AXIS) == MPIStateERROR)
	//	{
	//		double dActPos = GetActualPosition(FOCUS_AXIS);
	//		SetCommandPos(FOCUS_AXIS, dActPos);
	//		Clear(FOCUS_AXIS);
	//		ServoOnOff(FOCUS_AXIS, ON);
	//		Sleep(100);
	//		dActPos = GetActualPosition(FOCUS_AXIS);
	//		SetCommandPos(FOCUS_AXIS, dActPos);
	//	}
	//	//////////////////////////////////////////////////////////////////////////
	//}
	return (m_pMotionCard->Move(nMotionId, pTgtPos, bAbs, bWait));
}

BOOL CMotionNew::MoveRatio(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait)
{	
	if (m_nTotAxis > 3)
	{
		if (nMotionId == DUST_COVER_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
		if (CGvisAORView::m_pAORMasterView)
		{
			if (CGvisAORView::m_pAORMasterView->m_pManager)
			{
				if (!CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor)
				{
					if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
					{
						SetHomeAction(nMotionId, MPIActionE_STOP);
						m_pbHomeActionAbort[nMotionId] = TRUE;
					}
				}
			}
		}
	}
	else
	{
		if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
	}
	return (m_pMotionCard->Move(nMotionId, pTgtPos, dRatio, bAbs, bWait));
}

BOOL CMotionNew::Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
	if (m_nTotAxis > 3)
	{
		if (nMotionId == DUST_COVER_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
		if (CGvisAORView::m_pAORMasterView)
		{
			if (CGvisAORView::m_pAORMasterView->m_pManager)
			{
				if (!CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor)
				{
					if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
					{
						SetHomeAction(nMotionId, MPIActionE_STOP);
						m_pbHomeActionAbort[nMotionId] = TRUE;
					}
				}
			}
		}
	}
	else
	{
		if (nMotionId == CAM_AXIS || nMotionId == SCAN_AXIS)
		{
			SetHomeAction(nMotionId, MPIActionE_STOP);
			m_pbHomeActionAbort[nMotionId] = TRUE;
		}
	}
	return (m_pMotionCard->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait));
}

BOOL CMotionNew::IsMotionDone(int nMotionId)
{
	return (m_pMotionCard->IsMotionDone(nMotionId));
}

BOOL CMotionNew::IsInPosition(int nMotionId)
{
	return (m_pMotionCard->IsInPosition(nMotionId));
}






long CMotionNew::GetAxisState(int nAxisId)
{
	int dstate;
	dstate = m_pMotionCard->GetAxisState(nAxisId);
	return(dstate);
}

double CMotionNew::GetVelocity(int nAxisId)
{
	return 0.0;
	//return m_pMotionCard->GetVelocity(nAxisId);
}

BOOL CMotionNew::SetCommandPos(int nAxisID, double fPos)
{
	if (m_pMotionCard)
		m_pMotionCard->GetAxis(nAxisID)->SetCommandPosition(fPos);
	else
		return FALSE;

	return TRUE;
}

BOOL CMotionNew::SetActualPosition(int nAxisId, double fPos)
{
	if (m_pMotionCard)
		m_pMotionCard->GetAxis(nAxisId)->SetActualPosition(fPos);
	else
		return FALSE;

	return TRUE;
}

double CMotionNew::GetAxisVel(int nAxisId)
{
	return 0.0;
	//double dVel;
	//dVel = m_pMotionCard->GetAxisVel(nAxisId);
	//return(dVel);
}

BOOL CMotionNew::GetAxisAmpIsEnable(int nAxisId)
{
	BOOL bEnable;
	//bEnable = m_pMotionCard->GetAxisAmpIsEnable(nAxisId);
	bEnable = m_pMotionCard->GetAxis(nAxisId)->GetAmpEnable();
	return(bEnable);
}

double CMotionNew::LengthToPulse(int nIdxMotor, double fLength)
{
	return m_pMotionCard->Len2Pulse(nIdxMotor, fLength);
}

double CMotionNew::GetActualPosition(int nAxisId)
{
	double dPos = -10000.0;
	m_csGetActualPosition.Lock();
	dPos = m_pMotionCard->GetActualPosition(nAxisId);
	m_csGetActualPosition.Unlock();
	return(dPos);
}

BOOL CMotionNew::ObjectMapping()
{
	return TRUE;
	//return (m_pMotionCard->ObjectMapping());
}

BOOL CMotionNew::Stop(int nMotionId)
{
	return (m_pMotionCard->GetAxis(nMotionId)->Stop());
}

BOOL CMotionNew::EStop(int nMotionId)
{
	return (m_pMotionCard->GetAxis(nMotionId)->EStop());
}

BOOL CMotionNew::VMoveStop(int nMotionId, int nDir)
{
	return Stop(nMotionId);
	//return (m_pMotionCard->VMoveStop(nMotionId, nDir));
}

BOOL CMotionNew::ChkLimit(int nMotionId, int nDir)
{
	//return (m_pMotionCard->ChkLimit(nMotionId, nDir));
	return (!m_pMotionCard->GetAxis(nMotionId)->CheckLimitSwitch(nDir)); //?
}
BOOL CMotionNew::IsEnable(int nMotionId)
{
	return GetAxisAmpIsEnable(nMotionId);
	//return (m_pMotionCard->IsEnable(nMotionId));
}

BOOL CMotionNew::Clear(int nMotionId)
{
	return (m_pMotionCard->GetAxis(nMotionId)->ClearStatus());
}



BOOL CMotionNew::ReadBit(BYTE cBit, BOOL bInput)
{
	//return (m_pMotionCard->ReadBit(cBit, bInput));
	if (bInput)
	{
		return m_pMotionCard->ReadIn((long)cBit);
	}
	else
	{
		return m_pMotionCard->ReadOut((long)cBit);
	}

	return FALSE;
}

unsigned long CMotionNew::ReadAllBit(BOOL bInput)
{
	//return (m_pMotionCard->ReadAllBit(bInput));

	long nData;

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
}

void CMotionNew::WriteBit(BYTE cBit, BOOL bOn)
{
	//m_pMotionCard->WriteBit(cBit, bOn);
	m_pMotionCard->OutBit((long)cBit, bOn);
}

void CMotionNew::WriteData(long lData)
{
	//m_pMotionCard->WriteData(lData);
	m_pMotionCard->Out32(lData);
}

long CMotionNew::SetNotifyFlush()
{
	//return(m_pMotionCard->SetNotifyFlush());
	return 0L;
}




float CMotionNew::GetEStopTime(int nMotionId)
{
	//return (m_pMotionCard->GetEStopTime(nMotionId));
	return 0.0;
}

void CMotionNew::SetEStopTime(int nMotionId, float fEStopTime)
{
	//m_pMotionCard->SetEStopTime(nMotionId, fEStopTime);
}

void CMotionNew::ResetEStopTime(int nMotionId)
{
	//m_pMotionCard->ResetEStopTime(nMotionId);
}

long CMotionNew::GetState(int nMotionId)
{
	return (m_pMotionCard->GetAxisState(nMotionId));
}

BOOL CMotionNew::Abort(int nMotionId)
{
	//return (m_pMotionCard->Abort(nMotionId));

	if (nMotionId >= m_nTotAxis)
		return FALSE;

	return m_pMotionCard->GetAxis(nMotionId)->SetAmpEnable(FALSE);
}

double CMotionNew::GetInposition(int nAxis)
{
	//return (m_pMotionCard->GetInposition(nAxis));
	//return (m_pParamAxis[nAxis].fInpRange);
	return m_pMotionCard->GetAxis(nAxis)->m_stParam.Motor.fInpRange;
}
long CMotionNew::SetInposition(int nAxis, double fInpRange)
{
	//return (m_pMotionCard->SetInposition(nAxis, fInpRange));
	return (m_pMotionCard->GetAxis(nAxis)->SetInPosLength(fInpRange));
}




double CMotionNew::GetSpeedProfile(int nMode,int nAxisID,double fLength,double &fVel,double &fAcc,double &fJerk,int nSpeedType)
{
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


		if(fAcc<=0.0 || fAcc > fMaxAccel) 
			fAcc = fMaxAccel;
		

		if(fAccPeriod > 50.0)
			fAccPeriod = 50.0;

		if(fAccPeriod < 50.0) // trapezoidal profile
		{

			fAccLength = fLength - (fLength*fAccPeriod*2.0/100.0);
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}
		else // triangular profile
		{

			fAccLength = fLength/2.0;
			fAccTime = sqrt(2.0*fAccLength/fAcc);
		}


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

double CMotionNew::GetMotionTime(double dLen,double dVel,double dAcc,double dJerk)
{
	return dAcc/dJerk + dVel/dAcc + dLen/dVel;
}

double CMotionNew::GetSCurveVelocity(double dLen, double &dVel, double &dAcc, double &dJerk,double dMinJerkTime)
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

				AfxMessageBox(_T("Calculation Error at Speed profile of S-Curve motion"));
			}		
		}
	}while(1);

	return dVel;
}
void CMotionNew::SetJogSpd(int nSpd)
{
	double dSpdY, dAccY;
	for(int i=0;i<m_ParamCtrl.nTotMotion;i++)
	{
		switch(nSpd)
		{
		case 0: // VVL
			dSpdY = m_pParamMotion[i].Speed.fJogLowSpd / 200.0;
			dAccY = m_pParamMotion[i].Speed.fJogAcc / 200.0;
			break;
		case 1: // VL
			dSpdY = m_pParamMotion[i].Speed.fJogLowSpd / 50.0;
			dAccY = m_pParamMotion[i].Speed.fJogAcc / 50.0;
			break;
		case 2: // L
			dSpdY = m_pParamMotion[i].Speed.fJogLowSpd / 2.0;
			dAccY = m_pParamMotion[i].Speed.fJogAcc / 2.0;
			break;
		case 3: // M
			dSpdY = m_pParamMotion[i].Speed.fJogMidSpd / 2.0;
			dAccY = m_pParamMotion[i].Speed.fJogAcc / 2.0;
			break;
		case 4: // F
			dSpdY = m_pParamMotion[i].Speed.fJogFastSpd /2.0;
			dAccY = m_pParamMotion[i].Speed.fJogAcc /2.0;
			break;
		}
		m_pdVMoveVel[i] = dSpdY;
		m_pdVMoveAcc[i] = dAccY;
		SetVMove(i, dSpdY, dAccY);

	}
}


CNmcDevice *CMotionNew::GetMotionControl()
{
	return m_pMotionCard;
}

void CMotionNew::JogUp(int nMsId,int nDir)
{
	if(m_ParamCtrl.nTotMotion>nMsId)
	{
		VMoveStop(nMsId, nDir);
		Sleep(30);

		long lRtn = GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
		if(lRtn==2)//lRtn<0 || 
		{
			Abort(nMsId);
			Sleep(30);
		}

		Clear(nMsId);
		Sleep(30);

		if(lRtn==2)//lRtn<0 || 
		{
			ServoOnOff(nMsId, TRUE);

			Sleep(30);
		}
	}
}
void CMotionNew::JogDn(int nMsId,int nDir)
{
	if (m_ParamCtrl.nTotMotion > nMsId)
	{
		if (ChkLimit(nMsId, nDir))
		{
			if (m_nTotAxis > 3)
			{
				if (CGvisAORView::m_pAORMasterView)
				{
					if (CGvisAORView::m_pAORMasterView->m_pManager)
					{
						if (!CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor)
						{
							if (nMsId == CAM_AXIS || nMsId == SCAN_AXIS)
							{
								SetHomeAction(nMsId, MPIActionE_STOP);
								m_pbHomeActionAbort[nMsId] = TRUE;
							}
						}
					}
				}
			}
			else
			{
				if (nMsId == CAM_AXIS || nMsId == SCAN_AXIS)
				{
					SetHomeAction(nMsId, MPIActionE_STOP);
					m_pbHomeActionAbort[nMsId] = TRUE;
				}
			}
			SetVMove(nMsId, m_pdVMoveVel[nMsId] * nDir, m_pdVMoveAcc[nMsId]);
			VMove(nMsId, nDir);
		}
	}
}

double CMotionNew::GetPosSWLimitValue(int nAxisId)
{
	//return m_pMotionCard->GetPosSWLimitValue(nAxisId);
	return m_pMotionCard->GetAxis(nAxisId)->GetPosSoftwareLimit();
	//return m_fPosLimit[nAxisId];
}

double CMotionNew::GetNegSWLimitValue(int nAxisId)
{
	//	return m_pAoiMotionParam[nAxisId].Axis.fPosLimit;
	//return m_pMotionCard->GetNegSWLimitValue(nAxisId);
	return m_pMotionCard->GetAxis(nAxisId)->GetNegSoftwareLimit();
	//return m_fNegLimit[nAxisId];
}
double CMotionNew::GetCommandPosition(int nAxisId)
{
	//	return m_pAoiMotionParam[nAxisId].Axis.fPosLimit;
	//return m_pMotionCard->GetCommandPosition(nAxisId);
	return m_pMotionCard->GetAxis(nAxisId)->GetCommandPosition();
}
BOOL CMotionNew::WaitMotionDone(int nAxisId)
{
	//return (m_pMotionCard->WaitMotionDone(nAxisId));
	return (m_pMotionCard->GetAxis(nAxisId)->WaitUntilMotionDone());
}
BOOL CMotionNew::IsNegSWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsNegSWLimit(nMotorId));
	return m_pMotionCard->GetAxis(nMotorId)->CheckNegLimitSwitch();
}
BOOL CMotionNew::IsPosSWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsPosSWLimit(nMotorId));
	return m_pMotionCard->GetAxis(nMotorId)->CheckPosLimitSwitch();
}
BOOL CMotionNew::IsNegHWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsNegHWLimit(nMotorId));
	return (m_pMotionCard->GetAxis(nMotorId)->CheckNegLimitSwitch());
}
BOOL CMotionNew::IsPosHWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsPosHWLimit(nMotorId));
	return m_pMotionCard->GetAxis(nMotorId)->CheckPosLimitSwitch();
}
int CMotionNew::SearchMachineStroke(int nAxisId)
{
	// 현재위치를 파악
	double fCurPosition; // 20160104-syd
	fCurPosition = GetCommandPosition(nAxisId);
	// Check Origin Sensor and change Origin Position.
	SearchHomePos(nAxisId, FALSE);// syd-20110627

	double fSoftwareLimitMargin = 0.0;

	if (m_pParamMotor[nAxisId].bType == SERVO)
	{
		if (nAxisId == DUST_COVER_AXIS)
		{
			fSoftwareLimitMargin = 0.1;
		}
		else
		{
			fSoftwareLimitMargin = 1.0;
		}
	}
	else
	{
		fSoftwareLimitMargin = 0.05;
	}


	if (!GetAxisStroke(nAxisId, fSoftwareLimitMargin, fSoftwareLimitMargin))
		return FALSE;

	m_pParamMotor[nAxisId].fPosLimit = GetPosSWLimitValue(nAxisId);
	m_pParamMotor[nAxisId].fNegLimit = GetNegSWLimitValue(nAxisId);

	if (m_pParamMotor[nAxisId].bType == SERVO)
		MoveRatio(nAxisId, &fCurPosition, 25.0);
	else
		MoveRatio(nAxisId, &fCurPosition, 100.0);

	//m_pMotionCard->GetAxis(nAxisId)->SetPosSoftwareLimit(m_pParamMotor[nAxisId].fPosLimit, MPIActionE_STOP);
	//m_pMotionCard->GetAxis(nAxisId)->SetNegSoftwareLimit(m_pParamMotor[nAxisId].fNegLimit, MPIActionE_STOP);
	//m_pMotionCard->GetAxis(nAxisId)->SetPosHWLimitAction(MPIActionE_STOP);
	//m_pMotionCard->GetAxis(nAxisId)->SetNegHWLimitAction(MPIActionE_STOP);

	return TRUE;
}


// 지정된 축의 리미트센서 위치를 파악하여 소프트웨푳E리미트 값, 햨E최큱E스트로크 값을 결정하여 리턴한다.
int CMotionNew::GetAxisStroke(int nMotorId, double fPlusMargin, double fMinusMargin)
{
	int errCode = 0;

	if (!GetAxisAmpIsEnable(nMotorId))
		return FALSE;

	double fPosLimit, fNegLimit;

	Clear(nMotorId);
	while (!IsMotionDone(nMotorId))
	{
		Sleep(100);
	}

	// software limit value 최큱Eだ막?변컖E
	//ChangePosSWLimitValue(nMotorId, 10000.0, FALSE);
	//ChangeNegSWLimitValue(nMotorId, -10000.0, FALSE);

	//m_pMotionCard->SetPosSWLimitAction(nMotorId, MPIActionNONE);
	//m_pMotionCard->SetNegSWLimitAction(nMotorId, MPIActionNONE);
	m_pMotionCard->GetAxis(nMotorId)->SetPosSoftwareLimit(10000.0, MPIActionNONE);
	m_pMotionCard->GetAxis(nMotorId)->SetNegSoftwareLimit(-10000.0, MPIActionNONE);

	double dTempV;
	if (m_pParamMotion[nMotorId].Home.fEscLen < 0.0)
		dTempV = -m_pParamMotion[nMotorId].Home.fEscLen;
	else
		dTempV = m_pParamMotion[nMotorId].Home.fEscLen;


	//1. 원점 limit센서가 check된 상태에서는 일단 그센서위치를 벗엉救다.
	if (GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
	{
		Clear(nMotorId);
		StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f2ndSpd*-m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
		do {
		} while (GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir));
		Stop(nMotorId);
		while (!IsMotionDone(nMotorId))
		{
		}
		Clear(nMotorId);
	}

	//2. 원점 limit센서가 Check되햨E않은 위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점 센서를 찾음 
	StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
	while (!GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}
	while (!IsMotionDone(nMotorId))
	{
	}

	Clear(nMotorId);

	//3. Negative limit가 check된 상태에서 일단 일정위치 만큼 +쪽으로 센서위치를 벗엉救다.
	double dStartPosMove = dTempV*(-m_pParamMotion[nMotorId].Home.nDir);
	Move(nMotorId, &dStartPosMove, m_pParamMotion[nMotorId].Home.f2ndSpd, m_pParamMotion[nMotorId].Home.fAcc, m_pParamMotion[nMotorId].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}



	//float fEStopTime = m_pMotionCard->GetEStopTime(nMotorId);
	float fEStopTime = GetEStopTime(nMotorId);
	double fMachineSpeed, fENewStopTime;

#if USE_NEW_MOTION_SPEED == USE
	fMachineSpeed = m_pParamMotion[nMotorId].Home.f2ndSpd / 2.0;
	fENewStopTime = float(fMachineSpeed / 1000.0);
	if (fENewStopTime < 0.01)
		fENewStopTime = 0.01;
#else
	double fSpeed;
	double fTemp = m_pParamMotor[nMotorId].fLeadPitch / m_pParamMotor[nMotorId].nEncPulse;
	if (fTemp <= 0.001)
	{
		//fSpeed = (1.0*1000.0) / (m_pMotionCard->m_pMotor[nMotorId].m_fPosRes * 1000); // [mm/s]
		fSpeed = (1.0*1000.0) / (m_pMotionCard->GetAxis(nMotorId)->m_stParam.Motor.fPosRes * 1000); // [mm/s]
		fMachineSpeed = m_pMotionCard->Pulse2Len(nMotorId, fSpeed) / 2.0;
		fENewStopTime = float(fMachineSpeed / 1000.);

		//if (fMachineSpeed * fENewStopTime < m_pMotionCard->m_pMotor[nMotorId].m_fPosRes)	//20120601-ndy for ConvexDriver : Velocity * Time = Distance & Distance > LinearScaler Position Resolution
		if (fMachineSpeed * fENewStopTime < m_pMotionCard->GetAxis(nMotorId)->m_stParam.Motor.fPosRes)	//20120601-ndy for ConvexDriver : Velocity * Time = Distance & Distance > LinearScaler Position Resolution
			fENewStopTime = fENewStopTime * 100;
	}
	else
	{
		//fMachineSpeed = (m_pMotionCard->m_pMotor[nMotorId].m_fPosRes * 1000) / 2.0; // [mm/s]
		fMachineSpeed = (m_pMotionCard->GetAxis(nMotorId)->m_stParam.Motor.fPosRes * 1000.0) / 2.0; // [mm/s]
		fENewStopTime = float(fMachineSpeed / 1000.0);

		//if (fMachineSpeed * fENewStopTime < m_pMotionCard->m_pMotor[nMotorId].m_fPosRes)	//20120601-ndy for ConvexDriver
		if (fMachineSpeed * fENewStopTime < m_pMotionCard->GetAxis(nMotorId)->m_stParam.Motor.fPosRes)	//20120601-ndy for ConvexDriver
			fENewStopTime = fENewStopTime * 100.0;
	}
#endif




	//m_pMotionCard->SetEStopTime(nMotorId, fENewStopTime);
	SetEStopTime(nMotorId, fENewStopTime);

	// 홈방향으로 정밀 센싱을 함.
	StartVelMove(nMotorId, fMachineSpeed*m_pParamMotion[nMotorId].Home.nDir, 10.0*fMachineSpeed);
	while (!GetLimitInput(nMotorId, m_pParamMotion[nMotorId].Home.nDir))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}

	// 홈퉩EE위치를 체크.
	if (m_pParamMotion[nMotorId].Home.nDir == PLUS)
		fPosLimit = GetActualPosition(nMotorId);
	else
		fNegLimit = GetActualPosition(nMotorId);

	Sleep(100);

	Clear(nMotorId);
	//m_pMotionCard->SetEStopTime(nMotorId, fEStopTime);
	SetEStopTime(nMotorId, fEStopTime);



	//5. 원점 limit센서가 Check되햨E않은 위치에 모타가 있을때 
	// 원점 복귀 방향으로 움직이면서 일단 원점반큱E센서를 찾음 

	StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*-m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
	while (!GetLimitInput(nMotorId, -m_pParamMotion[nMotorId].Home.nDir))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}
	while (!IsMotionDone(nMotorId));


	Clear(nMotorId);

	// 원점방향으로 fEscLen만큼 벗엉毆.
	dStartPosMove = dTempV*(m_pParamMotion[nMotorId].Home.nDir);
	Move(nMotorId, &dStartPosMove, m_pParamMotion[nMotorId].Home.f2ndSpd, m_pParamMotion[nMotorId].Home.fAcc, m_pParamMotion[nMotorId].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}

#if USE_NEW_MOTION_SPEED == USE
	StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*-m_pParamMotion[nMotorId].Home.nDir, m_pParamMotion[nMotorId].Home.fAcc);
#else
	StartVelMove(nMotorId, m_pParamMotion[nMotorId].Home.f1stSpd*-m_pParamMotion[nMotorId].Home.nDir, 10.0*fMachineSpeed);
#endif

	while (!GetLimitInput(nMotorId, -m_pParamMotion[nMotorId].Home.nDir))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}

	// 원점 반큱E씔?위치를 체크함.
	if (m_pParamMotion[nMotorId].Home.nDir == PLUS)
		fNegLimit = GetActualPosition(nMotorId);
	else
		fPosLimit = GetActualPosition(nMotorId);

	Sleep(100);

	Clear(nMotorId);
	//m_pMotionCard->SetEStopTime(nMotorId, fEStopTime);
	SetEStopTime(nMotorId, fEStopTime);

	//원점방향으로 fEscLen만큼 벗엉毆.
	dStartPosMove = dTempV*(m_pParamMotion[nMotorId].Home.nDir);
	Move(nMotorId, &dStartPosMove, m_pParamMotion[nMotorId].Home.f2ndSpd, m_pParamMotion[nMotorId].Home.fAcc, m_pParamMotion[nMotorId].Home.fAcc, INC, NO_WAIT);
	while (!IsMotionDone(nMotorId) || !IsInPosition(nMotorId))
	{
		
		if (IsEmergency())   // IO 0 bit Emergency
		{
			Abort(nMotorId);
			return FALSE;
		}
		
	}


	m_pParamMotor[nMotorId].fPosLimit = fPosLimit - fabs(fPlusMargin);
	m_pParamMotor[nMotorId].fNegLimit = fNegLimit + fabs(fMinusMargin);

	//m_pMotionCard->SetPosSWLimitValue(nMotorId, m_pParamMotor[nMotorId].fPosLimit);
	//m_pMotionCard->SetNegSWLimitValue(nMotorId, m_pParamMotor[nMotorId].fNegLimit);
	m_pMotionCard->GetAxis(nMotorId)->SetPosSoftwareLimit(m_pParamMotor[nMotorId].fPosLimit, MPIActionE_STOP);
	m_pMotionCard->GetAxis(nMotorId)->SetNegSoftwareLimit(m_pParamMotor[nMotorId].fNegLimit, MPIActionE_STOP);

	return TRUE;
}

void CMotionNew::ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate)
{
	CString strData, strTemp;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;
	int nID, nComma;

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("POSITIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	m_pParamMotor[nMotorId].fPosLimit = fErrorLimitValue;
	//m_pMotionCard->ChangePosSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
	m_pMotionCard->GetAxis(nMotorId)->SetPosSoftwareLimit(fErrorLimitValue, DEFAULT_EVENT);

}

void CMotionNew::ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate)
{
	CString strData, strTemp;
	TCHAR szData[200];
	TCHAR sep[] = _T(",/;\r\n\t");
	TCHAR *token;
	TCHAR *stopstring;
	int nID, nComma;

	if (0 < ::GetPrivateProfileString(_T("MOTOR PARAM"), _T("NEGATIVE SOFTWARE LIMIT"), NULL, szData, sizeof(szData), m_strMotionParamFilepath))
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

	m_pParamMotor[nMotorId].fNegLimit = fErrorLimitValue;
	//m_pMotionCard->ChangeNegSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
	m_pMotionCard->GetAxis(nMotorId)->SetNegSoftwareLimit(fErrorLimitValue, DEFAULT_EVENT);

}

BOOL CMotionNew::GetLimitInput(int nMotorId, int nDir)
{
	BOOL bOnOff;

	if (nDir == PLUS)
	{
		//bOnOff = m_pMotionCard->IsLimit(nMotorId, CW);
		bOnOff = m_pMotionCard->GetAxis(nMotorId)->CheckPosLimitSwitch();
		if (!m_pParamMotor[nMotorId].bPosLimitLevel)
			bOnOff = !bOnOff;
	}
	else
	{
		//bOnOff = m_pMotionCard->IsLimit(nMotorId, CCW);
		bOnOff = m_pMotionCard->GetAxis(nMotorId)->CheckNegLimitSwitch();
		if (!m_pParamMotor[nMotorId].bNegLimitLevel)
			bOnOff = !bOnOff;
	}

	return bOnOff;
}

BOOL CMotionNew::StartVelMove(int nAxisId, double fVel, double fAcc)
{
	if (!m_pParamMotor[nAxisId].bPosLimitLevel)
	{
		//if ((!m_pMotionCard->IsLimit(nAxisId, PLUS) && fVel > 0.0) || (!m_pMotionCard->IsLimit(nAxisId, MINUS) && fVel < 0.0))
		if ((!m_pMotionCard->GetAxis(nAxisId)->CheckPosLimitSwitch() && fVel > 0.0) || (!m_pMotionCard->GetAxis(nAxisId)->CheckNegLimitSwitch() && fVel < 0.0))
			return FALSE;
	}
	else
	{
		//if ((m_pMotionCard->IsLimit(nAxisId, PLUS) && fVel > 0.0) || (m_pMotionCard->IsLimit(nAxisId, MINUS) && fVel < 0.0))
		if ((m_pMotionCard->GetAxis(nAxisId)->CheckPosLimitSwitch() && fVel > 0.0) || (m_pMotionCard->GetAxis(nAxisId)->CheckNegLimitSwitch() && fVel < 0.0))
			return FALSE;
	}

	if (GetAxisState(nAxisId) == MPIStateERROR)
	{
		//if ((m_pMotionCard->IsLimit(nAxisId, PLUS) && fVel > 0.0) || (m_pMotionCard->IsLimit(nAxisId, MINUS) && fVel < 0.0))
		if ((m_pMotionCard->GetAxis(nAxisId)->CheckPosLimitSwitch() && fVel > 0.0) || (m_pMotionCard->GetAxis(nAxisId)->CheckNegLimitSwitch() && fVel < 0.0))
			return FALSE;

		//if ((m_pMotionCard->IsLimit(nAxisId, PLUS) && fVel < 0.0) || (m_pMotionCard->IsLimit(nAxisId, MINUS) && fVel > 0.0))
		if ((m_pMotionCard->GetAxis(nAxisId)->CheckPosLimitSwitch() && fVel < 0.0) || (m_pMotionCard->GetAxis(nAxisId)->CheckNegLimitSwitch() && fVel > 0.0))
		{
			Abort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
			Clear(nAxisId);	// AMP를 idle상태로 픸E?磯?
			ServoOnOff(nAxisId, ON);	// AMP를 enable시킨다.
			Sleep(30);
		}
		else
		{
			if (/*IsEncoderFault(nAxisId) &&*/ m_pParamMotor[nAxisId].bType == SERVO)
			{
				return FALSE;
			}
			Abort(nAxisId);		// Command position컖EActual Position을 일치시킨다.
			Clear(nAxisId); // AMP를 idle상태로 픸E?磯?
			ServoOnOff(nAxisId, ON);		// AMP를 enable시킨다.
		}
	}

	m_pMotionCard->SetVMove(nAxisId, fVel, fAcc);
	return m_pMotionCard->VMove(nAxisId, fVel > 0.0 ? PLUS : MINUS);
}

BOOL CMotionNew::IsEmergency()
{
	BOOL bEmergency = FALSE;
#if USE_SSR_L_UL == USE
	g_bSafetySensorMsg = FALSE;
#endif
#if CUSTOMER == SAMSUNG_LCM
	if (!ReadBit(DI_SAFETY_SENSOR, TRUE) || ReadBit(DI_ESTOP, TRUE) || !ReadBit(DI_CYLINDER_OFF, TRUE))
	{
		bEmergency = TRUE;
	}
	else
	{
		if (m_bAlarmCheck)
		{
			if (ReadBit(DO_SOLPANEL, FALSE) && ReadBit(DO_RINGBLOWER, FALSE))
			{
				m_nCountAlarm++;
				if (m_nCountAlarm > 10)
				{
					if (ReadBit(DI_VACUUM, TRUE))
					{
						bEmergency = TRUE;
					}
				}
			}
			else
			{
				m_nCountAlarm = 0;
			}
		}
	}
#else
#if USE_3D_MODULE == USE
	if (CGvisAORDoc::m_pAORMasterDoc->m_bCheckLDTemp)
	{
		if (ReadBit(DI_LD_TEMP_ALARM, TRUE))
		{
			bEmergency = TRUE;
		}
	}
#endif
	if (m_nTotAxis > 3)
	{
		if (CGvisAORView::m_pAORMasterView->m_pManager)
		{
#if USE_SSR_L_UL == USE
			if (CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor) // 닫힌상태
			{
				if ((ReadBit(DI_ESTOP, TRUE)))
				{
					bEmergency = TRUE;
				}
			}
			else   //열린상태
			{
				//20200123 JCH 
				if (g_bSafetySensorDeActivate)
				{
					if ((ReadBit(DI_ESTOP, TRUE)))
					{
						bEmergency = TRUE;
						g_bSafetySensorDeActivate = 0;
					}
				}////
				else
				{
					if ((!ReadBit(DI_SAFETY_SENSOR, TRUE) || ReadBit(DI_ESTOP, TRUE)))
					{
						bEmergency = TRUE;
						if (!ReadBit(DI_SAFETY_SENSOR, TRUE))
						{
							g_bSafetySensorMsg = TRUE;
						}
						else
						{
							g_bSafetySensorMsg = FALSE;
						}
					}
				}
			}
#else

			if (CGvisAORView::m_pAORMasterView->m_pManager->m_bCoverSensor)
			{
				if ((ReadBit(DI_ESTOP, TRUE)))
				{
					bEmergency = TRUE;
				}
			}
			else
			{
				if ((!ReadBit(DI_SAFETY_SENSOR, TRUE) || ReadBit(DI_ESTOP, TRUE)))
				{
					bEmergency = TRUE;
				}
			}
#endif
		}
	}
	else
	{
#if USE_SSR_L_UL == USE
		if (g_bSafetySensorDeActivate) // JCH check
		{
			if ((ReadBit(DI_ESTOP, TRUE))) // 센서가 동작 않게 변경해주신거군요.
			{
				bEmergency = TRUE;
				g_bSafetySensorDeActivate = 0;
				g_bSafetySensorMsg = FALSE;
			}
		}
		else
		{
			if (!ReadBit(DI_SAFETY_SENSOR, TRUE) || ReadBit(DI_ESTOP, TRUE))
			{
				bEmergency = TRUE;
				if (!ReadBit(DI_SAFETY_SENSOR, TRUE))
				{
					g_bSafetySensorMsg = TRUE;
				}
				else
				{
					g_bSafetySensorMsg = FALSE;
				}
			}
		}
#else
		if (!ReadBit(DI_SAFETY_SENSOR, TRUE) || ReadBit(DI_ESTOP, TRUE))
		{
			bEmergency = TRUE;
		}
#endif
	}
#endif
	return bEmergency;
}

//BOOL CMotionNew::IsEmergency()
//{
//	return m_pMotionCard->IsEmergency();
//}



void CMotionNew::SetAlarm(BOOL bState)
{
	//m_pMotionCard->m_bAlarmCheck = bState;
	m_bAlarmCheck = bState;
}
void CMotionNew::TowerLampControl(int nLamp, BOOL bOnOff)
{
	//m_pMotionCard->TowerLampControl(nLamp, bOnOff);
	if (bOnOff)
	{
		if (nLamp == LAMP_RED)
		{
#if CUSTOMER == TOPPAN_SSR
			WriteBit(DO_TOWERR, TRUE);
#else
			WriteBit(DO_TOWERR, TRUE);
			WriteBit(DO_TOWERY, FALSE);
			WriteBit(DO_TOWERG, FALSE);
#endif
		}
		else if (nLamp == LAMP_YELLOW)
		{

#if CUSTOMER == TOPPAN_SSR
			WriteBit(DO_TOWERY, TRUE);
#elif CUSTOMER == SHINKO_SSR
			WriteBit(DO_TOWERR, FALSE);
			WriteBit(DO_TOWERY, FALSE);
			WriteBit(DO_TOWERG, FALSE);
#else
			WriteBit(DO_TOWERR, FALSE);
			WriteBit(DO_TOWERY, TRUE);
			WriteBit(DO_TOWERG, FALSE);
#endif
		}
		else if (nLamp == LAMP_GREEN)
		{
#if CUSTOMER == TOPPAN_SSR
			WriteBit(DO_TOWERG, TRUE);
#else
			WriteBit(DO_TOWERR, FALSE);
			WriteBit(DO_TOWERY, FALSE);
			WriteBit(DO_TOWERG, TRUE);
#endif
		}
		else if (nLamp == LAMP_ALL_OFF)
		{
			WriteBit(DO_TOWERR, FALSE);
			WriteBit(DO_TOWERY, FALSE);
			WriteBit(DO_TOWERG, FALSE);
		}
	}
	else
	{
		if (nLamp == LAMP_RED)
		{
			WriteBit(DO_TOWERR, FALSE);
		}
		else if (nLamp == LAMP_YELLOW)
		{
			WriteBit(DO_TOWERY, FALSE);
		}
		else if (nLamp == LAMP_GREEN)
		{
			WriteBit(DO_TOWERG, FALSE);
		}
	}
}

void CMotionNew::Alarm(BOOL bOnOff)
{
	//m_pMotionCard->Alarm(bOnOff);
	WriteBit(DO_BUZZER, bOnOff);
}

void CMotionNew::SetHomeAction(int nMotorID, MPIAction Action)
{
	//m_pMotionCard->SetHomeAction(nMotorID, Action);
	m_pMotionCard->GetAxis(nMotorID)->SetHomeAction(Action);
}
void CMotionNew::ResetHomeAction(int nMotorID)
{
	//m_pMotionCard->ResetHomeAction(nMotorID);
	m_pMotionCard->GetAxis(nMotorID)->SetHomeAction(NO_EVENT);
}
BOOL CMotionNew::GetOrgInput(int nMotorId)
{
	//return m_pMotionCard->GetOrgInput(nMotorId);
	return m_pMotionCard->GetAxis(nMotorId)->CheckHomeSwitch();
}
void CMotionNew::CheckAxisDustCover()
{
	if (m_nTotAxis > 3)
	{
		if (GetState(DUST_COVER_AXIS) == MPIStateMOVING)
		{
			if (GetVelocity(DUST_COVER_AXIS) == 0.0)
			{
				double dActPos = GetActualPosition(DUST_COVER_AXIS);
				double dCmdPos = GetCommandPosition(DUST_COVER_AXIS);

				long nActPulse = LengthToPulse(DUST_COVER_AXIS, dActPos);
				long nCmdPulse = LengthToPulse(DUST_COVER_AXIS, dCmdPos);
				long nInsp = LengthToPulse(DUST_COVER_AXIS, m_pParamAxis[DUST_COVER_AXIS].fInpRange);

				if (fabs(nActPulse - nCmdPulse) > nInsp)
				{
					Stop(DUST_COVER_AXIS);
					Clear(DUST_COVER_AXIS);
					SetCommandPos(DUST_COVER_AXIS, dActPos);
					Sleep(100);
					Clear(DUST_COVER_AXIS);
				}
			}
		}
		else if (GetState(DUST_COVER_AXIS) == MPIStateIDLE)
		{
			double dActPos = GetActualPosition(DUST_COVER_AXIS);
			double dCmdPos = GetCommandPosition(DUST_COVER_AXIS);

			long nActPulse = LengthToPulse(DUST_COVER_AXIS, dActPos);
			long nCmdPulse = LengthToPulse(DUST_COVER_AXIS, dCmdPos);
			long nInsp = LengthToPulse(DUST_COVER_AXIS, m_pParamAxis[DUST_COVER_AXIS].fInpRange);

			if (fabs(nActPulse - nCmdPulse) > nInsp)
			{
				Clear(DUST_COVER_AXIS);
				dActPos = GetActualPosition(DUST_COVER_AXIS);
				SetCommandPos(DUST_COVER_AXIS, dActPos);
				Sleep(30);
				if (!IsEnable(DUST_COVER_AXIS))
				{
					ServoOnOff(DUST_COVER_AXIS, ON);
					Sleep(100);
					dActPos = GetActualPosition(DUST_COVER_AXIS);
					SetCommandPos(DUST_COVER_AXIS, dActPos);
				}
			}
			else
			{
				if (!IsEnable(DUST_COVER_AXIS))
				{
					ServoOnOff(DUST_COVER_AXIS, ON);
					Sleep(100);
					dActPos = GetActualPosition(DUST_COVER_AXIS);
					SetCommandPos(DUST_COVER_AXIS, dActPos);
				}
			}
		}
		else if (GetState(DUST_COVER_AXIS) == MPIStateERROR)
		{
			Clear(DUST_COVER_AXIS);
			double dActPos = GetActualPosition(DUST_COVER_AXIS);
			SetCommandPos(DUST_COVER_AXIS, dActPos);
			ServoOnOff(DUST_COVER_AXIS, ON);
			Sleep(100);
			dActPos = GetActualPosition(DUST_COVER_AXIS);
			SetCommandPos(DUST_COVER_AXIS, dActPos);
		}
	}
}

#if USE_SSR_L_UL == USE
BOOL CMotionNew::ReadBitSeg(BYTE cBit, BOOL bInput, int nSeg)
{
	return (m_pMotionCard->ReadBitSegment(cBit, bInput, nSeg));
}

void CMotionNew::WriteBitSeg(int cBit, BOOL bOn, int nSeg)
{
	m_pMotionCard->WriteBitSegment(cBit, bOn, nSeg);
}
double CMotionNew::PulseToLength(int nAxisId, long nPulse)
{
	double fPulse = (double)nPulse;
	return m_pMotionCard->Pulse2Len(nAxisId, nPulse);
}

BOOL CMotionNew::SetCommandPosition(int nAxisId, double fData)
{
	return m_pMotionCard->SetCommandPosition(nAxisId, fData);
}

CString CMotionNew::GetMotorName(int nAxisId)
{
	return m_pParamAxis[nAxisId].sName;
}

BOOL CMotionNew::IsAmpFault(int nMotorId)
{
	return (m_pMotionCard->IsAmpFault(nMotorId));
}

BOOL CMotionNew::IsServoOn(int nMotorID)
{
	return (m_pMotionCard->IsServoOn(nMotorID));
}

BOOL CMotionNew::ClearMotionFault(int nMsId)
{
	return Clear(nMsId);
}

BOOL CMotionNew::CheckDoorOpenAndExitManual()
{
	if (g_bUseShinkoDoor == 0)
		return FALSE;

	if (ReadBitSeg(LD_SHK_DOOR, 1, LD_INSEGMENT))
	{
		CController::m_pController->AlarmCall(ALARM_DOOR_OPEN, 1);

		WriteBitSeg(EQ_SHK_ERROR, 1, LD_OUTSEGMENT);

		for (int nAxisId = 0; nAxisId < m_nTotAxis; nAxisId++)
		{
			EStop(nAxisId);
		}

		if (CController::m_pController->m_state == ECS_EQ_RUN)
		{
			CController::m_pController->ButtonCall(CTRL_STOP, 1);
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

BOOL CMotionNew::CheckShinkoDoorOpen()
{
	if (g_bUseShinkoDoor == 0)
		return FALSE;

	if (g_bInRepairThread)
	{
		return FALSE;
	}

	if (ReadBitSeg(LD_SHK_DOOR, 1, LD_INSEGMENT))
	{
		CController::m_pController->AlarmCall(ALARM_DOOR_OPEN, 1);

		WriteBitSeg(EQ_SHK_ERROR, 1, LD_OUTSEGMENT);

		for (int nAxisId = 0; nAxisId < m_nTotAxis; nAxisId++)
		{
			EStop(nAxisId);
		}

		if (CController::m_pController->m_state == ECS_EQ_RUN)
		{
			CController::m_pController->ButtonCall(CTRL_STOP, 1);
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}


void CMotionNew::MotorPowerOnOff(BOOL OnOff)
{
	WriteBit(DO_MC, OnOff);
	if (OnOff == ON)
		Sleep(2000);
}

int CMotionNew::FaultRestoreAndCheckReadyStatus()
{
	CString strLogMessage;
	CString strSequenceName = _T("모션 상태 확인 및 복구");
	BOOL bControllerReset = 0;

	if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
		strSequenceName = _T("모션 상태 확인 및 복구");
	else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
		strSequenceName = _T("モーション状態の確認と修復");
	else
		strSequenceName = _T("Check and restore motion status");

	SetMainMessage(strSequenceName);

	if (g_bAOIReady == 0)
		return FALSE;

	if (m_bMotionCreated == 0)
		return FALSE;

	if (ReadBit(DO_MC, 0) == 0)
	{
		MotorPowerOnOff(1);
	}

	if (1)
	{
		int i;
		int nFaultAxis = 0;
		for (i = 0; i < m_nTotAxis; i++)
		{
			long State = GetState(i);

			ClearMotionFault(i);
			Sleep(100);
			State = GetState(i);

			if (IsServoOn(i) == 0)
				ServoOnOff(i, 1);
			Sleep(100);
			ClearMotionFault(i);
			Sleep(100);

			State = GetState(i);

			double dCmd = GetCommandPosition(i);
			double dAct = GetActualPosition(i);

			double dActPls = LengthToPulse(i, dAct);
			double dCmdPls = LengthToPulse(i, dCmd);

			if (fabs(dActPls - dCmdPls) > 5)
			{
				SetCommandPosition(i, dAct);
			}

			if (1)
			{
				ClearMotionFault(i);
				Sleep(100);
				State = GetState(i);

				if (State == MPIStateERROR)
				{
					if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
						strLogMessage.Format(_T("[%s] %s축의 앰프 폴트 감지!!"), strSequenceName, GetMotorName(i));
					else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
						strLogMessage.Format(_T("[%s] %s軸アンプフォルト検出！"), strSequenceName, GetMotorName(i));
					else
						strLogMessage.Format(_T("[%s] Detection of %s axis amplifier fault !!"), strSequenceName, GetMotorName(i));

					SetMainMessage(strLogMessage);
					nFaultAxis++;
				}
			}

			if (State == MPIStateSTOPPING || State == MPIStateMOVING || State == MPIStateSTOPPING_ERROR)
			{
				if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
					strLogMessage.Format(_T("[%s] %s축의 이상동작 감지"), strSequenceName, GetMotorName(i));
				else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
					strLogMessage.Format(_T("[%s] %s軸以上の動作を検出"), strSequenceName, GetMotorName(i));
				else
					strLogMessage.Format(_T("[%s] %sAbnormal motion detection of axis"), strSequenceName, GetMotorName(i));

				SetMainMessage(strLogMessage);
				nFaultAxis++;
			}

			if (IsAmpFault(i))
			{
				if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
					strLogMessage.Format(_T("[%s] %s축의 앰프 폴트 감지!!"), strSequenceName, GetMotorName(i));
				else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
					strLogMessage.Format(_T("[%s] %s軸アンプフォルト検出！"), strSequenceName, GetMotorName(i));
				else
					strLogMessage.Format(_T("[%s] %sDetection of %s axis amplifier fault !!"), strSequenceName, GetMotorName(i));

				SetMainMessage(strLogMessage);
				nFaultAxis++;
			}

			if (IsHomeDone(i) == 0)
			{
				if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
					strLogMessage.Format(_T("[%s] %s축의 원점복귀 미동작 감지!!"), strSequenceName, GetMotorName(i));
				else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
					strLogMessage.Format(_T("[%s] %s軸の原点復帰未動作検知!!」"), strSequenceName, GetMotorName(i));
				else
					strLogMessage.Format(_T("[%s] Detection of origin motion of %s axis is not active !! "), strSequenceName, GetMotorName(i));

				SetMainMessage(strLogMessage);
				nFaultAxis++;
			}
			else
			{
				if (IsServoOn(i) == 0)
				{
					ServoOnOff(i, TRUE);
					Sleep(1000);

					State = GetState(i);

					if (State == MPIStateERROR)
					{
						ClearMotionFault(i);
						Sleep(500);

						State = GetState(i);

						if (State == MPIStateERROR)
						{
							if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
								strLogMessage.Format(_T("[%s] %s축의 앰프 폴트 감지!!"), strSequenceName, GetMotorName(i));
							else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
								strLogMessage.Format(_T("[%s] %s軸アンプフォルト検出！"), strSequenceName, GetMotorName(i));
							else
								strLogMessage.Format(_T("[%s] %sDetection of %s axis amplifier fault !!"), strSequenceName, GetMotorName(i));

							SetMainMessage(strLogMessage);
							nFaultAxis++;
						}
						else
						{
							if (IsAmpFault(i))
							{
								if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
									strLogMessage.Format(_T("[%s] %s축의 앰프 폴트 감지!!"), strSequenceName, GetMotorName(i));
								else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
									strLogMessage.Format(_T("[%s] %s軸アンプフォルト検出！"), strSequenceName, GetMotorName(i));
								else
									strLogMessage.Format(_T("[%s] %sDetection of %s axis amplifier fault !!"), strSequenceName, GetMotorName(i));

								SetMainMessage(strLogMessage);
								nFaultAxis++;
							}
							else
							{

							}
						}
					}
					else
					{
						if (IsAmpFault(i))
						{
							if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
								strLogMessage.Format(_T("[%s] %s축의 앰프 폴트 감지!!"), strSequenceName, GetMotorName(i));
							else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
								strLogMessage.Format(_T("[%s] %s軸アンプフォルト検出！"), strSequenceName, GetMotorName(i));
							else
								strLogMessage.Format(_T("[%s] %sDetection of %s axis amplifier fault !!"), strSequenceName, GetMotorName(i));

							SetMainMessage(strLogMessage);
							nFaultAxis++;
						}
						else
						{

						}
					}
				}

				double dPos = LengthToPulse(i, GetActualPosition(i));
				double dCmd = LengthToPulse(i, GetCommandPosition(i));

				if (fabs(dPos - dCmd) >= 10)
				{
					if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
						strLogMessage.Format(_T("[%s] %s축의 포지션 에러 감지"), strSequenceName, GetMotorName(i));
					else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
						strLogMessage.Format(_T("[%s] %s軸ポジションエラー検出"), strSequenceName, GetMotorName(i));
					else
						strLogMessage.Format(_T("[%s] Position error detection on axis %s"), strSequenceName, GetMotorName(i));

					SetMainMessage(strLogMessage);

					SetCommandPosition(i, dAct);
					Sleep(100);

					State = GetState(i);
					if (State == MPIStateERROR)
					{
						nFaultAxis++;
					}
				}
			}
		}

		if (nFaultAxis)
		{
			bControllerReset = TRUE;
		}
	}

	if (bControllerReset == 0)
	{
		m_pMotionCard->m_nInterlockStatus = 0;
	}

	return bControllerReset;
}



#endif

BOOL CMotionNew::IsLimitSensor(CString &strMsg)
{
	BOOL bLimit = FALSE;
	BOOL bPos = FALSE;
	BOOL bSW = FALSE;
	int nAxis = 0;
	for (int i = 0; i < 3; i++)
	{
		//if (GetMotionControl()->m_pMotion[i].m_bOrigin)
		if (GetMotionControl()->m_bOrigin[i])
		{
			if (IsNegSWLimit(i))
			{
				bPos = FALSE;
				bSW = TRUE;
				bLimit = TRUE;
				nAxis = i;
				break;
			}
			else if (IsPosSWLimit(i))
			{
				bPos = TRUE;
				bSW = TRUE;
				bLimit = TRUE;
				nAxis = i;
				break;
			}
			else if (IsNegHWLimit(i))
			{
				bPos = FALSE;
				bSW = FALSE;
				bLimit = TRUE;
				nAxis = i;
				break;
			}
			else if (IsPosHWLimit(i))
			{
				bPos = TRUE;
				bSW = FALSE;
				bLimit = TRUE;
				nAxis = i;
				break;
			}
		}
	}
	if (bLimit)
	{
		CString strAxisName;
		if (nAxis == 0)
		{
			strAxisName.Format(_T("X"));
		}
		else if (nAxis == 1)
		{
			strAxisName.Format(_T("Y"));
		}
		else
		{
			strAxisName.Format(_T("Focus"));
		}
		CString strPos;
		if (bPos)
		{
			strPos.Format(_T("+"));
		}
		else
		{
			strPos.Format(_T("-"));
		}
		if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::KOREAN)
			strMsg.Format(_T("%s 축 %s"), strAxisName, strPos);
		else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::JAPANESE)
			strMsg.Format(_T("%s 枢軸 %s"), strAxisName, strPos);
		else if (CGvisAORDoc::m_pAORMasterDoc->m_nCurrentLang == LanguageType::CHINESE)
			strMsg.Format(_T("%s 軸 %s"), strAxisName, strPos);
		else
			strMsg.Format(_T("%s Axis %s"), strAxisName, strPos);

		m_bPositiveOfLimitSensor = bPos;
		m_nAxisOfLimitSensor = nAxis;
	}
	return bLimit;
}

void CMotionNew::TriggerSetRange(int encAxisId, int nEcatAddr, int vAxisId, double dStPos, double dEdPos, double dPulseWd, double dTrigRes)	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
{
	m_pMotionCard->TriggerSetRange(encAxisId, nEcatAddr, vAxisId, dStPos, dEdPos, dPulseWd, dTrigRes);	// fDBNStartPos : mm , fDBNEndPos : mm , nPulseWidth : uSec [max 3276.8us = 65536 * 50nSec], fOptRes : um
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

int CMotionNew::MoveXY(int nAxisX, int nAxisY, double fTgtPosX, double fTgtPosY, BOOL bSpeed, BOOL bWait, BOOL bCheckClamp)
{
	if (fTgtPosX < GetNegSWLimitValue(nAxisX) || fTgtPosX > GetPosSWLimitValue(nAxisX))
	{
		return MOTION_LIMIT_OVER;
	}

	if (fTgtPosY < GetNegSWLimitValue(nAxisY) || fTgtPosY > GetPosSWLimitValue(nAxisY))
	{
		return MOTION_LIMIT_OVER;
	}

#ifdef USE_INTERPOLATION_MOVE
	m_pMotionCard->TwoStartPosMove(nAxisX, nAxisY, fTgtPosX, fTgtPosY, 75.0, ABS, NO_WAIT, OPTIMIZED);
#else
	m_pMotionCard->Move(nAxisX, &fTgtPosX, 75.0, ABS, NO_WAIT);
	m_pMotionCard->Move(nAxisY, &fTgtPosY, 75.0, ABS, NO_WAIT)
	//StartPosMove(nAxisX, fTgtPosX, 75.0, ABS, NO_WAIT, OPTIMIZED);
	//StartPosMove(nAxisY, fTgtPosY, 75.0, ABS, NO_WAIT, OPTIMIZED);
#endif

	if (bWait)
	{
		return WaitXYPosEnd(nAxisX, nAxisY);
	}

	return SUCCESS;
}

int CMotionNew::WaitXYPosEnd(int nAxisX, int nAxisY, DWORD dwTimeOver, BOOL bPumpMsg)
{
	DWORD dwStartTick = GetTickCount();
	int nRet = SUCCESS;

	BOOL bLoopWh = TRUE;
	while (bLoopWh)
	{
		if (IsMotionDone(nAxisX) && IsMotionDone(nAxisY))
		{
			nRet = SUCCESS;
			break;
		}

		if (GetAxisState(nAxisX) == MPIStateERROR || GetAxisState(nAxisY) == MPIStateERROR)
		{
			if (!IsEmergency())
			{
				nRet = OPERATE_INTERLOCK;
				break;
			}
		}

		if ((GetTickCount() - dwStartTick) >= dwTimeOver)
		{
			nRet = WAIT_TIME_OVER;
			break;
		}

		if (IsEmergency())
		{
			nRet = EMERGENCY_STOP;
			break;
		}

		Sleep(30);
	}

	return nRet;
}

#endif	// #ifdef USE_NMC