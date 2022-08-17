// DataMarking.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "DataMarking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataMarking

CDataMarking::CDataMarking(CWnd* pParent /*=NULL*/)
{
	m_pLayer = NULL;
	m_pDefPcs = NULL;
	m_pDefPos = NULL;
	m_pDefType = NULL;
	m_pCell = NULL;
	m_pImgSz = NULL;
	m_pImg = NULL;
	m_pMk = NULL;

	m_nTotRealDef = 0;
}

CDataMarking::~CDataMarking()
{
	FreeMem();
}

void CDataMarking::FreeMem()
{
	if (m_pLayer)
	{
		delete[] m_pLayer;
		m_pLayer = NULL;
	}

	if (m_pDefPcs)
	{
		delete[] m_pDefPcs;
		m_pDefPcs = NULL;
	}

	if (m_pDefPos)
	{
		delete[] m_pDefPos;
		m_pDefPos = NULL;
	}

	if(m_pMk)
	{
		delete[] m_pMk;
		m_pMk = NULL;
	}

	if(m_pImg)
	{
		delete[] m_pImg;
		m_pImg = NULL;
	}

	if(m_pImgSz)
	{
		delete[] m_pImgSz;
		m_pImgSz = NULL;
	}

	if(m_pCell)
	{
		delete[] m_pCell;
		m_pCell = NULL;
	}

	if (m_pDefType)
	{
		delete[] m_pDefType;
		m_pDefType = NULL;
	}
}


BEGIN_MESSAGE_MAP(CDataMarking, CWnd)
	//{{AFX_MSG_MAP(CDataMarking)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataMarking message handlers
void CDataMarking::Init(int nSerial, int nTot) // m_nIdx : From 0 to nTot.....
{
	int i = 0;

	if(nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.1"));
		return;
	}

	FreeMem();

	m_nSerial = nSerial;
	m_nTotDef = nTot;

	if(nTot > 0)
	{
		m_pLayer = new int[nTot];
		m_pDefPcs = new int[nTot];
		m_pDefPos = new CPoint[nTot];

		m_pDefType = new int[nTot];
		m_pCell = new int[nTot];
		m_pImgSz = new int[nTot];
		m_pImg = new int[nTot];
		m_pMk = new int[nTot];
	}

	for (i = 0; i < nTot; i++)
	{
		m_pLayer[i] = -1;
		m_pDefPcs[i] = -1;
		m_pDefPos[i] = CPoint(0,0);

		m_pDefType[i] = -1;
		m_pCell[i] = -1;
		m_pImgSz[i] = -1;
		m_pImg[i] = -1;
		m_pMk[i] = -1;
	}

	m_MkOffset.x = 0;
	m_MkOffset.y = 0;
}