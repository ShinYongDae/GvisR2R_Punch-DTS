#pragma once


// CDlgUtil08 대화 상자입니다.

class CDlgUtil08 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil08)

	BOOL FileBrowse(CString& sPath);
	CString DispOrderingMkUp(int nSerial);

public:
	CDlgUtil08(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUtil08();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_08 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
