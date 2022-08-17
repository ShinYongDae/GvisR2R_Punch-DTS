
// DlgClient.h : 헤더 파일
//

#pragma once

#include "../Device/TcpIpClient.h"
#include <afxwin.h>


// CDlgClient 대화 상자
class CDlgClient : public CDialog
{
	CString	m_strPortSvr, m_strAddrSvr;
	CString	m_strAddrCli;
	CTcpIpClient* m_pClient;
	int m_nServerID;

	void StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr);
	void StopClient();

// 생성입니다.
public:
	CDlgClient(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CDlgClient();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CLIENT };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT wmClientReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT wmClientClosed(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnClear();
	CEdit m_editMsgList;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedCheckSr1000w();
	afx_msg void OnBnClickedCheckEngrave();
};
