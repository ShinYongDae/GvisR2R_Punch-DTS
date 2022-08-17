#pragma once

#include "HardwareInfoEx.h"

/************************************************************************************************************
CFileOperation is designed by YongDaeShin on May, 6, 2016.
[개요]
윈도우즈(OS)는 파일(or폴더)을 두가지 측면에서 고려하고 있다.
하나는 파일(or폴더) 그 자체가 가지고 있는 속성(attribute)이고, 
다른 하나는 윈도우즈(OS)에서 파일에 대해 사용자가 가질 수 있는 권한(permission or right)이다.
여기서 사용자는 Local Machine에서의 계정 사용자뿐만 아니라, 네트워크 상의 계정 사용자를 포함한다.
계정에 의한 파일 권한 조작은 추후 추가할 예정이다.
*************************************************************************************************************/

//CFile Attribute {normal=0x00, readOnly=0x01, hidden=0x02, system=0x04, volume=0x08, directory=0x10, archive=0x20}
typedef enum {fileUnknown=0x00, Folder=0x01, fileHidden=0x02, fileReadWrite=0x04, fileReadOnly=0x08, File=0x10}tpAttribute;
typedef enum {rightAll=0x00, rRightExec=0x01, rightChange=0x02, rightWrite=0x04, rightRead=0x08}tpPermission;
typedef enum {accessExist=0x00, accessWrite=0x02, accessRead=0x04, accessReadWrite=0x06}tpAccess;
typedef enum {osUnknown=0x00, osWin2k=0x01, osWinXp=0x02, osWin7=0x04, osWin10=0x05}tpOs;
#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2
// DWORD dwOperationFlags : FileOperation Flag......
/*
FOF_SILENT					(0x0004)
FOF_RENAMEONCOLLISION		(0x0008)
FOF_NOCONFIRMATION			(0x0010)
FOF_ALLOWUNDO				(0x0040)
FOF_FILESONLY				(0x0080)
FOF_NOCONFIRMMKDIR			(0x0200)
FOF_NOERRORUI				(0x0400)
FOF_NOCOPYSECURITYATTRIBS	(0x0800)
FOF_NORECURSION				(0x1000)
FOF_NO_CONNECTED_ELEMENTS	(0x2000)
FOF_WANTNUKEWARNING			(0x4000)		over ride	FOF_NOCONFIRMATION			(0x0010)
FOFX_NOSKIPJUNCTIONS		(0x10000)
FOFX_PREFERHARDLINK			(0x20000)
FOFX_SHOWELEVATIONPROMPT	(0x40000)
FOFX_RECYCLEONDELETE		(0x80000)		Introduced in Windows 8.
FOFX_EARLYFAILURE			(0x100000)		This flag is valid only when FOF_NOERRORUI is set.
FOFX_PRESERVEFILEEXTENSIONS (0x200000)		This flag is valid only when FOF_RENAMEONCOLLISION is also set.
FOFX_KEEPNEWERFILE			(0x400000)
FOFX_NOCOPYHOOKS			(0x800000)
FOFX_NOMINIMIZEBOX			(0x1000000)
FOFX_MOVEACLSACROSSVOLUMES	(0x2000000)
FOFX_DONTDISPLAYSOURCEPATH	(0x4000000)
FOFX_DONTDISPLAYDESTPATH	(0x8000000)
FOFX_REQUIREELEVATION		(0x10000000)	Introduced in Windows Vista SP1. 
FOFX_ADDUNDORECORD			(0x20000000)	Introduced in Windows 8.
FOFX_COPYASDOWNLOAD			(0x40000000)	Introduced in Windows 7. 
FOFX_DONTDISPLAYLOCATIONS	(0x80000000)	Introduced in Windows 7.
*/
//160530 lgh mod
class CWindowVersion
{
	public:
		tpOs m_nOSVer;
		BOOL m_bCoInitialize;
		CHardwareInfoEx m_HardwareInfo;													// OS version을 확인하여 SHFileOperation / IFileOperation을 구분하여 사용.
	BOOL GetOsVersion();
	CWindowVersion();
	~CWindowVersion();

	CString m_strErrorMsg;

	CString GetError();

	 tpOs  GetOsInfo();
};


class CFileOperation
{
public:
	//CHardwareInfoEx m_HardwareInfo;													// OS version을 확인하여 SHFileOperation / IFileOperation을 구분하여 사용.
	CString m_sPath;																// Default file path. (is or not)
	tpOs m_nOS;
	//BOOL m_bCoInitialize;

	//BOOL GetOsVersion();

	// File Handling
	BOOL CheckPath(CString &sPath);													// Confirm file path for handling.
	BOOL DoCreate(CString sPath=_T(""), int nAttr=fileUnknown, BOOL bFolderOnly=TRUE);						// 폴더 or 파일을 생성						

