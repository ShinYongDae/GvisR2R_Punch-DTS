#if !defined(AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_)
#define AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReelMap.h : header file
//

#define MAX_DEFINFO		12
// #define MAX_PCS			10000

#include "ThreadTask.h"
#include "../Global/GlobalDefine.h"

#define FIX_PCS_SHOT_MAX	500
#define FIX_PCS_COL_MAX		100
#define FIX_PCS_ROW_MAX		100

#pragma warning(disable: 4244)
#pragma warning(disable: 4996)
#pragma warning(disable: 4477)

/////////////////////////////////////////////////////////////////////////////
// CReelMap window

class CReelMap : public CWnd
{
	BOOL m_FixPcs[FIX_PCS_SHOT_MAX][FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_FixPcsPrev[FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_nPrevSerial[2]; // [0] : -- , [1] : ++
	int m_nPnlBuf;
	short ***m_pPnlBuf;	// DefCode 3D Array : [nSerial][nRow][nCol]
	int m_nTotPcs, m_nGoodPcs, m_nBadPcs, m_nDef[MAX_DEF];	// [DefCode] : Total Num.
	int m_nDefStrip[4], m_nDefPerStrip[4][MAX_DEF];
	int m_nStripOut[4], m_nTotStOut;
	CString m_sPathShare, m_sPathBuf;
	CString m_sPathYield;
	double m_dAdjRatio; // Master Image의 Pixel 해상도에 따른 Reelmap에서의 식별용 간격 비율.
	int m_nIdxDefInfo;	// MAX_DEFINFO에 들어가는 정보의 Index.
	int m_nWritedSerial; // In Share folder Serial.
	BOOL m_bContFixDef;

	//int m_nCntFixPcs;

	void LoadConfig();
	BOOL UpdateRst(int nSerial);
	BOOL MakeDir();
	BOOL MakeDir(CString sModel, CString sLayer, CString sLot);
	BOOL MakeDirUser();
	BOOL MakeDirUser(CString sModel, CString sLayer, CString sLot);
	CString MakeDirUserRestore();
	CString MakeDirUserRestore(CString sModel, CString sLayer, CString sLot);
	int GetLastUserRestoreDir(CString strPath);

	char* StrToChar(CString str);
	void StrToChar(CString str, char* pCh);
	
	BOOL ReadYield(int nSerial, CString sPath);
	BOOL WriteYield(int nSerial, CString sPath);
	void ResetYield();
	
	BOOL LoadDefectTableIni();
	BOOL LoadDefectTableDB();
// Construction
public:
	CReelMap(int nLayer, int nPnl=0, int nPcs=0, int nDir=0);

// Attributes
public:
	CCriticalSection m_cs;

	int m_nLayer;
	CString m_sMc, m_sUser;
// 	CString m_sModel, m_sLayer, m_sLot;
	int m_nSerial; // On marking Serial.

	int nTotPnl;
	int nTotPcs;
	int nDir;

	int *m_pPnlNum, *m_pPnlDefNum;
	CRect *pFrmRgn;
	CRect **pPcsRgn;

	int **pPcsDef;
	CString m_sKorDef[MAX_DEF], m_sEngDef[MAX_DEF];
	char m_cBigDef[MAX_DEF], m_cSmallDef[MAX_DEF];
	COLORREF m_rgbDef[MAX_DEF];
	int m_nOdr[MAX_DEF];
	int m_nBkColor[3]; //RGB

// 	CString *pMkInfo;

	int m_nSelMarkingPnl;
	double m_dTotLen, m_dPnlLen, m_dLotLen, m_dTempPauseLen, m_dLotCutPosLen;
	BOOL m_bUseLotSep, m_bUseTempPause;
	int m_nLastShot, m_nCompletedShot;
	double m_dProgressRatio;

	stYield m_stYield;

// Operations
public:
	void SetRgbDef(int nDef, COLORREF rgbVal);
	void SetPnlNum(int *pPnlNum=NULL);
	void IncPnlNum();
	void AddPnlNum(int nNum);
	void SelMarkingPnl(int nNum);
	void SetAdjRatio(double dRatio);
	double GetAdjRatio();

	CString GetLotSt();
	CString GetLotEd();

	BOOL Open(CString sPath);
	BOOL OpenUser(CString sPath);
	BOOL Open(CString sPath, CString sModel, CString sLayer, CString sLot);
	BOOL OpenUser(CString sPath, CString sModel, CString sLayer, CString sLot);
	int Read(CString &sRead);
	BOOL Write(int nSerial, int nLayer);
	BOOL Disp(int nMkPnl, BOOL bDumy=FALSE);
	void SetLastSerial(int nSerial);
	void SetCompletedSerial(int nSerial);
	void SetLotSt();
	void SetLotEd();
	BOOL InitRst();
	void CloseRst();
	void ClrRst();
// 	BOOL SetRst(int nSerial);
	BOOL GetRst(int nFrom, int nTo);
	BOOL GetRst(int nSerial);
	int GetDefNum(int nDefCode);
	int GetDefStrip(int nStrip);
	int GetDefStrip(int nStrip, int nDefCode);
	void GetPcsNum(int &nGood, int &nBad);
	void ClrPnlNum();
	void Clear();
	int GetLastSerial();
	void InitPcs();
	void ClrPcs();
//	void ResetReelmap();

	void ClrFixPcs();
	void ClrFixPcs(int nCol, int nRow);
	void SetFixPcs(int nSerial, int nCol, int nRow); // nShot : 0 ~ 
	BOOL IsFixPcs(int nSerial, int &Col, int &Row);
	BOOL IsFixPcs(int nSerial, int* pCol, int* pRow, int &nTot);

	BOOL Write(int nSerial, int nLayer, CString sPath);
	void SetPathAtBuf(CString sPath);
	CString GetRmapPath(int nRmap);
	CString GetYieldPath(int nRmap);

	void SetPnlDefNum(int *pPnlDefNum);
	void ClrPnlDefNum();
	void AddPnlDefNum(int nDef);
	void UpdateTotVel(CString sVel, int nLayer);
	void UpdateProcessNum(CString sProcessNum, int nLayer);
	int GetStripOut(int nStrip);
	void SetFixPcs(int nSerial);

	BOOL m_bThreadAliveRemakeReelmap, m_bRtnThreadRemakeReelmap;
	CString m_sPathOnThread;
	CThreadTask m_ThreadTaskRemakeReelmap; // CThreadTask class, handles the threading code
	BOOL RemakeReelmap();
	void RemakeReelmap(CString sPath);
	void StartThreadRemakeReelmap();
	static BOOL ThreadProcRemakeReelmap(LPVOID lpContext);
	void StopThreadRemakeReelmap();

	void RestoreReelmap();

	BOOL m_bThreadAliveReloadRst, m_bRtnThreadReloadRst, m_bDoneReloadRst;
	int m_nLastOnThread, m_nPregressReloadRst, m_nTotalPregressReloadRst;
	CThreadTask m_ThreadTaskReloadRst; // CThreadTask class, handles the threading code
	BOOL ReloadRst();
	BOOL ReloadRst(int nTo);
	BOOL IsDoneReloadRst();
	int GetPregressReloadRst();
	void StartThreadReloadRst();
	static BOOL ThreadProcReloadRst(LPVOID lpContext);
	void StopThreadReloadRst();

	BOOL UpdateYield(int nSerial);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReelMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReelMap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReelMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_)
