// Field.cpp: implementation of the CField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Field.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CField::CField(CString ps_NameCOL, unsigned short pi_Type,  DWORD pd_Size)
{
	_NameCOL = ps_NameCOL;
	_type = pi_Type;
	_Size = pd_Size;
}


CField::~CField()
{
}

CString CField::GetColName()
{
	return _NameCOL;
}

DWORD CField::OnGetSize()
{
	return _Size;
}

unsigned short CField::OnGetType()
{
	return _type;
}
