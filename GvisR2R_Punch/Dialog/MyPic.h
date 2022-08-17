#if !defined(AFX_MYPIC_H__99A972BA_5BA8_4153_9B8D_8CB335D8350A__INCLUDED_)
#define AFX_MYPIC_H__99A972BA_5BA8_4153_9B8D_8CB335D8350A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPic window

class CMyPic : public CWnd
{
	IPicture* m_IPicture; // Same As LPPICTURE (typedef IPicture __RPC_FAR *LPPICTURE)

	void FreePic();
	BOOL LoadPic2Buff(BYTE *pPic, int nSize);

// Construction
public:
	CMyPic();

// Attributes
public:

// Operations
public:
	BOOL Load(CString sFilePathName);
	//BOOL Load(UINT ResourceName, LPCSTR ResourceType);
	BOOL Load(UINT ResourceName, LPCWSTR ResourceType);
	BOOL Show(CDC* pDC, CPoint LeftTop, CPoint WidthHeight); //, int MagnifyX, int MagnifyY);
	BOOL Show(CDC* pDC, CRect DrawRect);
	BOOL Save(CString sFilePathName);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPic)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPIC_H__99A972BA_5BA8_4153_9B8D_8CB335D8350A__INCLUDED_)
