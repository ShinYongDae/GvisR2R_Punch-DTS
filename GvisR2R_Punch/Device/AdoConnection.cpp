// AdoConnection.cpp: implementation of the CAdoConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "DBMS.h"
#include "AdoConnection.h"
#include "../Global/GlobalFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdoConnection::CAdoConnection()
{
	::CoInitialize(NULL);
	m_serverIP = _T("100.100.101.178");
	m_catalog = _T("GVISDB");
	m_userID = _T("root");
	m_userPassword = _T("My3309"); //Giga3309Vis__  GVIS__
	Create();
}

CAdoConnection::~CAdoConnection()
{
	if( m_pConnection != NULL )
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}


	::CoUninitialize();
}

void CAdoConnection::Create()
{	
	m_pConnection.CreateInstance(__uuidof(AdoNS::Connection));
}


void CAdoConnection::Close()
{
	if( m_pConnection )
	{
		if( m_pConnection->GetState() != AdoNS::adStateClosed )
			m_pConnection->Close();
	}
}

BOOL CAdoConnection::Open(LPCTSTR szConnection)
{

	BOOL fRet = TRUE;
	if( !szConnection ) return fRet;

	try{
		_bstr_t bstrConnection( szConnection );
		_bstr_t bstr_empty = _T("");
                if(m_nDBtype ==1)
	            m_pConnection->CursorLocation = AdoNS::adUseClient;

		m_pConnection->Open( bstrConnection, bstr_empty, bstr_empty, AdoNS::adConnectUnspecified );
	
		//DumpError( m_pConnection->GetErrors() );

		if( m_pConnection->GetState() == AdoNS::adStateClosed )
			fRet = FALSE;
	}
	catch( _com_error e )
	{
		DumpError ( e );
		AfxMessageBox((LPCTSTR)e.Description());

		fRet = FALSE;
	}

	return fRet;
}


void CAdoConnection::DumpError(_com_error e,LPCTSTR szQuery)
{
	m_strMsg = _T("");
	m_strMsg.Format(_T("Date       : %s\r\n")
					_T("Description: %s\r\n")
					_T("Exec Query : %s\r\n"),
					CTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")),
					(LPCTSTR)e.Description(),
					szQuery
					);
	//TRACE( m_strMsg );
}

CString CAdoConnection::GetLastError()
{
	return m_strMsg;
}

BOOL CAdoConnection::IsOpen()
{
	if( m_pConnection == NULL ) return FALSE;

	try
	{
		if( m_pConnection->GetState() == AdoNS::adStateClosed )
			return FALSE;
	}
	catch( _com_error e )
	{
		AfxMessageBox((LPCTSTR)e.Description());

		DumpError( e );
		return FALSE;
	}
	return TRUE;

}


void CAdoConnection::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szPassword,int nDbTyep)
{
	m_serverIP = szServerIP;
	m_catalog = szCatalog;
	m_userID = szUserID;
	m_userPassword = szPassword;
	m_nDBtype = nDbTyep;
}

void CAdoConnection::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szPassword)
{
	m_serverIP = szServerIP;
	m_catalog = szCatalog;
	m_userID = szUserID;
	m_userPassword = szPassword;	
}

//mssql 
CString CAdoConnection::GetConnectionString()
{

	//m_nDBtype = pGlobalDoc->m_nDBType;
/*	CString strConn;

	strConn.Format("Provider=SQLNCLI10;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;"  //"Provider=sqloledb;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;"
						, m_serverIP
						, m_catalog
						, m_userID
						, m_userPassword);

*/

	CString strConn;
	
	
	
	if(m_nDBtype ==1)
	{
		strConn.Format(_T("Driver={MySQL ODBC 5.3 Unicode Driver};Server=%s;Database=%s;UID=%s;PWD=%s")	
			, m_serverIP
			, m_catalog
			, m_userID
			, m_userPassword);   //Giga3309Vis__    GVIS__
	}
	else
	{
		strConn.Format(_T("Provider=sqloledb;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;Network Library=DBMSSOCN" ) //"Provider=sqloledb;Data Source=%s;Initial Catalog=%s;User Id=%s;Password=%s;"
			, m_serverIP
			, m_catalog
			, m_userID
			, m_userPassword);   //Giga3309Vis__    GVIS__
	}	
	return strConn;
}


//select 구문 실행
AdoNS::_RecordsetPtr CAdoConnection::Execute(LPCTSTR szQuery)
{
	_variant_t vRecsAffected(0L);
	AdoNS::_RecordsetPtr	RS;

	if( _tcslen(szQuery) < 1 ) 
	{		
		return NULL;
	}

	/*
#ifdef UNICODE
	char *pText = StringToChar(szQuery);

	if( strlen(pText) < 1 ) 
	{
		delete pText;
		pText = NULL;
		return NULL;
	}
	delete pText;
	pText = NULL;
#else
	if( strlen(szQuery) < 1 ) 
		return NULL;		
#endif
	*/
	CString myConn;

	myConn = GetConnectionString();


	try 
	{
		_bstr_t strConn( myConn );
		HRESULT hr = RS.CreateInstance(__uuidof(AdoNS::Recordset));

if(m_nDBtype ==1)
		RS->CursorLocation = AdoNS::adUseClient;
	
		RS->Open(szQuery, strConn, AdoNS::adOpenStatic, AdoNS::adLockReadOnly, AdoNS::adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox((LPCTSTR)e.Description()); // khc
		DumpError(e,szQuery);
		return NULL;
	}
	return RS;
}


//insert/update/delete 쿼리문 실행
int CAdoConnection::ExecuteNoRecords(LPCTSTR szQuery)
{
	_variant_t vRecsAffected(0L);
	int naffectedCnt = -1;

	if( !szQuery ) return FALSE;

	CString szConn = GetConnectionString();
#ifdef UNICODE

	char *pText = StringToChar(szConn);
	
	if(strlen(pText) > 0)
	{
		Close();
		Open(szConn);

		try
		{
			m_pConnection->Execute(szQuery,&vRecsAffected, AdoNS::adExecuteNoRecords);
			naffectedCnt = vRecsAffected.intVal;
		}
		catch(_com_error e)
		{
			//AfxMessageBox((LPCTSTR)e.Description());

			DumpError(e,szQuery);
		}
	}
	delete pText;
	pText = NULL;
#else
	if(strlen(szConn) > 0)
	{
		Close();
		Open(szConn);

		try
		{
			m_pConnection->Execute(szQuery,&vRecsAffected, AdoNS::adExecuteNoRecords);
			naffectedCnt = vRecsAffected.intVal;
		}
		catch(_com_error e)
		{
			//AfxMessageBox((LPCTSTR)e.Description());

			DumpError(e,szQuery);
		}
	}
#endif
	
	

	return naffectedCnt;
}

void CAdoConnection::SetDBtype(int nDBType)
{
	m_nDBtype = nDBType;
}