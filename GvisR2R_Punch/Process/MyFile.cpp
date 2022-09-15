// MyFile.cpp : implementation file
//

#include "stdafx.h"
//#include "gvisr2r.h"
#include "MyFile.h"
#include "../Global/GlobalFunc.h"
#include "../Global/GlobalDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//************************************************************************************************************

class CFExeption
{
public:
	CFExeption(DWORD dwErrCode);
	CFExeption(CString sErrText);
	CString GetErrorText() {return m_sError;}
	DWORD GetErrorCode() {return m_dwError;}

private:
	CString m_sError;
	DWORD m_dwError;
};

CFExeption::CFExeption(DWORD dwErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			      NULL, dwErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	m_sError = (LPTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
	m_dwError = dwErrCode;
}


CFExeption::CFExeption(CString sErrText)
{
	m_sError = sErrText;
	m_dwError = 0;
}

//************************************************************************************************************


/////////////////////////////////////////////////////////////////////////////
// CMyFile

CMyFile::CMyFile(CWnd* pParent)
{
	m_pParent = pParent;
	m_sError = _T("");
	
// 	if(!Create(NULL, _T("MyFile"), WS_CHILD, CRect(0,0,0,0), m_pParent, (UINT)this))
// 		AfxMessageBox(_T("CMyFile::Create() Failed!!!"));
}

CMyFile::~CMyFile()
{
}


BEGIN_MESSAGE_MAP(CMyFile, CWnd)
	//{{AFX_MSG_MAP(CMyFile)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyFile message handlers

// 파일을 copy한다.
BOOL CMyFile::Copy(CString sSource, CString sDest)
{
	if (CheckSelfCopy(sSource, sDest)) return true;
	bool bRes;
	try
	{
		DoCopy(sSource, sDest);
		bRes = true;
	}
	catch(CFExeption* e)
	{
		m_sError = e->GetErrorText();
		m_dwError = e->GetErrorCode();
		delete e;
		if (m_dwError == 0) bRes = true;
		bRes = false;
	}
	m_iRecursionLimit = -1;
	return bRes;
}

// 소스폴더를 그대로 그위치에 copy하는지 확인.
BOOL CMyFile::CheckSelfCopy(CString sSource, CString sDest)
{
	bool bRes = false;
	if (CheckPath(sSource) == PATH_IS_FOLDER)
	{
		CString sTmp = sSource;
		int pos = sTmp.ReverseFind('\\');
		if (pos != -1)
		{
			sTmp.Delete(pos, sTmp.GetLength() - pos);
			if (sTmp.CompareNoCase(sDest) == 0) bRes = true;
		}
	}
	return bRes;
}

// Path가 폴더인지 파일인지 그리고 존재하는지 확인.
int CMyFile::CheckPath(CString sPath)
{
	DWORD dwAttr = GetFileAttributes(sPath);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}

// Path뒤에 "\\"을 한번만 붙임.
void CMyFile::PreparePath(CString &sPath)
{
	if(sPath.Right(1) != "\\") sPath += "\\";
}

// Path에서 마지막 폴더 명을 Parsing하고, 나머지 Path를 참조한다.
CString CMyFile::ParseFolderName(CString sPathName, CString &sRemainPathName)
{
	CString sFolderName = sPathName;
	int pos = sFolderName.ReverseFind('\\');
	if (pos != -1) 
	{
		sFolderName.Delete(pos, sFolderName.GetLength() - pos);
		sRemainPathName = sFolderName;
	}

	pos = sFolderName.ReverseFind('\\');
	if (pos != -1) 
	{
		sFolderName = sFolderName.Right(sFolderName.GetLength() - pos - 1);
		sRemainPathName.Delete(pos, sRemainPathName.GetLength() - pos);
	}
	else 
	{
		sRemainPathName = sFolderName;
		sFolderName.Empty();
	}

	return sFolderName;
}

//반복하여 sDest의 하위 폴더를 Count한다.
void CMyFile::CheckSelfRecursion(CString sSource, CString sDest)
{
	if (sDest.Find(sSource) != -1)
	{
		int i = 0, count1 = 0, count2 = 0;
		for(i = 0; i < sSource.GetLength(); i ++)	if (sSource[i] == '\\') count1 ++;
		for(i = 0; i < sDest.GetLength(); i ++)	if (sDest[i] == '\\') count2 ++;
		if (count2 >= count1) m_iRecursionLimit = count2 - count1;
	}
}

// 파일과 폴더를 구분하여 copy한다.
void CMyFile::DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy)
{
	CString sRemainPathName, sFolderName;

	if(sDest.Find(_T("."),0) != -1)
	{
		sFolderName = ParseFolderName(sDest, sRemainPathName);
		PreparePath(sRemainPathName);
		sDest = sRemainPathName + sFolderName;
	}

	CheckSelfRecursion(sSource, sDest);
	// source not found
	if (CheckPath(sSource) == PATH_NOT_FOUND)
	{
		CString sError = sSource + CString(" not found");
		throw new CFExeption(sError);
	}
	// dest not found
	if (CheckPath(sDest) == PATH_NOT_FOUND)
	{
		CString sError = sDest + CString(" not found");
		throw new CFExeption(sError);
	}
	// folder to file
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FILE) 
	{
		throw new CFExeption(_T("Wrong operation"));
	}
	// folder to folder
	if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		CFileFind ff;
		CString sError = sSource + CString(" not found");
		PreparePath(sSource);
		PreparePath(sDest);
		sSource += "*.*";
		if (!ff.FindFile(sSource)) 
		{
			ff.Close();
			throw new CFExeption(sError);
		}
		if (!ff.FindNextFile()) 
		{
			ff.Close();
			throw new CFExeption(sError);
		}
		sFolderName = ParseFolderName(sSource, sRemainPathName);
		if (!sFolderName.IsEmpty()) // the source is not drive
		{
			sDest += sFolderName;
			PreparePath(sDest);
			if (!CreateDirectory(sDest, NULL))
			{
				DWORD dwErr = GetLastError();
				if (dwErr != 183)
				{
					ff.Close();
					throw new CFExeption(dwErr);
				}
			}
		}
		ff.Close();
		DoFolderCopy(sSource, sDest, bDelteAfterCopy);
	}
	// file to file
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FILE) 
	{
		DoFileCopy(sSource, sDest);
	}
	// file to folder
	if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FOLDER) 
	{
		PreparePath(sDest);
		char drive[MAX_PATH], dir[MAX_PATH], name[MAX_PATH], ext[MAX_PATH];
		char* pRtn = NULL;
		_splitpath(pRtn=StringToChar(sSource), drive, dir, name, ext);
		sDest = sDest + CString(name) + CString(ext);
		DoFileCopy(sSource, sDest);
		if(pRtn)
			delete pRtn;
	}
}

