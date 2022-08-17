#if !defined(AFX_MPDEVICE_H__17F5B7FA_86E6_4899_A1B0_01858E4E0C0B__INCLUDED_)
#define AFX_MPDEVICE_H__17F5B7FA_86E6_4899_A1B0_01858E4E0C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MpDevice.h : header file
//
#include "../Global/GlobalDefine.h"

#include "ymcPCAPI.h"
// #include <afxmt.h>
#pragma comment (lib, "ymcPCAPI_x64.lib")

#define M_REGISTER_MAX_ADDR		65532

/////////////////////////////////////////////////////////////////////////////
// CMpDevice window

class CMpDevice : public CWnd
{
#ifdef USE_MPE
	HDEVICE			m_hDevice;
#endif
	CWnd*			m_pParent;
	
#ifdef USE_MPE
	COM_DEVICE		m_ComDevice;
	HREGISTERDATA	m_hInRegData, m_hOutRegData;
#endif

	DWORD			m_dwReturnVal;
	int				m_nPrevDecodeRatio;

	BOOL GetIoDataValue(CString strRegAddr, long &InputData);
	BOOL SetIoDataValue(CString strRegAddr, long OutputData);
	void CloseMpDevice();
	BOOL WriteRegisterDataM(int nAddr, long lData); // Long형 데이타 저장
	BOOL ReadRegisterDataM(int nAddr, long &lReadBuffer); // Long형 데이타 읽기
	BOOL ReadTempRegDataM(int nAddr, long &lReadBuffer);
	BOOL SetRegisterBitM(int nAddr, int nBit, BOOL bFlag); // nBit: 32Bits(0 ~ 31) - Long형주소, Bit제어
	BOOL GetRegisterBitM(int nAddr, int nBit, BOOL &bFlag); // nBit: 32Bits(0 ~ 31) - Long형주소, Bit제어
	int GetIo(CString strRegAddr, LPVOID pIoData, int nIoSize);	// nRtn : GetDataSize
	BOOL SetIo(CString strRegAddr, LPVOID pIoData, int nIoSize);
// 	long Read(CString strRegAddr);
	int atoh(CString &strVal);

	//void SetConfigure(UINT16 nBoardId, UINT16 nDevIdIoIn, UINT16 nDevIdIoOut, INT nOffsetAxisID);


// Construction
public:
	CMpDevice(CWnd* pParent=NULL);
#ifdef USE_MPE
	HCONTROLLER		m_hController;
#endif


// Attributes
public:
	CCriticalSection m_cs;

// Operations
public:
	BOOL Init(unsigned short nCpuNumber, unsigned short nPortNumber);
	BOOL ClearRegisterM();

	unsigned long ReadBits(CString strRegAddr, int nSize=1); // strRegAddr is "MBxxxxxx", nSize is 16Bits or 32Bits.
	int Read(CString strRegAddr, int nSize, unsigned short *pData);
	int Read(CString strRegAddr, int nSize, unsigned long *pData);
	BOOL Write(CString strRegAddr, long lData, BOOL bCheck=FALSE);

	long Read(CString strRegAddr);
	unsigned short Read(int nSeg);
	unsigned short ReadI(int nSeg);
	int ReadI(int *pSeg, int nTot, unsigned short *pData);
	int ReadIO(int *pSeg, int nTot, unsigned short *pData);
	int ReadSignal(int *pSeg, int nTot, unsigned short *pData);
	int ReadData(int *pSeg, int nTot, long **pData);
	void Write(int nSeg, unsigned short nOut, BOOL bFromSliceIO=FALSE);
	void Write(int *pSeg, int nTot, unsigned short *pData, BOOL bFromSliceIO=FALSE);


	BOOL WriteRegisterDataO(int nAddr, long lData);
	long GetRegisterDataO(int nAddr);
	void GetMpeIO();
	void GetMpeSignal();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMpDevice)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMpDevice();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMpDevice)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPDEVICE_H__17F5B7FA_86E6_4899_A1B0_01858E4E0C0B__INCLUDED_)
