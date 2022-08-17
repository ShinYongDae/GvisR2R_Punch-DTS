#pragma once

typedef enum tagUserAccess
{
	OPERATOR,
	ENGINEER,
	ADMINISTRATOR
}USER_ACCESS;


class CUser
{
	public:
	CUser();
	~CUser();
	CString m_strUserID;   // 사용자의 id
	CString m_strUserName; //사용자의 name
	CString m_strPwd;	   //사용자의 password
	COleDateTime m_dtLogin; //Login한 시간
	USER_ACCESS m_AccessType;
	BOOL m_bLogined;
	int m_nAccessLevel;
	void SetAccessType(USER_ACCESS accss);
	BOOL m_bDeleted;
	void    operator =(CUser User) // Nonzero if the points are equal; otherwise 0.
	{
		m_bDeleted = 0;
		m_strUserID = User.m_strUserID;	// ID
		m_strUserName = User.m_strUserName;	// ID
		m_strPwd = User.m_strPwd;	// ID
		m_dtLogin = User.m_dtLogin;	// ID
		m_AccessType = User.m_AccessType;	// ID
		m_bLogined = User.m_bLogined;	// ID
		m_nAccessLevel = User.m_nAccessLevel;
	}
};