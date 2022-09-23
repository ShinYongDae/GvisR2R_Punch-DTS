#if !defined(AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_)
#define AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamMaster.h : header file
//

#include "CamMasterDefine.h"
#include "PcsRgn.h"
#include "../Global/GlobalDefine.h"


/////////////////////////////////////////////////////////////////////////////
// CCamMaster window

class CCamMaster : public CWnd
{
	_MasterPanel MstPnl;

	short FrameRgnNum;									// Panel�� �� ��Ʈ�� ��
	int PieceRgnNum;									// Panel�� �� �ǽ� ��
	int m_nCornerNum;									// �ǽ��� ������ ��
	int m_nDummy[MAX_PATH];
	int m_nPieceNum[MAX_STRIP];							// Strip�� �� �ǽ� ��
	REGIONS_FRAME FrameRgnPix[MAX_FRAME_RGN_NUM];
	REGIONS_FRAME_ID FrameRgnID[MAX_FRAME_RGN_NUM];
	REGIONS_PIECE_2 PieceRgnPix[MAX_PIECE_RGN_NUM];
//	REGIONS_PIECE_ID PieceRgnID[MAX_PIECE_RGN_NUM];
	CPoint	**PolygonPoints;
	int m_MkOrder2PnlPcsIdx[MAX_PIECE_RGN_NUM];			// ��ŷ������ �ǽ� �ε��� (�»�ܺ��� ������׷� ��ŷ)
	int m_PnlPcsIdx2MkOrder[MAX_PIECE_RGN_NUM];			// ķ������ �ǽ� �ε����� ��ŷ����
	int m_MkOrder2PnlPcsIdxMirror[MAX_PIECE_RGN_NUM];	// �¿� �̷��� ��ŷ������ �ǽ� �ε��� (�»�ܺ��� ������׷� ��ŷ)
	int m_PnlPcsIdx2MkOrderMirror[MAX_PIECE_RGN_NUM];	// �¿� �̷��� ķ������ �ǽ� �ε����� ��ŷ����

	int m_PcsIdxMirror[MAX_PIECE_RGN_NUM];				// ķ������ �ǽ� �ε����� ���� �¿� �̷��� �ǽ� �ε���

	void AllocPolygonRgnData();
	void FreePolygonRgnData();

	CString m_sPathCamSpecDir, m_sModel, m_sLayer, m_sLayerUp;
	long m_PinFileSize, m_PcsFileSize, m_AlignFileSize[4];


	BOOL LoadMasterSpec();
	void LoadPinImg();
	void LoadAlignImg();
 	CString GetCamPxlRes();
	BOOL LoadStripRgnFromCam();
	BOOL LoadPcsRgnFromCam();
	void SetMasterPanelInfo(); // for DTS
	BOOL LoadStripPieceRegion_Binary();
	void LoadPcsImg(CString sPath);
	void LoadCadImg();
	BOOL LoadCadAlignMkPos();
	BOOL LoadCad2PntAlignMkPos(CString sPath);
	BOOL LoadCad4PntAlignMkPos(CString sPath);
	void SetCad2PntAlignMkPos();
	void SetCad4PntAlignMkPos();
	void LoadPcsImg();

	BOOL PinImgBufAlloc(TCHAR *strCADImg, BOOL bOppLayerF);
	void PinImgFree();
	void DeleteFileInFolder(CString sPathDir);
	int CheckPath(CString strPath);
	void PcsImgFree();
	BOOL PcsImgBufAlloc(TCHAR *strCADImg, BOOL bOppLayerF);
	void CADLinkImgFree();
	BOOL CADImgBufAlloc(TCHAR *strCADImg, int CellNum, BOOL bOppLayerF);
	void AlignImgFree(int nPos=-1); // -1 : All
	BOOL AlignImgBufAlloc(TCHAR *strCADImg, int nPos);
	void InitOrderingMk();
	void WriteOrderingMk();
	void WriteOrderingMkMirror();

// Construction
public:
	CCamMaster();

// Attributes
public:
	BOOL m_bUse;

