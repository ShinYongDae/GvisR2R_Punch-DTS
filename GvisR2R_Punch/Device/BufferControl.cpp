// BufferControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "GroupBlendControl.h"
#include "BufferControl.h"

#include <math.h>
#include "NMC_Motion.h"

#define GROUP_MODE_READY 0

#ifdef USE_NMC

CBufferControl* g_pBufferControl = NULL;

// CBufferControl

IMPLEMENT_DYNAMIC(CBufferControl, CWnd)

/*
// 반드시 그룹 생성은 보드번호, 그룹번호, 축번호를 기반으로 생성되도록 이 생성자는 생략함.
CBufferControl::CBufferControl()
{
	g_pBufferControl = this;
	m_nGroupMode = -1;
	memset(m_arrBufferData, 0, sizeof(BUFFER_DATA)*MAX_BUFFER_DATA);
	m_nBufferCnt = 0;
	m_nMMCEInsertBufferCount = 0;
	m_nSamplingTimeMsec = 100;
	m_nRefBufferBlockSize = 30;
}
*/

CBufferControl::CBufferControl(int nBoardID, int nGroupID, int nAxisID_0, int nAxisID_1)
{
	g_pBufferControl = this;
	m_nGroupMode = -1;
	memset(m_arrBufferData, 0, sizeof(BUFFER_DATA)*MAX_BUFFER_DATA);
	m_nBufferCnt = 0;
	m_nMMCEInsertBufferCount = 0;
	m_nSamplingTimeMsec = 5;
	m_nRefBufferBlockSize = 30;

	m_nBoardID = nBoardID;
	m_nAxis0 = nAxisID_0;
	m_nAxis1 = nAxisID_1;

	m_nAddListIndex = 0;
}

CBufferControl::~CBufferControl()
{
}


BEGIN_MESSAGE_MAP(CBufferControl, CWnd)
END_MESSAGE_MAP()



// CBufferControl 메시지 처리기입니다.


bool g_bThreadListMotionRun = false;
void _thread_ListMotionRun(LPVOID pArg);
void _thread_ListMotionRun(LPVOID pArg)
{
	int nStep = 0;
	int nAddCount = 0;
	
	int EmptyDataCount = 0; 

	while (g_bThreadListMotionRun)
	{
		EmptyDataCount = g_pBufferControl->ReadMMCEInsertBufferCount() - g_pBufferControl->ReadActiveBufferCount();

		if( EmptyDataCount < g_pBufferControl->m_nRefBufferBlockSize )
		{
			nAddCount = g_pBufferControl->m_nRefBufferBlockSize  - EmptyDataCount;

			//위에서 계산한 갯수 만큼 집어 넣음.
			for(int i = 0 ; i < nAddCount ; i++)
			{
				//   MMCE 에 블렌딩 옵션으로 모션명령 들어간 갯수 < AddBufferMotion2Ax 함수로 메모리 배열에 쌓은 갯수..
				if ( g_pBufferControl->ReadMMCEInsertBufferCount() < g_pBufferControl->ReadBufferCount() )
				{
					g_pBufferControl->BufferToMMCE( g_pBufferControl->ReadMMCEInsertBufferCount() );
					
				} 
				else
				{
					if (g_pBufferControl->IsAxisStatusError() == true)
					{
						g_bThreadListMotionRun = false;
						return;
					}
				}
					
			}
		}
		//-------------------------
		Sleep(g_pBufferControl->m_nSamplingTimeMsec);
	}
}

bool CBufferControl::ReadListMotionRunning()
{
	return g_bThreadListMotionRun;
}

bool CBufferControl::StartMotion()
{
	BufferInit();
	if (g_bThreadListMotionRun == true)
	{
		return false;
	} 
	else
	{
		g_bThreadListMotionRun = true;
		_beginthread(_thread_ListMotionRun, NULL, NULL);
		return true;
	}
	return true;
}

