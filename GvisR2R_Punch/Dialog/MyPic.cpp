// MyPic.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "MyPic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ERROR_TITLE _T("CMyPic Error") // Error Title (Related To This Class)...

/////////////////////////////////////////////////////////////////////////////
// CMyPic

CMyPic::CMyPic()
{
	m_IPicture = NULL;
}

CMyPic::~CMyPic()
{
	if(m_IPicture != NULL) 
	{
		FreePic(); // Important - Avoid Leaks...
		m_IPicture = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyPic, CWnd)
	//{{AFX_MSG_MAP(CMyPic)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPic message handlers

void CMyPic::FreePic()
{
	if(m_IPicture != NULL)
	{
		m_IPicture->Release();
		m_IPicture = NULL;
	}
}

BOOL CMyPic::LoadPic2Buff(BYTE *pPic, int nSize) // Buff : m_IPicture
{
	BOOL bResult = FALSE;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);

	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, _T("Can not allocate enough memory\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}

	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pPic, nSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr;
		if((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_IPicture)) == E_NOINTERFACE)
		{
			HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
			MessageBoxEx(hWnd, _T("IPicture interface is not supported\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
			return(FALSE);
		}
		else // S_OK
		{
			pStream->Release();
			pStream = NULL;
			bResult = TRUE;
		}
	}

	//FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	if(DeleteObject(hGlobal))
		hGlobal = NULL;

	return(bResult);

}

BOOL CMyPic::Load(UINT ResourceName, LPCWSTR ResourceType)
{
	BOOL bResult = FALSE;

	HGLOBAL		hGlobal = NULL;
	HRSRC		hSource = NULL;
	LPVOID		lpVoid  = NULL;
	int			nSize   = 0;

	if(m_IPicture != NULL) FreePic(); // Important - Avoid Leaks...

	hSource = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(ResourceName), ResourceType);

	if(hSource == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, _T("FindResource() Failed\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}

	hGlobal = LoadResource(AfxGetResourceHandle(), hSource);
	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, _T("LoadResource() Failed\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}

	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, _T("LockResource() Failed\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}

	nSize = (UINT)SizeofResource(AfxGetResourceHandle(), hSource);
	if(LoadPic2Buff((BYTE*)hGlobal, nSize)) bResult = TRUE;

	UnlockResource(hGlobal); // 16Bit Windows Needs This
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)


	return(bResult);
}

BOOL CMyPic::Load(CString sFilePathName)
{
	BOOL bResult = FALSE;
	CFile PictureFile;
	CFileException e;
	int	nSize = 0;

	if(m_IPicture != NULL) FreePic(); // Important - Avoid Leaks...

	if(PictureFile.Open(sFilePathName, CFile::modeRead | CFile::typeBinary, &e))
	{
		nSize = PictureFile.GetLength();
		BYTE* pBuffer = new BYTE[nSize];
	
		if(PictureFile.Read(pBuffer, nSize) > 0)
		{
			if(LoadPic2Buff(pBuffer, nSize))	bResult = TRUE;
		}

		PictureFile.Close();
		//delete [] pBuffer;
		delete pBuffer;
	}
	else // Open Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, szCause, ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}

	return(bResult);
}

BOOL CMyPic::Show(CDC *pDC, CRect DrawRect)
{
    if (pDC == NULL || m_IPicture == NULL) 
		return FALSE;
    
    long Width  = 0;
    long Height = 0;
    m_IPicture->get_Width(&Width);
    m_IPicture->get_Height(&Height);

    HRESULT hrP = NULL;
    
    hrP = m_IPicture->Render(pDC->m_hDC,
							  DrawRect.left,                  // Left
							  DrawRect.top,                   // Top
							  DrawRect.right - DrawRect.left, // Right
							  DrawRect.bottom - DrawRect.top, // Bottom
							  0,
							  Height,
							  Width,
							  -Height,
							  &DrawRect);

    if (SUCCEEDED(hrP)) 
		return(TRUE);

	HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
	MessageBoxEx(hWnd, _T("Can not allocate enough memory\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);

    return(FALSE);
}

BOOL CMyPic::Show(CDC *pDC, CPoint LeftTop, CPoint WidthHeight)//, int MagnifyX, int MagnifyY)
{
    if (pDC == NULL || m_IPicture == NULL) return FALSE;
    
    long Width  = 0;
    long Height = 0;
    m_IPicture->get_Width(&Width);
    m_IPicture->get_Height(&Height);

	CRect DrawRect(LeftTop.x, LeftTop.y, WidthHeight.x, WidthHeight.y);

    HRESULT hrP = NULL;

    hrP = m_IPicture->Render(pDC->m_hDC,
							  LeftTop.x,				// Left
							  LeftTop.y,				// Top
							  WidthHeight.x,			// Width
							  WidthHeight.y,			// Height
							  0,
							  Height,
							  Width,
							  -Height,
							  &DrawRect);

    if(SUCCEEDED(hrP)) return(TRUE);

	HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
	MessageBoxEx(hWnd, _T("Can not allocate enough memory\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
    return(FALSE);
}

BOOL CMyPic::Save(CString sFilePathName)
{
	BOOL bResult = FALSE;
	ILockBytes *Buffer = 0;
	IStorage   *pStorage = 0;
	IStream    *FileStream = 0;
	BYTE	   *BufferBytes;
	STATSTG		BytesStatistics;
	DWORD		OutData;
	long		OutStream;
	CFile		BitmapFile;	CFileException e;
	double		SkipFloat = 0;
	DWORD		ByteSkip = 0;
	_ULARGE_INTEGER RealData;

	CreateILockBytesOnHGlobal(NULL, TRUE, &Buffer); // Create ILockBytes Buffer

	HRESULT hr = ::StgCreateDocfileOnILockBytes(Buffer,
				 STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);

	hr = pStorage->CreateStream(L"PICTURE",
		 STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &FileStream);

	m_IPicture->SaveAsFile(FileStream, TRUE, &OutStream); // Copy Data Stream
	FileStream->Release();
	pStorage->Release();
	Buffer->Flush(); 

	// Get Statistics For Final Size Of Byte Array
	Buffer->Stat(&BytesStatistics, STATFLAG_NONAME);

	// Cut UnNeeded Data Coming From SaveAsFile() (Leave Only "Pure" Picture Data)
	SkipFloat = (double(OutStream) / 512); // Must Be In a 512 Blocks...
	if(SkipFloat > DWORD(SkipFloat)) ByteSkip = (DWORD)SkipFloat + 1;
	else ByteSkip = (DWORD)SkipFloat;
	ByteSkip = ByteSkip * 512; // Must Be In a 512 Blocks...
	
	// Find Difference Between The Two Values
	ByteSkip = (DWORD)(BytesStatistics.cbSize.QuadPart - ByteSkip);

	// Allocate Only The "Pure" Picture Data
	RealData.LowPart = 0;
	RealData.HighPart = 0;
	RealData.QuadPart = ByteSkip;
	BufferBytes = (BYTE*)malloc(OutStream);
	memset(BufferBytes,' ',OutStream);
	if(BufferBytes == NULL)
	{
		Buffer->Release();
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, _T("Can not allocate enough memory\t"), ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	}

	Buffer->ReadAt(RealData, BufferBytes, OutStream, &OutData);

	if(BitmapFile.Open(sFilePathName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite, &e))
	{
		BitmapFile.Write(BufferBytes, OutData);
		BitmapFile.Close();
		bResult = TRUE;
	}
	else // Write File Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, szCause, ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}
	
	Buffer->Release();
	free(BufferBytes);

	return(bResult);
}
