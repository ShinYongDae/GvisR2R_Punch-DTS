// AdoConnectin.h: interface for the CAdoConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONN_H__0F60ED8A_AB15_4665_829B_4BF5E60B3822__INCLUDED_)
#define AFX_DBCONN_H__0F60ED8A_AB15_4665_829B_4BF5E60B3822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdoConnection  
{
public:
	AdoNS::_ConnectionPtr	m_pConnection;


public:
	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szPassword, int nDbTyep);
	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szPassword);
	
	AdoNS::_RecordsetPtr Execute(LPCTSTR szQuery);
	int ExecuteNoRecords(LPCTSTR szQuery);

	void DumpError(_com_error e,LPCTSTR szQuery=_T(""));

	BOOL IsOpen();
	BOOL Open(LPCTSTR szConnection);
	void Close();
	CString GetConnectionString();
	CString GetLastError();


	CAdoConnection();
	virtual ~CAdoConnection();

private:
	void Create();

	CString m_serverIP;
	CString m_catalog;
	CString m_userID;
	CString m_userPassword;
	CString m_strMsg;

	int m_nDBtype;
	void SetDBtype(int nDBType);  // 0 mssql, 1 mysql
};

#endif // !defined(AFX_DBCONN_H__0F60ED8A_AB15_4665_829B_4BF5E60B3822__INCLUDED_)
