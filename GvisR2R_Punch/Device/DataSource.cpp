// DataSource.cpp: implementation of the CDataSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "DBMS.h"
#include "DataSource.h"

#include "Data.h"
#include "Field.h"
#include "DataRow.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDataSource *pDataSource;

CDataSource::CDataSource()
{
	m_DataSource = new CObList();
	m_FieldInfo = new CObList();
	m_strDefTableName = _T("");
	m_iKeyFieldCount =0 ;
	m_ipKeyFields	  = NULL;
	m_strErrMsg = _T("");
}

CDataSource::~CDataSource()
{
	int i;
	while( i = m_DataSource->GetCount())
	{
		CDataRow* m_temp ;
		m_temp = (CDataRow*)m_DataSource->GetHead();

		if( !m_temp->RowList->IsEmpty())
		{
			while(m_temp->RowList->GetCount())
			{
				CData* data ;
				data = (CData*)m_temp->RowList->GetHead();
				delete data;
				m_temp->RowList->RemoveHead();
			}	
		}
		m_DataSource->RemoveHead();
		delete m_temp;
	}
	delete m_DataSource;

	while(m_FieldInfo->GetCount())
	{	
		CData* data ;
		data = (CData*)m_FieldInfo->GetHead();
		delete data;
		m_FieldInfo->RemoveHead();		
	}
	delete m_FieldInfo;

	if(m_ipKeyFields != NULL )
	{
		delete []	m_ipKeyFields;
		m_ipKeyFields = NULL;
	}
}


CString	CDataSource::OnGetTableName()
{
	return	m_strDefTableName;
}

void CDataSource::OnReleaseMem()
{
	while(m_DataSource->GetCount())
	{	 
		CDataRow* m_temp ;
		m_temp = (CDataRow*)m_DataSource->GetHead();

		if( !m_temp->RowList->IsEmpty())
		{ 
			while(m_temp->RowList->GetCount())
			{
				CData* data ;
				data = (CData*)m_temp->RowList->GetHead();
				delete data;
				m_temp->RowList->RemoveHead();
			}	
		}
		m_DataSource->RemoveHead();
		delete m_temp; // 2010,08,07 bug fix by khc
	}

	while(m_FieldInfo->GetCount())
	{	
		CData* data ;
		data = (CData*)m_FieldInfo->GetHead();
		delete data;
		m_FieldInfo->RemoveHead();		
	}

	if(m_ipKeyFields != NULL )
	{
		delete []	m_ipKeyFields;
		m_ipKeyFields = NULL;
	}

	m_iKeyFieldCount = 0;
	m_strDefTableName = _T("");
}


CString CDataSource::OnGetFieldValue(CString strCol,int row)
{
	return OnGetFieldValue( OnGetFieldId(strCol), row);
}

CString CDataSource::OnGetFieldValue( int col, int row)
{
	if( m_DataSource->IsEmpty())
	{		
		return _T("");
	}
	
	if( row < 0 || row > OnGetRecordCount() || col < 0 || col >= OnGetFieldCount() )
	{	
		return _T("");
	}

	if( row == OnGetRecordCount())
	{
		OnAppendRow();
		return _T("");
	}

	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(row));	

	return m_temp->OnGetFieldValue(col);	
}

bool CDataSource::OnSetFieldValue(CString szFieldName, long lRecord, CString szFieldValue)
{
	return OnSetFieldValue(OnGetFieldId(szFieldName), lRecord, szFieldValue);
}

bool CDataSource::OnSetFieldValue(int iField, long lRecord, CString szFieldValue)
{
	if( m_DataSource->IsEmpty())
	{		
		return false;
	}
	if( lRecord < 0 || lRecord >= OnGetRecordCount() )
	{		
		return false;
	}
	
	CDataRow* m_temp ; 
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(lRecord));
	
	return m_temp->OnSetFieldValue( iField, szFieldValue, OnKeyFieldCheck(iField));

}

