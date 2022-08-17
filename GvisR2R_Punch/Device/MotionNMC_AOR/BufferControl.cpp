// BufferControl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BufferControl.h"

#ifdef USE_NMC

#include <math.h>

#define GROUP_MODE_READY 0

CBufferControl* g_pBufferControl = NULL;

// CBufferControl

IMPLEMENT_DYNAMIC(CBufferControl, CWnd)

/*
// �ݵ�� �׷� ������ �����ȣ, �׷��ȣ, ���ȣ�� ������� �����ǵ��� �� �����ڴ� ������.
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
	m_nSamplingTimeMsec = 100;
	m_nRefBufferBlockSize = 30;

	m_nBoardID = nBoardID;
	m_nGroupID = nGroupID;
	m_nAxis0 = nAxisID_0;
	m_nAxis1 = nAxisID_1;

	m_nAddListIndex = 0;
}

CBufferControl::~CBufferControl()
{
}


BEGIN_MESSAGE_MAP(CBufferControl, CWnd)
END_MESSAGE_MAP()



// CBufferControl �޽��� ó�����Դϴ�.


bool g_bThreadListMotionRun = false;
void _thread_ListMotionRun(LPVOID pArg);
void _thread_ListMotionRun(LPVOID pArg)
{
	int nStep = 0;
	int nAddCount = 0;

	while (g_bThreadListMotionRun)
	{
		//-------------------------				
        //          MMCE ���� ���� ����.                <          ��ϴ��� �ʱ� �� 30��
		if ( g_pBufferControl->ReadEmptyBufferCount() < g_pBufferControl->m_nRefBufferBlockSize )
		{
			//��Ͽ��� MMCE ���� �������� ���� ��ŭ �� ����ֱ� ���� ���� ���.
			nAddCount = g_pBufferControl->m_nRefBufferBlockSize - g_pBufferControl->ReadEmptyBufferCount();
			//������ ����� ���� ��ŭ ���� ����.
			for(int i = 0 ; i < nAddCount ; i++)
			{
				//   MMCE �� ���� �ɼ����� ��Ǹ�� �� ���� < AddBufferMotion2Ax �Լ��� �޸� �迭�� ���� ����..
				if ( g_pBufferControl->ReadMMCEInsertBufferCount() < g_pBufferControl->ReadBufferCount() )
				{
					g_pBufferControl->BufferToMMCE( g_pBufferControl->ReadMMCEInsertBufferCount() );
					
					// Ȥ�ö� ù��° ��Ǹ�ɿ��� �ð��� ������ �߻��� ���� �߰� ����ó��.
					if ( g_pBufferControl->ReadMMCEInsertBufferCount() == 1 )
					{
						Sleep(20);
					}
				} 
				else
				{
					if (g_pBufferControl->IsGroupStatusStandby() == true )
					{
						g_bThreadListMotionRun = false;
						return;
					}
					else if (g_pBufferControl->IsGroupStatusError() == true)
					{
						g_bThreadListMotionRun = false;
						return;
					}
					else if (g_pBufferControl->IsGroupStatusMoving() == false)
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
	m_nGroupMode = -1;

	MC_STATUS ms;
	UINT32 nAxisStatus0 = 0;
	UINT32 nAxisStatus1 = 0;
	double fCurrentPosAx0 = 0;
	double fCurrentPosAx1 = 0;

	memset(m_arrBufferData, 0, sizeof(BUFFER_DATA)*MAX_BUFFER_DATA);
	m_nBufferCnt = 0;
	m_nMMCEInsertBufferCount = 0;
	///////////////////////////////////////////////
	// �ʱ�ȭ �� ��ǥ ���� ���� 0����..
	ms = MC_ReadAxisStatus(m_nBoardID, m_nAxis0, &nAxisStatus0);
	if (ms == MC_OK)
	{
		if (nAxisStatus0 & mcStandStill)
		{
			ms = MC_ReadCommandedPosition(m_nBoardID, m_nAxis0, &fCurrentPosAx0);
		}
		else if (nAxisStatus0 & mcDisabled)
		{
			ms = MC_ReadActualPosition(m_nBoardID, m_nAxis0, &fCurrentPosAx0);
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
	///////////////////////////////////////////////
	// �ʱ�ȭ �� ��ǥ ���� ���� 1����..
	ms = MC_ReadAxisStatus(m_nBoardID, m_nAxis1, &nAxisStatus1);
	if (ms == MC_OK)
	{
		if (nAxisStatus1 & mcStandStill)
		{
			ms = MC_ReadCommandedPosition(m_nBoardID, m_nAxis1, &fCurrentPosAx1);
		}
		else if (nAxisStatus1 & mcDisabled)
		{
			ms = MC_ReadActualPosition(m_nBoardID, m_nAxis1, &fCurrentPosAx1);
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

	///////////////////////
	//
	m_fLastTargetPosAx0 = fCurrentPosAx0;
	m_fLastTargetPosAx1 = fCurrentPosAx1;

	m_fStartPosAx0 = fCurrentPosAx0;
	m_fStartPosAx1 = fCurrentPosAx1;

	m_nGroupMode = GROUP_MODE_READY;

	return true;
}

bool CBufferControl::AddBufferMotion2Ax(double fPositionAx0, double fPositionAx1, double fVelocityX)
{
	double fConstantVelAx = 0;

	double fVectorPosX = fabs(fPositionAx0 - m_fLastTargetPosAx0);
	double fVectorPosY = fabs(fPositionAx1 - m_fLastTargetPosAx1);

	if (fVectorPosX == 0 && fVectorPosY == 0)
	{
		return false;
	}

	m_fLastTargetPosAx0 = fPositionAx0;
	m_fLastTargetPosAx1 = fPositionAx1;

	fConstantVelAx = sqrt( (fVelocityX*fVelocityX) + ((fVectorPosY/(fVectorPosX/fVelocityX))*(fVectorPosY/(fVectorPosX/fVelocityX))) );
	fConstantVelAx = fabs(fConstantVelAx);

	////////////////
	// ���ۿ� �߰�...
	m_arrBufferData[m_nBufferCnt].fPosAx0 = fPositionAx0;
	m_arrBufferData[m_nBufferCnt].fPosAx1 = fPositionAx1;
	m_arrBufferData[m_nBufferCnt].fConstantVel = fConstantVelAx;
	m_nBufferCnt++;
	//-------------

	return true;
}

bool CBufferControl::EditBufferMotion2Ax(int nIndex, double fPositionAx0, double fPositionAx1, double fVelocityX)
{
	if (nIndex >= m_nBufferCnt)
	{
		return false;
	}

	double fBeforePosAx0 = 0;
	double fBeforePosAx1 = 0;
	
	if (nIndex == 0)
	{
		fBeforePosAx0 = m_fStartPosAx0;
		fBeforePosAx1 = m_fStartPosAx1;
	}
	else
	{
		fBeforePosAx0 = m_arrBufferData[nIndex-1].fPosAx0;
		fBeforePosAx1 = m_arrBufferData[nIndex-1].fPosAx1;
	}

	double fConstantVelAx = 0;

	double fVectorPosX = fabs(fPositionAx0 - fBeforePosAx0);
	double fVectorPosY = fabs(fPositionAx1 - fBeforePosAx1);

	fConstantVelAx = sqrt( (fVelocityX*fVelocityX) + ((fVectorPosY/(fVectorPosX/fVelocityX))*(fVectorPosY/(fVectorPosX/fVelocityX))) );
	fConstantVelAx = fabs(fConstantVelAx);

	m_arrBufferData[nIndex].fPosAx0 = fPositionAx0;
	m_arrBufferData[nIndex].fPosAx1 = fPositionAx1;
	m_arrBufferData[nIndex].fConstantVel = fConstantVelAx;

	//---------------
	// �޺κ� ������ �ӵ� ó��..
	if (nIndex < m_nBufferCnt-1 )
	{
		double fNextPosAx0 = 0;
		double fNextPosAx1 = 0;
		double fNextVel = 0;

		fVectorPosX = fabs(m_arrBufferData[nIndex+1].fPosAx0 - m_arrBufferData[nIndex].fPosAx0);
		fVectorPosY = fabs(m_arrBufferData[nIndex+1].fPosAx1 - m_arrBufferData[nIndex].fPosAx1);
		fNextVel = m_arrBufferData[nIndex+1].fConstantVel;

		fConstantVelAx = sqrt( (fNextVel*fNextVel) + ((fVectorPosY/(fVectorPosX/fNextVel))*(fVectorPosY/(fVectorPosX/fNextVel))) );
		fConstantVelAx = fabs(fConstantVelAx);
		m_arrBufferData[nIndex+1].fConstantVel = fConstantVelAx;
	}

	return true;
}

int CBufferControl::BufferToMMCE(int nIndex)
{
	if (nIndex >= m_nBufferCnt)
	{
		return -1;
	}

	MC_STATUS ms;
	UINT32 nGroupStatus = 0;
	double dPara = 100;

	double fArrPos[2] = { m_arrBufferData[nIndex].fPosAx0, m_arrBufferData[nIndex].fPosAx1 };

	ms = MC_GroupReadStatus(m_nBoardID, m_nGroupID, &nGroupStatus);

	// GroupStandby : �׷��ε� ���¿��� ���� �������� �ʰ� ��� ���� ����.....
	if (nGroupStatus & GroupStandby) // when from stop to start... 
	{
		ms = MC_MoveLinearAbsolute(m_nBoardID, m_nGroupID, 2, fArrPos,
			m_arrBufferData[nIndex].fConstantVel, m_arrBufferData[nIndex].fConstantVel*1000, m_arrBufferData[nIndex].fConstantVel*1000, 0,
			mcACS, mcAborting, mcTMConstantVelocity, 1, &dPara);
		if (ms != MC_OK)
		{
			AfxMessageBox(_T("Error-MC_MoveLinearAbsolute."));
			return ms;
		}

		m_nMMCEInsertBufferCount++;
		Sleep(10);
	} 
	else // when on moving... 
	{
		ms = MC_MoveLinearAbsolute(m_nBoardID, m_nGroupID, 2, fArrPos, 
			m_arrBufferData[nIndex].fConstantVel, m_arrBufferData[nIndex].fConstantVel*1000, m_arrBufferData[nIndex].fConstantVel*1000, 0,
			mcACS, mcBufferedBlendingNext, mcTMConstantVelocity, 1, &dPara);
		if (ms != MC_OK)
		{
			AfxMessageBox(_T("Error-MC_MoveLinearAbsolute."));
			return ms;
		}
		m_nMMCEInsertBufferCount++;
		Sleep(10);
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
	MC_STATUS ms;
	UINT32 nCnt = 0;
	ms = MC_GroupReadRemainBuffer(m_nBoardID, m_nGroupID, &nCnt);
	if (ms == MC_OK)
	{
		if ( nCnt == 1000)
		{
			return 1000-nCnt;
		}
		else
		{
			return 999-nCnt;
		}
		
	}
	else
	{
		return 99999;
	}
	return 99999;
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
		stTemp.fPosAx0 = m_arrBufferData[nIndex].fPosAx0;
		stTemp.fPosAx1 = m_arrBufferData[nIndex].fPosAx1;
		stTemp.fConstantVel = m_arrBufferData[nIndex].fConstantVel;
		
		*pData = stTemp;
	}
	return true;
}

//-------------------------------------

int CBufferControl::UngroupAllAxes(int nBdNum)
{
	MC_STATUS ms;
	UINT16 i;

	for (i = 0; i < NMC_GROUPID_TOTAL; i++)
	{
		ms = MC_UngroupAllAxes(nBdNum, i);
	}

	return ms;
}

int CBufferControl::Grouping2Ax(int nBdNum, int nGroupNum, int nAxisNumX, int nAxisNumY)
{
	MC_STATUS ms;

	m_nBoardID = nBdNum;
	m_nGroupID = nGroupNum;

	m_nAxis0 = nAxisNumX;
	m_nAxis1 = nAxisNumY;

	ms = MC_GroupDisable(m_nBoardID, m_nGroupID);
	//ms = MC_UngroupAllAxes(m_nBoardID, m_nGroupID);
	//if(m_nGroupID == NMC_GROUPID_INTERPOLRATION)
	//	ms = MC_UngroupAllAxes(m_nBoardID, NMC_GROUPID_RTAF);
	//else if(m_nGroupID == NMC_GROUPID_RTAF)
	//	ms = MC_UngroupAllAxes(m_nBoardID, NMC_GROUPID_INTERPOLRATION);
	UngroupAllAxes(m_nBoardID);

	Sleep(5);

	ms = MC_AddAxisToGroup(m_nBoardID, m_nAxis0, m_nGroupID, 0);
	ms = MC_AddAxisToGroup(m_nBoardID, m_nAxis1, m_nGroupID, 1);

	ms = MC_GroupEnable(m_nBoardID, m_nGroupID);
	m_nAddListIndex = 0;

	return ms;
}

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

int CBufferControl::UnGroup2Ax(int nBdNum, int nGroupNum)
{
	MC_STATUS ms;

	ms = MC_GroupDisable(nBdNum, nGroupNum);
	ms = MC_UngroupAllAxes(nBdNum, nGroupNum);
	Sleep(5);

	return ms;
}

int CBufferControl::GroupAmpOn()
{
	MC_STATUS ms;
	ms = MC_Power(m_nBoardID, m_nAxis0, true);
	ms = MC_Power(m_nBoardID, m_nAxis1, true);
	return ms;
}
int CBufferControl::GroupAmpOff()
{
	MC_STATUS ms;
	ms = MC_Power(m_nBoardID, m_nAxis0, false);
	ms = MC_Power(m_nBoardID, m_nAxis1, false);
	return ms;
}
int CBufferControl::GroupAmpReset()
{
	MC_STATUS ms;
	ms = MC_GroupReset(m_nBoardID, m_nGroupID);
	ms = MC_Reset(m_nBoardID, m_nAxis0);
	ms = MC_Reset(m_nBoardID, m_nAxis1);
	return ms;
}

bool CBufferControl::IsGroupStatusStandby()
{
	UINT32 nGroupStatus = 0;
	MC_STATUS ms;

	ms = MC_GroupReadStatus(m_nBoardID, m_nGroupID, &nGroupStatus);

	if (ms == MC_OK)
	{
		if (nGroupStatus & GroupStandby)
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

bool CBufferControl::IsGroupStatusError()
{
	UINT32 nGroupStatus = 0;
	MC_STATUS ms;

	ms = MC_GroupReadStatus(m_nBoardID, m_nGroupID, &nGroupStatus);

	if (ms == MC_OK)
	{
		if (nGroupStatus & GroupErrorStop )
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

bool CBufferControl::IsGroupStatusMoving()
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