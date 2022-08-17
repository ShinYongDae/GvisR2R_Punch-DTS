#if !defined(AFX_MYFILEERRMAP_H__0A938905_C9AE_47E4_8E4F_301A6C0760E3__INCLUDED_)
#define AFX_MYFILEERRMAP_H__0A938905_C9AE_47E4_8E4F_301A6C0760E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileErrMap.h : header file
//


typedef struct stErrPos
{
	int nCol, nRow;
	double dErrX, dErrY;
}ErrPos;

/////////////////////////////////////////////////////////////////////////////
// CMyFileErrMap window

class CMyFileErrMap : public CWnd
{
	CWnd *m_pParent;

	BOOL LoadInfo(CString sPath);
	BOOL LoadX(int nRow, int nIdx, CString sPath);
	BOOL LoadY(int nRow, int nIdx, CString sPath);

// Construction
public:
	CMyFileErrMap(CWnd* pParent=NULL);

// Attributes
public:
	// [Info]
	int m_nCOMPENSATOR_DIMENSION;
	int m_nCOMPENSATOR_ID_X;
	int m_nCOMPENSATOR_ID_Y;

	int m_nAXIS_X_ID;
	int m_nAXIS_Y_ID;

	double m_dAXIS_X_RANGE_START;
	double m_dAXIS_Y_RANGE_START;

	double m_dAXIS_X_POINT_DELTA;
	double m_dAXIS_Y_POINT_DELTA;

	int m_nAXIS_X_TOTAL_POINT;
	int m_nAXIS_Y_TOTAL_POINT;

	// [XY-X], [XY-Y]
	ErrPos *m_pErrPos;

// Operations
public:
	BOOL Open(CString sPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileErrMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFileErrMap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyFileErrMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEERRMAP_H__0A938905_C9AE_47E4_8E4F_301A6C0760E3__INCLUDED_)
