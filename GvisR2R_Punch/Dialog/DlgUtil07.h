#pragma once


// CDlgUtil07 ��ȭ �����Դϴ�.

class CDlgUtil07 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil07)

public:
	CDlgUtil07(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUtil07();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_07 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
