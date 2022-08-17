#include "../stdafx.h"
#include "NMC_TriggerCom.h"

/*
extern "C" const GUID __declspec(selectany) LIBID_NmcTrigger =
{ 0x2be9e5d4,0x9c1c,0x446b,{ 0x8c,0xff,0x36,0x1b,0x49,0x75,0xd1,0xbd } };
extern "C" const GUID __declspec(selectany) CLSID_APPSDKLib =
{ 0x94b0da41,0xa81e,0x3379,{ 0x9d,0x4c,0xcf,0x08,0x7c,0x3f,0x9c,0xba } };
extern "C" const GUID __declspec(selectany) DIID_NMC_TriggerInterface =
{ 0x8183e386,0xe343,0x4f72,{ 0x86,0xbc,0x01,0xf0,0x8f,0xbb,0x05,0xa7 } };
extern "C" const GUID __declspec(selectany) CLSID_NMC_TriggerInterfaceImpl =
{ 0x932608de,0x7884,0x4d76,{ 0xab,0xad,0x81,0x62,0xd6,0xa0,0x71,0x9a } };
extern "C" const GUID __declspec(selectany) IID__APPSDKLib =
{ 0x772133d7,0x20da,0x3d04,{ 0xa0,0xd5,0x05,0x75,0x5e,0xaa,0x33,0xa6 } };
*/
NMC_Trigger::NMC_Trigger()
{
	m_pInterface = nullptr;

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(hr))
		{

		}
	}

	if (FAILED(hr = CoCreateInstance(CLSID_NMC_TriggerInterfaceImpl, 0, CLSCTX_INPROC_SERVER, DIID_NMC_TriggerInterface,reinterpret_cast<void**>(&m_pInterface))))
	{
		CoUninitialize();
		return;
	}
}

NMC_Trigger::~NMC_Trigger()
{
	CoUninitialize();	
}
