// DlgKeyNum.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgKeyNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyNum dialog


CDlgKeyNum::CDlgKeyNum(CString *strFileName, CRect* rect, CPoint ptSt, int nDir, CWnd* pParent/*=NULL*/)
	: CDialog(CDlgKeyNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgKeyNum)
		// NOTE: the ClassWizard will add member initialization here
	
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_ptSt = ptSt;
	m_nDir = nDir;

	m_pstrEdit=strFileName;
	m_strM = _T("");
	m_strCurr = _T("");
	m_strMin = _T("");
	m_strMax = _T("");

	m_Rect = rect;
	m_bInit = FALSE;
	m_dnMouse = FALSE;
	m_stPoint = (0,0);

	m_hBmpNum0 = NULL;
	m_hBmpNum1 = NULL;
	m_hBmpNum2 = NULL;
	m_hBmpNum3 = NULL;
	m_hBmpNum4 = NULL;
	m_hBmpNum5 = NULL;
	m_hBmpNum6 = NULL;
	m_hBmpNum7 = NULL;
	m_hBmpNum8 = NULL;
	m_hBmpNum9 = NULL;
	m_hBmpBs = NULL;
	m_hBmpDel = NULL;
	m_hBmpDot = NULL;
	m_hBmpLeft = NULL;
	m_hBmpMinus = NULL;
	m_hBmpRight = NULL;
	m_hBmpEsc = NULL;
	m_hBmpEnter = NULL;

	m_pBmpNum0 = NULL;
	m_pBmpNum1 = NULL;
	m_pBmpNum2 = NULL;
	m_pBmpNum3 = NULL;
	m_pBmpNum4 = NULL;
	m_pBmpNum5 = NULL;
	m_pBmpNum6 = NULL;
	m_pBmpNum7 = NULL;
	m_pBmpNum8 = NULL;
	m_pBmpNum9 = NULL;
	m_pBmpBs = NULL;
	m_pBmpDel = NULL;
	m_pBmpDot = NULL;
	m_pBmpLeft = NULL;
	m_pBmpMinus = NULL;
	m_pBmpRight = NULL;
	m_pBmpEsc = NULL;
	m_pBmpEnter = NULL;

// 	pView->m_bShowModalKeyPad[0] = TRUE;

	m_bTIM_DLG_KEYBDNUM_Lock = FALSE;
	m_bTIM_DLG_KEYBDNUM = FALSE;
}