bool CBufferControl::StopMotion()
{
	g_bThreadListMotionRun = false;
	return true;
}

bool CBufferControl::SetSamplingTimeMsec(int nMsec)
{
	m_nSamplingTimeMsec = nMsec;
	return true;
}
bool CBufferControl::SetRefBufferBlockSize(int nCount)
{
	if (nCount >= 998 || nCount < 1)
	{
		return false;
	}
	else
	{
		m_nRefBufferBlockSize = nCount;
		return true;
	}
	return true;	
}


bool CBufferControl::BufferInit()
{
	MC_STATUS ms = MC_OK;

	memset(m_arrBufferData, 0, sizeof(BUFFER_DATA)*MAX_BUFFER_DATA);
	m_nBufferCnt = 0;
	m_nMMCEInsertBufferCount = 0;
	UINT16 nIndex = 0;
	m_nAddListIndex = 0;
	ms = MC_RTAFListIndex(m_nBoardID, m_nAxis1, 1, &nIndex);
	if(ms != MC_OK) return false;

	return true;
}

bool CBufferControl::AddBufferMotion2Ax(double StartYPos, double YPosition, double ZPosition)
{
	m_arrBufferData[m_nBufferCnt].fYPosition = YPosition;
	m_arrBufferData[m_nBufferCnt].fZPosition = ZPosition;
	m_arrBufferData[m_nBufferCnt].fStartYPosition = StartYPos;
	m_nBufferCnt++;

	if (m_nBufferCnt >= MAX_BUFFER_DATA)
		m_nBufferCnt = 0;

	//-------------
	return true;
}

bool CBufferControl::EditBufferMotion2Ax(int nIndex, double StartYPos, double YPosition, double ZPosition)
{
	if (nIndex >= m_nBufferCnt)
	{
		return false;
	}

	m_arrBufferData[nIndex].fYPosition = YPosition;
	m_arrBufferData[nIndex].fZPosition = ZPosition;
	m_arrBufferData[nIndex].fStartYPosition = StartYPos;

	return true;
}

int CBufferControl::BufferToMMCE(int nIndex)
{
	if (nIndex >= m_nBufferCnt)
	{
		return -1;
	}

	MC_STATUS ms;
	double fCurPos = 0;


	if(nIndex == 0)
	{
		ms = MC_ReadCommandedPosition(m_nBoardID, m_nAxis1, &fCurPos);
		if (ms != MC_OK) return ms;

		ms = MC_RTAFPositionSync(m_nBoardID, m_nAxis1, fCurPos, m_arrBufferData[nIndex].fZPosition, 
			m_nAxis0, m_arrBufferData[nIndex].fStartYPosition, m_arrBufferData[nIndex].fYPosition, mcAborting);
		m_nMMCEInsertBufferCount++;
	}
	else
	{
		ms = MC_RTAFPositionSync(m_nBoardID, m_nAxis1, m_arrBufferData[nIndex-1].fZPosition, m_arrBufferData[nIndex].fZPosition, 
			m_nAxis0, m_arrBufferData[nIndex].fStartYPosition, m_arrBufferData[nIndex].fYPosition, mcBuffered);
		m_nMMCEInsertBufferCount++;
	}


	return ms;
}

int CBufferControl::ReadBufferCount()
{
	return m_nBufferCnt;
}

int CBufferControl::ReadMMCEInsertBufferCount()
{
	return m_nMMCEInsertBufferCount;
}

int CBufferControl::ReadEmptyBufferCount()
{
	int nCnt = ReadActiveBufferCount();
	return (1000-nCnt);
}

int CBufferControl::ReadActiveBufferCount()
{
	MC_STATUS ms;
	UINT16 nIndex = 0;
	ms = MC_RTAFListIndex(m_nBoardID, m_nAxis1, 0, &nIndex);
	if (ms == MC_OK)
	{
		return (nIndex);				
	}
	else
	{
		return 0;
	}
	return 0;
}

