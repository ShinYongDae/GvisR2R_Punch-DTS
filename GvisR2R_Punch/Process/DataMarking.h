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
// pcr������ ������ �޾Ƽ� ����ȭ���� ǥ���ϰ�, ��ŷ�۾� ������ Ȱ��.
// m_pPcr�� ����Ǵ� �����ʹ� PCR���Ͽ��� �о���� ������ �ε����� �����.
// PCR���� �о�� ������ �����͸� �ٽ� �ε��ϱ� ������ �ҷ��ǽ� �ε����� ������ ��ŷ�� ���� �ǽ� �ε���(m_pDefPcsMk)�� Ȯ��.

class CDataMarking : public CWnd 
{	// pcr���Ͽ��� �о�� ����(�ҷ��ǽ�)�� ����. - �� �ҷ� �ǽ���.

	void FreeMem();

	// Construction
public:
	CDataMarking(CWnd* pParent=NULL);
 	//CDataMarking(int nTotPnl, CWnd* pParent=NULL);

// Attributes
public:
	int m_nTotRealDef;
	int m_nIdx;			// ����ȭ�� ǥ�� �ε���
	CString m_sModel, m_sLayer, m_sLot;
	int m_nErrPnl, m_nSerial, m_nTotDef, m_nCamId;
	int *m_pLayer;		// 0:Up, 1:Dn 
	int *m_pDefPcsMk;	// ��ŷ������ ������ �ҷ� �ǽ� �ε��� : PCR���� �о�� ������ �ҷ��ǽ� �ε����� ������ ��ŷ�� ���� �ε����� ����.
	int *m_pDefPcs;		// �ҷ� �ǽ� �ε���
	int *m_pDefType;	// �ҷ��ڵ� (0:��ǰ, 1~:�ҷ��ڵ�)
	int *m_pCell;		// CellNum
	int *m_pImgSz;		// ImageSize
	int *m_pImg;		// ImageNum
	int *m_pMk;			// strMarkingCode : -2 (NoMarking)
	CPoint *m_pDefPos;	// BadPointPosition
	int m_nCol, m_nRow;	// ������.
	CPoint m_MkOffset;	// From PCS Center. - ������.

// Operations
public:
	void Init(int nSerial=-1, int nTot=0); // ��ǰ�ø���, Shot�� �Ѻҷ� �ǽ���

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
