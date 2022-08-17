// LibMilDisp.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "LibMilDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibMilDisp

CLibMilDisp::CLibMilDisp(MIL_ID SystemId)
{
	m_MilDisplay = M_NULL;
	MdispAlloc(SystemId, M_DEFAULT, _T("M_DEFAULT"), M_DEFAULT, &m_MilDisplay);

	m_MilDisplayOverlay = M_NULL;
	m_lOverlayColor = 0;
}

CLibMilDisp::~CLibMilDisp()
{
	if (m_MilDisplay)
	{
		//MdispSelect(m_MilDisplay, M_NULL);
		MdispFree(m_MilDisplay);
		m_MilDisplay = NULL;
	}
}


BEGIN_MESSAGE_MAP(CLibMilDisp, CWnd)
	//{{AFX_MSG_MAP(CLibMilDisp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLibMilDisp message handlers

BOOL CLibMilDisp::ClearOverlay(long pColor)
{
	if(m_MilDisplayOverlay)
	{
		MbufClear(m_MilDisplayOverlay, (double)pColor);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLibMilDisp::ClearOverlay()
{
	if(m_MilDisplayOverlay)
	{
		MbufClear(m_MilDisplayOverlay, (double)m_lOverlayColor);
		return TRUE;
	}
	else
		return FALSE;
}