CDlgKeyNum::~CDlgKeyNum()
{
	m_bTIM_DLG_KEYBDNUM = FALSE;
	DelImg();

	if(m_hBmpNum0)
	{
		if(DeleteObject(m_hBmpNum0))
			m_hBmpNum0 = NULL;
	}
	if(m_hBmpNum1 )
	{
		if(DeleteObject(m_hBmpNum1 ))
			m_hBmpNum1  = NULL;
	}
	if(m_hBmpNum2 )
	{
		if(DeleteObject(m_hBmpNum2 ))
			m_hBmpNum2  = NULL;
	}
	if(m_hBmpNum3 )
	{
		if(DeleteObject(m_hBmpNum3 ))
			m_hBmpNum3  = NULL;
	}
	if(m_hBmpNum4 )
	{
		if(DeleteObject(m_hBmpNum4 ))
			m_hBmpNum4  = NULL;
	}
	if(m_hBmpNum5 )
	{
		if(DeleteObject(m_hBmpNum5 ))
			m_hBmpNum5  = NULL;
	}
	if(m_hBmpNum6 )
	{
		if(DeleteObject(m_hBmpNum6 ))
			m_hBmpNum6  = NULL;
	}
	if(m_hBmpNum7 )
	{
		if(DeleteObject(m_hBmpNum7 ))
			m_hBmpNum7  = NULL;
	}
	if(m_hBmpNum8 )
	{
		if(DeleteObject(m_hBmpNum8 ))
			m_hBmpNum8  = NULL;
	}
	if(m_hBmpNum9 )
	{
		if(DeleteObject(m_hBmpNum9 ))
			m_hBmpNum9  = NULL;
	}
	if(m_hBmpBs )
	{
		if(DeleteObject(m_hBmpBs ))
			m_hBmpBs  = NULL;
	}
	if(m_hBmpDel )
	{
		if(DeleteObject(m_hBmpDel ))
			m_hBmpDel  = NULL;
	}
	if(m_hBmpDot )
	{
		if(DeleteObject(m_hBmpDot ))
			m_hBmpDot  = NULL;
	}
	if(m_hBmpLeft )
	{
		if(DeleteObject(m_hBmpLeft ))
			m_hBmpLeft  = NULL;
	}
	if(m_hBmpMinus )
	{
		if(DeleteObject(m_hBmpMinus ))
			m_hBmpMinus  = NULL;
	}
	if(m_hBmpRight )
	{
		if(DeleteObject(m_hBmpRight ))
			m_hBmpRight  = NULL;
	}
	if(m_hBmpEsc )
	{
		if(DeleteObject(m_hBmpEsc ))
			m_hBmpEsc  = NULL;
	}
	if(m_hBmpEnter )
	{
		if(DeleteObject(m_hBmpEnter ))
			m_hBmpEnter  = NULL;
	}

	if(m_pBmpNum0)
	{
		delete m_pBmpNum0;
		m_pBmpNum0 = NULL;
	}
	if(m_pBmpNum1 )
	{
		delete m_pBmpNum1;
		m_pBmpNum1  = NULL;
	}
	if(m_pBmpNum2 )
	{
		delete m_pBmpNum2;
		m_pBmpNum2  = NULL;
	}
	if(m_pBmpNum3 )
	{
		delete m_pBmpNum3;
		m_pBmpNum3  = NULL;
	}
	if(m_pBmpNum4 )
	{
		delete m_pBmpNum4;
		m_pBmpNum4  = NULL;
	}
	if(m_pBmpNum5 )
	{
		delete m_pBmpNum5;
		m_pBmpNum5  = NULL;
	}
	if(m_pBmpNum6 )
	{
		delete m_pBmpNum6;
		m_pBmpNum6  = NULL;
	}
	if(m_pBmpNum7 )
	{
		delete m_pBmpNum7;
		m_pBmpNum7  = NULL;
	}
	if(m_pBmpNum8 )
	{
		delete m_pBmpNum8;
		m_pBmpNum8  = NULL;
	}
	if(m_pBmpNum9 )
	{
		delete m_pBmpNum9;
		m_pBmpNum9  = NULL;
	}
	if(m_pBmpBs )
	{
		delete m_pBmpBs;
		m_pBmpBs  = NULL;
	}
	if(m_pBmpDel )
	{
		delete m_pBmpDel;
		m_pBmpDel  = NULL;
	}
	if(m_pBmpDot )
	{
		delete m_pBmpDot;
		m_pBmpDot  = NULL;
	}
	if(m_pBmpLeft )
	{
		delete m_pBmpLeft;
		m_pBmpLeft  = NULL;
	}
	if(m_pBmpMinus )
	{
		delete m_pBmpMinus;
		m_pBmpMinus  = NULL;
	}
	if(m_pBmpRight )
	{
		delete m_pBmpRight;
		m_pBmpRight  = NULL;
	}
	if(m_pBmpEsc )
	{
		delete m_pBmpEsc;
		m_pBmpEsc  = NULL;
	}
	if(m_pBmpEnter )
	{
		delete m_pBmpEnter;
		m_pBmpEnter  = NULL;
	}

// 	pView->m_bShowModalKeyPad[0] = FALSE;
}

void CDlgKeyNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKeyNum)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKeyNum, CDialog)
	//{{AFX_MSG_MAP(CDlgKeyNum)
	ON_BN_CLICKED(IDC_BUTTON_BS, OnButtonBs)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DOT, OnButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, OnButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_NUM0, OnButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_NUM1, OnButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM2, OnButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM3, OnButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM4, OnButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM5, OnButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM6, OnButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM7, OnButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM8, OnButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM9, OnButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnButtonRight)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ESC, OnButtonEsc)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_KEYPADNUM, OnMyKeyPadNum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyNum message handlers

void CDlgKeyNum::OnButtonBs() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,VK_BACK,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_KEYDOWN,VK_DELETE,0);
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonDot() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x2E,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonEnter() 
{
	// TODO: Add your control notification handler code here
	CString strDisplay=_T("");
	GetDlgItemText(IDC_EDIT_DISPLAY,strDisplay);
	m_bTIM_DLG_KEYBDNUM = FALSE;
	if(strDisplay==_T("")) 
	{
		OnOK();
		return;
	}
	*m_pstrEdit = strDisplay;
	OnOK();
}

