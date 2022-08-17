#include "stdafx.h"
#include "MsgBoxTimeout.h"

int MessageBoxTimeoutA(HWND hWnd, LPCSTR lpText, 
					   LPCSTR lpCaption, UINT uType, WORD wLanguageId, 
					   DWORD dwMilliseconds)
{
	static MSGBOXAAPI MsgBoxTOA = NULL;

	if (!MsgBoxTOA)
	{
		HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
		if (hUser32)
		{
			MsgBoxTOA = (MSGBOXAAPI)GetProcAddress(hUser32, 
				"MessageBoxTimeoutA");
			//fall through to 'if (MsgBoxTOA)...'
		}
		else
		{
			//stuff happened, add code to handle it here 
			//(possibly just call MessageBox())
			return 0;
		}
	}

	if (MsgBoxTOA)
	{
		return MsgBoxTOA(hWnd, lpText, lpCaption, 
			uType, wLanguageId, dwMilliseconds);
	}

	return 0;
}


int MessageBoxTimeoutW(HWND hWnd, LPCWSTR lpText, 
					   LPCWSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
{
	static MSGBOXWAPI MsgBoxTOW = NULL;

	if (!MsgBoxTOW)
	{
		HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
		if (hUser32)
		{
			MsgBoxTOW = (MSGBOXWAPI)GetProcAddress(hUser32, 
				"MessageBoxTimeoutW");
			//fall through to 'if (MsgBoxTOW)...'
		}
		else
		{
			//stuff happened, add code to handle it here 
			//(possibly just call MessageBox())
			return 0;
		}
	}

	if (MsgBoxTOW)
	{
		return MsgBoxTOW(hWnd, lpText, lpCaption, 
			uType, wLanguageId, dwMilliseconds);
	}

	return 0;
}