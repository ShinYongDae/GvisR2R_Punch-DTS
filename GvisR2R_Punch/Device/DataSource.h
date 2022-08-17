// DataSource.h: interface for the CDataSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASOURCE_H__C9A4F7D1_6700_4890_A4FF_7729A2FE0B64__INCLUDED_)
#define AFX_DATASOURCE_H__C9A4F7D1_6700_4890_A4FF_7729A2FE0B64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "adoconnection.h"

class CDataSource  
{
public:
	AdoNS::_RecordsetPtr m_pRS;

private:
	CObList		*m_DataSource;	
	CObList		*m_FieldInfo;
	CObList		*m_DefData;	
	CString		m_strTemp;
	CString		m_strErrMsg; // khc

	
public:
	CString		m_strDefTableName;
	int  *		m_ipKeyFields;		 
	int			m_iKeyFieldCount;
	CString		m_strDef[10];
	CAdoConnection	m_adoConn;
public:	
	bool OnKeyFieldCheck(int col);
	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword);
	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword, int nDbtype);
	BOOL DirectQuery(LPCTSTR szQuery);
	BOOL ExecuteQuery(LPCTSTR szQuery, CString strTableName=_T(""));
	CString GetLastError();
	CDataSource();
	virtual ~CDataSource();


	void		OnSetKeyField(int * iKeyField, int iKeyFieldCount);
	CString		OnRemoveRecord(const long lRecord, CString strWhere);
	CString		OnSetFieldData(int iField, CString szFieldValue);
	CString		OnGetQueryScript(long record, CString sWhere =_T("") ,bool bDBCommit=VF_COMMIT);

	void		OnReleaseMem();

	bool		OnSetFieldValue(CString szFieldName, long lRecord, CString szFieldValue);
	bool		OnSetFieldValue(int iField, long lRecord, CString szFieldValue);
	CString		OnGetFieldName(int col);
	CString		OnGetFieldValue( CString  strCol, int row);
	CString		OnGetFieldValue(int col,int row);
	int			OnGetFieldCount();
	int			OnGetRecordCount();

	DWORD		OnGetFieldSize(int iPos);
	WORD		OnGetFieldType(int iPos);
	int			OnGetFieldId(CString szFieldName);

	CString		OnGetTableName();
	UINT		OnGetRecordStatus(const long lRecord);
	void		OnSetRecordStatus(const long lRecord, UINT	uiFlag);

	bool		OnAppendRow();	 
	bool		OnRemoveRow(long row);

	bool		OnCommit( long row);											//VF_COMMITTED_R ·Î º¯°æ
	bool		OnCheckValueChanged(long row, int col);

protected:	
	void		SetDataBinding(AdoNS::_RecordsetPtr psRecordset, CString strTableName);

	CString		OnWriteUpdateScript(long record, CString m_where);
	CString		OnWriteInsertScript(long record);
};

#endif // !defined(AFX_DATASOURCE_H__C9A4F7D1_6700_4890_A4FF_7729A2FE0B64__INCLUDED_)
