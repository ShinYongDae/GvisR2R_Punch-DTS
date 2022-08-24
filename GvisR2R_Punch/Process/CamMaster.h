#if !defined(AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_)
#define AFX_CAMMASTER_H__0311C7CE_23AD_4FEF_9FEF_093106E40668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamMaster.h : header file
//

#include "PcsRgn.h"
#include "../Global/GlobalDefine.h"

// Allocation for HeasungDS RTR  ========================================================================================
#define RGN_STRIP_VARIABLE_NUM	23		// REGION_STRIP ����ü���� �迭�� ������ ������ ����
#define MAX_STRIP				4
#define MAX_NodeNumX			100		//200
#define MAX_NodeNumY			25		//200
#define MAX_NMSWATH				4		//100

#define MAX_PCS					10000

#define MAX_PIECE_NODE_X_NUM	100		//600		
#define MAX_PIECE_NODE_Y_NUM	100		//600

#define MAX_FRAME_RGN_NUM		100//4		//1000		// Maximum number of Strip regions
#define MAX_STRIP_X_NUM			10//1		//10			
#define MAX_STRIP_Y_NUM			10//4		//10			
#define MAX_PIECE_RGN_NUM		(MAX_PIECE_NODE_X_NUM * MAX_PIECE_NODE_Y_NUM)		// Maximum number of Cell regions
//=======================================================================================================================

struct stMasterInfo
{
	double dPixelSize;							// [MACHINE] PixelSize [um]
	CString strMasterLocation, strCADImgPath, strCADImgBackUpPath, strTwoMetalOppLayer;
	int nImageCompression, nNumOfAlignPoint;	// [SPEC] NumOfAlignPoint
	BOOL bTwoMetalInspection;
	double dTestRgnW, dTestRgnH;				// [PANEL INFO] InspectionWidth, InspectionHeight [mm]
	double dTestRgnLeft, dTestRgnTop;			// [ORIGIN COORD] MX, MY [mm]
	double dPinPosX, dPinPosY;					// [ORIGIN COORD] PX, PY [mm]

	stMasterInfo()
	{
		dPixelSize=0.0;
		strMasterLocation=_T(""); strCADImgPath=_T(""); strCADImgBackUpPath=_T(""); strTwoMetalOppLayer=_T("");
		nImageCompression = 0; nNumOfAlignPoint = 0;
		bTwoMetalInspection=FALSE;
		dTestRgnW = 0.0; dTestRgnH = 0.0;				// [PANEL INFO] InspectionWidth, InspectionHeight [mm]
		dTestRgnLeft = 0.0; dTestRgnTop = 0.0;			// [ORIGIN COORD] MX, MY [mm]
		dPinPosX = 0.0; dPinPosY = 0.0;					// [ORIGIN COORD] PX, PY [mm]
	}
};

typedef struct {
	int nMode;											// 0 : Full Mode, 1 : Strip Mode
	int nMSwath;										// ���Ͱ� ������ ȸ��.
	int NodeNumX, NodeNumY;								// �� ��� �� ����
	int PanelPixelX, PanelPixelY;						// ��ü �ǳ� �̹����� ũ��
	int OvrXPix, OvrYPix;								// �� ������ �������Ǵ� �κ��� ũ��
	int ProcSizeX, ProcSizeY;							// �� ��� ���� ũ��
	int nCell;											// ��ü �� ����
	int nIPU;											// AOI�� IPU ����
	int CameraValidPixel;								// ���� �˻��ϱ� ���� ���Ǵ� �̹����� X�� ũ��
	int nScanMarginX, nScanMarginY;						// ���� ���Ǵ� �̹����� �� ��������� �������� ���� �̹����� ũ�� Ex) nScanMarginX = (8192 ? CameraValidPixel)/2
	int nRegionX, nRegionY;								// Reserved Data
	int nCellDivideX, nCellDivideY;						// �� ��Ʈ���� �� �� ���� �� ����
	int nCADPinPosPixX, nCADPinPosPixY;					// Pin Position X, y
	int nProcMarginX, nProcMarginY;						// Reserved Data
	int StPosX[MAX_NodeNumX * MAX_NodeNumY];			// �� ���� ���� ���κ��� X ��ġ
	int StPosY[MAX_NodeNumX * MAX_NodeNumY];			// �� ���� ���� ���κ��� Y ��ġ
	int EdPosX[MAX_NodeNumX * MAX_NodeNumY];			// �� ���� ������ �Ʒ��κ��� X ��ġ
	int EdPosY[MAX_NodeNumX * MAX_NodeNumY];			// �� ���� ������ �Ʒ��κ��� Y ��ġ
	int XSwathPixPos[MAX_NMSWATH];						// ���Ͱ� �����̱� �����ϴ� ��ġ
} REGION_STRIP;		//Total 100476 Byte

struct stAlignMark2
{
	float X0, Y0;
	float X1, Y1;
	float X2, Y2;
	float X3, Y3;

	stAlignMark2()
	{
		X0 = 0.0; Y0 = 0.0;
		X1 = 0.0; Y1 = 0.0;
		X2 = 0.0; Y2 = 0.0;
		X3 = 0.0; Y3 = 0.0;
	}
};

struct stAlignMark
{
	float X0, Y0;
	float X1, Y1;

	stAlignMark()
	{
		X0 = 0.0; Y0 = 0.0;
		X1 = 0.0; Y1 = 0.0;
	}
};

struct stPieceMark
{
	float X, Y;

