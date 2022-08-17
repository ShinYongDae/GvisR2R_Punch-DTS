// CamMaster.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "CamMaster.h"
#include "../Global/GlobalDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <windowsx.h>

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CCamMaster

CCamMaster::CCamMaster()
{
	int i;

	m_pPinImg = NULL;
	m_PinFileSize = 0;
	m_pAlignImg[0] = NULL;
	m_pAlignImg[1] = NULL;
	m_pAlignImg[2] = NULL;
	m_pAlignImg[3] = NULL;
	m_AlignFileSize[0] = 0;
	m_AlignFileSize[1] = 0;
	m_AlignFileSize[2] = 0;
	m_AlignFileSize[3] = 0;

	m_pPcsImg = NULL;
	m_PcsFileSize = 0;

	m_pCellRgn = NULL;
	m_pPcsRgn = NULL;

	for(i = 0; i < MAX_CELL_NUM; i++)
	{
		m_pCADCellImg[i] = NULL;
		m_CADFileSize[i] = 0;
	}

	m_sPathCamSpecDir=_T("");
	m_sModel=_T("");
	m_sLayer=_T("");

	PolygonPoints = NULL;	//20140116-ndy debug

}

CCamMaster::~CCamMaster()
{
	FreePolygonRgnData();
	PinImgFree();
	AlignImgFree();
	PcsImgFree();
	CADLinkImgFree();
	
	if(m_pCellRgn)
	{
		delete m_pCellRgn;
		m_pCellRgn = NULL;
	}

	if(m_pPcsRgn)
	{
		delete m_pPcsRgn;
		m_pPcsRgn = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCamMaster, CWnd)
	//{{AFX_MSG_MAP(CCamMaster)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCamMaster message handlers
void CCamMaster::Init(CString sPathSpec, CString sModel, CString sLayer, CString sLayerUp)
{
	m_sPathCamSpecDir = sPathSpec;
	m_sModel = sModel;
	m_sLayer = sLayer;
	m_sLayerUp = sLayerUp;
}

BOOL CCamMaster::LoadMstInfo()
{
	CString sPath;

	LoadMasterSpec();
	LoadPinImg();
	LoadAlignImg();
// 	GetCamPxlRes();
	if(LoadStripRgnFromCam())
	{
		if(pDoc->WorkingInfo.System.bStripPcsRgnBin)
			LoadStripPieceRegion_Binary();
		else
			LoadPcsRgnFromCam();

		LoadPcsImg();
		LoadCadImg();

		LoadCadMk(); //.pch
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CCamMaster::LoadMasterSpec()
{
#ifdef USE_CAM_MASTER
	//char szData[100];
	TCHAR szData[100];
	CString sMsg, sPath, sPathDir;
	CFileFind findfile;
	BOOL bRtn = TRUE;
	sPathDir = m_sPathCamSpecDir;

	sPath.Format(_T("%s%s\\%s.ini"), sPathDir, 
								 m_sModel, 
								 m_sLayer);


	if(!findfile.FindFile(sPath))
	{
		sMsg.Format(_T("캠마스터 스팩 파일이 없습니다.\r\n%s"), sPath);
		pView->MsgBox(sMsg);
// 		AfxMessageBox(sMsg);
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("MACHINE"), _T("PixelSize"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.dPixelSize = _tstof(szData); // [um]
	else
	{
		MasterInfo.dPixelSize = 0.0; // [um]
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("MASTERLOCATION1"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.strMasterLocation = CString(szData);
	else
	{
		MasterInfo.strMasterLocation = _T("");
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("SPEC"), _T("Compression"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.nImageCompression = _tstoi(szData);
	else
		MasterInfo.nImageCompression = 1;

	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("CADLINKLOCATION"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.strCADImgPath = CString(szData);
	else
		MasterInfo.strCADImgPath = _T("");

	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("CadLinkLocationBackUp"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.strCADImgBackUpPath = CString(szData);
	else
		MasterInfo.strCADImgBackUpPath = _T("");

	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("TopBottomInspection"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.bTwoMetalInspection = _tstoi(szData);
	else
		MasterInfo.bTwoMetalInspection = FALSE;

	if (0 < ::GetPrivateProfileString(_T("MASTER"), _T("OppLayerName"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.strTwoMetalOppLayer = CString(szData);
	else
		MasterInfo.strTwoMetalOppLayer = _T("");

#else
	MasterInfo.dPixelSize = 33.0;	// [um]
#endif

	return TRUE;
}

// CamMaster Marking Index, Align Position Data. ========================
BOOL CCamMaster::LoadCadMk()
{
#ifdef USE_CAM_MASTER
	BOOL b2PointAlign = FALSE;
	BOOL b4PointAlign = FALSE;

	CFileFind findfile;
	CString sPath2, sPath;

	if (m_sPathCamSpecDir.Right(1) != "\\")
		sPath2.Format(_T("%s\\%s\\%s.pch2"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	else
		sPath2.Format(_T("%s%s\\%s.pch2"), m_sPathCamSpecDir, m_sModel, m_sLayer);

	if (findfile.FindFile(sPath2)) // find 4PointAlign file.
	{
		b4PointAlign = TRUE; 
	}

	if (m_sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.pch"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	else
		sPath.Format(_T("%s%s\\%s.pch"), m_sPathCamSpecDir, m_sModel, m_sLayer);

	if (findfile.FindFile(sPath)) // find 2PointAlign file.
	{
		b2PointAlign = TRUE; 
	}

	if(b4PointAlign && b2PointAlign)
	{
		if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
			return LoadCadMk4PntAlign(sPath2);
		else
			return LoadCadMk2PntAlign(sPath);
	}
	else if (b4PointAlign)
	{
		pDoc->WorkingInfo.LastJob.nAlignMethode = FOUR_POINT;
		return LoadCadMk4PntAlign(sPath2);
	}
	else if (b2PointAlign)
	{
		pDoc->WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
		return LoadCadMk2PntAlign(sPath);
	}
	else
		return FALSE;
#else
	return TRUE;
#endif
}

BOOL CCamMaster::LoadCadMk2PntAlign(CString sPath)
{
	CFile file;
	int nRet;
	int nSzAlignMk, nSzPcsMk;
	int i;

	// Structure Initialize
	m_stAlignMk.X0 = 0.0;
	m_stAlignMk.Y0 = 0.0;
	m_stAlignMk.X1 = 0.0;
	m_stAlignMk.Y1 = 0.0;
	m_lPcsNum = 0;
	for (i = 0; i < MAX_PCS; i++)
	{
		m_stPcsMk[i].X = 0.0;
		m_stPcsMk[i].Y = 0.0;
	}

	// Mark File Load
	if (file.Open((LPCTSTR)sPath, CFile::modeRead))
	{
		nSzAlignMk = sizeof(stAlignMark);

		nRet = file.Read((void *)&m_stAlignMk, nSzAlignMk);
		if (nRet != nSzAlignMk)
			return(FALSE);

		//nRet = file.Read((void *) &m_lPcsNum, sizeof(long));
		//if(nRet != sizeof(long))
		//	return(FALSE);
		nRet = file.Read((void *)&m_lPcsNum, sizeof(int));
		if (nRet != sizeof(int))
			return(FALSE);

		nSzPcsMk = m_lPcsNum * sizeof(stPieceMark);
		nRet = file.Read((void *)&m_stPcsMk, nSzPcsMk);
		if (nRet != nSzPcsMk)
			return(FALSE);

		file.Close();
		return(TRUE);
	}
	else
		return(FALSE);
}

BOOL CCamMaster::LoadCadMk4PntAlign(CString sPath)
{
	CFile file;
	int nRet;
	int nSzAlignMk, nSzPcsMk;
	int i;

	// Structure Initialize
	m_stAlignMk2.X0 = 0.0;
	m_stAlignMk2.Y0 = 0.0;
	m_stAlignMk2.X1 = 0.0;
	m_stAlignMk2.Y1 = 0.0;
	m_stAlignMk2.X2 = 0.0;
	m_stAlignMk2.Y2 = 0.0;
	m_stAlignMk2.X3 = 0.0;
	m_stAlignMk2.Y3 = 0.0;

	m_lPcsNum = 0;

	for (i = 0; i < MAX_PCS; i++)
	{
		m_stPcsMk[i].X = 0.0;
		m_stPcsMk[i].Y = 0.0;
	}

	// Mark File Load
	if (file.Open((LPCTSTR)sPath, CFile::modeRead))
	{
		nSzAlignMk = sizeof(stAlignMark2);

		nRet = file.Read((void *)&m_stAlignMk2, nSzAlignMk);
		if (nRet != nSzAlignMk)
			return(FALSE);

		//nRet = file.Read((void *) &m_lPcsNum, sizeof(long));
		//if(nRet != sizeof(long))
		//	return(FALSE);
		nRet = file.Read((void *)&m_lPcsNum, sizeof(int));
		if (nRet != sizeof(int))
			return(FALSE);

		nSzPcsMk = m_lPcsNum * sizeof(stPieceMark);
		nRet = file.Read((void *)&m_stPcsMk, nSzPcsMk);
		if (nRet != nSzPcsMk)
			return(FALSE);

		file.Close();
		return(TRUE);
	}
	else
		return(FALSE);
}

void CCamMaster::LoadPinImg()
{
	BOOL prcStopF = FALSE;
	TCHAR FileNLoc[200];
	CString StrDirectoryPath;
	CFileFind PinFindFile;
	CString strFileNPin;

	DWORD dwMilliseconds = 10;

	pView->DispMsg(_T("Pin 이미지를 다운로드 중입니다."), _T("Pin 이미지"), RGB_GREEN, DELAY_TIME_MSG);

	//sprintf(FileNLoc, "C:\\R2RSet\\Pin");
	wsprintf(FileNLoc, TEXT("%s"), _T("C:\\R2RSet\\Pin"));

	//if(!PinFindFile.FindFile(FileNLoc))
	if (!pDoc->DirectoryExists(FileNLoc))
	{
		if(!CreateDirectory(FileNLoc, NULL))
		{
			pView->MsgBox(_T("Can not Create Pin Directory"));
// 			AfxMessageBox(_T("Can not Create CAD Directory"),MB_ICONSTOP|MB_OK);
		}
	}
	else
	{
		StrDirectoryPath.Format(_T("%s"), FileNLoc);
		DeleteFileInFolder(FileNLoc);
	}
	PinImgFree();

#ifdef USE_CAM_MASTER
	int Cell;
	int i;
	TCHAR FileNPin[200];

	// CAM-Master File Copy and Local File Load
	//strcpy(FileNLoc, PATH_PIN_IMG);
	wsprintf(FileNLoc, TEXT("%s"), PATH_PIN_IMG);
	if (m_sPathCamSpecDir.Right(1) != "\\")
		strFileNPin.Format(_T("%s\\%s\\%s.TIF"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	else
		strFileNPin.Format(_T("%s%s\\%s.TIF"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	//sprintf(FileNPin, "%s", strFileNPin);
	wsprintf(FileNPin, TEXT("%s"), strFileNPin);

	if(!CopyFile((LPCTSTR)FileNPin, (LPCTSTR)FileNLoc, FALSE))
	{
		if(!CopyFile((LPCTSTR)FileNPin, (LPCTSTR)FileNLoc, FALSE))
		{
			if(!CopyFile((LPCTSTR)FileNPin, (LPCTSTR)FileNLoc, FALSE))
			{
				prcStopF = TRUE;
			}
		}
	}


	if (!prcStopF)
	{
		PinImgBufAlloc(FileNLoc, FALSE);
// 		pView->DispStsBar("Pin 이미지 다운로드를 완료하였습니다.", 0);
	}
	else
		pView->DispMsg(_T("Pin 이미지 다운로드를 실패하였습니다."), _T("경고"), RGB_GREEN, DELAY_TIME_MSG);
#else
	PinImgBufAlloc(PATH_PIN_IMG_, FALSE);
#endif

//	pView->ClrDispMsg();
}
// 
// CString CCamMaster::GetCamPxlRes()
// {
// 	CString sRes=_T("");
// 	CString sPath;
// #ifdef TEST_MODE
// 	sPath = PATH_PIN_IMG_;
// #else
// 	sPath.Format(_T("%s\\%s\\%s.mst"), m_sPathCamSpecDir, 
// 								   m_sModel, 
// 								   m_sLayer);
// #endif
// 
// 	int nPos = sPath.ReverseFind('-');
// 	if(nPos > 0)
// 	{
// 		sRes = sPath.Right(sPath.GetLength()-(nPos+1));
// 		nPos = sRes.ReverseFind('.');
// 		if(nPos > 0)
// 			sRes = sRes.Left(nPos);
// 		WorkingInfo.Vision[0].sCamPxlRes = sRes;
// 		WorkingInfo.Vision[1].sCamPxlRes = sRes;
// 	}
// 	return sRes;
// }

BOOL CCamMaster::LoadStripRgnFromCam() // sprintf(FileNCam,"%s%s\\%s.mst",strSpecPath,strModelPath,strLayerPath);
{
	CFile file;
	int Rsize, RsizeStPosX, RsizeStPosY, RsizeEdPosX, RsizeEdPosY, RsizeXSwathPixPos;
	int SizeI, SizeIXSwathPixPos;
	//char FileNCam[200];
	TCHAR FileNCam[200];

	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_CELL_MST;
#else
	if (m_sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.mst"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	else
		sPath.Format(_T("%s%s\\%s.mst"), m_sPathCamSpecDir, m_sModel, m_sLayer);
#endif

	//strcpy(FileNCam, sPath);
	wsprintf(FileNCam, TEXT("%s"), sPath);

	if(!file.Open(FileNCam, CFile::modeRead))
	{
		if(!file.Open(FileNCam, CFile::modeRead))
		{
			return(FALSE);
		}
	}
	
	if(m_pCellRgn)
		delete m_pCellRgn;
	m_pCellRgn = new REGION_STRIP;

	SizeI = sizeof(int) * RGN_STRIP_VARIABLE_NUM;
	Rsize = file.Read((void *) m_pCellRgn, SizeI);
	
	if(Rsize != SizeI)
	{
		pView->MsgBox(_T("MST File is incorrected."));
// 		AfxMessageBox(_T("MST File is incorrected."),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}
	
	SizeI = sizeof(int) * m_pCellRgn->NodeNumX * m_pCellRgn->NodeNumY;
	RsizeStPosX = file.Read((void *) &m_pCellRgn->StPosX, SizeI);
	RsizeStPosY = file.Read((void *) &m_pCellRgn->StPosY, SizeI);
	RsizeEdPosX = file.Read((void *) &m_pCellRgn->EdPosX, SizeI);
	RsizeEdPosY = file.Read((void *) &m_pCellRgn->EdPosY, SizeI);
	
	SizeIXSwathPixPos = sizeof(int) * m_pCellRgn->nMSwath;
	RsizeXSwathPixPos = file.Read((void *) &m_pCellRgn->XSwathPixPos, SizeIXSwathPixPos);

	if(RsizeStPosX != SizeI && RsizeStPosY != SizeI && RsizeEdPosX != SizeI && RsizeEdPosY != SizeI && RsizeXSwathPixPos != SizeIXSwathPixPos)
	{
		pView->MsgBox(_T("MST File is incorrected."));
// 		AfxMessageBox(_T("MST File is incorrected."),MB_ICONSTOP|MB_OK);
		return(FALSE);
	}

	return(TRUE);
}

BOOL CCamMaster::LoadPcsRgnFromCam()
{
	//char FileN[200];
	TCHAR FileN[200];
	int Size, RSize, SizeI = sizeof(short);
	CFile file;
	int i, nGroupID =0;
	int nPieceRgnNum = 0;

	int nPrevDecodeRatio=0, nDecodeRatio=0;
	CRect rtFrm(10000, 10000, 0, 0);
	double fData1,fData2,fData3,fData4;

	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_CELL_RGN;
#else
	if (MasterInfo.strMasterLocation.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s\\Piece.rgn"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);
	else
		sPath.Format(_T("%s%s\\%s\\Piece.rgn"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);
#endif

	// Select Block ID
	CString strMsg;
	short OldPieceRgnNum;

	//sprintf(FileN, "%s", sPath);
	_stprintf(FileN, _T("%s"), sPath);//sprintf

	if (!file.Open(FileN, CFile::modeRead))
	{
		if (!file.Open(FileN, CFile::modeRead))
		{
			nPieceRgnNum = 0;
			pView->MsgBox(_T("캠마스터에 피스정보가 설정되지 않았습니다."));
// 			AfxMessageBox(_T("캠마스터에 피스정보가 설정되지 않았습니다."));
			return FALSE;
		}
	}

	RSize = file.Read((void *) &OldPieceRgnNum, SizeI);
	nPieceRgnNum = OldPieceRgnNum;

	if (RSize != SizeI)
	{
		nPieceRgnNum = 0;
		file.Close();
		return(FALSE);
	}
	
	REGIONS_PIECE* PieceRegionsPix = NULL;
	if (nPieceRgnNum > 0)
	{
		PieceRegionsPix = new REGIONS_PIECE[nPieceRgnNum];
		Size = sizeof(REGIONS_PIECE) * nPieceRgnNum;
		RSize = file.Read((void *) PieceRegionsPix, Size);
	}
	else
		Size = RSize = 0;

	file.Close();
	
	if (RSize != Size)
	{
		nPieceRgnNum = 0;

		if(PieceRegionsPix)
			delete[] PieceRegionsPix;

		return(FALSE);
	}

	double mmPxl = MasterInfo.dPixelSize / 1000.0; // [mm]


	if (nPieceRgnNum > 0)
	{
		if(m_pPcsRgn)
			delete m_pPcsRgn;
		m_pPcsRgn = new CPcsRgn(nPieceRgnNum);

		int nCol=0, nRow=0;
		long lTop;
		for(i = 0; i < nPieceRgnNum; i++)
		{
			fData1 = (double)PieceRegionsPix[i].iStartX * mmPxl;
			fData2 = (double)PieceRegionsPix[i].iStartY * mmPxl;
			fData3 = (double)PieceRegionsPix[i].iEndX * mmPxl;
			fData4 = (double)PieceRegionsPix[i].iEndY * mmPxl;			
			m_pPcsRgn->pPcs[i] = CRect(fData1, fData2, fData3, fData4);// (left, top, right, bottom)
			m_pPcsRgn->pCenter[i] = CfPoint((fData1+fData3)/2.0, (fData2+fData4)/2.0);

			if(i>0)
			{
// 				if(m_pPcsRgn->pPcs[i].top == m_pPcsRgn->pPcs[i-1].top)
// 					nCol++;
				if(lTop == m_pPcsRgn->pPcs[i].top)
					nCol++;
			}
			else
				lTop = m_pPcsRgn->pPcs[i].top;

			if(m_pPcsRgn->pPcs[i].left < rtFrm.left)
				rtFrm.left = m_pPcsRgn->pPcs[i].left;
			if(m_pPcsRgn->pPcs[i].top < rtFrm.top)
				rtFrm.top = m_pPcsRgn->pPcs[i].top;
			if(m_pPcsRgn->pPcs[i].right > rtFrm.right)
				rtFrm.right = m_pPcsRgn->pPcs[i].right;
			if(m_pPcsRgn->pPcs[i].bottom > rtFrm.bottom)
				rtFrm.bottom = m_pPcsRgn->pPcs[i].bottom;
			
			nDecodeRatio = (i+1)*100/nPieceRgnNum;
// 			if(nDecodeRatio != nPrevDecodeRatio)
// 			{
// 				nPrevDecodeRatio = nDecodeRatio;
// 				pFrm->IncProgress(nDecodeRatio);
// 			}
		}
		nCol++;

		m_pPcsRgn->nCol = nCol;
		m_pPcsRgn->nRow = nPieceRgnNum/nCol;

		double dWidth = rtFrm.right - rtFrm.left;
		double dHeight =  rtFrm.bottom - rtFrm.top;
		double dMarginW = dWidth * 0.05;
		double dMarginH = dHeight * 0.05;
		double dMargin = (dMarginW > dMarginH) ? dMarginH : dMarginW;

		m_pPcsRgn->rtFrm.left = rtFrm.left - dMargin;
		m_pPcsRgn->rtFrm.top = rtFrm.top - dMargin;
		m_pPcsRgn->rtFrm.right = rtFrm.right + dMargin;
		m_pPcsRgn->rtFrm.bottom = rtFrm.bottom + dMargin;
	}

	if(PieceRegionsPix)
		delete[] PieceRegionsPix;

	return(TRUE);
}

////////////////////////////////////////////////////////////////////////////
//
//	Object:	1. Load region data "strpcs.rgn"
//			2. Set Piece Data
//			3. Set Strip Data
BOOL CCamMaster::LoadStripPieceRegion_Binary()	//20121120-ndy for PairPanel
{
	//TCHAR		FileNCam[1024];
	CString		strFileNCam;
	CFileFind	find;
	CString ModelName = m_sModel;
	CString Layer = m_sLayer;

	// 1. Find File "strpcs.rgn"
	if (MasterInfo.strMasterLocation.Right(1) != "\\")
		strFileNCam.Format(_T("%s\\%s\\%s\\strpcs.bin"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);
	else
		strFileNCam.Format(_T("%s%s\\%s\\strpcs.bin"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);

	if (!find.FindFile(strFileNCam))
	{
		pView->MsgBox(_T("캠마스터에 피스정보가 설정되지 않았습니다."));
		return(FALSE);
	}

	// 2. Read Strip & Piece Info
	int		i, j, k, nDummy;
	int		nPieceNum, nPieceCount;
	CFile	file;
	TCHAR	FileNLoc[1024];
	_stprintf(FileNLoc, _T("%s"), strFileNCam);

	if (!file.Open(FileNLoc, CFile::modeRead))
	{
		if (!file.Open(FileNLoc, CFile::modeRead))
		{
			pView->MsgBox(_T("캠마스터에 피스정보파일을 Open하지 못했습니다."));
			return(FALSE);
		}
	}

	int nFrameSize = 0;

	file.Read((void *)&nFrameSize, sizeof(int));
	FrameRgnNum = nFrameSize; 

	file.Read((void *)&PieceRgnNum, sizeof(int));
	file.Read((void *)&m_nCornerNum, sizeof(int));
	file.Read((void *)&m_nDummy[0], sizeof(int));	// reserved
	file.Read((void *)&m_nDummy[1], sizeof(int));	// reserved
	file.Read((void *)&m_nDummy[2], sizeof(int));	// reserved
	file.Read((void *)&m_nDummy[3], sizeof(int));	// reserved
	file.Read((void *)&m_nDummy[4], sizeof(int));	// reserved

												// Init. Buffers
	for (i = 0; i < FrameRgnNum; i++)
	{
		FrameRgnPix[i].iStartX = 0;
		FrameRgnPix[i].iStartY = 0;
		FrameRgnPix[i].iEndX = 0;
		FrameRgnPix[i].iEndY = 0;
		FrameRgnPix[i].FRotate = 0;
		FrameRgnPix[i].FMirror = 0;
		FrameRgnID[i].Col = -1;
		FrameRgnID[i].Row = -1;
	}

	for (i = 0; i < MAX_PIECE_RGN_NUM; i++)
	{
		PieceRgnPix[i].iStartX = 0;
		PieceRgnPix[i].iStartY = 0;
		PieceRgnPix[i].iEndX = 0;
		PieceRgnPix[i].iEndY = 0;
		PieceRgnPix[i].FRotate = 0;
		PieceRgnPix[i].FMirror = 0;

		//PieceRgnID[i].nHeight = 0; //140204lgh
		//PieceRgnID[i].nWidth = 0; //140204 lgh

		PieceRgnPix[i].Col = -1;
		PieceRgnPix[i].Row = -1;
	}

	if (PieceRgnNum > MAX_PIECE_RGN_NUM) // 110803 jsy
	{
		file.Close();
		pView->MsgBox(_T("캠마스터에서 설정한 피스 갯수가 최대치를 초과했습니다."));
		return FALSE;
	}

	//start for previous pcs info.
	CRect rtFrm(10000, 10000, 0, 0);
	double fData1, fData2, fData3, fData4;
	long lTop = 0;
	int nCol = 0, nRow = 0;
	int Size = 0;
	int nPieceRgnNum = 0;

	double mmPxl = MasterInfo.dPixelSize / 1000.0; // [mm]
	//end for previous pcs info.

	AllocPolygonRgnData();

	nPieceCount = 0;
	for (j = 0; j < FrameRgnNum; j++)
	{
		// 4. Set Strip Info.
		file.Read((void *)&FrameRgnID[j].nId, sizeof(int));								// Strip ID

		//file.Read((void *)&nDummy, sizeof(int));			// reserved
		//file.Read((void *)&nDummy, sizeof(int));			// reserved
		file.Read((void *)&FrameRgnID[j].Row, sizeof(int));						// Row
		file.Read((void *)&FrameRgnID[j].Col, sizeof(int));						// Col

		file.Read((void *)&m_nDummy[5], sizeof(int));							// Rotation Info (0 : 0  1 : 90  2 : 180  3 : 270 [Degree])
		file.Read((void *)&(FrameRgnPix[j]), sizeof(REGIONS_FRAME));			// (int * 6)

		// 5. Set Piece Info.
		file.Read((void *)&m_nPieceNum[j], sizeof(int));

		for (i = 0; i < m_nPieceNum[j]; i++)
		{
			file.Read((void *)&PieceRgnPix[i + nPieceCount].nId, sizeof(int));	// Piece ID
			file.Read((void *)&PieceRgnPix[i + nPieceCount].Row, sizeof(int));	// Row
			file.Read((void *)&PieceRgnPix[i + nPieceCount].Col, sizeof(int));	// Col
			file.Read((void *)&m_nDummy[6 + i + nPieceCount], sizeof(int));							// Rotation Info (0 : 0  1 : 90  2 : 180  3 : 270 [Degree])

			////////////////////////////////////////////////////////////////////////////////////////////////
			// Set Piece position
			nDummy = sizeof(CPoint);
			file.Read((void *)PolygonPoints[i + nPieceCount], sizeof(CPoint)*m_nCornerNum);
			file.Read((void *)&(PieceRgnPix[i + nPieceCount].FRotate), sizeof(int));

			// Set PieceRgnPix, 120828 jsy
			PieceRgnPix[i + nPieceCount].iStartX = PolygonPoints[i + nPieceCount][0].x;
			PieceRgnPix[i + nPieceCount].iStartY = PolygonPoints[i + nPieceCount][0].y;
			PieceRgnPix[i + nPieceCount].iEndX = PolygonPoints[i + nPieceCount][0].x;
			PieceRgnPix[i + nPieceCount].iEndY = PolygonPoints[i + nPieceCount][0].y;
			for (k = 0; k < m_nCornerNum; k++)
			{
				PieceRgnPix[i + nPieceCount].iStartX = min(PieceRgnPix[i + nPieceCount].iStartX, PolygonPoints[i + nPieceCount][k].x);
				PieceRgnPix[i + nPieceCount].iStartY = min(PieceRgnPix[i + nPieceCount].iStartY, PolygonPoints[i + nPieceCount][k].y);
				PieceRgnPix[i + nPieceCount].iEndX = max(PieceRgnPix[i + nPieceCount].iEndX, PolygonPoints[i + nPieceCount][k].x);
				PieceRgnPix[i + nPieceCount].iEndY = max(PieceRgnPix[i + nPieceCount].iEndY, PolygonPoints[i + nPieceCount][k].y);
			}
		}
		nPieceCount += m_nPieceNum[j];

	}// for(j = 0; j < FrameRgnNum; j++)


	//start for previous pcs info.
	nPieceRgnNum = PieceRgnNum;

	if (nPieceRgnNum > 0)
	{
		if (m_pPcsRgn)
			delete m_pPcsRgn;
		m_pPcsRgn = new CPcsRgn(nPieceRgnNum);
	}
	else
	{
		file.Close();
		Size = 0;
		pView->MsgBox(_T("캠마스터에서 설정한 피스 갯수가 없습니다."));
		return(FALSE);
	}

	for (i = 0; i < nPieceRgnNum; i++)
	{

		fData1 = (double)PieceRgnPix[i].iStartX * mmPxl;
		fData2 = (double)PieceRgnPix[i].iStartY * mmPxl;
		fData3 = (double)PieceRgnPix[i].iEndX * mmPxl;
		fData4 = (double)PieceRgnPix[i].iEndY * mmPxl;
		m_pPcsRgn->pPcs[i] = CRect(fData1, fData2, fData3, fData4);// (left, top, right, bottom)
		m_pPcsRgn->pCenter[i] = CfPoint((fData1 + fData3) / 2.0, (fData2 + fData4) / 2.0);

		if (i > 0)
		{
			if (lTop == m_pPcsRgn->pPcs[i].top)
				nCol++;

			//if(m_pPcsRgn->pPcs[i].top == m_pPcsRgn->pPcs[i-1].top)
			//	nCol++;
		}
		else
			lTop = m_pPcsRgn->pPcs[i].top;

		if (m_pPcsRgn->pPcs[i].left < rtFrm.left)
			rtFrm.left = m_pPcsRgn->pPcs[i].left;
		if (m_pPcsRgn->pPcs[i].top < rtFrm.top)
			rtFrm.top = m_pPcsRgn->pPcs[i].top;
		if (m_pPcsRgn->pPcs[i].right > rtFrm.right)
			rtFrm.right = m_pPcsRgn->pPcs[i].right;
		if (m_pPcsRgn->pPcs[i].bottom > rtFrm.bottom)
			rtFrm.bottom = m_pPcsRgn->pPcs[i].bottom;
	}

	nCol++;

	m_pPcsRgn->nCol = nCol;
	m_pPcsRgn->nRow = nPieceRgnNum / nCol;

	double dWidth = rtFrm.right - rtFrm.left;
	double dHeight = rtFrm.bottom - rtFrm.top;
	double dMarginW = dWidth * 0.05;
	double dMarginH = dHeight * 0.05;
	double dMargin = (dMarginW > dMarginH) ? dMarginH : dMarginW;

	m_pPcsRgn->rtFrm.left = rtFrm.left - dMargin;
	m_pPcsRgn->rtFrm.top = rtFrm.top - dMargin;
	m_pPcsRgn->rtFrm.right = rtFrm.right + dMargin;
	m_pPcsRgn->rtFrm.bottom = rtFrm.bottom + dMargin;
	//end for previous pcs info.

	file.Close();


	return TRUE;
}

BOOL CCamMaster::WriteStripPieceRegion_Text(CString sBasePath, CString sLot)
{
	if (!pDoc->WorkingInfo.System.bStripPcsRgnBin)
		return FALSE;

	CString sModel = m_sModel;
	CString sLayer = m_sLayer;
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	// Make Directory...
	sPath.Format(_T("%s"), sBasePath);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (sModel.IsEmpty() || sLot.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), sBasePath, sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), sBasePath, sModel, sLot);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), sBasePath, sModel, sLot, sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	// Write WriteStripPieceRegion_Text...
	sPath.Format(_T("%s%s\\%s\\%s\\strpcs.txt"), sBasePath, sModel, sLot, sLayer);

	int		i, j, k;
	CString strTitle, strSubTitle, sData;

	sData.Format(_T("%d"), FrameRgnNum);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("NumOfStripFrame"), sData, sPath);
	sData.Format(_T("%d"), PieceRgnNum);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("NumOfTotalPiece"), sData, sPath);
	sData.Format(_T("%d"), m_nCornerNum);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("NumOfPieceCorner"), sData, sPath);
	sData.Format(_T("%d"), m_nDummy[0]);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("nDummy[0]"), sData, sPath);
	sData.Format(_T("%d"), m_nDummy[1]);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("nDummy[1]"), sData, sPath);
	sData.Format(_T("%d"), m_nDummy[2]);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("nDummy[2]"), sData, sPath);
	sData.Format(_T("%d"), m_nDummy[3]);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("nDummy[3]"), sData, sPath);
	sData.Format(_T("%d"), m_nDummy[4]);
	WritePrivateProfileString(_T("STRIPFRAME INFO"), _T("nDummy[4]"), sData, sPath);

	int nPieceCount = 0;
	for (j = 0; j < FrameRgnNum; j++)
	{
		// 4. Set Strip Info.
		strTitle.Format(_T("STRIPFRAME%d"), j);

		sData.Format(_T("%d"), FrameRgnID[j].nId);
		WritePrivateProfileString(strTitle, _T("StripID"), sData, sPath);
		sData.Format(_T("%d"), FrameRgnID[j].Row);
		WritePrivateProfileString(strTitle, _T("StripRow"), sData, sPath);
		sData.Format(_T("%d"), FrameRgnID[j].Col);
		WritePrivateProfileString(strTitle, _T("StripCol"), sData, sPath);
		sData.Format(_T("%d"), m_nDummy[5]);
		WritePrivateProfileString(strTitle, _T("StripRotation"), sData, sPath); // Rotation Info (0 : 0  1 : 90  2 : 180  3 : 270 [Degree])
		sData.Format(_T("%d, %d, %d, %d"), FrameRgnPix[j].iStartX, FrameRgnPix[j].iStartY, FrameRgnPix[j].iEndX, FrameRgnPix[j].iEndY);
		WritePrivateProfileString(strTitle, _T("StripRect"), sData, sPath);
		sData.Format(_T("%d"), FrameRgnPix[j].FMirror);
		WritePrivateProfileString(strTitle, _T("StripMirror"), sData, sPath);	//0 : 원본 1 : 상하미러  2 : 좌우미러.
		sData.Format(_T("%d"), FrameRgnPix[j].FRotate);
		WritePrivateProfileString(strTitle, _T("StripRotate"), sData, sPath);	//0 : 0도  1 : 90도  2 : 180도  3 : 270도

		// 5. Set Piece Info.
		sData.Format(_T("%d"), m_nPieceNum[j]);
		WritePrivateProfileString(strTitle, _T("NumOfPiece"), sData, sPath);
		for (i = 0; i < m_nPieceNum[j]; i++)
		{
			strSubTitle.Format(_T("Piece%dID"), i);
			sData.Format(_T("%d"), PieceRgnPix[i + nPieceCount].nId);
			WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);
			strSubTitle.Format(_T("Piece%dRow"), i);
			sData.Format(_T("%d"), PieceRgnPix[i + nPieceCount].Row);
			WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);
			strSubTitle.Format(_T("Piece%dCol"), i);
			sData.Format(_T("%d"), PieceRgnPix[i + nPieceCount].Col);
			WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);
			strSubTitle.Format(_T("Piece%dDummy"), i);
			sData.Format(_T("%d"), m_nDummy[6 + i + nPieceCount]);
			WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);	//0 : 0도  1 : 90도  2 : 180도  3 : 270도

			// Set Piece position
			for (k = 0; k < m_nCornerNum; k++)
			{
				strSubTitle.Format(_T("Piece%dPoint%d"), i, k);
				sData.Format(_T("%d, %d"), PolygonPoints[i + nPieceCount][k].x, PolygonPoints[i + nPieceCount][k].y);
				WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);
			}
			strSubTitle.Format(_T("Piece%dRotate"), i);
			sData.Format(_T("%d"), PieceRgnPix[i + nPieceCount].FRotate);
			WritePrivateProfileString(strTitle, strSubTitle, sData, sPath);

		}
		nPieceCount += m_nPieceNum[j];

	}// for(j = 0; j < FrameRgnNum; j++)

	return TRUE;
}

void CCamMaster::AllocPolygonRgnData() //120809 jsy
{
	int i;

	FreePolygonRgnData();

	//150910 lgh 
	PolygonPoints = (CPoint **)malloc(sizeof(CPoint*) * MAX_PIECE_RGN_NUM);
	int nRefinePieceNum = max(1, PieceRgnNum);
	for (i = 0; i < nRefinePieceNum; i++)
		PolygonPoints[i] = (CPoint *)malloc(sizeof(CPoint) * m_nCornerNum);

	for (i = nRefinePieceNum; i < MAX_PIECE_RGN_NUM; i++)
	{
		PolygonPoints[i] = NULL;
	}
}

void CCamMaster::FreePolygonRgnData()	// 120809 jsy
{
	int i;
	if (PolygonPoints != NULL)
	{
		for (i = 0; i < MAX_PIECE_RGN_NUM; i++)
		{
			if (PolygonPoints[i] != NULL)
			{
				free(PolygonPoints[i]);
				PolygonPoints[i] = NULL;
			}
		}

		free(PolygonPoints);
		PolygonPoints = NULL;
	}
}



void CCamMaster::LoadPcsImg()
{
	BOOL prcStopF = FALSE;
	//char FileS[200], FileD[200];
	TCHAR FileD[200]; 
	CFileFind PcsFindFile;
	CString StrDirectoryPath;

	//sprintf(FileD, "C:\\R2RSet\\Pcs");
	_stprintf(FileD, _T("C:\\R2RSet\\Pcs"));
	//if(!PcsFindFile.FindFile(FileD))
	if (!pDoc->DirectoryExists(FileD))
	{
		if(!CreateDirectory(FileD, NULL))
		{
			pView->MsgBox(_T("Can not Create PCS Directory"));
		}
	}
	else
	{
		StrDirectoryPath.Format(_T("%s"), FileD);
		DeleteFileInFolder(FileD);
	}
	PcsImgFree();
		
#ifdef USE_CAM_MASTER
	TCHAR FileS[200];
	//sprintf(FileS,"%s%s\\%s\\Piece.tif", MasterInfo.strMasterLocation, m_sModel, m_sLayer);
	//strcpy(FileD, PATH_PCS_IMG);
	if (MasterInfo.strMasterLocation.Right(1) != "\\")
		_stprintf(FileS, _T("%s\\%s\\%s\\Piece.tif"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);
	else
		_stprintf(FileS, _T("%s%s\\%s\\Piece.tif"), MasterInfo.strMasterLocation, m_sModel, m_sLayer);
	_stprintf(FileD, PATH_PCS_IMG);

	if(!CopyFile((LPCTSTR)FileS, (LPCTSTR)FileD, FALSE))
	{
		if(!CopyFile((LPCTSTR)FileS, (LPCTSTR)FileD, FALSE))
		{
			if(!CopyFile((LPCTSTR)FileS, (LPCTSTR)FileD, FALSE))
				prcStopF = TRUE;
		}
	}

	if (!prcStopF)
	{
		PcsImgBufAlloc(FileD, FALSE);
// 		pView->DispStsBar("PCS 이미지 다운로드를 완료하였습니다.", 0);
	}
	else
		pView->DispMsg(_T("PCS 이미지 다운로드를 실패하였습니다."), _T("경고"), RGB_GREEN, DELAY_TIME_MSG);
#else
	PcsImgBufAlloc(PATH_PCS_IMG_, FALSE);
#endif

//	pView->ClrDispMsg();
}

void CCamMaster::LoadCadImg()
{
	BOOL prcStopF = FALSE;
	//char FileNCam[200], FileNLoc[200];
	TCHAR FileNLoc[200]; 
	CString StrDirectoryPath;
	CFileFind CADFindFile;
	CString strFileNCam;
	int nStripCell;
	int i;
	int CurrSwath;
	int SwathCellNum, SwathNumX;

	DWORD dwMilliseconds = 10;

	pView->DispMsg(_T("CAD 이미지를 다운로드 중입니다."), _T("CAD 이미지"), RGB_GREEN, DELAY_TIME_MSG);

	//sprintf(FileNLoc, "C:\\R2RSet\\CAD");
	_stprintf(FileNLoc, _T("%s"), _T("C:\\R2RSet\\CAD"));

	//if(!CADFindFile.FindFile(FileNLoc))
	if (!pDoc->DirectoryExists(FileNLoc))
	{
		if(!CreateDirectory(FileNLoc, NULL))
		{
			pView->MsgBox(_T("Can not Create CAD Directory"));
// 			AfxMessageBox(_T("Can not Create CAD Directory"),MB_ICONSTOP|MB_OK);
		}
	}
	else
	{
		StrDirectoryPath.Format(_T("%s"), FileNLoc);
		DeleteFileInFolder(FileNLoc);
	}
	CADLinkImgFree();

	// 한 스트립의 총 셀수 = 한 스트립의 x축상의 셀수 * 한 스트립의 y축상의 셀수.
	nStripCell = m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY;

	SwathCellNum = m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY;

	for(i = 0; i < (m_pCellRgn->NodeNumX * m_pCellRgn->NodeNumY); i++) // 한 Shot의 전체 셀 수.
	{
		SwathNumX = (i / m_pCellRgn->NodeNumY) / m_pCellRgn->nCellDivideX;
		CellInspID[i] = (i / m_pCellRgn->NodeNumY) * m_pCellRgn->nCellDivideY + (i % m_pCellRgn->nCellDivideY) - (SwathCellNum * SwathNumX);
	}
	
	for(i = 0; i < (m_pCellRgn->nCellDivideX * m_pCellRgn->nCellDivideY); i++) // 한 Strip의 전체 셀 수.
	{
		CurrSwath = i / m_pCellRgn->nCellDivideY;
		CellLoadID[i] = (i % m_pCellRgn->nCellDivideY) + (CurrSwath * m_pCellRgn->NodeNumY);
	}

#ifdef USE_CAM_MASTER
	int Cell;
	TCHAR FileNCam[200];

	for (Cell = 0; Cell < nStripCell; Cell++) // 한 스트립의 총 셀수.
	{
		// CAM-Master File Copy and Local File Load
		strFileNCam.Format(_T("%s\\CORG%c%d.TIF"), MasterInfo.strCADImgPath, 'O', CellLoadID[Cell]);
		//sprintf(FileNCam, "%s", strFileNCam);
		//sprintf(FileNLoc, "C:\\R2RSet\\CAD\\CORG%c%d.TIF", 'O', CellLoadID[Cell]);
		_stprintf(FileNCam, _T("%s"), strFileNCam);
		_stprintf(FileNLoc, _T("C:\\R2RSet\\CAD\\CORG%c%d.TIF"), 'O', CellLoadID[Cell]);

		if(!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
		{
			if(!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
			{
				if(!CopyFile((LPCTSTR)FileNCam, (LPCTSTR)FileNLoc, FALSE))
				{
					prcStopF = TRUE;
					break;
				}
			}
		}

		CADImgBufAlloc(FileNLoc, Cell, FALSE);
	}

	if (!prcStopF)
		pView->DispMsg(_T("CAD 이미지 다운로드를 완료하였습니다."), _T("CAD 이미지"), RGB_GREEN, DELAY_TIME_MSG);
	else
		pView->DispMsg(_T("CAD 이미지 다운로드를 실패하였습니다."), _T("CAD 이미지"), RGB_GREEN, DELAY_TIME_MSG);
#else
	CADImgBufAlloc(PATH_CAD_IMG, 0, FALSE);
#endif

	pView->ClrDispMsg();
}

BOOL CCamMaster::PinImgBufAlloc(TCHAR *strPinImg, BOOL bOppLayerF)
{
	CFile file;
	int RSize;

	if (!file.Open(strPinImg, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strPinImg, CFile::modeRead | CFile::typeBinary))
		{
			return(FALSE);
		}
	}
	
	m_PinFileSize = file.GetLength();
	if(m_PinFileSize != 0)
	{
		if(m_pPinImg)
			PinImgFree();

		m_pPinImg = (UCHAR *) GlobalAllocPtr(GMEM_MOVEABLE, m_PinFileSize);
	}

	RSize = file.Read((void *) m_pPinImg, m_PinFileSize);
	if (RSize != m_PinFileSize)
	{
		file.Close();
		return(FALSE);
	}	

	file.Close();
	return TRUE;
}

void CCamMaster::DeleteFileInFolder(CString sPathDir)
{
	BOOL bRes;
	CFileFind ff;

	if (CheckPath(sPathDir) == PATH_IS_FILE)
	{
		DeleteFile(sPathDir);
		return;
	}

	if (sPathDir.Right(1) != _T("\\"))
		sPathDir.Append(_T("\\"));	//sPathDir += _T("\\");
	
	sPathDir.Append(_T("*.*"));	//sPathDir += _T("*.*");

	bRes = ff.FindFile(sPathDir);

	while(bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			sPathDir = ff.GetFilePath();
			DeleteFileInFolder(sPathDir);
		}
		else
			DeleteFileInFolder(ff.GetFilePath());
	}

	ff.Close();
}

int CCamMaster::CheckPath(CString strPath)
{
	DWORD dwAttr = GetFileAttributes(strPath);
	if (dwAttr == 0xffffffff) 
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}

void CCamMaster::PinImgFree()
{
	if(m_pPinImg != NULL)
	{
		GlobalFreePtr(m_pPinImg);
		m_pPinImg = NULL;
	}
}

BOOL CCamMaster::PcsImgBufAlloc(TCHAR *strPcsImg, BOOL bOppLayerF)
{
	CFile file;
	int RSize;

	if (!file.Open(strPcsImg, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strPcsImg, CFile::modeRead | CFile::typeBinary))
		{
			return(FALSE);
		}
	}
	
	m_PcsFileSize = file.GetLength();
	if(m_PcsFileSize != 0)
	{
		if(m_pPcsImg)
			PcsImgFree();

		m_pPcsImg = (UCHAR *) GlobalAllocPtr(GMEM_MOVEABLE, m_PcsFileSize);
	}

	RSize = file.Read((void *) m_pPcsImg, m_PcsFileSize);
	if (RSize != m_PcsFileSize)
	{
		file.Close();
		return(FALSE);
	}	

	file.Close();
	return TRUE;
}

void CCamMaster::CADLinkImgFree()
{
	int i;

	for(i = 0; i < MAX_CELL_NUM; i++)
	{
		if(m_pCADCellImg[i] != NULL)
		{
			GlobalFreePtr(m_pCADCellImg[i]);
			m_pCADCellImg[i] = NULL;
		}
	}
}

BOOL CCamMaster::CADImgBufAlloc(TCHAR *strCADImg, int CellNum, BOOL bOppLayerF)
{
	CFile file;
	int RSize;

	if (!file.Open(strCADImg, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strCADImg, CFile::modeRead | CFile::typeBinary))
		{
			return(FALSE);
		}
	}
	
	m_CADFileSize[CellNum] = file.GetLength();
	if(m_CADFileSize[CellNum] != 0)
		m_pCADCellImg[CellNum] = (UCHAR *) GlobalAllocPtr(GMEM_MOVEABLE, m_CADFileSize[CellNum]);

	RSize = file.Read((void *) m_pCADCellImg[CellNum], m_CADFileSize[CellNum]);
	if (RSize != m_CADFileSize[CellNum])
	{
		file.Close();
		return(FALSE);
	}	

	file.Close();
	return TRUE;
}

void CCamMaster::LoadAlignImg()
{
/*
	char FN[200];

	sprintf(FN, "%s%s\\%s-md0.tif",strSpecPath,strModel,strLayer);
	VicFileLoad(pGlobalView->m_pDlgSetPunchAlign->m_VisionFirst.m_MilAlign0PositionImg, FN);
	
	sprintf(FN, "%s%s\\%s-md1.tif",strSpecPath,strModel,strLayer);
	VicFileLoad(pGlobalView->m_pDlgSetPunchAlign->m_VisionFirst.m_MilAlign1PositionImg, FN);

	sprintf(FN, "c:\\test100.tif");
	MbufSave(FN, pGlobalView->m_pDlgSetPunchAlign->m_VisionFirst.m_MilAlign0PositionImg);
	sprintf(FN, "c:\\test200.tif");
	MbufSave(FN, pGlobalView->m_pDlgSetPunchAlign->m_VisionFirst.m_MilAlign1PositionImg);
*/
	BOOL prcStopF = FALSE;
	//char FileNAlign[200], FileNLoc[200];
	TCHAR FileNLoc[200]; 
	CString StrDirectoryPath;
	CFileFind AlignFindFile;
	CString strFileNAlign;

	DWORD dwMilliseconds = 10;

	pView->DispMsg(_T("Align 이미지를 다운로드 중입니다."), _T("Align 이미지"), RGB_GREEN, DELAY_TIME_MSG);
	//sprintf(FileNLoc, "C:\\R2RSet\\Align");
	_stprintf(FileNLoc, _T("%s"), _T("C:\\R2RSet\\Align"));

	//if(!AlignFindFile.FindFile(FileNLoc))
	if (!pDoc->DirectoryExists(FileNLoc))
	{
		if(!CreateDirectory(FileNLoc, NULL))
		{
			pView->MsgBox(_T("Can not Create Align Directory"));
// 			AfxMessageBox(_T("Can not Create CAD Directory"),MB_ICONSTOP|MB_OK);
		}
	}
	else
	{
		StrDirectoryPath.Format(_T("%s"), FileNLoc);
		DeleteFileInFolder(FileNLoc);
	}
	AlignImgFree();

#ifdef USE_CAM_MASTER
	int i;
	int Cell;
	TCHAR FileNAlign[200];

	// CAM-Master File Copy and Local File Load
	//strcpy(FileNLoc, PATH_ALIGN0_IMG);
	_stprintf(FileNLoc, _T("%s"), PATH_ALIGN0_IMG);
	if(m_sLayerUp.IsEmpty())
	{ 
		if (m_sPathCamSpecDir.Right(1) != "\\")
			strFileNAlign.Format(_T("%s\\%s\\%s-md0.tif"), m_sPathCamSpecDir, m_sModel, m_sLayer);
		else
			strFileNAlign.Format(_T("%s%s\\%s-md0.tif"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	}
	else
	{
		if (m_sPathCamSpecDir.Right(1) != "\\")
			strFileNAlign.Format(_T("%s\\%s\\%s-md0.tif"), m_sPathCamSpecDir, m_sModel, m_sLayerUp);
		else
			strFileNAlign.Format(_T("%s%s\\%s-md0.tif"), m_sPathCamSpecDir, m_sModel, m_sLayerUp);
	}
	//sprintf(FileNAlign, "%s", strFileNAlign);
	_stprintf(FileNAlign, _T("%s"), strFileNAlign);

	if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
	{
		if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
		{
			if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
			{
				prcStopF = TRUE;
			}
		}
	}

	if (!prcStopF)
	{
		AlignImgBufAlloc(FileNLoc, 0);
// 		pView->DispStsBar("Align0 이미지 다운로드를 완료하였습니다.", 0);
	}
	else
		pView->DispMsg(_T("Align0 이미지 다운로드를 실패하였습니다."), _T("경고"), RGB_GREEN, DELAY_TIME_MSG);

	// CAM-Master File Copy and Local File Load
	//strcpy(FileNLoc, PATH_ALIGN1_IMG);
	_stprintf(FileNLoc, _T("%s"), PATH_ALIGN1_IMG);
	if(m_sLayerUp.IsEmpty())
	{
		if (m_sPathCamSpecDir.Right(1) != "\\")
			strFileNAlign.Format(_T("%s\\%s\\%s-md1.tif"), m_sPathCamSpecDir, m_sModel, m_sLayer); 
		else
			strFileNAlign.Format(_T("%s%s\\%s-md1.tif"), m_sPathCamSpecDir, m_sModel, m_sLayer);
	}
	else
	{
		if (m_sPathCamSpecDir.Right(1) != "\\")
			strFileNAlign.Format(_T("%s\\%s\\%s-md1.tif"), m_sPathCamSpecDir, m_sModel, m_sLayerUp);
		else
			strFileNAlign.Format(_T("%s%s\\%s-md1.tif"), m_sPathCamSpecDir, m_sModel, m_sLayerUp);
	}
	//sprintf(FileNAlign, "%s", strFileNAlign);
	_stprintf(FileNAlign, _T("%s"), strFileNAlign);

	if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
	{
		if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
		{
			if(!CopyFile((LPCTSTR)FileNAlign, (LPCTSTR)FileNLoc, FALSE))
			{
				prcStopF = TRUE;
			}
		}
	}

	if (!prcStopF)
	{
		AlignImgBufAlloc(FileNLoc, 1);
// 		pView->DispStsBar("Align1 이미지 다운로드를 완료하였습니다.", 0);
	}
	else
		pView->DispMsg(_T("Align1 이미지 다운로드를 실패하였습니다."), _T("경고"), RGB_GREEN, DELAY_TIME_MSG);
#else
	AlignImgBufAlloc(PATH_ALIGN0_IMG, 1);
#endif
}

void CCamMaster::AlignImgFree(int nPos)
{
	if(nPos<0)
	{
		if(m_pAlignImg[0] != NULL)
		{
			GlobalFreePtr(m_pAlignImg[0]);
			m_pAlignImg[0] = NULL;
		}

		if(m_pAlignImg[1] != NULL)
		{
			GlobalFreePtr(m_pAlignImg[1]);
			m_pAlignImg[1] = NULL;
		}

		if (m_pAlignImg[2] != NULL)
		{
			GlobalFreePtr(m_pAlignImg[2]);
			m_pAlignImg[2] = NULL;
		}

		if (m_pAlignImg[3] != NULL)
		{
			GlobalFreePtr(m_pAlignImg[3]);
			m_pAlignImg[3] = NULL;
		}
	}
	else
	{
		if(m_pAlignImg[nPos] != NULL)
		{
			GlobalFreePtr(m_pAlignImg[nPos]);
			m_pAlignImg[nPos] = NULL;
		}
	}
}

void CCamMaster::PcsImgFree()
{
	if(m_pPcsImg != NULL)
	{
		GlobalFreePtr(m_pPcsImg);
		m_pPcsImg = NULL;
	}
}

BOOL CCamMaster::AlignImgBufAlloc(TCHAR *sAlignImg, int nPos)
{
	CFile file;
	int RSize;

	if (!file.Open(sAlignImg, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(sAlignImg, CFile::modeRead | CFile::typeBinary))
		{
			return(FALSE);
		}
	}
	
	m_AlignFileSize[nPos] = file.GetLength();
	if(m_AlignFileSize[nPos] != 0)
	{
		if(m_pAlignImg[nPos])
			AlignImgFree(nPos);

		m_pAlignImg[nPos] = (UCHAR *) GlobalAllocPtr(GMEM_MOVEABLE, m_AlignFileSize[nPos]);
	}

	RSize = file.Read((void *) m_pAlignImg[nPos], m_AlignFileSize[nPos]);
	if (RSize != m_AlignFileSize[nPos])
	{
		file.Close();
		return(FALSE);
	}	

	file.Close();
	return TRUE;
}


