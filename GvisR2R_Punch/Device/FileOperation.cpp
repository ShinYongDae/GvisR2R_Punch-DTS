#include "StdAfx.h"
#include "FileOperation.h"

#include <stdio.h>
#include <io.h>

#ifdef _UNICODE
#define Access(p0, p1) _waccess(p0, p1)
#else
#define Access(p0, p1) _access(p0, p1)
#endif

BOOL g_bInitSecurity = 0;
BOOL CWindowVersion::GetOsVersion()
{
	CString sOS = m_HardwareInfo.GetItemValueOs(OsCaption);
	//AfxMessageBox(sOS);

	// //100828 hsm windows 10 add
	if (sOS.Find(_T("Windows 10")) >= 0)
		m_nOSVer = osWin10;
	else if(sOS.Find(_T("Windows 7")) >= 0)
		m_nOSVer = osWin7;
	else if(sOS.Find(_T("Windows XP")) >= 0)
		m_nOSVer = osWinXp;
	else if(sOS.Find(_T("Windows 2000")) >= 0)
		m_nOSVer = osWin2k;
	else
	{
		m_nOSVer = osUnknown;
		m_strErrorMsg.Format(_T("warnning : Unknown OS version.\r\n"));
		return FALSE;
	}

	return TRUE;
}

 tpOs CWindowVersion::GetOsInfo()
{
	return m_nOSVer;
}

CWindowVersion::CWindowVersion()
{
	if(!m_HardwareInfo.GetWmiInfoOs())
	{
		m_nOSVer = osUnknown;
		//		AfxMessageBox(_T("Failed - m_HardwareInfo.UpdateData()."));
		m_strErrorMsg = _T("Failed - m_HardwareInfo.UpdateData()."); 
	}
	else
		GetOsVersion();	

	if(m_nOSVer > osWinXp)
	{
		HRESULT hr;

		// COM Start....
		//Specify the appropriate COM threading model for your application. COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE
		hr=CoInitializeEx(NULL, COINIT_MULTITHREADED);

		if(FAILED(hr))
		{
			hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		}


		if (SUCCEEDED(hr))
		{
			m_bCoInitialize = TRUE;

			if((hr=CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
				RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0)) != S_OK)
			{
				//				AfxMessageBox(_T("Failed-CoInitializeSecurity()."));
				m_strErrorMsg = _T("Failed-CoInitializeSecurity()."); 
			}
		}
		else
			m_bCoInitialize = FALSE;
	}
	else
		m_bCoInitialize = FALSE;
}

CWindowVersion::~CWindowVersion()
{
	//if(m_nOS > osWinXp)
	//{
		if(m_bCoInitialize)
			CoUninitialize();
	//}
}


CFileOperation::CFileOperation(tpOs nOSVer,CString sPath)
{
	m_sPath = sPath;
	m_nOS = nOSVer;
	m_strErrorMsg.Empty();
/*
	if(!m_HardwareInfo.GetWmiInfoOs())
	{
		m_nOS = osUnknown;
//		AfxMessageBox(_T("Failed - m_HardwareInfo.UpdateData()."));
		m_strErrorMsg = _T("Failed - m_HardwareInfo.UpdateData()."); 
	}
	else
		GetOsVersion();	

	if(m_nOS > osWinXp)
	{
		HRESULT hr;

		// COM Start....
		//Specify the appropriate COM threading model for your application.
		hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			m_bCoInitialize = TRUE;

			if((hr=CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
				RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0)) != S_OK)
			{
//				AfxMessageBox(_T("Failed-CoInitializeSecurity()."));
				m_strErrorMsg = _T("Failed-CoInitializeSecurity()."); 
			}
		}
		else
			m_bCoInitialize = FALSE;
	}
	else
		m_bCoInitialize = FALSE;
*/
}


CFileOperation::~CFileOperation()
{
/*
	if(m_nOS > osWinXp)
	{
		if(m_bCoInitialize)
			CoUninitialize();
	}
*/
}

BOOL CFileOperation::CopyFileRepeat(CString strSrc,CString strDst,int nIterMax, BOOL bOverWrite)
{
	CFileFind finder;


	if (finder.FindFile(strSrc) == FALSE)
	{
		return FALSE;
	}
	//DoDeleteFile(strDst);

	BOOL bCopy=FALSE;
	int nIter=0;

	while(bCopy==FALSE)
	{
		if(bCopy=DoCopyFile(strSrc,strDst,bOverWrite))
		{
			break;
		}
		else
			nIter++;



		CString err = GetErrorMsg();

		//CString Log;

		//Log.Format(_T("Fail : CopyFileRepeat[%d] %s -> %s Error:%s"),nIter,strSrc,strDst,err);
		//SaveLog(Log);

		if(nIter>nIterMax)
		{
			bCopy=FALSE;
			break;
		}

		Sleep(100);
	}

	return bCopy;
}
/*
BOOL CFileOperation::GetOsVersion()
{
	CString sOS = m_HardwareInfo.GetItemValueOs(OsCaption);
	//AfxMessageBox(sOS);

	if(sOS.Find(_T("Windows 7")) >= 0)
		m_nOS = osWin7;
	else if(sOS.Find(_T("Windows XP")) >= 0)
		m_nOS = osWinXp;
	else if(sOS.Find(_T("Windows 2000")) >= 0)
		m_nOS = osWin2k;
	else
	{
		m_nOS = osUnknown;
		m_strErrorMsg.Format(_T("warnning : Unknown OS version.\r\n"));
		return FALSE;
	}

	return TRUE;
}
*/
BOOL CFileOperation::CheckPath(CString &sPath)
{
	if(sPath.IsEmpty())
	{
		if(!m_sPath.IsEmpty())
			sPath = m_sPath;
		else
		{
//			AfxMessageBox(_T("Path is empty."));
			m_strErrorMsg = _T("Path is empty.");
			return FALSE;
		}
	}

	return TRUE;
}


// File Handling