bool CBufferControl::ReadBufferData(int nIndex, BUFFER_DATA* pData)
{
	BUFFER_DATA stTemp;

	if (nIndex >= m_nBufferCnt)
	{
		pData = NULL;
		return false;
	}
	else
	{
		stTemp.fStartYPosition = m_arrBufferData[nIndex].fStartYPosition;
		stTemp.fYPosition = m_arrBufferData[nIndex].fYPosition;
		stTemp.fZPosition = m_arrBufferData[nIndex].fZPosition;
		
		*pData = stTemp;
	}
	return true;
}

//-------------------------------------

//int CBufferControl::Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY)
int CBufferControl::Grouping2Ax()
{
	MC_STATUS ms;

	//m_nBoardID = nBdNum;
	//m_nGroupID = nGroupNum;

	//m_nAxis0 = nAxisNumX;
	//m_nAxis1 = nAxisNumY;

	ms = MC_GroupDisable(m_nBoardID, m_nGroupID);
	ms = MC_UngroupAllAxes(m_nBoardID, m_nGroupID);
	Sleep(5);

	ms = MC_AddAxisToGroup(m_nBoardID, m_nAxis0, m_nGroupID, 0);
	ms = MC_AddAxisToGroup(m_nBoardID, m_nAxis1, m_nGroupID, 1);

	ms = MC_GroupEnable(m_nBoardID, m_nGroupID);
	m_nAddListIndex = 0;

	return ms;
}
int CBufferControl::UnGroup2Ax()
{
	MC_STATUS ms;

	ms = MC_GroupDisable(m_nBoardID, m_nGroupID);
	ms = MC_UngroupAllAxes(m_nBoardID, m_nGroupID);
	Sleep(5);

	return ms;
}

int CBufferControl::AmpOn()
{
	MC_STATUS ms;
	ms = MC_Power(m_nBoardID, m_nAxis0, true);
	ms = MC_Power(m_nBoardID, m_nAxis1, true);
	return ms;
}
int CBufferControl::AmpOff()
{
	MC_STATUS ms;
	ms = MC_Power(m_nBoardID, m_nAxis0, false);
	ms = MC_Power(m_nBoardID, m_nAxis1, false);
	return ms;
}
int CBufferControl::AmpReset()
{
	MC_STATUS ms;
	ms = MC_Reset(m_nBoardID, m_nAxis0);
	ms = MC_Reset(m_nBoardID, m_nAxis1);
	return ms;
}

bool CBufferControl::IsAxisStatusStandby()
{
	UINT32 nAxisStatus = 0;
	MC_STATUS ms;

	ms = MC_ReadAxisStatus(m_nBoardID, m_nGroupID, &nAxisStatus);

	if (ms == MC_OK)
	{
		if (nAxisStatus & GroupStandby)
		{
			return true;
		}
		else
		{
			return false;
		}
	} 
	else
	{
		return false;
	}
	return false;
}

bool CBufferControl::IsAxisStatusError()
{
	UINT32 nAxisStatus = 0;
	MC_STATUS ms;

	ms = MC_ReadAxisStatus(m_nBoardID, m_nAxis1, &nAxisStatus);

	if (ms == MC_OK)
	{
		if (nAxisStatus & mcErrorStop )
		{
			return true;
		}
		else
		{
			return false;
		}
	} 
	else
	{
		return false;
	}
	return false;
}

bool CBufferControl::IsAxisStatusMoving()
{
	UINT32 nGroupStatus = 0;
	MC_STATUS ms;

	ms = MC_GroupReadStatus(m_nBoardID, m_nGroupID, &nGroupStatus);

	if (ms == MC_OK)
	{
		if (nGroupStatus & GroupMoving )
		{
			return true;
		}
		else
		{
			return false;
		}
	} 
	else
	{
		return false;
	}
	return false;
}

#endif