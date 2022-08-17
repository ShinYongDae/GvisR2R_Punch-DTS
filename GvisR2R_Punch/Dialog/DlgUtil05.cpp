// DlgUtil05.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgUtil05.h"

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
// CDlgUtil05 dialog


CDlgUtil05::CDlgUtil05(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUtil05::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUtil05)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_pParent = pParent;
	m_strSym = _T("");
	m_strOutName = _T("");
	m_strParam[0] = _T("");
	m_strParam[1] = _T("");
// 	m_strParam[2] = _T("");
	m_bLoadImg = FALSE;
	m_bTIM_DLG_MY_IO_Lock = FALSE;
	m_nSegment = -1;
	m_nIdxSliceIo = -1;
	m_nIdxMpeIo = -1;
}

CDlgUtil05::~CDlgUtil05()
{
	m_bTIM_DLG_MY_IO = FALSE;
	AtDlgHide();

	if(m_pRect)	
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}


BOOL CDlgUtil05::Create()
{
	return CDialog::Create(CDlgUtil05::IDD);
}

void CDlgUtil05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUtil05)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUtil05, CDialog)
	//{{AFX_MSG_MAP(CDlgUtil05)
	ON_BN_CLICKED(IDC_BTN_00, OnBtn00)
	ON_BN_CLICKED(IDC_BTN_01, OnBtn01)
	ON_BN_CLICKED(IDC_BTN_02, OnBtn02)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MY_IO, OnMyIo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUtil05 message handlers

BOOL CDlgUtil05::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl Å°°¡ ´­·ÁÁø »óÅÂ
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgUtil05::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;	
	
		CRect rect(0,0,0,0);
		GetClientRect(m_pRect);
		GetWindowRect(rect);
		//ClientToScreen(m_pRect);
 		//int nHeight = m_pRect->Height();
		//int nWidth = m_pRect->Width();
		int nHeight = rect.Height();
		int nWidth = rect.Width();
		m_pRect->top = (1024 - nHeight) / 2;
		m_pRect->bottom = m_pRect->top + nHeight;
		m_pRect->left = (1280 - nWidth) / 2;
		m_pRect->right = m_pRect->left + nWidth;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
	
}

void CDlgUtil05::AtDlgShow()
{
	LoadImg();
}

void CDlgUtil05::AtDlgHide()
{
// 	m_bTIM_DLG_MY_IO = FALSE;
	DelImg();
	Sleep(300);
}

void CDlgUtil05::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myBtn[0].LoadBkImage(IMG_BTN_UP_DlgUtil05, BTN_IMG_UP);
	myBtn[0].LoadBkImage(IMG_BTN_DN_DlgUtil05, BTN_IMG_DN);
 	myBtn[0].LoadImage(ICO_LED_GRY_DlgUtil05, BTN_IMG_UP, CSize(50,50), BTN_POS_LEFT);
 	myBtn[0].LoadImage(ICO_LED_RED_DlgUtil05, BTN_IMG_DN, CSize(50,50), BTN_POS_LEFT);

	myBtn[1].LoadBkImage(IMG_BTN_UP_DlgUtil05, BTN_IMG_UP);
	myBtn[1].LoadBkImage(IMG_BTN_DN_DlgUtil05, BTN_IMG_DN);
 	myBtn[1].LoadImage(ICO_LED_GRY_DlgUtil05, BTN_IMG_UP, CSize(50,50), BTN_POS_LEFT);
 	myBtn[1].LoadImage(ICO_LED_RED_DlgUtil05, BTN_IMG_DN, CSize(50,50), BTN_POS_LEFT);

	myBtn[2].LoadBkImage(IMG_BTN_UP_DlgUtil05, BTN_IMG_UP);
	myBtn[2].LoadBkImage(IMG_BTN_DN_DlgUtil05, BTN_IMG_DN);
 	myBtn[2].LoadImage(ICO_LED_GRY_DlgUtil05, BTN_IMG_UP, CSize(50,50), BTN_POS_LEFT);
 	myBtn[2].LoadImage(ICO_LED_RED_DlgUtil05, BTN_IMG_DN, CSize(50,50), BTN_POS_LEFT);
}