// 파일과 폴더를 구분하여 copy한다.
void CMyFile::DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy)
{
	CFileFind ff;
	CString sPathSource = sSourceFolder;
	BOOL bRes = ff.FindFile(sPathSource);
	while (bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory()) // source is a folder
		{
			if (m_iRecursionLimit == 0) continue;
			sPathSource = ff.GetFilePath() + CString("\\") + CString("*.*");
			CString sPathDest = sDestFolder + ff.GetFileName() + CString("\\");
			if (CheckPath(sPathDest) == PATH_NOT_FOUND) 
			{
				if (!CreateDirectory(sPathDest, NULL))
				{
					ff.Close();
					throw new CFExeption(GetLastError());
				}
			}
			if (m_iRecursionLimit > 0) m_iRecursionLimit --;
			DoFolderCopy(sPathSource, sPathDest, bDelteAfterCopy);
		}
		else // source is a file
		{
			CString sNewFileName = sDestFolder + ff.GetFileName();
			DoFileCopy(ff.GetFilePath(), sNewFileName, bDelteAfterCopy);
		}
	}
	ff.Close();
}

// 파일이 존재하는지 확인한다.
bool CMyFile::IsFileExist(CString sPathName)
{
	HANDLE hFile;
	hFile = CreateFile(sPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return false;
	CloseHandle(hFile);
	return true;
}

// 파일을 copy한다.
void CMyFile::DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy)
{
	BOOL bOvrwriteFails = FALSE;
	if (!m_bOverwriteMode)
	{
		while (IsFileExist(sDestFile)) 
		{
			sDestFile = ChangeFileName(sDestFile);
		}
		bOvrwriteFails = TRUE;
	}
	if (!CopyFile(sSourceFile, sDestFile, bOvrwriteFails)) throw new CFExeption(GetLastError());
	if (bDelteAfterCopy)
	{
		DoDelete(sSourceFile);
	}
}