void CDlgKeyNum::OnButtonLeft() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_KEYDOWN,VK_LEFT,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonMinus() 
{
	// TODO: Add your control notification handler code here
	
	CString sPrev, str;
	GetDlgItem(IDC_EDIT_DISPLAY)->GetWindowText(sPrev);
	if(!sPrev.IsEmpty())
	{
		if(_tstof(sPrev) > 0)
		{
			str.Format(_T("-%s"), sPrev);
			GetDlgItem(IDC_EDIT_DISPLAY)->SetWindowText(str);
		}
		else if(_tstof(sPrev) < 0)
		{
			str.Format(_T("%s"), sPrev.Right(sPrev.GetLength()-1));
			GetDlgItem(IDC_EDIT_DISPLAY)->SetWindowText(str);
		}
		//this->SetFocus();
		GetDlgItem(IDC_BUTTON_ENTER)->SetFocus();
		for(int i=0; i<str.GetLength(); i++)
			SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_KEYDOWN,VK_RIGHT,0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
		SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x2D,0);
	}
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum0() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x30,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x31,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum2() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x32,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum3() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x33,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum4() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x34,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum5() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x35,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum6() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x36,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum7() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x37,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum8() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x38,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonNum9() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_CHAR,0x39,0);
	myEdit.Refresh();
}

void CDlgKeyNum::OnButtonRight() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFocus();
	SendDlgItemMessage(IDC_EDIT_DISPLAY,WM_KEYDOWN,VK_RIGHT,0);
	myEdit.Refresh();
}

HBRUSH CDlgKeyNum::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
// 	if (nCtlColor == CTLCOLOR_DLG) 
// 	{
//    		pDC->SetBkMode(TRANSPARENT);
//      	hbr = CreateSolidBrush(RGB(0,0,0));
//  	} 	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgKeyNum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rt(0,0,0,0), rc(0,0,0,0);
	// 다이얼로그 윈도우의 사이즈를 얻는다. 
	this->GetWindowRect( &rt );
	int nWidth = rt.right - rt.left;
	int nHeight = rt.bottom - rt.top;
	
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, FALSE);
	int nSWidth = rc.right - rc.left;
	int nSHeight = rc.bottom - rc.top;
	
	if(nSHeight < (m_ptSt.y+nHeight))
	{
		m_nDir &= ~(TO_BOTTOM);
		m_nDir |= TO_TOP;
	}
	else if(m_ptSt.y < nHeight)
	{
		m_nDir &= ~(TO_TOP);
		m_nDir |= TO_BOTTOM;
	}
	
	if(nSWidth < (m_ptSt.x+nWidth))
	{
		m_nDir &= ~(TO_RIGHT);
		m_nDir |= TO_LEFT;
	}
	else if(m_ptSt.x < nWidth)
	{
		m_nDir &= ~(TO_LEFT);
		m_nDir |= TO_RIGHT;
	}

	switch(m_nDir)
	{
	case TO_TOP|TO_LEFT:
		rt.bottom = m_ptSt.y;
		rt.right = m_ptSt.x;
		rt.top = rt.bottom - nHeight;
		rt.left = rt.right - nWidth;
		break;	
	case TO_TOP|TO_RIGHT:
		rt.bottom = m_ptSt.y;
		rt.left = m_ptSt.x;
		rt.top = rt.bottom - nHeight;
		rt.right = rt.left + nWidth;
		break;	
	case TO_BOTTOM|TO_LEFT:
		rt.top = m_ptSt.y;
		rt.right = m_ptSt.x;
		rt.bottom = rt.top + nHeight;
		rt.left = rt.right - nWidth;
		break;	
	case TO_BOTTOM|TO_RIGHT:
		rt.top = m_ptSt.y;
		rt.left = m_ptSt.x;
		rt.bottom = rt.top + nHeight;
		rt.right = rt.left + nWidth;
	default:
		MoveWindow(1280-nWidth-5,78,nWidth,nHeight,TRUE);
		break;	
	}

	if(m_nDir != TO_NONE)
	{
		m_Rect->top = rt.top;
		m_Rect->bottom = rt.bottom;
		m_Rect->left = rt.left;
		m_Rect->right = rt.right;
		MoveWindow(rt);
	}
	//MoveWindow(1280-nWidth-5,78,nWidth,nHeight,TRUE);

	InitBtn();
	InitPic();
	InitEdit();
	InitLabel();
	InitStatic();

	m_bInit = TRUE;
	m_bTIM_DLG_KEYBDNUM = TRUE;
	SetTimer(TIM_SHOW_KEYPADNUM, 30, NULL);

	LoadImg();
	DispVal();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKeyNum::DispVal()
{
	if(m_strM.IsEmpty())
		GetDlgItem(IDC_STC_M)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_STC_M)->SetWindowText(m_strM);

	if(m_strCurr.IsEmpty())
		GetDlgItem(IDC_STC_CURR)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_STC_CURR)->SetWindowText(m_strCurr);

	if(m_strMin.IsEmpty())
		GetDlgItem(IDC_STC_MIN)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_STC_MIN)->SetWindowText(m_strMin);

	if(m_strMax.IsEmpty())
		GetDlgItem(IDC_STC_MAX)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_STC_MAX)->SetWindowText(m_strMax);

	if(m_strMin.IsEmpty() && m_strMax.IsEmpty())
		GetDlgItem(IDC_STC_SEP)->ShowWindow(SW_HIDE);
}

