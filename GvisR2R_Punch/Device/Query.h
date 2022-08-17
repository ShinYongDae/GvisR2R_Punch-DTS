#pragma once

#include "DataSource.h"
#include "../Global/MyData.h"
#include "User.h"
#include <vector>

#define CODE_GROUP_ACCESS_LEVEL		_T("1")
#define CODE_GROUP_CUSTOMER_TYPE	_T("2")
#define CODE_GROUP_SHIP_TYPE		_T("3")
#define CODE_GROUP_SIDE_TYPE		_T("4")
#define CODE_GROUP_MIRROR_TYPE		_T("5")
#define CODE_GROUP_ROTATE_TYPE		_T("6")
#define CODE_GROUP_STATUS_TYPE		_T("7")
#define CODE_GROUP_USING			_T("8")

using namespace std;

class CQuery
{
	CDataSource m_dataSource;
	BOOL m_bInitDB;

	void Log(CString strMsg, int nType = 0);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 
	BOOL DirectoryExists(LPCTSTR szPath);

public:
	CQuery();
	~CQuery();

	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword);
	void InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword, int nDbtype);

	int GetCustomerNameList(CStringArray &strCustomerName);
	BOOL FindCustomerCode(CString strCustomerName, CString &strCustomerCode);
	int GetUserInfo(CStringArray &sUserID, CStringArray &sPW, CStringArray &sKName, CStringArray &sEName, CStringArray &sJName, CStringArray &sCName, CStringArray &sAccLv);
	int GetUserNameList(CStringArray &strUserName);
	int GetUserList(CUser* pList, int& nUserCount);
	BOOL GetUserName(int nIndex, CString &strUserName);
	BOOL FindUserCode(CString strUserName, CString &strUserCode);
	int GetEquipmentNameList(CStringArray &strEquipName);
	BOOL GetEquipmentName(int nIndex, CString &strEquipName);
	BOOL FindEquipCode(CString strEquipName, CString &strEquipCode);
	CString GetCamInfoData(CString strModelName, CString strLayerCode, CString strLayerName);
	CString GetCamSpecDirFromRSTH(CString strLotCode, CString strLayerCode, CString strLayerName);
	BOOL GetInspectionRegionByModel(CString strModelName, double &fWidth, double &fHeight);
	BOOL GetNumberOfStripRegionFromMODEL(CString strModelCode, int &nNumOfRegion);
	BOOL GetModelCode(CString strModelName, CString &strModelCode);
	int GetLotSize(CString strModelName);
	int GetLotNameList(CStringArray &strLotArray, CString strModelName, CString strStartDate, CString strEndDate);
	BOOL GetLotName(int nIndex, CString strModelName, CString &strLotName);
	BOOL GetLotCode(CString strModelName, CString strLotName, CString &strLotCode);
	BOOL GetLayerCode(CString strModelName, CString strLayerName, CString &strLayerCode);
	int GetLayerNameList(CString strModelName, CStringArray &strLayerName);
	BOOL GetLayerName(int nIndex, CString strModelName, CString &strLayerName);
	int  GetLayerCodeList(CString strModelName, CStringArray &strLayerCode);
	BOOL GetLayerCodeFromCamInfo(CString strModelName, CString strLayerName, CString &strLayerCode);
	BOOL GetLayerCodeFromLayer(CString strModelName, CString strLayerName, CString &strLayerCode);
	int GetSerialNameList(CString strModelName, CString strLayerName, CString strLotName, CStringArray &strSerialName);
	BOOL GetSerialName(int nIndex, CString strModelName, CString strlayerName, CString strLotName, CString &strSerialName);
	BOOL GetSerialCode(CString strModelName, CString strlayerName, CString strLotName, CString strSerialName, CString &strSerialCode);
	BOOL LoadStripExtentBySTRIP(CString strModelCode, int nNumOfRegion, vector <DRECT> &vecExtentList);
	int GetStripRegionPosByModel(CString strModelName, CStringArray &strStripRegionList);
	BOOL GetStripRegionByLot(CString strLotCode, CStringArray &strStripRegionList);
	int  GetStripNameList(CString strModelName, CStringArray &strStripName);
	BOOL GetStripName(int nIndex, CString strModelName, CString &strStripName);
	BOOL GetStripCode(CString strModelName, CString strStripName, CString &strStripCode);
	int GetPieceCenterposByModel(CString strModelName, float *fCenterx, float *fCentery);
	BOOL GetNumberOfPieceRegionFromMODEL(CString strModelCode, int &nNumOfRegion);
	BOOL LoadPieceExtentByPIECE(CString strModelCode, int nNumOfRegion, vector <DRECT> &vecExtentList);
	BOOL GetPieceRegionByLot(CString strLotCode, CStringArray &strPieceRegionList);
	int GetPieceRegionPosByModel(CString strModelName, CStringArray &strPieceRegionList);
	int GetPieceNameList(CString strModelName, CStringArray &strPieceName);
	BOOL LoadDefectTable(int* pDefCode, COLORREF* pDefColor, CString* pKorN, CString * pEngN, int* pMaxR, int* pMaxC);
	int GetDefectSize(CString strLotCode, CString strLayerCode, CString strSerialCode, BOOL bBadFileMode);
	BOOL GetRotateCode(CString strModelName, CString strLotName, CString strLayerName, CString &strRotateCode);
	int GetMirrorTypeNameList(CStringArray &strMirrorName);
	BOOL GetMirrorTypeName(int nIndex, CString &strMirrorName);
	BOOL GetMirrorTypeCode(CString strModelName, CString strLotName, CString strLayerName, CString &strMirrorCode);
	BOOL FindMirrorTypeCode(CString strMirrorName, CString &strMirrorCode);
};

