#include "stdafx.h"
#include "Dts.h"
#include "../Global/GlobalDefine.h"

CDts::CDts(CWnd* pParent /*=NULL*/)
{
	if (pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	LoadInfo();
	InitDB(m_strDtsIp, m_strDbName, m_strUserID, m_strUserPw, m_nDBType);
}


CDts::~CDts()
{
}


void CDts::LoadInfo()
{
	TCHAR szFilePath[MAX_PATH];
	CString strFilePath, strConfigFileName, strConfigFilePath;
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("UseDts"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bUseDts = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bUseDts = FALSE;

	// [tpDB::MsSQL, tpDB::MySQL]
	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("DBType"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_nDBType = _ttoi(szData);
	else
		m_nDBType = tpDB::MySQL;

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("Server IP"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_strDtsIp = szData;
	else
		m_strDtsIp = _T("100.100.101.10");

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("DBNAME"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_strDbName = szData;
	else
		m_strDbName = _T("GVISDB");


	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("USER"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_strUserID = szData;
	else
		m_strUserID = _T("root");

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("PASSWORD"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_strUserPw = szData;
	else
		m_strUserPw = _T("My3309");
}

BOOL CDts::IsUseDts()
{
	return m_bUseDts;
}