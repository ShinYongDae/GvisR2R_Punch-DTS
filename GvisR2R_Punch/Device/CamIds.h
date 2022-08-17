#if !defined(AFX_CAMIDS_H__6CDCE3E0_B875_4906_A0E6_6985E077735A__INCLUDED_)
#define AFX_CAMIDS_H__6CDCE3E0_B875_4906_A0E6_6985E077735A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamIds.h : header file
//

#ifdef USE_IDS

#include "ueye.h"
#include "ueye_deprecated.h"

#ifdef _WIN64
#pragma comment (lib, "uEye_api_64.lib")
#else
#pragma comment (lib, "uEye_api.lib")
#endif

// some definitions
#define MAX_EV      32
#define SEQ_BUFFERS 5

#define IS_UEYE_MESSAGE0 (WM_USER+100)
#define IS_UEYE_MESSAGE1 (WM_USER+101)

/////////////////////////////////////////////////////////////////////////////
// CCamIds window

class CCamIds : public CWnd
{
	CWnd *m_pParent;
	int m_nIdxCam, m_nIdxSeq;

	// uEye varibles
	HIDS	m_hCam;				// handle to camera
	HWND	m_hWndDisplay;		// handle to diplay window
	INT		m_nColorMode;		// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;	// number of bits needed store one pixel
	INT		m_nSizeX;			// width of image
	INT		m_nSizeY;			// height of image
	INT		m_nPosX;			// left offset of image
	INT		m_nPosY;			// right offset of image

	// memory needed for live display while using DIB
	INT		m_lMemoryId;		// camera memory - buffer ID
	TCHAR*	m_pcImageMemory;	// camera memory - pointer to buffer
	SENSORINFO m_sInfo;			// sensor information struct
	INT     m_nRenderMode;		// render  mode
	INT     m_nFlipHor;			// horizontal flip flag
	INT     m_nFlipVert;		// vertical flip flag

	// Memory and sequence buffers, no host memory needed for live display while using Direct3D
	INT		m_lSeqMemId[SEQ_BUFFERS];		// camera memory - buffer ID
	TCHAR*	m_pcSeqImgMem[SEQ_BUFFERS];		// camera memory - pointer to buffer

	INT InitCamera (HIDS *hCam, HWND hWnd);
	BOOL OpenCamera();
	BOOL ExitCamera();
	int  InitDisplayMode();
	void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
	BOOL SeqBuilt();
	BOOL SeqKill();

	BOOL RenderBitmap();

// Construction
public:
	CCamIds(int nIdx, HWND hCtrl, CWnd* pParent=NULL);

// Attributes
public:

// Operations
public:	
	LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);
	void Acquire();
	BOOL LoadParameters();
	int StartLive();
	void StopLive();

	TCHAR* GetImg();
	int GetSeqImgMemIdx();
	int GetBitsPerPixel();
	double GetFramRate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamIds)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCamIds();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCamIds)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMIDS_H__6CDCE3E0_B875_4906_A0E6_6985E077735A__INCLUDED_)