// 지울수 있는지 속성을 파악한다.
bool CMyFile::CanDelete(CString sPathName)
{
	DWORD dwAttr = GetFileAttributes(sPathName);
	if (dwAttr == -1) return false;
	if (dwAttr & FILE_ATTRIBUTE_READONLY)
	{
		if (m_bAskIfReadOnly)
		{
			CString sTmp = sPathName;
			int pos = sTmp.ReverseFind('\\');
			if (pos != -1) sTmp.Delete(0, pos + 1);
			CString sText = sTmp + CString(" is read olny. Do you want delete it?");
			int iRes = MessageBox(sText, _T("Warning"), MB_YESNOCANCEL | MB_ICONQUESTION);
			switch (iRes)
			{
				case IDYES:
				{
					if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
					return true;
				}
				case IDNO:
				{
					return false;
				}
				case IDCANCEL:
				{
					m_bAborted = true;
					throw new CFExeption(0);
					return false;
				}
			}
		}
		else
		{
			if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
			return true;
		}
	}
	return true;
}

// 파일이나 폴더 그리고 그하위 폴더까지 지운다.
BOOL CMyFile::DeleteFolerOrFile(CString strFolerOrFilePath)
{
	CFileFind finder;
	if(finder.FindFile(strFolerOrFilePath))
	{
		DoDelete(strFolerOrFilePath);
		return TRUE;
	}
	else
		return FALSE;
}

// 파일이나 폴더 그리고 그하위 폴더까지 지운다.
void CMyFile::DoDelete(CString sPathName)
{
	CFileFind ff;
	CString sPath = sPathName;

	if (CheckPath(sPath) == PATH_IS_FILE)
	{
		if (!CanDelete(sPath)) 
		{
			m_bAborted = true;
			return;
		}
		if (!DeleteFile(sPath)) throw new CFExeption(GetLastError());
		return;
	}

	PreparePath(sPath);
	sPath += "*.*";

	BOOL bRes = ff.FindFile(sPath);
	while(bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots()) continue;
		if (ff.IsDirectory())
		{
			sPath = ff.GetFilePath();
			DoDelete(sPath);
		}
		else DoDelete(ff.GetFilePath());
	}
	ff.Close();
	if (!RemoveDirectory(sPathName) && !m_bAborted) 
	{
		//throw new CFExeption(GetLastError());
	}
}

// 같은 파일이 존재하면 새로 copy된 파일 앞에 "Copy of ", 그리고 매 반복시에는 "Copy (1) of "의 (?)에 반복수를 표시.
CString CMyFile::ChangeFileName(CString sFileName)
{
	CString sName, sNewName, sResult;
	char drive[MAX_PATH];
	char dir  [MAX_PATH];
	char name [MAX_PATH];
	char ext  [MAX_PATH];
	char* pRtn = NULL;
	_splitpath(pRtn=StringToChar(sFileName), drive, dir, name, ext);
	sName = name;

	int pos = sName.Find(_T("Copy "));
	if (pos == -1)
	{
		sNewName = CString("Copy of ") + sName + CString(ext);
	}
	else
	{
		int pos1 = sName.Find('(');
		if (pos1 == -1)
		{
			sNewName = sName;
			sNewName.Delete(0, 8);
			sNewName = CString("Copy (1) of ") + sNewName + CString(ext);
		}
		else
		{
			CString sCount;
			int pos2 = sName.Find(')');
			if (pos2 == -1)
			{
				sNewName = CString("Copy of ") + sNewName + CString(ext);
			}
			else
			{
				sCount = sName.Mid(pos1 + 1, pos2 - pos1 - 1);
				sName.Delete(0, pos2 + 5);
				int iCount = _tstoi(sCount);
				iCount ++;
				sNewName.Format(_T("%s%d%s%s%s"), _T("Copy ("), iCount, _T(") of "), (LPCTSTR)sName, ext);
			}
		}
	}

	sResult = CString(drive) + CString(dir) + sNewName;

	if(pRtn)
		delete pRtn;

	return sResult;
}

void CMyFile::ShowError() //Show error message
{
	MessageBox(m_sError, _T("Error"), MB_OK | MB_ICONERROR);
}

/////////////////////////////////////////////////////////////////////////////
// for R2R

