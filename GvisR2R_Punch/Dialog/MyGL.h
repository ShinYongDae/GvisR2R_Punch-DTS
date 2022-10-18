#if !defined(AFX_MYGL_H__6C5A671A_7936_4FAF_B053_954B4AB21980__INCLUDED_)
#define AFX_MYGL_H__6C5A671A_7936_4FAF_B053_954B4AB21980__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyGL.h : header file
//

#include "../Global/GlobalDefine.h"
#include "../Process/GVGL.h"
#include "../Global/MyData.h"
#include "../Process/ReelMap.h"

#define MYGL_SIDE_MARGIN	10
// #define MYGL_SIDE_MARGIN	22
#define MYGL_GAP_NUM		3
#define MYGL_SIZE_NUM		3
#define MYGL_SIZE_DEFNUM	3
// #define MYGL_SIZE_NUM		2
#define MYGL_SIZE_CHAR		(MYGL_SIZE_NUM*5.0)

/////////////////////////////////////////////////////////////////////////////
// CMyGL window

class CMyGL : public CWnd
{
	CWnd* m_pParent;
	CCriticalSection m_cs;

	CDC *m_hDC;
	HGLRC m_hRC;

	GVertex vtScrSt, vtScrEd;
	GVertex vtBkSt, vtBkEd;
	GVertex vtMkPnt;

	HWND m_hDispCtrl;
	CRect m_rtDispCtrl;
	int m_nCtrlId;

	int m_nTotPnl, m_nTotPcs;
	int m_nPrevTotPnl, m_nPrevTotPcs;
	int *m_pPnlNum, *m_pPnlDefNum;

	GVertex **m_pFrmRgn;
	GVertex ***m_pPcsPnt;	// [pnl][pcsIdx][LT], 	// [pnl][pcsIdx][RB]

	int m_nWorldW, m_nWorldH;
	GVColor m_rgbDef[MAX_DEF];
	GLfloat cameraposmap[3];
	GLfloat Angle[3];

	int m_nTexTureBuff;
	GLubyte *TexTureBuff[2];
	int m_nTxTrSzX, m_nTxTrSzY;

	GVColor m_rgbWhite, m_rgbBlack, m_rgbRed, m_rgbGreen, m_rgbBlue;
	GVColor m_rgbBk;

	void DrawBack();
	void DrawRgn();
	void DrawPnlNum();
	void DrawPnlDefNum();

	void DrawPcs();
	void DrawPin();


// Construction
public:
	CMyGL(CWnd* pParent=NULL);

// Attributes
public:
	CReelMap* m_pReelMap;

// Operations
public:
	void Init(int nCtrlId, CReelMap* pReelMap);
	void Draw();

	void SetRgn();
	void ResetRgn();
	void SetPcsDef();
	void SetPnlNum();
	void SetPnlDefNum();

	void LoadPcsImg(CString sPath);
	void GetPixelInfo(int &nSzCtrlX, int &nSzCtrlY, int &nSzImgX, int &nSzImgY);

	void SetMkPos(CfPoint ptPnt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGL)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyGL();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyGL)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGL_H__6C5A671A_7936_4FAF_B053_954B4AB21980__INCLUDED_)
