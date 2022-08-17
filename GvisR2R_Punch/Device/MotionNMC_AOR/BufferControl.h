#pragma once


// CBufferControl
#ifdef USE_NMC
typedef struct _BUFFER_DATA_
{
	double fPosAx0;
	double fPosAx1;
	double fConstantVel;
}BUFFER_DATA;

#define MAX_BUFFER_DATA 1000

class CBufferControl : public CWnd
{
	DECLARE_DYNAMIC(CBufferControl)

public:
	//CBufferControl();
	CBufferControl(int nBoardID, int nGroupID, int nAxisID_0, int nAxisID_1);
	virtual ~CBufferControl();

	BUFFER_DATA m_arrBufferData[MAX_BUFFER_DATA];
	int m_nBufferCnt;
	int m_nMMCEInsertBufferCount;

	int m_nBoardID;
	int m_nGroupID;
	int m_nAxis0;
	int m_nAxis1;

	double m_fLastTargetPosAx0;
	double m_fLastTargetPosAx1;

	double m_fStartPosAx0;
	double m_fStartPosAx1;

	int m_nGroupMode;

	int m_nSamplingTimeMsec;
	int m_nRefBufferBlockSize;

	int m_nAddListIndex;


	bool StartMotion();
	bool StopMotion();

	bool ReadListMotionRunning();

	// 버퍼 샘플링 주기 설정.
	bool SetSamplingTimeMsec(int nMsec);

	// MMCE 버퍼가 유지할 갯수 설정.
	bool SetRefBufferBlockSize(int nCount);

	// 리스트 모션 동작 직전에 1번 호출 해야함. 버퍼 메모리 및 변수 초기화.
	bool BufferInit();

	// 리스트에 모션 추가.
	bool AddBufferMotion2Ax(double fPositionAx0, double fPositionAx1, double fVelocityX);

	// 리스트버퍼 모션 수정.
	bool EditBufferMotion2Ax(int nIndex, double fPositionAx0, double fPositionAx1, double fVelocityX);

	// 버퍼에 있는 모션을 MMCE 로 전달.
	int BufferToMMCE(int nIndex);

	// 메모리 버퍼 리스트에 남은 갯수.
	int ReadBufferCount();

	// MMCE 버퍼에 탑재 갯수.
	int ReadMMCEInsertBufferCount();

	// MMC 버퍼 잔여 갯수 반환.
	int ReadEmptyBufferCount();

	// 메모리 버퍼에 쌓은 데이터 확인.
	bool ReadBufferData(int nIndex, BUFFER_DATA* pData);

	//-----------------
	int UngroupAllAxes(int nBdNum);
	int Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY);
	int Grouping2Ax();
	int UnGroup2Ax();
	int UnGroup2Ax(int nBdNum, int nGroupNum);
	int GroupAmpOn();
	int GroupAmpOff();
	int GroupAmpReset();
	bool IsGroupStatusStandby();
	bool IsGroupStatusError();
	bool IsGroupStatusMoving();
	

protected:
	DECLARE_MESSAGE_MAP()
};


#endif