void CDlgUtil05::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	for(int i=0; i<3; i++)
		myBtn[i].DelImgList();
}


BOOL CDlgUtil05::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitPic();
	InitStatic();
	InitBtn();
	
	Disp();

	m_bTIM_DLG_MY_IO = TRUE;
	SetTimer(TIM_DLG_MY_IO, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUtil05::InitPic()
{
	myPic.Load(IMG_BTN_DN_DlgUtil05);
}

void CDlgUtil05::InitStatic()
{
	m_stc[0].SubclassDlgItem(IDC_STC_00, this);
	m_stc[0].SetBkColor(RGB(226,233,251));
	m_stc[0].SetTextColor(RGB_BLACK);
	m_stc[0].SetFontName(_T("±¼¸²"));
	m_stc[0].SetFontSize(18);
	m_stc[0].SetFontBold(TRUE);
	
	m_stc[1].SubclassDlgItem(IDC_STC_10, this);
	m_stc[1].SetBkColor(RGB_WHITE);
	m_stc[1].SetTextColor(RGB_BROWN);
	m_stc[1].SetFontName(_T("±¼¸²"));
	m_stc[1].SetFontSize(14);
	m_stc[1].SetFontBold(TRUE);
}

void CDlgUtil05::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_BTN_00, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);
	myBtn[1].SubclassDlgItem(IDC_BTN_01, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_01);
	myBtn[2].SubclassDlgItem(IDC_BTN_02, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_02);
	for(int i=0; i<3; i++)
	{
		myBtn[i].SetFont(_T("±¼¸²Ã¼"),24,TRUE);
		myBtn[i].SetTextColor(RGB_DARKMAGENTA);
	}
}

void CDlgUtil05::Disp()
{
	m_stc[0].SetText(m_strSym);
	m_stc[1].SetText(m_strOutName);
}

void CDlgUtil05::SetDisp(CString strParam0, CString strParam1)
{
	m_strSym = strParam0;
	m_strOutName = strParam1;
}

void CDlgUtil05::SetParamMp2100(CString *pParam)
{
	m_strParam[0] = pParam[0];	// OB*****
// 	m_strParam[1] = pParam[1];	// MB******
	m_strParam[1] = pParam[1];	// DblSol Index			// Old version is MB****** (DblSol)
}

void CDlgUtil05::SetParamSliceIo(int nSegment, int nIdx)
{
	m_nSegment = nSegment;
	m_nIdxSliceIo = nIdx;
	m_nIdxMpeIo = -1;
}

void CDlgUtil05::SetParamMpeIo(int nSegment, int nIdx)
{
	m_nSegment = nSegment;
	m_nIdxSliceIo = -1;
	m_nIdxMpeIo = nIdx;
}

BOOL CDlgUtil05::IsDblSol()
{
	if(!m_strParam[1].IsEmpty())
	{
		m_strParam[1].MakeUpper();
		if(m_strParam[1].Find(_T("MB"),0) > -1 && m_strParam[2].GetLength() == 8)
			return TRUE;
	}
	return FALSE;
}

void CDlgUtil05::OnBtn00() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_MPE
	BOOL bOn;

	if(m_nIdxMpeIo > -1)
	{
		bOn = pDoc->m_pMpeIo[m_nSegment] & (0x01<<m_nIdxMpeIo);
		if(!bOn)
		{
			pDoc->m_pMpeIo[m_nSegment] |= (0x01<<m_nIdxMpeIo);
			if(IsDblSol())
				pDoc->m_pMpeIo[m_nSegment] &= ~(0x01<<_tstoi(m_strParam[1])); // m_strParam[2] : DblSol Index
		}
	}
// 	else if(m_nIdxSliceIo > -1)
// 	{
// 		bOn = pDoc->m_pSliceIo[m_nSegment] & (0x01<<m_nIdxSliceIo);
// 		if(!bOn)
// 		{
// 			pDoc->m_pSliceIo[m_nSegment] |= (0x01<<m_nIdxSliceIo);
// 			if(IsDblSol())
// 				pDoc->m_pSliceIo[m_nSegment] &= ~(0x01<<_tstoi(m_strParam[2])); // m_strParam[2] : DblSol Index
// 		}
// 	}
	else
		AfxMessageBox(_T("Failed IO control..."));
