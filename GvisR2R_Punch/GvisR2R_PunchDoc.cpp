
// GvisR2R_PunchDoc.cpp : CGvisR2R_PunchDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GvisR2R_Punch.h"
#endif

#include "GvisR2R_PunchDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Process/DataFile.h"
//#include "Device/Vicdefs.h"
#include "Device/Vic7defs.h"
#include "Global/GlobalFunc.h"


#include "MainFrm.h"
#include "GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

// CGvisR2R_PunchDoc

IMPLEMENT_DYNCREATE(CGvisR2R_PunchDoc, CDocument)

BEGIN_MESSAGE_MAP(CGvisR2R_PunchDoc, CDocument)
END_MESSAGE_MAP()


// CGvisR2R_PunchDoc 생성/소멸

CGvisR2R_PunchDoc::CGvisR2R_PunchDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	int i, k;
	pDoc = this;
	m_strUserNameList = _T("");

	m_bBufEmpty[0] = FALSE; // Exist
	m_bBufEmpty[1] = FALSE; // Exist
	m_bBufEmptyF[0] = FALSE; // Exist
	m_bBufEmptyF[1] = FALSE; // Exist


	// 	m_pCellRgn = NULL;
	// 	m_pPcsRgn = NULL;
	m_pReelMap = NULL;
	m_pReelMapUp = NULL;
	m_pReelMapDn = NULL;
	m_pReelMapAllUp = NULL;
	m_pReelMapAllDn = NULL;

	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
			m_pPcr[i][k] = NULL;
	}

	pMkInfo = NULL;

	AoiDummyShot[0] = 3;
	AoiDummyShot[1] = 3;

	// 	for(i = 0; i < MAX_CELL_NUM; i++)
	// 	{
	// 		m_pCADCellImg[i] = NULL;
	// 		m_CADFileSize[i] = 0;
	// 	}

	// 	m_pPinImg = NULL;
	// 	m_PinFileSize = 0;
	// 	m_pAlignImg[0] = NULL;
	// 	m_pAlignImg[1] = NULL;
	// 	m_AlignFileSize[0] = 0;
	// 	m_AlignFileSize[1] = 0;
	// 
	// 	m_pPcsImg = NULL;
	// 	m_PcsFileSize = 0;

	m_nSliceIo = 0;

	m_nMpeIb = 0;
	m_pMpeIb = NULL;
	m_pMpeIF = NULL;

	m_nMpeIo = 0;
	m_pMpeIo = NULL;
	m_pMpeIoF = NULL;

	m_nMpeSignal = 0;
	m_pMpeSignal = NULL;

	m_nMpeData = 0;
	m_pMpeData = NULL;

	// 	m_pPrevMpeIo = NULL;

	// 	m_pSliceIo = NULL;
	// 	m_pSliceIoF = NULL;
	// 	m_pPrevSliceIo = NULL;
	// 	
	// 	m_pSliceMpeIo = NULL;
	// 	m_pSliceMpeIoF = NULL;
	// 	m_pPrevSliceMpeIo = NULL;

	m_pSpecLocal = NULL;

	m_pFile = new CMyFile();
	InitPcr();

	m_sPassword = _T("");
	m_sAlmMsg = _T("");
	m_sPrevAlmMsg = _T("");

	m_nPrevSerial = 0;
	m_bNewLotShare[0] = FALSE;
	m_bNewLotShare[1] = FALSE;
	m_bNewLotBuf[0] = FALSE;
	m_bNewLotBuf[1] = FALSE;
	m_bDoneChgLot = FALSE;

	m_cBigDefCode[0] = '*';		//	None
	m_cBigDefCode[1] = 'N';		//	NICK
	m_cBigDefCode[2] = 'D';		//	PROTRUSION
	m_cBigDefCode[3] = 'A';		//	SPACE
	m_cBigDefCode[4] = 'O';		//	OPEN
	m_cBigDefCode[5] = 'S';		//	SHORT
	m_cBigDefCode[6] = 'U';		//	USHORT
	m_cBigDefCode[7] = 'I';		//	PINHOLE
	m_cBigDefCode[8] = 'H';		//	HOLE_MISS
	m_cBigDefCode[9] = 'E';		//	EXTRA
	m_cBigDefCode[10] = 'P';	//	PAD
	m_cBigDefCode[11] = 'L';	//	HOLE_POSITION
	m_cBigDefCode[12] = 'X';	//	POI
	m_cBigDefCode[13] = 'T';	//	VH_POSITION
	m_cBigDefCode[14] = 'M';	//	VH_MISS
	m_cBigDefCode[15] = 'F';	//	HOLE_DEFECT
	m_cBigDefCode[16] = 'C';	//	HOLE_OPEN
	m_cBigDefCode[17] = 'G';	//	VH_OPEN
	m_cBigDefCode[18] = 'V';	//	VH_DEF
	m_cBigDefCode[19] = 'K';	//	E.Nick
	m_cBigDefCode[20] = 'B';	//	E.Prot
	m_cBigDefCode[21] = 'J';	//	E.Space
	m_cBigDefCode[22] = 'Q';	//	UDD1
	m_cBigDefCode[23] = 'R';	//	Narrow
	m_cBigDefCode[24] = 'W';	//	Wide
	m_cBigDefCode[25] = '?';	//	Light

	m_cSmallDefCode[0] = '*';	//	None
	m_cSmallDefCode[1] = 'n';	//	NICK
	m_cSmallDefCode[2] = 'd';	//	PROTRUSION
	m_cSmallDefCode[3] = 'a';	//	SPACE
	m_cSmallDefCode[4] = 'o';
	m_cSmallDefCode[5] = 's';
	m_cSmallDefCode[6] = 'u';
	m_cSmallDefCode[7] = 'i';
	m_cSmallDefCode[8] = 'h';
	m_cSmallDefCode[9] = 'e';
	m_cSmallDefCode[10] = 'p';
	m_cSmallDefCode[11] = 'l';
	m_cSmallDefCode[12] = 'x';
	m_cSmallDefCode[13] = 't';
	m_cSmallDefCode[14] = 'm';
	m_cSmallDefCode[15] = 'f';
	m_cSmallDefCode[16] = 'c';
	m_cSmallDefCode[17] = 'g';
	m_cSmallDefCode[18] = 'v';
	m_cSmallDefCode[19] = 'k';	//	E.Nick
	m_cSmallDefCode[20] = 'b';	//	E.Prot
	m_cSmallDefCode[21] = 'j';	//	E.Space
	m_cSmallDefCode[22] = 'q';	//	UDD1
	m_cSmallDefCode[23] = 'r';	//	Narrow
	m_cSmallDefCode[24] = 'w';	//	Wide
	m_cSmallDefCode[25] = '?';	//	Light

// 	for(i=19; i<MAX_DEF; i++)
// 	{
// 		m_cBigDefCode[i] = '?';	//	VH_DEF
// 		m_cSmallDefCode[i] = '?';
// 	}

	// 20160926-syd
	m_bUseRTRYShiftAdjust = FALSE;
	m_dRTRShiftVal = 0.0;
	m_dShiftAdjustRatio = 0.0;

	m_nOrderNum = 0;
	m_nShotNum = 0;
	m_sOrderNum = _T("");
	m_sShotNum = _T("");
	m_nTestOrderNum = 0;
	m_nTestShotNum = 0;
	m_sTestOrderNum = _T("");
	m_sTestShotNum = _T("");

	// Communcation
	m_nBad[0] = 0; m_nBad[1] = 0; m_nBad[2] = 0;
	m_nGood[0] = 0; m_nGood[1] = 0; m_nGood[2] = 0;
	m_dBadRatio[0] = 0.0; m_dBadRatio[1] = 0.0; m_dBadRatio[2] = 0.0;
	m_dGoodRatio[0] = 0.0; m_dGoodRatio[1] = 0.0; m_dGoodRatio[2] = 0.0;
	m_nTestNum[0] = 0; m_nTestNum[1] = 0; m_nTestNum[2] = 0;
	m_sLotStTime = _T(""); m_sLotEdTime = _T(""); m_sLotRunTime = _T("");

	for (i = 0; i < 3; i++)
	{
		for (k = 0; k < 5; k++)
		{
			m_dStripRatio[i][k] = 0.0;				// [3]: Up/Dn/ALL , [5]: Strip 0~3, ALL
			m_dTotRatio[i][k];						// [3]: Up/Dn/ALL , [5]: Strip 0~3, ALL
		}
	}

	for (i = 0; i < MAX_DEF; i++)
		m_nDef[i] = 0;

	m_dMkBuffCurrPos = 0.0;
}

CGvisR2R_PunchDoc::~CGvisR2R_PunchDoc()
{
	int k, i;

	// 	PinImgFree();
	// 	AlignImgFree();
	// 	PcsImgFree();
	// 	CADLinkImgFree();

	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	if (m_pReelMap)
	{
		delete m_pReelMap;
		m_pReelMap = NULL;
	}

	if (m_pReelMapUp)
	{
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}

	if (m_pReelMapDn)
	{
		delete m_pReelMapDn;
		m_pReelMapDn = NULL;
	}

	if (m_pReelMapAllUp)
	{
		delete m_pReelMapAllUp;
		m_pReelMapAllUp = NULL;
	}

	if (m_pReelMapAllDn)
	{
		delete m_pReelMapAllDn;
		m_pReelMapAllDn = NULL;
	}

	if (pMkInfo)
	{
		delete[] pMkInfo;
		pMkInfo = NULL;
	}

	if (m_pMpeIb)
	{
		delete[] m_pMpeIb;
		m_pMpeIb = NULL;
	}

	if (m_pMpeIF)
	{
		delete[] m_pMpeIF;
		m_pMpeIF = NULL;
	}

	if (m_pMpeIo)
	{
		delete[] m_pMpeIo;
		m_pMpeIo = NULL;
	}

	if (m_pMpeIoF)
	{
		delete[] m_pMpeIoF;
		m_pMpeIoF = NULL;
	}

	if (m_pMpeSignal)
	{
		delete[] m_pMpeSignal;
		m_pMpeSignal = NULL;
	}

	if (m_pMpeData)
	{
		for (k = 0; k < m_nMpeData; k++)
		{
			if (m_pMpeData[k])
			{
				delete[] m_pMpeData[k];
				m_pMpeData[k] = NULL;
			}
		}
		delete[] m_pMpeData;
		m_pMpeData = NULL;
	}

// 	if(m_pPrevMpeIo)
// 	{
// 		delete[] m_pPrevMpeIo;
// 		m_pPrevMpeIo = NULL;
// 	}

	if (m_pSpecLocal)
	{
		delete m_pSpecLocal;
		m_pSpecLocal = NULL;
	}

	//Sleep(3000);

	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
		{
			if (m_pPcr[i][k])
			{
				delete m_pPcr[i][k];
				m_pPcr[i][k] = NULL;
			}
		}
		//delete[] m_pPcr[i];
		//m_pPcr[i] = NULL;
	}
}

BOOL CGvisR2R_PunchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGvisR2R_PunchDoc serialization

void CGvisR2R_PunchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGvisR2R_PunchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGvisR2R_PunchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGvisR2R_PunchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGvisR2R_PunchDoc 진단

#ifdef _DEBUG
void CGvisR2R_PunchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGvisR2R_PunchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGvisR2R_PunchDoc 명령

void CGvisR2R_PunchDoc::UpdateData()
{
	if (!m_pReelMap)
		return;

	m_pReelMap->m_sMc = WorkingInfo.System.sMcName;
	m_pReelMap->m_sUser = WorkingInfo.LastJob.sSelUserName;
	// 	m_pReelMap->m_sModel = WorkingInfo.LastJob.sModel;	
	// 	m_pReelMap->m_sLayer = WorkingInfo.LastJob.sLayer;	
	// 	m_pReelMap->m_sLot = WorkingInfo.LastJob.sLot;		

	m_pReelMap->m_bUseLotSep = WorkingInfo.LastJob.bLotSep;
	m_pReelMap->m_bUseTempPause = WorkingInfo.LastJob.bTempPause;

	m_pReelMap->m_dTotLen = _tstof(WorkingInfo.LastJob.sReelTotLen)*1000.0;
	m_pReelMap->m_dPnlLen = _tstof(WorkingInfo.LastJob.sOnePnlLen);
	m_pReelMap->m_dLotCutPosLen = _tstof(WorkingInfo.LastJob.sLotCutPosLen);
	if (m_pReelMap->m_bUseLotSep)
		m_pReelMap->m_dLotLen = _tstof(WorkingInfo.LastJob.sLotSepLen);
	else
		m_pReelMap->m_dLotLen = m_pReelMap->m_dTotLen;
	m_pReelMap->m_dTempPauseLen = _tstof(WorkingInfo.LastJob.sTempPauseLen);

}

BOOL CGvisR2R_PunchDoc::LoadMySpec() // 상면 기준으로 마킹부 상면작업 로컬데이타만.
{
	if (m_pSpecLocal)
	{
		delete m_pSpecLocal;
		m_pSpecLocal = NULL;
	}
	if (!WorkingInfo.LastJob.sModelUp.IsEmpty() && !WorkingInfo.LastJob.sLayerUp.IsEmpty())
	{
		if (!m_pSpecLocal)
		{
			m_pSpecLocal = new CMySpec(WorkingInfo.LastJob.sModelUp, WorkingInfo.LastJob.sLayerUp);
			return FALSE;
		}
		else
		{
			if (IsPinData())// || IsMkOffsetData())
				m_pSpecLocal->Load();
		}
	}

	return TRUE;
}