int CDataSource::OnGetRecordCount()
{
	if( m_DataSource->IsEmpty())
	{	
		return 0;
	}		

	return m_DataSource->GetCount();
}

CString CDataSource::OnGetFieldName(int col)
{
	if( m_FieldInfo->IsEmpty())
	{	
		return _T("");
	}

	if( m_FieldInfo->GetCount() <=0 || m_FieldInfo->GetCount() < col+1)
		return _T("");
	CField* field;
	field = (CField*)m_FieldInfo->GetAt(m_FieldInfo->FindIndex(col));

	return field->GetColName();
}

int CDataSource::OnGetFieldCount()
{
	if( m_FieldInfo->IsEmpty())
	{ 
		return 0;
	}   

	return m_FieldInfo->GetCount();
}

DWORD CDataSource::OnGetFieldSize(int iPos)
{
	if( m_FieldInfo->IsEmpty())
	{		
		return 0;
	}
	if( m_FieldInfo->GetCount() <=0 || m_FieldInfo->GetCount() < iPos+1)  
		return 0;

	CField* field;
	field = (CField*)m_FieldInfo->GetAt(m_FieldInfo->FindIndex(iPos));

	return field->OnGetSize();
}


WORD CDataSource::OnGetFieldType(int iPos)
{
	if( m_FieldInfo->IsEmpty())		
		return 0;	
	if( m_FieldInfo->GetCount() <=0 || m_FieldInfo->GetCount() < iPos+1)  
		return 0; 
	
	CField* field;
	field = (CField*)m_FieldInfo->GetAt(m_FieldInfo->FindIndex(iPos));

	return field->OnGetType();
}

int CDataSource::OnGetFieldId(CString szFieldName)
{
	if( m_FieldInfo->IsEmpty())
	{	
		return -1;
	}
	if( m_FieldInfo->GetCount() <=0 )
		return -1;

	int m_return = -1;
	CField* data;
	for(long i = 0 ; i < m_FieldInfo->GetCount() ; i++)
	{
		data = (CField*)m_FieldInfo->GetAt(m_FieldInfo->FindIndex(i));
		if( data->GetColName() == szFieldName)
		{
			m_return = i;
			break;
		}
	}

	return m_return;
}

UINT CDataSource::OnGetRecordStatus(const long lRecord)
{
	if( m_DataSource->IsEmpty())
		return VF_EMPTY_R;
	if( lRecord < 0 || lRecord >= OnGetRecordCount() )
		return VF_EMPTY_R;
	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(lRecord));

	return m_temp->OnGetRecordStatus();
}

void CDataSource::OnSetRecordStatus(const long lRecord, UINT	uiFlag)
{
	if( m_DataSource->IsEmpty())
		return ;
	if( lRecord < 0 || lRecord >= OnGetRecordCount() )
		return ;
	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(lRecord));
	m_temp->OnCommit();
	m_temp->OnSetRecordStatus(uiFlag);
}


bool CDataSource::OnCommit(long row)
{
	if( m_DataSource->IsEmpty())
		return false;
	if( row < 0 || row >= OnGetRecordCount() )
		return false;
	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(row));
	return m_temp->OnCommit();
}


bool CDataSource::OnAppendRow()
{
	if( m_FieldInfo->IsEmpty())
	{	
		return false;
	}
	
	CDataRow *rowData = new CDataRow();
	CData* data ;
	for( long j = 0 ; j < OnGetFieldCount() ;j++)
	{	
		data = new CData();		
		rowData->RowList->AddTail(data);
	}
	rowData->OnSetRecordStatus(VF_EMPTY_R);
	m_DataSource->AddTail(rowData);	
	return true;
}


bool CDataSource::OnRemoveRow(long row)
{
	if( m_DataSource->IsEmpty()) 	
		return false;	
	if( m_DataSource->GetCount() <= 0 || m_DataSource->GetCount() < row+1)			
		return false;	

	CDataSource *m_dsDDS = new CDataSource();
	
	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(row));

	if( !m_temp->RowList->IsEmpty())
	{
		while(m_temp->RowList->GetCount())
		{
			CData* data ; 
			data = (CData*)m_temp->RowList->GetHead();
			delete data;
			m_temp->RowList->RemoveHead();
		}	
	}

	m_DataSource->RemoveAt(m_DataSource->FindIndex(row));
	return true;
}

