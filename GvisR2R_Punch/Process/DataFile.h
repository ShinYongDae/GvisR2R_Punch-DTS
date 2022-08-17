// DataFile.h: interface for the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILE_H__CAEFF315_A227_4574_8B2E_C52CD9C09100__INCLUDED_)
#define AFX_DATAFILE_H__CAEFF315_A227_4574_8B2E_C52CD9C09100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataFile  
{

// Construction
public:
	CDataFile();
	CDataFile(LPCTSTR lpszFileName);

// Attributes
public:
	BOOL    m_bOpened;	// File open flag
private:
	BOOL	m_bModified;	// Contents Modified Flag
	CString m_FilePath; // 파일의 Fullpath name
	CString m_str;		// 파일내용의 복사본
	int     m_nTotalLines;
	CUIntArray m_arLine;
	UINT GetFileStructure();
	LONG Find(TCHAR ch);
	LONG Find(LPCTSTR lpszSub);
	LONG Find(TCHAR ch, int nStart);
	LONG Find(LPCTSTR pstr, int nStart);
	LONG Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	LONG Delete(int nIndex=0,int nCount=1);
	LONG Insert(int nIndex,LPCTSTR str);
	LONG Append(LPCTSTR str);
	LONG GetSize();

// Implementation
public:
	CString ExtractFirstWord(CString *str, CString sep, BOOL bTrim=FALSE);
	BOOL ReadData(CString strDataGroup, CString strDescription, int nTh,double &fData);
	BOOL MoveLine(int nSrcLine,int nDestLine);
	LONG ReplaceLine(UINT nLine,LPCTSTR lpszNew);
	LONG ReplaceLineString(UINT nLine,LPCTSTR lpszOld,LPCTSTR lpszNew);
	LONG DeleteLine(UINT nLine);
	LONG InsertLine(UINT nLine,LPCTSTR strInsert);
	LONG AppendLine(LPCTSTR str);
	BOOL GetLineString(UINT nLine,CString& rtnStr);
	CString GetLineString(int nStart,int nEnd);
	CString GetLineString(UINT nLine);
	CString GetString(int nStartIndex, int nEndIndex);
	CString GetAllString();
	int	GetLineStartPosition(UINT nLine);
	int	GetLineEndPosition(int nLine);
	int	GetLineNumber(LPCTSTR str,int nStart=1);
	int GetLineNumOfDescriptor(LPCTSTR Descriptor, int nStart=1);
	UINT GetTotalLines();
	BOOL Close();
	BOOL Open(CString sFileName);	// BOOL Open(LPCTSTR lpszFileName);
	virtual ~CDataFile();
};
#endif // !defined(AFX_DATAFILE_H__CAEFF315_A227_4574_8B2E_C52CD9C09100__INCLUDED_)