BOOL CGvisR2R_PunchDoc::LoadIoInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = { _T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3, *token4;// , *token5;

	CString strIdx, strInSeg, strOutSeg;
	CString strIReg, strAddr, strMReg, strSymbol, strName, strMBoth;

	// MpeIO
	strPath.Format(_T("%s"), PATH_MP2100_IO);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nOutSeg = (long)0L;


	if (MkIo.MpeIo.nInSeg > MkIo.MpeIo.nOutSeg)
		nSeg = MkIo.MpeIo.nInSeg;
	else
		nSeg = MkIo.MpeIo.nOutSeg;

	MkIo.MpeIo.pAddrIn = new CString*[MkIo.MpeIo.nInSeg];
	for (k = 0; k < MkIo.MpeIo.nInSeg; k++)
		MkIo.MpeIo.pAddrIn[k] = new CString[16];

	MkIo.MpeIo.pAddrOut = new CString*[MkIo.MpeIo.nOutSeg];
	for (k = 0; k < MkIo.MpeIo.nOutSeg; k++)
		MkIo.MpeIo.pAddrOut[k] = new CString[16];

	MkIo.MpeIo.pSymIn = new CString*[MkIo.MpeIo.nInSeg];
	for (k = 0; k < MkIo.MpeIo.nInSeg; k++)
		MkIo.MpeIo.pSymIn[k] = new CString[16];

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeIo.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeIo.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strSymbol = CString(token2);
					strName = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strSymbol = _T("");
					strName = _T("");
				}
				MkIo.MpeIo.pAddrIn[k][nR] = strAddr;
				strIReg = strSymbol.Right(4);
				MkIo.MpeIo.pSymIn[k][nR] = _T("IB0") + strIReg;
			}

			if (k < MkIo.MpeIo.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);
					token4 = _tcstok(NULL, sep);
					//token5 = _tcstok(NULL,sep);

					strAddr = CString(token1);
					//strMReg = CString(token2);
					strSymbol = CString(token2);
					strName = CString(token3);
					strMBoth = CString(token4);
				}
				else
				{
					strAddr = _T("");
					//strMReg = _T("");
					strSymbol = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeIo.pAddrOut[k][nR] = strAddr;//strMReg;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadSignalInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3;// , *token4;

	CString strIdx, strInSeg, strOutSeg;
	CString strAddr, strName, strMBoth;


	// MpeSignal
	strPath.Format(_T("%s"), PATH_MP2100_SIGNAL);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nOutSeg = (long)0L;


	if (MkIo.MpeSignal.nInSeg > MkIo.MpeSignal.nOutSeg)
		nSeg = MkIo.MpeSignal.nInSeg;
	else
		nSeg = MkIo.MpeSignal.nOutSeg;

	MkIo.MpeSignal.pAddrIn = new CString*[MkIo.MpeSignal.nInSeg];
	for (k = 0; k < MkIo.MpeSignal.nInSeg; k++)
		MkIo.MpeSignal.pAddrIn[k] = new CString[16];

	MkIo.MpeSignal.pAddrOut = new CString*[MkIo.MpeSignal.nOutSeg];
	for (k = 0; k < MkIo.MpeSignal.nOutSeg; k++)
		MkIo.MpeSignal.pAddrOut[k] = new CString[16];


	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeSignal.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeSignal.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
				}
				MkIo.MpeSignal.pAddrIn[k][nR] = strAddr;
			}

			if (k < MkIo.MpeSignal.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
					strMBoth = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeSignal.pAddrOut[k][nR] = strAddr;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadDataInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3;// , *token4;

	CString strIdx, strInSeg, strOutSeg;
	CString strAddr, strName, strMBoth;


	// MpeSignal
	strPath.Format(_T("%s"), PATH_MP2100_DATA);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nOutSeg = (long)0L;


	if (MkIo.MpeData.nInSeg > MkIo.MpeData.nOutSeg)
		nSeg = MkIo.MpeData.nInSeg;
	else
		nSeg = MkIo.MpeData.nOutSeg;

	MkIo.MpeData.pAddrIn = new CString*[MkIo.MpeData.nInSeg];
	for (k = 0; k < MkIo.MpeData.nInSeg; k++)
		MkIo.MpeData.pAddrIn[k] = new CString[16];

	MkIo.MpeData.pAddrOut = new CString*[MkIo.MpeData.nOutSeg];
	for (k = 0; k < MkIo.MpeData.nOutSeg; k++)
		MkIo.MpeData.pAddrOut[k] = new CString[16];


	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeData.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeData.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
				}
				MkIo.MpeData.pAddrIn[k][nR] = strAddr;
			}

			if (k < MkIo.MpeData.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
					strMBoth = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeData.pAddrOut[k][nR] = strAddr;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadWorkingInfo()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	// [System]

	// 20160926 - syd
	if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), NULL, szData, sizeof(szData), sPath))
	{
		m_bUseRTRYShiftAdjust = (BOOL)(_ttoi(szData) ? TRUE : FALSE);
	}
	else
		m_bUseRTRYShiftAdjust = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), NULL, szData, sizeof(szData), sPath))
	{
		m_dShiftAdjustRatio = (double)_ttof(szData);
	}
	else
		m_dShiftAdjustRatio = 0.5;


	if (0 < ::GetPrivateProfileString(_T("System"), _T("Password"), NULL, szData, sizeof(szData), sPath))
		pDoc->m_sPassword = CString(szData);
	else
		pDoc->m_sPassword = _T("gvis");

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MachineName"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMcName = CString(szData);
	else
	{
		AfxMessageBox(_T("설비이름이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sMcName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("CamSpecDir"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathCamSpecDir = CString(szData);
	else
	{
		AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathCamSpecDir = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUp = CString(szData);
	else
	{
		AfxMessageBox(_T("AOIUpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(szData);
	else
	{
		AfxMessageBox(_T("AoiUpCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpOffset = CString(szData);
	else
	{
		AfxMessageBox(_T("AoiUpCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUp VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpVrsData = CString(szData);
	else
	{
		AfxMessageBox(_T("AOIUp VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpVrsData = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDn = CString(szData);
	else
	{
		AfxMessageBox(_T("AOIDnPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(szData);
	else
	{
		AfxMessageBox(_T("AoiDnCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnOffset = CString(szData);
	else
	{
		AfxMessageBox(_T("AoiDnCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDn VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnVrsData = CString(szData);
	else
	{
		AfxMessageBox(_T("AOIDn VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnVrsData = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareUp = CString(szData);
	else
	{
		AfxMessageBox(_T("VrsShareUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufUp = CString(szData);
	else
	{
		AfxMessageBox(_T("VrsBufferUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareDn = CString(szData);
	else
	{
		AfxMessageBox(_T("VrsShareDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufDn = CString(szData);
	else
	{
		AfxMessageBox(_T("VrsBufferDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufDn = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsUp Data Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareUp = CString(szData);
	else
	{
		AfxMessageBox(_T("VSUp Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsDn Data Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareDn = CString(szData);
	else
	{
		AfxMessageBox(_T("VSDn Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareDn = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsOldFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathOldFile = CString(szData);
	else
	{
		AfxMessageBox(_T("VRS 완료 파일 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathOldFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Sapp3Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathSapp3 = CString(szData);
	else
	{
		AfxMessageBox(_T("Sapp3Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathSapp3 = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveLog"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveLog = _ttoi(szData);
	else
		WorkingInfo.System.bSaveLog = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NO_MARKING"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bNoMk = _ttoi(szData);	// 0 : 마킹모드, 1 : 비젼모드
	else
		WorkingInfo.System.bNoMk = FALSE;			// 0 : 마킹모드, 1 : 비젼모드

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveMkImage"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveMkImg = _ttoi(szData);	
	else
		WorkingInfo.System.bSaveMkImg = FALSE;			

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseStripPcsRgnBin"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bStripPcsRgnBin = _ttoi(szData);
	else
		WorkingInfo.System.bStripPcsRgnBin = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("REVIEW_MARKING_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sReViewMkLen = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 ReView가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sReViewMkLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_REVIEW_MARKING"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bReViewMk = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bReViewMk = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMaxDispDefImg = CString(szData);
	else
		WorkingInfo.System.sMaxDispDefImg = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam1Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[0] = CString(szData);
	else
		WorkingInfo.System.sCamSn[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam2Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[1] = CString(szData);
	else
		WorkingInfo.System.sCamSn[1] = CString(_T(""));

	// READER 2D BARCODE
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(_T("192.168.100.99"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(_T("192.168.100.100"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_SR1000W] = CString(_T("9004"));

	// ENGRAVE PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(_T("100.100.100.113"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(_T("8800"));

	// PUNCH PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(_T("100.100.101.225"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_PUNCH] = CString(_T("8800"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Delay Show Time"), NULL, szData, sizeof(szData), sPath))
		m_nDelayShow = _ttoi(szData);
	else
		m_nDelayShow = 500;

	// [Last Job]
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Process Code"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sProcessNum = CString(szData);
	else
	{
		WorkingInfo.LastJob.sProcessNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("ModelUp Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sModelUp = CString(szData);
	else
	{
		AfxMessageBox(_T("ModelUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sModelUp = CString(_T(""));
	}
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("ModelDn Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sModelDn = CString(szData);
	else
	{
		AfxMessageBox(_T("ModelDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sModelDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerUp Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLayerUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LayerUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLayerUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Test Mode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nTestMode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nTestMode = 0;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDualTest = TRUE;

	if (WorkingInfo.LastJob.bDualTest)
	{
		if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerDn Name"), NULL, szData, sizeof(szData), sPath))
			WorkingInfo.LastJob.sLayerDn = CString(szData);
		else
		{
			AfxMessageBox(_T("LayerDn가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
			WorkingInfo.LastJob.sLayerDn = CString(_T(""));
		}
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LotUp No"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotUp = CString(szData);
	else
	{
		AfxMessageBox(_T("LotUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotUp = CString(_T(""));
	}
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LotDn No"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotDn = CString(szData);
	else
	{
		AfxMessageBox(_T("LotDn가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialUp = CString(szData);
	else
	{
		AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialDn = CString(szData);
	else
	{
		AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialUp = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialDn = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Operator Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSelUserName = CString(szData);
	else
	{
		AfxMessageBox(_T("작업자가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSelUserName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Reel Total Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sReelTotLen = CString(szData);
	else
	{
		AfxMessageBox(_T("Reel 전체 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sReelTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Panel Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sOnePnlLen = CString(szData);
	else
	{
		AfxMessageBox(_T("한 판넬 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sOnePnlLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLotSep = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLotSep = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotSepLen = CString(szData);
	else
	{
		AfxMessageBox(_T("Lot분리 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotSepLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotCutPosLen = CString(szData);
	else
	{
		AfxMessageBox(_T("Lot절단위치 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotCutPosLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTempPause = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTempPause = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Merging Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nMergingLayer = _ttoi(szData);
	else
		WorkingInfo.LastJob.nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseTempPause = WorkingInfo.LastJob.bTempPause;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sTempPauseLen = CString(szData);
	else
	{
		AfxMessageBox(_T("일시정지 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sTempPauseLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtTotYld = CString(szData);
	else
	{
		AfxMessageBox(_T("전체 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtTotYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtPatlYld = CString(szData);
	else
	{
		AfxMessageBox(_T("부분 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtPatlYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bContFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bContFixDef = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumRangeFixDef = CString(szData);
	else
	{
		AfxMessageBox(_T("연속 고정불량 판정 누적 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumRangeFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumContFixDef = CString(szData);
	else
	{
		AfxMessageBox(_T("연속 고정불량 판정 발생 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumContFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(szData);
	else
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(_T("5.0"));


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bRclDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiUpDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiUpDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiDnDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiDnDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUclDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngvDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngvDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bBufDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bBufDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispMkPcs = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispMkPcs = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bStopFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bStopFixDef = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkSftySen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiSftySen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sStripOutRatio = CString(szData);
	else
		WorkingInfo.LastJob.sStripOutRatio = _T("20.0"); // Minimum 20%

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCustomNeedRatio = CString(szData);
	else
		WorkingInfo.LastJob.sCustomNeedRatio = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Partial Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sPartialSpd = CString(szData);
	else
		WorkingInfo.LastJob.sPartialSpd = _T("10.0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Up Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dn Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = TRUE;


	// 	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light Value"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.LastJob.sLightVal = CString(szData);
	// 	else
	// 		WorkingInfo.LastJob.sLightVal = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLightOn = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLightOn = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Jog Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sJogSpd = CString(szData);
	else
		WorkingInfo.LastJob.sJogSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngraveOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngraveOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Verify"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVerify = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bVerify = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Verify Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sVerifyLen = CString(szData);
	else
		WorkingInfo.LastJob.sVerifyLen = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Review"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bReview = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bReview = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num for Furge"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeNum = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeNum = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Furge Num at Run"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeRun = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeRun = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sMkLastShot = CString(szData);
	else
		WorkingInfo.LastJob.sMkLastShot = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[0] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[0] = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[1] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[1] = _T("0");


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bOneMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bOneMetal = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Two Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTwoMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTwoMetal = FALSE;


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bSampleTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bSampleTest = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Recoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Recoiler = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Uncoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Uncoiler = FALSE;


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 2Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse2Layer = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse2Layer = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngraveOrderNum = CString(szData);
	else
		WorkingInfo.LastJob.sEngraveOrderNum = _T("");


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Align Methode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlignMethode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlignMethode = TWO_POINT;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseEngraveCleanner = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseEngraveCleanner = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnCleanner = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseAoiDnCleanner = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngraveCleanner = _ttoi(szData);
	else
		WorkingInfo.LastJob.bEngraveCleanner = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiDnCleanner = _ttoi(szData);
	else
		WorkingInfo.LastJob.bAoiDnCleanner = FALSE;


	// [Motion]
	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_ROLLAR_HOMMING_RUN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bBufHomming = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bBufHomming = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdOvrNum = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdErr = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		AfxMessageBox(_T("마킹부 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		AfxMessageBox(_T("AOI 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdDist = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVel = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogVel = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdAcc = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogAcc = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdTotLen = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTotVel = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkPatlVel = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdDist = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVel = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogVel = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdAcc = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogAcc = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdTotLen = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTotVel = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiPatlVel = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin0 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin0 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin1 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin1 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹0 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹0 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosX[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosY[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sStBufPos = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeSpd = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeAcc = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeAcc = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdLead = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdLead = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI(상)에서 AOI(하)까지 진행 Shot수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ON_START_WITH_NEW_PRODUCT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bStNewPrdt = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bStNewPrdt = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bMkTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bMkTq = TRUE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bAoiTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bAoiTq = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("SAFE_ZONE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sSafeZone = CString(szData);
	else
		WorkingInfo.Motion.sSafeZone = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionLength = CString(szData);
	else
		WorkingInfo.Motion.sCollisionLength = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_MARGIN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionMargin = CString(szData);
	else
		WorkingInfo.Motion.sCollisionMargin = CString(_T(""));

	// ENGRAVE ==========================================================================================================

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(szData);
	else
	{
		AfxMessageBox(_T("각인위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(szData);
	else
	{
		AfxMessageBox(_T("AOI 검사위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(szData);
	else
	{
		AfxMessageBox(_T("Punching 시작위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(szData);
	else
	{
		AfxMessageBox(_T("바코드 위치 이송 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(szData);
	else
	{
		AfxMessageBox(_T("바코드 위치 이송 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_AUTO_LIMIT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(szData);
	else
	{
		AfxMessageBox(_T("바코드 각인 위치 자동 보정 한계값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_MANUAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(szData);
	else
	{
		AfxMessageBox(_T("바코드 각인 위치 수동 보정값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdLead = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdVacOff = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부 롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		AfxMessageBox(_T("각인기 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveTq = CString(_T(""));
	}

	WorkingInfo.Motion.bEngraveTq = TRUE;	// "ENGRAVE_TENSION_SERVO_TORQUE_ON"


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(szData);
	else
	{
		AfxMessageBox(_T("각인부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sEngraveStBufPos = _T("");


	// [VoiceCoil0]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sPort = CString(szData);
	else
	{
		AfxMessageBox(_T("보이스코일0 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(szData);
	else
	{
		AfxMessageBox(_T("보이스코일0 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = _tstoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = FALSE;

	// [VoiceCoil1]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sPort = CString(szData);
	else
	{
		AfxMessageBox(_T("보이스코일1 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(szData);
	else
	{
		AfxMessageBox(_T("보이스코일1 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = FALSE;

	// [Fluck RS232 DATA]
#ifdef USE_FLUCK
	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("USE_FLUCK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bUse = _ttoi(szData) ? TRUE : FALSE;
	else
	{
		AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.bUse = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sPort = CString(szData);
	else
	{
		AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sBaudRate = CString(szData);
	else
	{
		AfxMessageBox(_T("Fluck 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bLf = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_THRESHOLD"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sThreshold = CString(szData);
	else
		WorkingInfo.Fluck.sThreshold = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#else

	if (0 < ::GetPrivateProfileString(_T("MarkingTest"), _T("TOTAL_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#endif

	// [Light RS232 DATA]
	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sPort = CString(szData);
	else
	{
		AfxMessageBox(_T("조명 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sBaudRate = CString(szData);
	else
	{
		AfxMessageBox(_T("조명 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bLf = FALSE;

	// [Light0]

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("조명0의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("조명0의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[0] = CString(_T(""));
	}

	// [Light1]

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("조명1의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("조명1의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[1] = CString(_T(""));
	}



	// [Vision0]
	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPinImgSz = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResX = CString(szData);
	else
	{
		AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResY = CString(szData);
	else
	{
		AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetX = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetY = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sStdScr = CString(szData);
	else
	{
		//		AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[0].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPtMatScr = CString(szData);
	else
	{
		AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPtMatScr = CString(_T(""));
	}

	// [Vision1]
	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPinImgSz = CString(szData);
	else
	{
		AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResX = CString(szData);
	else
	{
		AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResY = CString(szData);
	else
	{
		AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetX = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetY = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sStdScr = CString(szData);
	else
	{
		//		AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[1].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(szData);
	else
	{
		AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPtMatScr = CString(szData);
	else
	{
		AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPtMatScr = CString(_T(""));
	}


	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.dwStTick = _ttoi(szData);
	else
		WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sTotalReelDist = CString(szData);
	else
		WorkingInfo.Lot.sTotalReelDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sSeparateDist = CString(szData);
	else
		WorkingInfo.Lot.sSeparateDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sCuttingDist = CString(szData);
	else
		WorkingInfo.Lot.sCuttingDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sStopDist = CString(szData);
	else
		WorkingInfo.Lot.sStopDist = CString(_T(""));


	// [Marking0]

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[0].sAverDist = CString(_T(""));


	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkLimit = 31536000;


	// [Marking1]

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[1].sAverDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkLimit = 31536000;



	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("Use"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("StopOnOpen"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = TRUE;


	// [ElecCheck0]

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[0].sRepeatTestNum = CString(_T("3"));


	// [ElecCheck1]

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[1].sRepeatTestNum = CString(_T("3"));


	// Sapp3code =====================================================================================

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("OPEN"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_OPEN] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_OPEN] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT_USHORT"), NULL, szData, sizeof(szData), sPath))
	// 		m_nSapp3Code[SAPP3_SHORT_USHORT] = _ttoi(szData);
	// 	else
	// 		m_nSapp3Code[SAPP3_SHORT_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_SHORT] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_SHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("USHORT"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_USHORT] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("NICK"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_NICK] = _tstoi(szData);
	else
		m_nSapp3Code[SAPP3_NICK] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SPACE_EXTRA_PROTRUSION"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE_PAD"), NULL, szData, sizeof(szData), sPath))
	// 		m_nSapp3Code[SAPP3_PINHOLE_PAD] = _ttoi(szData);
	// 	else
	// 		m_nSapp3Code[SAPP3_PINHOLE_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_PINHOLE] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_PINHOLE] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PAD"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_PAD] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HOPEN"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_HOPEN] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_HOPEN] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HMISS_HPOS_HBAD"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = _tstoi(szData);
	else
		m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = 0;


	return TRUE;
}


int CGvisR2R_PunchDoc::GetMkCntL() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[0].nMkCnt);
}

void CGvisR2R_PunchDoc::AddMkCntL()
{
	WorkingInfo.Marking[0].nMkCnt++;
}

void CGvisR2R_PunchDoc::SaveMkCntL()
{
	int nNum = WorkingInfo.Marking[0].nMkCnt;
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkCntL(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[0].nMkCnt = nNum;
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, sPath);
}

int CGvisR2R_PunchDoc::GetMkLimitL() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[0].nMkLimit);
}

void CGvisR2R_PunchDoc::SetMkLimitL(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[0].nMkLimit = nNum;
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Limit"), sData, sPath);
}

int CGvisR2R_PunchDoc::GetMkCntR() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[1].nMkCnt);
}

void CGvisR2R_PunchDoc::AddMkCntR()
{
	WorkingInfo.Marking[1].nMkCnt++;
}

void CGvisR2R_PunchDoc::SaveMkCntR()
{
	int nNum = WorkingInfo.Marking[1].nMkCnt;
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkCntR(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[1].nMkCnt = nNum;
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, sPath);
}

int CGvisR2R_PunchDoc::GetMkLimitR() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[1].nMkLimit);
}

void CGvisR2R_PunchDoc::SetMkLimitR(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[1].nMkLimit = nNum;
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Limit"), sData, sPath);
}

void CGvisR2R_PunchDoc::SaveWorkingInfo()
{
	CString sData, sPath = PATH_WORKING_INFO;

	// [System]

	sData.Format(_T("%d"), WorkingInfo.System.bNoMk ? 1 : 0);					// 0 : 마킹모드, 1 : 비젼모드
	::WritePrivateProfileString(_T("System"), _T("NO_MARKING"), sData, sPath);

	sData = WorkingInfo.System.sReViewMkLen;
	::WritePrivateProfileString(_T("System"), _T("REVIEW_MARKING_LENGTH"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.System.bReViewMk ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("USE_REVIEW_MARKING"), sData, sPath);


	// [Last Job]
	sData = WorkingInfo.LastJob.sProcessNum;
	::WritePrivateProfileString(_T("Last Job"), _T("Process Code"), sData, sPath);

	sData = WorkingInfo.LastJob.sModelUp;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LotUp No"), sData, sPath);

	sData = WorkingInfo.LastJob.sSerialUp;
	::WritePrivateProfileString(_T("Last Job"), _T("Last SerialUp"), sData, sPath);

	sData = WorkingInfo.LastJob.sCompletedSerialUp;
	::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), sData, sPath);

	sData = WorkingInfo.LastJob.sModelDn;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelDn Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LotDn No"), sData, sPath);

	sData = WorkingInfo.LastJob.sSerialDn;
	::WritePrivateProfileString(_T("Last Job"), _T("Last SerialDn"), sData, sPath);

	sData = WorkingInfo.LastJob.sCompletedSerialDn;
	::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), sData, sPath);

	sData = WorkingInfo.LastJob.sSelUserName;
	::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sReelTotLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sOnePnlLen;
	::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bLotSep ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotSepLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotCutPosLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bTempPause ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.nMergingLayer); 	// [0]:AOI-Up , [1]:AOI-Dn
	::WritePrivateProfileString(_T("Last Job"), _T("Merging Layer"), sData, sPath);

	sData = WorkingInfo.LastJob.sTempPauseLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sLmtTotYld;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), sData, sPath);

	sData = WorkingInfo.LastJob.sLmtPatlYld;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bContFixDef ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sNumRangeFixDef;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sNumContFixDef;
	::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sUltraSonicCleannerStTim;
	::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bRclDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bEngvDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bUclDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bBufDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bDispMkPcs ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bStopFixDef ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkSftySen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Safty Senso"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiSftySen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), sData, sPath);

	// 	sData = WorkingInfo.LastJob.sLightVal;
	// 	::WritePrivateProfileString(_T("Last Job"), _T("Light Value"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bLightOn ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Light On"), sData, sPath);

	sData = WorkingInfo.LastJob.sJogSpd;
	::WritePrivateProfileString(_T("Last Job"), _T("Jog Speed"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotSerial;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Serial"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bEngraveOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bVerify ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Verify"), sData, sPath);

	sData = WorkingInfo.LastJob.sVerifyLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Verify Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bReview ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Review"), sData, sPath);

	// 	sData = WorkingInfo.LastJob.sDistAoiMk;
	// 	::WritePrivateProfileString(_T("Last Job"), _T("DistanceAoiMk"), sData, sPath);

	sData = WorkingInfo.LastJob.sFurgeNum;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num for Furge"), sData, sPath);

	sData = WorkingInfo.LastJob.sFurgeRun;
	::WritePrivateProfileString(_T("Last Job"), _T("Furge Num at Run"), sData, sPath);

	sData = WorkingInfo.LastJob.sMkLastShot;
	::WritePrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sAoiLastShot[0];
	::WritePrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sAoiLastShot[1];
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sEngraveLastShot;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sStripOutRatio;
	::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), sData, sPath);

	sData = WorkingInfo.LastJob.sCustomNeedRatio;
	::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), sData, sPath);

	sData = WorkingInfo.LastJob.sPartialSpd;
	::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bEngraveCleanner ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Cleanner"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiDnCleanner ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner"), sData, sPath);



	// [Motion]
	sData.Format(_T("%d"), WorkingInfo.Motion.bBufHomming ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_ROLLAR_HOMMING_RUN"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdAdjOffSet;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdOvrNum;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdErr;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkTq;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiTq;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdDist;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkJogVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdAcc;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sMkJogAcc;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sMkTotVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TOTAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkPatlVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_PARTIAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdDist;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiJogVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdAcc;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiJogAcc;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiTotVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TOTAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiPatlVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_PARTIAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosX[0];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosY[0];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosX[1];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosY[1];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosX[0];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS0_X"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosY[0];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosX[1];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS1_X"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosY[1];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStBufPos;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sFdMkAoiInitDist;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sFdAoiAoiDistShot;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), sData, sPath);

	// 	sData = WorkingInfo.Motion.sBufStdPosDist;
	// 	::WritePrivateProfileString(_T("Motion"), _T("BUF_STD_POS_FEEDING_INIT_DIST"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bStNewPrdt ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("ON_START_WITH_NEW_PRODUCT"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bMkTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bAoiTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bEngraveTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), sData, sPath);

	// Engrave =======================================================================================
	sData = WorkingInfo.Motion.sEngraveFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sFdBarcodeOffsetVel;
	::WritePrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sFdBarcodeOffsetAcc;
	::WritePrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_AUTO_LIMIT"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLaserAdjOffSetManual;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_MANUAL"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveTq;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), sData, sPath);


	sData = WorkingInfo.Motion.sFdEngraveAoiInitDist;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdErr;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdAdjOffSet;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdOvrNum;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveStBufPos;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), sData, sPath);


	// [Light]
	sData = WorkingInfo.Light.sPort;
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_COM_PORT"), sData, sPath);

	sData = WorkingInfo.Light.sBaudRate;
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_BAUDRATE"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Light.bCr ? 1 : 0);
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CR"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Light.bLf ? 1 : 0);
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_LF"), sData, sPath);

	// [Light0]

	sData = WorkingInfo.Light.sCh[0];
	::WritePrivateProfileString(_T("Light0"), _T("LIGHT_CHANNEL"), sData, sPath);

	sData = WorkingInfo.Light.sVal[0];
	::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), sData, sPath);

	// [Light1]

	sData = WorkingInfo.Light.sCh[1];
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_CHANNEL"), sData, sPath);

	sData = WorkingInfo.Light.sVal[1];
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), sData, sPath);


	// [Vision0]
	sData = WorkingInfo.Vision[0].sPinImgSz;
	::WritePrivateProfileString(_T("Vision0"), _T("PIN_IMG_SIZE"), sData, sPath);

	sData = WorkingInfo.Vision[0].sResX;
	::WritePrivateProfileString(_T("Vision0"), _T("RESOLUTION_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sResY;
	::WritePrivateProfileString(_T("Vision0"), _T("RESOLUTION_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sMkOffsetX;
	::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sMkOffsetY;
	::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sStdScr;
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosX[0];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosY[0];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosX[1];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosY[1];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sPtMatScr;
	::WritePrivateProfileString(_T("Vision0"), _T("PtMatchingScore"), sData, sPath);

	// [Vision1]
	sData = WorkingInfo.Vision[1].sPinImgSz;
	::WritePrivateProfileString(_T("Vision1"), _T("PIN_IMG_SIZE"), sData, sPath);

	sData = WorkingInfo.Vision[1].sResX;
	::WritePrivateProfileString(_T("Vision1"), _T("RESOLUTION_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sResY;
	::WritePrivateProfileString(_T("Vision1"), _T("RESOLUTION_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sMkOffsetX;
	::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sMkOffsetY;
	::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sStdScr;
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosX[0];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosY[0];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosX[1];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosY[1];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sPtMatScr;
	::WritePrivateProfileString(_T("Vision1"), _T("PtMatchingScore"), sData, sPath);

	// [Lot]
	sData.Format(_T("%d"), WorkingInfo.Lot.dwStTick);
	::WritePrivateProfileString(_T("Lot"), _T("Start Tick"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("Start Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("Start Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("Start Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("Start Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("Start Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("Start Second"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("End Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("End Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("End Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("End Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("End Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("End Second"), sData, sPath);
}


void CGvisR2R_PunchDoc::ClrPcr()
{
	//CString sMsg;

	int nIdx, i, k;
	for (i = 0; i < MAX_PCR; i++)
	{
		if (m_pPcr[i][0])
		{
			for (nIdx = 0; nIdx < MAX_PCR_PNL; nIdx++)
			{
				m_pPcr[i][nIdx]->m_nIdx = 0;							// m_nIdx : From 0 to nTot.....
				m_pPcr[i][nIdx]->m_nSerial = 0;
				m_pPcr[i][nIdx]->m_nErrPnl = 0;
				m_pPcr[i][nIdx]->m_sModel = _T("");
				m_pPcr[i][nIdx]->m_sLayer = _T("");
				m_pPcr[i][nIdx]->m_sLot = _T("");

				m_pPcr[i][nIdx]->m_nCamId = 0;
				m_pPcr[i][nIdx]->m_nTotDef = 0;
				m_pPcr[i][nIdx]->m_nTotRealDef = 0;

				//sMsg.Format(_T("%d"), m_pPcr[i][nIdx]->m_nTotDef);
				//AfxMessageBox(sMsg);

				//for (k = 0; k < m_pPcr[i][nIdx]->m_nTotDef; k++)
				//{
				//	m_pPcr[i][nIdx]->m_pLayer[k] = -1;
				//	m_pPcr[i][nIdx]->m_pDefPcs[k] = 0;
				//	m_pPcr[i][nIdx]->m_pDefPos[k].x = 0;
				//	m_pPcr[i][nIdx]->m_pDefPos[k].y = 0;
				//	m_pPcr[i][nIdx]->m_pDefType[k] = 0;
				//	m_pPcr[i][nIdx]->m_pCell[k] = 0;
				//	m_pPcr[i][nIdx]->m_pImgSz[k] = 0;
				//	m_pPcr[i][nIdx]->m_pImg[k] = 0;
				//	m_pPcr[i][nIdx]->m_pMk[k] = 0;
				//}
			}
		}
	}
}

BOOL CGvisR2R_PunchDoc::InitReelmap()
{
	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		// 		AfxMessageBox(strMsg,MB_ICONSTOP);
		return FALSE;
	}

	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMap)
	{
		//m_pReelMap->ResetReelmap();
		delete m_pReelMap;
		m_pReelMap = NULL;
	}
	m_pReelMap = new CReelMap(RMAP_NONE, MAX_DISP_PNL, nTotPcs);

	if (m_pReelMapUp)
	{
		//m_pReelMapUp->ResetReelmap();
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}
	m_pReelMapUp = new CReelMap(RMAP_UP, MAX_DISP_PNL, nTotPcs);
	//m_pReelMapUp->m_nLayer = RMAP_UP;

	if (bDualTest)
	{
		if (m_pReelMapDn)
		{
			//m_pReelMapDn->ResetReelmap();
			delete m_pReelMapDn;
			m_pReelMapDn = NULL;
		}
		m_pReelMapDn = new CReelMap(RMAP_DN, MAX_DISP_PNL, nTotPcs);
		//m_pReelMapDn->m_nLayer = RMAP_DN;

		if (m_pReelMapAllUp)
		{
			//m_pReelMapAllUp->ResetReelmap();
			delete m_pReelMapAllUp;
			m_pReelMapAllUp = NULL;
		}
		m_pReelMapAllUp = new CReelMap(RMAP_ALLUP, MAX_DISP_PNL, nTotPcs);
		//m_pReelMapAllUp->m_nLayer = RMAP_ALLUP;

		if (m_pReelMapAllDn)
		{
			//m_pReelMapAllDn->ResetReelmap();
			delete m_pReelMapAllDn;
			m_pReelMapAllDn = NULL;
		}
		m_pReelMapAllDn = new CReelMap(RMAP_ALLDN, MAX_DISP_PNL, nTotPcs);
		//m_pReelMapAllDn->m_nLayer = RMAP_ALLDN;
	}

	if (pMkInfo)
	{
		delete[] pMkInfo;
		pMkInfo = NULL;
	}
	if (!pMkInfo)
		pMkInfo = new CString[nTotPcs];

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapUp()
{
	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		// 		AfxMessageBox(strMsg,MB_ICONSTOP);
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;

	if (m_pReelMap)
	{
		//m_pReelMap->ResetReelmap();
		delete m_pReelMap;
		m_pReelMap = NULL;
	}
	m_pReelMap = new CReelMap(RMAP_NONE, MAX_DISP_PNL, nTotPcs);


	if (m_pReelMap->m_nLayer < 0)
		m_pReelMap->m_nLayer = pView->m_nSelRmap;

	if (m_pReelMap->m_nLayer == RMAP_UP || m_pReelMap->m_nLayer == RMAP_ALLUP)
	{
		//if (m_pReelMap)
		//{
		//	//m_pReelMap->ResetReelmap();
		//	delete m_pReelMap;
		//	m_pReelMap = NULL;
		//}
		//m_pReelMap = new CReelMap(MAX_DISP_PNL, nTotPcs);

		if (pMkInfo)
		{
			delete[] pMkInfo;
			pMkInfo = NULL;
		}
		if (!pMkInfo)
			pMkInfo = new CString[nTotPcs];
	}

	if (m_pReelMapUp)
	{
		//m_pReelMapUp->ResetReelmap();
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}
	m_pReelMapUp = new CReelMap(RMAP_UP, MAX_DISP_PNL, nTotPcs);
	//m_pReelMapUp->m_nLayer = RMAP_UP;

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
		{
			//m_pReelMapAllUp->ResetReelmap();
			delete m_pReelMapAllUp;
			m_pReelMapAllUp = NULL;
		}
		m_pReelMapAllUp = new CReelMap(RMAP_ALLUP, MAX_DISP_PNL, nTotPcs);
		//m_pReelMapAllUp->m_nLayer = RMAP_ALLUP;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		// 		AfxMessageBox(strMsg,MB_ICONSTOP);
		return FALSE;
	}

	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;

	if (m_pReelMap)
	{
		//m_pReelMap->ResetReelmap();
		delete m_pReelMap;
		m_pReelMap = NULL;
	}
	m_pReelMap = new CReelMap(RMAP_NONE, MAX_DISP_PNL, nTotPcs);


	if (m_pReelMap->m_nLayer < 0)
		m_pReelMap->m_nLayer = pView->m_nSelRmap;

	if (m_pReelMap->m_nLayer == RMAP_DN || m_pReelMap->m_nLayer == RMAP_ALLDN)
	{
		//if (m_pReelMap)
		//{
		//	//m_pReelMap->ResetReelmap();
		//	delete m_pReelMap;
		//	m_pReelMap = NULL;
		//}
		//m_pReelMap = new CReelMap(MAX_DISP_PNL, nTotPcs);

		if (pMkInfo)
		{
			delete[] pMkInfo;
			pMkInfo = NULL;
		}
		if (!pMkInfo)
			pMkInfo = new CString[nTotPcs];
	}

	if (m_pReelMapDn)
	{
		//m_pReelMapDn->ResetReelmap();
		delete m_pReelMapDn;
		m_pReelMapDn = NULL;
	}
	m_pReelMapDn = new CReelMap(RMAP_DN, MAX_DISP_PNL, nTotPcs);
	//m_pReelMapDn->m_nLayer = RMAP_DN;

	if (m_pReelMapAllDn)
	{
		//m_pReelMapAllDn->ResetReelmap();
		delete m_pReelMapAllDn;
		m_pReelMapAllDn = NULL;
	}
	m_pReelMapAllDn = new CReelMap(RMAP_ALLDN, MAX_DISP_PNL, nTotPcs);
	//m_pReelMapAllDn->m_nLayer = RMAP_ALLDN;

	return TRUE;
}

void CGvisR2R_PunchDoc::InitPcr()
{
	int k, i;
	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
		{
			if (m_pPcr[i][k])
			{
				delete m_pPcr[i][k];
				m_pPcr[i][k] = NULL;
			}

			m_pPcr[i][k] = new CDataMarking();
		}
	}
}

void CGvisR2R_PunchDoc::SetReelmap(int nDir)
{
	if (!m_pReelMap || !m_Master[0].m_pPcsRgn)
		return;

	if (!m_pReelMap->pFrmRgn || !m_pReelMap->pPcsRgn)
		return;

	int i, k;
	double fData1, fData2, fData3, fData4, fDistX, fDistY;
	double fWidth, fHeight, fRight, fBottom;

	m_pReelMap->nDir = nDir;

	int nTotPnl = m_pReelMap->nTotPnl;
	int nTotPcs = m_pReelMap->nTotPcs;

	// 	double dScale = (MasterInfo.dPixelSize/10.0);
	double dScale = (m_Master[0].MasterInfo.dPixelSize / 10.0);
	m_pReelMap->SetAdjRatio(dScale);
	dScale = m_pReelMap->GetAdjRatio();

	for (k = 0; k < nTotPnl; k++)
	{
		switch (nDir)
		{
		case ROT_NONE:
			fWidth = (m_Master[0].m_pPcsRgn->pPcs[0].right - m_Master[0].m_pPcsRgn->pPcs[0].left);
			fHeight = (m_Master[0].m_pPcsRgn->pPcs[0].bottom - m_Master[0].m_pPcsRgn->pPcs[0].top);
			fRight = m_Master[0].m_pPcsRgn->rtFrm.right - fWidth * (1.0 - RMAP_PCS_SCALE);
			fBottom = m_Master[0].m_pPcsRgn->rtFrm.bottom - fHeight * (1.0 - RMAP_PCS_SCALE);;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.left;
			m_pReelMap->pFrmRgn[k].top = m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
			m_pReelMap->pFrmRgn[k].bottom = fBottom;
			// 			m_pReelMap->pFrmRgn[k].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.left;
			// 			m_pReelMap->pFrmRgn[k].top = m_pPcsRgn->rtFrm.top;
			// 			m_pReelMap->pFrmRgn[k].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.right;
			// 			m_pReelMap->pFrmRgn[k].bottom = m_pPcsRgn->rtFrm.bottom;

			for (i = 0; i < nTotPcs; i++)
			{
				fWidth = (m_Master[0].m_pPcsRgn->pPcs[i].right - m_Master[0].m_pPcsRgn->pPcs[i].left);
				fHeight = (m_Master[0].m_pPcsRgn->pPcs[i].bottom - m_Master[0].m_pPcsRgn->pPcs[i].top);
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].left;	// left
				fData2 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// top
				fData3 = fData1 + fWidth * RMAP_PCS_SCALE; // right
				fData4 = fData2 + fHeight * RMAP_PCS_SCALE; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;
			}
			break;
			// 		case ROT_NONE:
			// 			m_pReelMap->pFrmRgn[k].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.left;
			// 			m_pReelMap->pFrmRgn[k].top = m_pPcsRgn->rtFrm.top;
			// 			m_pReelMap->pFrmRgn[k].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.right;
			// 			m_pReelMap->pFrmRgn[k].bottom = m_pPcsRgn->rtFrm.bottom;
			// 
			// 			for(i=0; i<nTotPcs; i++)
			// 			{
			// 				fData1 = m_pPcsRgn->pPcs[i].left; // left
			// 				fData2 = m_pPcsRgn->pPcs[i].top; // top
			// 				fData3 = m_pPcsRgn->pPcs[i].right; // right
			// 				fData4 = m_pPcsRgn->pPcs[i].bottom; // bottom
			// 
			// 				m_pReelMap->pPcsRgn[k][i].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+fData1;
			// 				m_pReelMap->pPcsRgn[k][i].top = fData2;
			// 				m_pReelMap->pPcsRgn[k][i].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+fData3;
			// 				m_pReelMap->pPcsRgn[k][i].bottom = fData4;
			// 			}
			// 			break;
		case ROT_CCW_90: // right->bottom, top->left, bottom->right, left->top ; Dir (x *= 1, y *= -1) 
			fDistX = 0;
			fDistY = m_Master[0].m_pPcsRgn->rtFrm.left + m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.bottom;
			m_pReelMap->pFrmRgn[k].bottom = fDistY - m_Master[0].m_pPcsRgn->rtFrm.left;

			for (i = 0; i < nTotPcs; i++)
			{
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// left
				fData2 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].right;	// top
				fData3 = m_Master[0].m_pPcsRgn->pPcs[i].bottom;	// right
				fData4 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].left; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;
			}
			break;
		}
	}
}

CString CGvisR2R_PunchDoc::GetCamPxlRes()
{
	CString sRes = _T("");
	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_PIN_IMG_;
#else
	if (pDoc->WorkingInfo.System.sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp);
	else
		sPath.Format(_T("%s%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp);
#endif

	int nPos = sPath.ReverseFind('-');
	if (nPos > 0)
	{
		sRes = sPath.Right(sPath.GetLength() - (nPos + 1));
		nPos = sRes.ReverseFind('.');
		if (nPos > 0)
			sRes = sRes.Left(nPos);
		WorkingInfo.Vision[0].sCamPxlRes = sRes;
		WorkingInfo.Vision[1].sCamPxlRes = sRes;
	}
	return sRes;
}

int CGvisR2R_PunchDoc::GetPcrIdx(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.8"));
		return 0;
	}

	int nIdx = -1;
	// 	if(!bNewLot)
	// 		nIdx = (nSerial-1)%MAX_PCR_PNL;
	// 	else // New Lot...
	// 		nIdx = (m_nLotLastShot + nSerial - 1)%MAX_PCR_PNL;

	return nIdx;
}

int CGvisR2R_PunchDoc::GetPcrIdx0(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.9"));
		return 0;
	}

	int nIdx;

	if (!bNewLot)
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
		{
			if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
				nIdx = (nSerial - 1) % MAX_PCR_PNL;
			else // New Lot...
				nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		}
		else
		{
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
		}
	}
	else
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		else
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	}
	return nIdx;
}

int CGvisR2R_PunchDoc::GetPcrIdx1(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.10"));
		return 0;
	}

	int nIdx;

	if (!bNewLot)
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
		{
			if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
				nIdx = (nSerial - 1) % MAX_PCR_PNL;
			else // New Lot...
				nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		}
		else
		{
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
		}
	}
	else
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		else
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	}
	return nIdx;
}

BOOL CGvisR2R_PunchDoc::GetAoiUpInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.11"));
		return 0;
	}

	BOOL Info0;//, Info1;
	Info0 = GetAoiInfoUp(nSerial, pNewLot, bFromBuf);

	if (Info0)
		return TRUE;

	// 	double dCurPosMkFd = (double)m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
	// 	double dTgtFd = _tstof(WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(WorkingInfo.Motion.sAoiFdDist);
	// 	if(dCurPosMkFd < dTgtFd-_tstof(WorkingInfo.Motion.sAoiFdDist) + 10.0)
	// 	{
	// 		if(Info0)
	// 			return TRUE;
	// 	}
	// 	else
	// 	{
	// 		Info1 = GetAoiInfoDn(nSerial);
	// 		if(Info0 && Info1)
	// 			return TRUE;
	// 	}

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::GetAoiDnInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.12"));
		return 0;
	}

	BOOL Info1;//Info0, 
			   // 	Info0 = GetAoiInfoUp(nSerial);

			   // 	double dCurPosMkFd = (double)m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
			   // 	double dTgtFd = _tstof(WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(WorkingInfo.Motion.sAoiFdDist);
			   // 	if(dCurPosMkFd < dTgtFd-_tstof(WorkingInfo.Motion.sAoiFdDist) + 10.0)
			   // 		return FALSE;
			   // 	else
	{
		Info1 = GetAoiInfoDn(nSerial, pNewLot, bFromBuf);
		if (Info1)
			return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::GetAoiInfoUp(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	//	if(bFromBuf)
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
	// 	else
	// 		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareUp, nSerial);
#endif

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	// 	m_pPcr[nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find('\n', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sLot = strLot;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);



	if (Status.PcrShare[0].sModel.IsEmpty() || Status.PcrShare[0].sLayer.IsEmpty() || Status.PcrShare[0].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		// 		AfxMessageBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	if (WorkingInfo.LastJob.sModelUp != Status.PcrShare[0].sModel || WorkingInfo.LastJob.sLayerUp != Status.PcrShare[0].sLayer || WorkingInfo.LastJob.sLotUp != Status.PcrShare[0].sLot)
	{
		if (m_bBufEmptyF[0])
		{
			if (!m_bBufEmpty[0])
				m_bBufEmptyF[0] = FALSE;

			WorkingInfo.LastJob.sModelUp = Status.PcrShare[0].sModel;
			WorkingInfo.LastJob.sLayerUp = Status.PcrShare[0].sLayer;
			WorkingInfo.LastJob.sLotUp = Status.PcrShare[0].sLot;
			return TRUE;
		}
	}

	if(bFromBuf)
	{
		if (WorkingInfo.LastJob.sLotUp != Status.PcrShare[0].sLot)
		{
			//if (GetLastShotUp() && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
			//{
				*pNewLot = 1;
				pView->m_sNewLotUp = Status.PcrShare[0].sLot;
			//}
			//else
			//{
				WorkingInfo.LastJob.sLotUp = Status.PcrShare[0].sLot;
				SetModelInfoUp();
				pView->OpenReelmapUp(); // At Start...
				pView->SetPathAtBufUp();
				if (pView->m_pDlgMenu01)
				{
					pView->m_pDlgMenu01->UpdateData();
					if (pView->m_nSelRmap == RMAP_UP || pView->m_nSelRmap == RMAP_ALLUP)
						pView->m_pDlgMenu01->OpenReelmap(pView->m_nSelRmap);
				}
			//}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::GetAoiInfoDn(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	// 	if(bFromBuf)
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
	// 	else
	// 		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareDn, nSerial);
#endif

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	// 	m_pPcr[nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find('\n', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sLot = strLot;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);



	if (Status.PcrShare[1].sModel.IsEmpty() || Status.PcrShare[1].sLayer.IsEmpty() || Status.PcrShare[1].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		// 		AfxMessageBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	if (WorkingInfo.LastJob.sModelDn != Status.PcrShare[1].sModel || WorkingInfo.LastJob.sLayerDn != Status.PcrShare[1].sLayer || WorkingInfo.LastJob.sLotDn != Status.PcrShare[1].sLot)
	{
		if (m_bBufEmptyF[1])
		{
			if (!m_bBufEmpty[1])
				m_bBufEmptyF[1] = FALSE;

			WorkingInfo.LastJob.sModelDn = Status.PcrShare[1].sModel;
			WorkingInfo.LastJob.sLayerDn = Status.PcrShare[1].sLayer;
			WorkingInfo.LastJob.sLotDn = Status.PcrShare[1].sLot;
			return TRUE;
		}
	}

	if (bFromBuf)
	{
		if (WorkingInfo.LastJob.sLotDn != Status.PcrShare[1].sLot)
		{
			//if (GetLastShotDn() && (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
			//{
				*pNewLot = 1;
				pView->m_sNewLotDn = Status.PcrShare[1].sLot;
			//}
			//else
			//{
				WorkingInfo.LastJob.sLotDn = Status.PcrShare[1].sLot;
				SetModelInfoDn();

				pView->OpenReelmapDn(); // At Start...
				pView->SetPathAtBufDn();
				if (pView->m_pDlgMenu01)
				{
					pView->m_pDlgMenu01->UpdateData();
					if (pView->m_nSelRmap == RMAP_DN || pView->m_nSelRmap == RMAP_ALLDN)
						pView->m_pDlgMenu01->OpenReelmap(pView->m_nSelRmap);
				}
			//}
		}
	}

	return FALSE;
}

int CGvisR2R_PunchDoc::LoadPCR(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.13"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nRtn[4];
	nRtn[0] = LoadPCRUp(nSerial, bFromShare);
	if (bDualTest)
	{
		nRtn[1] = LoadPCRDn(nSerial, bFromShare);
#ifndef TEST_MODE
		nRtn[2] = LoadPCRAllUp(nSerial, bFromShare);
		nRtn[3] = LoadPCRAllDn(nSerial, bFromShare);
#endif
	}

	if (nRtn[0] != 1)
		return nRtn[0];
	if (bDualTest)
	{
		if (nRtn[1] != 1)
			return nRtn[1];
#ifndef TEST_MODE
		if (nRtn[2] != 1)
			return nRtn[2];
#endif
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCR0(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.14"));
		return 0;
	}

	int nRtn[2];
	nRtn[0] = LoadPCRUp(nSerial, bFromShare);
	//	nRtn[1] = LoadPCRAllUp(nSerial, bFromShare);

	if (nRtn[0] != 1)
		return nRtn[0];
	// 	if( nRtn[1] != 1 )
	// 		return nRtn[1];

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCR1(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.15"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nRtn[3];
	nRtn[0] = LoadPCRDn(nSerial, bFromShare);
	nRtn[1] = LoadPCRAllDn(nSerial, bFromShare);
	nRtn[2] = LoadPCRAllUp(nSerial, bFromShare);

	if (nRtn[0] != 1)
		return nRtn[0];
	if (nRtn[1] != 1)
		return nRtn[1];
	if (nRtn[2] != 1)
		return nRtn[2];

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRAllUp(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
#ifdef TEST_MODE
	return 0;
#endif

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.16"));
		return 0;
	}

	int i, idx;//, k
	CString str;

	if (nSerial < 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[2])
	{
		str.Format(_T("PCR[2]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE);
	else
		nIdx = GetPcrIdx0(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[2])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[2][nIdx])
		return(2);

	m_pPcr[2][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[2][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[2][nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcr[2][nIdx]->m_sLayer = m_pPcr[0][nIdx]->m_sLayer;

	// Lot
	m_pPcr[2][nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3];
	nTotDef[0] = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	nTotDef[1] = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	//	nTotDef[2] = nTotDef[0] + nTotDef[1];

	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs];

	int nComp, nPcsId;
	if (nTotDef[0] > nTotDef[1])
		nComp = nTotDef[0];
	else
		nComp = nTotDef[1];

	// 	for(i=0; i<nComp; i++)
	// 	{
	// 		if(i<nTotDef[1])
	// 		{
	// 			nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
	// 			pPcrMgr[nPcsId].nIdx = i;
	// 			pPcrMgr[nPcsId].nIdxDn = i; // Dn
	// 		}
	// 		if(i<nTotDef[0])
	// 		{
	// 			nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
	// 			pPcrMgr[nPcsId].nIdx = i;
	// 			pPcrMgr[nPcsId].nIdxUp = i;	// Up
	// 		}
	// 	}

	for (i = 0; i < nTotDef[1]; i++)
	{
		nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxDn = i; // Dn
	}
	for (i = 0; i < nTotDef[0]; i++)
	{
		nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxUp = i;	// Up
	}


	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)
	{
		if (pPcrMgr[i].nIdx > -1)
			nTotDef[2]++;
	}

	m_pPcr[2][nIdx]->Init(nSerial, nTotDef[2]);

	int nId[2];
	idx = 0;
	if (nTotDef[2] > 0)
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;
			nId[1] = pPcrMgr[nPcsId].nIdxDn;

			if (nId[0] > -1)
			{
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[nId[0]];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[nId[0]];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[nId[0]].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[nId[0]].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[nId[0]];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[nId[0]];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[nId[0]];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[nId[0]];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[nId[0]];

				idx++;
			}
			else if (nId[1] > -1)
			{
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[nId[1]];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[nId[1]];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[nId[1]].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[nId[1]].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[nId[1]];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[nId[1]];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[nId[1]];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[nId[1]];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[nId[1]];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRAllDn(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	int i, idx;//k, 
	CString str;

	if (nSerial <= 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[3])
	{
		str.Format(_T("PCR[3]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[3])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[3][nIdx])
		return(2);

	m_pPcr[3][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[3][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[3][nIdx]->m_sModel = m_pPcr[1][nIdx]->m_sModel;

	// Layer
	m_pPcr[3][nIdx]->m_sLayer = m_pPcr[1][nIdx]->m_sLayer;

	// Lot
	m_pPcr[3][nIdx]->m_sLot = m_pPcr[1][nIdx]->m_sLot;

	int nTotDef[3];
	nTotDef[0] = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	nTotDef[1] = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	//	nTotDef[2] = nTotDef[0] + nTotDef[1];

	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs];

	int nComp, nPcsId;
	if (nTotDef[0] > nTotDef[1])
		nComp = nTotDef[0];
	else
		nComp = nTotDef[1];

	// 	for(i=0; i<nComp; i++)
	// 	{
	// 		if(i<nTotDef[0])
	// 		{
	// 			nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
	// 			pPcrMgr[nPcsId].nIdx = i;
	// 			pPcrMgr[nPcsId].nIdxUp = i;	// Up
	// 		}
	// 		if(i<nTotDef[1])
	// 		{
	// 			nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
	// 			pPcrMgr[nPcsId].nIdx = i;
	// 			pPcrMgr[nPcsId].nIdxDn = i; // Dn
	// 		}
	// 	}
	for (i = 0; i < nTotDef[0]; i++)
	{
		nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxUp = i;	// Up
	}
	for (i = 0; i < nTotDef[1]; i++)
	{
		nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxDn = i; // Dn
	}

	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)
	{
		if (pPcrMgr[i].nIdx > -1)
			nTotDef[2]++;
	}

	m_pPcr[3][nIdx]->Init(nSerial, nTotDef[2]);

	int nId[2];
	idx = 0;
	if (nTotDef[2] > 0)
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;
			nId[1] = pPcrMgr[nPcsId].nIdxDn;

			if (nId[1] > -1)
			{
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[nId[1]];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[nId[1]];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[nId[1]].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[nId[1]].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[nId[1]];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[nId[1]];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[nId[1]];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[nId[1]];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[nId[1]];

				idx++;
			}
			else if (nId[0] > -1)
			{
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[nId[0]];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[nId[0]];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[nId[0]].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[nId[0]].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[nId[0]];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[nId[0]];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[nId[0]];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[nId[0]];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[nId[0]];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRUp(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0])
	{
		strFileData.Format(_T("PCR[0]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE);
	else
		nIdx = GetPcrIdx0(nSerial);

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	if (bFromShare)
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareUp, nSerial);
	else
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
#endif

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Up] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0])
		return(2);
	if (!m_pPcr[0][nIdx])
		return(2);

	BOOL bResetMkInfo = FALSE;

	m_pPcr[0][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[0][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find('\n', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLot = strLot;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	if (!strModel.IsEmpty() && !strLot.IsEmpty() && !strLayer.IsEmpty())
	{
		if (WorkingInfo.LastJob.sModelUp.IsEmpty() || WorkingInfo.LastJob.sLotUp.IsEmpty() || WorkingInfo.LastJob.sLayerUp.IsEmpty())
		{
			WorkingInfo.LastJob.sModelUp = strModel;
			WorkingInfo.LastJob.sLotUp = strLot;
			WorkingInfo.LastJob.sLayerUp = strLayer;

			if (!WorkingInfo.LastJob.bDualTest)
			{
				pView->ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
			}
		}
	}

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[0][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			m_pPcr[0][nIdx]->m_pLayer[i] = 0; // Up

			// BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
				// 	return(m_pPcr[0][nIdx]->m_nErrPnl);
}

int CGvisR2R_PunchDoc::LoadPCRDn(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1])
	{
		strFileData.Format(_T("PCR[1]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);
	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	if (bFromShare)
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareDn, nSerial);
	else
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
#endif

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Dn] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		//		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1])
		return(2);
	if (!m_pPcr[1][nIdx])
		return(2);

	m_pPcr[1][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[1][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find('\n', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLot = strLot;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;


	if (!strModel.IsEmpty() && !strLot.IsEmpty() && !strLayer.IsEmpty())
	{
		if (WorkingInfo.LastJob.sModelDn.IsEmpty() || WorkingInfo.LastJob.sLotDn.IsEmpty() || WorkingInfo.LastJob.sLayerDn.IsEmpty())
		{
			WorkingInfo.LastJob.sModelDn = strModel;
			WorkingInfo.LastJob.sLotDn = strLot;
			WorkingInfo.LastJob.sLayerDn = strLayer;

			if (WorkingInfo.LastJob.bDualTest)
			{
				pView->ResetMkInfo(2); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
			}
		}
	}


	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[1][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)
				m_pPcr[1][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);				// DTS용
			else
				m_pPcr[1][nIdx]->m_pDefPcs[i] = Mirroring(_tstoi(strPieceID));	// 초기 양면검사기용

			m_pPcr[1][nIdx]->m_pLayer[i] = 1; // Dn

			// BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
	//return(m_pPcr[1][nIdx]->m_nErrPnl);
}

BOOL CGvisR2R_PunchDoc::CopyDefImg(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.17"));
		return 0;
	}

	BOOL bRtn[2];
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		bRtn[0] = CopyDefImgUp(nSerial);
		bRtn[1] = CopyDefImgDn(nSerial);

		if (!bRtn[0] || !bRtn[1])
			return FALSE;
	}
	else
	{
		bRtn[0] = CopyDefImgUp(nSerial);

		if (!bRtn[0])
			return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_PunchDoc::CopyDefImg(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bUp, bDn;
	if (bDualTest)
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);
		bDn = CopyDefImgDn(nSerial, sNewLot);

		if (!bUp || !bDn)
			return FALSE;
	}
	else
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);

		if (!bUp)
			return FALSE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::CopyDefImgUp(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.18"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD, strMakeFolderPath;
	int i;
	CString strAOIImgDataPath;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	if (sNewLot.IsEmpty())
		sLot = WorkingInfo.LastJob.sLotUp;
	else
		sLot = sNewLot;

	strAOIImgDataPath.Format(_T("%s\\VRSImage"), WorkingInfo.System.sPathAoiUp);


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp);
	else
		strMakeFolderPath.Format(_T("%s%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp);

	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelUp,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	int nIdx = GetIdxPcrBufUp(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[0])
	{
		if (m_pPcr[0][nIdx])
			nTotDef = m_pPcr[0][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[0][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[0][nIdx]->m_pImg[i];
			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
				}
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::CopyDefImgDn(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.19"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD, strMakeFolderPath;
	int i;
	CString strAOIImgDataPath;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	if (sNewLot.IsEmpty())
		sLot = WorkingInfo.LastJob.sLotDn;
	else
		sLot = sNewLot;

	strAOIImgDataPath.Format(_T("%s\\VRSImage"), WorkingInfo.System.sPathAoiDn);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn);
	else
		strMakeFolderPath.Format(_T("%s%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModelDn,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);
	//if (!finder.FindFile(strMakeFolderPath))
	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	int nIdx = GetIdxPcrBufDn(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[1])
	{
		if (m_pPcr[1][nIdx])
			nTotDef = m_pPcr[1][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[1][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[1][nIdx]->m_pImg[i];

			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
				}
			}
		}
	}

	return TRUE;
}


int CGvisR2R_PunchDoc::GetIdxPcrBuf(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.20"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[0])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[0][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

int CGvisR2R_PunchDoc::GetIdxPcrBufUp(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.21"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[0])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[0][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

int CGvisR2R_PunchDoc::GetIdxPcrBufDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.22"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[1])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[1][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

void CGvisR2R_PunchDoc::DeleteFileInFolder(CString sPathDir)
{
	BOOL bRes;
	CFileFind ff;

	if (CheckPath(sPathDir) == PATH_IS_FILE)
	{
		DeleteFile(sPathDir);
		return;
	}

	if (sPathDir.Right(1) != _T("\\"))
		sPathDir.Append(_T("\\")); //sPathDir += _T("\\");

	sPathDir.Append(_T("*.*")); //sPathDir += _T("*.*");

	bRes = ff.FindFile(sPathDir);

	while (bRes)
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

int CGvisR2R_PunchDoc::CheckPath(CString strPath)
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

int CGvisR2R_PunchDoc::GetLastSerial()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nMkLastShot;

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLastShotMk()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nMkLastShot;

	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (!nLastShot && pView->m_bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLotDn,
					WorkingInfo.LastJob.sLayerDn);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLotDn,
					WorkingInfo.LastJob.sLayerDn);

			sRmapPath.Format(_T("%s\\ReelMapDataDn.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}
	else
	{
		if (!nLastShot && pView->m_bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerUp);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerUp);

			sRmapPath.Format(_T("%s\\ReelMapDataUp.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}

	return (nLastShot);
}


int CGvisR2R_PunchDoc::GetLastShotUp()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[0];

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLastShotDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[1];

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLotSerial()
{
	int nLotSerial;

	double dLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * (m_pReelMap->m_nSerial - 1);
	if (dLen < _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen))
		nLotSerial = 1;
	else
		nLotSerial = 2;

	return nLotSerial;
}

void CGvisR2R_PunchDoc::SetMkPnt(int nCam)	// Pin위치에 의한 정렬.
{
	if (m_Master[0].m_pPcsRgn)
		m_Master[0].m_pPcsRgn->SetMkPnt(nCam);
}

void CGvisR2R_PunchDoc::SetOnePnlLen(double dLen)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLen);
	WorkingInfo.LastJob.sOnePnlLen = sVal;
	WorkingInfo.Motion.sMkFdDist = WorkingInfo.Motion.sAoiFdDist = sVal;
	m_pReelMap->m_dPnlLen = dLen;
	::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45032"), lData);	// 한 판넬 길이 (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetOnePnlLen()
{
	return (_tstof(WorkingInfo.Motion.sMkFdDist));
}

void CGvisR2R_PunchDoc::SetOnePnlVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkFdVel = WorkingInfo.Motion.sAoiFdVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dVel * 1000.0);
	pView->m_pMpe->Write(_T("ML45034"), lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
#endif
}

void CGvisR2R_PunchDoc::SetFdJogVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogVel = WorkingInfo.Motion.sAoiJogVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dVel * 1000.0);
	pView->m_pMpe->Write(_T("ML45038"), lData);	// 연속공급 속도 (단위 mm/sec * 1000)
#endif
}

void CGvisR2R_PunchDoc::SetFdJogAcc(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogAcc = WorkingInfo.Motion.sAoiJogAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dVel * 1000.0);
	pView->m_pMpe->Write(_T("ML45040"), lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetOnePnlVel()
{
	return (_tstof(WorkingInfo.Motion.sMkFdVel));
}

double CGvisR2R_PunchDoc::GetFdJogVel()
{
	return (_tstof(WorkingInfo.Motion.sMkJogVel));
}

double CGvisR2R_PunchDoc::GetFdJogAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkJogAcc));
}

void CGvisR2R_PunchDoc::SetOnePnlAcc(double dAcc)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sMkFdAcc = WorkingInfo.Motion.sAoiFdAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dAcc * 1000.0);
	pView->m_pMpe->Write(_T("ML45036"), lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetOnePnlAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkFdAcc));
}

void CGvisR2R_PunchDoc::SetAoiFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sAoiFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dPitch * 1000.0);
	pView->m_pMpe->Write(_T("ML45012"), lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	pView->m_pMpe->Write(_T("ML45020"), lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetAoiFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sAoiFdLead));
}

void CGvisR2R_PunchDoc::SetMkFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sMkFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dPitch * 1000.0);
	pView->m_pMpe->Write(_T("ML45014"), lData);	// 마킹부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetMkFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sMkFdLead));
}

void CGvisR2R_PunchDoc::SetFdErrLmt(double dLmt)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLmt);
	WorkingInfo.Motion.sLmtFdErr = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetFdErrLmt()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdErr));
}

void CGvisR2R_PunchDoc::SetFdErrRng(double dRng)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dRng);
	WorkingInfo.Motion.sLmtFdAdjOffSet = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetFdErrRng()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdAdjOffSet));
}

void CGvisR2R_PunchDoc::SetFdErrNum(int nNum)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%d"), nNum);
	WorkingInfo.Motion.sLmtFdOvrNum = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), sVal, sPath);
}

int CGvisR2R_PunchDoc::GetFdErrNum()
{
	return (_tstoi(WorkingInfo.Motion.sLmtFdOvrNum));
}

void CGvisR2R_PunchDoc::SetBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dPos * 1000.0);
	pView->m_pMpe->Write(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
}

void CGvisR2R_PunchDoc::SetBufInitPos(double dVel, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sBufHomeSpd = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), sData, sPath);
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sBufHomeAcc = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetBuffInitPos()
{
	return (_tstof(WorkingInfo.Motion.sStBufPos));
}

void CGvisR2R_PunchDoc::SetAoiMkDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdMkAoiInitDist = sData;
	//::WritePrivateProfileString(_T("Motion"), _T("FEEDING_INIT_DIST_OFFSET"), sData, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45008"), lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
#endif
}

void CGvisR2R_PunchDoc::SetAoiAoiDist(int nShot)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nShot);
	WorkingInfo.Motion.sFdAoiAoiDistShot = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(nShot * 1000);
	pView->m_pMpe->Write(_T("ML45010"), lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
#endif
}


double CGvisR2R_PunchDoc::GetAoiMkDist()
{
	return (_tstof(WorkingInfo.Motion.sFdMkAoiInitDist));
}

int CGvisR2R_PunchDoc::GetAoiAoiDist()
{
	return (_tstoi(WorkingInfo.Motion.sFdAoiAoiDistShot));
}

void CGvisR2R_PunchDoc::SetWaitPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitPos));
}

void CGvisR2R_PunchDoc::SetWaitVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitVel));
}

void CGvisR2R_PunchDoc::SetWaitAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitAcc));
}

void CGvisR2R_PunchDoc::SetMarkingPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingPos));
}

void CGvisR2R_PunchDoc::SetMarkingVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingVel));
}

void CGvisR2R_PunchDoc::SetMarkingAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingAcc));
}

void CGvisR2R_PunchDoc::SetMarkingToq1(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[0].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMarkingToq1Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[0].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingToq1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingToq));
}

void CGvisR2R_PunchDoc::SetPosX1_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[0]));
}

void CGvisR2R_PunchDoc::SetPosY1_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[0]));
}

void CGvisR2R_PunchDoc::SetPosX1_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[1]));
}

void CGvisR2R_PunchDoc::SetPosY1_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[1]));
}

void CGvisR2R_PunchDoc::SetPosX1_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisR2R_PunchDoc::SetPosY1_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[2]));
}

void CGvisR2R_PunchDoc::SetPosX1_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[3]));
}

void CGvisR2R_PunchDoc::SetPosY1_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[3]));
}

void CGvisR2R_PunchDoc::SetAverDist1(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[0].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetAverDist1()
{
	return (_tstof(WorkingInfo.Marking[0].sAverDist));
}

void CGvisR2R_PunchDoc::SetWaitPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitPos));
}

void CGvisR2R_PunchDoc::SetWaitVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitVel));
}

void CGvisR2R_PunchDoc::SetWaitAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitAcc));
}

void CGvisR2R_PunchDoc::SetMarkingPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingPos));
}

void CGvisR2R_PunchDoc::SetMarkingVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingVel));
}

void CGvisR2R_PunchDoc::SetMarkingAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingAcc));
}

void CGvisR2R_PunchDoc::SetMarkingToq2(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[1].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMarkingToq2Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[1].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingToq2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingToq));
}

void CGvisR2R_PunchDoc::SetPosX2_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[0]));
}

void CGvisR2R_PunchDoc::SetPosY2_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[0]));
}

void CGvisR2R_PunchDoc::SetPosX2_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[1]));
}

void CGvisR2R_PunchDoc::SetPosY2_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[1]));
}

void CGvisR2R_PunchDoc::SetPosX2_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisR2R_PunchDoc::SetPosY2_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_3()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[2]));
}

void CGvisR2R_PunchDoc::SetPosX2_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[3]));
}

void CGvisR2R_PunchDoc::SetPosY2_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[3]));
}

void CGvisR2R_PunchDoc::SetAverDist2(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[1].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetAverDist2()
{
	return (_tstof(WorkingInfo.Marking[1].sAverDist));
}

void CGvisR2R_PunchDoc::SetTotalReelDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sTotalReelDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), sData, sPath);


	pDoc->WorkingInfo.LastJob.sReelTotLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dTotLen = _tstof(sData) * 1000.0;
	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	long lData = (long)(dDist * 1000.0);
	pView->m_pMpe->Write(_T("ML45000"), lData);	// 전체 Reel 길이 (단위 M * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetTotalReelDist()
{
	return (_tstof(WorkingInfo.Lot.sTotalReelDist));
}

void CGvisR2R_PunchDoc::SetSeparateDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sSeparateDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotSepLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	long lData = (long)(dDist * 1000.0);
	pView->m_pMpe->Write(_T("ML45002"), lData);	// Lot 분리 길이 (단위 M * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetSeparateDist()
{
	return (_tstof(WorkingInfo.Lot.sSeparateDist));
}

void CGvisR2R_PunchDoc::SetCuttingDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sCuttingDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotCutPosLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	//pView->IoWrite("ML45004", long(_tstof(sVal)*1000.0));	// Lot 분리 후 절단위치 (단위 M * 1000)
	// 	pView->m_pMpe->Write(_T("ML45004", long(_tstof(sVal)*1000.0));

	long lData = (long)(dDist * 1000.0);
	pView->m_pMpe->Write(_T("ML45004"), lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetCuttingDist()
{
	return (_tstof(WorkingInfo.Lot.sCuttingDist));
}

void CGvisR2R_PunchDoc::SetStopDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sStopDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), sData, sPath);


	pDoc->WorkingInfo.LastJob.sTempPauseLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dTempPauseLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	//pView->IoWrite("ML45006", long(_tstof(sVal)*1000.0));	// 일시정지 길이 (단위 M * 1000)
	// 	pView->m_pMpe->Write(_T("ML45006"), long(_tstof(sVal)*1000.0));

	long lData = (long)(dDist * 1000.0);
	pView->m_pMpe->Write(_T("ML45006"), lData);	// 일시정지 길이 (단위 M * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetStopDist()
{
	return (_tstof(WorkingInfo.Lot.sStopDist));
}

void CGvisR2R_PunchDoc::SetAOIToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sAoiTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dToq * 1000.0);
	pView->m_pMpe->Write(_T("ML45042"), lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetAOIToq()
{
	return (_tstof(WorkingInfo.Motion.sAoiTq));
}

void CGvisR2R_PunchDoc::SetMarkingToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sMkTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dToq * 1000.0);
	pView->m_pMpe->Write(_T("ML45044"), lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetMarkingToq()
{
	return (_tstof(WorkingInfo.Motion.sMkTq));
}

void CGvisR2R_PunchDoc::SetEngraveToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sEngraveTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dToq * 1000.0);
	pView->m_pMpe->Write(_T("ML45050"), lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetEngraveToq()
{
	return (_tstof(WorkingInfo.Motion.sEngraveTq));
}

int CGvisR2R_PunchDoc::CopyPcrAll()  // return : Serial
{
	int nS0, nS1;
	if (pView->m_bLoadShare[0])
	{
		nS0 = CopyPcrUp();
		// 		if(pView->m_pDlgFrameHigh)
		// 			pView->m_pDlgFrameHigh->SetAoiLastShot(0, nS0);
	}
	if (pView->m_bLoadShare[1])
	{
		nS1 = CopyPcrDn();
		// 		if(pView->m_pDlgFrameHigh)
		// 			pView->m_pDlgFrameHigh->SetAoiLastShot(1, nS1);
	}

	if (pView->m_bLoadShare[0] && pView->m_bLoadShare[1])
	{
		if (nS0 > 0 && nS1 > 0)
			return TRUE;
	}
	else if (pView->m_bLoadShare[0] && !pView->m_bLoadShare[1])
	{
		if (nS0 > 0)
			return TRUE;
	}
	else if (!pView->m_bLoadShare[0] && pView->m_bLoadShare[1])
	{
		if (nS1 > 0)
			return TRUE;
	}

	return FALSE;
}

int CGvisR2R_PunchDoc::CopyPcrUp()  // return : Serial
{
	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareUp;
	CString sDest = WorkingInfo.System.sPathVrsBufUp;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

int CGvisR2R_PunchDoc::CopyPcrDn()  // return : Serial
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareDn;
	CString sDest = WorkingInfo.System.sPathVrsBufDn;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

void CGvisR2R_PunchDoc::DelSharePcr()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (pView->m_bLoadShare[0])
			DelSharePcrUp();
		if (pView->m_bLoadShare[1])
			DelSharePcrDn();
	}
	else
	{
		if (pView->m_bLoadShare[0])
			DelSharePcrUp();
		if (pView->IsShareDn())
			DelSharePcrDn();
	}
}
void CGvisR2R_PunchDoc::DelSharePcrUp()
{
	CString sPath = WorkingInfo.System.sPathVrsShareUp;

	if (m_pFile)
		m_pFile->DelPcrAll(sPath);
}

void CGvisR2R_PunchDoc::DelSharePcrDn()
{
	CString sPath = WorkingInfo.System.sPathVrsShareDn;

	if (m_pFile)
		m_pFile->DelPcrAll(sPath);
}

void CGvisR2R_PunchDoc::DelPcrAll()
{
	DelPcrUp();
	DelPcrDn();
}

void CGvisR2R_PunchDoc::DelPcrUp()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufUp;
		m_pFile->DelPcrAll(sPath);
	}
}

void CGvisR2R_PunchDoc::DelPcrDn()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufDn;
		m_pFile->DelPcrAll(sPath);
	}
}

void CGvisR2R_PunchDoc::SaveLotTime(DWORD dwStTick)
{
	CString sData, sPath = PATH_WORKING_INFO;

	// [Lot]
	WorkingInfo.Lot.dwStTick = dwStTick;
	sData.Format(_T("%d"), dwStTick);
	::WritePrivateProfileString(_T("Lot"), _T("Start Tick"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("Start Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("Start Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("Start Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("Start Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("Start Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("Start Second"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("End Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("End Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("End Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("End Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("End Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("End Second"), sData, sPath);
}

BOOL CGvisR2R_PunchDoc::Shift2Mk(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.23"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfo;
	CString sSrc, sDest;

	BOOL bRst[4] = { 0 };
	if (m_pFile)
	{
		sSrc.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
		if (!GetPcrInfo(sSrc, stInfo))
		{
			pView->DispStsBar(_T("E(2)"), 5);
			AfxMessageBox(_T("Error-GetPcrInfo(2)"));
			return FALSE;
		}

		if (!MakeMkDir(stInfo))
		{
			AfxMessageBox(_T("Error-MakeMkDir()"));
			return FALSE;
		}
		sDest.Format(_T("%s%s\\%s\\%s\\%04d.pcr"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
			stInfo.sLot, stInfo.sLayer, nSerial);

		bRst[0] = m_pFile->Copy(sSrc, sDest);			// 마킹한 파일을 복사한다.
		bRst[1] = m_pFile->DeleteFolerOrFile(sSrc);		// 마킹한 파일을 지운다.


		if (bDualTest)
		{
			sSrc.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
			if (!GetPcrInfo(sSrc, stInfo))
			{
				pView->DispStsBar(_T("E(3)"), 5);
				AfxMessageBox(_T("Error-GetPcrInfo(3)"));
				return FALSE;
			}

			if (!MakeMkDir(stInfo))
			{
				AfxMessageBox(_T("Error-MakeMkDir()"));
				return FALSE;
			}
			sDest.Format(_T("%s%s\\%s\\%s\\%04d.pcr"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
				stInfo.sLot, stInfo.sLayer, nSerial);
			bRst[2] = m_pFile->Copy(sSrc, sDest);			// 마킹한 파일을 복사한다.
			bRst[3] = m_pFile->DeleteFolerOrFile(sSrc);		// 마킹한 파일을 지운다.
		}
		else
		{
			bRst[2] = TRUE;
			bRst[3] = TRUE;
		}
	}
	else
		return FALSE;

	if (bRst[0] && bRst[1] && bRst[2] && bRst[3])
	{
		SetLastSerial(nSerial);
		return TRUE;
	}

	return FALSE;
}

void CGvisR2R_PunchDoc::SetLastSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.24"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		WorkingInfo.LastJob.sSerialUp = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Last SerialUp"), str, sPath);
		WorkingInfo.LastJob.sSerialDn = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Last SerialDn"), str, sPath);

		// 		if(m_pReelMap)
		// 			m_pReelMap->SetLastSerial(nSerial);
		if (m_pReelMapUp)
			m_pReelMapUp->SetLastSerial(nSerial);
		if (bDualTest)
		{
			if (m_pReelMapDn)
				m_pReelMapDn->SetLastSerial(nSerial);
			if (m_pReelMapAllUp)
				m_pReelMapAllUp->SetLastSerial(nSerial);
			if (m_pReelMapAllDn)
				m_pReelMapAllDn->SetLastSerial(nSerial);
		}
	}
}

void CGvisR2R_PunchDoc::UpdateYield(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.66"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->UpdateYield(nSerial);
	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->UpdateYield(nSerial);
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->UpdateYield(nSerial);
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->UpdateYield(nSerial);
	}
}

void CGvisR2R_PunchDoc::SetCompletedSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		AfxMessageBox(_T("Serial Error.49"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		WorkingInfo.LastJob.sCompletedSerialUp = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), str, sPath);
		WorkingInfo.LastJob.sCompletedSerialDn = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), str, sPath);

		// 		if(m_pReelMap)
		// 			m_pReelMap->SetCompletedSerial(nSerial);
		if (m_pReelMapUp)
			m_pReelMapUp->SetCompletedSerial(nSerial);
		if (bDualTest)
		{
			if (m_pReelMapDn)
				m_pReelMapDn->SetCompletedSerial(nSerial);
			if (m_pReelMapAllUp)
				m_pReelMapAllUp->SetCompletedSerial(nSerial);
			if (m_pReelMapAllDn)
				m_pReelMapAllDn->SetCompletedSerial(nSerial);
		}
	}
}

void CGvisR2R_PunchDoc::SetModelInfoUp()
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData = WorkingInfo.LastJob.sModelUp;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LotUp No"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetModelInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	CString sData, sPath = PATH_WORKING_INFO;
	sData = WorkingInfo.LastJob.sModelDn;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelDn Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LotDn No"), sData, sPath);
}

BOOL CGvisR2R_PunchDoc::MakeMkDir(CString sModel, CString sLot, CString sLayer)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (sModel.IsEmpty() || sLot.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel,
		sLot);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel,
		sLot,
		sLayer);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::MakeMkDir()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL b0, b1;

	if (bDualTest)
	{
		b0 = MakeMkDirUp();
		b1 = MakeMkDirDn();
		if (!b0 || !b1)
			return FALSE;
	}
	else
	{
		b0 = MakeMkDirUp();
		if (!b0)
			return FALSE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::MakeMkDirUp()
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (WorkingInfo.LastJob.sModelUp.IsEmpty() || WorkingInfo.LastJob.sLotUp.IsEmpty() || WorkingInfo.LastJob.sLayerUp.IsEmpty())
	{
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelUp);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelUp,
		WorkingInfo.LastJob.sLotUp);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelUp,
		WorkingInfo.LastJob.sLotUp,
		WorkingInfo.LastJob.sLayerUp);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::MakeMkDirDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (WorkingInfo.LastJob.sModelDn.IsEmpty() || WorkingInfo.LastJob.sLotDn.IsEmpty() || WorkingInfo.LastJob.sLayerDn.IsEmpty())
	{
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelDn);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelDn,
		WorkingInfo.LastJob.sLotDn);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		WorkingInfo.LastJob.sModelDn,
		WorkingInfo.LastJob.sLotDn,
		WorkingInfo.LastJob.sLayerDn);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::ChkLotEnd(CString sPath)
{
	if (!m_pFile)
		return FALSE;

	return (m_pFile->ChkLotEnd(sPath));
}

BOOL CGvisR2R_PunchDoc::IsPinMkData()
{
	if (m_pSpecLocal)
		return m_pSpecLocal->IsPinMkData();

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::IsPinData()
{
	if (m_pSpecLocal)
		return m_pSpecLocal->IsPinData();

	return FALSE;
}
CString CGvisR2R_PunchDoc::GetProcessNum()
{
	CString sPath = WorkingInfo.System.sPathAoiUpCurrInfo;
	TCHAR szData[200];
	CString sCode = _T("");

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Process Unit Code"), NULL, szData, sizeof(szData), sPath))
		sCode = CString(szData);

	return sCode;
}

BOOL CGvisR2R_PunchDoc::GetAoiUpOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiUpOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = _tstof(szData);
	else
	{
		OfSt.x = 0.0;
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
		OfSt.y = _tstof(szData);
	else
	{
		OfSt.y = 0.0;
		bRtn = FALSE;
	}

	return bRtn;
}

BOOL CGvisR2R_PunchDoc::GetAoiDnOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiDnOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = -1.0*_tstof(szData);
	else
	{
		OfSt.x = 0.0;
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
		OfSt.y = _tstof(szData);
	else
	{
		OfSt.y = 0.0;
		bRtn = FALSE;
	}

	return bRtn;
}

CString CGvisR2R_PunchDoc::GetMin(int nDlgId, int nCtrlId)
{
	if (nDlgId == IDD_DLG_INFO)
	{
		switch (nCtrlId)
		{
		case IDC_STC_174:
			return _T("20");
			//return _T("0");
		case IDC_STC_61:
			return _T("0");
		}
	}

	if (nDlgId == IDD_DLG_MENU_04)
	{
		switch (nCtrlId)
		{
		case IDC_STC_010:
			return _T("30");
		case IDC_STC_007:
			return _T("30");
		case IDC_STC_25:
			return _T("30");
		case IDC_STC_148:
			return _T("-12");
		case IDC_STC_152:
			return _T("-12");
		case IDC_STC_48:
			return _T("-5");
		case IDC_STC_93:
			return _T("-5");
		case IDC_STC_58:
			return _T("-5");
		case IDC_STC_113:
			return _T("-5");
		case IDC_STC_52:
			return _T("0");
		case IDC_STC_62:
			return _T("0");
		case IDC_STC_117:
			return _T("0");
		case IDC_STC_55:
			return _T("0");
		case IDC_STC_110:
			return _T("0");
		case IDC_STC_65:
			return _T("0");
		case IDC_STC_120:
			return _T("0");
		case IDC_STC_68:
			return _T("0");
		case IDC_STC_123:
			return _T("0");
		case IDC_STC_97:
			return _T("0");
		}
	}

#ifdef USE_FLUCK
	if (nDlgId == IDD_DLG_UTIL_06)
	{
		switch (nCtrlId)
		{
		case IDC_STC_132:
			return _T("-5");
		case IDC_STC_136:
			return _T("0");
		case IDC_STC_139:
			return _T("0");

		case IDC_STC_143:
			return _T("-5");
		case IDC_STC_147:
			return _T("0");
		case IDC_STC_150:
			return _T("0");
		case IDC_STC_153:
			return _T("0");


		case IDC_STC_93:
			return _T("-5");
		case IDC_STC_97:
			return _T("0");
		case IDC_STC_110:
			return _T("0");

		case IDC_STC_113:
			return _T("-5");
		case IDC_STC_117:
			return _T("0");
		case IDC_STC_120:
			return _T("0");
		case IDC_STC_123:
			return _T("0");
		}
	}
#endif

	return _T("");
}

CString CGvisR2R_PunchDoc::GetMax(int nDlgId, int nCtrlId)
{
	CString sVal;

	if (nDlgId == IDD_DLG_INFO)
	{
		switch (nCtrlId)
		{
		case IDC_STC_174:
			return _T("100");
		case IDC_STC_61:
			sVal.Format(_T("%d"), FIX_PCS_SHOT_MAX);
			return sVal;
		}
	}

	if (nDlgId == IDD_DLG_MENU_04)
	{
		switch (nCtrlId)
		{
		case IDC_STC_010:
			return _T("100000000");
		case IDC_STC_007:
			return _T("1000");
		case IDC_STC_25:
			return _T("300");
		case IDC_STC_148:
			return _T("12");
		case IDC_STC_152:
			return _T("12");
		case IDC_STC_48:
			return _T("10");
		case IDC_STC_93:
			return _T("10");
		case IDC_STC_58:
			return _T("15");
			//return _T("10";
		case IDC_STC_113:
			return _T("15");
			//return _T("10";
		case IDC_STC_52:
			return _T("2000");
		case IDC_STC_97:
			return _T("2000");
		case IDC_STC_62:
			return _T("2000");
		case IDC_STC_117:
			return _T("2000");
		case IDC_STC_55:
			return _T("20000");
		case IDC_STC_110:
			return _T("20000");
		case IDC_STC_65:
			return _T("20000");
		case IDC_STC_120:
			return _T("20000");
		case IDC_STC_68:
			return _T("1900");
		case IDC_STC_123:
			return _T("1900");

		}
	}
#ifdef USE_FLUCK
	if (nDlgId == IDD_DLG_UTIL_06)
	{
		switch (nCtrlId)
		{
		case IDC_STC_132:
			return _T("10");
		case IDC_STC_136:
			return _T("2000");
		case IDC_STC_139:
			return _T("20000");

		case IDC_STC_143:
			return _T("10");
		case IDC_STC_147:
			return _T("2000");
		case IDC_STC_150:
			return _T("20000");
		case IDC_STC_153:
			return _T("1900");


		case IDC_STC_93:
			return _T("10");
		case IDC_STC_97:
			return _T("2000");
		case IDC_STC_110:
			return _T("20000");

		case IDC_STC_113:
			return _T("10");
		case IDC_STC_117:
			return _T("2000");
		case IDC_STC_120:
			return _T("20000");
		case IDC_STC_123:
			return _T("1900");
		}
	}
#endif

	return _T("");
}

int CGvisR2R_PunchDoc::Mirroring(int nPcsId)
{
#ifdef TEST_MODE
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_Master[0].m_pPcsRgn->nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->nRow;

	nCol = int(nPcsId / nNodeY);
	if (nCol % 2)
		nRow = nNodeY*(nCol + 1) - nPcsId - 1;
	else
		nRow = nPcsId - nNodeY*nCol;

	nR = nRow;
	nC = (nNodeX - 1) - nCol;
	if (nC % 2)
		nId = nNodeY*(nC + 1) - nR - 1;
	else
		nId = nR + nNodeY*nC;

	return nId;
}

BOOL CGvisR2R_PunchDoc::MakeMkDir(stModelInfo stInfo)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (stInfo.sModel.IsEmpty() || stInfo.sLot.IsEmpty() || stInfo.sLayer.IsEmpty())
	{
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot,
		stInfo.sLayer);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot,
		stInfo.sLayer);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::GetPcrInfo(CString sPath, stModelInfo &stInfo)
{

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, strTotalBadPieceNum;


	//strcpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(_T(','), 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//m_pPcr[nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(_T(','), 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(_T(','), 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(_T('\n'), 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sLot = strLot;

	nTemp = strFileData.Find(_T('\n'), 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	stInfo.sModel = strModel;
	stInfo.sLayer = strLayer;
	stInfo.sLot = strLot;

	return TRUE;
}

void CGvisR2R_PunchDoc::UpdateProcessNum(CString sProcessNum)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pDoc->m_pReelMapUp->UpdateProcessNum(sProcessNum, 0); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	if (bDualTest)
	{
		pDoc->m_pReelMapDn->UpdateProcessNum(sProcessNum, 1); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllUp->UpdateProcessNum(sProcessNum, 2); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllDn->UpdateProcessNum(sProcessNum, 3); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
	pDoc->m_pReelMap->UpdateProcessNum(sProcessNum, pView->m_nSelRmap); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
}


void CGvisR2R_PunchDoc::SetProbWaitPos(int nProb, double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[0].sWaitPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitPos(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sWaitPos));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sWaitPos));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbWaitVel(int nProb, double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitVel = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_VEL"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sWaitVel = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_VEL"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitVel(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Marking[0].sWaitVel));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Marking[1].sWaitVel));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbWaitAcc(int nProb, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitAcc = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_ACC"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sWaitAcc = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_ACC"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitAcc(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sWaitAcc));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sWaitAcc));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingPos(int nProb, double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_POS"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingPos = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_POS"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingPos(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingPos));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingPos));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingVel(int nProb, double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingVel = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_VEL"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingVel = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_VEL"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingVel(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingVel));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingVel));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingAcc(int nProb, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingAcc = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_ACC"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingAcc = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_ACC"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingAcc(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingAcc));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingAcc));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingToq(int nProb, double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingToq = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[0].sProbingToq = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingToq(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingToq));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingToq));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbPosX(int nProb, double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sMeasurePosX = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sMeasurePosX = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), sData, sPath);
	}
}

void CGvisR2R_PunchDoc::SetProbPosY(int nProb, double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sMeasurePosY = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sMeasurePosY = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbPosX(int nProb)
{
	double dPos;
	if (nProb == 0)
		dPos = (_tstof(WorkingInfo.Probing[0].sMeasurePosX));
	else if (nProb == 1)
		dPos = (_tstof(WorkingInfo.Probing[1].sMeasurePosX));
	return dPos;
}

double CGvisR2R_PunchDoc::GetProbPosY(int nProb)
{
	double dPos;
	if (nProb == 0)
		dPos = (_tstof(WorkingInfo.Probing[0].sMeasurePosY));
	else if (nProb == 1)
		dPos = (_tstof(WorkingInfo.Probing[1].sMeasurePosY));
	return dPos;
}

void CGvisR2R_PunchDoc::SetProbingThreshold(double dThreshold)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dThreshold);
	WorkingInfo.Fluck.sThreshold = sData;
	::WritePrivateProfileString(_T("Fluck"), _T("PROBING_THRESHOLD"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetProbingThreshold()
{
	return (_tstof(WorkingInfo.Fluck.sThreshold));
}

void CGvisR2R_PunchDoc::SetStripRejectMkNum(int nNum)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Fluck.sRejectMkNum = sData;
	::WritePrivateProfileString(_T("Fluck"), _T("PROBING_REJECT_MK"), sData, sPath);
}

int CGvisR2R_PunchDoc::GetStripRejectMkNum()
{
	return (_tstoi(WorkingInfo.Fluck.sRejectMkNum));
}

int CGvisR2R_PunchDoc::GetLastShotEngrave()
{
	int nLastShot = 0;
	//if (pView->m_pDlgFrameHigh)
	//	nLastShot = pView->m_pDlgFrameHigh->m_nEngraveLastShot;

	return (nLastShot);
}

void CGvisR2R_PunchDoc::WriteElecData(CString sData)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString strDestPath, sLot, sPnl;
	int nSerial;

	if (bDualTest)
	{
		sLot = WorkingInfo.LastJob.sLotDn;
		if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			strDestPath.Format(_T("%s\\%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModelDn,
				sLot, sLot);
		else
			strDestPath.Format(_T("%s%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModelDn,
				sLot, sLot);
		nSerial = pView->m_nBufDnSerial[0] + 1;
	}
	else
	{
		sLot = WorkingInfo.LastJob.sLotUp;
		if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			strDestPath.Format(_T("%s\\%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModelUp,
				sLot, sLot);
		else
			strDestPath.Format(_T("%s%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModelUp,
				sLot, sLot);
		nSerial = pView->m_nBufUpSerial[0] + 1;
	}

	sPnl.Format(_T("%04d"), nSerial);
	::WritePrivateProfileString( sLot, sPnl, sData, strDestPath );
}

double CGvisR2R_PunchDoc::GetEngraveAoiDist()
{
	return (_tstof(WorkingInfo.Motion.sFdEngraveAoiInitDist));
}

void CGvisR2R_PunchDoc::SetEngraveAoiDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdEngraveAoiInitDist = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45018"), lData);	// 각인부에서 AOI(상)까지 거리 (단위 mm * 1000)
#endif
}

void CGvisR2R_PunchDoc::SetEngraveReaderDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sEngraveFdBarcodeOffset = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45090"), lData);	// 각인위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetEngraveReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sEngraveFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::SetAoiReaderDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sAoiFdBarcodeOffset = sData;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45092"), lData);	// AOI 검사위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetAoiReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sAoiFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::SetMkReaderDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sMkFdBarcodeOffset = sData;
	::WritePrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->m_pMpe->Write(_T("ML45094"), lData);	// Punching 시작위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetMkReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sMkFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::Set2DReaderPosMoveVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sFdBarcodeOffsetVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dVel * 1000.0);
	pView->m_pMpe->Write(_T("ML45060"), lData);	// 2D 바코드 리더기위치까지 Feeding 속도 (단위 mm/sec * 1000)
#endif
}

void CGvisR2R_PunchDoc::Set2DReaderPosMoveAcc(double dAcc)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sFdBarcodeOffsetAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dAcc * 1000.0);
	pView->m_pMpe->Write(_T("ML45062"), lData);	// 2D 바코드 리더기위치까지 Feeding 가속도 (단위 mm/sec * 1000)
#endif
}

double CGvisR2R_PunchDoc::Get2DReaderPosMoveVel()
{
	return (_tstof(WorkingInfo.Motion.sFdBarcodeOffsetVel));
}

double CGvisR2R_PunchDoc::Get2DReaderPosMoveAcc()
{
	return (_tstof(WorkingInfo.Motion.sFdBarcodeOffsetAcc));
}

void CGvisR2R_PunchDoc::SetEngraveFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sEngraveFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
#ifdef USE_MPE
	long lData = (long)(dPitch * 1000.0);
	pView->m_pMpe->Write(_T("ML45020"), lData);	// 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetEngraveFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sEngraveFdLead));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrLmt(double dLmt)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLmt);
	WorkingInfo.Motion.sEngraveLmtFdErr = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetEngraveFdErrLmt()
{
	return (_tstof(WorkingInfo.Motion.sEngraveLmtFdErr));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrRng(double dRng)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dRng);
	WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetEngraveFdErrRng()
{
	return (_tstof(WorkingInfo.Motion.sEngraveLmtFdAdjOffSet));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrNum(int nNum)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%d"), nNum);
	WorkingInfo.Motion.sEngraveLmtFdOvrNum = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), sVal, sPath);
}

int CGvisR2R_PunchDoc::GetEngraveFdErrNum()
{
	return (_tstoi(WorkingInfo.Motion.sEngraveLmtFdOvrNum));
}

void CGvisR2R_PunchDoc::SetEngraveBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sEngraveStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dPos * 1000.0);
	pView->m_pMpe->Write(_T("ML45022"), lData);	// 각인부 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
}

double CGvisR2R_PunchDoc::GetEngraveBuffInitPos()
{
	return (_tstof(WorkingInfo.Motion.sEngraveStBufPos));
}


BOOL CGvisR2R_PunchDoc::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
