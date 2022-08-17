#pragma once

#include "NMC_Motion.h"

struct NMC_TriggerInterface;
class NMC_Trigger
{
private:
	NMC_TriggerInterface* m_pInterface;
public:
	NMC_Trigger();
	~NMC_Trigger();

	MC_STATUS NMC_StatusChange_OP(unsigned short BoardID, unsigned short* DeviceID, unsigned short Size, unsigned short Mode, unsigned long TimmeOut)
	{
		if (!m_pInterface)
			return MC_ERROR;

		HLOCAL hLocal = NULL;
		LPSAFEARRAYBOUND lpSafeArrayBound = NULL;
		LPSAFEARRAY lpSafeArray = NULL;

		hLocal = LocalAlloc(LHND, sizeof(SAFEARRAYBOUND));
		lpSafeArrayBound = (LPSAFEARRAYBOUND)LocalLock(hLocal);
		lpSafeArrayBound->lLbound = 0;
		lpSafeArrayBound->cElements = Size;
		
		lpSafeArray = SafeArrayCreate(VT_UI2, 1, lpSafeArrayBound);

		unsigned short* pData;
		SafeArrayLock(lpSafeArray);
		SafeArrayAccessData(lpSafeArray, (void **)&pData);
		for (int i = 0; i < Size; i++)
		{
			pData[i] = DeviceID[i];
		}
		SafeArrayUnaccessData(lpSafeArray);
		SafeArrayUnlock(lpSafeArray);

		MC_STATUS ERR = (MC_STATUS)m_pInterface->NMC_StatusChange_OP(BoardID, lpSafeArray, Size, Mode, TimmeOut);

		SafeArrayDestroy(lpSafeArray);

		return ERR;
	}

	 MC_STATUS NMC_TR_SetTriggerConfig(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex, unsigned short EncSorceIndex, bool InvertEncDir, double EncRatio, unsigned long VLevel, unsigned long Mode)
	 {
		 if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_SetTriggerConfig(BoardID, DeviceID, TriggerIndex, EncSorceIndex, InvertEncDir, EncRatio, VLevel, Mode);
		 else
			 return MC_ERROR;
	}
	 MC_STATUS NMC_TR_SetTableTriggerData(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex, double* Pos, unsigned long* Mode, unsigned short Width, unsigned short Count)
	{
		 if (!m_pInterface)
			 return MC_ERROR;

		 int nSize = 1;

		 HLOCAL hLocal = NULL;
		 LPSAFEARRAYBOUND lpSafeArrayBound = NULL;
		 LPSAFEARRAY lpSafeArray = NULL;

		 hLocal = LocalAlloc(LHND, sizeof(SAFEARRAYBOUND));
		 lpSafeArrayBound = (LPSAFEARRAYBOUND)LocalLock(hLocal);
		 lpSafeArrayBound->lLbound = 0;
		 lpSafeArrayBound->cElements = nSize;

		 lpSafeArray = SafeArrayCreate(VT_R8, 1, lpSafeArrayBound);

		 double* pData;
		 SafeArrayLock(lpSafeArray);
		 SafeArrayAccessData(lpSafeArray, (void **)&pData);
		 for (int i = 0; i < nSize; i++)
	     {
			 pData[i] = Pos[i];
	      }
		 SafeArrayUnaccessData(lpSafeArray);
		 SafeArrayUnlock(lpSafeArray);


		 HLOCAL hLocal2 = NULL;
		 LPSAFEARRAYBOUND lpSafeArrayBound2 = NULL;
		 LPSAFEARRAY lpSafeArray2 = NULL;

		 hLocal2 = LocalAlloc(LHND, sizeof(SAFEARRAYBOUND));
		 lpSafeArrayBound2 = (LPSAFEARRAYBOUND)LocalLock(hLocal2);
		 lpSafeArrayBound2->lLbound = 0;
		 lpSafeArrayBound2->cElements = nSize;

		 lpSafeArray2 = SafeArrayCreate(VT_UI4, 1, lpSafeArrayBound2);

		 unsigned int* pData2;
		 SafeArrayLock(lpSafeArray2);
		 SafeArrayAccessData(lpSafeArray2, (void **)&pData2);
			 for (int i = 0; i < nSize; i++)
			 {
				 pData2[i] = Mode[i];
			 }
		 SafeArrayUnaccessData(lpSafeArray2);
		 SafeArrayUnlock(lpSafeArray2);



		MC_STATUS err=	 (MC_STATUS)m_pInterface->NMC_TR_SetTableTriggerData(BoardID, DeviceID, TriggerIndex, lpSafeArray, lpSafeArray2, Width, Count);

		SafeArrayDestroy(lpSafeArray);
		SafeArrayDestroy(lpSafeArray2);



		return err;
	}
	 MC_STATUS NMC_TR_TableTriggerDataClear(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex)
	{
		if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_TableTriggerDataClear(BoardID, DeviceID, TriggerIndex);
		else
			return MC_ERROR;
	}
	  MC_STATUS NMC_TR_TriggerEncoderCounterClear(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex)
	{
		if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_TriggerEncoderCounterClear(BoardID, DeviceID, TriggerIndex);
		else
			return MC_ERROR;
	}
	 MC_STATUS NMC_TR_TriggerOneShot(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex, unsigned short Width)
	{
		if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_TriggerOneShot(BoardID, DeviceID, TriggerIndex, Width);
		else
			return MC_ERROR;
	}

	 MC_STATUS NMC_TR_SetIntervalTriggerData(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex, double StartPos, double EndPos, double Period, unsigned short Width)
	{
		if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_SetIntervalTriggerData(BoardID, DeviceID, TriggerIndex, StartPos, EndPos, Period, Width);
		else
			return MC_ERROR;
	}

	 MC_STATUS NMC_TR_SetIntervalTriggerEnable(unsigned short BoardID, unsigned short DeviceID, unsigned short TriggerIndex, bool Enable, unsigned short Mode)
	{
		if (m_pInterface)
			return (MC_STATUS)m_pInterface->NMC_TR_SetIntervalTriggerEnable(BoardID, DeviceID, TriggerIndex, Enable, Mode);
		else
			return MC_ERROR;
	}

};