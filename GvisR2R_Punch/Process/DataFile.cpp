// DataFile.cpp: implementation of the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDataFile::CDataFile()
{
	m_str = _T("");
	m_bModified = FALSE;
	m_bOpened = FALSE;
	m_FilePath = _T("");
	m_nTotalLines = -1;
	m_arLine.RemoveAll();
}
CDataFile::CDataFile(LPCTSTR lpszFileName)
{
	m_str = _T("");
	m_bModified = FALSE;
	m_bOpened = FALSE;
	m_FilePath = _T("");
	m_nTotalLines = -1;
	m_arLine.RemoveAll();
	if(!Open(lpszFileName))
	{
		CFile file;
		CFileException pError;
		if(!file.Open(lpszFileName,CFile::modeCreate|CFile::modeReadWrite,&pError))
		{
			// 파일 오픈에 실패시 
			#ifdef _DEBUG
			   afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
			#endif
		}
		else
		{
			Open(lpszFileName);
		}
	}
}
CDataFile::~CDataFile()
{
//	if(m_bModified)
	//	Close();

}


BOOL CDataFile::Open(CString sFileName)	// BOOL CDataFile::Open(LPCTSTR lpszFileName)
{
	//if(!m_FilePath.Compare(lpszFileName)) return TRUE; // 이미 열려 있는 경우 TRUE를 리턴한다.
	if(!m_FilePath.Compare(sFileName)) return TRUE; // 이미 열려 있는 경우 TRUE를 리턴한다.

	TCHAR FileNCam[200];
	wsprintf(FileNCam, TEXT("%s"), sFileName);

	CFile file;
	CFileException pError;
	if(!file.Open(FileNCam,CFile::modeRead,&pError)) //		if (!file.Open(lpszFileName, CFile::modeRead, &pError))
	{
		// 파일 오픈에 실패시 
		#ifdef _DEBUG
		   afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
		#endif
		m_bOpened = FALSE;
		return FALSE;
	}
	else
	{
		m_str = _T("");
		file.SeekToBegin();
		DWORD dwFileLength = file.GetLength();

		// 파일 크기 만큼 버퍼 메모리를 확보한다.
		char *cpTemp;
		cpTemp = (char *)malloc(sizeof(char) * dwFileLength+1);

		//file의 내용을 버퍼로 읽어 들인다.
		UINT nBytesRead = file.Read(cpTemp,dwFileLength);
		
		// 버퍼의 끝처리를 한다.
		*(cpTemp+dwFileLength) = NULL;
		// 버퍼의 내용을 CString으로 옮긴다.
		m_str = cpTemp;
		// 버퍼 메모리를 릴리즈 한다.
		free(cpTemp);

		//m_FilePath = lpszFileName;
		m_FilePath = sFileName;

		file.Close();

		m_bModified = FALSE;
		m_bOpened = TRUE;

		GetFileStructure();

	}
	return TRUE;
}

