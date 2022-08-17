// DataRow.h: interface for the CDataRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAROW_H__998F94B7_A876_4B7D_990B_9E23595382E3__INCLUDED_)
#define AFX_DATAROW_H__998F94B7_A876_4B7D_990B_9E23595382E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDataRow : public CObject  
{
public:
	bool OnIsValueChanged( int col);
	bool OnCommit();

	CString OnGetFieldValue(int col);
	CDataRow();
	virtual ~CDataRow();
	CObList *RowList;
	void OnSetRecordStatus(UINT status);
	UINT OnGetRecordStatus();

	bool OnSetFieldValue(int iField, CString szFieldValue, bool isChange=false);

private :
	UINT m_RowStatus;

	
protected:
	void SetRecordStatusChange();
};

#endif // !defined(AFX_DATAROW_H__998F94B7_A876_4B7D_990B_9E23595382E3__INCLUDED_)
