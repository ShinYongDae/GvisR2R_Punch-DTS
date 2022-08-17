// Smac.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "Smac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc *pDoc;
extern CGvisR2R_PunchView *pView;

/////////////////////////////////////////////////////////////////////////////
// CSmac

CSmac::CSmac(CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	m_bRs232Run = FALSE;
	CWnd::Create(NULL,NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, CRect(0,0,0,0), pParent, 0);

	m_bTimerStop = FALSE;
	m_bReturnFirstSmacCmdEnd = FALSE;
	m_bReturnSecondSmacCmdEnd = FALSE;
	m_bRunTimerCheckFirstSmacEnd = FALSE;
	m_bRunTimerCheckSecondSmacEnd = FALSE;

	m_nCh = 0;
	int i;
	for(i=0; i<MAX_VOICE_COIL; i++)
	{
		m_bCh[i] = FALSE;
	}
}

CSmac::~CSmac()
{
	Close();
}


BEGIN_MESSAGE_MAP(CSmac, CWnd)
	//{{AFX_MSG_MAP(CSmac)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE_RS232, OnReceiveRs232)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSmac message handlers

void CSmac::Init()
{
	m_Rs232.SetHwnd(this->GetSafeHwnd());
	m_Rs232.m_nPort = (BYTE)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sPort);
	m_Rs232.m_dwBaudRate = (DWORD)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sBaudRate);
	m_Rs232.m_bCR = pDoc->WorkingInfo.VoiceCoil[m_nCh].bCr;
	m_Rs232.m_bLF = pDoc->WorkingInfo.VoiceCoil[m_nCh].bLf;
	m_bRs232Run = m_Rs232.OpenComPort();

#ifdef USE_SMAC
	if(!m_bRs232Run)
	{
		CString sMsg;
		sMsg.Format(_T("보이스코일이 초기화되지 않았습니다.(COM%d)"), m_Rs232.m_nPort);
//		pView->MsgBox(_T("보이스코일이 초기화되지 않았습니다.");
		AfxMessageBox(sMsg);
	}
#endif
	m_sRcvRs232 = _T("");
}

void CSmac::Close()
{
	if(m_bRs232Run)
	{
		m_Rs232.DestroyRs232();
		m_bRs232Run = FALSE;
	}
}

CString CSmac::Rcv()
{
	CString sRcv = m_sRcvRs232;
	m_Rs232.ClearReceive();
	m_sRcvRs232 = _T("");

	return sRcv;
}

BOOL CSmac::Send(CString str)
{
	int nLen = str.GetLength();
	
	char* pRtn = NULL;
	char* cSend = new char[nLen+1];
	strcpy(cSend, pRtn=StringToChar(str));
	BOOL bRtn = m_Rs232.WriteRs232Block(cSend, nLen);
	delete cSend;

	if(pRtn)
		delete pRtn;

	return TRUE;
}

void CSmac::SetCh(int nCh)
{
	m_nCh = nCh;
}

void CSmac::Reset(int nCh)
{

}

void CSmac::SearchHomeSmac(int nCamNum)
{
#ifdef USE_SMAC
//	double dPos;
	CString strSend;
	strSend.Format(_T("MS220")); // Homming
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		Wait(30);
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
// 		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("HOME_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Homming First Smac."));
	
//		dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		Wait(30);
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
// 		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("HOME_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Homming Second Smac."));
//		dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
	}
	
	//ClearReceive();
#endif
}

BOOL CSmac::IsDoneSearchHomeSmac0()
{
	int nCamNum = VOICE_COIL_FIRST_CAM;

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!WaitSmacCmdEnd0(_T("OK")))
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!WaitSmacCmdEnd1(_T("OK")))
			return FALSE;
	}

	ClearReceive();
	return TRUE;
}

BOOL CSmac::IsDoneSearchHomeSmac1()
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!WaitSmacCmdEnd0(_T("OK")))
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!WaitSmacCmdEnd1(_T("OK")))
			return FALSE;
	}
	
	ClearReceive();
	return TRUE;
}