	stPieceMark()
	{
		X=0.0; Y=0.0;
	}
};

typedef struct {
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : ���� 1 : ���Ϲ̷�  2 : �¿�E̷?
	int FRotate;	//0 : 0��  1 : 90��  2 : 180��  3 : 270��
} REGIONS_FRAME;


typedef struct {
	int nId;
	int Col;
	int Row;
} REGIONS_FRAME_ID;

//typedef struct {
//	int nWidth;
//	int nHeight;
//} REGIONS_PIECE_ID;


// for DTS
struct _Point{
	double dX, dY;

	_Point()
	{
		dX = 0.0; dY = 0.0;
	}
};

struct _Rect {
	double dLeft, dTop, dRight, dBottom;

	_Rect()
	{
		dLeft = 0.0; dTop = 0.0; dRight = 0.0; dBottom = 0.0;
	}
};

struct _MasterPiece {
	int nMstPieceIndex;
	_Point MkPos;
	_Rect Area;

	_MasterPiece()
	{
		nMstPieceIndex = -1;
		MkPos.dX = 0.0; MkPos.dY = 0.0; // [mm]
		Area.dLeft = 0.0; Area.dTop = 0.0; Area.dRight = 0.0; Area.dBottom = 0.0; // [mm]
	}
};

struct _MasterStrip {
	int nMstStripIndex;
	int nTotalPiece;
	int nTotalPieceRow, nTotalPieceCol;
	_Rect Area;
	_MasterPiece Piece[MAX_NodeNumY][MAX_NodeNumX]; // Piece[Row][Col] 

	_MasterStrip()
	{
		nMstStripIndex = -1;
		nTotalPiece = 0;
		nTotalPieceRow = 0; nTotalPieceCol = 0;
		Area.dLeft = 0.0; Area.dTop = 0.0; Area.dRight = 0.0; Area.dBottom = 0.0; // [mm]
	}
};

struct _MasterPanel {
	int nTotalStrip; // Strip Index : 0 ~ (TotalStrip-1)
	int nTotalStripRow, nTotalStripCol;
	int nTotalPiece, nPcsCorner;
	int nTotalAlignPos;
	_Point AlignPos[4];
	_Point PinPos;
	_Rect Area;
	CString sPathPinImg, sPathPcsImg, sPathAlignImg[4]; // * CadlinkImg�� �˻�� Cell Image��.
	CString sPath2ptAlignAndMkPos, sPath4ptAlignAndMkPos;
	CString sPathPinPos;								// [ORIGIN COORD] (����): PX, PY ; (�˻翵�� �»�): MX, MY
														// [PANEL INFO] (�˻翵�� W, H): InspectionWidth, InspectionHeight

	_MasterStrip Strip[MAX_STRIP]; //Strip[Row]

	_MasterPanel()
	{
		nTotalStrip = 0; // Strip Index : 0 ~ (TotalStrip-1)
		nTotalStripRow = 0; nTotalStripCol = 0;
		nTotalPiece = 0;	nPcsCorner = 0;
		nTotalAlignPos = 0;
		AlignPos[0].dX = 0.0; AlignPos[1].dX = 0.0; AlignPos[2].dX = 0.0; AlignPos[3].dX = 0.0; // [mm]
		AlignPos[0].dY = 0.0; AlignPos[1].dY = 0.0; AlignPos[2].dY = 0.0; AlignPos[3].dY = 0.0; // [mm]
		PinPos.dX = 0.0; PinPos.dY = 0.0; // [mm]
		Area.dLeft = 0.0; Area.dTop = 0.0; Area.dRight = 0.0; Area.dBottom = 0.0; // [mm]
		sPathPinImg = _T(""); sPathPcsImg = _T(""); 
		sPathAlignImg[0] = _T(""); sPathAlignImg[1] = _T(""); sPathAlignImg[2] = _T(""); sPathAlignImg[3] = _T("");
		sPath2ptAlignAndMkPos = _T(""); sPath4ptAlignAndMkPos = _T(""); sPathPinPos = _T("");
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCamMaster window

class CCamMaster : public CWnd
{
	_MasterPanel MstPnl;

	short FrameRgnNum;
	int PieceRgnNum;
	int m_nCornerNum;
	int m_nDummy[MAX_PATH];
	int m_nPieceNum[MAX_PATH];
	REGIONS_FRAME FrameRgnPix[MAX_FRAME_RGN_NUM];
	REGIONS_FRAME_ID FrameRgnID[MAX_FRAME_RGN_NUM];
	REGIONS_PIECE_2 PieceRgnPix[MAX_PIECE_RGN_NUM];
//	REGIONS_PIECE_ID PieceRgnID[MAX_PIECE_RGN_NUM];
	CPoint	**PolygonPoints;

	void AllocPolygonRgnData();
	void FreePolygonRgnData();

	CString m_sPathCamSpecDir, m_sModel, m_sLayer, m_sLayerUp;
	long m_PinFileSize, m_PcsFileSize, m_AlignFileSize[4];

	BOOL m_bUseDTS;

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


// Construction
public:
	CCamMaster();

// Attributes
public:
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
	void Init(CString sModel, CString sLayer, CString sPathSpec = _T(""), CString sLayerUp=_T(""), BOOL bUseDTS=TRUE);
	BOOL LoadMstInfo();
	BOOL WriteStripPieceRegion_Text(CString sBasePath, CString sLot);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamMaster)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCamMaster();

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