BOOL CFileOperation::DoCreate(CString sPath, int nAttr, BOOL bFolderOnly)
{
	if(!CheckPath(sPath))
		return FALSE;

	CString sTemp, sMsg, sLastName;
	TCHAR ch;

	int i, nLen;

	i = 0;
	nLen = sPath.GetLength();

	sTemp = sPath.Left(2);
	if(sTemp.CompareNoCase(_T("\\\\"))==0)		//remote computer name
	{
		i = sPath.Find(_T("\\"),2)+1;
		sTemp = sPath.Left(i);
	}
	else
		sTemp=_T("");


	for (; i < nLen ; i++ )
	{
		ch = sPath.GetAt(i);
		sTemp += ch;
		sLastName += ch;
		if ( ch == _T('\\') || ch == _T('/') )
		{
			sLastName = _T("");
			if (!PathFileExists(sTemp))
			{
				if (!::CreateDirectory(sTemp,NULL))
				{
					if (GetLastError() != ERROR_ALREADY_EXISTS )
					{
						m_strErrorMsg.Format(_T("Cannot create the folder!!\n%s"), sTemp);
						return FALSE;
				}
			}
		}
	}
	}

	if(sLastName.IsEmpty())
		return TRUE;

	//ch = _T('.');
	//if((i=sLastName.ReverseFind(ch)) < 0)
	//{
	//	if (!PathFileExists(sTemp))
	//	{
	//		if (!::CreateDirectory(sTemp,NULL))
	//		{
	//			if (GetLastError() != ERROR_ALREADY_EXISTS )
	//				return FALSE;
	//		}
	//	}
	//}
	if(nAttr == Folder)
	{
		if (!PathFileExists(sTemp))
		{
			if (!::CreateDirectory(sTemp,NULL))
			{
				if (GetLastError() != ERROR_ALREADY_EXISTS )
				{
					m_strErrorMsg.Format(_T("Cannot create the folder!!\n%s"), sTemp);
					return FALSE;
			}
		}
	}
	}

	if(IsFolder(sPath))
		return TRUE;

	if(!bFolderOnly && nAttr == File)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE; 
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
		hFile = CreateFile(sPath, dwDesiredAccess, dwShareMode, NULL, CREATE_NEW, 0, NULL);  
		if(hFile == INVALID_HANDLE_VALUE)  
		{
			m_strErrorMsg.Format(_T("error : failed to create file.\r\n%s"), sPath);
			return FALSE;  
		}

		CloseHandle(hFile);
	}

	return TRUE;
}


void CFileOperation::DeleteFileInFolder(CString strFolderPath)
{

	BOOL bRes;
	CFileFind ff;

	if (CheckPathisFile(strFolderPath) == PATH_IS_FILE)
	{
		DoDeleteFile(strFolderPath);
		return;
	}

	if(strFolderPath.Right(1) != "\\")
		strFolderPath += "\\";

	strFolderPath += "*.*";

	bRes = ff.FindFile(strFolderPath);

	while(bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			strFolderPath = ff.GetFilePath();
			DeleteFileInFolder(strFolderPath);
		}
		else
			DeleteFileInFolder(ff.GetFilePath());
	}

	ff.Close();
}

int CFileOperation::CheckPathisFile(CString strPath)
{
	DWORD dwAttr = GetFileAttributes(strPath);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}


BOOL CFileOperation::DoCreateFolder(CString srcPath, BOOL bNew)							// 폴더 그리고 폴더 이하 전부 복사					
{
	m_strErrorMsg.Empty();
	if(!IsFolderAttr(srcPath, Folder))
		return FALSE;

	if(bNew)
		DoDelete(srcPath);

	return DoCreate(srcPath, Folder, TRUE);
}

BOOL CFileOperation::DoDelete(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	CString sMsg;
	BOOL bRtn=FALSE;

	CString sTemp=_T(""), strPath=_T("");
	int nPos[3] = {-1};

	nPos[0] = sPath.ReverseFind(_T('\\'));
	nPos[1] = sPath.Find(_T("*.*"), nPos[0]);
	sTemp = sPath.Right(sPath.GetLength()-nPos[0]-1);
	sTemp.Trim();
	
	if(nPos[0] < nPos[1] && sTemp == _T("*.*")) // find "*.*"
		strPath = sPath.Left(nPos[0]);
	else // not find "*.*"
	{
		nPos[2] = sTemp.ReverseFind(_T('*'));
		if(nPos[2] < 0)
			strPath = sPath;
		else
		{
			bRtn = ApiDelete(sPath);
			return bRtn;
		}
	}

	if(IsFolderAttr(strPath))
	{
		if(strPath.Right(1) == _T("\\"))
			strPath = strPath.Left(strPath.GetLength()-1);
	}

//	bRtn = ApiDelete(strPath);
	
	if(m_nOS == osUnknown)
		bRtn = ApiDelete(strPath);
	else if(m_nOS <= osWinXp)
	{
		bRtn = ShDelete(strPath);
		if(IsFolder(strPath))
		{
			if(CanIfind(strPath))
				bRtn = RemoveDirectory(strPath);
		}
	}
	else
	{
		bRtn = IDelete(strPath);
		if(bRtn)
		{
			if(IsFolder(strPath))
			{
				if(CanIfind(strPath))
				{
					bRtn = RemoveDirectory(strPath);
					if(!bRtn)
					{
						if(!IDelete(strPath))
						{
							m_strErrorMsg.Format(_T("Failed to delete folder.\r\n%s"), strPath);
						}
//						AfxMessageBox(sMsg);
					}
				}
			}
		}
		else
		{
			m_strErrorMsg.Format(_T("Failed to delete file or folder.\r\n%s"), strPath);
//			AfxMessageBox(sMsg);
		}
	}
	
	return bRtn;
}

BOOL CFileOperation::ApiDelete(CString sPath)
{
	BOOL bRtn=TRUE;

	CString strPath, sMsg, strTemp;
	strPath = sPath;

	if(IsFolderAttr(sPath))
	{
		if(strPath.Right(1) != _T("\\"))
			strPath += _T("\\");
		strPath += _T("*.*");
	}

	CFileFind ff;
	BOOL bFind = ff.FindFile(strPath);
	if(!bFind)
	{
		if(IsFolderAttr(sPath))
		{
			if(ff.FindFile(sPath))
				return RemoveDirectory(sPath); // remove the empty directory
		}

		return FALSE;
	}
	else
	{
		while(bFind) // until we find an entry
		{
			bFind = ff.FindNextFile();
			if(ff.IsDots())
				continue;

			if(ff.IsDirectory())// we have found a directory, recurse
			{
				strPath = ff.GetFilePath();
				
				if(DoDelete(strPath))
					continue;
				else
				{
					m_strErrorMsg.Format(_T("Failed to delete folder.\r\n%s"), strPath);
//					AfxMessageBox(m_strErrorMsg);
					bRtn = FALSE;
					return bRtn;
				}
			}
			else
			{
				strTemp=ff.GetFilePath();
				if(IsFileReadOnly(strTemp))
				{
					if(!SetFileReadWrite(strTemp))
					{
						m_strErrorMsg.Format(_T("Failed to set file attribute(ReadWrite).\r\n%s"), strTemp);
//						AfxMessageBox(m_strErrorMsg);
						bRtn = FALSE;
						continue;
					}
				}

				if(!CanIreadwrite(strTemp))
				{
					//AfxMessageBox(_T("Failed to set file attribute(ReadWrite)."));
					m_strErrorMsg.Format(_T("Failed to set file attribute(ReadWrite).\r\n%s"), strTemp);
//					AfxMessageBox(m_strErrorMsg);
					bRtn = FALSE;
					continue;
				}

				if(IsOpenedFile(strTemp))
				{
					//AfxMessageBox(_T("error : File is opened."));
					m_strErrorMsg.Format(_T("error : File is opened.\r\n%s"), strTemp);
//					AfxMessageBox(m_strErrorMsg);
					bRtn = FALSE;
					continue;
				}

				//if(!Delete(strTemp))
				if(!::DeleteFile(strTemp))
				{
					DWORD dwErrCode = GetLastError();
					//AfxMessageBox(_T("Failed to delete file."));
					m_strErrorMsg.Format(_T("Failed to delete file.\r\n%s"), strTemp);
//					AfxMessageBox(m_strErrorMsg);
					bRtn = FALSE;
					continue;
				}
			}
		}
		ff.Close();
	}

	if(CanIfind(sPath))
	{
		if(IsFolder(sPath) && bRtn)
			bRtn = RemoveDirectory(sPath); // remove the empty directory
	}

	return bRtn;
}

