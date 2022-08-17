#include "stdafx.h"
#include "Query.h"

#include "../Global/Token.h"
#include "../Global/MsgBoxTimeout.h"

#include "SafeLock.h"
CCriticalSection g_LogLockQuery;

CQuery::CQuery()
{
	m_bInitDB = FALSE;
}


CQuery::~CQuery()
{
}


void CQuery::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword)
{
	if (!m_bInitDB)
	{
		m_bInitDB = TRUE;
		m_dataSource.InitDB(szServerIP, szCatalog, szUserID, szUserPassword);
	}
}

void CQuery::InitDB(LPCTSTR szServerIP, LPCTSTR szCatalog, LPCTSTR szUserID, LPCTSTR szUserPassword, int nDbType)
{
	if (!m_bInitDB)
	{
		m_bInitDB = TRUE;
		m_dataSource.InitDB(szServerIP, szCatalog, szUserID, szUserPassword, nDbType);
	}
}

int CQuery::GetCustomerNameList(CStringArray &strCustomerName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("CUSTOMER");

	strCustomerName.RemoveAll();

	strQuery.Format(_T("SELECT KOR_NAME FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetCustomerNameList()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch business area name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strCustomerName.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}


	CString strMsg;
	strMsg.Format(_T("Not found customer name for this customer index"));
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::FindCustomerCode(CString strCustomerName, CString &strCustomerCode)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("CUSTOMER");
	strQuery.Format(_T("SELECT KOR_NAME, CUST_CODE FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at FindCustomerCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch customer name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			if (strData == strCustomerName)
			{
				// fetch customer code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strCustomerCode = strData;
				return TRUE;
			}
			m_dataSource.m_pRS->MoveNext();
		}
	}


	CString strMsg;
	strMsg.Format(_T("Not found customer code for this customer name:%s"), strCustomerName);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

int CQuery::GetUserInfo(CStringArray &sUserID, CStringArray &sPW, CStringArray &sKName, CStringArray &sEName, CStringArray &sJName, CStringArray &sCName, CStringArray &sAccLv)
{
	CString strQuery;
	int nMaxCols, nMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	strQuery.Format(_T("SELECT * FROM USERINFO"));
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetUserInfoFromDBForVrs()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}

	sUserID.RemoveAll();
	sPW.RemoveAll();
	sKName.RemoveAll();
	sEName.RemoveAll();
	sJName.RemoveAll();
	sCName.RemoveAll();
	sAccLv.RemoveAll();

	nMaxCols = m_dataSource.m_pRS->Fields->Count;
	nMaxRows = m_dataSource.m_pRS->RecordCount;

	if (nMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < nMaxRows; lRow++)
		{

			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sUserID.Add(strData);
			}
			else
			{
				sUserID.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sPW.Add(strData);
			}
			else
			{
				sPW.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)3]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sKName.Add(strData);
			}
			else
			{
				sKName.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)4]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sEName.Add(strData);
			}
			else
			{
				sEName.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)5]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sJName.Add(strData);
			}
			else
			{
				sJName.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)6]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sCName.Add(strData);
			}
			else
			{
				sCName.Add(_T(""));
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)8]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				sAccLv.Add(strData);
			}
			else
			{
				sAccLv.Add(_T(""));
			}

			m_dataSource.m_pRS->MoveNext();
		}
	}
	return nMaxRows;
}

int CQuery::GetUserNameList(CStringArray &strUserName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("USERINFO");

	strUserName.RemoveAll();

	strQuery.Format(_T("SELECT KOR_NAME, ID_USER FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetUserNameList()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch user name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strUserName.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}

	CString strMsg;
	strMsg.Format(_T("Not found user name for this user index"));
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

int CQuery::GetUserList(CUser* pList, int& nUserCount)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("USERINFO");

	nUserCount = 0;

	strQuery.Format(_T("SELECT ID_USER, PASSWD,LOCAL_NAME,ACCESS_LEVEL FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetUserNameList()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			pList[lRow].m_strUserID = strData;

			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;

				pList[lRow].m_strPwd = strData;
			}
			else
			{
				pList[lRow].m_strPwd = _T("");
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)2]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;

				pList[lRow].m_strUserName = strData;
			}
			else
			{
				pList[lRow].m_strUserName = _T("");
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)3]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_INT);
				int nAccessLevel = vValue.intVal;

				pList[lRow].m_nAccessLevel = nAccessLevel + 1;
			}
			else
			{
				pList[lRow].m_nAccessLevel = OPERATOR;
			}

			nUserCount++;
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}

	CString strMsg;
	strMsg.Format(_T("Not found user name for this user index"));
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetUserName(int nIndex, CString &strUserName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("USERINFO");
	strQuery.Format(_T("SELECT KOR_NAME, ID_USER FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetUserName()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	{
		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0 && lMaxRows > nIndex)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < nIndex; lRow++)
				m_dataSource.m_pRS->MoveNext();

			// fetch user name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strUserName = vValue.bstrVal;
			return TRUE;

		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found user name for this user index:%d"), nIndex);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::FindUserCode(CString strUserName, CString &strUserCode)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	_variant_t vColName, vValue;
	CString strData;
	CString strTableName = _T("USERINFO");
	strQuery.Format(_T("SELECT KOR_NAME, ID_USER FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at FindUserCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	{
		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch user name
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strUserName)
				{
					// fetch user code & return
					vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
					vValue.ChangeType(VT_BSTR);
					strData = vValue.bstrVal;
					strUserCode = strData;
					return TRUE;
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}
	}

	if (strData != _T(""))	// 110712 jsy
		strUserCode = strData;

	CString strMsg;
	strMsg.Format(_T("Not found user code for this user name:%s"), strUserName);
	//	MessageBoxTimeout(NULL, strMsg,_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;

}

