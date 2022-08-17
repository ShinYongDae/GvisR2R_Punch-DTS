#pragma once


#ifdef USE_CREVIS

#ifndef _INCLUDE_IMAGE_H
#define _INCLUDE_IMAGE_H
// TLI include
#include <TLI/TLI.h>
#include <RegMapRef.h>
// Image include
#include <Image.h>

#pragma comment (lib, "TLI.lib")
#pragma comment (lib, "ImageDll.lib")
#endif


#define WM_MESSAGE_GRAB_THREAD				WM_APP+100
#define	ON_START_GRAB_THREAD				0
#define	ON_ERROR_GRAB_THREAD				1
#define	ON_STOP_GRAB_THREAD					2


#define TIMER_FRAMERATE			1
#define TIMER_FRAM_INFO			2
#define COUNT_ALLOC_BUFF		5


// Processing
#define SHARPENESS_DISABLE			0		
#define SHARPENESS_LAPLACIAN		1	// Laplacian mask
#define	SHARPENESS_LOG				2	// Laplacian of a Gaussian mask


#define LIMIT(val, limit)	(val<limit) ? ((val<0) ? 0 : val) : limit


#include "ThreadTask.h"

// CCamCrevis

class CCamCrevis : public CWnd
{
	DECLARE_DYNAMIC(CCamCrevis)

	CWnd *m_pParent;

	int		m_nIdxCam;
	HWND	m_hWndDisplay;		// handle to diplay window
	CRect	m_rcDisplay;

	BOOL m_bThreadAlive, m_bRtnThread;
	CThreadTask m_ThreadTask; // CThreadTask class, handles the threading code
	CRITICAL_SECTION m_csImg;

	CStringList	m_DeviceNameList;


	// TL
	DEV_HANDLE	m_hDevice;
	PORT_HANDLE	m_hDevicePort;
	DS_HANDLE	m_hDatastream;
	HANDLE		m_hRegister;

	// Grab
	//	HANDLE		m_hGrabThread;
	HANDLE		m_eTerminate;
	UINT		m_AcqCount;
	BOOL		m_bIsGrabbing;

	// View
	CImage*		m_pImage;
	CPoint		m_scrollPosition;

	// ImageInfo
	CString		m_strDispName;
	int			m_GrayLevel;
	int			m_Width;
	int			m_Height;
	CString		m_strDataType;
	CDialog*	m_pCtrlDlg;	

	//Frame info
	double		m_currentFrameRate;     
	double		m_FrameCounter;	
	double		m_OldFrameCounter;
	double		m_oldTime;
	UINT		m_LostImageCounter;

	// Processing
	BOOL		m_SharpnessType;
	void*		m_pCpyImg;
	int			m_LaplacianFactor;
	int			m_LoGFactor;
	

	void Init();

	void GrabStart();
	void GrabStop();

	void StartThread();
	static BOOL ThreadProc(LPVOID lpContext);
	void StopThread();
	DS_HANDLE GetDataStream(DEV_HANDLE hDev);

	void ReceiveImage(void* pSrcImg);
	void ImageProc(void* pSrcImg);
	void DrawImage();
	BOOL CreateImage();
	void DeleteImage();

	DEV_HANDLE SelectDevice(IF_HANDLE hInterface);
	PORT_HANDLE GetDevicePort(DEV_HANDLE hDev);
	HANDLE GetRegMap(PORT_HANDLE hDevPort);

	void ProcSharpening(void* pSrcImg);
	void HighBoostFiltering_LaplacianMono8(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor);
	void HighBoostFiltering_LoGMono8(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor);
	void HighBoostFiltering_LaplacianMono10(WORD* pSrc, WORD* pDest, int Width, int Height, int Factor);
	void HighBoostFiltering_LoGMono10(WORD* pSrc, WORD* pDest, int Width, int Height, int Factor);
	void HighBoostFiltering_LaplacianRGB8Packed(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor);
	void HighBoostFiltering_LoGRGB8Packed(BYTE* pSrc, BYTE* pDest, int Width, int Height, int Factor);

public:
	CCamCrevis(int nIdx, HWND hCtrl, CWnd* pParent =NULL);
	virtual ~CCamCrevis();

	CCriticalSection m_cs;


	BOOL OpenDevice();
	void CloseDevice();

	void StartLive();
	void StopLive();

	DWORD GrabThreadProc(void *pClassInst);
	afx_msg LRESULT OnMessageGrabThread(WPARAM wParam, LPARAM lParam);

	CString GetLasetError();

	int GetImgWidth();
	int GetImgHeight();
	BOOL GetFramesPerSecond(double* dFPS);


protected:
	DECLARE_MESSAGE_MAP()
};

#endif


