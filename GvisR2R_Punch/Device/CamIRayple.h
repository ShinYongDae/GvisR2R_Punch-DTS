#pragma once
#include "afxwin.h"

#include "Render.h"
#include "Tool.h"
#include <list>

class CCamIRayple : public CWnd
{
	IMV_HANDLE	m_devHandle;
	HANDLE		m_displayThreadHandle;

	std::list<FrameBuffer*>	m_listImages;

	CTool		m_mxTime;
	CTool		m_mutexQue;
	CRender     m_Render;

	CWnd		*m_pParent;
	int			m_nIdxCam;
	HWND		m_hWndDisplay;		// handle to diplay window
	CRect		m_rcDisplay;

	int			m_nWidth;
	int			m_nHeight;
	int			m_dDisplayInterval;
	uint64_t	m_nFirstFrameTime;
	uint64_t	m_nLastFrameTime;
	BOOL		m_bConnected;
	BOOL		m_bResumeGrabbing;
	BOOL		m_bRunning;
	CString		m_sCameraKey;
	CString		m_sFormat;
	CString		m_sTriggerMode;
	double		m_dFrameRateEdit;
	double		m_dExposureEdit;
	double		m_dGainEdit;
	long		m_lCamWidth;
	long		m_lCamHeight;


	FrameBuffer* getConvertedImage();
	void addConvertedImage(FrameBuffer* &pConvertedImage);
	void clearConvertedImage();

	void StringToChar(CString str, char *szStr);
	CString CharToString(char *szStr);
	TCHAR* CharToTCHAR(char *str);
	char* TCHARToChar(TCHAR *tszStr);
	char* TCHARToChar(const TCHAR *tszStr);

	BOOL isTimeToDisplay();
	void setDisplayFPS(int nFPS);
	void initParamProperty();
	void initPixelFormatParamProperty();
	void initTriggerModeParamProperty();
	void Getparam();
	void SetParamProperty();
	BOOL StartStreamGrabbing(bool bResumeConnect);
	BOOL StopStreamGrabbing(bool bResumeConnect);

public:
	CCamIRayple(int nIdx, HWND hCtrl, CWnd* pParent = NULL);
	~CCamIRayple();

	void frameProc(IMV_Frame frame);
	void deviceLinkNotifyProc(IMV_SConnectArg connectArg);
	void displayProc();


	void Connect(BOOL bConnect);
	void StartLive();
	void StopLive();
	int GetImgWidth();
	int GetImgHeight();
	void SetTriggermode(CString sTriggerMode);

	BOOL OneshotGrab();
};

