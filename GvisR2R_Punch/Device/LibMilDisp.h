#if !defined(AFX_LIBMILDISP_H__F69ECC9B_7FA4_4F6F_AA92_F219ECAF5DF5__INCLUDED_)
#define AFX_LIBMILDISP_H__F69ECC9B_7FA4_4F6F_AA92_F219ECAF5DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibMilDisp.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CLibMilDisp window

class CLibMilDisp : public CWnd
{

// Construction
public:
	CLibMilDisp(MIL_ID SystemId);

// Attributes
public:
	MIL_ID m_MilDisplay;
	MIL_ID m_MilDisplayOverlay;

	long m_lOverlayColor;

// Operations
public:
	BOOL ClearOverlay(long pColor);
	BOOL ClearOverlay();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibMilDisp)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLibMilDisp();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLibMilDisp)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBMILDISP_H__F69ECC9B_7FA4_4F6F_AA92_F219ECAF5DF5__INCLUDED_)