BOOL CDataFile::Close()
{
	if(m_bOpened && m_bModified) // 파일이 수정되었을 경우 수정된 내용를 원본 파일에 반영한다.
	{
		CFile file;
		CFileException pError;
		if(!file.Open(m_FilePath,CFile::modeWrite,&pError))
		{
			// 파일 오픈에 실패시 
			#ifdef _DEBUG
			   afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
			#endif
			return FALSE;
		}
		else
		{	
			//버퍼의 내용을 file에 복사한다.
			file.SeekToBegin();
			file.Write(m_str,m_str.GetLength());
			file.SetLength(m_str.GetLength());
			file.Close();
			m_bModified = FALSE;
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////
// 파일의 크기(Size)를 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
LONG CDataFile::GetSize()
{
	if(!m_bOpened) return 0;
	return (LONG)m_str.GetLength();
}

LONG CDataFile::Find(TCHAR ch)
{
	return (LONG)m_str.Find(ch);
}
LONG CDataFile::Find(LPCTSTR lpszSub)
{
	return (LONG)m_str.Find(lpszSub);
}
LONG CDataFile::Find(TCHAR ch, int nStart)
{
	return (LONG)m_str.Find(ch,nStart);
}
LONG CDataFile::Find(LPCTSTR pstr, int nStart)
{
	return (LONG)m_str.Find(pstr,nStart);
}

//////////////////////////////////////////////////////
// 파일의 끝에 문자열을 붙인다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
LONG CDataFile::Append(LPCTSTR str)
{
	if(!m_bOpened) 
		return 0;
	m_str.Insert(m_str.GetLength(),str);
	m_bModified = TRUE;
	return GetSize();
}

//////////////////////////////////////////////////////
// 파일의 시작을 기준으로 nIndex의 위치에 str의 문자열을 삽입한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
LONG CDataFile::Insert(int nIndex,LPCTSTR str)
{
	if(!m_bOpened) return 0;
	m_str.Insert(nIndex,str);
	m_bModified = TRUE;
	return GetSize();
}
//////////////////////////////////////////////////////
// 파일의 시작을 기준으로 nIndex의 위치로 부터 nCount의 문자를 삭제한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
LONG CDataFile::Delete(int nIndex, int nCount)
{
	if(!m_bOpened) return 0;
	m_str.Delete(nIndex,nCount);
	m_bModified = TRUE;
	return GetSize();
}

////////////////////////////////////////////////////////
// 파일에서 처음부터 lpszOld로 지정된 문자열을 모두 찾아 lpszNew로 바꾼다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
LONG CDataFile::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	if(!m_bOpened) return 0;
	int nCount = m_str.Replace(lpszOld,lpszNew);
	m_bModified = TRUE;
	return GetSize();
}
////////////////////////////////////////////////////////
// 파일의 라인수를 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
UINT CDataFile::GetTotalLines()
{
	if(!m_bOpened) return 0;

	return m_nTotalLines;
}

////////////////////////////////////////////////////////
// 파일의 라인수를 리턴한다.
// 파일이 열려 있지 않으면 0를 리턴한다.
UINT CDataFile::GetFileStructure()
{
	if(!m_bOpened) 
		return 0;

	UINT nLine = 0;
	int nCurPos,nPrevPos = 0;
	CString strTemp;

	m_arLine.RemoveAll();
	m_nTotalLines = m_arLine.GetSize();

	do{
		nCurPos = m_str.Find('\n',nPrevPos);
		if(nCurPos == -1)
		{
			break;
		}
		else
		{
			m_arLine.Add(nPrevPos);
			nLine++;
			nPrevPos = (nCurPos+1);
		}
	}while(1);
		
	m_nTotalLines = nLine;
	m_nTotalLines = m_arLine.GetSize();

	return nLine;
}


////////////////////////////////////////////////////////
// 파일에서 nStart위치로 부터 str을 찾아 str의 라인 위치를 리턴한다.
// 파일이 열려 있지 않거나, 해당 스트링이 없으면 -1을 리턴한다.
int CDataFile::GetLineNumber(LPCTSTR str, int nStart)
{
	if(!m_bOpened) return -1;

	CString strLine;
	UINT nMaxLine = GetTotalLines();
	for(UINT nLine = nStart; nLine <= nMaxLine; nLine++)
	{
		// 라인의 스트링을 읽어 들인다.
		strLine = GetLineString(nLine);
		// str이 존재 하는 지를 조사 한다.
		if(strLine.Find(str,0) != -1)
			return nLine;
	}
	return -1;
}

////////////////////////////////////////////////////////
// 파일에서 nStart위치로 부터 Descriptor를 찾아 라인 위치를 리턴한다.
// 파일이 열려 있지 않거나, 해당 스트링이 없으면 -1을 리턴한다.
int CDataFile::GetLineNumOfDescriptor(LPCTSTR Descriptor, int nStart)
{
	if(!m_bOpened) return -1;

	CString strLine,strValue;
	UINT nMaxLine = GetTotalLines();
	for(UINT nLine = nStart; nLine <= nMaxLine; nLine++)
	{
		// 라인의 스트링을 읽어 들인다.
		strLine = GetLineString(nLine);
		// Descriptor부분을 추출한다.
		CString sep = _T("=");
		strValue = ExtractFirstWord(&strLine, sep, TRUE);
		// 찾고자 하는 Descriptor가 존재 하는 지를 조사 한다.
		if(!strValue.Compare(Descriptor))
			return nLine;
	}
	return -1;
}

////////////////////////////////////////////////////////
// 파일에서 nLine으로 지정된 라인의 시작 위치를 리턴한다.
// 파일이 열려 있지 않으면 -1을 리턴한다.
int CDataFile::GetLineStartPosition(UINT nLine)
{
	if(!m_bOpened) return -1;
	
	int nPos = 0;
	if(nLine >0 && (int)nLine <= m_nTotalLines)
		nPos = m_arLine.GetAt(nLine-1);
	return nPos;
}

////////////////////////////////////////////////////////
// 파일에서 현재 라인의 종료 위치를 리턴한다.
// 파일이 열려 있지 않으면 -1을 리턴한다.
int CDataFile::GetLineEndPosition(int nLine)
{
	if(!m_bOpened) return -1;

	int nPos = 0;
	if(nLine >0 && nLine < m_nTotalLines)
		nPos = m_arLine.GetAt(nLine);
	return nPos;
}


////////////////////////////////////////////////////////
// 파일에서 nStartIndex위치에서 nEndIndex의 위치까지의 문자열을 반환한다.
// 이때 반환되는 문자열은 NULL로 종료되는 문자열이다.
// 파일이 열려 있지 않으면 NULL을 리턴한다.
CString CDataFile::GetString(int nStartIndex, int nEndIndex)
{
	if(!m_bOpened) return CString("");

	CString str;
	str = m_str.Mid(nStartIndex,nEndIndex-nStartIndex);
	return str;
}
////////////////////////////////////////////////////////
// 파일에서 nLine위치의 문자열을 반환한다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 NULL을 리턴한다.
CString CDataFile::GetLineString(UINT nLine)
{
	if(!m_bOpened) return CString("");

	// 파일의 라인수를 읽는다.
	UINT nMaxLine = GetTotalLines();
	
	// 읽고자 하는 라인번호가 적당한지를 조사한다.
	if (nLine > nMaxLine || nLine < 1)
	{
		AfxMessageBox(_T("Line string read error"));
		return CString("");
	}

	int nStartIndex,nEndIndex;
	
	// 라인의 시작 위치를 찾는다.
	nStartIndex = GetLineStartPosition(nLine);

	// 다음 라인의 시작 위치를 찾는다.
	if (nLine < nMaxLine)
		nEndIndex = GetLineEndPosition(nLine);
	else
		nEndIndex = m_str.GetLength();

	// 라인의 스트링을 읽어 들인다.
	CString strLine;
	strLine = GetString(nStartIndex,nEndIndex);

	// 라인에서 라인피드 문자가 있을 경우 자동 삭제 된다.
	strLine.Remove('\r');
	// 라인에서 뉴라인 문자가 있을 경우 자동 삭제 된다.
	strLine.Remove('\n');
	
	return strLine;
}

////////////////////////////////////////////////////////
// 파일에서 nLine위치의 문자열을 반환한다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 NULL을 리턴한다.
BOOL CDataFile::GetLineString(UINT nLine,CString& rtnStr)
{
	if(!m_bOpened) return FALSE;

	// 파일의 라인수를 읽는다.
	UINT nMaxLine = GetTotalLines();
	
	// 읽고자 하는 라인번호가 적당한지를 조사한다.
	if (nLine > nMaxLine || nLine < 1) return FALSE;

	int nStartIndex,nEndIndex;
	
	// 라인의 시작 위치를 찾는다.
	nStartIndex = GetLineStartPosition(nLine);

	// 다음 라인의 시작 위치를 찾는다.
	if (nLine < nMaxLine)
		nEndIndex = GetLineEndPosition(nLine);
	else
		nEndIndex = m_str.GetLength();

	// 라인의 스트링을 읽어 들인다.
	rtnStr = GetString(nStartIndex,nEndIndex);

	// 라인에서 라인피드 문자가 있을 경우 자동 삭제 된다.
	rtnStr.Remove('\r');
	// 라인에서 뉴라인 문자가 있을 경우 자동 삭제 된다.
	rtnStr.Remove('\n');
	

	return TRUE;
}
////////////////////////////////////////////////////////
// 파일에서 nStart라인부터 nEnd라인 까지의 문자열을 반환한다.
// 이때 반환되는 문자열은 NULL로 종료되는 문자열이다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 NULL을 리턴한다.
CString CDataFile::GetLineString(int nStart, int nEnd)
{
	if(!m_bOpened) return CString("");
	
	// 파일의 라인수를 읽는다.
	int nMaxLine = GetTotalLines();
	
	// 읽고자 하는 라인번호가 적당한지를 조사한다.
	if (nStart < 1 || nEnd > nMaxLine || nStart > nEnd)
	{
		AfxMessageBox(_T("Multi line string read error"));
		return CString("");
	}

	if(nStart == nEnd)
		return GetLineString(nStart);

	int nStartIndex,nEndIndex;
	
	// 라인의 시작 위치를 찾는다.
	nStartIndex = GetLineStartPosition(nStart);

	// 다음 라인의 시작 위치를 찾는다.
	if (nEnd < nMaxLine)
		nEndIndex = GetLineEndPosition(nEnd);
	else
		nEndIndex = m_str.GetLength();

	// 라인(Block)의 스트링을 읽어 들인다.
	CString strLine;
	strLine = GetString(nStartIndex,nEndIndex);


	// 스트링의 끝에 라인피드 문자가 있을 경우 자동 삭제 된다.
	int n = strLine.ReverseFind('\r');
	if (n > 0)
		strLine.Delete(n,1);

	// 스트링의 끝에 뉴라인 문자가 있을 경우 자동 삭제 된다..
	n = strLine.ReverseFind('\n');
	if (n > 0)
		strLine.Delete(n,1);
	
	return strLine;
}

////////////////////////////////////////////////////////
// 파일의 끝에 str로 지정된 한라인의 문자열을 추가 한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 -1을 리턴한다.
//khc
LONG CDataFile::AppendLine(LPCTSTR str)
{
	if(!m_bOpened) return -1;

	int nStringFileLength = m_str.GetLength();
	if(!nStringFileLength) 
		m_arLine.RemoveAll();

	CString strTemp;
	//strTemp = (CString)str + _T("\r\n");	
	strTemp.Format(_T("%s%s"), str, _T("\r\n"));
	Append(strTemp);
	m_arLine.Add(nStringFileLength);
	m_nTotalLines++;

	return GetSize();
}

////////////////////////////////////////////////////////
// 파일의 지정된 라인앞에 문자열을 삽입한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않으면 NULL을 리턴한다.
// khc
LONG CDataFile::InsertLine(UINT nLine, LPCTSTR strInsert)
{
	if(!m_bOpened) return -1;
	CString str;

	int nStringFileLength = m_str.GetLength();
	if(!nStringFileLength)
	{
		//str = (CString)strInsert + _T("\r\n");
		str.Format(_T("%s%s"), strInsert, _T("\r\n"));
		Append(str);
		m_arLine.RemoveAll();
		m_arLine.Add(nStringFileLength);
		m_nTotalLines++;
	}
	else
	{
		// 파일의 라인수를 읽는다.
		UINT nMaxLine = GetTotalLines();
		
		// 삽입하고자 하는 라인번호가 적당한지를 조사한다.
		if (nLine < 1)
		{
			AfxMessageBox(_T("Line string insert error"));
			return -1;
		}

		if(nLine > nMaxLine)
		{
			AppendLine(strInsert);
		}
		else
		{
			// 삽입하고자하는 스트링에 라인피드 및 뉴라인 문자가 없을 경우 자동 추가 된다.	
			str = strInsert;
			int n = str.Find(_T("\r\n"),0);
			if (n < 0)
				str += _T("\r\n");
			
			int nIndex;
			// 라인의 시작위치를 찾는다.
			nIndex = GetLineStartPosition(nLine);
			CString strTemp = GetLineString(nLine);
			Insert(nIndex,str);

			UINT nInserSize = str.GetLength();
			m_arLine.InsertAt(nLine,nIndex+nInserSize);
			m_nTotalLines++;			
			for(nIndex=nLine+1;nIndex < m_nTotalLines; nIndex++)
			{
				UINT nNewValue = m_arLine.GetAt(nIndex);
				nNewValue += nInserSize;
				m_arLine.SetAt(nIndex,nNewValue);
			}
		}
	}
	return GetSize();
}
////////////////////////////////////////////////////////
// 파일에서 nLine의 문자열을 삭제 한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 -1을 리턴한다.
// khc
LONG CDataFile::DeleteLine(UINT nLine)
{
	if(!m_bOpened) return -1;
	
	if(!m_str.GetLength())
		return -1;

	// 파일의 라인수를 읽는다.
	int nMaxLine = GetTotalLines();
	
	// 지우고자 하는 라인번호가 적당한지를 조사한다.
	if (nLine < 1)
	{
		AfxMessageBox(_T("Line string Delete error"));
		return -1;
	}
	if ((int)nLine > nMaxLine)
	{
		AfxMessageBox(_T("Delete Line Number mismatched"));
		return -1;
	}
	
	int nStartIndex,nEndIndex;
	// 라인의 시작위치를 찾는다.
	nStartIndex = GetLineStartPosition(nLine);
	// 라인의 종료 위치를 찾는다.
	nEndIndex = GetLineStartPosition(nLine+1);

	int nDeleteSize = nEndIndex-nStartIndex;
	Delete(nStartIndex,nDeleteSize);

	for(int nIndex=nLine;nIndex < m_nTotalLines-1; nIndex++)
	{
		UINT nNewValue = m_arLine.GetAt(nIndex+1);
		nNewValue -= nDeleteSize;
		m_arLine.SetAt(nIndex,nNewValue);
	}
	m_arLine.RemoveAt(m_nTotalLines-1);
	m_nTotalLines--;

	return GetSize();
}

////////////////////////////////////////////////////////
// 파일에서 nLine의 문자열을 lpszNew의 문자열로 변경한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 -1을 리턴한다.
//khc
LONG CDataFile::ReplaceLine(UINT nLine,LPCTSTR lpszNew)
{
	if(!m_bOpened) return -1;
	// 파일의 라인수를 읽는다.
	UINT nMaxLine = GetTotalLines();
	// 변경 하고자 하는 라인번호가 적당한지를 조사한다.
	if (nLine < 1 || nLine > nMaxLine)
	{
		AfxMessageBox(_T("Line string Replace Error"));
		return -1;
	}
	
	DeleteLine(nLine);
	InsertLine(nLine,lpszNew);
	return GetSize();
}

////////////////////////////////////////////////////////
// 파일에서 nLine의 문자열중 lpszOld부분을 lpszNew의 문자열로 변경한다.
// 변경된 파일의 크기를 LONG단위로 리턴한다.
// 파일이 열려 있지 않거나, nLine이 잘못 지정되면 -1을 리턴한다.
LONG CDataFile::ReplaceLineString(UINT nLine,LPCTSTR lpszOld,LPCTSTR lpszNew)
{
	if(!m_bOpened) return -1;
	// 파일의 라인수를 읽는다.
	UINT nMaxLine = GetTotalLines();
	// 변경 하고자 하는 라인번호가 적당한지를 조사한다.
	if (nLine < 1 || nLine > nMaxLine)
	{
		AfxMessageBox(_T("Line string Replace Error"));
		return -1;
	}
	CString strLine = GetLineString(nLine);
	strLine.Replace(lpszOld,lpszNew);

	DeleteLine(nLine);
	InsertLine(nLine,strLine);
	return GetSize();
}

// nSrcLine의 라인을, int nDestLine으로 옮긴다.
//khc
BOOL CDataFile::MoveLine(int nSrcLine, int nDestLine)
{
	if(!m_bOpened) return FALSE;
	// 파일의 라인수를 읽는다.
	int nMaxLine = GetTotalLines();
	
	// 이동 하고자 하는 라인번호가 적당한지를 조사한다.
	if (nSrcLine < 1 || nDestLine < 1 || nSrcLine > nMaxLine || nDestLine > nMaxLine)
	{
		AfxMessageBox(_T("Line string move error"));
		return FALSE;
	}

	CString strLine;
	strLine = GetLineString(nSrcLine);
	if(nSrcLine != nDestLine )
	{
		InsertLine(nDestLine,strLine);
		DeleteLine(nSrcLine);
	}

	return TRUE;
}

BOOL CDataFile::ReadData(CString strDataGroup, CString strDescription,int nTh,double &fData)
{
	CString strValue, strLine;
	int nLine, nStartRow;

	// 파일에서 데이타 그룹의 시작 라인 번호를 읽어 온다.
	nStartRow = GetLineNumber(strDataGroup,1);
	if(nStartRow < 0)
	{
		strLine.Format(_T("None Exist Group Name in %s file"),m_FilePath);
		AfxMessageBox(strLine,MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	nStartRow++;
	// 데이터 기술자(Decriptor)를 기준으로 데이터의 라인 번호를 얻는다.
	nLine = GetLineNumOfDescriptor(strDescription,nStartRow);
	if(nLine < 0)
	{
		strLine.Format(_T("None Exist Data Descriptor in %s file"),m_FilePath);
		AfxMessageBox(strLine,MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	// 파일로 부터 데이타 문자열을 읽는다.
	strLine = GetLineString(nLine);
	// 데이터 라인으로부터 주석문을 제거 한다.
	strValue = ExtractFirstWord(&strLine, _T("//"),TRUE); // Row Index 의 String
	// 데이타 라인으로브터 기술자를 제거한다.
	ExtractFirstWord(&strValue, _T("=")); // Row Index 의 String

	if(nTh <= 0)
	{
		AfxMessageBox(_T("Data Index Muse be more than 0"),MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	
	// 데이타 n번째 이전 데이터를 제거한다.
	for(int i = 0; i < nTh-1; i++)
		ExtractFirstWord(&strValue, _T(","));

	fData = _tcstod(strValue,NULL);

	return TRUE;
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
CString CDataFile::ExtractFirstWord(CString *str, CString sep, BOOL bTrim)
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

CString CDataFile::GetAllString()
{
	return m_str;
}