void CDlgKeyNum::LoadImg()
{
	myLabel.LoadBkImage(IMG_BK_BLACK);
}

void CDlgKeyNum::DelImg()
{
	myLabel.DelImgList();
}


void CDlgKeyNum::InitLabel()
{
	myLabel.SubclassDlgItem(IDC_STC_SEP, this);
	myLabel.SetFontName(_T("Arial"));
	myLabel.SetFontSize(32);
	myLabel.SetFontBold(TRUE);
	myLabel.SetTextColor(RGB_YELLOW);
	//myLabel.DrawBoarder(0,0);
}

void CDlgKeyNum::InitStatic()
{
	myStatic[0].SubclassDlgItem(IDC_STC_MIN, this);
	myStatic[0].SetTextColor(RGB_YELLOW);
	myStatic[0].SetBkColor(RGB_BLACK);
	myStatic[0].SetFontSize(14);

	myStatic[1].SubclassDlgItem(IDC_STC_MAX, this);
	myStatic[1].SetTextColor(RGB_YELLOW);
	myStatic[1].SetBkColor(RGB_BLACK);
	myStatic[1].SetFontSize(14);

	myStatic[2].SubclassDlgItem(IDC_STC_M, this);
	myStatic[2].SetTextColor(RGB_BLUE);
	myStatic[2].SetBkColor(RGB_GREEN);
	myStatic[2].SetFontSize(14);

	myStatic[3].SubclassDlgItem(IDC_STC_CURR, this);
	myStatic[3].SetTextColor(RGB_BLACK);
	myStatic[3].SetBkColor(RGB_YELLOW);
	myStatic[3].SetFontSize(18);

	for(int i=0; i<MAX_STATIC_KEYPAD; i++)
	{
		myStatic[i].SetFontName(_T("Arial"));
		myStatic[i].SetFontBold(TRUE);
	}
}

void CDlgKeyNum::InitEdit()
{
	myEdit.SubclassDlgItem(IDC_EDIT_DISPLAY, this);
	myEdit.SetFontName(_T("Arial"));
	myEdit.SetFontSize(FONT_SIZE_KEYNUM);
	myEdit.SetFontBold(TRUE);
	myEdit.SetTextColor(RGB_BLUE);
	myEdit.SetBkColor(RGB_WHITE);
	myEdit.SetText(_T(""));
	myEdit.SetFont((CFont*)&myEdit.m_font, TRUE);
}


void CDlgKeyNum::InitPic()
{
	myPic.Load(IMG_KEYPAD_BK);
}

