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

	// ���� ���ø� �ֱ� ����.
	bool SetSamplingTimeMsec(int nMsec);

	// MMCE ���۰� ������ ���� ����.
	bool SetRefBufferBlockSize(int nCount);

	// ����Ʈ ��� ���� ������ 1�� ȣ�� �ؾ���. ���� �޸� �� ���� �ʱ�ȭ.
	bool BufferInit();

	// ����Ʈ�� ��� �߰�.
	bool AddBufferMotion2Ax(double fPositionAx0, double fPositionAx1, double fVelocityX);

	// ����Ʈ���� ��� ����.
	bool EditBufferMotion2Ax(int nIndex, double fPositionAx0, double fPositionAx1, double fVelocityX);

	// ���ۿ� �ִ� ����� MMCE �� ����.
	int BufferToMMCE(int nIndex);

	// �޸� ���� ����Ʈ�� ���� ����.
	int ReadBufferCount();

	// MMCE ���ۿ� ž�� ����.
	int ReadMMCEInsertBufferCount();

	// MMC ���� �ܿ� ���� ��ȯ.
	int ReadEmptyBufferCount();

	// �޸� ���ۿ� ���� ������ Ȯ��.
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