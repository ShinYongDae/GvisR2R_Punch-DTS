// GlobalFunc.cpp: implementation
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalFunc.h"
#include <math.h>

#pragma comment(lib,"shlwapi")
#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CCriticalSection g_LogLock;


char* StringToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

							   //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}

void StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}


char* StringToChar(CString str, int &nLen) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLen = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLen];  //메모리 할당 

							 //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLen, 0, 0);
	return szStr;
}

CString CharToString(const char *szStr)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(CP_ACP, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}

CString CharToStringEncode(const char *szStr, int nEncode)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(nEncode, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}

// CString → TCHAR
TCHAR* StringToTCHAR(CString str) // TCHAR* returned must be deleted... 
{
	TCHAR *tszStr = NULL;
	int nLen = str.GetLength() + 1;
	tszStr = new TCHAR[nLen];
	memset(tszStr, 0x00, nLen * sizeof(TCHAR));
	_tcscpy(tszStr, str);

	return tszStr;
}

// TCHAR → CString
CString TCHARToString(TCHAR *str)
{
	CString cStr;
	cStr.Format(_T("%s"), str);
	return cStr;
}


wchar_t* StringToWCHAR(CString str, int &nLen) // wchar_t* returned must be deleted... 
{
	char * chText;
	wchar_t *szCh = NULL;
	chText = (char *)str.GetBuffer(0);
	nLen = MultiByteToWideChar(949, 0, chText, -1, NULL, NULL);
	szCh = new wchar_t[nLen];
	MultiByteToWideChar(949, 0, chText, -1, szCh, nLen);
	return szCh;
}

wchar_t* CharToWCHAR(const char *str, int &nLen) // wchar_t* returned must be deleted... 
{
	wchar_t *szCh = NULL;
	nLen = MultiByteToWideChar(949, 0, str, -1, NULL, NULL);
	szCh = new wchar_t[nLen];
	MultiByteToWideChar(949, 0, str, -1, szCh, nLen);
	return szCh;
}

char* WCHARToChar(const wchar_t *str) // char* returned must be deleted... 
{
	int nLength = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *pText = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, str, -1, pText, nLength, 0, 0);
	return pText;
}

TCHAR* CharToTCHAR(char *str) // TCHAR* returned must be deleted... 
{
	int nLength = strlen(str) + sizeof(char);

	LPWSTR lpWideCharStr = new WCHAR[nLength];
	MultiByteToWideChar(CP_ACP, 0, str, -1, lpWideCharStr, nLength * sizeof(TCHAR));

	TCHAR *tszStr = NULL;
	tszStr = new TCHAR[nLength];

	USES_CONVERSION;
	_tcscpy(tszStr, W2T(lpWideCharStr));

	delete[] lpWideCharStr;
	return tszStr;
}

char* TCHARToChar(TCHAR *tszStr) // TCHAR* returned must be deleted... 
{
	char *szStr = NULL;
	int nLength;

	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(tszStr);

	nLength = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, szStr, nLength, NULL, NULL);

	return szStr;
}

char* TCHARToChar(const TCHAR *tszStr) // char* returned must be deleted... 
{
	char *szStr = NULL;
	int nLength;

#ifdef UNICODE
	nLength = WideCharToMultiByte(CP_ACP, 0, tszStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, tszStr, -1, szStr, nLength, NULL, NULL);
#else
	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(tszStr);

	nLength = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, szStr, nLength, NULL, NULL);
#endif

	return szStr;
}

char* UnicodeToMultibyte(CString strUnicode)
{
	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(strUnicode.GetBuffer(0));

	return UnicodeToMultibyte(lpWideCharStr);
}

char* UnicodeToMultibyte(LPCWSTR szUnicode) // char* returned must be deleted... 
{
	int nLen;
	char* pMultibyte = NULL;

	nLen = WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, NULL, 0, NULL, NULL);
	pMultibyte = new char[nLen];
	nLen = WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, pMultibyte, nLen, NULL, NULL);
	return pMultibyte;
}

CString MultibyteToUnicode(LPCSTR pMultibyte)
{
	CString strUnicode;
	int		nLength = int(strlen(pMultibyte) + sizeof(CHAR));

	LPWSTR lpWideCharStr = (LPWSTR)malloc(nLength * sizeof(WCHAR));				// UCS2 용 버퍼 잡기
	if (lpWideCharStr == NULL)
	{
		ASSERT(0);
		strUnicode.Empty();
		return strUnicode;
	}

	nLength = MultiByteToWideChar(CP_ACP, 0, pMultibyte, -1, lpWideCharStr, nLength);

	strUnicode = lpWideCharStr;
	free(lpWideCharStr);

	return strUnicode;
}


CString  GetThisPath(HMODULE hModule)
{
	static TCHAR szModuleName[MAX_PATH * 4] = { '\0' };
	if(!*szModuleName)
	{
		::GetModuleFileName(hModule, szModuleName, MAX_PATH * 4);
		TCHAR *pLastBackslash = _tcsrchr(szModuleName, '\\');
		if(pLastBackslash) *(pLastBackslash + 1) = '\0';
	}
	return CString(szModuleName);
}