	stMasterInfo MasterInfo;
	REGION_STRIP* m_pCellRgn;
	CPcsRgn* m_pPcsRgn;
	UCHAR *m_pPinImg, *m_pPcsImg, *m_pAlignImg[4];

	UCHAR *m_pCADCellImg[MAX_CELL_NUM];
	long m_CADFileSize[MAX_CELL_NUM];
	int   CellLoadID[MAX_CELL_NUM];
	int   CellInspID[MAX_CELL_NUM];

	int m_lPcsNum;									// ķ�����Ϳ��� ������ ��ŷ ���ؽ��� ��.
	//long m_lPcsNum;									// ķ�����Ϳ��� ������ ��ŷ ���ؽ��� ��.
	stAlignMark m_stAlignMk;						// ķ�����Ϳ��� ������ 2point ���Ŀ��� �̹��� ��ǥ.
	stAlignMark2 m_stAlignMk2;						// ķ�����Ϳ��� ������ 4point ���Ŀ��� �̹��� ��ǥ.

	stPieceMark m_stPcsMk[MAX_PCS];					// ķ�����Ϳ��� ������ ��ŷ �ε��� ��ǥ.
	stPieceMark m_stPcsMkAdj[2][MAX_PCS];			// 2Point ��������� ������ ��ǥ[��/��].


// Operations
public:
	void Init(CString sModel, CString sLayer, CString sPathSpec = _T(""), CString sLayerUp=_T(""));
	BOOL LoadMstInfo();
	BOOL WriteStripPieceRegion_Text(CString sBasePath, CString sLot);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamMaster)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCamMaster();

	// for PcsRgn
	BOOL GetMkMatrix(int nPcsId, int &nC, int &nR);					// Shot���� nC:0~ , nR:0~
	BOOL GetMkMatrix(int nPcsId, int &nStrip, int &nC, int &nR);	// nStrip:0~3 , Strip���� nC:0~ , nR:0~
	void SetPinPos(int nCam, CfPoint ptPnt);
	void GetShotRowCol(int& nR, int& nC);
	CfPoint GetMkPnt(int nCam, int nPcsId);							// nCam : [0]-LeftCamera, [1]-RightCamera
	void GetMkPnt(int nC, int nR, int &nPcsId, CfPoint &ptPnt);		// nC, nR : Cam(or Reelmap) ����
	void SetMkPnt(int nCam, int nMkIdx, CfPoint fPt);				// nCam : [0]-LeftCamera, [1]-RightCamera
	CRect GetPcsRgn(int nPcsId);
	void GetPcsRgn(int nC, int nR, int &nPcsId, CRect &ptRect);		// nC, nR : Cam(or Reelmap) ����
	int GetTotPcs();
	double GetPcsWidth();
	double GetPcsHeight();

	// for Punching order
	int GetPnlMkPcsIdx(int nMkIdx);									// �ǳ� ��ü �ǽ��� ��ŷ������ ���� �ǽ� �ε���
	int GetPnlMkPcsOrder(int nPcsIdx);								// �ǳ� ��ü �ǽ� �ε����� ���� ��ŷ���� �ε���
	int GetPnlMkPcsIdxMirror(int nMkIdx);							// �ǳ� ��ü �ǽ��� ��ŷ������ ���� �ǽ� �ε���
	int GetPnlMkPcsOrderMirror(int nPcsIdx);						// �ǳ� ��ü �ǽ� �ε����� ���� ��ŷ���� �ε���
	int GetPnlPcsIdxMirror(int nIdx);								// �ǳ� ��ü �ǽ� �ε����� ���� �¿�̷��� �ε���

	// Generated message map functions
protected:
	//{{AFX_MSG(CCamMaster)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_)
