#pragma once

// Allocation for HeasungDS RTR  ========================================================================================
#define RGN_STRIP_VARIABLE_NUM	23		// REGION_STRIP 구조체에서 배열을 제외한 변수의 갯수
#define MAX_STRIP				4
#define MAX_NodeNumX			100		//200
#define MAX_NodeNumY			100		//200
#define MAX_NMSWATH				100

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
		dPixelSize = 0.0;
		strMasterLocation = _T(""); strCADImgPath = _T(""); strCADImgBackUpPath = _T(""); strTwoMetalOppLayer = _T("");
		nImageCompression = 0; nNumOfAlignPoint = 0;
		bTwoMetalInspection = FALSE;
		dTestRgnW = 0.0; dTestRgnH = 0.0;				// [PANEL INFO] InspectionWidth, InspectionHeight [mm]
		dTestRgnLeft = 0.0; dTestRgnTop = 0.0;			// [ORIGIN COORD] MX, MY [mm]
		dPinPosX = 0.0; dPinPosY = 0.0;					// [ORIGIN COORD] PX, PY [mm]
	}
};

typedef struct {
	int nMode;											// 0 : Full Mode, 1 : Strip Mode
	int nMSwath;										// 모터가 움직일 회수.
	int NodeNumX, NodeNumY;								// 각 축당 셀 갯수
	int PanelPixelX, PanelPixelY;						// 전체 판넬 이미지의 크기
	int OvrXPix, OvrYPix;								// 각 축으로 오버랩되는 부분의 크기
	int ProcSizeX, ProcSizeY;							// 각 축당 셀의 크기
	int nCell;											// 전체 셀 갯수
	int nIPU;											// AOI의 IPU 갯수
	int CameraValidPixel;								// 실제 검사하기 위해 사용되는 이미지의 X축 크기
	int nScanMarginX, nScanMarginY;						// 실제 사용되는 이미지에 각 축방향으로 더해지는 여분 이미지의 크기 Ex) nScanMarginX = (8192 ? CameraValidPixel)/2
	int nRegionX, nRegionY;								// Reserved Data
	int nCellDivideX, nCellDivideY;						// 한 스트립당 각 축 방향 셀 갯수
	int nCADPinPosPixX, nCADPinPosPixY;					// Pin Position X, y
	int nProcMarginX, nProcMarginY;						// Reserved Data
	int StPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 X 위치
	int StPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 왼쪽 윗부분의 Y 위치
	int EdPosX[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 X 위치
	int EdPosY[MAX_NodeNumX * MAX_NodeNumY];			// 각 셀의 오른쪽 아래부분의 Y 위치
	int XSwathPixPos[MAX_NMSWATH];						// 모터가 움직이기 시작하는 위치
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
		X = 0.0; Y = 0.0;
	}
};

typedef struct {
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : 원본 1 : 상하미러  2 : 좌퓖E肩?
	int FRotate;	//0 : 0도  1 : 90도  2 : 180도  3 : 270도
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
struct _Point {
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
	int nMstPcsIdx, nMstStripIdx;
	int nMstStripRow, nMstStripCol;
	int nMstPcsRow, nMstPcsCol;
	_Point MkPos;
	_Rect Area;

	_MasterPiece()
	{
		nMstPcsIdx = -1; nMstStripIdx = -1;		// from 0 ~
		nMstStripRow = -1; nMstStripCol = -1;	// from 0 ~
		nMstPcsRow = -1; nMstPcsCol = -1;		// from 0 ~
		MkPos.dX = 0.0; MkPos.dY = 0.0;			// [mm]
		Area.dLeft = 0.0; Area.dTop = 0.0; Area.dRight = 0.0; Area.dBottom = 0.0; // [mm]
	}
};

struct _MasterStrip {
	int nMstStripIdx;
	int nTotalPiece;
	int nTotalPieceRow, nTotalPieceCol;
	_Rect Area;
	_MasterPiece Piece[MAX_NodeNumY][MAX_NodeNumX]; // Piece[Row][Col] 

	_MasterStrip()
	{
		nMstStripIdx = -1;
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
	CString sPathPinImg, sPathPcsImg, sPathAlignImg[4]; // * CadlinkImg는 검사용 Cell Image임.
	CString sPath2ptAlignAndMkPos, sPath4ptAlignAndMkPos;
	CString sPathPinPos;								// [ORIGIN COORD] (원점): PX, PY ; (검사영역 좌상): MX, MY
														// [PANEL INFO] (검사영역 W, H): InspectionWidth, InspectionHeight

	_MasterStrip Strip[MAX_STRIP];	// Strip[Row] - CamMaster의 Row, Col 순으로
	_MasterPiece Piece[MAX_PCS];	// CamMaster의 인덱스 순으로

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
