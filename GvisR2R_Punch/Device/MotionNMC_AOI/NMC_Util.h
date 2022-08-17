#pragma once


#ifdef _WIN64
#include "NMC_Motion.h"
//#pragma comment (lib, "Device/NMC/x64/NMC_Comm.lib")
//#pragma comment (lib, "Device/NMC/x64/NMC_Driver.lib")
#pragma comment (lib, "../lib/NMC_Motion.lib")
#pragma comment (lib, "../lib/NMC_APP_Motion.lib")

#else
/*
#include "Device/MMCE/x86/NMC_Motion.h"
//#pragma comment (lib, "Device/NMC/x86/NMC_Comm.lib")
//#pragma comment (lib, "Device/NMC/x86/NMC_Driver.lib")
#pragma comment (lib, "Device/NMC/x86/NMC_Motion.lib")
*/
#endif


CString GetNMCErrorMessage(UINT32 ErrorCode);