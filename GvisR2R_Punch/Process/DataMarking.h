#if !defined(AFX_DATAMARKING_H__D6995996_1066_4304_81D0_C6243010656C__INCLUDED_)
#define AFX_DATAMARKING_H__D6995996_1066_4304_81D0_C6243010656C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMarking.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMarking window

class CDataMarking : public CWnd
{

	void FreeMem();

	// Construction
public:
	CDataMarking(CWnd* pParent=NULL);
// 	CDataMarking(int nTotPnl, CWnd* pParent=NULL);

// Attributes
public:
	int m_nTotRealDef;
	int m_nIdx; //m_nTotPnl, 
	CString m_sModel, m_sLayer, m_sLot;
	int m_nErrPnl, m_nSerial, m_nTotDef, m_nCamId;
	int *m_pLayer;	// 0:Up, 1:Dn
	int *m_pDefPcs;
	int *m_pDefType;
	int *m_pCell;
	int *m_pImgSz;
	int *m_pImg;
	int *m_pMk;
	CPoint *m_pDefPos;
	int m_nCol, m_nRow;
	CPoint m_MkOffset; // From PCS Center.

// Operations
public:
	void Init(int nSerial=-1, int nTot=0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMarking)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDataMarking();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataMarking)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMARKING_H__D6995996_1066_4304_81D0_C6243010656C__INCLUDED_)
