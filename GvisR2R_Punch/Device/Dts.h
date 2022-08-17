#pragma once

#include "Query.h"

typedef enum { MsSQL = 0, MySQL}tpDB;

class CDts : public CQuery
{
	HWND m_hParentWnd;

	BOOL m_bUseDts;
	int m_nDBType;
	CString m_strDtsIp;
	CString m_strDbName;
	CString m_strUserID;
	CString m_strUserPw;

	void LoadInfo();

public:
	CDts(CWnd* pParent=NULL);
	~CDts();

	BOOL IsUseDts();
};

