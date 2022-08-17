// DataRow.cpp: implementation of the CDataRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataRow.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataRow::CDataRow()
{
	RowList = new CObList();
	m_RowStatus = VF_DEFAULT_R;
}

CDataRow::~CDataRow()
{
	while(RowList->GetCount())
	{	
		CData* data ;
		data = (CData*)RowList->GetHead();
		delete data;	
		RowList->RemoveHead();
	}
	delete RowList;
}

CString CDataRow::OnGetFieldValue(int col)
{
	if( RowList->IsEmpty())
	{	
		return _T("");
	}
	if(col < 0 || col >= RowList->GetCount())
	{	
		return _T("");
	}

	return ((CData*)RowList->GetAt(RowList->FindIndex(col)))->GetValue();
}

UINT CDataRow::OnGetRecordStatus()
{
	return m_RowStatus;
}

void CDataRow::OnSetRecordStatus(UINT status)
{
	m_RowStatus = status;
}


bool CDataRow::OnSetFieldValue(int iField, CString szFieldValue, bool isChange)
{
	if( RowList->IsEmpty())
	{	
		return false ;
	}

	CData* m_data ;
	m_data = (CData*)RowList->GetAt(RowList->FindIndex( iField ));
	if( isChange ) SetRecordStatusChange();
	return m_data->SetValue(szFieldValue);
}

void CDataRow::SetRecordStatusChange()
{
	if( m_RowStatus == VF_EMPTY_R ) m_RowStatus = VF_INSERTING_R;
	else if( m_RowStatus == VF_COMMITTED_R ) m_RowStatus = VF_UPDATING_R;
	else if( m_RowStatus == VF_DEFAULT_R ) m_RowStatus = VF_UPDATING_R;
}


bool CDataRow::OnCommit()
{
	if( RowList->IsEmpty()) 
	{	
		return true ;
	}

	CData* data;
	for(long i = 0 ; i < RowList->GetCount() ; i++)
	{
		data = (CData*)RowList->GetAt(RowList->FindIndex(i));		
		data->OnCommit();
	}
	m_RowStatus = VF_COMMITTED_R;
	return true;
}

bool CDataRow::OnIsValueChanged(int col)
{
	if( RowList->IsEmpty())
	{	
		return false ;
	}
	if( OnGetRecordStatus() != VF_UPDATING_R && OnGetRecordStatus() != VF_INSERTING_R )
		return false;

	CData* m_data ;
	m_data = (CData*)RowList->GetAt(RowList->FindIndex( col ));
	return m_data->OnIsValueChanged();

}