int CMyFile::CopyPcrAll(CString sSrc, CString sDest)  // return : Serial
{
	CString sFolder, sRemain, sSrcFolder, sFile;
	CString sLot, sFilePath, sDestFolder;
	CFileFind cFile;
	BOOL bExist;
	int nPos;

	// 파일명을 얻음.
	nPos = sDest.ReverseFind('\\');
	sFile = sDest.Right(sDest.GetLength()-nPos-1);
	if(sFile.Find(_T("."), 0) != -1)
	{
		nPos = sFile.Find(_T("."), 0);
		sFile.Delete(nPos, sFile.GetLength() - nPos);
	}
	sFolder = ParseFolderName(sDest, sRemain);
	if(sFolder.IsEmpty()) return -1; // 폴더가 존재하지 않음.
	sDestFolder = sRemain + _T("\\") + sFolder + _T("\\");

	sFolder = ParseFolderName(sSrc, sRemain);
	if(sFolder.IsEmpty()) return -1; // 폴더가 존재하지 않음.
	sSrcFolder = sRemain + _T("\\") + sFolder + _T("\\");

	bExist = cFile.FindFile(sSrcFolder + _T("*.pcr"));
	if(!bExist)
	{
		return -2; // pcr파일이 존재하지 않음.
	}

	int nCopyed=0, nSerial;

	BeginWaitCursor();
	while(bExist)
	{

		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFile = cFile.GetFileName();
			
			// 파일 Path를 얻음.
			sFilePath = cFile.GetFilePath();

			m_sError = _T("No error");
			m_dwError = 0; // 에러코드 초기화.
			m_bAskIfReadOnly = FALSE; // File을 Read Only로 하지 않음.
			m_bOverwriteMode = TRUE; // File을 Over Write로.
			m_bAborted = FALSE; // 파일관련 처리를 Abort하지 않는다.
			m_iRecursionLimit = -1; // 하위폴더의 갯수를 초기화한다. 양수부터 의미 있음.

			sDest = sDestFolder + sFile;
			if (!Copy(sFilePath, sDest))
			{
				ShowError();
				return -3; // Copy 실패.
			}
			
			nCopyed++;
			nSerial = _tstoi(sFile);
		}
	}
	EndWaitCursor();
	
	if(nCopyed > 0)
		return nSerial;

	return 0;
}

void CMyFile::DelPcrAll(CString strPath)
{
	CString strFileName;
	CString strFilePath;
	CFileFind cFile;
	BOOL bExist;

	bExist = cFile.FindFile(strPath + _T("*.pcr"));
	if(!bExist)
		return;

	BeginWaitCursor();
	while(bExist)
	{
		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			strFileName = cFile.GetFileName();
			
			// 파일 Path를 얻음.
			strFilePath = cFile.GetFilePath();

			m_sError = _T("No error");
			m_dwError = 0; // 에러코드 초기화.
			m_bAskIfReadOnly = FALSE; // File을 Read Only로 하지 않음.
			m_bOverwriteMode = TRUE; // File을 Over Write로.
			m_bAborted = FALSE; // 파일관련 처리를 Abort하지 않는다.
			m_iRecursionLimit = -1; // 하위폴더의 갯수를 초기화한다. 양수부터 의미 있음.
		
			if (!DeleteFolerOrFile(strFilePath))
			{
				ShowError();
				return; // 실패.
			}
		}
	}
	EndWaitCursor();
	
	return; // Sucess.
}

BOOL CMyFile::ChkLotEnd(CString sPath)
{
	// 파일을 읽는다.
	char FileD[MAX_PATH];
	char *FileData;
	CString strFileData;
	size_t nFileSize, nRSize;
	int nTemp;
	CString strFileName;
	FILE *fp;

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize+1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free( FileData );
	}
	else
		return FALSE;

	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;
	
	//strHeaderErrorInfo
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);		// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	strFileData.Delete(0, nTemp+1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp+1);
	nFileSize = nFileSize - nTemp - 1;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp+1);
	nFileSize = nFileSize - nTemp - 1;

	// Lot
	nTemp = strFileData.Find('\n', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp+1);
	nFileSize = nFileSize - nTemp - 1;

	//strTotalBadPieceNum = strFileData;
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp+1);
	nFileSize = nFileSize - nTemp - 1;

	
	if(_tstoi(strHeaderErrorInfo) == -2)
		return TRUE;
	
	return FALSE;
}