void CSmac::SearchHomeSmac0()
{
	int nCamNum = VOICE_COIL_FIRST_CAM;

#ifdef USE_SMAC
	//	double dPos;
	CString strSend;
	strSend.Format(_T("MS220")); // Homming
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		Wait(30);
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd0(_T("OK"));
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		Wait(30);
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd1(_T("OK"));
	}
	
//	ClearReceive();
#endif
}

void CSmac::SearchHomeSmac1()
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
#ifdef USE_SMAC
	//	double dPos;
	CString strSend;
	strSend.Format(_T("MS220")); // Homming
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		Wait(30);
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd0(_T("OK"));
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		Wait(30);
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd1(_T("OK"));
	}
	
//	ClearReceive();
#endif
}

double CSmac::GetSmacPosition(int nCamNum)
{
	double dGetSmacPos=0.0;
	int nPos1, nPos2, nGetSmacPos;
	CString strSend, strVal;
	strSend.Format(_T("MS200")); // Get Position.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);

//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T(":GET_POS_OK;")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail GetPosition First Smac."));
//			dGetSmacPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
			return 0.0;
		}

		strVal = m_strReceiveVoiceCoilFirstCam;

		nPos1 = strVal.Find(_T("OK"));
		nPos2 = strVal.GetLength();
		strVal = strVal.Left(nPos1-3);

		nPos1 = strVal.ReverseFind(':');
		nPos2 = strVal.GetLength();
		strVal.Delete(0, nPos1+3);

		nGetSmacPos = _tstoi(strVal);
		dGetSmacPos = double(nGetSmacPos * 0.005 ); // 엔코더 해상도 : [5um / 1pluse]
		strVal.Format(_T("%.3f"), dGetSmacPos);
		m_strReceiveVoiceCoilFirstCam = _T("");

		//SetTimer(TIMER_FIRST_SMAC_GET_POS, 10, NULL);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);

//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T(":GET_POS_OK;")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail GetPosition Second Smac."));
//			dGetSmacPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
			return 0.0;
		}

		strVal = m_strReceiveVoiceCoilSecondCam;

		nPos1 = strVal.Find(_T("OK"));
		nPos2 = strVal.GetLength();
		strVal = strVal.Left(nPos1-3);

		nPos1 = strVal.ReverseFind(':');
		nPos2 = strVal.GetLength();
		strVal.Delete(0, nPos1+3);

		nGetSmacPos = _tstoi(strVal);
		dGetSmacPos = double(nGetSmacPos * 0.005 ); // 엔코더 해상도 : [5um / 1pluse]
		strVal.Format(_T("%.3f"), dGetSmacPos);
		m_strReceiveVoiceCoilSecondCam = _T("");
		
		//SetTimer(TIMER_SECOND_SMAC_GET_POS, 10, NULL);
	}

	return dGetSmacPos;
}

void CSmac::MoveSmacShiftPos(int nCamNum)
{
//	double dPos;
	CString strSend;
	strSend.Format(_T("MS40")); // Move Shift Position.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("START_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Shift Position - First Smac."));
	
//		dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
		Sleep(30);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("START_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Shift Position - Second Smac."));

//		dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
		Sleep(30);
	}
	
	//ClearReceive();
}

BOOL CSmac::IsDoneMoveSmacShiftPos0()
{
	int nCamNum = VOICE_COIL_FIRST_CAM;

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!WaitSmacCmdEnd0(_T("OK")))
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!WaitSmacCmdEnd1(_T("OK")))
			return FALSE;
	}
	ClearReceive();
	return TRUE;
}

BOOL CSmac::IsDoneMoveSmacShiftPos1()
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!WaitSmacCmdEnd0(_T("OK")))
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!WaitSmacCmdEnd1(_T("OK")))
			return FALSE;
	}
	ClearReceive();
	return TRUE;
}
	
void CSmac::MoveSmacShiftPos0()
{
	int nCamNum = VOICE_COIL_FIRST_CAM;

	CString strSend;
	strSend.Format(_T("MS40")); // Move Shift Position.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd0(_T("OK"));
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd1(_T("OK"));
	}
	
//	ClearReceive();
}

void CSmac::MoveSmacShiftPos1()
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
	CString strSend;
	strSend.Format(_T("MS40")); // Move Shift Position.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd0(_T("OK"));
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetWaitSmacCmdEnd1(_T("OK"));
	}
	