bool CDataSource::OnCheckValueChanged(long row, int col)
{
	if( ! OnKeyFieldCheck(col) ) return false;

	if( m_DataSource->IsEmpty())
		return false;
	if( row < 0 || row >= OnGetRecordCount() )
		return false;

	CDataRow* m_temp ;
	m_temp = (CDataRow*)m_DataSource->GetAt(m_DataSource->FindIndex(row));

	return m_temp->OnIsValueChanged(col);
}

CString CDataSource::OnGetQueryScript(long record, CString sWhere , bool bDBCommit)
{
	if( m_strDefTableName == _T("") || m_strDefTableName == _T("NO_TABLE"))
		return _T("");

	UINT wState = OnGetRecordStatus(record);
	CString m_RetVal =	_T("");
	switch (wState ){
	case VF_EMPTY_R:		
	case VF_COMMITTED_R:	
		break;
	case VF_UPDATING_R:
		m_RetVal = OnWriteUpdateScript(record, sWhere);
		if(bDBCommit)
			OnCommit(record);
		break;
	case VF_INSERTING_R:
		m_RetVal = OnWriteInsertScript(record);
		if(bDBCommit)
			OnCommit(record);
		break;
	}
	return	m_RetVal;		
}

CString CDataSource::OnWriteInsertScript(long record)
{
	int			iFindPos, iFindPos2;
	CString		strFieldValue, strTemp;
	CString m_strInsertQuery, m_strInsertQuery2;
	bool isINSERT_DT = false;
	
	m_strInsertQuery.Empty();
	m_strInsertQuery2.Empty();

	CString _FieldName=_T("");

	m_strInsertQuery.Format(_T("INSERT INTO %s ( "), m_strDefTableName);
	m_strInsertQuery2.Format(_T(" ) VALUES ( ") );

	
	for( int iField=0; iField< OnGetFieldCount() ; iField++)
	{	
		m_strTemp.Format(_T("%s"), OnGetFieldName(iField));
		m_strTemp.Replace(_T("'"), _T("''"));
		if(m_strTemp != _T("") && OnCheckValueChanged(record, iField)) {
			strFieldValue = OnSetFieldData(iField, OnGetFieldValue(iField,record));		
			m_strInsertQuery2 += strFieldValue  + _T(", ");		
			m_strInsertQuery += OnGetFieldName(iField) + _T(", ");
		}
	}

	iFindPos  = m_strInsertQuery.ReverseFind(_T(','));
	iFindPos2 = m_strInsertQuery2.ReverseFind(_T(','));
	
	return	m_strInsertQuery.Left(iFindPos) + m_strInsertQuery2.Left(iFindPos2) + _T(" )");
}

CString CDataSource::OnWriteUpdateScript(long record, CString m_where)
{
	int			iFindPos;
	CString		_FieldName, strFieldValue, strField;
	CString		m_strUpdateQuery, szFieldValue;
	bool isINSERT_DT = false;

	m_strUpdateQuery.Empty();
	m_strUpdateQuery.Format (_T("UPDATE   %s  SET "), m_strDefTableName);
	
	for( int iField=0; iField< OnGetFieldCount() ; iField++)
	{	
		m_strTemp.Format(_T("%s"), OnGetFieldName(iField));
		m_strTemp.Replace(_T("'"), _T("''"));
		if(m_strTemp != _T("") && OnCheckValueChanged(record, iField)) {

			szFieldValue = OnGetFieldValue(iField,record);
			_FieldName = _T(" ") + OnGetFieldName(iField) + _T(" = ");
			szFieldValue.Replace(_T("'"),_T( "''"));
			strField.Format(_T(" %s %s"), _FieldName, OnSetFieldData(iField, szFieldValue));
			m_strUpdateQuery += strField + _T(", ");
		}
	}

	CString m_tempUpdateQuery = m_strUpdateQuery;
	iFindPos = m_strUpdateQuery.ReverseFind(_T(','));
	m_strTemp = m_strUpdateQuery.Left(iFindPos) + _T(" WHERE ") + m_where;	
	return	 m_strTemp;
}