int CQuery::GetEquipmentNameList(CStringArray &strEquipName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("EQUIPMENT");

	strEquipName.RemoveAll();

	strQuery.Format(_T("SELECT EQUIP_NAME, EQUIP_CODE FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at FindCustomerCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch equipment name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strEquipName.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;

	}

	CString strMsg;
	strMsg.Format(_T("Not found equipment name for this equipment index"));
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetEquipmentName(int nIndex, CString &strEquipName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("EQUIPMENT");
	strQuery.Format(_T("SELECT EQUIP_NAME, EQUIP_CODE FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetEquipmentName()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0 && lMaxRows > nIndex)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < nIndex; lRow++)
			m_dataSource.m_pRS->MoveNext();

		// fetch equipment name
		vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
		vValue.ChangeType(VT_BSTR);
		strEquipName = vValue.bstrVal;
		return TRUE;

	}


	CString strMsg;
	strMsg.Format(_T("Not found equipment name for this equipment index:%d"), nIndex);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::FindEquipCode(CString strEquipName, CString &strEquipCode)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strTableName = _T("EQUIPMENT");
	strQuery.Format(_T("SELECT EQUIP_NAME, EQUIP_CODE FROM %s"), strTableName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at FindEquipCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();

		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch equipment name
			if (m_dataSource.m_pRS->Fields->Item[(long)0]->Value.vt != VT_NULL)
			{
				if (m_dataSource.m_pRS->Fields->Item[(long)0]->Value.vt != VT_EMPTY)
				{
					int ttt = 0;
					vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				}
				else
				{
					vValue.iVal = 0;
					return FALSE;
				}
			}
			else
			{
				vValue.iVal = 0;
				return FALSE;
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			if (strData == strEquipName)
			{
				// fetch equipment code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strEquipCode = strData;
				return TRUE;
			}
			m_dataSource.m_pRS->MoveNext();
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found equipment code for this equipment name:%s"), strEquipName);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

CString CQuery::GetCamInfoData(CString strModelName, CString strLayerCode, CString strLayerName)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData, strData2;
	_variant_t vColName, vValue;
	CString MasterPath = _T("");


	// Get piece region for model
	CString strModelCode;
	GetModelCode(strModelName, strModelCode);

	strQuery.Format(_T("SELECT * FROM CAMINFO WHERE MODEL_CODE = '%s' AND LAYER_CODE = '%s' AND LAYER_NAME = '%s'"), strModelCode,
		strLayerCode,
		strLayerName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetCamInfoData()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return _T("");
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;     //master offsetx
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				MasterPath = strData;
			}
			else
			{
				MasterPath = _T("");
			}
		}
	}

	return MasterPath;

}

CString CQuery::GetCamSpecDirFromRSTH(CString strLotCode, CString strLayerCode, CString strLayerName)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData, strData2;
	_variant_t vColName, vValue;
	CString MasterPath = _T("");

	strQuery.Format(_T("SELECT CAM_SPEC_DIR FROM RSTH WHERE LOT_CODE = '%s' AND LAYER_CODE = '%s' AND LAYER_NAME = '%s'"), strLotCode, strLayerCode, strLayerName);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetCamSpecDirFromRSTH()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return _T("");
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		//	for(long lRow=0; lRow<lMaxRows; lRow++)
		{
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;     //master offsetx
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				MasterPath = strData;
			}
			else
			{
				MasterPath = _T("");
			}
		}
	}

	return MasterPath;

}

BOOL CQuery::GetInspectionRegionByModel(CString strModelName, double &fWidth, double &fHeight)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	CString strModelCode;
	GetModelCode(strModelName, strModelCode);
	// Get inspection area size for Lot
	strQuery.Format(_T("SELECT PN_HOR, PN_VER FROM MODEL WHERE MODEL_CODE = '%s'"), strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetInspectionRegionByModel()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();

		vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
		vValue.ChangeType(VT_BSTR);
		strData = vValue.bstrVal;
		fWidth = _ttof(strData);

		vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
		vValue.ChangeType(VT_BSTR);
		strData = vValue.bstrVal;
		fHeight = _ttof(strData);

		return TRUE;
	}

	return FALSE;
}

BOOL CQuery::GetModelCode(CString strModelName, CString &strModelCode)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strTableName = _T("");
	strTableName.Format(_T("MODEL"));

	strQuery.Format(_T("SELECT MODEL_CODE FROM MODEL WHERE MODEL_NAME = '%s'"), strModelName);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetModelCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg);
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData = _T("");

		m_dataSource.m_pRS->MoveFirst();

		// fetch model name
		vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
		vValue.ChangeType(VT_BSTR);
		strData = vValue.bstrVal;
		strData.MakeUpper();
		strModelCode = strData;

		return TRUE;
	}

	CString strMsg;
	strMsg.Format(_T("Not found model code for this model name:%s"), strModelName);
	AfxMessageBox(strMsg);
	Log(strMsg);

	return FALSE;
}

int CQuery::GetLotSize(CString strModelName)
{
	CString strModelCode;
	long lMaxRows = 0;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		CString strTableName = _T("LOT");

		strQuery.Format(_T("SELECT LOT_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLotSize()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		}
		else
			lMaxRows = m_dataSource.m_pRS->RecordCount;
	}
	return lMaxRows;
}

int CQuery::GetLotNameList(CStringArray &strLotArray, CString strModelName, CString strStartDate, CString strEndDate)
{
	int nCount;

	CString strQuery;
	CString strStart, strEnd;
	long lMaxCols, lMaxRows;
	CString strModelCode, strName, strData;
	_variant_t vColName, vValue;

	strLotArray.RemoveAll();

	if (strModelName.Compare(_T("")) == 0)
	{
		strQuery.Format(_T("SELECT LOT_CODE FROM LOT WHERE DT_REGIST BETWEEN '%s' AND '%s'"), strStartDate, strEndDate);
	}
	else
	{
		GetModelCode(strModelName, strModelCode);
		strQuery.Format(_T("SELECT LOT_CODE FROM LOT WHERE MODEL_CODE = '%s'"), strModelCode);
	}

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLotNameList()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg);	AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		nCount = 0;
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			for (long lCol = 0; lCol < lMaxCols; lCol++)
			{
				vValue = m_dataSource.m_pRS->Fields->Item[(long)lCol]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
			}

			strLotArray.Add(strData);
			m_dataSource.m_pRS->MoveNext();
			nCount++;
		}
		return nCount;
	}
	return FALSE;
}