//	ClearReceive();
}

void CSmac::MoveSmacFinalPos(int nCamNum)
{
	double dPos;
	CString strSend;
	strSend.Format(_T("MS50")); // Move Final Position.
	ClearReceive();
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("END_POS_OK"))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Final Position - First Smac."));
	
		dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("END_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Final Position - Second Smac."));

		dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
	}

	
	//ClearReceive();

}

void CSmac::MoveSmacMeasPos(int nCamNum)
{
	double dPos;
	CString strSend;
	strSend.Format(_T("MS70")); // Move Measuring Position with Prob.
	ClearReceive();
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("END_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Final Position - First Smac."));
		
		dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("END_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Final Position - Second Smac."));
		
		dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
	}
	
	
	//ClearReceive();
	
}

void CSmac::ResetSmac(int nCamNum) 
{
	CString strSend;
	strSend.Format(_T("MS110")); // Reset.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(150);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(150);
	}

	//ClearReceive();
}

CString CSmac::GetSmacStatus(int nCamNum)
{
	double dGetSmacPos=0.0;
	CString strSend, strVal;
	strSend.Format(_T("MS20")); // Get Status.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");

		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);

//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("_OK;")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail GetStatus First Smac."));
//			dGetSmacPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
			return _T("NG");
		}

		if(m_strReceiveVoiceCoilFirstCam.Find(_T(":STATUS_OK;")) != -1)
			strVal = _T("Good");		

		if(m_strReceiveVoiceCoilFirstCam.Find(_T(":SERVO_OFF_OK;")) != -1)
			strVal = _T("Motor Off");

		if(m_strReceiveVoiceCoilFirstCam.Find(_T(":SERVO_ERROR_OK;")) != -1)
			strVal = _T("Error");

//		dGetSmacPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
		m_strReceiveVoiceCoilFirstCam = _T("");
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");

		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);

//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("_OK;"))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail GetStatus Second Smac."));
//			dGetSmacPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
			return _T("NG");
		}

		if(m_strReceiveVoiceCoilSecondCam.Find(_T(":STATUS_OK;")) != -1)
			strVal = _T("Good");		

		if(m_strReceiveVoiceCoilSecondCam.Find(_T(":SERVO_OFF_OK;")) != -1)
			strVal = _T("Motor Off");

		if(m_strReceiveVoiceCoilSecondCam.Find(_T(":SERVO_ERROR_OK;")) != -1)
			strVal = _T("Error");

//		dGetSmacPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
		m_strReceiveVoiceCoilSecondCam = _T("");
	}

	
	//ClearReceive();

	return strVal;
}




double CSmac::GetSmacMeasureOfSurface(int nCamNum)
{
	CString strVal;
	double dPos, dFinalPos;
	CString strSend;
	int nPos1, nPos2;

	strSend.Format(_T("MS70")); // Measure Surface.
	ClearReceive();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T(":CHECK_END_POS_OK;")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail Measuring Surface First Smac."));
			dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
			return 0.0;
		}

		strVal = m_strReceiveVoiceCoilFirstCam;

		nPos1 = strVal.Find(_T("OK"));
		nPos2 = strVal.GetLength();
		strVal = strVal.Left(nPos1-3);

		nPos1 = strVal.ReverseFind(':');
		nPos2 = strVal.GetLength();
		strVal.Delete(0, nPos1+3);

		dFinalPos = _tstoi(strVal);

		m_strReceiveVoiceCoilFirstCam = _T("");
		Sleep(500);
		SearchHomeSmac(VOICE_COIL_FIRST_CAM);	
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T(":CHECK_END_POS_OK;")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
		{
			AfxMessageBox(_T("Fail Measuring Surface Second Smac."));
			dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
			return dPos;
		}

		strVal = m_strReceiveVoiceCoilSecondCam;

		nPos1 = strVal.Find(_T("OK"));
		nPos2 = strVal.GetLength();
		strVal = strVal.Left(nPos1-3);

		nPos1 = strVal.ReverseFind(':');
		nPos2 = strVal.GetLength();
		strVal.Delete(0, nPos1+3);

		dFinalPos = _tstoi(strVal);

		m_strReceiveVoiceCoilSecondCam = _T("");
		Sleep(30);
		SearchHomeSmac(VOICE_COIL_SECOND_CAM);	
	}
		
	return (double(dFinalPos * 0.005));	
}