CString CDataSource::OnSetFieldData(int iField, CString szFieldValue)
{
	m_strTemp = szFieldValue;
	
	WORD wType = OnGetFieldType( iField );
	switch(wType){
		case SQL_NUMERIC:
		case SQL_DECIMAL:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_DOUBLE:
		// integers
		case SQL_BIT:
		case SQL_TINYINT:
		case SQL_SMALLINT:
		case SQL_INTEGER:
		case SQL_BIGINT:
			if(m_strTemp.GetLength() <= 0)	m_strTemp= _T("0");
		break;
		
		case 65527:				//SQL_WVARCHAR  (-9)
		case 65528:				//SQL_WCHAR		(-8)
		case SQL_WVARCHAR:
		case SQL_WCHAR:	
		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:	//SQL_LONGVARCHAR는 -1(65535)
		case 65535:			
			m_strTemp = _T("\'") + szFieldValue + _T("\'");		
			break;
		// date time
		case SQL_DATE:
		case SQL_TIME:
		case SQL_TIMESTAMP:
			m_strTemp = _T("\'") + szFieldValue + _T("\'");
		break;

		// binary 
		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:		
		default:
			ASSERT(0);
	}
	return	m_strTemp;
}

CString CDataSource::OnRemoveRecord(const long lRecord, CString strWhere)
{
	strWhere.Replace(_T('\t'), _T(' '));
	strWhere.Replace(_T('\n'), _T(' '));
	strWhere.TrimLeft();

	m_strTemp.Format(_T("delete %s where %s"), m_strDefTableName, strWhere);

	if(m_adoConn.ExecuteNoRecords(m_strTemp))
		OnRemoveRow(lRecord);

	return	m_strTemp;

}


void CDataSource::OnSetKeyField(int *iKeyField, int iKeyFieldCount)
{
	if( iKeyField != NULL && iKeyFieldCount >0)
	{
		if(m_ipKeyFields != NULL ){
			delete	m_ipKeyFields;
			m_ipKeyFields = NULL;
		}
		m_ipKeyFields = new int[iKeyFieldCount];

		m_iKeyFieldCount = iKeyFieldCount;
		for(int i=0; i<m_iKeyFieldCount; i++)
			m_ipKeyFields[i] = iKeyField[i];
	}   
}

