// LibMilBuf.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "LibMilBuf.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLibMilBuf

CLibMilBuf::CLibMilBuf(MIL_ID SystemId, int SizeBand, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute)
{
	m_MilImageChild = NULL;

	m_MilImage = M_NULL;
	MbufAllocColor (SystemId, SizeBand, SizeX, SizeY, Type, Attribute, &m_MilImage);
	Clear();
}

CLibMilBuf::CLibMilBuf(MIL_ID SystemId, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute)
{
	m_MilImageChild = NULL;

	m_MilImage = M_NULL;
	MbufAlloc2d(SystemId, SizeX, SizeY, Type, Attribute, &m_MilImage);
	Clear();
}

CLibMilBuf::~CLibMilBuf()
{
 	if(m_MilImageChild)
 	{
 		MbufFree(m_MilImageChild);
 		m_MilImageChild = M_NULL;
 	}

	if(m_MilImage)
	{
		MbufFree(m_MilImage);
		m_MilImage = M_NULL;
	}			
}


BEGIN_MESSAGE_MAP(CLibMilBuf, CWnd)
	//{{AFX_MSG_MAP(CLibMilBuf)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLibMilBuf message handlers

void CLibMilBuf::Clear()
{
	if(m_MilImage)
		MbufClear(m_MilImage, M_NULL);
}

void CLibMilBuf::ChildBuffer2d(long OffX, long OffY, long SizeX, long SizeY)
{
 	MbufChild2d(m_MilImage, OffX, OffY, SizeX, SizeY, &m_MilImageChild);
}

void CLibMilBuf::BufferLoad(TCHAR* FileName)
{
	MbufLoad(FileName, m_MilImage);
}
