#include "stdafx.h"
#include "HardwareInfoDefine.h"
#include "HardwareInfoEx.h"

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

IMPLEMENT_DYNAMIC(CHardwareInfoEx, CWnd)

CHardwareInfoEx::CHardwareInfoEx()
{
	HRESULT hr;

	// COM Start....
	if ((hr = CoInitializeEx(NULL,COINIT_MULTITHREADED)) == S_OK)
	{
		m_bCoInitialize = TRUE;

		if(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
			RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0) != S_OK)
		{
			TRACE("Failed-CoInitializeSecurity().\n");
			//AfxMessageBox(_T("Failed-CoInitializeSecurity()."));
		}
	}
	else
		m_bCoInitialize = FALSE;
}

CHardwareInfoEx::~CHardwareInfoEx()
{
	if(m_bCoInitialize)
		CoUninitialize();

	if(m_ListPc.GetSize() > 0)
		m_ListPc.RemoveAll();
	if(m_ListCpu.GetSize() > 0)
		m_ListCpu.RemoveAll();
	if(m_ListBios.GetSize() > 0)
		m_ListBios.RemoveAll();
	if(m_ListMainBoard.GetSize() > 0)
		m_ListMainBoard.RemoveAll();
	if(m_ListRam.GetSize() > 0)
		m_ListRam.RemoveAll();
	if(m_ListHdd.GetSize() > 0)
		m_ListHdd.RemoveAll();
	if(m_ListLogicalDisk.GetSize() > 0)
		m_ListLogicalDisk.RemoveAll();
	if(m_ListUsb.GetSize() > 0)
		m_ListUsb.RemoveAll();
	if(m_ListOs.GetSize() > 0)
		m_ListOs.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHardwareInfoEx, CWnd)
END_MESSAGE_MAP()



// CHardwareInfoEx 메시지 처리기입니다.

BOOL CHardwareInfoEx::ExecQuery(ListInfo &List, wchar_t* pQuery, wchar_t** pProperties, int nTotalProperties)
{
	IWbemLocator *pIWbemLocator = NULL;
	IWbemServices *pIWbemServices = NULL;
	IEnumWbemClassObject *pEnumObj = NULL;

	wchar_t* pType = OLESTR("WQL");
	BSTR bstrNameSpace = OLESTR("root\\cimv2");

	if(CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL,
		CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
		IID_IUnknown, (void**)&pIWbemLocator) != S_OK)
	{
		return FALSE;
	}

	if(pIWbemLocator->ConnectServer(bstrNameSpace, NULL, NULL, NULL, 0, 
		NULL, NULL, &pIWbemServices) != S_OK)
	{
		pIWbemLocator->Release();
		return FALSE;
	}

	if(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
		NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE) != S_OK)
	{
		pIWbemServices->Release();
		pIWbemLocator->Release();
		return FALSE;
	}

	BSTR sType = SysAllocString(pType);
	BSTR sQuery;
	HRESULT hRst;

	sQuery = SysAllocString(pQuery);
	hRst = pIWbemServices->ExecQuery(sType, sQuery, WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumObj);
	SysFreeString(sType);
	SysFreeString(sQuery);

	if(hRst != S_OK)
	{
		pIWbemServices->Release();
		pIWbemLocator->Release();
		return FALSE;
	}

	hRst = pEnumObj->Reset();
	if(hRst != S_OK)
		return FALSE;

	ULONG uCount = 1;
	ULONG uReturned;
	IWbemClassObject *pClassObject=NULL;
	VARIANT var;
	CString sVar;
	BSTR sClassProp;
	_bstr_t bstrT;

	SAFEARRAY *pSA; 
	BSTR *bstrArray;
	TCHAR* pbstr;

	if(List.GetSize() > 0)
		List.RemoveAll();

	int i, nInstance=0;
	int nFrom=0, nTo=nTotalProperties;

	while(pEnumObj)
	{
		hRst = pEnumObj->Next(WBEM_INFINITE, uCount, &pClassObject, &uReturned);
		if(!uReturned)
			break;

		for(i=nFrom; i<nTo; i++)
		{
			sClassProp = SysAllocString(pProperties[i]);
			hRst = pClassObject->Get(sClassProp, 0, &var, 0, 0);
			SysFreeString(sClassProp);
			if(var.vt != VT_NULL)
			{
				if(var.vt & VT_ARRAY)
				{	
					pSA = var.parray;   //작업대상 safearray 포인터 assign
					SafeArrayAccessData(pSA, (void**)&bstrArray);  // 작업시작 메모리 락해주기.

					USES_CONVERSION; //convert를 위한 매크로
					pbstr=W2T(*bstrArray); //bstr을 char로 convert
					sVar.Format(_T("%s"), pbstr);
					VariantClear(&var);
					List.Add(sVar);
				}
				else
				{
					bstrT = &var;
					sVar.Format(_T("%s"),(LPCTSTR)bstrT);



					VariantClear(&var);
					List.Add(sVar);
				}
			}
			else
				List.Add(CString(_T("")));
		}
	
		nInstance++;

		pClassObject->Release();
	}

	pEnumObj->Release();
	pIWbemServices->Release();
	pIWbemLocator->Release();

	return TRUE;
}