void CDataSource::SetDataBinding(AdoNS::_RecordsetPtr psRecordset, CString strTableName)
{
	if( psRecordset == NULL) 
		return ; 
	
	psRecordset->put_CursorType(AdoNS::adOpenDynamic);

	OnReleaseMem();

	if( psRecordset->Fields->GetCount() <= 0)
		return;

	CString strNM;
	_variant_t vColName,vValue; 

	//조회된 컬럼정보 
	for( long nField = 0 ; nField < psRecordset->Fields->GetCount() ;nField++)
	{
		_variant_t vColName = psRecordset->Fields->Item[(long)nField]->GetName();			
		strNM =  vColName.bstrVal;

		WORD pdwType,rsType;	
		rsType = psRecordset->Fields->GetItem((long)nField)->Type;
		switch(rsType)
		{
			case AdoNS::adBinary:			pdwType = SQL_BINARY;			break;
			case AdoNS::adVarBinary:		pdwType = SQL_VARBINARY;		break;
			case AdoNS::adLongVarBinary:	pdwType = SQL_LONGVARBINARY;	break;
			case AdoNS::adTinyInt:			pdwType = SQL_TINYINT;			break;
			case AdoNS::adDBTimeStamp:		pdwType = SQL_TIMESTAMP;		break;
			case AdoNS::adDBDate:			pdwType = SQL_DATE;				break;
			case AdoNS::adDBTime:			pdwType = SQL_TIME;				break;
			case AdoNS::adChar:			pdwType = SQL_CHAR;				break;
			case AdoNS::adVarChar:			pdwType = SQL_VARCHAR;			break;
			case AdoNS::adLongVarChar:		pdwType = SQL_LONGVARCHAR;		break;
			case AdoNS::adSmallInt:		pdwType = SQL_SMALLINT;			break;
			case AdoNS::adInteger:			pdwType = SQL_INTEGER;			break;
			case AdoNS::adNumeric:			pdwType = SQL_NUMERIC;			break;
			case AdoNS::adDecimal:			pdwType = SQL_DECIMAL;			break;
			case AdoNS::adDouble:			pdwType = SQL_DOUBLE;			break;
			case AdoNS::adBigInt:			pdwType = SQL_BIGINT;			break;
			case AdoNS::adWChar:			pdwType = SQL_WVARCHAR;			break;
			case AdoNS::adVarWChar:		pdwType = SQL_WVARCHAR;			break;

			default:				pdwType = SQL_VARCHAR;
		}
		CField* field = new CField(strNM, pdwType,  psRecordset->Fields->GetItem((long)nField)->DefinedSize);	
		m_FieldInfo->AddTail(field);		
	}

	if(!psRecordset->EndOfFile)
	{
		psRecordset->MoveFirst();
		while(!psRecordset->EndOfFile)
		{
			CDataRow *rowData = new CDataRow();
			for( long j = 0 ; j < psRecordset->Fields->GetCount() ;j++)
			{
				vValue = psRecordset->Fields->Item[j]->Value;

				if(vValue.vt !=VT_NULL)
				{
					vValue.ChangeType(VT_BSTR);
					strNM = vValue.bstrVal ;
				}
				else
				{
					strNM.Empty();
				}

				CData* data = new CData(strNM);			
				rowData->RowList->AddTail(data);
			}	
			m_DataSource->AddTail(rowData);
			psRecordset->MoveNext();
			
		}
	}

	m_strDefTableName = strTableName;
}

//조회쿼리문 실행
BOOL CDataSource::ExecuteQuery(LPCTSTR szQuery, CString strTableName)
{

//	_RecordsetPtr pRS = m_adoConn.Execute(szQuery);
	AdoNS::_RecordsetPtr pRS=NULL;

	m_pRS = pRS = m_adoConn.Execute(szQuery);


//khc	int i = m_pRS->RecordCount;

	if(pRS == NULL)
	{
		m_strErrMsg = m_adoConn.GetLastError();
		return FALSE;
	}
/*	
	if(pRS == NULL)
		SetDataBinding(pRS,strTableName);
	else
		return FALSE;
*/

	return TRUE;
}

//update, insert, delete 쿼리문 실행
BOOL CDataSource::DirectQuery(LPCTSTR szQuery)
{

	int cnt = m_adoConn.ExecuteNoRecords(szQuery);
//	TRACE("%s\n",szQuery);
	if(cnt >= 0)
	{
		return TRUE;
	}
	else
	{
		m_strErrMsg = m_adoConn.GetLastError();
	}

	return FALSE;
}

CString CDataSource::GetLastError()
{
	return m_strErrMsg;
}

void CDataSource::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword)
{
	m_adoConn.InitDB(szServerIP, szCatalog, szUserID, szUserPassword);
}

void CDataSource::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword, int nDbType)
{
	m_adoConn.InitDB(szServerIP, szCatalog,szUserID, szUserPassword, nDbType);
}

 bool CDataSource::OnKeyFieldCheck(int col)
 {
 	if(m_iKeyFieldCount > 0 && m_ipKeyFields != NULL )
 	{
 		for(int i=0; i<m_iKeyFieldCount; i++)
 		{
 			if(m_ipKeyFields[i] == col) 
 				return true;
 		}		
 	}
 	return false;
 }

