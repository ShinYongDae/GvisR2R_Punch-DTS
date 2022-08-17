// MyFileErrMap.cpp : implementation file
//

#include "stdafx.h"
//#include "GvisR2R.h"
#include "MyFileErrMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileErrMap

CMyFileErrMap::CMyFileErrMap(CWnd* pParent)
{
	m_pParent = pParent;
	m_pErrPos = NULL;
}

CMyFileErrMap::~CMyFileErrMap()
{
	if(m_pErrPos)
	{
		delete[] m_pErrPos;
		m_pErrPos = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyFileErrMap, CWnd)
	//{{AFX_MSG_MAP(CMyFileErrMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyFileErrMap message handlers

BOOL CMyFileErrMap::Open(CString sPath)
{
	// [Info]
	if(!LoadInfo(sPath))
		return FALSE;

	// [Point Error Data]
	if(m_pErrPos)
	{
		delete[] m_pErrPos;
		m_pErrPos = NULL;
	}

	int nTotErrPos = (m_nAXIS_X_TOTAL_POINT+1)*(m_nAXIS_Y_TOTAL_POINT+1);
	m_pErrPos = new ErrPos[nTotErrPos];

	int nRow=0, nCol=0, nIdx=0;
	for(nRow=0; nRow<m_nAXIS_Y_TOTAL_POINT+1; nRow++)
	{
		if(!nRow)
		{
			for(nCol=0; nCol<m_nAXIS_X_TOTAL_POINT+1; nCol++)
			{
				m_pErrPos[nIdx].nCol = nCol;
				m_pErrPos[nIdx].nRow = nRow;
				m_pErrPos[nIdx].dErrX = 0.0;
				m_pErrPos[nIdx].dErrY = 0.0;

				nIdx++;
			}
		}
		else
		{
			if(!LoadX(nRow, nIdx, sPath))
				return FALSE;
			if(!LoadY(nRow, nIdx, sPath))
				return FALSE;

			nIdx += (m_nAXIS_X_TOTAL_POINT+1);
		}
	}

	return TRUE;
}

BOOL CMyFileErrMap::LoadInfo(CString sPath)
{
	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *stopstring;

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("COMPENSATOR_DIMENSION"), NULL, szData, sizeof(szData), sPath))
		m_nCOMPENSATOR_DIMENSION = _ttoi(szData);
	else
	{
		m_nCOMPENSATOR_DIMENSION = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("COMPENSATOR_ID_X"), NULL, szData, sizeof(szData), sPath))
		m_nCOMPENSATOR_ID_X = _ttoi(szData);
	else
	{
		m_nCOMPENSATOR_ID_X = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("COMPENSATOR_ID_Y"), NULL, szData, sizeof(szData), sPath))
		m_nCOMPENSATOR_ID_Y = _ttoi(szData);
	else
	{
		m_nCOMPENSATOR_ID_Y = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_X_ID"), NULL, szData, sizeof(szData), sPath))
		m_nAXIS_X_ID = _ttoi(szData);
	else
	{
		m_nAXIS_X_ID = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_Y_ID"), NULL, szData, sizeof(szData), sPath))
		m_nAXIS_Y_ID = _ttoi(szData);
	else
	{
		m_nAXIS_Y_ID = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_X_RANGE_START"), NULL, szData, sizeof(szData), sPath))
		m_dAXIS_X_RANGE_START = _tcstod(szData, &stopstring);
	else
	{
		m_dAXIS_X_RANGE_START = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_Y_RANGE_START"), NULL, szData, sizeof(szData), sPath))
		m_dAXIS_Y_RANGE_START = _tcstod(szData, &stopstring);
	else
	{
		m_dAXIS_Y_RANGE_START = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_X_POINT_DELTA"), NULL, szData, sizeof(szData), sPath))
		m_dAXIS_X_POINT_DELTA = _tcstod(szData, &stopstring);
	else
	{
		m_dAXIS_X_POINT_DELTA = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_Y_POINT_DELTA"), NULL, szData, sizeof(szData), sPath))
		m_dAXIS_Y_POINT_DELTA = _tcstod(szData, &stopstring);
	else
	{
		m_dAXIS_Y_POINT_DELTA = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_X_TOTAL_POINT"), NULL, szData, sizeof(szData), sPath))
		m_nAXIS_X_TOTAL_POINT = _ttoi(szData);
	else
	{
		m_nAXIS_X_TOTAL_POINT = 0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("AXIS_Y_TOTAL_POINT"), NULL, szData, sizeof(szData), sPath))
		m_nAXIS_Y_TOTAL_POINT = _ttoi(szData);
	else
	{
		m_nAXIS_Y_TOTAL_POINT = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL CMyFileErrMap::LoadX(int nRow, int nIdx, CString sPath)
{
	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	CString str;

	TCHAR *stopstring;
	TCHAR *token;

	int nCol=0;
	str.Format(_T("%d"), nRow);
	if (0 < ::GetPrivateProfileString(_T("XY-X"), str, NULL, szData, sizeof(szData), sPath))
	{
		for(nCol=0; nCol<m_nAXIS_X_TOTAL_POINT+1; nCol++)
		{
			if(!nCol)
			{
				m_pErrPos[nIdx].nCol = nCol;
				m_pErrPos[nIdx].nRow = nRow;
				m_pErrPos[nIdx].dErrX = 0.0;

				nIdx++;
			}
			else
			{
				if(!(nCol-1))
					token = _tcstok(szData,sep);
				else
					token = _tcstok(NULL,sep);

				m_pErrPos[nIdx].nCol = nCol;
				m_pErrPos[nIdx].nRow = nRow;
				m_pErrPos[nIdx].dErrX = (double)_tcstod(token, &stopstring);

				nIdx++;
			}
		}
	}
	else
	{
		for(nCol=0; nCol<m_nAXIS_X_TOTAL_POINT+1; nCol++)
		{
			m_pErrPos[nIdx].nCol = nCol;
			m_pErrPos[nIdx].nRow = nRow;
			m_pErrPos[nIdx].dErrX = 0.0;

			nIdx++;
		}

		AfxMessageBox(_T("Error - Open FileErrMap X!!!"));

		return FALSE;
	}

	return TRUE;
}


BOOL CMyFileErrMap::LoadY(int nRow, int nIdx, CString sPath)
{
	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	CString str;

	TCHAR *stopstring;
	TCHAR *token;

	int nCol=0;
	str.Format(_T("%d"), nRow);
	if (0 < ::GetPrivateProfileString(_T("XY-Y"), str, NULL, szData, sizeof(szData), sPath))
	{
		for(nCol=0; nCol<m_nAXIS_X_TOTAL_POINT+1; nCol++)
		{
			if(!nCol)
			{
				m_pErrPos[nIdx].nCol = nCol;
				m_pErrPos[nIdx].nRow = nRow;
				m_pErrPos[nIdx].dErrY = 0.0;

				nIdx++;
			}
			else
			{
				if(!(nCol-1))
					token = _tcstok(szData,sep);
				else
					token = _tcstok(NULL,sep);

				m_pErrPos[nIdx].nCol = nCol;
				m_pErrPos[nIdx].nRow = nRow;
				m_pErrPos[nIdx].dErrY = (double)_tcstod(token, &stopstring);

				nIdx++;
			}
		}
	}
	else
	{
		for(nCol=0; nCol<m_nAXIS_X_TOTAL_POINT+1; nCol++)
		{
			m_pErrPos[nIdx].nCol = nCol;
			m_pErrPos[nIdx].nRow = nRow;
			m_pErrPos[nIdx].dErrY = 0.0;

			nIdx++;
		}

		AfxMessageBox(_T("Error - Open FileErrMap Y!!!"));

		return FALSE;
	}

	return TRUE;
}