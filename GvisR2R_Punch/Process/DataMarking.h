#if !defined(AFX_DATAMARKING_H__D6995996_1066_4304_81D0_C6243010656C__INCLUDED_)
#define AFX_DATAMARKING_H__D6995996_1066_4304_81D0_C6243010656C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMarking.h : header file
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDataMarking window
//
// pcr파일의 내용을 받아서 릴맵화면을 표시하고, 마킹작업 정보로 활용.
// m_pPcr에 저장되는 데이터는 PCR파일에서 읽어오는 순서로 인덱스에 저장됨.
// PCR에서 읽어온 순서로 데이터를 다시 로딩하기 때문에 불량피스 인덱스와 별도의 마킹을 위한 피스 인덱스(m_pDefPcsMk)를 확인.

class CDataMarking : public CWnd 
{	// pcr파일에서 읽어온 정보(불량피스)를 보관. - 총 불량 피스수.

	void FreeMem();

	// Construction
public:
	CDataMarking(CWnd* pParent=NULL);
 	//CDataMarking(int nTotPnl, CWnd* pParent=NULL);

// Attributes
public:
	int m_nTotRealDef;
	int m_nIdx;			// 릴맵화면 표시 인덱스
	CString m_sModel, m_sLayer, m_sLot;
	int m_nErrPnl, m_nSerial, m_nTotDef, m_nCamId;
	int *m_pLayer;		// 0:Up, 1:Dn 
	int *m_pDefPcsMk;	// 마킹순서가 정해진 불량 피스 인덱스 : PCR에서 읽어온 순서에 불량피스 인덱스와 별도로 마킹을 위한 인덱스를 저장.
	int *m_pDefPcs;		// 불량 피스 인덱스
	int *m_pDefType;	// 불량코드 (0:양품, 1~:불량코드)
	int *m_pCell;		// CellNum
	int *m_pImgSz;		// ImageSize
	int *m_pImg;		// ImageNum
	int *m_pMk;			// strMarkingCode : -2 (NoMarking)
	CPoint *m_pDefPos;	// BadPointPosition
	int m_nCol, m_nRow;	// 사용안함.
	CPoint m_MkOffset;	// From PCS Center. - 사용안함.

// Operations
public:
	void Init(int nSerial=-1, int nTot=0); // 제품시리얼, Shot내 총불량 피스수

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
