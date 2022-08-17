// Motion.cpp : implementation file
//

#include "stdafx.h"
#include "Motion.h"
#include <math.h>
#include "stdafx.h"
#include "Controller.h"
#include "GvisAORDoc.h"
#include "GvisAORView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMotion

CMotion::CMotion(CWnd* pParent,CString strFilePath)
{
#ifdef USE_ZMP
	m_pMotionOld = new CMotionOld(pParent, strFilePath);
#else
#ifdef USE_NMC
	m_pMotionNew = new CMotionNew(pParent, strFilePath);
#endif
#endif

}

CMotion::~CMotion()
{
#ifdef USE_ZMP
	if (m_pMotionOld)
	{
		delete m_pMotionOld;
		m_pMotionOld = NULL;
	}
#else
#ifdef USE_NMC
	if (m_pMotionNew)
	{
		delete m_pMotionNew;
		m_pMotionNew = NULL;
	}
#endif
#endif

}

BOOL CMotion::InitXmpBoard()
{
#ifdef USE_ZMP
	return m_pMotionOld->InitXmpBoard();
#else
#ifdef USE_NMC
	return m_pMotionNew->InitNmcBoard();
#endif
#endif
}

BOOL CMotion::AmpReset(int nAxisId)
{
	//return Clear(nAxisId);


#ifdef USE_ZMP
	return m_pMotionOld->AmpReset(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->AmpReset(nAxisId);
#endif
#endif


}

BOOL CMotion::ServoOnOff(int nAxisId,BOOL bOnOff)
{
	//return m_pMotionCard->m_pMotor[nAxisId].Enable(bOnOff);


#ifdef USE_ZMP
	return m_pMotionOld->ServoOnOff(nAxisId, bOnOff);
#else
#ifdef USE_NMC
	return m_pMotionNew->ServoOnOff(nAxisId, bOnOff);
#endif
#endif

}

BOOL CMotion::SearchHome()
{
#ifdef USE_ZMP
	return m_pMotionOld->SearchHome();
#else
#ifdef USE_NMC
	return m_pMotionNew->SearchHome();
#endif
#endif
}

BOOL CMotion::SearchHomePos(int nMotionId, BOOL bThread)
{
#ifdef USE_ZMP
	return m_pMotionOld->SearchHomePos(nMotionId, bThread);
#else
#ifdef USE_NMC
	return m_pMotionNew->SearchHomePos(nMotionId, bThread);
#endif
#endif
}


BOOL CMotion::IsHomeDone()
{
	//return (m_pMotionCard->IsHomeDone());

#ifdef USE_ZMP
	return m_pMotionOld->IsHomeDone();
#else
#ifdef USE_NMC
	return m_pMotionNew->IsHomeDone();
#endif
#endif
}

BOOL CMotion::IsHomeDone(int nMotionId)
{
	//return (m_pMotionCard->IsHomeDone(nMotionId));

#ifdef USE_ZMP
	return m_pMotionOld->IsHomeDone(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsHomeDone(nMotionId);
#endif
#endif
}

BOOL CMotion::IsHomeSuccess()
{
	//return (m_pMotionCard->IsHomeSuccess());
#ifdef USE_ZMP
	return m_pMotionOld->IsHomeSuccess();
#else
#ifdef USE_NMC
	return m_pMotionNew->IsHomeSuccess();
#endif
#endif
}

BOOL CMotion::IsHomeSuccess(int nMotionId)
{
	//return (m_pMotionCard->IsHomeSuccess(nMotionId));
#ifdef USE_ZMP
	return m_pMotionOld->IsHomeSuccess(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsHomeSuccess(nMotionId);
#endif
#endif
}

BOOL CMotion::SetVMove(int nMotionId, double fVel, double fAcc)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetVMove(nMotionId, fVel, fAcc);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetVMove(nMotionId, fVel, fAcc);
#endif
#endif
}

BOOL CMotion::VMove(int nMotionId, int nDir)
{	
#ifdef USE_ZMP
	return m_pMotionOld->VMove(nMotionId, nDir);
#else
#ifdef USE_NMC
	return m_pMotionNew->VMove(nMotionId, nDir);
#endif
#endif
}

BOOL CMotion::Move(int nMotionId, double *pTgtPos, BOOL bAbs, BOOL bWait)
{
#ifdef USE_ZMP
	return m_pMotionOld->Move(nMotionId, pTgtPos, bAbs, bWait);
#else
#ifdef USE_NMC
	return m_pMotionNew->Move(nMotionId, pTgtPos, bAbs, bWait);
#endif
#endif
}

BOOL CMotion::MoveRatio(int nMotionId, double *pTgtPos, double dRatio, BOOL bAbs, BOOL bWait)
{	
#ifdef USE_ZMP
	return m_pMotionOld->MoveRatio(nMotionId, pTgtPos, dRatio, bAbs, bWait);
#else
#ifdef USE_NMC
	return m_pMotionNew->MoveRatio(nMotionId, pTgtPos, dRatio, bAbs, bWait);
#endif
#endif
}

BOOL CMotion::Move(int nMotionId, double *pTgtPos, double dSpd, double dAcc, double dDec, BOOL bAbs, BOOL bWait)
{
#ifdef USE_ZMP
	return m_pMotionOld->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait);
#else
	#ifdef USE_NMC
	return m_pMotionNew->Move(nMotionId, pTgtPos, dSpd, dAcc, dDec, bAbs, bWait);
	#endif
#endif
}

BOOL CMotion::IsMotionDone(int nMotionId)
{
	//return (m_pMotionCard->IsMotionDone(nMotionId));
#ifdef USE_ZMP
	return m_pMotionOld->IsMotionDone(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsMotionDone(nMotionId);
#endif
#endif
}

BOOL CMotion::IsInPosition(int nMotionId)
{
	//return (m_pMotionCard->IsInPosition(nMotionId));
#ifdef USE_ZMP
	return m_pMotionOld->IsInPosition(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsInPosition(nMotionId);
#endif
#endif
}



long CMotion::GetAxisState(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetAxisState(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetAxisState(nAxisId);
#endif
#endif
}

double CMotion::GetVelocity(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetVelocity(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetVelocity(nAxisId);
#endif
#endif
}
BOOL CMotion::SetCommandPos(int nAxisID, double fPos)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetCommandPos(nAxisID, fPos);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetCommandPos(nAxisID, fPos);
#endif
#endif
}
BOOL CMotion::SetActualPosition(int nAxisId, double fPos)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetActualPosition(nAxisId, fPos);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetActualPosition(nAxisId, fPos);
#endif
#endif
}

double CMotion::GetAxisVel(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetAxisVel(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetAxisVel(nAxisId);
#endif
#endif
}

BOOL CMotion::GetAxisAmpIsEnable(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetAxisAmpIsEnable(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetAxisAmpIsEnable(nAxisId);
#endif
#endif
}

double CMotion::LengthToPulse(int nIdxMotor, double fLength)
{
#ifdef USE_ZMP
	return m_pMotionOld->LengthToPulse(nIdxMotor, fLength);
#else
#ifdef USE_NMC
	return m_pMotionNew->LengthToPulse(nIdxMotor, fLength);
#endif
#endif
}
double CMotion::GetActualPosition(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetActualPosition(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetActualPosition(nAxisId);
#endif
#endif
}

BOOL CMotion::ObjectMapping()
{
#ifdef USE_ZMP
	return m_pMotionOld->ObjectMapping();
#else
#ifdef USE_NMC
	return m_pMotionNew->ObjectMapping();
#endif
#endif
}


BOOL CMotion::Stop(int nMotionId)
{

#ifdef USE_ZMP
	return m_pMotionOld->Stop(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->Stop(nMotionId);
#endif
#endif
}

BOOL CMotion::EStop(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->EStop(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->EStop(nMotionId);
#endif
#endif
}

BOOL CMotion::VMoveStop(int nMotionId, int nDir)
{
#ifdef USE_ZMP
	return m_pMotionOld->VMoveStop(nMotionId, nDir);
#else
#ifdef USE_NMC
	return m_pMotionNew->VMoveStop(nMotionId, nDir);
#endif
#endif
}

BOOL CMotion::ChkLimit(int nMotionId, int nDir)
{
#ifdef USE_ZMP
	return m_pMotionOld->ChkLimit(nMotionId, nDir);
#else
#ifdef USE_NMC
	return m_pMotionNew->ChkLimit(nMotionId, nDir);
#endif
#endif
}

BOOL CMotion::IsEnable(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->IsEnable(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsEnable(nMotionId);
#endif
#endif
}

BOOL CMotion::Clear(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->Clear(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->Clear(nMotionId);
#endif
#endif
}



BOOL CMotion::ReadBit(BYTE cBit, BOOL bInput)
{
#ifdef USE_ZMP
	return m_pMotionOld->ReadBit(cBit, bInput);
#else
#ifdef USE_NMC
	return m_pMotionNew->ReadBit(cBit, bInput);
#endif
#endif
}

unsigned long CMotion::ReadAllBit(BOOL bInput)
{
#ifdef USE_ZMP
	return m_pMotionOld->ReadAllBit(bInput);
#else
#ifdef USE_NMC
	return m_pMotionNew->ReadAllBit(bInput);
#endif
#endif
}

void CMotion::WriteBit(BYTE cBit, BOOL bOn)
{
#ifdef USE_ZMP
	return m_pMotionOld->WriteBit(cBit, bOn);
#else
#ifdef USE_NMC
	return m_pMotionNew->WriteBit(cBit, bOn);
#endif
#endif
}

void CMotion::WriteData(long lData)
{
#ifdef USE_ZMP
	return m_pMotionOld->WriteData(lData);
#else
#ifdef USE_NMC
	return m_pMotionNew->WriteData(lData);
#endif
#endif
}

long CMotion::SetNotifyFlush()
{
#ifdef USE_ZMP
	return m_pMotionOld->SetNotifyFlush();
#else
#ifdef USE_NMC
	return m_pMotionNew->SetNotifyFlush();
#endif
#endif
}


float CMotion::GetEStopTime(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetEStopTime(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetEStopTime(nMotionId);
#endif
#endif
}

void CMotion::SetEStopTime(int nMotionId, float fEStopTime)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetEStopTime(nMotionId, fEStopTime);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetEStopTime(nMotionId, fEStopTime);
#endif
#endif
}

void CMotion::ResetEStopTime(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->ResetEStopTime(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->ResetEStopTime(nMotionId);
#endif
#endif
}

long CMotion::GetState(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetState(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetState(nMotionId);
#endif
#endif
}

BOOL CMotion::Abort(int nMotionId)
{
#ifdef USE_ZMP
	return m_pMotionOld->Abort(nMotionId);
#else
#ifdef USE_NMC
	return m_pMotionNew->Abort(nMotionId);
#endif
#endif
}
double CMotion::GetInposition(int nAxis)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetInposition(nAxis);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetInposition(nAxis);
#endif
#endif
}
long CMotion::SetInposition(int nAxis, double fInpRange)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetInposition(nAxis, fInpRange);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetInposition(nAxis, fInpRange);
#endif
#endif
}


void CMotion::SetJogSpd(int nSpd)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetJogSpd(nSpd);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetJogSpd(nSpd);
#endif
#endif
}

#ifdef USE_ZMP
CZmpControl *CMotion::GetMotionControl()
{
	return m_pMotionOld->m_pMotionCard;
}
#else
#ifdef USE_NMC
CNmcDevice *CMotion::GetMotionControl()
{
	return m_pMotionNew->m_pMotionCard;
}
#endif
#endif



void CMotion::JogUp(int nMsId,int nDir)
{
#ifdef USE_ZMP
	return m_pMotionOld->JogUp(nMsId, nDir);
#else
#ifdef USE_NMC
	return m_pMotionNew->JogUp(nMsId, nDir);
#endif
#endif
}
void CMotion::JogDn(int nMsId,int nDir)
{
#ifdef USE_ZMP
	m_pMotionOld->JogDn(nMsId, nDir);
#else
#ifdef USE_NMC
	m_pMotionNew->JogDn(nMsId, nDir);
#endif
#endif
}
double CMotion::GetPosSWLimitValue(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetPosSWLimitValue(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetPosSWLimitValue(nAxisId);
#endif
#endif
}

double CMotion::GetNegSWLimitValue(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetNegSWLimitValue(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetNegSWLimitValue(nAxisId);
#endif
#endif
}
double CMotion::GetCommandPosition(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetCommandPosition(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetCommandPosition(nAxisId);
#endif
#endif
}

BOOL CMotion::WaitMotionDone(int nAxisId)
{
	//return (m_pMotionCard->WaitMotionDone(nAxisId));
#ifdef USE_ZMP
	return m_pMotionOld->WaitMotionDone(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->WaitMotionDone(nAxisId);
#endif
#endif
}
BOOL CMotion::IsNegSWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsNegSWLimit(nMotorId));
#ifdef USE_ZMP
	return m_pMotionOld->IsNegSWLimit(nMotorId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsNegSWLimit(nMotorId);
#endif
#endif
}
BOOL CMotion::IsPosSWLimit(int nMotorId)
{
	//return (m_pMotionCard->IsPosSWLimit(nMotorId));
#ifdef USE_ZMP
	return m_pMotionOld->IsPosSWLimit(nMotorId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsPosSWLimit(nMotorId);
#endif
#endif
}
BOOL CMotion::IsNegHWLimit(int nMotorId)
{
#ifdef USE_ZMP
	return m_pMotionOld->IsNegHWLimit(nMotorId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsNegHWLimit(nMotorId);
#endif
#endif
}
BOOL CMotion::IsPosHWLimit(int nMotorId)
{
#ifdef USE_ZMP
	return m_pMotionOld->IsPosHWLimit(nMotorId);
#else
#ifdef USE_NMC
	return m_pMotionNew->IsPosHWLimit(nMotorId);
#endif
#endif
}
int CMotion::SearchMachineStroke(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->SearchMachineStroke(nAxisId);
#else
#ifdef USE_NMC
	return m_pMotionNew->SearchMachineStroke(nAxisId);
#endif
#endif
}


// 지정된 축의 리미트센서 위치를 파악하여 소프트웨푳E리미트 값, 햨E최큱E스트로크 값을 결정하여 리턴한다.
int CMotion::GetAxisStroke(int nMotorId, double fPlusMargin, double fMinusMargin)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetAxisStroke(nMotorId, fPlusMargin, fMinusMargin);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetAxisStroke(nMotorId, fPlusMargin, fMinusMargin);
#endif
#endif
}

void CMotion::ChangePosSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate)
{
#ifdef USE_ZMP
	m_pMotionOld->ChangePosSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
#else
#ifdef USE_NMC
	m_pMotionNew->ChangePosSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
#endif
#endif

}

void CMotion::ChangeNegSWLimitValue(int nMotorId, double fErrorLimitValue, BOOL bUpdate)
{
#ifdef USE_ZMP
	m_pMotionOld->ChangeNegSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
#else
#ifdef USE_NMC
	m_pMotionNew->ChangeNegSWLimitValue(nMotorId, fErrorLimitValue, bUpdate);
#endif
#endif

}
BOOL CMotion::GetLimitInput(int nMotorId, int nDir)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetLimitInput(nMotorId, nDir);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetLimitInput(nMotorId, nDir);
#endif
#endif
}

BOOL CMotion::StartVelMove(int nAxisId, double fVel, double fAcc)
{
#ifdef USE_ZMP
	return m_pMotionOld->StartVelMove(nAxisId, fVel, fAcc);
#else
#ifdef USE_NMC
	return m_pMotionNew->StartVelMove(nAxisId, fVel, fAcc);
#endif
#endif
}

BOOL CMotion::IsEmergency()
{
#ifdef USE_ZMP
	return m_pMotionOld->IsEmergency();
#else
#ifdef USE_NMC
	return m_pMotionNew->IsEmergency();
#endif
#endif
}

void CMotion::SetAlarm(BOOL bState)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetAlarm(bState);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetAlarm(bState);
#endif
#endif
}

void CMotion::TowerLampControl(int nLamp, BOOL bOnOff)
{
#ifdef USE_ZMP
	return m_pMotionOld->TowerLampControl(nLamp, bOnOff);
#else
#ifdef USE_NMC
	return m_pMotionNew->TowerLampControl(nLamp, bOnOff);
#endif
#endif
}

void CMotion::Alarm(BOOL bOnOff)
{
#ifdef USE_ZMP
	return m_pMotionOld->Alarm(bOnOff);
#else
#ifdef USE_NMC
	return m_pMotionNew->Alarm(bOnOff);
#endif
#endif
}

void CMotion::SetHomeAction(int nMotorID, MPIAction Action)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetHomeAction(nMotorID, Action);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetHomeAction(nMotorID, Action);
#endif
#endif
}

void CMotion::ResetHomeAction(int nMotorID)
{
#ifdef USE_ZMP
	return m_pMotionOld->ResetHomeAction(nMotorID);
#else
#ifdef USE_NMC
	return m_pMotionNew->ResetHomeAction(nMotorID);
#endif
#endif
}
BOOL CMotion::GetOrgInput(int nMotorId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetOrgInput(nMotorId);
#else
#ifdef USE_NMC
	return m_pMotionNew->GetOrgInput(nMotorId);
#endif
#endif
}

void CMotion::CheckAxisDustCover()
{
#ifdef USE_ZMP
	return m_pMotionOld->CheckAxisDustCover();
#else
#ifdef USE_NMC
	return m_pMotionNew->CheckAxisDustCover();
#endif
#endif
}

#if USE_SSR_L_UL == USE
BOOL CMotion::ReadBitSeg(BYTE cBit, BOOL bInput, int nSeg)
{
#ifdef USE_ZMP
	return m_pMotionOld->ReadBitSeg(cBit, bInput, nSeg));
#else
#ifdef USE_NMC
	return m_pMotionNew->ReadBitSeg(cBit, bInput, nSeg));
#endif
#endif
}

void CMotion::WriteBitSeg(int cBit, BOOL bOn, int nSeg)
{
#ifdef USE_ZMP
	return m_pMotionOld->WriteBitSeg(cBit, bOn, nSeg);
#else
#ifdef USE_NMC
	return m_pMotionNew->WriteBitSeg(cBit, bOn, nSeg);
#endif
#endif
}
double CMotion::PulseToLength(int nAxisId, long nPulse)
{
#ifdef USE_ZMP
	return m_pMotionOld->PulseToLength(nAxisId, nPulse);
#else
#ifdef USE_NMC
	return m_pMotionNew->PulseToLength(nAxisId, nPulse);
#endif
#endif
}

BOOL CMotion::SetCommandPosition(int nAxisId, double fData)
{
#ifdef USE_ZMP
	return m_pMotionOld->SetCommandPosition(nAxisId, fData);
#else
#ifdef USE_NMC
	return m_pMotionNew->SetCommandPosition(nAxisId, fData);
#endif
#endif
}

CString CMotion::GetMotorName(int nAxisId)
{
#ifdef USE_ZMP
	return m_pMotionOld->GetMotorName(nAxisId));
#else
#ifdef USE_NMC
	return m_pMotionNew->GetMotorName(nAxisId));
#endif
#endif
}

BOOL CMotion::IsAmpFault(int nMotorId)
{
#ifdef USE_ZMP
	return m_pMotionOld->IsAmpFault(nMotorId));
#else
#ifdef USE_NMC
	return m_pMotionNew->IsAmpFault(nMotorId));
#endif
#endif
}

BOOL CMotion::IsServoOn(int nMotorID)
{
#ifdef USE_ZMP
	return m_pMotionOld->IsServoOn(nMotorID));
#else
#ifdef USE_NMC
	return m_pMotionNew->IsServoOn(nMotorID));
#endif
#endif
}

BOOL CMotion::ClearMotionFault(int nMsId)
{
#ifdef USE_ZMP
	return m_pMotionOld->Clear(nMsId);
#else
#ifdef USE_NMC
	return m_pMotionNew->Clear(nMsId);
#endif
#endif
}

BOOL CMotion::CheckDoorOpenAndExitManual()
{
#ifdef USE_ZMP
	return m_pMotionOld->CheckDoorOpenAndExitManual();
#else
#ifdef USE_NMC
	return m_pMotionNew->CheckDoorOpenAndExitManual();
#endif
#endif
}

BOOL CMotion::CheckShinkoDoorOpen()
{
#ifdef USE_ZMP
	return m_pMotionOld->CheckShinkoDoorOpen();
#else
#ifdef USE_NMC
	return m_pMotionNew->CheckShinkoDoorOpen();
#endif
#endif
}


void CMotion::MotorPowerOnOff(BOOL OnOff)
{
#ifdef USE_ZMP
	m_pMotionOld->MotorPowerOnOff(OnOff);
#else
#ifdef USE_NMC
	m_pMotionNew->MotorPowerOnOff(OnOff);
#endif
#endif
}

int CMotion::FaultRestoreAndCheckReadyStatus()
{
#ifdef USE_ZMP
	return m_pMotionOld->FaultRestoreAndCheckReadyStatus();
#else
#ifdef USE_NMC
	return m_pMotionNew->FaultRestoreAndCheckReadyStatus();
#endif
#endif
}


BOOL CMotion::GetMotionCreated()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_bMotionCreated;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_bMotionCreated;
#endif
#endif
}



