#include "stdafx.h"
#include "User.h"

CUser::CUser()
{
	 m_strUserID = _T("");   // 사용자의 id
	 m_strUserName = _T(""); //사용자의 name
	 m_strPwd = _T("");	   //사용자의 password
	 m_dtLogin = COleDateTime::GetCurrentTime(); //Login한 시간
	 m_AccessType = OPERATOR;
	 m_bLogined = 0;
	 m_nAccessLevel = 1;
}

CUser::~CUser()
{

}

void CUser::SetAccessType(USER_ACCESS accss)
{
	m_AccessType = accss;
}
