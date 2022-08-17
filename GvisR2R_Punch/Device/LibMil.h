#if !defined(AFX_LIBMIL_H__C0404020_1E38_4073_B1B6_D7F52DC66B42__INCLUDED_)
#define AFX_LIBMIL_H__C0404020_1E38_4073_B1B6_D7F52DC66B42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibMil.h : header file
//
#include <afxmt.h>

#ifndef _INCLUDE_MIL_H
#define _INCLUDE_MIL_H
#include <mil.h>
#pragma comment (lib, "mil.lib")
#pragma comment (lib, "milcolor.lib")
#pragma comment (lib, "Milim.lib")
#pragma comment (lib, "Miledge.lib")
#pragma comment (lib, "MilMeas.lib")
#pragma comment (lib, "Milmetrol.lib")
#pragma comment (lib, "milBlob.lib")
#pragma comment (lib, "milcal.lib")
#pragma comment (lib, "Milcode.lib")
#pragma comment (lib, "Milreg.lib")
#pragma comment (lib, "MilStr.lib")
#pragma comment (lib, "milpat.lib")
#pragma comment (lib, "milmod.lib")
#pragma comment (lib, "milocr.lib")
#endif


#include "LibMilBuf.h"
#include "LibMilDisp.h"
#include "LibMilDraw.h"

#pragma warning(disable: 4995)

#define DISPLAY_FIT_MODE_CENTERVIEW		0
#define DISPLAY_FIT_MODE_MIN_RATIO		1
#define DISPLAY_FIT_MODE_MAX_RATIO		2
#define DISPLAY_FIT_MODE_X_RATIO		3
#define DISPLAY_FIT_MODE_Y_RATIO		4
#define DISPLAY_FIT_MODE_XY_RATIO		5


#define PATTERN_MATCHING_RESULT_NUM_LIMIT	16L

// OneShotGrab Color Mode
#define GRAB_COLOR_COLOR		0
#define GRAB_COLOR_RED			1
#define GRAB_COLOR_GREEN		2
#define GRAB_COLOR_BLUE			3


/////////////////////////////////////////////////////////////////////////////
// CLibMil window

class CLibMil : public CWnd
{
	int m_nIdx;
	CCriticalSection m_cs;
	CWnd *m_pParent;
	
	MIL_ID	MilApplication,				/* Application identifier.  */
			MilSystem,					/* System identifier.       */
			MilImageCam,				/* Image buffer identifier.      */
			MilImageCamRotate,			/* Image buffer identifier.      */
			MilImageCamFlip;			/* Image buffer identifier.      */
// 			MilDisplay[4],				/* Display identifier.      */
// 			MilDisplayOverlay[4],
// 			MilGraphicContextID[4],
// 			MilImage[4];				/* Image buffer identifier. */
	CLibMilDisp*	MilDisplay[4];
	CLibMilBuf*	MilImage[4];
	CLibMilDraw* MilDraw[4];

	HWND	UserWindowHandle[4];
	int		m_nPixelMode[4];
	
	long	m_lFontName;
	double m_dFontScaleX;
	double m_dFontScaleY;

	////////////////////////////////////////////////////////////////
	
	// Resize, Fit
	int m_nCameraImageSizeX;
	int m_nCameraImageSizeY;
	int m_nDisplaySizeX;
	int m_nDisplaySizeY;
	double m_dFitResizeRatio;
	double m_dFitResizeRatioX;
	double m_dFitResizeRatioY;
	int m_nDisplayFitMode;
	int m_nCenterViewOffsetX;
	int m_nCenterViewOffsetY;

	void CalcFitResizeRatio(int nCameraImageSizeX, int nCameraImageSizeY, int nDisplayRectSizeX, int nDisplayRectSizeY, int nDisplayFitMode=DISPLAY_FIT_MODE_CENTERVIEW);
	void DisplaySelect(MIL_ID DisplayId, MIL_ID ImageId, HWND ClientWindowHandle, int nDisplaySizeX, int nDisplaySizeY, int nDisplayFitMode=DISPLAY_FIT_MODE_CENTERVIEW);
	void CreateOverlay();


	// Morphology ========================================================================================================
	void Erode(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode);
	void Dilate(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode);
	void Open(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode);
	void Close(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT NbIteration, MIL_INT64 ProcMode);

