// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__88F515E9_8E43_4F5D_B327_979FFD66A353__INCLUDED_)
#define AFX_DATA_H__88F515E9_8E43_4F5D_B327_979FFD66A353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CData : public CObject  
{
public:
	bool OnIsValueChanged();
	bool OnCommit();

	bool SetValue(CString m_Value);
	CString GetValue();
	CData( );
	CData( CString ps_Value);
	virtual ~CData();
protected:

	CString _Value;
	bool	_IsChanged;

};

#endif // !defined(AFX_DATA_H__88F515E9_8E43_4F5D_B327_979FFD66A353__INCLUDED_)