BOOL CFileOperation::Delete(CString sPath)
{
	BOOL bRtn = FALSE;
	
	if(m_nOS == osUnknown)
	{
		if(IsFolder(sPath))
		{
			if(CanIfind(sPath))
				bRtn = RemoveDirectory(sPath);
		}
		else
			bRtn = ::DeleteFile(sPath);
	}
	else if(m_nOS <= osWinXp)
	{
		bRtn = ShDelete(sPath);
		if(IsFolder(sPath))
		{
			if(CanIfind(sPath))
				bRtn = RemoveDirectory(sPath);
		}
	}
	else
	{
		bRtn = IDelete(sPath);
		if(IsFolder(sPath))
		{
			if(CanIfind(sPath))
				bRtn = RemoveDirectory(sPath);
		}
	}

	return bRtn;
}

BOOL CFileOperation::ShDelete(CString sPath) 
{
	int nStrSize = sPath.GetLength();
	TCHAR *pszFrom = new TCHAR[nStrSize+2]; 
#ifdef _UNICODE
	_tcscpy(pszFrom, sPath);
#else
	strcpy(pszFrom, sPath);
#endif
	pszFrom[nStrSize] = 0;
	pszFrom[nStrSize+1] = 0;

	SHFILEOPSTRUCT shfo={0};

	shfo.hwnd = NULL;
	shfo.wFunc = FO_DELETE;
	shfo.pFrom = pszFrom;

	shfo.fFlags = FOF_NOCONFIRMATION; // 작업 플래그 설정	FOF_NOCONFIRMMKDIR

	shfo.fAnyOperationsAborted = FALSE; 
	shfo.lpszProgressTitle = _T("Delete");

	BOOL bRtn = SHFileOperation(&shfo);

	delete [] pszFrom;
	
	if (shfo.fAnyOperationsAborted || bRtn)
		bRtn = FALSE;
	else
		bRtn = TRUE;

	return bRtn;
}

BOOL CFileOperation::Copy(CString srcPath, CString destPath, BOOL bOverWrite, int nAttr) // strFrom : File ; strTo : File 
{
	CString sUpFolderPath=_T("");

	if(m_nOS > osWinXp)
	{
		int nDelPos=0;
		TCHAR ch = _T('\\');

		if(destPath.Right(1) != ch)
		{
			nDelPos = destPath.ReverseFind(ch);
			sUpFolderPath = destPath.Left(nDelPos);
		}
		else
		{
			sUpFolderPath = destPath.Left(destPath.GetLength()-1);
			nDelPos = destPath.ReverseFind(ch);
			sUpFolderPath = destPath.Left(nDelPos);
		}
	}

	BOOL bRtn = FALSE;
	bRtn = CopyFile(srcPath, destPath, !bOverWrite); // bFailIfExists = !bOverWrite

/*
	if(m_nOS == osUnknown)
		bRtn = CopyFile(srcPath, destPath, !bOverWrite); // bFailIfExists = !bOverWrite
	else if(m_nOS <= osWinXp)
		bRtn = ShCopyFile(srcPath, destPath, bOverWrite);
	else
		bRtn = ICopyFile(srcPath, sUpFolderPath, destPath, bOverWrite);
*/
	if(!bOverWrite)
		return TRUE;

	return bRtn;
}

BOOL CFileOperation::ShCopyFile(CString srcPath, CString destPath, BOOL bOverWrite, int nAttr) 
{
	int nStrSizeF = srcPath.GetLength();
	int nStrSizeT = destPath.GetLength();
	TCHAR *pszFrom = new TCHAR[nStrSizeF+2];
	TCHAR *pszTo = new TCHAR[nStrSizeT+2];

#ifdef _UNICODE
	_tcscpy(pszFrom, srcPath);
	_tcscpy(pszTo, destPath);
#else
	strcpy(pszFrom, srcPath);
	strcpy(pszTo, destPath);
#endif
	pszFrom[nStrSizeF] = 0;
	pszFrom[nStrSizeF+1] = 0;
	pszTo[nStrSizeT] = 0;
	pszTo[nStrSizeT+1] = 0;

	SHFILEOPSTRUCT shfo={0};
	//ZeroMemory(&shfo, sizeof shfo);

	shfo.hwnd = NULL;//AfxGetMainWnd()->m_hWnd; //hWnd;
	shfo.wFunc = FO_COPY;
	shfo.pFrom = pszFrom;
	shfo.pTo = pszTo;
	shfo.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR; // 작업 플래그 설정	
	if(!bOverWrite)
		shfo.fFlags |= FOF_RENAMEONCOLLISION;

	shfo.lpszProgressTitle = _T("File Copy");

	BOOL bRtn = SHFileOperation(&shfo); 

	delete [] pszFrom;
	delete [] pszTo;

	if (shfo.fAnyOperationsAborted || bRtn)
		bRtn = FALSE;
	else
		bRtn = TRUE;

	return bRtn;
}
BOOL CFileOperation::ShMoveFile(CString srcPath, CString destPath, BOOL bOverWrite, int nAttr) 
{
	int nStrSizeF = srcPath.GetLength();
	int nStrSizeT = destPath.GetLength();
	TCHAR *pszFrom = new TCHAR[nStrSizeF+2];
	TCHAR *pszTo = new TCHAR[nStrSizeT+2];

#ifdef _UNICODE
	_tcscpy(pszFrom, srcPath);
	_tcscpy(pszTo, destPath);
#else
	strcpy(pszFrom, srcPath);
	strcpy(pszTo, destPath);
#endif
	pszFrom[nStrSizeF] = 0;
	pszFrom[nStrSizeF+1] = 0;
	pszTo[nStrSizeT] = 0;
	pszTo[nStrSizeT+1] = 0;

	SHFILEOPSTRUCT shfo={0};
	//ZeroMemory(&shfo, sizeof shfo);

	shfo.hwnd = NULL;//AfxGetMainWnd()->m_hWnd; //hWnd;
	shfo.wFunc = FO_MOVE;
	shfo.pFrom = pszFrom;
	shfo.pTo = pszTo;
	shfo.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR; // 작업 플래그 설정	
	if(!bOverWrite)
		shfo.fFlags |= FOF_RENAMEONCOLLISION;

	shfo.lpszProgressTitle = _T("File Move");

	BOOL bRtn = SHFileOperation(&shfo); 

	delete [] pszFrom;
	delete [] pszTo;

	if (shfo.fAnyOperationsAborted || bRtn)
		bRtn = FALSE;
	else
		bRtn = TRUE;

	return bRtn;
}