void CSmac::SendStringToFirstCamVoiceCoil(CString strSend)
{
	CString strTemp = strSend +_T("\n");
	TRACE(strTemp);
	int strSendLength;
	
	strSendLength=strSend.GetLength();

	LPWSTR pchData = new WCHAR[strSendLength];
	VARIANT vrOutp;
	char* pRtn = NULL;
	MultiByteToWideChar(CP_ACP, 0, pRtn=StringToChar(strSend), strSendLength, pchData, strSendLength);
	vrOutp.vt = VT_BSTR;
	vrOutp.bstrVal = ::SysAllocStringLen(pchData, strSendLength);

	delete pchData;

	if(pRtn)
		delete pRtn;
	
	Send(strSend);
	::SysFreeString(vrOutp.bstrVal);
}

void CSmac::SendStringToSecondCamVoiceCoil(CString strSend)
{
	CString strTemp = strSend +_T("\n");
	TRACE(strTemp);
	int strSendLength;
	
	strSendLength=strSend.GetLength();

	LPWSTR pchData = new WCHAR[strSendLength];
	VARIANT vrOutp;
	char* pRtn = NULL;
	MultiByteToWideChar(CP_ACP, 0, pRtn=StringToChar(strSend), strSendLength, pchData, strSendLength);
	vrOutp.vt = VT_BSTR;
	vrOutp.bstrVal = ::SysAllocStringLen(pchData, strSendLength);

	delete pchData;
	if(pRtn)
		delete pRtn;
	
	Send(strTemp);
	::SysFreeString(vrOutp.bstrVal);
}

BOOL CSmac::SetWaitSmacCmdEnd0(CString strEndCmd)
{
	int nCamNum = VOICE_COIL_FIRST_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_bReturnFirstSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckFirstSmacEnd)
		{
			m_bRunTimerCheckFirstSmacEnd = TRUE;
			m_strFirstSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_FIRST_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_bReturnSecondSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckSecondSmacEnd)
		{
			m_bRunTimerCheckSecondSmacEnd = TRUE;
			m_strSecondSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_SECOND_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}

	return TRUE;
}

BOOL CSmac::SetWaitSmacCmdEnd1(CString strEndCmd)
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_bReturnFirstSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckFirstSmacEnd)
		{
			m_bRunTimerCheckFirstSmacEnd = TRUE;
			m_strFirstSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_FIRST_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_bReturnSecondSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckSecondSmacEnd)
		{
			m_bRunTimerCheckSecondSmacEnd = TRUE;
			m_strSecondSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_SECOND_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}

	return TRUE;
}

BOOL CSmac::WaitSmacCmdEnd0(CString strEndCmd)
{
	int nCamNum = VOICE_COIL_FIRST_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!m_bReturnFirstSmacCmdEnd)
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!m_bReturnSecondSmacCmdEnd)
			return FALSE;
	}

	m_strFirstSmacEndCmd = _T("");
	return TRUE;
}

BOOL CSmac::WaitSmacCmdEnd1(CString strEndCmd)
{
	int nCamNum = VOICE_COIL_SECOND_CAM;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(!m_bReturnFirstSmacCmdEnd)
			return FALSE;
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(!m_bReturnSecondSmacCmdEnd)
			return FALSE;
	}

	m_strSecondSmacEndCmd = _T("");
	return TRUE;
}
	