#endif

BOOL CMotion::IsLimitSensor(CString &strMsg)
{
	BOOL bLimit = FALSE;
#ifdef USE_ZMP
	bLimit = m_pMotionOld->IsLimitSensor(strMsg);
#else
#ifdef USE_NMC
	bLimit = m_pMotionNew->IsLimitSensor(strMsg);
#endif
#endif
	return bLimit;
}


BOOL CMotion::GetPositiveOfLimitSensor()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_bPositiveOfLimitSensor;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_bPositiveOfLimitSensor;
#endif
#endif
}
int CMotion::GetAxisOfLimitSensor()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_nAxisOfLimitSensor;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_nAxisOfLimitSensor;
#endif
#endif
}

int CMotion::GetTotAxis()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_nTotAxis;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_nTotAxis;
#endif
#endif

}


MotionControl CMotion::GetParamParamCtrl()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_ParamCtrl;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_ParamCtrl;
#endif
#endif

}


MotionMotion* CMotion::GetParamMotion()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_pParamMotion;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_pParamMotion;
#endif
#endif

}

MotionAxis* CMotion::GetParamAxis()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_pParamAxis;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_pParamAxis;
#endif
#endif

}

MotionMotor* CMotion::GetParamMotor()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_pParamMotor;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_pParamMotor;
#endif
#endif

}

CString CMotion::GetMotionParamFilepath()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_strMotionParamFilepath;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_strMotionParamFilepath;
#endif
#endif
}

BOOL *CMotion::GetDO()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_bDO;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_bDO;
#endif
#endif
}

BOOL *CMotion::GetHomeActionAbort()
{
#ifdef USE_ZMP
	return m_pMotionOld->m_pbHomeActionAbort;
#else
#ifdef USE_NMC
	return m_pMotionNew->m_pbHomeActionAbort;
#endif
#endif
}