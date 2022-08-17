
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainFrame : public CFrameWnd
{
	int m_ProgressID;
	BOOL m_bProgressCreated;
	CProgressCtrl m_Progress;

	BOOL m_bLockDispStsBar;
	CString m_sDispMsg[10];

protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	CCriticalSection m_cs;
	CStringArray m_strStatusMsgArray;

// 작업입니다.
public:
	void DispStatusBar(CString strMsg, int nStatusBarID);
	void CreateProgressBar();
	void IncProgress(int nVal = 0);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();
	afx_msg void OnDatabaseConnection();
	afx_msg void OnTestIo();
	afx_msg void OnAppMinimize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};


