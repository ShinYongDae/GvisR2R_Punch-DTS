#pragma once


// CDlgUtil07 대화 상자입니다.

class CDlgUtil07 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil07)

public:
	CDlgUtil07(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUtil07();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_07 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