BOOL CSmac::WaitSmacCmdEnd(int nCamNum, CString strEndCmd)
{
	
	DWORD CurTimer, StartTimer;
	MSG message;

	StartTimer=GetTickCount();
	CurTimer=GetTickCount();

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_bReturnFirstSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckFirstSmacEnd)
		{
			m_bRunTimerCheckFirstSmacEnd = TRUE;
			m_strFirstSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_FIRST_SMAC_CHECK_END_CMD, 100, NULL);
		}
		
		while(DELAY_TIME_ONE_MINUTE > int(CurTimer-StartTimer) && !m_bReturnFirstSmacCmdEnd)
		{
			CurTimer=GetTickCount();
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				if(!IsDialogMessage(&message))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		m_strFirstSmacEndCmd = _T("");

		if(DELAY_TIME_ONE_MINUTE <= int(CurTimer-StartTimer))
		{
			if(m_bReturnFirstSmacCmdEnd)
			{
				int sydTest = 0;
			}
			else
			{
				AfxMessageBox(_T("Error, Wait First Smac Cmd End."));
				return FALSE;
			}
		}
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_bReturnSecondSmacCmdEnd = FALSE;
		if(!m_bRunTimerCheckSecondSmacEnd)
		{
			m_bRunTimerCheckSecondSmacEnd = TRUE;
			m_strSecondSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_SECOND_SMAC_CHECK_END_CMD, 100, NULL);
		}
		
		while(DELAY_TIME_ONE_MINUTE > int(CurTimer-StartTimer) && !m_bReturnSecondSmacCmdEnd)
		{
			CurTimer=GetTickCount();
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				if(!IsDialogMessage(&message))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		m_strSecondSmacEndCmd = _T("");

		if(DELAY_TIME_ONE_MINUTE <= int(CurTimer-StartTimer))
		{
			if(m_bReturnSecondSmacCmdEnd)
			{
				int sydTest = 0;
			}
			else
			{
				AfxMessageBox(_T("Error, Wait Second Smac Cmd End."));
				return FALSE;
			}
		}
	}

	return TRUE;
}


void CSmac::Wait(int imSecond)
{
	DWORD CurTimer, StartTimer;
	MSG message;
	StartTimer=GetTickCount();
	CurTimer=GetTickCount();
	while(imSecond > int(CurTimer-StartTimer))
	{
		CurTimer=GetTickCount();
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
}


int CSmac::CheckSmacErrorCode(int nCamNum)
{
	int nLength, nPos, nErrCode=0;
	CString strVal, strTemp; 

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		strVal = m_strReceiveVoiceCoilFirstCam;
		
		if(strVal.Find(_T("?")) != -1)
		{
			nPos=strVal.Find(_T("?"));
			nLength = strVal.Delete(0, nPos+1);
			nErrCode = _tstoi(strVal);
			strTemp.Format(_T("Error-%d"), nErrCode);	
		}
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		strVal = m_strReceiveVoiceCoilSecondCam;
		
		if(nPos=strVal.Find(_T("?")) != -1)
		{
			nLength = strVal.Delete(0, nPos+1);
			nErrCode = _tstoi(strVal);
			strTemp.Format(_T("Error-%d"), nErrCode);	
		}
	}

	return nErrCode; // 0 : Not exist error code...
}

CString CSmac::DisplaySmacErrorList(int nErrCode)
{
	CString strError, strVal;

	switch(nErrCode)
	{
	case 0:
		strError = _T("NONE");
		return strError;
	case 1:
		strError = _T("ARGUMENT ERROR");
		break;
	case 2:
		strError = _T("INVALID COMMAND");
		break;
	case 3:
		strError = _T("INVALID MACRO COMMAND");
		break;
	case 4:
		strError = _T("MACRO ARGUMENT ERROR");
		break;
	case 5:
		strError = _T("MACRO NOT DEFINED");
		break;
	case 6:
		strError = _T("MACRO OUT OF RANGE");
		break;
	case 7:
		strError = _T("OUT OF MACRO SPACE");
		break;
	case 8:
		strError = _T("CAN'T DEFINE MACRO IN A MACRO");
		break;
	case 9:
		strError = _T("CAN'T DEFINE MACRO WHILE SERVO ENABLED");
		break;
	case 10:
		strError = _T("MACRO JUMP ERROR");
		break;
	case 11:
		strError = _T("OUT OF MACRO STACK SPACE");
		break;
	case 12:
		strError = _T("MACRO MUST BE FIRST COMMAND");
		break;
	case 13:
		strError = _T("STRING ERROR");
		break;
	case 14:
		strError = _T("MACRO STRING ERROR");
		break;
	case 15:
		strError = _T("SYNTAX ERROR");
		break;
	case 16:
		strError = _T("MACRO SYNTAX ERROR");
		break;
	case 17:
		strError = _T("AXIS RANGE ERROR");
		break;
	case 18:
		strError = _T("INTERRUPT MACRO NOT DEFINED");
		break;
	case 19:
		strError = _T("INTERRUPT MACRO STACK ERROR");
		break;
	case 20:
		strError = _T("MACRO STACK OVERFLOW");
		break;
	case 21:
		strError = _T("MACRO STACK UNDERFLOW");
		break;
	default:
		strError=_T("");
		return strError;
	}

	strVal.Format(_T("SMAC Error code is %s ."), strError);
	AfxMessageBox(strVal);

	return strError;
}