BOOL CQuery::GetLotName(int nIndex, CString strModelName, CString &strLotName)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("LOT");

		strQuery.Format(_T("SELECT LOT_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLotName()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0 && lMaxRows > nIndex)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < nIndex; lRow++)
				m_dataSource.m_pRS->MoveNext();

			// fetch lot name & return
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strLotName = vValue.bstrVal;
			return TRUE;
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found lot name for this model and lot index:%s/%d"), strModelName, nIndex);
	AfxMessageBox(strMsg);
	Log(strMsg);
	return FALSE;
}

BOOL CQuery::GetLotCode(CString strModelName, CString strLotName, CString &strLotCode)
{
	CString strModelCode;

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("LOT");

		strQuery.Format(_T("SELECT LOT_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLotCode()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strLotName)
				{
					strLotCode = strData;
					return TRUE;
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found lot code for this model and lot name:%s/%s"), strModelName, strLotName);
	AfxMessageBox(strMsg);
	Log(strMsg);
	return FALSE;
}

BOOL CQuery::GetLayerCode(CString strModelName, CString strLayerName, CString &strLayerCode)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows;
		//CString strTableName = _T("LAYER");
		//CString strTableName = _T("CAMINFO");

		strQuery.Format(_T("SELECT LAYER_CODE FROM CAMINFO WHERE MODEL_CODE = '%s' AND LAYER_NAME = '%s'"), strModelCode, strLayerName);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLayerCodeFromCamInfo()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;

			m_dataSource.m_pRS->MoveFirst();

			// fetch layer name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strData.MakeUpper();
			strLayerCode = strData;
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found layer code for this model and layer name:%s/%s"), strModelName, strLayerName);
	AfxMessageBox(strMsg);
	Log(strMsg);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

int CQuery::GetLayerNameList(CString strModelName, CStringArray &strLayerName)
{
	CString strModelCode;
	strLayerName.RemoveAll();

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("CAMINFO");

		//if (pVrsDoc->m_bDBVersion)
		//	strQuery.Format(_T("SELECT LAYER_NAME FROM %s WHERE MODEL_CODE = '%s' AND MODEL_NAME = '%s'"), strTableName, strModelCode, strModelName);
		//else
			strQuery.Format(_T("SELECT LAYER_NAME FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);

		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLayerNameList()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return 0;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch layer name
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strLayerName.Add(strData);
				m_dataSource.m_pRS->MoveNext();
			}
			return lMaxRows;
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found layer name for this model and layer index:%s"), strModelName);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetLayerName(int nIndex, CString strModelName, CString &strLayerName)
{
	CStringArray strArrayName;
	int nCount = GetLayerNameList(strModelName, strArrayName);
	if (nCount > nIndex)
	{
		strLayerName = strArrayName.GetAt(nIndex);
		return TRUE;
	}

	CString strMsg;
	strMsg.Format(_T("Not found layer name for this model and layer index:%s/%d"), strModelName, nIndex);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

int  CQuery::GetLayerCodeList(CString strModelName, CStringArray &strLayerCode)
{
	CString strModelCode;
	strLayerCode.RemoveAll();

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		//		CString strTableName = _T("LAYER");
		CString strTableName = _T("CAMINFO");

		strQuery.Format(_T("SELECT LAYER_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);

		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLayerCodeList()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return 0;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch layer name
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strLayerCode.Add(strData);
				m_dataSource.m_pRS->MoveNext();
			}
			return lMaxRows;
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found layer code for this model and layer index:%s"), strModelName);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetLayerCodeFromCamInfo(CString strModelName, CString strLayerName, CString &strLayerCode)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows;
		//CString strTableName = _T("LAYER");
		//CString strTableName = _T("CAMINFO");

		strQuery.Format(_T("SELECT LAYER_CODE FROM CAMINFO WHERE MODEL_CODE = '%s' AND LAYER_NAME = '%s'"), strModelCode, strLayerName);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLayerCodeFromCamInfo()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;

			m_dataSource.m_pRS->MoveFirst();

			// fetch layer name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strData.MakeUpper();
			strLayerCode = strData;
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found layer code for this model and layer name:%s/%s"), strModelName, strLayerName);
	AfxMessageBox(strMsg);
	Log(strMsg);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::GetLayerCodeFromLayer(CString strModelName, CString strLayerName, CString &strLayerCode)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows;
		CString strTableName = _T("LAYER");

		strQuery.Format(_T("SELECT LAYER_CODE FROM %s WHERE MODEL_CODE = '%s' AND LAYER_NAME = '%s'"), strTableName, strModelCode, strLayerName);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetLayerCodeFromLayer()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;

			m_dataSource.m_pRS->MoveFirst();

			// fetch layer name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strData.MakeUpper();
			strLayerCode = strData;
			return TRUE;
		}
	}
	CString strMsg;
	strMsg.Format(_T("Not found layer code for this model and layer name:%s/%s"), strModelName, strLayerName);
	AfxMessageBox(strMsg);
	Log(strMsg);
	return FALSE;
}

int CQuery::GetSerialNameList(CString strModelName, CString strLayerName, CString strLotName, CStringArray &strSerialName)
{
	CString strModelCode;
	CString strLayerCode;
	CString strCamPath;

	strSerialName.RemoveAll();

	if (GetLayerCodeFromCamInfo(strModelName, strLayerName, strLayerCode))
	{
		//strCamPath = GetCamInfoData(strModelName, strLayerCode, strLayerName);
		strCamPath = GetCamSpecDirFromRSTH(strLotName, strLayerCode, strLayerName);	//2016.01.18 hyk

		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("SERIAL");

#ifdef FEATURE_USE_MARIA_DB

		if (pVrsDoc->m_bDBVersion)
		{
			strQuery.Format(_T("CALL USP_LOAD_VRSCOMBOSRL ('%s', '%s', '%s', '%s', '%s')"),
				strCamPath,
				strLotName,
				strLayerCode,
				strLayerName,
				strModelName);
		}
		else
		{
			strQuery.Format(_T("CALL USP_LOAD_VRSCOMBOSRL ('%s', '%s', '%s', '%s')"),
				strCamPath,
				strLotName,
				strLayerCode,
				strLayerName);
		}

		// 		strQuery.Format(_T("CALL USP_LOAD_VRSCOMBOSRL ('%s', '%s', '%s', '%s')"),
		// 													strCamPath,
		// 													strLotName,
		// 													strLayerCode,
		// 													strLayerName );
#else
		//if (pVrsDoc->m_bDBVersion)
		//{
		//	strQuery.Format(_T("EXEC USP_LOAD_VRSCOMBOSRL @CAM_SPEC_DIR = '%s', @LOT_CODE = '%s', @LAYER_CODE = '%s', @LAYER_NAME = '%s', @MODEL_NAME = '%s'"),
		//		strCamPath,
		//		strLotName,
		//		strLayerCode,
		//		strLayerName,
		//		strModelName);
		//}
		//else
		{
			strQuery.Format(_T("EXEC USP_LOAD_VRSCOMBOSRL @CAM_SPEC_DIR = '%s', @LOT_CODE = '%s', @LAYER_CODE = '%s', @LAYER_NAME = '%s'"),
				strCamPath,
				strLotName,
				strLayerCode,
				strLayerName);
		}

		// 		strQuery.Format(_T("EXEC USP_LOAD_VRSCOMBOSRL @CAM_SPEC_DIR = '%s', @LOT_CODE = '%s', @LAYER_CODE = '%s', @LAYER_NAME = '%s'"),
		// 													strCamPath,
		// 													strLotName,
		// 													strLayerCode,
		// 													strLayerName );
#endif

		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetSerialNameList()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return 0;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;


		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strSerialName.Add(strData);
				m_dataSource.m_pRS->MoveNext();
			}
			return lMaxRows;
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found serial name for this model , layer, lot and serial index:%s/%s/%s"), strModelName, strLayerName, strLotName);
	// 	AfxMessageBox(strMsg);
	// 	Log(strMsg);
	return 0;
}

BOOL CQuery::GetSerialName(int nIndex, CString strModelName, CString strlayerName, CString strLotName, CString &strSerialName)
{
	CString strModelCode;
	CString strLayerCode;

	if (GetLayerCodeFromCamInfo(strModelName, strlayerName, strLayerCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("SERIAL");

		strQuery.Format(_T("SELECT SERIAL_CODE FROM %s WHERE LAYER_CODE = '%s' AND LOT_CODE = '%s'"), strTableName, strLayerCode, strLotName);

		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetSerialName()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0 && lMaxRows > nIndex)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();

			for (lRow = 0; lRow < nIndex; lRow++)
				m_dataSource.m_pRS->MoveNext();

			// fetch lot code & return
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strSerialName = vValue.bstrVal;
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found serial name for this model , layer, lot and serial index:%s/%s/%s/%d"), strModelName, strlayerName, strLotName, nIndex);
	AfxMessageBox(strMsg);
	Log(strMsg);
	return FALSE;
}

BOOL CQuery::GetSerialCode(CString strModelName, CString strlayerName, CString strLotName, CString strSerialName, CString &strSerialCode)
{
	CString strModelCode;
	CString strLayerCode;

	if (GetLayerCodeFromCamInfo(strModelName, strlayerName, strLayerCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;

		strQuery.Format(_T("SELECT SERIAL_CODE FROM SERIAL WHERE LAYER_CODE = '%s' AND LOT_CODE = '%s'"), strLayerCode, strLotName);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetSerialCode()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strSerialName)
				{
					strSerialCode = strData;
					return TRUE;
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found serial code for this model , layer, lot and serial name:%s/%s/%s/%s"), strModelName, strlayerName, strLotName, strSerialName);
	AfxMessageBox(strMsg);
	Log(strMsg);
	return FALSE;
}

BOOL CQuery::LoadStripExtentBySTRIP(CString strModelCode, int nNumOfRegion, vector <DRECT> &vecExtentList)
{
	CToken token;
	CString strQuery;
	long lMaxCols = 0, lMaxRows = 0;

	int i = 0;
	int nStripCode = 0;
	CString strCoord = _T("");

	_variant_t vValue;
	CString strData = _T("");

	double dValue;
	DRECT fExtent;
	vecExtentList.clear();

	strQuery.Format(_T("SELECT STRIP_CODE, COORDINATE FROM STRIP WHERE MODEL_CODE = '%s' ORDER BY STRIP_CODE"), strModelCode);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at LoadStripExtentBySTRIP()\r\n%s"), m_dataSource.GetLastError());
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();

		for (i = 0; i < lMaxRows; i++)
		{
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			nStripCode = _ttoi(strData);
			if (nStripCode > nNumOfRegion)
				return FALSE;

			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			token.Init(strData, ',');
			strCoord = token.GetNextToken();
			if (strCoord.IsEmpty())
				return FALSE;

			fExtent.left = _tcstod(strCoord, NULL);

			strCoord = token.GetNextToken();
			if (strCoord.IsEmpty())
				return FALSE;
			fExtent.top = _tcstod(strCoord, NULL);

			strCoord = token.GetNextToken();
			if (strCoord.IsEmpty())
				return FALSE;
			fExtent.right = _tcstod(strCoord, NULL);

			strCoord = token.GetNextToken();
			if (strCoord.IsEmpty())
				return FALSE;
			fExtent.bottom = _tcstod(strCoord, NULL);

			if (fExtent.left > fExtent.right)
			{
				dValue = fExtent.left;
				fExtent.left = fExtent.right;
				fExtent.right = dValue;
			}
			if (fExtent.bottom > fExtent.top)
			{
				dValue = fExtent.bottom;
				fExtent.bottom = fExtent.top;
				fExtent.top = dValue;
			}

			vecExtentList.push_back(fExtent);

			m_dataSource.m_pRS->MoveNext();
		}

		return TRUE;
	}
	return FALSE;
}

BOOL CQuery::GetNumberOfStripRegionFromMODEL(CString strModelCode, int &nNumOfRegion)
{
	long lMaxRows = 0, lMaxCols = 0;
	CString strQuery, strData;
	_variant_t vValue;

	CString strTableName = _T("MODEL");

	strQuery.Format(_T("SELECT STRIP_AMT FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceSize()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;     //master offsetx
		if (vValue.vt != VT_NULL)
		{
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			nNumOfRegion = _ttoi(strData);
			return TRUE;
		}
	}

	return FALSE;
}

int CQuery::GetStripRegionPosByModel(CString strModelName, CStringArray &strStripRegionList)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	// Get piece region for model
	CString strModelCode;

	strStripRegionList.RemoveAll();

	GetModelCode(strModelName, strModelCode);
	strQuery.Format(_T("SELECT STRIP_CODE, COL, ROW, COORDINATE FROM STRIP WHERE MODEL_CODE = '%s'"), strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetStripRegionByModel()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			for (long lCol = 0; lCol < lMaxCols; lCol++)
			{
				if (lCol == 3)
				{
					vValue = m_dataSource.m_pRS->Fields->Item[(long)lCol]->Value;
					vValue.ChangeType(VT_BSTR);
					strData += vValue.bstrVal;
				}
			}
			//	strData.Delete(strData.GetLength()-1);
			strStripRegionList.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}
	return FALSE;
}

BOOL CQuery::GetStripRegionByLot(CString strLotCode, CStringArray &strStripRegionList)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	strStripRegionList.RemoveAll();

	// Get strip region for Lot
	strQuery.Format(_T("SELECT S.STRIP_CODE, S.COL, S.ROW, S.COORDINATE FROM STRIP S JOIN LOT L ON S.MODEL_CODE = L.MODEL_CODE WHERE L.LOT_CODE = '%s'"), strLotCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetStripRegionByLot()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			for (long lCol = 0; lCol < lMaxCols; lCol++)
			{
				vValue = m_dataSource.m_pRS->Fields->Item[(long)lCol]->Value;
				vValue.ChangeType(VT_BSTR);
				strData += vValue.bstrVal;
				strData += _T(",");
			}
			strStripRegionList.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return TRUE;
	}
	return FALSE;
}

int  CQuery::GetStripNameList(CString strModelName, CStringArray &strStripName)
{
	CString strModelCode;
	strStripName.RemoveAll();

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("STRIP");
		strQuery.Format(_T("SELECT STRIP_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetStripNameList()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch strip name(Number) & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strStripName.Add(strData);
				m_dataSource.m_pRS->MoveNext();
			}
			return lMaxRows;
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found strip name for this model and strip index:%s"), strModelName);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetStripName(int nIndex, CString strModelName, CString &strStripName)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("STRIP");
		strQuery.Format(_T("SELECT STRIP_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetStripName()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0 && lMaxRows > nIndex)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < nIndex; lRow++)
				m_dataSource.m_pRS->MoveNext();

			// fetch strip name(Number) & return
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strStripName = vValue.bstrVal;
			return TRUE;
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found strip name for this model and strip index:%s/%d"), strModelName, nIndex);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::GetStripCode(CString strModelName, CString strStripName, CString &strStripCode)
{
	CString strModelCode;
	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("STRIP");
		strQuery.Format(_T("SELECT STRIP_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetStripCode()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;

			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strStripName)
				{
					strStripCode = strData;
					return TRUE;
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found strip code for this model and strip name:%s/%s"), strModelName, strStripName);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::GetNumberOfPieceRegionFromMODEL(CString strModelCode, int &nNumOfRegion)
{
	long lMaxRows = 0, lMaxCols = 0;
	CString strQuery, strData;
	_variant_t vValue;
	CString strTableName = _T("MODEL");

	nNumOfRegion = 0;

	strQuery.Format(_T("SELECT PIECE_AMT FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceSize()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;     //master offsetx
		if (vValue.vt != VT_NULL)
		{
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			nNumOfRegion = _ttoi(strData);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CQuery::LoadPieceExtentByPIECE(CString strModelCode, int nNumOfRegion, vector <DRECT> &vecExtentList)
{
	CToken token;
	CString strQuery;
	long lMaxCols = 0, lMaxRows = 0;

	int i = 0, j, nIndex = 0;
	int nPieceCode = 0, nS = 0, nE = 0;
	CString strCoord = _T("");
	CString strMsg;

	DPOINT fpt;
	vector <DPOINT> vecPointList;
	DRECT fExtent;

	_variant_t vValue;
	CString strData = _T("");

	vecPointList.clear();
	vecExtentList.clear();

	//// Piece Region
	strQuery.Format(_T("SELECT PIECE_CODE, COORDINATE FROM PIECE WHERE MODEL_CODE = '%s' ORDER BY PIECE_CODE"), strModelCode);

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{

		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at LoadPieceExtentByPIECE()\r\n%s"), m_dataSource.GetLastError());
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();

		for (j = 0; j < lMaxRows; j++)
		{
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			nPieceCode = _ttoi(strData);
			if (nPieceCode > nNumOfRegion)
				return FALSE;

			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;

			nS = 0;
			nE = strData.Find(_T(":"));
			if (nE == -1)
				return FALSE;

			strData = strData.Mid(nE + 1);
			token.Init(strData, ',');

			nIndex = 0;
			while (1)
			{
				nIndex++;
				strCoord = token.GetNextToken();
				if (strCoord.IsEmpty())
					break;

				if (nIndex % 2 == 1)
				{
					fpt.x = _tcstod(strCoord, NULL);
				}
				else
				{
					fpt.y = _tcstod(strCoord, NULL);

					vecPointList.push_back(fpt);
				}
			}

			fExtent.left = 999999;
			fExtent.top = -999999;
			fExtent.right = -999999;
			fExtent.bottom = 999999;
			for (i = 0; i < (int)vecPointList.size(); i++)
			{
				if (fExtent.left > vecPointList.at(i).x)		fExtent.left = vecPointList.at(i).x;
				if (fExtent.top < vecPointList.at(i).y)		fExtent.top = vecPointList.at(i).y;
				if (fExtent.right < vecPointList.at(i).x)	fExtent.right = vecPointList.at(i).x;
				if (fExtent.bottom > vecPointList.at(i).y)	fExtent.bottom = vecPointList.at(i).y;
			}

			vecExtentList.push_back(fExtent);
			vecPointList.clear();

			m_dataSource.m_pRS->MoveNext();
		}

		return TRUE;
	}
	return FALSE;
}

int CQuery::GetPieceCenterposByModel(CString strModelName, float *fCenterx, float *fCentery)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;
	CString strTemp;
	int lenth;
	int width = 0;
	TCHAR cstring;
	TCHAR Buffer[1000] = { 0, };

	// Get piece region for model
	CString strModelCode;
	GetModelCode(strModelName, strModelCode);
	strQuery.Format(_T("SELECT PIECE_CODE, CENTER_POS FROM PIECE WHERE MODEL_CODE = '%s' ORDER BY PIECE_CODE"), strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceRegionByModel()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			width = 0;


			if (m_dataSource.m_pRS->Fields->Item[(long)1]->Value.vt != VT_NULL)
			{
				if (m_dataSource.m_pRS->Fields->Item[(long)1]->Value.vt != VT_EMPTY)
					vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
				else
				{
					vValue.iVal = 0;
					//return FALSE;
				}
			}
			else
			{
				vValue.iVal = 0;
				//return FALSE;
			}

			vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
			if (vValue.vt != VT_NULL) //130107 jun
			{
				vValue.ChangeType(VT_BSTR);
				strTemp = vValue.bstrVal;

				lenth = strTemp.GetLength();
				for (int i = 0; i < lenth; i++)
				{
					cstring = strTemp[i];

					if (cstring == ',')
					{
						fCenterx[lRow] = (float)(_ttof(Buffer));
						memset(Buffer, 0, 100);
						width = 0;
					}
					else
					{
						Buffer[width] = cstring;
						width++;
					}
				}
				fCentery[lRow] = (float)(_ttof(Buffer));
				memset(Buffer, 0, 100);
				width = 0;
			}
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}

	return 0;
}

int CQuery::GetPieceRegionPosByModel(CString strModelName, CStringArray &strPieceRegionList)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	// Get piece region for model
	CString strModelCode;

	strPieceRegionList.RemoveAll();

	GetModelCode(strModelName, strModelCode);
	strQuery.Format(_T("SELECT PIECE_CODE, COL, ROW, COORDINATE FROM PIECE WHERE MODEL_CODE = '%s'"), strModelCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceRegionByModel()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			for (long lCol = 0; lCol < lMaxCols; lCol++)
			{
				if (lCol == 3)
				{
					vValue = m_dataSource.m_pRS->Fields->Item[(long)lCol]->Value;
					vValue.ChangeType(VT_BSTR);
					strData += vValue.bstrVal;
				}
			}
			strPieceRegionList.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}
	return 0;
}

BOOL CQuery::GetPieceRegionByLot(CString strLotCode, CStringArray &strPieceRegionList)
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData;
	_variant_t vColName, vValue;

	strPieceRegionList.RemoveAll();

	// Get piece region for Lot
	strQuery.Format(_T("SELECT S.PIECE_CODE, S.COL, S.ROW, S.COORDINATE FROM PIECE S JOIN LOT L ON S.MODEL_CODE = L.MODEL_CODE WHERE L.LOT_CODE = '%s'"), strLotCode);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceRegionByLot()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;
	if (lMaxRows > 0)
	{
		m_dataSource.m_pRS->MoveFirst();
		for (long lRow = 0; lRow < lMaxRows; lRow++)
		{
			strData = _T("");
			for (long lCol = 0; lCol < lMaxCols; lCol++)
			{
				vValue = m_dataSource.m_pRS->Fields->Item[(long)lCol]->Value;
				vValue.ChangeType(VT_BSTR);
				strData += vValue.bstrVal;
				strData += _T(",");
			}
			strData.Delete(strData.GetLength() - 1);
			strPieceRegionList.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return TRUE;
	}
	return FALSE;
}

int  CQuery::GetPieceNameList(CString strModelName, CStringArray &strPieceName)
{
	CString strModelCode;
	strPieceName.RemoveAll();

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		CString strTableName = _T("PIECE");
		strQuery.Format(_T("SELECT PIECE_CODE FROM %s WHERE MODEL_CODE = '%s'"), strTableName, strModelCode);
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetPieceNameList()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return 0;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();

			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch piece name(Number) & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				strPieceName.Add(strData);
				m_dataSource.m_pRS->MoveNext();
			}
			return lMaxRows;

		}

	}
	CString strMsg;
	strMsg.Format(_T("Not found piece name for this model and piece index:%s"), strModelName);
	MessageBoxTimeout(NULL, strMsg, _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::LoadDefectTable(int* pDefCode, COLORREF* pDefColor, CString* pKorN, CString * pEngN, int* pMaxR, int* pMaxC)
{
	CString strQuery;
	strQuery.Format(_T("SELECT DEF_CODE,DEF_COLOR,KOR_NAME,ENG_NAME FROM `gvisdb`.`defect`"));

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))	//2012.09.01 jun
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at NotifyVMSEnd()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	long lMaxCols = *pMaxC = m_dataSource.m_pRS->Fields->Count;
	long lMaxRows = *pMaxR =m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (int lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch lot code & return
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			if (vValue.vt != VT_NULL)
			{
				vValue.ChangeType(VT_INT);
				pDefCode[lRow] = vValue.intVal;

				vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
				if (vValue.vt != VT_NULL)
				{
					vValue.ChangeType(VT_INT);
					pDefColor[lRow] = (COLORREF)vValue.intVal;
				}

				vValue = m_dataSource.m_pRS->Fields->Item[(long)2]->Value;
				if (vValue.vt != VT_NULL)
				{
					vValue.ChangeType(VT_BSTR);
					pKorN[lRow] = (CString)vValue.bstrVal;
				}

				vValue = m_dataSource.m_pRS->Fields->Item[(long)2]->Value;
				if (vValue.vt != VT_NULL)
				{
					vValue.ChangeType(VT_BSTR);
					pEngN[lRow] = (CString)vValue.bstrVal;
				}
			}
			m_dataSource.m_pRS->MoveNext();
		}

		return TRUE;
	}

	return FALSE;
}

int CQuery::GetDefectSize(CString strLotCode, CString strLayerCode, CString strSerialCode, BOOL bBadFileMode)  //2011.04.25 jun
{
	CString strQuery;
	long lMaxCols, lMaxRows;
	CString strName, strData, strTemp;
	_variant_t vColName, vValue;

	// Get defect data for specified serial
	CString strModelCode;

	if (bBadFileMode) // 101103 jsy
	{
		// 110329 JSY Modify
		strQuery.Format(_T("SELECT CAMERA_NO, X_POS, Y_POS, DEF_CODE, DEF_SIZE, PIECE_CODE, FINAL_CODE FROM RSTD A ,SERIAL B WHERE A.LOT_CODE = '%s' AND A.LAYER_CODE = '%s' AND B.SERIAL_CODE = '%s' AND A.LOT_CODE = B.LOT_CODE AND A.LAYER_CODE = B.LAYER_CODE AND A.SERIAL_CODE = B.LAYER_SERIAL AND A.FINAL_CODE <> '0' AND A.FINAL_CODE <> '-1'"),
			strLotCode, strLayerCode, strSerialCode);
	}
	else
	{
		strQuery.Format(_T("SELECT CAMERA_NO, X_POS, Y_POS, DEF_CODE, DEF_SIZE, PIECE_CODE, FINAL_CODE FROM RSTD A ,SERIAL B WHERE A.LOT_CODE = '%s' AND A.LAYER_CODE = '%s' AND B.SERIAL_CODE = '%s' AND A.LOT_CODE = B.LOT_CODE AND A.LAYER_CODE = B.LAYER_CODE AND A.SERIAL_CODE = B.LAYER_SERIAL ORDER BY A.DEF_ORDER"),
			strLotCode, strLayerCode, strSerialCode);
	}

	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetRSTData()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return 0;
	}
	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	return lMaxRows;
}

BOOL CQuery::GetRotateCode(CString strModelName, CString strLotName, CString strLayerName, CString &strRotateCode)
{
	CString strMsg;
	CString strModelCode;
	CString strSpecpath;

	CString strLotCode, strLayerCode;
	GetLotCode(strModelName, strLotName, strLotCode);
	GetLayerCode(strModelName, strLayerName, strLayerCode);
	strSpecpath = GetCamSpecDirFromRSTH(strLotCode, strLayerCode, strLayerName);
	//strSpecpath = pVrsDoc->m_strEachSpecPath[pVrsDoc->m_nAoiMachineId];	//2018.03.14 kjs

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		// 		CString strTableName = _T("LAYER");
		CString strTableName = _T("CAMINFO");

		//if (pVrsDoc->m_bDBVersion)
		//	strQuery.Format(_T("SELECT LAYER_NAME, ROTATION FROM %s WHERE MODEL_CODE = '%s' AND MODEL_NAME = '%s' AND CAM_SPEC_DIR = '%s'"), strTableName, strModelCode, strModelName, strSpecpath);	//2018.03.14 kjs
		//else
			strQuery.Format(_T("SELECT LAYER_NAME, ROTATION FROM %s WHERE MODEL_CODE = '%s' AND CAM_SPEC_DIR = '%s'"), strTableName, strModelCode, strSpecpath);	//2018.03.14 kjs

																																									// 		strQuery.Format(_T("SELECT LAYER_NAME, ROTATION FROM %s WHERE MODEL_CODE = '%s' AND CAM_SPEC_DIR = '%s'"), strTableName, strModelCode, strSpecpath);	//2018.03.14 kjs
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetRotateCode()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strLayerName)
				{
					vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
					if (vValue.vt != VT_NULL)   //jun 20110726 
					{
						vValue.ChangeType(VT_BSTR);
						strData = vValue.bstrVal;
						strRotateCode = strData;
						return TRUE;
					}
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}

	}

	strMsg.Format(_T("Not found rotate code for this model and layer name:%s/%s"), strModelName, strLayerName);
	Log(strMsg);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

int CQuery::GetMirrorTypeNameList(CStringArray &strMirrorName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strCodeGroup = CODE_GROUP_MIRROR_TYPE;

	strMirrorName.RemoveAll();

	strQuery.Format(_T("SELECT ENG_DETAIL, CODE_DETAIL FROM CODEINFO WHERE CODE_GROUP = %s"), strCodeGroup);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetMirrorTypeNameList()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	lMaxCols = m_dataSource.m_pRS->Fields->Count;
	lMaxRows = m_dataSource.m_pRS->RecordCount;

	if (lMaxRows > 0)
	{
		_variant_t vColName, vValue;
		CString strData;
		m_dataSource.m_pRS->MoveFirst();
		for (lRow = 0; lRow < lMaxRows; lRow++)
		{
			// fetch mirror name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strData = vValue.bstrVal;
			strMirrorName.Add(strData);
			m_dataSource.m_pRS->MoveNext();
		}
		return lMaxRows;
	}

	CString strMsg;
	strMsg.Format(_T("Not found mirror name for this mirror index"));
	MessageBoxTimeout(NULL, (strMsg), _T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return 0;
}

BOOL CQuery::GetMirrorTypeName(int nIndex, CString &strMirrorName)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strCodeGroup = CODE_GROUP_MIRROR_TYPE;
	strQuery.Format(_T("SELECT ENG_DETAIL, CODE_DETAIL FROM CODEINFO WHERE CODE_GROUP = %s"), strCodeGroup);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetMirrorTypeName()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	{
		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0 && lMaxRows > nIndex)
		{
			_variant_t vColName, vValue;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < nIndex; lRow++)
				m_dataSource.m_pRS->MoveNext();

			// fetch mirror name
			vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
			vValue.ChangeType(VT_BSTR);
			strMirrorName = vValue.bstrVal;
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found mirror name for this mirror index:%d"), nIndex);
	TRACE(_T("%s\n"), strMsg);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::GetMirrorTypeCode(CString strModelName, CString strLotName, CString strLayerName, CString &strMirrorCode)
{
	CString strModelCode;
	CString strSpecpath;

	CString strLotCode, strLayerCode;
	GetLotCode(strModelName, strLotName, strLotCode);
	GetLayerCode(strModelName, strLayerName, strLayerCode);
	strSpecpath = GetCamSpecDirFromRSTH(strLotCode, strLayerCode, strLayerName);
	//strSpecpath = pVrsDoc->m_strEachSpecPath[pVrsDoc->m_nAoiMachineId];	//2018.03.14 kjs

	if (GetModelCode(strModelName, strModelCode))
	{
		CString strQuery;
		long lMaxCols, lMaxRows, lRow;
		// 		CString strTableName = _T("LAYER");
		CString strTableName = _T("CAMINFO");

		//if (pVrsDoc->m_bDBVersion)
		//	strQuery.Format(_T("SELECT LAYER_NAME, MRR_TYPE FROM %s WHERE MODEL_CODE = '%s' AND MODEL_NAME = '%s' AND CAM_SPEC_DIR = '%s'"), strTableName, strModelCode, strModelName, strSpecpath);	//2018.03.14 kjs
		//else
			strQuery.Format(_T("SELECT LAYER_NAME, MRR_TYPE FROM %s WHERE MODEL_CODE = '%s' AND CAM_SPEC_DIR = '%s'"), strTableName, strModelCode, strSpecpath);	//2018.03.14 kjs

																																									// 		strQuery.Format(_T("SELECT LAYER_NAME, MRR_TYPE FROM %s WHERE MODEL_CODE = '%s' AND CAM_SPEC_DIR = '%s'"),strTableName,strModelCode, strSpecpath);	//2018.03.14 kjs
		if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
		{
			CString strMsg;
			strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at GetMirrorTypeCode()\r\n%s"), m_dataSource.GetLastError());
			Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
			return FALSE;
		}

		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch lot code & return
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strLayerName)
				{
					vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;

					if (vValue.vt != VT_NULL)   //jun 20110726 
					{
						vValue.ChangeType(VT_BSTR);
						strData = vValue.bstrVal;
						strMirrorCode = strData;
						return TRUE;
					}

				}
				m_dataSource.m_pRS->MoveNext();
			}
		}
	}
	CString strMsg;
	strMsg.Format(_T("Not found mirror type code for this model and layer name:%s/%s"), strModelName, strLayerName);
	TRACE(_T("%s\n"), strMsg);
	//MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}

BOOL CQuery::FindMirrorTypeCode(CString strMirrorName, CString &strMirrorCode)
{
	CString strQuery;
	long lMaxCols, lMaxRows, lRow;
	CString strCodeGroup = CODE_GROUP_MIRROR_TYPE;
	strQuery.Format(_T("SELECT ENG_DETAIL, CODE_DETAIL FROM CODEINFO WHERE CODE_GROUP = %s"), strCodeGroup);
	if (!m_dataSource.ExecuteQuery((LPCTSTR)strQuery))
	{
		CString strMsg;
		strMsg.Format(_T("Error occur at m_dataSource.ExecuteQuery() at FindMirrorTypeCode()\r\n%s"), m_dataSource.GetLastError());
		Log(strMsg); AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	{
		lMaxCols = m_dataSource.m_pRS->Fields->Count;
		lMaxRows = m_dataSource.m_pRS->RecordCount;

		if (lMaxRows > 0)
		{
			_variant_t vColName, vValue;
			CString strData;
			m_dataSource.m_pRS->MoveFirst();
			for (lRow = 0; lRow < lMaxRows; lRow++)
			{
				// fetch mirror name
				vValue = m_dataSource.m_pRS->Fields->Item[(long)0]->Value;
				vValue.ChangeType(VT_BSTR);
				strData = vValue.bstrVal;
				if (strData == strMirrorName)
				{
					// fetch mirror code & return
					vValue = m_dataSource.m_pRS->Fields->Item[(long)1]->Value;
					if (vValue.vt != VT_NULL)   //jun 20110726 
					{
						vValue.ChangeType(VT_BSTR);
						strData = vValue.bstrVal;
						strMirrorCode = strData;
						return TRUE;
					}
				}
				m_dataSource.m_pRS->MoveNext();
			}
		}
	}

	CString strMsg;
	strMsg.Format(_T("Not found mirror code for this mirror name:%s"), strMirrorName);
	TRACE(_T("%s\n"), strMsg);
	//	MessageBoxTimeout(NULL, (strMsg),_T("Warnning"), MB_OK | MB_SETFOREGROUND | MB_APPLMODAL | MB_ICONSTOP, 0, 500);
	return FALSE;
}




// Write Log for Query

void CQuery::Log(CString strMsg, int nType)
{
	CSafeLock lock(&g_LogLockQuery);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, _T("C:\R2RSet\Log"));
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, _T("C:\R2RSet\Log\DTS"));
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	file.Write(cameraKey, nLenth);
	file.Flush();
	file.Close();
}

BOOL CQuery::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CQuery::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}
