#if !defined(AFX_LIBMILDRAW_H__8E39D5F1_73E9_42AB_B698_C242993D6480__INCLUDED_)
#define AFX_LIBMILDRAW_H__8E39D5F1_73E9_42AB_B698_C242993D6480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibMilDraw.h : header file
//

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

#include "LibMilDisp.h"

/////////////////////////////////////////////////////////////////////////////
// CLibMilDraw window

class CLibMilDraw : public CWnd
{

	long m_lFontName;
	double m_dFontScaleX;
	double m_dFontScaleY;
	long m_lForegroundColor;
	long m_lBackgroundColor;

// Construction
public:
	CLibMilDraw(MIL_ID SystemId, CLibMilDisp* MLibDisp);

// Attributes
public:
	MIL_ID m_MilBuffer;
	MIL_ID m_MilGraphicContextID;

// Operations
public:
	void SetDrawColor(long lForegroundColor);
	void SetDrawBackColor(long lBackgroundColor);
	void SetTextFontScale(double dFontScaleX, double dFontScaleY);
	BOOL DrawCross(double ForegroundColor, long XCenter, long YCenter, long XSize, long YSize, long XCenterSpace, long YCenterSpace);
	BOOL DrawRectFill(double ForegroundColor, long XStart, long YStart, long XEnd, long YEnd);
	BOOL DrawText(long XStart, long YStart, MIL_TEXT_PTR String);
	void SetTextFont(long FontName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibMilDraw)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLibMilDraw();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLibMilDraw)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBMILDRAW_H__8E39D5F1_73E9_42AB_B698_C242993D6480__INCLUDED_)