void CSmac::CalcAveSmacHeight(int nCamNum)
{
	CString strVal;
	int i;
	double dAverDist=0.0;

	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		for (i=0; i<4; i++)
			dAverDist += _tstof(pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[i]);

		strVal.Format(_T("%.3f"), dAverDist/4.0);
		pDoc->WorkingInfo.Marking[VOICE_COIL_FIRST_CAM].sAverDist = strVal;
//		pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacAverageDist = strVal;
		::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), strVal, PATH_WORKING_INFO);
//		::WritePrivateProfileString(_T("VoiceCoil0"), _T("Smac_Average_Distance"), strVal, PATH_WORKING_INFO);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		for (i=0; i<4; i++)
			dAverDist += _tstof(pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacHeightAtPos[i]);

		strVal.Format(_T("%.3f"), dAverDist/4.0);
		pDoc->WorkingInfo.Marking[VOICE_COIL_SECOND_CAM].sAverDist = strVal;
//		pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_SECOND_CAM].sSmacAverageDist = strVal;
		::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), strVal, PATH_WORKING_INFO);
//		::WritePrivateProfileString(_T("VoiceCoil1"), _T("Smac_Average_Distance"), strVal, PATH_WORKING_INFO);

	}
}

LRESULT CSmac::OnReceiveRs232(WPARAM wP, LPARAM lP)
{
	CString str;
	if(m_sRcvRs232.IsEmpty())
		m_sRcvRs232.Format(_T("%s"),m_Rs232.m_cRxBuffer);
	else
	{
		str.Format(_T("%s"),m_Rs232.m_cRxBuffer);
		m_sRcvRs232 += str;

		if (!m_nCh)
			m_strReceiveVoiceCoilFirstCam = m_sRcvRs232;
		else
			m_strReceiveVoiceCoilSecondCam = m_sRcvRs232;

	}

	m_Rs232.ClearReceive();

	return 0L;
}

void CSmac::ClearReceive()
{
	m_sRcvRs232 = _T("");
}

BOOL CSmac::SetCmdEndChk(int nCamNum, CString strEndCmd)
{
	
	DWORD CurTimer, StartTimer;
	MSG message;
	
	StartTimer=GetTickCount();
	CurTimer=GetTickCount();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_bReturnFirstSmacCmdEnd = FALSE; // Chk 
		if(!m_bRunTimerCheckFirstSmacEnd)
		{
			m_bRunTimerCheckFirstSmacEnd = TRUE;
			m_strFirstSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_FIRST_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_bReturnSecondSmacCmdEnd = FALSE; // Chk
		if(!m_bRunTimerCheckSecondSmacEnd)
		{
			m_bRunTimerCheckSecondSmacEnd = TRUE;
			m_strSecondSmacEndCmd = strEndCmd;
			if(!m_bTimerStop)
				SetTimer(TIMER_SECOND_SMAC_CHECK_END_CMD, 100, NULL);
		}
	}
	
	return TRUE;
}