	void Filter(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_ID KernelId, MIL_INT Rank, MIL_INT64 Mode);	// rank filter on the pixels in an image
	void Convolution(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_ID KernelId);							// general convolution operation
	long Binarize(MIL_ID SrcImageId);	// Binarize
	void Binarize(MIL_ID SrcImageId, MIL_ID DestImageId, MIL_INT64 Condition, MIL_DOUBLE CondLow, MIL_DOUBLE CondHigh);	// Binarize
	void Resize(MIL_ID SrcImageBufId, MIL_ID DestImageBufId, double ScaleFactorX, double ScaleFactorY, long InterpolationMode);	// Resize
	void Rotate(MIL_ID SrcImageBufId, MIL_ID DestImageBufId, double Angle, double SrcCenX, double SrcCenY, double DstCenX, double DstCenY, long InterpolationMode);	// Rotate
	void Translate(MIL_ID SrcImageBufId, MIL_ID DestImageBufId, double XDisplacement, double YDisplacement, long InterpolationMode);	// Translate
	void Flip(MIL_ID SrcImageBufId, MIL_ID DestImageBufId, long Operation,long OpFlag); 
	//===================================================================================================================

// Construction
public:
	CLibMil(int nIdx, MIL_ID &MilSysId, HWND *hCtrl, CWnd* pParent=NULL);
	CLibMil(int nIdx, int *nPixelMode, HWND *hCtrl, CWnd* pParent=NULL);

// Attributes
public:

// Operations
public:
	void BufPut2d(int nIdx, long nSzX, long nSzY, TCHAR* pSrc);
	void BufPutColor2d(int nIdx, int nBitsPerPixel, long nSzX, long nSzY, TCHAR* pSrc);
	void BufPutColor2d(int nIdx, long nSzX, long nSzY, TCHAR* pSrc);
	void BufPutColor2d0(long nSzX, long nSzY, TCHAR* pSrc);
	void BufPutColor2d1(long nSzX, long nSzY, TCHAR* pSrc);

	////////////////////////////////////////////////////////////////////////////////

	MIL_ID GetSystemID();

	void DisplaySelect(CLibMilDisp* MLibDisp, CLibMilBuf* MLibImage, HWND ClientWindowHandle, int nDisplaySizeX, int nDisplaySizeY, int nDisplayFitMode=DISPLAY_FIT_MODE_CENTERVIEW);
	void DisplayZoom(double dValueX, double dValueY);
	void DisplayPan(double dValueX, double dValueY);

	CLibMilBuf* AllocBuf(int SizeBand, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute);
	CLibMilBuf* AllocBuf(long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute);
	CLibMilDisp* AllocDisp();
	CLibMilDraw* AllocDraw(CLibMilDisp* MLibDisp);
	void CreateOverlay(CLibMilDisp* MLibDisp, long nColor);
	BOOL ClearLiveOverlay();
	BOOL ClearCenterMarkArea(long XStart, long YStart, long XEnd, long YEnd);
	BOOL DrawCross(double ForegroundColor, long XCenter, long YCenter, long XSize, long YSize, long XCenterSpace, long YCenterSpace);
	void DrawText(CString str, long lX, long lY, long lColor);
	void SetTextFontScale(double dFontScaleX, double dFontScaleY);
	void SetTextFont(long FontName);
	void SetDrawColor(long lForegroundColor);

	BOOL OneshotGrab(MIL_ID MilDestImage, int nColor);// nColor -> 0: Color, 1: Red, 2: Green, 3:Blue

	//=============================================================================================================================
	// pattern matching

	MIL_ID m_PatternMatchingModel;
	MIL_ID m_PatternMatchingResult;
	// result
	int m_nPatternMatchingResultNum;
	double *m_dPatternMatchingResultPosX;
	double *m_dPatternMatchingResultPosY;
	double *m_dPatternMatchingResultAngle;
	double *m_dPatternMatchingResultScore;
	double m_dPatternMatchingResultTime;
	// output
	int m_nPatternMatchingResultSelectNum;
	double m_dPatternMatchingResultSelectPosX;
	double m_dPatternMatchingResultSelectPosY;
	double m_dPatternMatchingResultSelectAngle;
	double m_dPatternMatchingResultSelectScore;

	void PatternMatchingResultDataAlloc(int nObjectNum);
	void PatternMatchingResultDataFree();

	void PatternMatchingAlloc(MIL_ID MilImage, double dScore=30.0);
	BOOL PatternMatchingAction(MIL_ID MilImage, BOOL bDraw=FALSE);//, MIL_ID MilPatternMatchingResultDisplayBuffer, MIL_ID GraphContextId);
	void PatternMatchingFree();

	//=============================================================================================================================

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibMil)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLibMil();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLibMil)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBMIL_H__C0404020_1E38_4073_B1B6_D7F52DC66B42__INCLUDED_)
