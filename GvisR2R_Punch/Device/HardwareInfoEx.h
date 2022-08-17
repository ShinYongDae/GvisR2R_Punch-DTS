#pragma once

enum FieldIdxOs
{
	OsBootDevice=0,
	OsBuildNumber,
	OsBuildType,
	OsCaption,
	OsCodeSet,
	OsCountryCode,
	OsCreationClassName,
	OsCSCreationClassName,
	OsCSDVersion,
	OsCSName,
	OsCurrentTimeZone,
	OsDataExecutionPrevention_32BitApplications,
	OsDataExecutionPrevention_Available,
	OsDataExecutionPrevention_Drivers,
	OsDataExecutionPrevention_SupportPolicy,
	OsDebug,
	OsDescription,
	OsDistributed,
	OsEncryptionLevel,
	OsForegroundApplicationBoost,
	OsFreePhysicalMemory,
	OsFreeSpaceInPagingFiles,
	OsFreeVirtualMemory,
	OsInstallDate,
	OsLargeSystemCache,
	OsLastBootUpTime,
	OsLocalDateTime,
	OsLocale,
	OsManufacturer,
	OsMaxNumberOfProcesses,
	OsMaxProcessMemorySize,
	OsMUILanguages,
	OsName,
	OsNumberOfLicensedUsers,
	OsNumberOfProcesses,
	OsNumberOfUsers,
	OsOperatingSystemSKU,
	OsOrganization,
	OsOSArchitecture,
	OsOSLanguage,
	OsOSProductSuite,
	OsOSType,
	OsOtherTypeDescription,
	OsPAEEnabled,
	OsPlusProductID,
	OsPlusVersionNumber,
	OsPrimary,
	OsProductType,
	OsRegisteredUser,
	OsSerialNumber,
	OsServicePackMajorVersion,
	OsServicePackMinorVersion,
	OsSizeStoredInPagingFiles,
	OsStatus,
	OsSuiteMask,
	OsSystemDevice,
	OsSystemDirectory,
	OsSystemDrive,
	OsTotalSwapSpaceSize,
	OsTotalVirtualMemorySize,
	OsTotalVisibleMemorySize,
	OsVersion,
	OsWindowsDirectory,
	OsTotal
};

typedef CArray<CString, CString&>ListInfo;

// CHardwareInfoEx

class CHardwareInfoEx : public CWnd
{
	DECLARE_DYNAMIC(CHardwareInfoEx)

	BOOL m_bCoInitialize;

	ListInfo m_ListPc;
	ListInfo m_ListCpu;
	ListInfo m_ListBios;
	ListInfo m_ListMainBoard;
	ListInfo m_ListRam;
	ListInfo m_ListHdd;
	ListInfo m_ListLogicalDisk;
	ListInfo m_ListUsb;
	ListInfo m_ListOs;

	BOOL ExecQuery(ListInfo &List, wchar_t* pQuery, wchar_t** pProperties, int nTotalProperties);

	BOOL GetWmiInfoPc();
	BOOL GetWmiInfoCpu();
	BOOL GetWmiInfoBios();
	BOOL GetWmiInfoMainBoard();
	BOOL GetWmiInfoRam();
	BOOL GetWmiInfoHdd();
	BOOL GetWmiInfoLogicalDisk();
	BOOL GetWmiInfoUsb();
	BOOL GetWmiInfoOs();

public:
	CHardwareInfoEx();
	virtual ~CHardwareInfoEx();

	BOOL UpdateData();

	int GetFieldTotalPc();
	int GetFieldTotalCpu();
	int GetFieldTotalBios();
	int GetFieldTotalMainBoard();
	int GetFieldTotalRam();
	int GetFieldTotalHdd();
	int GetFieldTotalLogicalDisk();
	int GetFieldTotalUsb();
	int GetFieldTotalOs();

	int GetItemTotalPc();
	int GetItemTotalCpu();
	int GetItemTotalBios();
	int GetItemTotalMainBoard();
	int GetItemTotalRam();
	int GetItemTotalHdd();
	int GetItemTotalLogicalDisk();
	int GetItemTotalUsb();
	int GetItemTotalOs();

	CString GetItemValuePc(int nIndex);
	CString GetItemValueCpu(int nIndex);
	CString GetItemValueBios(int nIndex);
	CString GetItemValueMainBoard(int nIndex);
	CString GetItemValueRam(int nIndex);
	CString GetItemValueHdd(int nIndex);
	CString GetItemValueLogicalDisk(int nIndex);
	CString GetItemValueUsb(int nIndex);
	CString GetItemValueOs(int nIndex);

	CString GetItemNamePc(int nIndex);
	CString GetItemNameCpu(int nIndex);
	CString GetItemNameBios(int nIndex);
	CString GetItemNameMainBoard(int nIndex);
	CString GetItemNameRam(int nIndex);
	CString GetItemNameHdd(int nIndex);
	CString GetItemNameLogicalDisk(int nIndex);
	CString GetItemNameUsb(int nIndex);
	CString GetItemNameOs(int nIndex);

	CString QueryDirectoryEx(char *sFolder, int iFolder);
	int m_dwPixelWidth;
	CString m_strFontsDir;

protected:
	DECLARE_MESSAGE_MAP()
};


