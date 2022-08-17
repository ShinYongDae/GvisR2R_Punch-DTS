// LibMilDraw.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "LibMilDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibMilDraw

CLibMilDraw::CLibMilDraw(MIL_ID SystemId, CLibMilDisp* MLibDisp)
{
	m_MilBuffer = MLibDisp->m_MilDisplayOverlay;
	m_MilGraphicContextID = M_NULL;
	m_lFontName = M_FONT_DEFAULT_SMALL;
	m_dFontScaleX = 1.0;
	m_dFontScaleY = 1.0;


	if(!m_MilGraphicContextID)
		MgraAlloc(SystemId, &m_MilGraphicContextID);

	MgraFont(m_MilGraphicContextID, m_lFontName);
	MgraFontScale(m_MilGraphicContextID, m_dFontScaleX, m_dFontScaleY);
	MgraControl(m_MilGraphicContextID, M_BACKGROUND_MODE, M_OPAQUE);
}

CLibMilDraw::~CLibMilDraw()
{
	if(m_MilGraphicContextID)
	{
		MgraFree(m_MilGraphicContextID);
		m_MilGraphicContextID = M_NULL;
	}
}


BEGIN_MESSAGE_MAP(CLibMilDraw, CWnd)
	//{{AFX_MSG_MAP(CLibMilDraw)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLibMilDraw message handlers

void CLibMilDraw::SetDrawColor(long lForegroundColor)
{
	if(!m_MilGraphicContextID)	
		return;
	m_lForegroundColor = lForegroundColor;
	MgraColor(m_MilGraphicContextID, lForegroundColor);
}

void CLibMilDraw::SetDrawBackColor(long lBackgroundColor)
{
	if(!m_MilGraphicContextID)	
		return;
	m_lBackgroundColor = lBackgroundColor;
	MgraBackColor(m_MilGraphicContextID, lBackgroundColor);
}

void CLibMilDraw::SetTextFontScale(double dFontScaleX, double dFontScaleY)
{
	if(!m_MilGraphicContextID)	
		return;
	m_dFontScaleX = dFontScaleX;
	m_dFontScaleY = dFontScaleY;
	MgraFontScale(m_MilGraphicContextID, dFontScaleX, dFontScaleY);
}

void CLibMilDraw::SetTextFont(long FontName)
{
	m_lFontName = FontName;
	MgraFont(m_MilGraphicContextID, FontName);
}

BOOL CLibMilDraw::DrawText(long XStart, long YStart, MIL_TEXT_PTR String)
{
	if(!m_MilBuffer)
		return FALSE;

	DrawRectFill(m_lBackgroundColor, XStart, YStart, 480, YStart);
	MgraColor(m_MilGraphicContextID, m_lForegroundColor);
	MgraText(m_MilGraphicContextID, m_MilBuffer, XStart, YStart, String);
	return TRUE;
}

BOOL CLibMilDraw::DrawCross(double ForegroundColor, long XCenter, long YCenter, long XSize, long YSize, long XCenterSpace, long YCenterSpace)
{
	if(!m_MilBuffer)
		return FALSE;

	int nBufferSizeX=0, nBufferSizeY=0;
	nBufferSizeX = MbufInquire(m_MilBuffer, M_SIZE_X, M_NULL);	//MbufInquire(m_MilBuffer, M_SIZE_X, &nBufferSizeX);
	nBufferSizeY = MbufInquire(m_MilBuffer, M_SIZE_Y, M_NULL);	//MbufInquire(m_MilBuffer, M_SIZE_Y, &nBufferSizeY);
	
	if(XCenter < 0 || XCenter > (nBufferSizeX-1))
		XCenter = nBufferSizeX / 2;
	if(YCenter < 0 || YCenter > (nBufferSizeY-1))
		YCenter = nBufferSizeY / 2;

	MgraColor(m_MilGraphicContextID, ForegroundColor);
	MgraLine(m_MilGraphicContextID, m_MilBuffer, XCenter-(XSize/2), YCenter, XCenter-(XCenterSpace/2), YCenter);
	MgraLine(m_MilGraphicContextID, m_MilBuffer, XCenter+(XSize/2), YCenter, XCenter+(XCenterSpace/2), YCenter);
	MgraLine(m_MilGraphicContextID, m_MilBuffer, XCenter, YCenter-(YSize/2), XCenter, YCenter-(YCenterSpace/2));
	MgraLine(m_MilGraphicContextID, m_MilBuffer, XCenter, YCenter+(YSize/2), XCenter, YCenter+(YCenterSpace/2));

	return TRUE;
}

BOOL CLibMilDraw::DrawRectFill(double ForegroundColor, long XStart, long YStart, long XEnd, long YEnd)
{	
	if(!m_MilBuffer)
		return FALSE;

	MgraColor(m_MilGraphicContextID, ForegroundColor);
	MgraRectFill(m_MilGraphicContextID, m_MilBuffer, XStart, YStart, XEnd, YEnd);

	return TRUE;
}