void CSmac::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int nErrCode;

	switch (nIDEvent)
	{
		case TIMER_FIRST_SMAC_CHECK_END_CMD:
			if(m_strReceiveVoiceCoilFirstCam.Find(m_strFirstSmacEndCmd) != -1)
			{
				if(m_bRunTimerCheckFirstSmacEnd)
				{
					KillTimer(TIMER_FIRST_SMAC_CHECK_END_CMD);
					m_bRunTimerCheckFirstSmacEnd = FALSE;
				}
				m_bReturnFirstSmacCmdEnd = TRUE;
				Sleep(10);
			}
			break;

		case TIMER_SECOND_SMAC_CHECK_END_CMD:
			if(m_strReceiveVoiceCoilSecondCam.Find(m_strSecondSmacEndCmd) != -1)
			{
				if(m_bRunTimerCheckSecondSmacEnd)
				{
					KillTimer(TIMER_SECOND_SMAC_CHECK_END_CMD);
					m_bRunTimerCheckSecondSmacEnd = FALSE;
				}
				m_bReturnSecondSmacCmdEnd = TRUE;
				Sleep(10);
			}
			break;

		case TIMER_FIRST_SMAC_CHECK_ERROR_CODE:    
			KillTimer(TIMER_FIRST_SMAC_CHECK_ERROR_CODE);
			nErrCode = CheckSmacErrorCode(VOICE_COIL_FIRST_CAM);
			if(nErrCode) // 0: No Error...
				DisplaySmacErrorList(nErrCode);
			break;

		case TIMER_SECOND_SMAC_CHECK_ERROR_CODE: 
			KillTimer(TIMER_SECOND_SMAC_CHECK_ERROR_CODE);
			nErrCode = CheckSmacErrorCode(VOICE_COIL_SECOND_CAM);
			if(nErrCode) // 0: No Error...
				DisplaySmacErrorList(nErrCode);
			break;

		default:    //MPIMessageFATAL_ERROR;				

			break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CSmac::SetMarking(int nCamNum)
{
	//	double dPos;
	CString strSend;
	strSend.Format(_T("MS60")); // Punching
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("PUNCHING_OK")))
			//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Punching First Smac."));
		Sleep(30);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("PUNCHING_OK")))
			//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Punching Second Smac."));
		Sleep(30);
	}
	
	
	//ClearReceive();
}

void CSmac::SetMarkShiftData(int nCamNum)
{
	double dShiftPos, dShiftVel, dShiftAcc;
	CString strVal;
	
	dShiftPos = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitPos) * 200;
	dShiftVel = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitVel) * 2621;
	dShiftAcc = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitAcc) * 0.5;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		strVal.Format(_T("AL%d,AR14"), int(dShiftPos));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR30"), int(dShiftVel));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR32"), int(dShiftAcc));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_FIRST_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		strVal.Format(_T("AL%d,AR14"), int(dShiftPos));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR30"), int(dShiftVel));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR32"), int(dShiftAcc));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_SECOND_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
	
}

void CSmac::SetMarkFinalData(int nCamNum)
{
	double dFinalPos, dFinalVel, dFinalAcc, dFinalTorque, dFinalTorqueOffset;
	CString strVal;
	
	dFinalPos = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingPos) * 200;
	dFinalVel = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingVel) * 2621;
	dFinalAcc = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingAcc) * 0.5;
	//	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingToq) * 16.9;
	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingToq)/3.19;
	dFinalTorqueOffset = 8000 + (_tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingToqOffset))*18.0;
	
	if(dFinalTorque > 1024.0)
		dFinalTorque = 1024.0;
	if(dFinalTorqueOffset > 32767.0)
		dFinalTorqueOffset = 32767.0;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		strVal.Format(_T("AL%d,AR16"), int(dFinalPos));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR20"), int(dFinalVel));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR22"), int(dFinalAcc));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR40"), int(dFinalTorque));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR48"), int(dFinalTorqueOffset));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_FIRST_SMAC_CHECK_ERROR_CODE, 100, NULL);
		
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		strVal.Format(_T("AL%d,AR16"), int(dFinalPos));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR20"), int(dFinalVel));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR22"), int(dFinalAcc));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR40"), int(dFinalTorque));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR48"), int(dFinalTorqueOffset));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);

		if(!m_bTimerStop)
			SetTimer(TIMER_SECOND_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
}


void CSmac::SetMark(int nCamNum)
{
	CString strSend;
	strSend.Format(_T("MS60")); // Punching
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetCmdEndChk(nCamNum, _T("OK"));
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		SetCmdEndChk(nCamNum, _T("OK"));
	}
}