int OpenFileDialog(HWND hwnd,TCHAR* path,TCHAR *FileFilter,TCHAR* Title,int mode)
{
	int findex;
  // OPENFILENAME ofn;
    OPENFILENAME ofn;
	memset(&ofn,0,sizeof(OPENFILENAME)); *path = 0;
    ofn.lStructSize         = sizeof(OPENFILENAME); 
    ofn.hwndOwner        = hwnd; 
    ofn.lpstrFilter           = FileFilter; // 파일 형식 필터 
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter    = 0; 
    ofn.nFilterIndex        = 1; 
	findex = (int)ofn.nFilterIndex;
    ofn.lpstrFile            = path; // 경로가 들어간다. 
    ofn.nMaxFile            = sizeof(WCHAR)*MAX_PATH; 
    ofn.lpstrTitle            = Title; // 타이틀 이름 
    ofn.lpstrInitialDir    = _T("File Open\0");  // 파일이 열기가 실행된 위치 
    ofn.lpstrFileTitle       = NULL; 
    ofn.lpstrDefExt         = NULL; 
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    
	if(mode == 1)
	return GetOpenFileName((LPOPENFILENAME)&ofn);
	else
	return GetSaveFileName((LPOPENFILENAME)&ofn);
}


//////////////////////////////////////////////////////////////////
// Function name    : GetDiskSerial
// Description      : HDD의 Serial Number를 리턴한다.
// Return type      : DWORD  
// Argument         : int drv(0,1,2,...)
DWORD GetDiskSerial(int drv) 
{ 
	CString strRootPathName;
	TCHAR strVolumeNameBuffer[129]; 
	DWORD dwVolumeSerialNumber=0; 
	DWORD dwMaximumComponentLength=0; 
	DWORD dwFileSystemFlags=0; 
	TCHAR  strFileSystemNameBuffer[129]; 
	strRootPathName.Format(_T("%c:\\"), drv+'A');
//	_stprintf( szBuffer, "%c:\", drv+'A'); 

	GetVolumeInformation(strRootPathName,// address of root directory of the file system
						 strVolumeNameBuffer, // address of name of the volume
						 sizeof(strVolumeNameBuffer)-1, // length of lpVolumeNameBuffer
						 &dwVolumeSerialNumber, // address of volume serial number
						 &dwMaximumComponentLength, // address of system's maximum filename length
						 &dwFileSystemFlags, // address of file system flags
						 strFileSystemNameBuffer, // address of name of file system
						 sizeof(strFileSystemNameBuffer)-1); // length of lpFileSystemNameBuffer
	return dwVolumeSerialNumber;
}

