#if !defined(AFX_MYFILE_H__AFD6891D_ED89_43AE_9AF1_EEB2EDF7D75F__INCLUDED_)
#define AFX_MYFILE_H__AFD6891D_ED89_43AE_9AF1_EEB2EDF7D75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFile.h : header file
//

#pragma warning(disable: 4996)

/////////////////////////////////////////////////////////////////////////////
// CMyFile window

class CMyFile : public CWnd
{
	CWnd* m_pParent;
	CString m_sError;
	DWORD m_dwError;
	int m_iRecursionLimit;	
	bool m_bAskIfReadOnly;
	bool m_bOverwriteMode;
	bool m_bAborted;

	void ShowError(); //Show error message

// Construction
public:
	CMyFile(CWnd* pParent=NULL);

// Attributes
public:

// Operations
public:
	BOOL Copy(CString sSource, CString sDest);
	BOOL CheckSelfCopy(CString sSource, CString sDest);
	int CheckPath(CString sPath);
	void PreparePath(CString &sPath);
	CString ParseFolderName(CString sPathName, CString &sRemainPathName);
	void CheckSelfRecursion(CString sSource, CString sDest);
	void DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy = FALSE);
	void DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy = FALSE);
	bool IsFileExist(CString sPathName);
	void DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy);
	bool CanDelete(CString sPathName);
	BOOL DeleteFolerOrFile(CString strFolerOrFilePath);
	void DoDelete(CString sPathName);
	CString ChangeFileName(CString sFileName);

	// for R2R
	void DelPcrAll(CString strPath);
	void DelSharePcr();
	int CopyPcrAll(CString sSrc, CString sDest);  // return : Serial
	BOOL ChkLotEnd(CString sPath);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFile)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFile();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyFile)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILE_H__AFD6891D_ED89_43AE_9AF1_EEB2EDF7D75F__INCLUDED_)