#endif
}

void CDlgUtil05::OnBtn01() 
{
	// TODO: Add your control notification handler code here
#ifdef USE_MPE
	BOOL bOn;

	if(m_nIdxMpeIo > -1)
	{
		bOn = pDoc->m_pMpeIo[m_nSegment] & (0x01<<m_nIdxMpeIo);
		if(bOn)
		{
			pDoc->m_pMpeIo[m_nSegment] &= ~(0x01<<m_nIdxMpeIo);
			if(IsDblSol())
				pDoc->m_pMpeIo[m_nSegment] |= (0x01<<_tstoi(m_strParam[1])); // m_strParam[2] : DblSol Index
		}
	}
// 	else if(m_nIdxSliceIo > -1)
// 	{
// 		bOn = pDoc->m_pSliceIo[m_nSegment] & (0x01<<m_nIdxSliceIo);
// 		if(bOn)
// 		{
// 			pDoc->m_pSliceIo[m_nSegment] &= ~(0x01<<m_nIdxSliceIo);
// 			if(IsDblSol())
// 				pDoc->m_pSliceIo[m_nSegment] |= (0x01<<_tstoi(m_strParam[2])); // m_strParam[2] : DblSol Index
// 		}
// 	}
	else
		AfxMessageBox(_T("Failed IO control..."));
#endif
}

void CDlgUtil05::OnBtn02() 
{
	// TODO: Add your control notification handler code here
	myBtn[2].SetImage(BTN_IMG_DN);
	OnOK();
}

void CDlgUtil05::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect(0,0,0,0);
	GetClientRect(&rect);
	myPic.Show(&dc,rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgUtil05::DispIo() 
{
	BOOL bStatus=FALSE;
#ifdef USE_MPE
	if(m_nIdxMpeIo > -1)
		bStatus = pDoc->m_pMpeIo[m_nSegment] & (0x01<<m_nIdxMpeIo);
// 	else if(m_nIdxSliceIo > -1)
// 		bStatus = pDoc->m_pSliceIo[m_nSegment] & (0x01<<m_nIdxSliceIo);
#endif
	if(bStatus)
	{
		if(myBtn[0].GetImage() != BTN_IMG_DN)
		{
			myBtn[0].SetImage(BTN_IMG_DN);
			myBtn[0].Refresh();
		}
		if(myBtn[1].GetImage() != BTN_IMG_UP)
		{
			myBtn[1].SetImage(BTN_IMG_UP);
			myBtn[1].Refresh();
		}
	}
	else
	{
		if(myBtn[0].GetImage() != BTN_IMG_UP)
		{
			myBtn[0].SetImage(BTN_IMG_UP);
			myBtn[0].Refresh();
		}
		if(myBtn[1].GetImage() != BTN_IMG_DN)
		{
			myBtn[1].SetImage(BTN_IMG_DN);
			myBtn[1].Refresh();
		}
	}
}

void CDlgUtil05::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIM_DLG_MY_IO)
	{
		KillTimer(TIM_DLG_MY_IO);
		if(!m_bTIM_DLG_MY_IO_Lock)
		{
			m_bTIM_DLG_MY_IO_Lock = TRUE;
			this->PostMessage(WM_MY_IO,0,0);
			//DispIo();
		}

		if(!this->IsWindowVisible())
			this->ShowWindow(SW_SHOW);

		if(m_bTIM_DLG_MY_IO)
			SetTimer(TIM_DLG_MY_IO, 100, NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT  CDlgUtil05::OnMyIo(WPARAM wPara, LPARAM lPara)
{
	if(!this->IsWindowVisible())
		this->ShowWindow(SW_SHOW);

	DispIo();
	m_bTIM_DLG_MY_IO_Lock = FALSE;

	return 0L;
}
