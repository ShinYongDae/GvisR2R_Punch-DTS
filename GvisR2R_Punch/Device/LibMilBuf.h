#if !defined(AFX_LIBMILBUF_H__9DCB3CBA_6FCE_463C_917D_C799395FA99E__INCLUDED_)
#define AFX_LIBMILBUF_H__9DCB3CBA_6FCE_463C_917D_C799395FA99E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LibMilBuf.h : header file
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

#pragma warning(disable: 4995)

/////////////////////////////////////////////////////////////////////////////
// CLibMilBuf window

class CLibMilBuf : public CWnd
{

// Construction
public:
	CLibMilBuf(MIL_ID SystemId, int SizeBand, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute);
	CLibMilBuf(MIL_ID SystemId, long SizeX, long SizeY, long Type, BUFATTRTYPE Attribute);

// Attributes
public:
 	MIL_ID m_MilImage;
	MIL_ID m_MilImageChild;

// Operations
public:
	void Clear();
	void ChildBuffer2d(long OffX, long OffY, long SizeX, long SizeY);
	void BufferLoad(TCHAR* FileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibMilBuf)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLibMilBuf();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLibMilBuf)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBMILBUF_H__9DCB3CBA_6FCE_463C_917D_C799395FA99E__INCLUDED_)