BOOL CFileOperation::IDelete(CString sPath) // 컴파일된 이 함수는 Windows Vista 이후만 가능, XP에선 실행파일은 실행 안됨. 
{
	if(m_nOS <= osWinXp)
		return TRUE;

	HRESULT hr; 
	IFileOperation* pfo = NULL; 
	
	LPCWSTR wszPath;
#ifdef _UNICODE
	wszPath = sPath;
#else
	USES_CONVERSION;
	wszPath = T2W(sPath.GetBuffer(0));
#endif
	
	hr=CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if(FAILED(hr))
		{
			m_strErrorMsg.Format(_T("error : failed to call CoInitializeEx().\r\n"));
			return FALSE; 
		}
	}
	if(FAILED(hr=CoCreateInstance(CLSID_FileOperation,0,CLSCTX_INPROC_SERVER,IID_IFileOperation,(void**)&pfo))) 
	{
		m_strErrorMsg.Format(_T("error : failed to call CoCreateInstance().\r\n"));
		CoUninitialize();
		return FALSE; 
	}

	DWORD dwFlag = FOF_NOCONFIRMATION; // 작업 플래그 설정

	if(FAILED(hr=pfo->SetOperationFlags(dwFlag))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call SetOperationFlags().\r\n"));
		CoUninitialize();
		return FALSE; 
	}

	IShellItem* pShellItem=NULL;	

	if(FAILED(hr=SHCreateItemFromParsingName(wszPath,NULL,IID_IShellItem,(void**)&pShellItem))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call SHCreateItemFromParsingName().\r\n"));
		CoUninitialize();
		return FALSE; 
	}

	if(FAILED(hr=pfo->DeleteItem(pShellItem,NULL))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call DeleteItem().\r\n"));
		CoUninitialize();
		return FALSE; 
	}

	if(FAILED(hr=pfo->PerformOperations())) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call PerformOperations().\r\n"));
		CoUninitialize();
		return FALSE; 
	}

	pfo->Release();
	CoUninitialize();

	return TRUE;
}

BOOL CFileOperation::ICopyFile(CString srcPath, CString destUpFolder, CString destPath, BOOL bOverWrite, int nAttr) // 컴파일된 이 함수는 Windows Vista 이후만 가능, XP에선 실행파일은 실행 안됨.  
{
	if(m_nOS <= osWinXp)
		return TRUE;

	HRESULT hr; 
	IFileOperation* pfo = NULL; 

	LPCWSTR wszSrcPath;
	LPCWSTR wszFolder;
	LPCWSTR wszDestPath;

#ifdef _UNICODE
	wszSrcPath = srcPath;
	wszFolder = destUpFolder;
	wszDestPath = destPath;
#else
	USES_CONVERSION;
	wszSrcPath = T2W(srcPath.GetBuffer(0));
	wszFolder = T2W(destUpFolder.GetBuffer(0));
	wszDestPath = T2W(destPath.GetBuffer(0));
#endif

	hr=CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if(FAILED(hr))
		{
			m_strErrorMsg.Format(_T("error : failed to call CoInitializeEx().\r\n"));
			return FALSE; 
		}
	}
	if(FAILED(hr=CoCreateInstance(CLSID_FileOperation,0,CLSCTX_INPROC_SERVER,IID_IFileOperation,(void**)&pfo))) 
	{
		m_strErrorMsg.Format(_T("error : failed to call CoCreateInstance().\r\n"));
			CoUninitialize();
		return FALSE;
	}

	DWORD dwFlag = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;	 // 작업 플래그 설정
	if(!bOverWrite)
		dwFlag |= FOF_RENAMEONCOLLISION;

	if(FAILED(hr=pfo->SetOperationFlags(dwFlag))) 
	{
		pfo->Release();
			CoUninitialize();
		m_strErrorMsg.Format(_T("error : failed to call SetOperationFlags().\r\n"));
		return FALSE; 
	}

	IShellItem* pShellItemFrom=NULL;	
	IShellItem* pShellItemTo=NULL;	

	if(FAILED(hr=SHCreateItemFromParsingName(wszSrcPath,NULL,IID_IShellItem,(void**)&pShellItemFrom))) 
	{
		pfo->Release();
			CoUninitialize();
		m_strErrorMsg.Format(_T("error : failed to call SHCreateItemFromParsingName().\r\n"));
		return FALSE; 
	}

	if(FAILED(hr=SHCreateItemFromParsingName(wszFolder,NULL,IID_IShellItem,(void**)&pShellItemTo))) 
	{
		pfo->Release();
			CoUninitialize();
		m_strErrorMsg.Format(_T("error : failed to call SHCreateItemFromParsingName().\r\n"));
		return FALSE; 
	}

	if(FAILED(hr=pfo->CopyItem(pShellItemFrom,pShellItemTo,wszDestPath,NULL))) 
	{
		pfo->Release();
			CoUninitialize();
		m_strErrorMsg.Format(_T("error : failed to call CopyItem().\r\n"));
		return FALSE;
	}

	if(FAILED(hr=pfo->PerformOperations())) 
	{
		pfo->Release();
			CoUninitialize();
		m_strErrorMsg.Format(_T("error : failed to call PerformOperations().\r\n"));
		return FALSE; 
	}

	pfo->Release();
	CoUninitialize();

	return TRUE;
}
BOOL CFileOperation::IMoveFile(CString srcPath, CString destUpFolder, CString destPath, BOOL bOverWrite, int nAttr) // 컴파일된 이 함수는 Windows Vista 이후만 가능, XP에선 실행파일은 실행 안됨.  
{
	if(m_nOS <= osWinXp)
		return TRUE;

	HRESULT hr; 
	IFileOperation* pfo = NULL; 

	LPCWSTR wszSrcPath;
	LPCWSTR wszFolder;
	LPCWSTR wszDestPath;

#ifdef _UNICODE
	wszSrcPath = srcPath;
	wszFolder = destUpFolder;
	wszDestPath = destPath;
#else
	USES_CONVERSION;
	wszSrcPath = T2W(srcPath.GetBuffer(0));
	wszFolder = T2W(destUpFolder.GetBuffer(0));
	wszDestPath = T2W(destPath.GetBuffer(0));
#endif

	hr=CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		hr=CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if(FAILED(hr))
		{
			m_strErrorMsg.Format(_T("error : failed to call CoInitializeEx().\r\n"));
			return FALSE; 
		}
	}

	if(FAILED(hr=CoCreateInstance(CLSID_FileOperation,0,CLSCTX_INPROC_SERVER,IID_IFileOperation,(void**)&pfo))) 
	{
		m_strErrorMsg.Format(_T("error : failed to call CoCreateInstance().\r\n"));
			CoUninitialize();
		return FALSE;
	}

	DWORD dwFlag = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;	 // 작업 플래그 설정
	if(!bOverWrite)
		dwFlag |= FOF_RENAMEONCOLLISION;

	if(FAILED(hr=pfo->SetOperationFlags(dwFlag))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call SetOperationFlags().\r\n"));
			CoUninitialize();
		return FALSE; 
	}

	IShellItem* pShellItemFrom=NULL;	
	IShellItem* pShellItemTo=NULL;	

	if(FAILED(hr=SHCreateItemFromParsingName(wszSrcPath,NULL,IID_IShellItem,(void**)&pShellItemFrom))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call SHCreateItemFromParsingName().\r\n"));
			CoUninitialize();
		return FALSE; 
	}

	if(FAILED(hr=SHCreateItemFromParsingName(wszFolder,NULL,IID_IShellItem,(void**)&pShellItemTo))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call SHCreateItemFromParsingName().\r\n"));
			CoUninitialize();
		return FALSE; 
	}

	if(FAILED(hr=pfo->MoveItem(pShellItemFrom,pShellItemTo,wszDestPath,NULL))) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call CopyItem().\r\n"));
			CoUninitialize();
		return FALSE;
	}

	if(FAILED(hr=pfo->PerformOperations())) 
	{
		pfo->Release();
		m_strErrorMsg.Format(_T("error : failed to call PerformOperations().\r\n"));
			CoUninitialize();
		return FALSE; 
	}

	pfo->Release();
	CoUninitialize();

	return TRUE;
}
//BOOL CFileOperation::DoRename(CString sPathTo, CString sPathFrom)
//{
//	if(!CheckPath(sPathFrom))
//		return FALSE;
//
//	if(!CanIfind(sPathFrom))
//		return FALSE;
//
//	if(sPathTo.IsEmpty())
//		return FALSE;
//
//	return ShRename(sPathTo, sPathFrom);
//}
//
//BOOL CFileOperation::ShRename(CString sPathTo, CString sPathFrom)
//{
//	SHFILEOPSTRUCT shfo;
//
//	TCHAR pszFrom[1024] = {0};
//	TCHAR pszTo[1024] = {0};
//
//	WORD wFlag = 0;
//
//	_tcscpy(pszFrom, sPathFrom);
//	_tcscpy(pszTo, sPathTo);
//
//	pszFrom[lstrlen(pszFrom) + 1] = 0;
//
//	shfo.hwnd = NULL;
//	shfo.wFunc = FO_RENAME;
//	shfo.pFrom = pszFrom;
//	shfo.pTo = pszTo;
//	shfo.fFlags = FOF_NOCONFIRMATION;
//	shfo.lpszProgressTitle = _T("File Rename..");
//
//	return SHFileOperation(&shfo); 
//}

