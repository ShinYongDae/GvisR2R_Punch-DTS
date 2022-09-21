// DlgUtil08.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgUtil08.h"
#include "afxdialogex.h"


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

// CDlgUtil08 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUtil08, CDialog)

CDlgUtil08::CDlgUtil08(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_UTIL_08, pParent)
{

}

CDlgUtil08::~CDlgUtil08()
{
}

void CDlgUtil08::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUtil08, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgUtil08::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgUtil08 메시지 처리기입니다.


void CDlgUtil08::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sPath;
	int nPos, nSerial;

	if (!FileBrowse(sPath))
		return;
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(sPath);

	CString sTemp, sSerial;
	nPos = sPath.ReverseFind('.');
	sTemp = sPath.Left(nPos);
	nPos = sPath.ReverseFind('\\');
	sSerial = sTemp.Right(nPos + 1);
	nSerial = _tstoi(sSerial);

	if (pView->LoadPcrUp(nSerial))				// Default: From Buffer, TRUE: From Share
	{
		pView->OrderingMkUp(nSerial, TRUE);
		DispOrderingMkUp(nSerial);
	}

	//if (pView->LoadPcrDn(nSerial))				// Default: From Buffer, TRUE: From Share
	//{
	//	pView->OrderingMkDn(nSerial);
	//}

	GetDlgItem(IDC_EDIT1)->SetWindowText(DispOrderingMkUp(nSerial));

}

BOOL CDlgUtil08::FileBrowse(CString& sPath)
{
	sPath = _T("");

	/* Load from file */
	CString FilePath;
	CFileFind finder;
	CString SrchPath, strTitleMsg, strErrMsg;

	CWaitCursor mCursor;

	CString DirPath[10];
	CString strWorkDir;
	BOOL bResult;

	CString strMcNum;
	int nAoiMachineNum = 0;

	// File Open Filter 
	static TCHAR BASED_CODE szFilter[] = _T("Mst Files (*.mst)|*.mst|All Files (*.*)|*.*||");

	// CFileDialog 
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);

	// Win2k Style FileDialog Box
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12; // +12를 Win2k Style로 다이얼로그 박스가 Open됨.

													   // Open Directory
	TCHAR strPrevDir[MAX_PATH];
	DWORD dwLength = GetCurrentDirectory(MAX_PATH, strPrevDir);
	strWorkDir = strPrevDir;
	dlg.m_ofn.lpstrInitialDir = strWorkDir;

	bResult = 0;

	// Dialog Open
	if (dlg.DoModal() == IDOK)
	{
		sPath = FilePath = dlg.GetPathName();
		return TRUE;
	}

	return FALSE;
}

CString CDlgUtil08::DispOrderingMkUp(int nSerial)
{
	int i, nIdx, nTotDef;
	CString str, sData = _T("");

	if (pDoc->WorkingInfo.System.bStripPcsRgnBin)
	{
		nIdx = pDoc->GetPcrIdx0(nSerial);													// 릴맵화면버퍼 인덱스
		nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;											// 상면 불량 피스 수

		for (i = 0; i < nTotDef; i++)														// 상면 불량 피스 수
		{
			str.Format(_T("%d"), pDoc->m_pPcr[0][nIdx]->m_pDefPcsMk[i]);
			sData += str;
			sData += _T("\r\n");
		}
	}

	return sData;
}