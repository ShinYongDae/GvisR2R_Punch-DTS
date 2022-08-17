// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
CDlgProgress	*gpDlgProgress;

CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	gpDlgProgress = this;
	m_pParentWnd = CWnd::GetSafeOwner(pParent); // // Get the true parent of the dialog
    m_strCaption = _T("");
	m_bCancel=FALSE;
    m_nLower=0;
    m_nUpper=100;
    m_nStep=1;
    m_bParentDisabled = FALSE;
}

CDlgProgress::CDlgProgress(CFormView* pView)
{
	gpDlgProgress = this;
	m_pView = pView;
}

CDlgProgress::~CDlgProgress()
{
	if(m_hWnd!=NULL)
		DestroyWindow();
}
BOOL CDlgProgress::DestroyWindow() 
{
	ReEnableParent();
	return CDialog::DestroyWindow();
}
BOOL CDlgProgress::Create(CString strCaption)
{
    m_strCaption = strCaption;
	
	// m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
		m_pParentWnd->EnableWindow(FALSE);
		m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CDlgProgress::IDD,m_pParentWnd))
    {
		ReEnableParent();
		return FALSE;
    }
	return TRUE;
}
void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers
BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ctrlProgress.SetRange(m_nLower,m_nUpper);
    m_ctrlProgress.SetStep(m_nStep);
    m_ctrlProgress.SetPos(m_nLower);
    SetCaption(m_strCaption);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProgress::OnCancel() 
{
	m_bCancel=TRUE;
}

void CDlgProgress::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

HBRUSH CDlgProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgProgress::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

}

void CDlgProgress::ReEnableParent()
{
	if(m_bParentDisabled && (m_pParentWnd!=NULL))
		m_pParentWnd->EnableWindow(TRUE);
	m_bParentDisabled=FALSE;
}

void CDlgProgress::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

void CDlgProgress::UpdatePercent(int nNewPos)
{
    CWnd *pWndPercent = GetDlgItem(IDC_STATUS);
    int nPercent;
    
    int nDivisor = m_nUpper - m_nLower;
    ASSERT(nDivisor>0);  // m_nLower should be smaller than m_nUpper

    int nDividend = (nNewPos - m_nLower);
    ASSERT(nDividend>=0);   // Current position should be greater than m_nLower

    nPercent = nDividend * 100 / nDivisor;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPercent!=100)
      nPercent %= 100;

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%c"),nPercent,_T('%'));

	CString strCur; // get current percentage
    pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}

void CDlgProgress::Quit()
{
	gpDlgProgress = NULL;
	CDialog::OnCancel();
}

int CDlgProgress::SetPos(int nPos)
{
    PumpMessages();
    int iResult = m_ctrlProgress.SetPos(nPos);
    UpdatePercent(nPos);
	return iResult;
}

void CDlgProgress::SetRange(int nLower, int nUpper)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_ctrlProgress.SetRange(nLower,nUpper);
}

int CDlgProgress::SetStep(int nStep)
{
    m_nStep = nStep; // Store for later use in calculating percentage
    return m_ctrlProgress.SetStep(nStep);
}

int CDlgProgress::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_ctrlProgress.OffsetPos(nPos);
    UpdatePercent(iResult+nPos);
    return iResult;
}

int CDlgProgress::StepIt()
{
    PumpMessages();
    int iResult = m_ctrlProgress.StepIt();
    UpdatePercent(iResult+m_nStep);
    return iResult;
}

void CDlgProgress::SetCaption(CString strCaption)
{
	SetWindowText(strCaption);
}

BOOL CDlgProgress::CheckCancelButton()
{
   // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}