BOOL CFileOperation::DoCopy(CString destPath, BOOL bOverWrite, int nAttr, CString srcPath)
{
	if(!CheckPath(srcPath))
		return FALSE;

	if(!CanIfind(srcPath))
		return FALSE;

	if(destPath.IsEmpty())
	{
		m_strErrorMsg.Format(_T("error : path is empty."));
		return FALSE;
	}

	CString sUpFolderPath=_T("");
	CString sToFolderName=_T("");

	if(m_nOS != osUnknown)
	{
		int nDelPos=0;
		TCHAR ch = _T('\\');

		if(destPath.Right(1) != ch)
		{
			nDelPos = destPath.ReverseFind(ch);
			sUpFolderPath = destPath.Left(nDelPos+1);
			sToFolderName = destPath.Right(destPath.GetLength()-nDelPos-1);
		}
		else
		{
			sUpFolderPath = destPath.Left(destPath.GetLength()-1);
			nDelPos = sUpFolderPath.ReverseFind(ch);
			sUpFolderPath = sUpFolderPath.Left(nDelPos+1);
			sToFolderName = destPath.Right(destPath.GetLength()-nDelPos-1);
			sToFolderName = sToFolderName.Left(sToFolderName.GetLength()-1);
		}

		if(!DoCreate(sUpFolderPath, nAttr, TRUE))
		{
			m_strErrorMsg.Format(_T("error : to create folder."));
			return FALSE;
		}
	}

	//CString strPath=_T("");
	//int nPos[3] = {-1};
	//CFileFind ff;
	//BOOL bFind;

	//bFind = ff.FindFile(srcPath);

	//nPos[0] = srcPath.ReverseFind(_T('\\'));
	//nPos[1] = srcPath.Find(_T("*.*"), nPos[0]);
	////nPos[2] = sPath.ReverseFind(_T('*'));
	//if(nPos[0] < nPos[1]) // find "*.*"
	//	strPath = srcPath.Left(nPos[0]);
	//else // not find "*.*"
	//	strPath = srcPath;


	BOOL bRtn = FALSE;

	CString sTemp=_T(""), strPath=_T("");
	int nPos[3] = {-1};
	
	nPos[0] = srcPath.ReverseFind(_T('\\'));
	nPos[1] = srcPath.Find(_T("*.*"), nPos[0]);
	sTemp = srcPath.Right(srcPath.GetLength()-nPos[0]-1);
	sTemp.Trim();

	if(nPos[0] < nPos[1] && sTemp == _T("*.*")) // find "*.*"
		strPath = srcPath.Left(nPos[0]);
	else // not find "*.*"
	{
		nPos[2] = sTemp.ReverseFind(_T('*'));
		if(nPos[2] < 0)
			strPath = srcPath;
		else
		{
			bRtn = ApiCopyFile(srcPath, destPath, bOverWrite, nAttr);
			return bRtn;
		}
	}

//	bRtn = ApiCopyFile(strPath, destPath, bOverWrite, nAttr);
	
	if(m_nOS == osUnknown)
		bRtn = ApiCopyFile(strPath, destPath, bOverWrite, nAttr);
	else if(m_nOS <= osWinXp)
		bRtn = ShCopyFile(strPath, destPath, bOverWrite, nAttr);
	else
	{
		bRtn = ICopyFile(strPath, sUpFolderPath, sToFolderName, bOverWrite, nAttr);
		//// 20160720 Add
		if(!bRtn)
		{
			bRtn = ApiCopyFile(strPath, destPath, bOverWrite, nAttr);
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	CFileFind finder;

	if(bRtn)
	{
		bRtn = finder.FindFile(destPath);
	}
	finder.Close();

	return bRtn;
}

BOOL CFileOperation::DoMove(CString destPath, BOOL bOverWrite, int nAttr, CString srcPath)
{
	if(!CheckPath(srcPath))
		return FALSE;

	if(!CanIfind(srcPath))
		return FALSE;

	if(destPath.IsEmpty())
		return FALSE;

	CString sUpFolderPath=_T("");
	CString sToFolderName=_T("");

	if(m_nOS != osUnknown)
	{
		int nDelPos=0;
		TCHAR ch = _T('\\');

		if(destPath.Right(1) != ch)
		{
			nDelPos = destPath.ReverseFind(ch);
			sUpFolderPath = destPath.Left(nDelPos+1);
			sToFolderName = destPath.Right(destPath.GetLength()-nDelPos-1);
		}
		else
		{
			sUpFolderPath = destPath.Left(destPath.GetLength()-1);
			nDelPos = sUpFolderPath.ReverseFind(ch);
			sUpFolderPath = sUpFolderPath.Left(nDelPos+1);
			sToFolderName = destPath.Right(destPath.GetLength()-nDelPos-1);
			sToFolderName = sToFolderName.Left(sToFolderName.GetLength()-1);
		}

		if(!DoCreate(sUpFolderPath, nAttr, TRUE))
			return FALSE;
	}

	BOOL bRtn = FALSE;

	if(m_nOS == osUnknown)
		bRtn = ApiCopyFile(srcPath, destPath, bOverWrite, nAttr);
	else if(m_nOS <= osWinXp)
		bRtn = ShMoveFile(srcPath, destPath, bOverWrite, nAttr);
	else
		bRtn = IMoveFile(srcPath, sUpFolderPath, sToFolderName, bOverWrite, nAttr);

	if(bRtn)
	{
		CFileFind finder;
		if(finder.FindFile(srcPath))
		{
			finder.Close();
			bRtn = DoDelete(srcPath);
			if(!bRtn)
			{
				m_strErrorMsg.Format(_T("error : Failed to delete.\r\n%s"), srcPath);
	//			AfxMessageBox(m_strErrorMsg);
			}
		}
	}

	return bRtn;
}

BOOL CFileOperation::ApiCopyFile(CString srcPath, CString destPath, BOOL bOverWrite, int nAttr)
{
	TCHAR ch;

	CString sFrom, sTo, sFileName, sMsg;
	//if(IsFolder(srcPath))
	if(IsFolderAttr(srcPath, nAttr))
	{
		/*
		ch = _T('.');
		nPos = destPath.ReverseFind(ch);	
		if(nPos < 0)	//if(IsFolder(sPathTo))
		{
		*/
			if(destPath.Right(1) != _T('\\'))
				sFrom = srcPath + _T("\\*.*");
			else
				sFrom = srcPath + _T("*.*");

			sTo = destPath;
		/*
		}
		else
		{
			m_strErrorMsg.Format(_T("error : Attribute of source is folder, but destination isn't.\r\n%s"), srcPath);
//			AfxMessageBox(m_strErrorMsg);
			//AfxMessageBox(_T("error : Attribute of source is folder, but destination isn't."));
			return FALSE;
		}
		*/
	}
	else
	{
		sFrom = srcPath;

		if(IsFolderAttr(destPath, nAttr))
		{
			ch = _T('\\');
			int nDelPos = 0;			

			if(destPath.Right(1) != ch)
			{
				nDelPos = srcPath.ReverseFind(ch);	
				sFileName = srcPath.Right(srcPath.GetLength()-nDelPos);
				sTo = destPath + sFileName;
			}
			else
			{
				sFileName = srcPath.Left(srcPath.GetLength()-1);
				nDelPos = sFileName.ReverseFind(ch);
				sFileName = srcPath.Right(srcPath.GetLength()-nDelPos-1);
				sTo = destPath + sFileName;
			}
		}
		else
		{
			sTo = destPath;
			if(CanIfind(sTo))
			{
				if(IsOpenedFile(sTo))
				{
					//AfxMessageBox(_T("error : Destination file is opened."));
					m_strErrorMsg.Format(_T("error : Destination file is opened.\r\n%s"), sTo);
//					AfxMessageBox(m_strErrorMsg);
					return FALSE;
				}
			}
		}
	}

	int pPos[3] = {-1};
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sFrom);

	BOOL bExist = bWorking;

	if(!DoCreate(destPath, nAttr, TRUE))
	{
		//AfxMessageBox(_T("Save Directory making Failure!"));
		m_strErrorMsg.Format(_T("Save Directory making Failure.\r\n%s"), destPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	CString sFrom2, sTo2, sFileTemp, sTemp;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we"d		
		if(finder.IsDots()) continue; // recur infinitely!
		
		sFrom2 = finder.GetFilePath();
		
		if(finder.IsDirectory())
		{
			sFileTemp = finder.GetFileName();
			sTo2 = sTo + _T("\\") + sFileTemp;
			if(!DoCopy(sTo2, bOverWrite, nAttr, sFrom2))
			{
				//AfxMessageBox(_T("error : File copy is failed."));
				m_strErrorMsg.Format(_T("error : File copy is failed.\r\nFrom: %s\r\nTo: %s"), sFrom2, sTo2);
//				AfxMessageBox(m_strErrorMsg);
				return FALSE;
			}
		}
		else
		{
			if(!CanIread(sFrom2))
			{
				//AfxMessageBox(_T("error : Can't read file."));
				m_strErrorMsg.Format(_T("error : Can't read file.\r\n%s"), sFrom2);
//				AfxMessageBox(m_strErrorMsg);
				return FALSE;
			}
	
			pPos[0] = sTo.ReverseFind(_T('\\'));
			pPos[1] = sTo.Find(_T('*'), pPos[0]);
			sTemp = sTo.Right(sTo.GetLength()-pPos[0]-1);
			sTemp.Trim();
			pPos[2] = sTemp.ReverseFind(_T('*'));

			if(pPos[2] >= 0 && pPos[0] < pPos[1])
			{
				sTemp = sTo.Left(pPos[0]);

				if(IsFolderAttr(sTemp))
				{
					sFileTemp = finder.GetFileName();
					sTo2 = sTemp + _T("\\") + sFileTemp;
				}
				else
					sTo2 = sTo;

				if(!CopyFile(sFrom2, sTo2, !bOverWrite)) // bFailIfExists = !bOverWrite
				{
					if(IsOpenedFile(sFrom2))
					{
						//AfxMessageBox(_T("error : File copy is failed."));
						m_strErrorMsg.Format(_T("error : Source file is opened.\r\n%s"), sFrom2);
//						AfxMessageBox(m_strErrorMsg);
					}
					else 
					{
						//AfxMessageBox(_T("error : File copy is failed."));
						m_strErrorMsg.Format(_T("error : File copy is failed.\r\n%s"), sFrom2);
//						AfxMessageBox(m_strErrorMsg);
					}
					return FALSE;
				}
			}
			else
			{
				if(IsFolderAttr(sTo, nAttr))
				{
					sFileTemp = finder.GetFileName();
					sTo2 = sTo + _T("\\") + sFileTemp;
				}
				else
					sTo2 = sTo;

				if(!Copy(sFrom2, sTo2, bOverWrite))
				{
					if(IsOpenedFile(sFrom2))
					{
						//AfxMessageBox(_T("error : File copy is failed."));
						m_strErrorMsg.Format(_T("error : Source file is opened.\r\n%s"), sFrom2);
//						AfxMessageBox(m_strErrorMsg);
					}
					else 
					{
						//AfxMessageBox(_T("error : File copy is failed."));
						m_strErrorMsg.Format(_T("error : File copy is failed.\r\n%s"), sFrom2);
//						AfxMessageBox(m_strErrorMsg);
					}
					return FALSE;
				}
			}
		}
	}
	finder.Close();
	return bExist;
}


BOOL CFileOperation::IsOpenedFile(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	CString sMsg;
	if(IsFolderAttr(sPath))
	{
		m_strErrorMsg.Format(_T("error : This is folder.\r\n%s"), sPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	HANDLE hFile = INVALID_HANDLE_VALUE; 

	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwShareMode = 0;
	hFile = CreateFile(sPath, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, 0, NULL);  
	if(hFile == INVALID_HANDLE_VALUE)  
		return TRUE;  

	CloseHandle(hFile);

	return FALSE;  
}


// File Attribute
BOOL CFileOperation::SetAttribute(tpAttribute nAttribue, CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	DWORD attrib;
	switch(nAttribue)
	{
	case fileHidden:	attrib = FILE_ATTRIBUTE_HIDDEN;		break;
	case fileReadWrite:	attrib = FILE_ATTRIBUTE_NORMAL;		break;
	case fileReadOnly:	attrib = FILE_ATTRIBUTE_READONLY;	break;
	default:
		return FALSE;
	}

	DWORD attr = GetFileAttributes(sPath);
	if ((attr & attrib) == 0) 
	{
		if(SetFileAttributes(sPath, attr | attrib))
			return TRUE;

		m_strErrorMsg=_T("error : SetAttribute()");
		switch(GetLastError())
		{
		case FILE_ATTRIBUTE_COMPRESSED:
			m_strErrorMsg = _T("error : File is compressed.");
			break;
		case FILE_ATTRIBUTE_DEVICE:
			m_strErrorMsg = _T("error : File is device. (do not use)");
			break;
		case FILE_ATTRIBUTE_DIRECTORY:
			m_strErrorMsg = _T("error : This is folder.");
			break;
		case FILE_ATTRIBUTE_ENCRYPTED:
			m_strErrorMsg = _T("error : File is encrypted.");
			break;
		case FILE_ATTRIBUTE_REPARSE_POINT:
			m_strErrorMsg = _T("error : File is reparsing point.");
			break;
		case FILE_ATTRIBUTE_SPARSE_FILE:
			m_strErrorMsg = _T("error : File is sparse.");
			break;
		}

//		AfxMessageBox(m_strErrorMsg);
	}

	return FALSE;
}

tpAttribute CFileOperation::GetAttribute(CString sPath)
{
	//if(!CheckPath(sPath))
	//	return Unknown;

	//if(!CanIfind(sPath))
	//	return Unknown;

	DWORD attrib;
	attrib = GetFileAttributes(sPath);

	if(attrib & FILE_ATTRIBUTE_DIRECTORY)		return Folder;
	if(attrib & FILE_ATTRIBUTE_HIDDEN)			return fileHidden;
	if(attrib & FILE_ATTRIBUTE_NORMAL)			return fileReadWrite;
	if(attrib & FILE_ATTRIBUTE_READONLY)		return fileReadOnly;

	return fileUnknown;

	// 이외의 다른 속성은 추후 추가...
	//	FILE_ATTRIBUTE_ARCHIVE
	//	FILE_ATTRIBUTE_COMPRESSED		// 압축파일
	//	FILE_ATTRIBUTE_DIRECTORY		// 디렉토리(폴더)
	//	FILE_ATTRIBUTE_ENCRYPTED
	//	FILE_ATTRIBUTE_HIDDEN			// 숨김 화일
	//	FILE_ATTRIBUTE_NORMAL  
	//	FILE_ATTRIBUTE_OFFLINE 
	//	FILE_ATTRIBUTE_READONLY			// 읽기 전용
	//	FILE_ATTRIBUTE_REPARSE_POINT 
	//	FILE_ATTRIBUTE_SPARSE_FILE
	//	FILE_ATTRIBUTE_SYSTEM			// 시스템 파일
	//	FILE_ATTRIBUTE_TEMPORARY		// 임시 파일
}

BOOL CFileOperation::IsFolderAttr(CString sPath, int nAttr)
{
	if(!CheckPath(sPath))
		return FALSE;

	//CFileFind ff;
	//if(!ff.FindFile(sPath))
	//	return FALSE;

	if(nAttr == Folder)
		return TRUE;
	else if(nAttr == File)
		return FALSE;
	else if(nAttr == fileUnknown)
	{
		if(GetAttribute(sPath) & Folder)
			return TRUE;
		//{
		//	CString sTemp = _T("");
		//	int pPos[3] = {0};
		//	pPos[0] = sPath.ReverseFind(_T('\\'));
		//	pPos[1] = sPath.Find(_T('*'), pPos[0]);
		//	sTemp = sPath.Right(sPath.GetLength()-pPos[0]-1);
		//	sTemp.Trim();
		//	pPos[2] = sTemp.ReverseFind(_T('*'));

		//	if(pPos[2] >= 0 && pPos[0] < pPos[1])
		//		return FALSE;
		//}
		//return TRUE;
	}

	return FALSE;
}

BOOL CFileOperation::IsFolder(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	CFileFind ff;
	BOOL bFind = ff.FindFile(sPath);
	if(bFind)
	{
		bFind = ff.FindNextFile();
		if(ff.IsDirectory())
			return TRUE;
	}
	ff.Close();

	return FALSE;
}

BOOL CFileOperation::IsFileHidden(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if(GetAttribute(sPath) & fileHidden)
		return TRUE;

	return FALSE;
}

BOOL CFileOperation::IsFileReadOnly(CString sPath)	
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if(GetAttribute(sPath) & fileReadOnly)
		return TRUE;

	return FALSE;
}

BOOL CFileOperation::IsFileReadWrite(CString sPath)	
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if(GetAttribute(sPath) & fileReadWrite)
		return TRUE;

	return FALSE;
}

BOOL CFileOperation::SetFileHidden(CString sPath)
{
	return SetAttribute(fileHidden, sPath);
}

BOOL CFileOperation::SetFileReadOnly(CString sPath)
{
	return SetAttribute(fileReadOnly, sPath);
}

BOOL CFileOperation::SetFileReadWrite(CString sPath)
{
	return SetAttribute(fileReadWrite, sPath);
}


// File Access
// 파일 검사 {존재(0), 쓰기(2), 읽기(4), 읽기와쓰기(6)}

BOOL CFileOperation::CanIfind(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	CString strPath=_T("");
	int nPos[3] = {-1};
	CFileFind ff;
	BOOL bFind;

	bFind = ff.FindFile(sPath);

	nPos[0] = sPath.ReverseFind(_T('\\'));
	nPos[1] = sPath.Find(_T('*'), nPos[0]);
	//nPos[2] = sPath.ReverseFind(_T('*'));
	if(nPos[0] < nPos[1]) // find '*'
	{
		while(bFind) // until we find an entry
		{
			bFind = ff.FindNextFile();
			if(!ff.IsDots())
			{
				strPath=ff.GetFilePath();
				break;
			}
		}

		if(strPath.IsEmpty())
		{
			m_strErrorMsg.Format(_T("error : failed to find path."));
			return FALSE;
		}
	}
	else // not find '*'
		strPath = sPath;
	ff.Close();

	if (0 == Access(strPath, accessExist))
		return TRUE;

	m_strErrorMsg.Format(_T("error : failed to access path."));
	return FALSE;
}

BOOL CFileOperation::CanIread(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if (0 == Access(sPath, accessRead))
		return TRUE;

	return FALSE;
}

BOOL CFileOperation::CanIwrite(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if (0 == Access(sPath, accessWrite))
		return TRUE;

	return FALSE;
}

BOOL CFileOperation::CanIreadwrite(CString sPath)
{
	if(!CheckPath(sPath))
		return FALSE;

	if(!CanIfind(sPath))
		return FALSE;

	if (0 == Access(sPath, accessReadWrite))
		return TRUE;

	return FALSE;
}


BOOL CFileOperation::DoDeleteFile(CString strPath)							// 파일 삭제
{
	m_strErrorMsg.Empty();
	if(IsFolderAttr(strPath, File))
	{
		m_strErrorMsg.Format(_T("error : This is folder.\r\n%s"), strPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}
		
	return DoDelete(strPath);
}

BOOL CFileOperation::DoCopyFile(CString srcPath, CString destPath, BOOL bOverWrite)		// 파일 복사	
{
	m_strErrorMsg.Empty();
	if(IsFolderAttr(srcPath, File))
	{
		m_strErrorMsg.Format(_T("error : This is folder.\r\n%s"), srcPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	return DoCopy(destPath, bOverWrite, File, srcPath);
}

BOOL CFileOperation::DoMoveFile(CString srcPath, CString destPath, BOOL bOverWrite)		// 파일 이동	
{
	m_strErrorMsg.Empty();
	if(IsFolderAttr(srcPath, File))
	{
		m_strErrorMsg.Format(_T("error : This is folder.\r\n%s"), srcPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	return DoMove(destPath, bOverWrite, File, srcPath);
}

BOOL CFileOperation::DoDeleteFolder(CString strPath)	// 폴더 그리고 폴더 이하 전부 삭제
{
	m_strErrorMsg.Empty();
	if(!IsFolderAttr(strPath, Folder))
	{
		m_strErrorMsg.Format(_T("error : This is file.\r\n%s"), strPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	return DoDelete(strPath);
}

BOOL CFileOperation::DoCopyFolder(CString srcPath, CString destPath, BOOL bOverWrite)		// 폴더 그리고 폴더 이하 전부 복사
{
	m_strErrorMsg.Empty();
	if(!IsFolderAttr(srcPath, Folder))
	{
		m_strErrorMsg.Format(_T("error : This is file.\r\n%s"), srcPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	return DoCopy(destPath, bOverWrite, Folder, srcPath);
}

BOOL CFileOperation::DoMoveFolder(CString srcPath, CString destPath, BOOL bOverWrite)		// 폴더 그리고 폴더 이하 전부 이동	
{
	m_strErrorMsg.Empty();
	if(!IsFolderAttr(srcPath, Folder))
	{
		m_strErrorMsg.Format(_T("error : This is file.\r\n%s"), srcPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	if(!IsFolderAttr(destPath, Folder))
	{
		m_strErrorMsg.Format(_T("error : This is file.\r\n%s"), destPath);
//		AfxMessageBox(m_strErrorMsg);
		return FALSE;
	}

	return DoMove(destPath, bOverWrite, Folder, srcPath);
}

CString CFileOperation::GetErrorMsg()
{
	return m_strErrorMsg;
}
BOOL CFileOperation::GetFileDateInfo(CString strFilePath, CString &strCreateTime, CString &strAccessTime, CString &strWriteTime)
{
	CFileFind finder;

	if (!finder.FindFile(strFilePath))
		return FALSE;

	HANDLE hFile = INVALID_HANDLE_VALUE;

	DWORD dwDesiredAccess = GENERIC_READ;
	DWORD dwShareMode = FILE_SHARE_READ;
	hFile = CreateFile(strFilePath, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME create_time, access_time, write_time;
		SYSTEMTIME c_system_time, a_system_time, w_system_time;

		GetFileTime(hFile, &create_time, &access_time, &write_time);
		FileTimeToSystemTime(&create_time, &c_system_time);
		FileTimeToSystemTime(&access_time, &a_system_time);
		FileTimeToSystemTime(&write_time, &w_system_time);

		strCreateTime.Format(_T("%d.%02d.%02d"), c_system_time.wYear, c_system_time.wMonth, c_system_time.wDay);
		strAccessTime.Format(_T("%d.%02d.%02d"), a_system_time.wYear, a_system_time.wMonth, a_system_time.wDay);
		strWriteTime.Format(_T("%d.%02d.%02d"), w_system_time.wYear, w_system_time.wMonth, w_system_time.wDay);
		
		CloseHandle(hFile);
		return TRUE;
	}

	return FALSE;
}