void CDlgKeyNum::InitBtn()
{
	//Bitmap Button Init....

	CSize sz;
	CRect	rClient;

	if(!m_pBmpNum0)
		m_pBmpNum0 = new CBitmap;
	if(!m_pBmpNum1)
		m_pBmpNum1 = new CBitmap;
	if(!m_pBmpNum2)
		m_pBmpNum2 = new CBitmap;
	if(!m_pBmpNum3)
		m_pBmpNum3 = new CBitmap;
	if(!m_pBmpNum4)
		m_pBmpNum4 = new CBitmap;
	if(!m_pBmpNum5)
		m_pBmpNum5 = new CBitmap;
	if(!m_pBmpNum6)
		m_pBmpNum6 = new CBitmap;
	if(!m_pBmpNum7)
		m_pBmpNum7 = new CBitmap;
	if(!m_pBmpNum8)
		m_pBmpNum8 = new CBitmap;
	if(!m_pBmpNum9)
		m_pBmpNum9 = new CBitmap;
	if(!m_pBmpBs)
		m_pBmpBs = new CBitmap;
	if(!m_pBmpDel)
		m_pBmpDel = new CBitmap;
	if(!m_pBmpDot)
		m_pBmpDot = new CBitmap;
	if(!m_pBmpLeft)
		m_pBmpLeft = new CBitmap;
	if(!m_pBmpMinus)
		m_pBmpMinus = new CBitmap;
	if(!m_pBmpRight)
		m_pBmpRight = new CBitmap;
	if(!m_pBmpEsc)
		m_pBmpEsc = new CBitmap;
	if(!m_pBmpEnter)
		m_pBmpEnter = new CBitmap;

	((CButton*)GetDlgItem(IDC_BUTTON_NUM0))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum0)
	{
		m_hBmpNum0 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM0, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum0->Attach(m_hBmpNum0);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM0))->SetBitmap(m_hBmpNum0);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM1))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum1)
	{
		m_hBmpNum1 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM1, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum1->Attach(m_hBmpNum1);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM1))->SetBitmap(m_hBmpNum1);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM2))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum2)
	{
		m_hBmpNum2 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM2, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum2->Attach(m_hBmpNum2);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM2))->SetBitmap(m_hBmpNum2);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM3))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum3)
	{
		m_hBmpNum3 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM3, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum3->Attach(m_hBmpNum3);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM3))->SetBitmap(m_hBmpNum3);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM4))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum4)
	{
		m_hBmpNum4 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM4, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum4->Attach(m_hBmpNum4);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM4))->SetBitmap(m_hBmpNum4);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM5))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum5)
	{
		m_hBmpNum5 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM5, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum5->Attach(m_hBmpNum5);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM5))->SetBitmap(m_hBmpNum5);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM6))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum6)
	{
		m_hBmpNum6 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM6, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum6->Attach(m_hBmpNum6);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM6))->SetBitmap(m_hBmpNum6);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM7))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum7)
	{
		m_hBmpNum7 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM7, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum7->Attach(m_hBmpNum7);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM7))->SetBitmap(m_hBmpNum7);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM8))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum8)
	{
		m_hBmpNum8 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM8, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum8->Attach(m_hBmpNum8);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM8))->SetBitmap(m_hBmpNum8);

	((CButton*)GetDlgItem(IDC_BUTTON_NUM9))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpNum9)
	{
		m_hBmpNum9 = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_NUM9, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpNum9->Attach(m_hBmpNum9);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_NUM9))->SetBitmap(m_hBmpNum9);

	((CButton*)GetDlgItem(IDC_BUTTON_ESC))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpEsc)
	{
		m_hBmpEsc = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_ESC, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpEsc->Attach(m_hBmpEsc);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_ESC))->SetBitmap(m_hBmpEsc);

	((CButton*)GetDlgItem(IDC_BUTTON_BS))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpBs)
	{
		m_hBmpBs = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_BS, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpBs->Attach(m_hBmpBs);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_BS))->SetBitmap(m_hBmpBs);

	((CButton*)GetDlgItem(IDC_BUTTON_DEL))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpDel)
	{
		m_hBmpDel = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_DEL, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpDel->Attach(m_hBmpDel);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_DEL))->SetBitmap(m_hBmpDel);

	((CButton*)GetDlgItem(IDC_BUTTON_DOT))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpDot)
	{
		m_hBmpDot = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_DOT, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpDot->Attach(m_hBmpDot);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_DOT))->SetBitmap(m_hBmpDot);

	((CButton*)GetDlgItem(IDC_BUTTON_LEFT))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpLeft)
	{
		m_hBmpLeft = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_LEFT, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpLeft->Attach(m_hBmpLeft);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_LEFT))->SetBitmap(m_hBmpLeft);

	((CButton*)GetDlgItem(IDC_BUTTON_RIGHT))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpRight)
	{
		m_hBmpRight = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_RIGHT, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpRight->Attach(m_hBmpRight);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_RIGHT))->SetBitmap(m_hBmpRight);

	((CButton*)GetDlgItem(IDC_BUTTON_MINUS))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpMinus)
	{
		m_hBmpMinus = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_MINUS, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpMinus->Attach(m_hBmpMinus);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_MINUS))->SetBitmap(m_hBmpMinus);

	((CButton*)GetDlgItem(IDC_BUTTON_ENTER))->GetClientRect(&rClient);
	sz.cx = rClient.Width()-6;
	sz.cy = rClient.Height()-6;
	if(!m_hBmpEnter)
	{
		m_hBmpEnter = (HBITMAP)::LoadImage(NULL, IMG_KEYPAD_ENTER, IMAGE_BITMAP, sz.cx, sz.cy, LR_LOADFROMFILE);
		m_pBmpEnter->Attach(m_hBmpEnter);
	}
	((CButton*)GetDlgItem(IDC_BUTTON_ENTER))->SetBitmap(m_hBmpEnter);

}

