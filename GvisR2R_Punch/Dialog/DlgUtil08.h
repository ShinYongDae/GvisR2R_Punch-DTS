#pragma once


// CDlgUtil08 ��ȭ �����Դϴ�.

class CDlgUtil08 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil08)

	BOOL FileBrowse(CString& sPath);
	CString DispOrderingMkUp(int nSerial);

public:
	CDlgUtil08(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUtil08();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_08 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