DWORD GetDiskSpace() 
{
	ULARGE_INTEGER i64FreeBytesToCaller,i64TotalBytes,i64FreeBytes;
    TCHAR * driveList = new TCHAR[105];
    memset(driveList,0,sizeof(TCHAR)*105);
    GetLogicalDriveStrings(104,driveList);    
    TCHAR drive[10];        
    CString    info,name;

    int i = 0;    
    while(driveList[i] != 0)
	{        
        memset(drive,0,sizeof(TCHAR)*10);        
        _stprintf(drive,_T("%c:\\\\"),toupper(driveList[i]));    
    
        if(GetDriveType(drive) == DRIVE_UNKNOWN 
            || GetDriveType(drive) == DRIVE_REMOTE     
            || GetDriveType(drive) == DRIVE_CDROM)    
        {            
			i += 4;        
			continue;
        }
        ZeroMemory(&i64FreeBytesToCaller, sizeof(i64FreeBytesToCaller));        
        ZeroMemory(&i64TotalBytes,sizeof(i64TotalBytes));    
        ZeroMemory(&i64FreeBytes,sizeof(i64FreeBytes));    
            
        GetDiskFreeSpaceEx (drive,                
							(PULARGE_INTEGER)&i64FreeBytesToCaller,                
							(PULARGE_INTEGER)&i64TotalBytes,                
							(PULARGE_INTEGER)&i64FreeBytes);        
        name.Format(_T("\nDrive %c: TotalSize %I64u MB FreeSize %I64u MB"),
					toupper(driveList[ i ]), 
					i64TotalBytes.QuadPart/(1024*1024),
					i64FreeBytes.QuadPart/(1024*1024));        
        info += name;
        i += 4;            
    }
    AfxMessageBox(info);
	delete []driveList;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
// Function name    : IsRRN
// Description      : 입력된 문자열을 검사해서 주민등록번호인지 여부를 판단한다.
// Return type      : BOOL 
// Argument         : CString sRRN
// 공식
// 1. 각 숫자와 2,3,4,5,6,7,8,9,2,3,4,5
// 2. 11로 나눈후 나머지를 구해서 다시 11에서 뺌.
// 3. 다시 10으로 나눈후 나머지를 취함
BOOL IsRRN(CString sRRN)
{
#define _RRN_LEN    14  //주민등록번호 길이

    if (sRRN.IsEmpty())
		return FALSE;
    sRRN.Replace(_T(" "), _T("")); //공백 제거
    if (sRRN.GetLength() != _RRN_LEN) 
		return FALSE; //길이 검사
    if (sRRN[6] != '-')
		return FALSE; //'-' 위치 검사

    int Gender;
    Gender = sRRN[7]-'0'; //남성(1,3) 여성(2,4)

    if (Gender < 1 || Gender > 4)
		return FALSE;
    int Val = 0;
    int nWeight[] = {2, 3, 4, 5, 6, 7, 
			        0, //'-' 값 무효화 가중치
					8, 9, 2, 3, 4, 5};
    for (int i=0; i<_RRN_LEN-1; i++) 
        Val += (sRRN[i] - '0') * nWeight[i];

    Val = 11 - (Val % 11);
    Val = Val % 10;
    if (Val == (sRRN[13]-'0'))
		return TRUE;
    return FALSE;
}

//////////////////////////////////////////////////////////////////
// Function name    : ExtractFirstWord
// Description      : 입력된 문자열중 첫 워드를 제거한다.
// Return type      : CString  입력된 문자열중 제거된 첫 워드
// Argument         : CString *str, CString sep,BOOL bTrim
// 
// 1. str : Source String
// 2. sep : Delimiter
// 3. bTrim : 좌우 Space의 제거 Option
CString ExtractFirstWord(CString *str, CString sep,BOOL bTrim)
{
	CString csRtn;
	int nDelPos = str->Find(sep,0);
	if(nDelPos < 0)
	{
		csRtn = *str;
		*str = _T("");
	}
	else
	{
		csRtn = str->Left(nDelPos);
		*str = str->Right(str->GetLength()-(csRtn.GetLength()+sep.GetLength()));
	}
	
	if(bTrim)
	{
		csRtn.TrimLeft(VK_SPACE); csRtn.TrimLeft(VK_TAB);
		csRtn.TrimRight(VK_SPACE); csRtn.TrimRight(VK_TAB);
	}

	return csRtn;
}
/*
CString ExtractFirstWord(CString& str, CString sep,BOOL bTrim=FALSE)
{
	CString csRtn;
	int nDelPos = str.Find(sep,0);
	if(nDelPos < 0)
	{
		csRtn = str;
		str = _T("");
	}
	else
	{
		csRtn = str.Left(nDelPos);
		str = str.Right(str.GetLength()-(csRtn.GetLength()+sep.GetLength()));
	}
	
	if(bTrim)
	{
		csRtn.TrimLeft(VK_SPACE); csRtn.TrimLeft(VK_TAB);
		csRtn.TrimRight(VK_SPACE); csRtn.TrimRight(VK_TAB);
	}
	return csRtn;
}
*/

CString ExtractFirstWord(CString& str, CString sep,BOOL bTrim)
{
	CString csRtn;
	int nDelPos = str.Find(sep,0);
	int i;

	if(nDelPos < 0)
	{
		csRtn = str;
		str = _T("");
	}
	else
	{
		csRtn = str.Left(nDelPos);
		i=sep.GetLength();
		str.Delete(0,nDelPos+i);		//ndy20071009
	}
	
	if(bTrim)
	{
		csRtn.TrimLeft(VK_SPACE); csRtn.TrimLeft(VK_TAB);
		csRtn.TrimRight(VK_SPACE); csRtn.TrimRight(VK_TAB);
	}
	return csRtn;
}

//////////////////////////////////////////////////////////////////
// Function name    : ExtractLastWord
// Description      : 입력된 문자열중 마지막 워드를 제거한다.
// Return type      : CString  입력된 문자열중 제거된 마지막 워드
// Argument         : CString *str, CString sep,BOOL bTrim
// 
// 1. str : Source String
// 2. sep : Delimiter
// 3. bTrim : 좌우 Space의 제거 Option
CString ExtractLastWord(CString *str, TCHAR sep,BOOL bTrim)
{
	CString csRtn;
	int nDelPos = str->ReverseFind(sep);
	if(nDelPos < 0)
	{
		csRtn = *str;
		*str = _T("");
	}
	else
	{
		csRtn = str->Right(str->GetLength()-nDelPos-1);
		*str = str->Left(nDelPos);
	}
	
	if(bTrim)
	{
		csRtn.TrimLeft(VK_SPACE); csRtn.TrimLeft(VK_TAB);
		csRtn.TrimRight(VK_SPACE); csRtn.TrimRight(VK_TAB);
	}

	return csRtn;
}

CString ExtractLastWord(CString& str, TCHAR sep,BOOL bTrim)
{
	CString csRtn;
	int nDelPos = str.ReverseFind(sep);
	if(nDelPos < 0)
	{
		csRtn = str;
		str = _T("");
	}
	else
	{
		csRtn = str.Right(str.GetLength()-nDelPos-1);
		str = str.Left(nDelPos);
	}
	
	if(bTrim)
	{
		csRtn.TrimLeft(VK_SPACE); csRtn.TrimLeft(VK_TAB);
		csRtn.TrimRight(VK_SPACE); csRtn.TrimRight(VK_TAB);
	}
	return csRtn;
}

//////////////////////////////////////////////////////////////////
// Function name    : GetCurrentDisplayMode
// Description      : 현재 Display Setting을 얻어온다.
// Return type      : BOOL
// Argument         : DEVMODE *devMode
BOOL GetCurrentDisplayMode(DEVMODE *devMode)
{
    HDC hDC;
    int iOrgX, iOrgY, iOrgBpp, iOrgRef;
    DWORD iModeNum;
    BOOL bResult;

    hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    iOrgX = GetDeviceCaps(hDC, HORZRES);
    iOrgY = GetDeviceCaps(hDC, VERTRES);
    iOrgBpp = GetDeviceCaps(hDC, BITSPIXEL);
    iOrgRef = GetDeviceCaps(hDC, VREFRESH);
    DeleteDC(hDC);

    // start mode enumeration
    iModeNum = 0;
    bResult = EnumDisplaySettings(NULL, iModeNum, devMode);

    while (bResult) 
    {
        if ((int)devMode->dmPelsWidth == iOrgX && (int)devMode->dmPelsHeight == iOrgY &&
             (int)devMode->dmBitsPerPel == iOrgBpp && (int)devMode->dmDisplayFrequency == iOrgRef) 
        {
            return TRUE;
        }

        // enumerate next available display modes
        iModeNum++;
        bResult = EnumDisplaySettings(NULL, iModeNum, devMode);
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////
// Function name    : GetCurrentDisplayMode
// Description      : Display Setting을 바꾸어준다.
// Return type      : BOOL
// Argument         : DEVMODE *devMode
BOOL SetCurrentDisplayMode(DEVMODE *devMode)
{
    LONG lReturn;

    lReturn = ChangeDisplaySettings(devMode, 0);

    // set new mode for auto training OK
    if (lReturn == DISP_CHANGE_SUCCESSFUL || lReturn == DISP_CHANGE_RESTART) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//int MessageBoxTimeoutA(HWND hWnd, LPCSTR lpText,LPCSTR lpCaption, UINT uType, WORD wLanguageId,DWORD dwMilliseconds)
//{
//    static MSGBOXAAPI MsgBoxTOA = NULL;
//
//    if (!MsgBoxTOA)
//    {
//        HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
//        if (hUser32)
//        {
//            MsgBoxTOA = (MSGBOXAAPI)GetProcAddress(hUser32,"MessageBoxTimeoutA");
//            //fall through to 'if (MsgBoxTOA)...'
//        }
//        else
//        {
//            //stuff happened, add code to handle it here
//            //(possibly just call MessageBox())
//            return 0;
//        }
//    }
//
//    if (MsgBoxTOA)
//    {
//        return MsgBoxTOA(hWnd, lpText, lpCaption,uType, wLanguageId, dwMilliseconds);
//    }
//
//    return 0;
//}
//
//int MessageBoxTimeoutW(HWND hWnd, LPCWSTR lpText,LPCWSTR lpCaption, UINT uType, WORD wLanguageId,DWORD dwMilliseconds)
//{
//    static MSGBOXWAPI MsgBoxTOW = NULL;
//
//    if (!MsgBoxTOW)
//    {
//        HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
//        if (hUser32)
//        {
//            MsgBoxTOW = (MSGBOXWAPI)GetProcAddress(hUser32,"MessageBoxTimeoutW");
//            //fall through to 'if (MsgBoxTOW)...'
//        }
//        else
//        {
//            //stuff happened, add code to handle it here
//            //(possibly just call MessageBox())
//            return 0;
//        }
//    }
//
//    if (MsgBoxTOW)
//    {
//        return MsgBoxTOW(hWnd, lpText, lpCaption,uType, wLanguageId, dwMilliseconds);
//    }
//
//    return 0;
//}


CString DtoA(double fTmp)
{
	CString str;
	str.Format(_T("%3.2f"),fTmp);
	return str;
} 

CString DtoA(TCHAR* szFormat, double fTmp)
{
	CString str;
	str.Format(szFormat,fTmp);
	return str;
} 

CString DtoA(TCHAR* szFormat, int nTmp)
{
	CString str;
	str.Format(szFormat,nTmp);
	return str;
} 

CString DtoA(int nTmp)
{
	CString str;
	str.Format(_T("%02d"),nTmp);
	return str;
}

CString DtoA(UINT nTmp)
{
	CString str;
	str.Format(_T("%02d"),nTmp);
	return str;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double mm2um(double fmm)
{
	return fmm*1000.0;
}
double um2mm(double fum)
{
	return fum/1000.0;

}

double GetDistance(CfPoint3D FirstPoint,CfPoint3D SecondPoint)
{
	return sqrt(pow(SecondPoint.x-FirstPoint.x,2)+pow(SecondPoint.y-FirstPoint.y,2));
}

double D2R(double fDeg)
{
	double fVal = fDeg* DEG2RAD ;
	return fVal;
}


double R2D(double fRadian)
{
	double fVal = fRadian*RAD2DEG;
	return fVal;
}

void swap(double& p1,double& p2){
	double temp = p1;
	p1 = p2;
	p2 = temp;
}

CfPoint RotatePoint(CfPoint fptRef,CfPoint fptCenter,double fRotAngle)
{
	if(fRotAngle == 0.0)
		return fptRef;

	fptRef = fptRef - fptCenter;
	double fRadius = sqrt(fptRef.x*fptRef.x + fptRef.y*fptRef.y);
	double fRefAngle = atan2(fptRef.y,fptRef.x)*RAD2DEG;
	double fAngle = fRotAngle + fRefAngle;

	CfPoint fpt;		
	fpt.x = fRadius*cos(fAngle*DEG2RAD);
	fpt.y = fRadius*sin(fAngle*DEG2RAD);
	fpt += fptCenter;

	return fpt;
}

BOOL IsPerpendicular(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint)
{
	double yDelta_a= middlePoint.y - startPoint.y;
	double xDelta_a= middlePoint.x - startPoint.x;
	double yDelta_b= endPoint.y - middlePoint.y;
	double xDelta_b= endPoint.x - middlePoint.x;

	// checking whether the line of the two pts are vertical
	if (fabs(xDelta_a) <= 0.0 && fabs(yDelta_b) <= 0.0)
	{
		TRACE(_T("The points are pependicular and parallel to x-y axis\n"));
		return TRUE;
	}
	if (fabs(yDelta_a) <= 0.0)
	{
		TRACE(_T(" A line of two point are perpendicular to x-axis 1\n"));
		return TRUE;
	}
	else if (fabs(yDelta_b) <= 0.0)
	{
		TRACE(_T(" A line of two point are perpendicular to x-axis 2\n"));
		return TRUE;
	}
	else if (fabs(xDelta_a) <= 0.0)
	{
		TRACE(_T(" A line of two point are perpendicular to y-axis 1\n"));
		return TRUE;
	}
	else if (fabs(xDelta_b) <= 0.0)
	{
		TRACE(_T(" A line of two point are perpendicular to y-axis 2\n"));
		return TRUE;
	}
	else
		return FALSE ;

	return TRUE;
}

//Equation of a Circle from 3 Points
CfPoint ArcCenter(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint)
{
	double ma = (middlePoint.y - startPoint.y) / (middlePoint.x - startPoint.x);
	double mb = (endPoint.y - middlePoint.y) / (endPoint.x - middlePoint.x);

	double centerX = (ma * mb * (startPoint.y - endPoint.y) + mb * (startPoint.x + middlePoint.x) - 
	ma * (middlePoint.x + endPoint.x)) / (2 * (mb - ma)); 
	double centerY = (-1 * (centerX - (startPoint.x + middlePoint.x) / 2) / ma) + 
	((startPoint.y + middlePoint.y) / 2); 

	CfPoint fptCenter;
	fptCenter = CfPoint(centerX,centerY);
	return fptCenter;
}

double ArcRadius(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint)
{
	CfPoint fptCenter = ArcCenter(startPoint,middlePoint,endPoint);
	double fRadius = sqrt((startPoint.x-fptCenter.x)*(startPoint.x-fptCenter.x)+(startPoint.y-fptCenter.y)*(startPoint.y-fptCenter.y));
	return fRadius;
}

double ArcAngle(CfPoint startPoint,CfPoint endPoint,CfPoint centerPoint)
{
	// 두 선분의 각도 계산
	double dy_a= startPoint.y - centerPoint.y;
	double dx_a= startPoint.x - centerPoint.x;
    double angle_a = atan2(dy_a , dx_a) * 180.0 / PI;
	angle_a = fmod(angle_a+360.0,360);
	
	double dy_b= endPoint.y - centerPoint.y;
	double dx_b= endPoint.x - centerPoint.x;
	double angle_b = atan2(dy_b , dx_b) * 180.0 / PI;
	angle_b = fmod(angle_b+360.0,360);

	if(angle_b < angle_a)
		angle_b += 360.0;
	double fAngle = angle_b-angle_a;

	return fAngle;
}

//++++++++++++++++++++++++++++
// Arc길이가 주어졌을때 각도 계산
double ArcAngle(CfPoint startPoint,CfPoint centerPoint,double fArcLength)
{
	double fRadius = sqrt((startPoint.x-centerPoint.x)*(startPoint.x-centerPoint.x)+(startPoint.y-centerPoint.y)*(startPoint.y-centerPoint.y));
	double fCircleLength = 2*PI*fRadius;
	
	if(fCircleLength == 0)
		return 0;

	double fAngle = (fArcLength/fCircleLength)*360.0;

	return fmod(fAngle,360.0);
}

double ArcLength(CfPoint startPoint,CfPoint middlePoint,CfPoint endPoint)
{
	CfPoint centerPoint;
	if (!IsPerpendicular(startPoint, middlePoint, endPoint))
		centerPoint = ArcCenter(startPoint, middlePoint, endPoint);	
	else if (!IsPerpendicular(startPoint, endPoint, middlePoint))
		centerPoint = ArcCenter(startPoint, endPoint, middlePoint);	
	else if (!IsPerpendicular(middlePoint, startPoint, endPoint))
		centerPoint = ArcCenter(middlePoint, startPoint, endPoint);	
	else if (!IsPerpendicular(middlePoint, endPoint, startPoint))
		centerPoint = ArcCenter(middlePoint, endPoint, startPoint);	
	else if (!IsPerpendicular(endPoint, middlePoint, startPoint))
		centerPoint = ArcCenter(endPoint, middlePoint, startPoint);	
	else if (!IsPerpendicular(endPoint, startPoint, middlePoint))
		centerPoint = ArcCenter(endPoint, startPoint, middlePoint);	
	else { 
		TRACE(_T("\nThe three pts are perpendicular to axis\n"));
		return -1;
	}
	// 각도 계산
	double fAngle = ArcAngle(startPoint,endPoint,centerPoint);
	fAngle = fabs(fAngle);

	double fRadius = sqrt((startPoint.x-centerPoint.x)*(startPoint.x-centerPoint.x) + (startPoint.y-centerPoint.y)*(startPoint.y-centerPoint.y));
	double fArcLength = 2*PI* fRadius;
	fArcLength = fArcLength * fAngle / 360.0;
	return fArcLength;
}

//******************************************************************************
// CheckPointInQuadrilateral finds if a point is inside a convex quadrilateral in 2D.
// Modified:
// 06 February 1999
// Author:
// John Burkardt
// Parameters:
// Input, real X1, Y1, X2, Y2, X3, Y3, X4, Y4, the X and Y coordinates of the quadrilateral.
// Input, real X, Y, the point to be checked.
// Output, TRUE if ( X,Y) is inside the quadrilateral or on its boundary, and FALSE otherwise.
BOOL CheckPointInQuad(
		double x1,double y1,double x2,double y2,double x3,double y3,
		double x4,double y4,double x,double y)
{
	double angle_123;
	double angle_12x;
	double angle_234;
	double angle_23x;
	double angle_341;
	double angle_34x;
	double angle_412;
	double angle_41x;
	BOOL inside;

	// This will only handle convex quadrilaterals.
	inside = FALSE;

	angle_12x = GetInteriorAngle( x1, y1, x2, y2, x, y );
	angle_123 = GetInteriorAngle( x1, y1, x2, y2, x3, y3);

	if ( angle_12x > angle_123 )
		return FALSE;

	angle_23x = GetInteriorAngle( x2, y2, x3, y3, x, y );
	angle_234 = GetInteriorAngle( x2, y2, x3, y3, x4, y4 );

	if ( angle_23x > angle_234 )
		return FALSE;

	angle_34x = GetInteriorAngle( x3, y3, x4, y4, x, y );
	angle_341 = GetInteriorAngle( x3, y3, x4, y4, x1, y1 );

	if ( angle_34x > angle_341 )
		return FALSE;

	angle_41x = GetInteriorAngle( x4, y4, x1, y1, x, y );
	angle_412 = GetInteriorAngle( x4, y4, x1, y1, x2, y2 );

	if ( angle_41x > angle_412 )
		return FALSE;

	return TRUE;
}

//////////////////////////////////////
// 3점이 주어졌을때 나머지 한점을 계산하여 리턴한다.
// 이때 구해지는 나머지 한점은 파라메타 입력순서에 영향을 받는다.
// 파라메타의 입력방향으로 한점이 생긴다.
void GetCornerPoint(double x1,double y1,double x2,double y2,double x3,double y3,double *x0,double *y0) 
{
	double a1,b1,a2,b2,cx,cy;
	
	cx = x1-x2+x3;	
	cy = y1-y2+y3;	

	*x0 = cx;
	*y0 = cy;
}

// 두선분의 벡터 방향을 나타낸다.
// 시계방향일 경우 (1)을 반시계 방향일경우 (-1)을  리턴한다.
// 두선분이 동일한 방향이라면 0를 리턴한다.
int GetVectorDir(double ax, double ay, double bx, double by, double cx, double cy)
{
	double temp;
	temp = bx*cy - ay*bx - ax*cy - by*cx + ax*by + ay*cx;
	if(temp >0.0)
		return CW;
	else if(temp <0.0) 
		return CCW;
	else 
		return PARALLEL;
}

int GetVectorDir(FPOINT start,FPOINT middle,FPOINT end)
{
	double temp;
	temp = middle.x*end.y - start.y*middle.x - start.x*end.y - middle.y*end.x 
		   + start.x*middle.y + start.y*end.x;
	if(temp >0.0)
		return CW;
	else if(temp <0.0) 
		return CCW;
	else 
		return PARALLEL;
}

int GetVectorDir(FPOINT3D start,FPOINT3D middle,FPOINT3D end)
{
	double temp;
	temp = middle.x*end.y - start.y*middle.x - start.x*end.y - middle.y*end.x 
		   + start.x*middle.y + start.y*end.x;
	if(temp >0.0)
		return CW;
	else if(temp <0.0) 
		return CCW;
	else 
		return PARALLEL;
}


/////////////////////////////////////
// GetInteriorAngle returns the interior angle in radians between two rays in 2D.
// Modified:
// 19 April 1999
// Author:
// John Burkardt
// Parameters:
// Input, double X1, Y1, X2, Y2, X3, Y3, define the rays
// (X1-X2,Y1-Y2) and (X3-X2,Y3-Y2) which in turn define the angle.
// Output, the angle swept out by the rays, measured in radians.
// This value satisfies 0 <= ANGLEI_RAD_2D 
// ray is of zero length, then ANGLEI_RAD_2D is returned as 0.
double GetInteriorAngle(double x1,double y1,double x2,double y2,double x3,double y3)
{
	double fAngleRad;
	double x;
	double y;

	x = ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
	y = ( x1 - x2 ) * ( y3 - y2 ) - ( y1 - y2 ) * ( x3 - x2 );

	if ( x == 0.0 && y == 0.0)
		fAngleRad = 0.0;
	else
	{
		fAngleRad = atan2(y,x);
		if (fAngleRad <0.0)
			fAngleRad = fAngleRad + 2.0 * PI;
	}
	
	if(GetVectorDir(x1,y1,x2,y2,x3,y3) == 1)
		fAngleRad = 2.0 * PI - fAngleRad;
		
	return fAngleRad/PI*180.; //  Degree단위로 
}

// 두선분으로 이루어진 내각을 절대값으로 리턴한다.
double GetInteriorAngle(FPOINT start,FPOINT middle,FPOINT end)
{
	double fAngle; // Radian 단위임
	double fX;
	double fY;

	fX=(start.x-middle.x)*(end.x-middle.x)+(start.y-middle.y)*(end.y-middle.y);
	fY=(start.x-middle.x)*(end.y-middle.y)-(start.y-middle.y)*(end.x-middle.x);

	if ( fX == 0. && fY == 0.)
		fAngle = 0.0;
	else
	{
		fAngle = atan2(fY,fX);
		if (fAngle <0.)
			fAngle = fAngle + 2. * PI;
	}
	
	if(GetVectorDir(start,middle,end) == CW)
		fAngle = 2. * PI - fAngle;
		
	fAngle = fAngle/PI*180.; //  Degree단위로 
	return fAngle;
}


// 두선분으로 이루어진 내각을 절대값으로 리턴한다.
double GetInteriorAngle(FPOINT3D start,FPOINT3D middle,FPOINT3D end)
{
	double fAngle; // Radian 단위임
	double fX;
	double fY;

	fX=(start.x-middle.x)*(end.x-middle.x)+(start.y-middle.y)*(end.y-middle.y);
	fY=(start.x-middle.x)*(end.y-middle.y)-(start.y-middle.y)*(end.x-middle.x);

	if ( fX == 0. && fY == 0.)
		fAngle = 0.0;
	else
	{
		fAngle = atan2(fY,fX);
		if (fAngle <0.)
			fAngle = fAngle + 2. * PI;
	}
	
	if(GetVectorDir(start,middle,end) == CW)
		fAngle = 2. * PI - fAngle;
		
	fAngle = fAngle/PI*180.; //  Degree단위로 
	return fAngle;
}

// 두 선분이 교차하는지를 조사.
// 교차하면 1을 그렇지 않으면 -1을 리턴한다.
// 만약 한선분의 끝점이 다른 선분상에 존재하면 0을 리턴한다.
int CheckIntersect(FLINE fLine1,FLINE fLine2)
{
	//C로 배우는 알고리즘,이재규,도처출판 세화,
	int r123,r124,r341,r342;

	if(fLine1.p1.x > fLine1.p2.x)
	{
		swap(fLine1.p1.x,fLine1.p2.x);
		swap(fLine1.p1.y,fLine1.p2.y);
	}

	if(fLine2.p1.x > fLine2.p2.x)
	{
		swap(fLine2.p1.x,fLine2.p2.x);
		swap(fLine2.p1.y,fLine2.p2.y);
	}

	r123 = GetVectorDir(fLine1.p1,fLine1.p2,fLine2.p1);
	r124 = GetVectorDir(fLine1.p1,fLine1.p2,fLine2.p2);
	r341 = GetVectorDir(fLine2.p1,fLine2.p2,fLine1.p1);
	r342 = GetVectorDir(fLine2.p1,fLine2.p2,fLine1.p2);

	if(r123 * r124 < 0 && r341 * r342 < 0) return 1; // 교차하는 경우

	if(r123 == 0 && r124 == 0) // 두 선분이 같은 방향일 경우
	{
		if(!(fLine2.p1.x > fLine1.p2.x || fLine1.p1.x > fLine2.p2.x))
			return 0;
		else
			return -1;
	}

	if(r123 == 0)
	{
		if(fLine1.p1.x <= fLine2.p1.x && fLine2.p1.x < fLine1.p2.x)
			return 0;
		else return -1;
	}

	if(r124 == 0)
	{
		if(fLine1.p1.x <= fLine2.p2.x && fLine2.p2.x < fLine1.p2.x)
			return 0;
		else return -1;
	}

	if(r341 == 0)
	{
		if(fLine2.p1.x <= fLine1.p1.x && fLine1.p1.x < fLine2.p2.x)
			return 0;
		else return -1;
	}
	
	if(r342 == 0)
	{
		if(fLine2.p1.x <= fLine1.p2.x && fLine1.p2.x < fLine2.p2.x)
			return 0;
		else return -1;
	}
	return -1;
}


BOOL GetIntersectPoint(FPOINT fPt1,FPOINT fPt2,FPOINT fPt3,FPOINT fPt4,FPOINT &fPt)
{
	double t;
    double s; 
    double under = (fPt4.y-fPt3.y)*(fPt2.x-fPt1.x)-(fPt4.x-fPt3.x)*(fPt2.y-fPt1.y);
    if(under==0)
		return FALSE; // 두 선분은 평행하다.

    double _t = (fPt4.x-fPt3.x)*(fPt1.y-fPt3.y) - (fPt4.y-fPt3.y)*(fPt1.x-fPt3.x);
    double _s = (fPt2.x-fPt1.x)*(fPt1.y-fPt3.y) - (fPt2.y-fPt1.y)*(fPt1.x-fPt3.x); 

    t = _t/under;
    s = _s/under; 

    if(t<0.0 || t>1.0 || s<0.0 || s>1.0)
		return FALSE; // 두 선분은 교차하지 않는다.
    if(_t==0 && _s==0)
		return FALSE; // 두 선분은 동일한 선이다

	fPt.x = fPt1.x + t * (double)(fPt2.x-fPt1.x);
    fPt.y = fPt1.y + t * (double)(fPt2.y-fPt1.y);

	return TRUE;
}

BOOL GetIntersectPoint(FPOINT3D fPt1,FPOINT3D fPt2,FPOINT3D fPt3,FPOINT3D fPt4,FPOINT3D &fPt)
{
	double t;
    double s; 
    double under = (fPt4.y-fPt3.y)*(fPt2.x-fPt1.x)-(fPt4.x-fPt3.x)*(fPt2.y-fPt1.y);
    if(under==0)
		return FALSE; // 두 선분은 평행하다.

    double _t = (fPt4.x-fPt3.x)*(fPt1.y-fPt3.y) - (fPt4.y-fPt3.y)*(fPt1.x-fPt3.x);
    double _s = (fPt2.x-fPt1.x)*(fPt1.y-fPt3.y) - (fPt2.y-fPt1.y)*(fPt1.x-fPt3.x); 

    t = _t/under;
    s = _s/under; 

    if(t<0.0 || t>1.0 || s<0.0 || s>1.0)
		return FALSE; // 두 선분은 교차하지 않는다.
    if(_t==0 && _s==0)
		return FALSE; // 두 선분은 동일한 선이다

	fPt.x = fPt1.x + t * (double)(fPt2.x-fPt1.x);
    fPt.y = fPt1.y + t * (double)(fPt2.y-fPt1.y);

	return TRUE;
}

BOOL GetIntersectPoint(FLINE fLine1,FLINE fLine2,FPOINT &fPt)
{
	return GetIntersectPoint(fLine1.p1,fLine1.p2,fLine2.p1,fLine2.p2,fPt);
}

BOOL GetIntersectPoint(FLINE3D fLine1,FLINE3D fLine2,FPOINT3D &fPt)
{
	return GetIntersectPoint(fLine1.p1,fLine1.p2,fLine2.p1,fLine2.p2,fPt);
}

void SaveLog(CString strMsg, int nType)
{
	CSafeLock lock(&g_LogLock);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	//GetModuleFileName(NULL, szPath, MAX_PATH);
	//pszPos = _tcsrchr(szPath, '\\');

	//*pszPos = NULL;

	_stprintf(szPath, _T("C:\R2RSet\Log"));
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, _T("C:\R2RSet\Log\Global"));
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	file.Write(cameraKey, nLenth);
	file.Flush();
	file.Close();
}

BOOL DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