BOOL CDlgKeyNum::SetCtrlColor(const UINT nID, CString strText, COLORREF bgColor, COLORREF fgColor)
{
	RECT rect;
	CWnd* wndCtrl = NULL;
    CString str=_T("");

	wndCtrl = this->GetDescendantWindow(nID);
	// Get rectangular boundary of Control window
	// and reset coordinates to client space
	wndCtrl->GetWindowRect(&rect);
	wndCtrl->ScreenToClient(&rect);  
	// Get device context of button window
	CDC* dc = wndCtrl->GetDC();
	dc->FillSolidRect(&rect,bgColor);
	dc->SetTextColor(fgColor);
	wndCtrl->GetWindowText(str);
	dc->DrawText(strText,&rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc->DrawEdge(&rect, BDR_RAISEDINNER | BDR_RAISEDOUTER, BF_RECT);
	// Release device context each time
	ReleaseDC(dc);
	return TRUE;

}

void CDlgKeyNum::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CDlgKeyNum::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			OnButtonEnter();
			return TRUE;
		case VK_ESCAPE:
			m_bTIM_DLG_KEYBDNUM = FALSE;
			OnOK();
			return TRUE;
		case VK_SPACE:
			return TRUE;
		case VK_TAB:
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgKeyNum::OnButtonEsc() 
{
	// TODO: Add your control notification handler code here
	m_bTIM_DLG_KEYBDNUM = FALSE;
	OnOK();
}

void CDlgKeyNum::KillFocus(int nID)
{
	SetTimer(TIM_KILL_FOCUS_, 30, NULL);
}


void CDlgKeyNum::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	if(nIDEvent == TIM_SHOW_KEYPADNUM)
// 	{
// 		KillTimer(TIM_SHOW_KEYPADNUM);
// 		if(!this->IsWindowVisible())
// 		{
// 			ShowWindow(SW_SHOW);
// 			SetTimer(TIM_SHOW_KEYPADNUM, 300, NULL);
// 		}
// 	}
	if(nIDEvent == TIM_SHOW_KEYPADNUM)
	{
		KillTimer(TIM_SHOW_KEYPADNUM);
// 		if(!m_bTIM_DLG_KEYBDNUM_Lock)
// 		{
// 			m_bTIM_DLG_KEYBDNUM_Lock = TRUE;
// 			this->PostMessage(WM_MY_KEYPADNUM,0,0);
// 			//DispIo();
// 		}
		if(!this->IsWindowVisible())
			this->ShowWindow(SW_SHOW);

		if(m_bTIM_DLG_KEYBDNUM)
			SetTimer(TIM_SHOW_KEYPADNUM, 500, NULL);
	}

	if(nIDEvent == TIM_KILL_FOCUS_)
	{
		KillTimer(TIM_KILL_FOCUS_);

		InitEdit();
		GetDlgItem(IDC_BUTTON_ENTER)->SetFocus();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgKeyNum::SetPasswordStyle() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_DISPLAY))->SetPasswordChar('*');
}

void CDlgKeyNum::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
//	if(m_Rect && m_bInit)
	{
		CRect rect(0,0,0,0);
		GetWindowRect(&rect);
		*m_Rect = rect;
	}
}

void CDlgKeyNum::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);

// 	CPoint pt;
// 	pt.x = 10;
// 	pt.y = 10;
// 	dc.RoundRect(rect,pt);
//	rect.DeflateRect(3,3,3,3);
	myPic.Show(&dc,rect);

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgKeyNum::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_dnMouse = TRUE;
	m_stPoint = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgKeyNum::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_dnMouse)
	{
		CRect rect(0,0,0,0);
		GetWindowRect(&rect);
		rect.TopLeft() += (point-m_stPoint);
		rect.BottomRight() += (point-m_stPoint);
		MoveWindow(rect);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgKeyNum::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_dnMouse = FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);
}

LRESULT CDlgKeyNum::OnMyKeyPadNum(WPARAM wPara, LPARAM lPara)
{
	if(!m_bTIM_DLG_KEYBDNUM)
		return 0L;

	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);

	m_bTIM_DLG_KEYBDNUM_Lock = FALSE;

	return 0L;
} 
