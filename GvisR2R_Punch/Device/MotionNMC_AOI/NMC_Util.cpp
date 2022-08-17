#include "../stdafx.h"
#include "NMC_Util.h"


CString GetNMCErrorMessage(UINT32 ErrorCode)
{
	char szMsg[MAX_ERR_LEN];
	MC_GetErrorMessage(ErrorCode, MAX_ERR_LEN, szMsg);

	CString strMsg = CharToString(szMsg);

	return strMsg;
}