BOOL CSmac::IsDoneMark(int nCamNum)
{
	if(nCamNum == VOICE_COIL_FIRST_CAM)
		return m_bReturnFirstSmacCmdEnd;
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
		return m_bReturnSecondSmacCmdEnd;
	
	return FALSE;
}

// Elec Check =================================

void CSmac::SetProbShiftData(int nCamNum)
{
	double dShiftPos, dShiftVel, dShiftAcc;
	CString strVal;
	
	dShiftPos = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitPos) * 200;
	dShiftVel = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitVel) * 2621;
	dShiftAcc = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sWaitAcc) * 0.5;
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		strVal.Format(_T("AL%d,AR54"), int(dShiftPos));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR70"), int(dShiftVel));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR72"), int(dShiftAcc));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_FIRST_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		strVal.Format(_T("AL%d,AR54"), int(dShiftPos));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR70"), int(dShiftVel));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR72"), int(dShiftAcc));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_SECOND_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
	
}

void CSmac::SetProbFinalData(int nCamNum)
{
	double dFinalPos, dFinalVel, dFinalAcc, dFinalTorque;
	CString strVal;
	
	dFinalPos = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingPos) * 200;
	dFinalVel = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingVel) * 2621;
	dFinalAcc = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingAcc) * 0.5;
	//	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingToq) * 16.9;
	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[nCamNum].sMarkingToq)/3.19;
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		if(dFinalTorque > 32767.0)
			dFinalTorque = 32767.0;
		
		strVal.Format(_T("AL%d,AR56"), int(dFinalPos));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR70"), int(dFinalVel));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR72"), int(dFinalAcc));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR80"), int(dFinalTorque));
		SendStringToFirstCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_FIRST_SMAC_CHECK_ERROR_CODE, 100, NULL);
		
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		if(dFinalTorque > 32767.0)
			dFinalTorque = 32767.0;
		
		strVal.Format(_T("AL%d,AR56"), int(dFinalPos));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR60"), int(dFinalVel));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR62"), int(dFinalAcc));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		strVal.Format(_T("AL%d,AR60"), int(dFinalTorque));
		SendStringToSecondCamVoiceCoil(strVal);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		
		if(!m_bTimerStop)
			SetTimer(TIMER_SECOND_SMAC_CHECK_ERROR_CODE, 100, NULL);
	}
}



void CSmac::SetProbing(int nCamNum)
{
	//	double dPos;
	CString strSend;
	strSend.Format(_T("MS90"));
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Punching First Smac."));
		Sleep(30);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Punching Second Smac."));
		Sleep(30);
	}
	
	
	//ClearReceive();
}

BOOL CSmac::IsDoneProb(int nCamNum)
{
	if(nCamNum == VOICE_COIL_FIRST_CAM)
		return m_bReturnFirstSmacCmdEnd;
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
		return m_bReturnSecondSmacCmdEnd;
	
	return FALSE;
}

void CSmac::MoveProbFinalPos(int nCamNum)
{
	//	double dPos;
	CString strSend;
	strSend.Format(_T("MS100")); // Move final Position.
	ClearReceive();
	
	if(nCamNum == VOICE_COIL_FIRST_CAM)
	{
		m_strReceiveVoiceCoilFirstCam = _T("");
		SendStringToFirstCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToFirstCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		//		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("START_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_FIRST_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Shift Position - First Smac."));
		
		//		dPos = GetSmacPosition(VOICE_COIL_FIRST_CAM);
		Sleep(30);
	}
	else if(nCamNum == VOICE_COIL_SECOND_CAM)
	{
		m_strReceiveVoiceCoilSecondCam = _T("");
		SendStringToSecondCamVoiceCoil(strSend);
		Sleep(30);
		SendStringToSecondCamVoiceCoil(_T("\r\n"));
		Sleep(30);
		//		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("START_POS_OK")))
		if(!WaitSmacCmdEnd(VOICE_COIL_SECOND_CAM, _T("OK")))
			AfxMessageBox(_T("Fail Moving Shift Position - Second Smac."));
		
		//		dPos = GetSmacPosition(VOICE_COIL_SECOND_CAM);
		Sleep(30);
	}
	
	
	//ClearReceive();	
}

