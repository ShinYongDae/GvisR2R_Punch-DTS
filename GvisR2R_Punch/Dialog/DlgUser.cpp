// DlgUser.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgUser.h"

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
// CDlgUser dialog


CDlgUser::CDlgUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgUser::~CDlgUser()
{

}

void CDlgUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUser, CDialog)
	//{{AFX_MSG_MAP(CDlgUser)
	ON_BN_CLICKED(IDC_CHK_00, OnChk00)
	ON_BN_CLICKED(IDC_BTN_00, OnBtn00)
	ON_BN_CLICKED(IDC_CHK_02, OnChk02)
	ON_BN_CLICKED(IDC_CHK_01, OnChk01)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUser message handlers

BOOL CDlgUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 24; // 24 point
	lf.lfWeight = FW_EXTRABOLD;
	_stprintf(lf.lfFaceName, _T("%s"), _T("굴림"));	//strcpy(lf.lfFaceName, _T("굴림"));


	m_FontOfListCtrl.CreateFontIndirect(&lf); 
	CListBox* pCtlList = (CListBox*)GetDlgItem(IDC_NAME_LIST);
	pCtlList->SetFont((CFont*)&m_FontOfListCtrl, TRUE);

	m_strOrgUserName = pDoc->WorkingInfo.LastJob.sSelUserName;

	int nFileSize = ReadUserNameListFile();

	DisplayUserNameList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUser::DisplayUserNameList()
{
	CString strName, strNameList;
	int nPos, nRemainStringSize;

	((CListBox*)GetDlgItem(IDC_NAME_LIST))->ResetContent();

	strNameList = pDoc->m_strUserNameList;
	nRemainStringSize = strNameList.GetLength();

	while(nRemainStringSize > 0)
	{
		strName = strNameList;
		nPos = strNameList.Find('\n', 0);
		if(nPos > 0)
		{
			strName = strNameList.Left(nPos);
			strNameList.Delete(0, nPos+1);
			((CListBox*)GetDlgItem(IDC_NAME_LIST))->AddString(strName);
		}
		else
		{	
			if(strNameList.GetLength() > 0)
			{
				strName = strNameList;
				((CListBox*)GetDlgItem(IDC_NAME_LIST))->AddString(strName);
			}

			break;
		}

	}	
	
}

int CDlgUser::ReadUserNameListFile()
{
	//char FileD[200];
	TCHAR FileD[200];
	char *FileData;
	CString strFileData;
	size_t nFileSize, nRSize;
	CString strFileName;
	
	FILE *fp;

	//파일을 불러옴.
	strFileName = _T("C:\\R2RSet\\UserNameList.ini");
	//strcpy(FileD, strFileName);
	_stprintf(FileD, _T("%s"), strFileName);
	char* pRtn = NULL;
	if((fp = fopen(pRtn=TCHARToChar(FileD), "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize+1, sizeof(char));
		
		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		pDoc->m_strUserNameList = strFileData;

		fclose(fp);
		free( FileData );
	}
	else
	{
		pView->MsgBox(_T("파일이 존재하지 않습니다.\r\nC:\\R2RSet\\UserNameList.ini"));
		//AfxMessageBox(_T("파일이 존재하지 않습니다.\r\nC:\\R2RSet\\UserNameList.ini"));
	}

	if(pRtn)
		delete pRtn;
	return (int)nFileSize;
}

void CDlgUser::WriteUserNameListFile()
{
	FILE *fp = NULL;
	//char FileName[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	CString strFileName = _T("C:\\R2RSet\\UserNameList.ini");
	//strcpy(FileName, strFileName);
	_stprintf(FileName, _T("%s"), strFileName);
	char* pRtn = NULL;
	fp = fopen(pRtn=TCHARToChar(FileName), "w+");

	if (fp != NULL)
	{
		fprintf(fp, "%s", pRtn = StringToChar(pDoc->m_strUserNameList));
	}
	else
	{
		pView->MsgBox(_T("It is trouble to open UserNameList.ini"),MB_ICONWARNING|MB_OK);
		//AfxMessageBox(_T("It is trouble to open UserNameList.ini"),MB_ICONWARNING|MB_OK);
	}

	fclose(fp);
	if(pRtn)
		delete pRtn;
}

void CDlgUser::OnChk00() 
{
	// TODO: Add your control notification handler code here
	int nIndex = ((CListBox*)GetDlgItem(IDC_NAME_LIST))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CListBox*)GetDlgItem(IDC_NAME_LIST))->GetText(nIndex, pDoc->WorkingInfo.LastJob.sSelUserName);
	}

	OnOK();
}

void CDlgUser::OnBtn00() 
{
	// TODO: Add your control notification handler code here
	pDoc->WorkingInfo.LastJob.sSelUserName = m_strOrgUserName;
	OnOK();
}

void CDlgUser::OnChk02() 
{
	// TODO: Add your control notification handler code here
	CString strUserName;
	int nPos, nSize;

	GetDlgItem(IDC_EDIT_ADD_USER_NAEM)->GetWindowText(strUserName);

	nSize = pDoc->m_strUserNameList.GetLength();
	nPos = pDoc->m_strUserNameList.ReverseFind('\n');

	if (nPos + 1 >= nSize)
		pDoc->m_strUserNameList += strUserName;
	else if (nSize > 0)
		//pDoc->m_strUserNameList.Format(_T("\n%s"), strUserName);
		pDoc->m_strUserNameList += _T("\r\n") + strUserName;
	else
		pDoc->m_strUserNameList += strUserName;

	WriteUserNameListFile();
	DisplayUserNameList();
	((CButton*)GetDlgItem(IDC_CHK_02))->SetCheck(FALSE);
}

void CDlgUser::OnChk01() 
{
	// TODO: Add your control notification handler code here
	CString strSelectedUserName, strUserNameList;
	int nPos1, nPos2, nSize;

	strUserNameList = pDoc->m_strUserNameList;
	int nIndex = ((CListBox*)GetDlgItem(IDC_NAME_LIST))->GetCurSel();
	if (nIndex != LB_ERR)
	{
		((CListBox*)GetDlgItem(IDC_NAME_LIST))->GetText(nIndex, strSelectedUserName);
		((CListBox*)GetDlgItem(IDC_NAME_LIST))->DeleteString(nIndex);

		nPos1 = pDoc->m_strUserNameList.Find(strSelectedUserName,0);
		nPos2 = strUserNameList.Find('\n', nPos1);
		nSize = strSelectedUserName.GetLength();
		if(nPos2 > nPos1+nSize+1)
		{				
			strUserNameList.Delete(nPos1, nSize);
		}
		else if(nPos2 > 0)
		{
			strUserNameList.Delete(nPos1, nSize+1);
		}
		else if(nSize > 0)
		{
			strUserNameList.Delete(nPos1, nSize);
		}

		pDoc->m_strUserNameList = strUserNameList;
		WriteUserNameListFile();
	}
	((CButton*)GetDlgItem(IDC_CHK_01))->SetCheck(FALSE);
}

BOOL CDlgUser::PreTranslateMessage(MSG* pMsg) 
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
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
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