	BOOL Copy(CString srcPath, CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown);
	BOOL DoCopy(CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown, CString srcPaht=_T(""));		// 파일 및 폴더 그리고 폴더 이하 전부 복사					
	BOOL ApiCopyFile( CString srcPath, CString destPath,BOOL bOverWrite=TRUE, int nAttr=fileUnknown);
	BOOL ShCopyFile(CString srcPath, CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown);
	BOOL ShMoveFile(CString srcPath, CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown);
	BOOL ICopyFile(CString srcPath, CString destUpFolder, CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown);
	BOOL IMoveFile(CString srcPath, CString destUpFolder, CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown);

	BOOL Delete(CString sPath);
	BOOL DoDelete(CString sPath=_T(""));					// 파일 및 폴더 그리고 폴더 이하 전부 삭제
	BOOL ApiDelete(CString sPath);
	BOOL ShDelete(CString sPath);
	BOOL IDelete(CString sPath);

	BOOL DoMove(CString destPath, BOOL bOverWrite=TRUE, int nAttr=fileUnknown, CString sPathFrom=_T(""));		// 파일 및 폴더 그리고 폴더 이하 전부 이동					

	BOOL IsOpenedFile(CString IsOpenedFile=_T(""));									// 파일이 Open된 상태인지 확인
	//BOOL ShRename(CString sPathTo, CString sPathFrom=_T(""));	
	//BOOL DoRename(CString sPathTo, CString sPathFrom=_T(""));	

	// File Attribute
	BOOL SetAttribute(tpAttribute nAttribue=fileUnknown, CString sPath=_T(""));
	tpAttribute GetAttribute(CString sPath=_T(""));	

	// Error Message
	CString m_strErrorMsg;

public:
	CFileOperation(tpOs nOSVer = osWin7,CString sPath=_T(""));											// Default file path. (is or not)
	~CFileOperation();
	int CheckPathisFile(CString strPath);
	void DeleteFileInFolder(CString strFolderPath); //폴더는남겨준채 파일만 지운다.
	BOOL CopyFileRepeat(CString strSrc,CString strDst,int nIterMax= 3, BOOL bOverWrite=TRUE);

	// File Handling
	BOOL DoCopyFile(CString srcPath, CString destPath, BOOL bOverWrite=TRUE);		// 파일 복사					
	BOOL DoDeleteFile(CString strPath);												// 파일 삭제
	BOOL DoMoveFile(CString srcPath, CString destPath, BOOL bOverWrite=TRUE);		// 파일 이동

	BOOL DoCreateFolder(CString srcPath, BOOL bNew=FALSE);							// 폴더 그리고 폴더 이하 전부 복사					
	BOOL DoCopyFolder(CString srcPath, CString destPath, BOOL bOverWrite=TRUE);		// 폴더 그리고 폴더 이하 전부 복사					
	BOOL DoDeleteFolder(CString strPath);					// 폴더 그리고 폴더 이하 전부 삭제
	BOOL DoMoveFolder(CString srcPath, CString destPath, BOOL bOverWrite=TRUE);		// 폴더 그리고 폴더 이하 전부 이동				
	// File Date Info
	BOOL GetFileDateInfo(CString strFilePath, CString &strCreateTime, CString &strAccessTime, CString &strWriteTime);
	// Error Message
	CString GetErrorMsg();
	BOOL IsFileReadWrite(CString sPath=_T(""));								// ﾆﾄﾀﾏ ｼﾓｼｺ ﾈｮﾀﾎ
	// File Attribute
	BOOL IsFolderAttr(CString sPath=_T(""), int nAttr=fileUnknown);										// 파일 속성 확인(폴더)
	BOOL IsFolder(CString sPath=_T(""));											// 폴더 존재 확인
	BOOL IsFileHidden(CString sPath=_T(""));										// 파일 속성 확인
	BOOL IsFileReadOnly(CString sPath=_T(""));										// 파일 속성 확인
	//BOOL IsFileReadWrite(CString sPath=_T(""));										// 파일 속성 확인
	BOOL SetFileHidden(CString sPath=_T(""));										// 파일 속성 변경
	BOOL SetFileReadOnly(CString sPath=_T(""));										// 파일 속성 변경
	BOOL SetFileReadWrite(CString sPath=_T(""));									// 파일 속성 변경

	// File Access Permission 
	// { STANDARD_RIGHTS_ALL(모든 권한), STANDARD_RIGHTS_EXECUTE(실행), STANDARD_RIGHTS_REQUIRED(수정), STANDARD_RIGHTS_READ(읽기), STANDARD_RIGHTS_WRITE(쓰기), SPECIFIC_RIGHTS_ALL(특정 권한) }
	// 계정에 대한 권한여부 대신에 access함수를 이용하여 실제 파일접근성을 검사한다.
	BOOL CanIfind(CString sPath=_T(""));											// 접근 권한 확인						
	BOOL CanIread(CString sPath=_T(""));											// 접근 권한 확인												
	BOOL CanIwrite(CString sPath=_T(""));											// 접근 권한 확인												
	BOOL CanIreadwrite(CString sPath=_T(""));										// 접근 권한 확인												
};

