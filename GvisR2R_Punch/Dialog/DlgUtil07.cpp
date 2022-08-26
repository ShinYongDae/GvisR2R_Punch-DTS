// DlgUtil07.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgUtil07.h"
#include "afxdialogex.h"


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

// CDlgUtil07 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUtil07, CDialog)

CDlgUtil07::CDlgUtil07(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_UTIL_07, pParent)
{

}

CDlgUtil07::~CDlgUtil07()
{
}

void CDlgUtil07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUtil07, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgUtil07::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgUtil07::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgUtil07 메시지 처리기입니다.


void CDlgUtil07::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sQuery, sDisp=_T("");

	GetDlgItem(IDC_EDIT2)->GetWindowText(sQuery);
	if (sQuery.IsEmpty())
		return;

	if(pView && pView->m_pDts)
	{
		int nTotRow=0, nTotCol=0;
		CStringArray sData;
		if(pView->m_pDts->Execute(sQuery, sData, nTotRow, nTotCol) > 0)
		{
			int nCount = sData.GetSize();
			if (nCount > 0)
			{
				CString sRecord;
				for (int i = 0; i < nTotRow; i++)
				{
					for (int j = 0; j < nTotCol; j++)
					{
						sRecord = sData.GetAt(j);
						//sData.RemoveAt(j);
						sDisp += sRecord;
						sDisp += _T("\t");
					}
					sDisp += _T("\r\n");
				}

				GetDlgItem(IDC_EDIT1)->SetWindowText(sDisp);
				sData.RemoveAll();
			}

		}
	}
}


void CDlgUtil07::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
}
