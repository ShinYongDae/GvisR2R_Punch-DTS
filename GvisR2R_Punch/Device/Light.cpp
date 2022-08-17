// Light.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "Light.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CLight

CLight::CLight(CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	m_bRs232Run = FALSE;
	CWnd::Create(NULL,NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, CRect(0,0,0,0), pParent, 0);

	m_Rs232.SetHwnd(this->GetSafeHwnd());
	m_Rs232.m_nPort = (BYTE)_tstoi(pDoc->WorkingInfo.Light.sPort); // _ttoi()
	m_Rs232.m_dwBaudRate = (DWORD)_tstoi(pDoc->WorkingInfo.Light.sBaudRate);
	m_Rs232.m_bCR = pDoc->WorkingInfo.Light.bCr;
	m_Rs232.m_bLF = pDoc->WorkingInfo.Light.bLf;
	m_bRs232Run = m_Rs232.OpenComPort();

#ifdef USE_LIGHT
	if(!m_bRs232Run)
	{
		pView->MsgBox(_T("조명 제어장치가 초기화되지 않았습니다."));
// 		AfxMessageBox(_T("조명 제어장치가 초기화되지 않았습니다."));
	}
#endif
	m_sRcvRs232 = _T("");

	int i;
	for(i=0; i<MAX_LIGHT_CH; i++)
	{
		m_bCh[i] = FALSE;
		m_nCh[i] = 0;
	}
}

CLight::~CLight()
{
	Close();
}


BEGIN_MESSAGE_MAP(CLight, CWnd)
	//{{AFX_MSG_MAP(CLight)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE_RS232, OnReceiveRs232)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLight message handlers
void CLight::Init()
{
	CString str;
	str.Format(_T("setonex ffffffff%c"), 0x0D);
	Send(str);
}

void CLight::Close()
{
	if(m_bRs232Run)
	{
		m_Rs232.DestroyRs232();
		m_bRs232Run = FALSE;
	}
}

LRESULT CLight::OnReceiveRs232(WPARAM wP, LPARAM lP)
{
// 	if(m_sRcvRs232.IsEmpty())
// 		m_sRcvRs232.Format(_T("%s"),m_Rs232.m_cRxBuffer);
// 	else
// 		m_sRcvRs232.Format(_T("%s%s"), m_sRcvRs232, m_Rs232.m_cRxBuffer);
// 
// 	//m_Rs232.ClearReceive();
	return 0L;
}

CString CLight::Rcv()
{
	CString sRcv = m_sRcvRs232;
	m_Rs232.ClearReceive();
	m_sRcvRs232 = _T("");

	return sRcv;
}

BOOL CLight::Send(CString str)
{
	int nLen = str.GetLength();
	
	//char* cSend = new char[nLen+1];
	//strcpy(cSend, str);
	char* pRtn = NULL;
	TCHAR* cSend = new TCHAR[nLen + 1];
	wsprintf(cSend, TEXT("%s"), str);	
	BOOL bRtn = m_Rs232.WriteRs232Block(pRtn = TCHARToChar(cSend), nLen); if (pRtn) delete pRtn; pRtn = NULL;
	delete cSend;

	return TRUE;
}

BOOL CLight::Set(int nCh, int nVal)
{
	if(nVal < 0)
		nVal = m_nCh[nCh];
	else
	{
		if(nVal > 255)
			nVal = 255;
		m_nCh[nCh] = nVal;
	}

	if(!nVal)
		m_bCh[nCh] = FALSE;
	else
		m_bCh[nCh] = TRUE;

	CString str;
	str.Format(_T("setbrightness %d %d"),nCh, nVal);
	Send(str);

	return TRUE;
}

int CLight::Get(int nCh)
{
	return m_nCh[nCh];
}

void CLight::Reset(int nCh)
{
	CString str;
	str.Format(_T("setbrightness %d %d"),nCh, 0);
	Send(str);
	m_bCh[nCh] = FALSE;
}