int CHardwareInfoEx::GetItemTotalPc()
{
	return m_ListPc.GetCount();
}

int CHardwareInfoEx::GetItemTotalCpu()
{
	return m_ListCpu.GetCount();
}

int CHardwareInfoEx::GetItemTotalBios()
{
	return m_ListBios.GetCount();
}

int CHardwareInfoEx::GetItemTotalMainBoard()
{
	return m_ListMainBoard.GetCount();
}

int CHardwareInfoEx::GetItemTotalRam()
{
	return m_ListRam.GetCount();
}

int CHardwareInfoEx::GetItemTotalHdd()
{
	return m_ListHdd.GetCount();
}

int CHardwareInfoEx::GetItemTotalLogicalDisk()
{
	return m_ListLogicalDisk.GetCount();
}

int CHardwareInfoEx::GetItemTotalUsb()
{
	return m_ListUsb.GetCount();
}
int CHardwareInfoEx::GetItemTotalOs()
{
	return m_ListOs.GetCount();
}

int CHardwareInfoEx::GetFieldTotalPc()
{
	return PcTotal;
}

int CHardwareInfoEx::GetFieldTotalCpu()
{
	return CpuTotal;
}

int CHardwareInfoEx::GetFieldTotalBios()
{
	return BiosTotal;
}

int CHardwareInfoEx::GetFieldTotalMainBoard()
{
	return MainBoardTotal;
}

int CHardwareInfoEx::GetFieldTotalRam()
{
	return RamTotal;
}

int CHardwareInfoEx::GetFieldTotalHdd()
{
	return HddTotal;
}

int CHardwareInfoEx::GetFieldTotalLogicalDisk()
{
	return LogicalDiskTotal;
}

