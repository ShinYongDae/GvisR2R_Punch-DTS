// Field.h: interface for the CField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELD_H__42CF0FB6_ECA1_40D3_8587_EC5E7D97D353__INCLUDED_)
#define AFX_FIELD_H__42CF0FB6_ECA1_40D3_8587_EC5E7D97D353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CField : public CObject  
{
public:
	CField(CString ps_NameCOL, unsigned short pi_Type,  DWORD pd_Size);
	virtual ~CField();

public:	
	unsigned short OnGetType();
	DWORD OnGetSize();	
	CString GetColName();

protected:
	unsigned short _type;
	DWORD	_Size;
	CString _NameCOL;
};

#endif // !defined(AFX_FIELD_H__42CF0FB6_ECA1_40D3_8587_EC5E7D97D353__INCLUDED_)
