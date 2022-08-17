// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData( CString ps_Value)
{
	_Value = ps_Value;
	_IsChanged = false;
}

CData::CData(  )
{
	_Value = "";
	_IsChanged = false;
}

CData::~CData()
{
}

CString CData::GetValue()
{
	return _Value;
}

bool CData::SetValue(CString m_Value)
{
	_Value = m_Value;
	_IsChanged = true;
	return true;
}

bool CData::OnCommit()
{
	_IsChanged = false;
	return true;
}

bool CData::OnIsValueChanged()
{
	return _IsChanged;
}