int CHardwareInfoEx::GetFieldTotalUsb()
{
	return UsbTotal;
}
int CHardwareInfoEx::GetFieldTotalOs()
{
	return OsTotal;
}
CString CHardwareInfoEx::GetItemValuePc(int nIndex)
{
	if(m_ListPc.IsEmpty())
		return CString(_T(""));

	return m_ListPc.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueCpu(int nIndex)
{
	if(m_ListCpu.IsEmpty())
		return CString(_T(""));

	return m_ListCpu.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueBios(int nIndex)
{
	if(m_ListBios.IsEmpty())
		return CString(_T(""));

	return m_ListBios.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueMainBoard(int nIndex)
{
	if(m_ListMainBoard.IsEmpty())
		return CString(_T(""));

	return m_ListMainBoard.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueRam(int nIndex)
{
	if(m_ListRam.IsEmpty())
		return CString(_T(""));

	return m_ListRam.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueHdd(int nIndex)
{
	if(m_ListHdd.IsEmpty())
		return CString(_T(""));

	return m_ListHdd.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueLogicalDisk(int nIndex)
{
	if(m_ListLogicalDisk.IsEmpty())
		return CString(_T(""));

	return m_ListLogicalDisk.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemValueUsb(int nIndex)
{
	if(m_ListUsb.IsEmpty())
		return CString(_T(""));

	return m_ListUsb.GetAt(nIndex);
}
CString CHardwareInfoEx::GetItemValueOs(int nIndex)
{
	if(m_ListOs.IsEmpty())
		return CString(_T(""));

	return m_ListOs.GetAt(nIndex);
}

CString CHardwareInfoEx::GetItemNamePc(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjPc[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);


	return str;
}

CString CHardwareInfoEx::GetItemNameCpu(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjCpu[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);


	return str;
}

CString CHardwareInfoEx::GetItemNameBios(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjBios[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);


	return str;
}

CString CHardwareInfoEx::GetItemNameMainBoard(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjMainBoard[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);


	return str;
}

CString CHardwareInfoEx::GetItemNameRam(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjRam[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);

	return str;
}

CString CHardwareInfoEx::GetItemNameHdd(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjHdd[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);

	return str;
}

CString CHardwareInfoEx::GetItemNameLogicalDisk(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjLogicalDisk[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);

	return str;
}

CString CHardwareInfoEx::GetItemNameUsb(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjUsb[nIndex];
	str.Format(_T("%s"), (LPCTSTR)bstrT);

	return str;
}
CString CHardwareInfoEx::GetItemNameOs(int nIndex)
{
	CString str;

	_bstr_t bstrT = FieldObjOs[nIndex];
	//size_t	sz = wcslen((TCHAR*)bstrT);
	//TCHAR *pChar = (TCHAR*)malloc(sz+1);
	//_tcscpy(pChar, (TCHAR*)bstrT);
	//pChar[sz] = _T('\0');
	//str.Format(_T("%s"), pChar);
	//free(pChar);
	str.Format(_T("%s"), (LPCTSTR)bstrT);

	return str;
}

BOOL CHardwareInfoEx::UpdateData()
{
	DEVMODE DevMode;
	DevMode.dmSize=sizeof(DEVMODE);

	if (EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&DevMode))
	{
		m_dwPixelWidth = DevMode.dmPelsWidth;
	}
	m_strFontsDir = QueryDirectoryEx("Fonts",CSIDL_FONTS);
	if(!GetWmiInfoPc())
		return FALSE;
	if(!GetWmiInfoCpu())
		return FALSE;
	if(!GetWmiInfoBios())
		return FALSE;
	if(!GetWmiInfoMainBoard())
		return FALSE;
	if(!GetWmiInfoRam())
		return FALSE;
	if(!GetWmiInfoHdd())
		return FALSE;
	if(!GetWmiInfoLogicalDisk())
		return FALSE;
	if(!GetWmiInfoUsb())
		return FALSE;

	return TRUE;
}

BOOL CHardwareInfoEx::GetWmiInfoPc()
{
	BOOL bRtn = ExecQuery( m_ListPc, OLESTR("Select * from Win32_ComputerSystem"), FieldObjPc, PcTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoCpu()
{
	BOOL bRtn = ExecQuery( m_ListCpu, OLESTR("Select * from Win32_Processor"), FieldObjCpu, CpuTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoBios()
{
	BOOL bRtn = ExecQuery( m_ListBios, OLESTR("Select * from Win32_BIOS"), FieldObjBios, BiosTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoMainBoard()
{
	BOOL bRtn = ExecQuery( m_ListMainBoard, OLESTR("Select * from Win32_BaseBoard"), FieldObjMainBoard, MainBoardTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoRam()
{
	BOOL bRtn = ExecQuery( m_ListRam, OLESTR("Select * from Win32_MemoryDevice"), FieldObjRam, RamTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoHdd()
{
	BOOL bRtn = ExecQuery( m_ListHdd, OLESTR("Select * from Win32_DiskDrive"), FieldObjHdd, HddTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoLogicalDisk()
{
	BOOL bRtn = ExecQuery( m_ListLogicalDisk, OLESTR("Select * from Win32_LogicalDisk"), FieldObjLogicalDisk, LogicalDiskTotal );
	return bRtn;
}

BOOL CHardwareInfoEx::GetWmiInfoUsb()
{
	BOOL bRtn = ExecQuery( m_ListUsb, OLESTR("Select * from Win32_USBHub"), FieldObjUsb, UsbTotal );
	return bRtn;
}
BOOL CHardwareInfoEx::GetWmiInfoOs()
{
	BOOL bRtn = ExecQuery( m_ListOs, OLESTR("Select * from Win32_OperatingSystem"), FieldObjOs, OsTotal );
	return bRtn;
}
CString CHardwareInfoEx::QueryDirectoryEx(char *sFolder, int iFolder)
{
	CString strPath;
	TCHAR path[MAX_PATH];
#ifdef UNICODE
	wchar_t szCh[MAX_PATH];
	int nLen;
	if (SHGetSpecialFolderPath(NULL,path,iFolder,FALSE))
	{
		nLen = MultiByteToWideChar(949,0,sFolder,-1,NULL,NULL);
		MultiByteToWideChar(949,0,sFolder,-1,szCh,nLen);
		strPath.Format(_T("%s"),path);
	}
	else
		strPath.Empty();
#else
	if (SHGetSpecialFolderPath(NULL,path,iFolder,FALSE))
	{
		strPath.Format(_T("%s"),path);
	}
	else
		strPath.Empty();
#endif

	return strPath;
}


