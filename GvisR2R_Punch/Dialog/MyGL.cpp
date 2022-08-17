// MyGL.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "MyGL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CMyGL

CMyGL::CMyGL(CWnd* pParent)
{
	m_pParent = pParent;
	m_nCtrlId = -1;

	RECT rt={0,0,0,0};
	if(!Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rt, pParent, 0))
		pView->MsgBox(_T("CMyGL::Create() Failed!!!"));
		//AfxMessageBox(_T("CMyGL::Create() Failed!!!"));

	m_hDC = NULL;
	m_hRC = NULL;

	m_nTexTureBuff = -1;
	TexTureBuff[0] = NULL;
	TexTureBuff[1] = NULL;

	m_nTotPnl = 0;
	m_nTotPcs = 0;
	m_nPrevTotPnl = 0;
	m_nPrevTotPcs = 0;
	m_pFrmRgn = NULL;
	m_pPcsPnt = NULL;
	m_pPnlNum = NULL;
	m_pPnlDefNum = NULL;

	cameraposmap[0]=0.0f;
	cameraposmap[1]=0.0f;
	cameraposmap[2]=7000.0f;
	Angle[0]=0.0f;
	Angle[1]=0.0f;
	Angle[2]=0.0f;
}

CMyGL::~CMyGL()
{
	int k, i;

	if(m_pPnlNum)
	{
		delete[] m_pPnlNum;
		m_pPnlNum = NULL;
	}

	if(m_pPnlDefNum)
	{
		delete[] m_pPnlDefNum;
		m_pPnlDefNum = NULL;
	}

	if(m_pFrmRgn)
	{
		for(k=0; k<m_nTotPnl; k++)
		{
			if(m_pFrmRgn[k])
			{
				delete[] m_pFrmRgn[k];
				m_pFrmRgn[k] = NULL;
			}
		}
		delete[] m_pFrmRgn;
		m_pFrmRgn = NULL;
	}

	if(m_pPcsPnt)
	{
		for(k=0; k<m_nTotPnl; k++)
		{
			if(m_pPcsPnt[k])
			{
				for(i=0; i<m_nTotPcs; i++)
				{
					delete[] m_pPcsPnt[k][i];
					m_pPcsPnt[k][i] = NULL;
				}

				delete[] m_pPcsPnt[k];
				m_pPcsPnt[k] = NULL;
			}
		}
		delete[] m_pPcsPnt;
		m_pPcsPnt = NULL;
	}

	if(m_hRC)
	{
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if(m_hDC)
	{
		::ReleaseDC(m_hWnd, m_hDC->GetSafeHdc());
		delete m_hDC;
		m_hDC = NULL;
	}

	if(TexTureBuff[0])
		GVGLReleaseMapping(&TexTureBuff[0]);

	if(TexTureBuff[1])
		GVGLReleaseMapping(&TexTureBuff[1]);

}


BEGIN_MESSAGE_MAP(CMyGL, CWnd)
	//{{AFX_MSG_MAP(CMyGL)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyGL message handlers

void CMyGL::Init(int nCtrlId)
{
	m_nCtrlId = nCtrlId;

	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	//const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte *extension = glGetString(GL_EXTENSIONS);

	
	HWND hDispCtrl = m_pParent->GetDlgItem(nCtrlId)->GetSafeHwnd();
	CRect rtDispCtrl;
	m_pParent->GetDlgItem(nCtrlId)->GetWindowRect(&rtDispCtrl);
	m_hDispCtrl = hDispCtrl;
	m_rtDispCtrl = rtDispCtrl;

	float fBkR = 0.0, fBkG = 0.0, fBkB = 0.0;
	if(pDoc->m_pReelMap)
	{
		fBkR = (float)pDoc->m_pReelMap->m_nBkColor[0] / 256.0;
		fBkG = (float)pDoc->m_pReelMap->m_nBkColor[1] / 256.0;
		fBkB = (float)pDoc->m_pReelMap->m_nBkColor[2] / 256.0;
	}

	m_hDC =new CClientDC(FromHandle(hDispCtrl));
	GVGLinit(m_hDC, &m_hRC);

	GVGLColorFill(&m_rgbWhite, 1.0f, 1.0f, 1.0f, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	GVGLColorFill(&m_rgbBlack, 0.0f, 0.0f, 0.0f, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	GVGLColorFill(&m_rgbBk, fBkR, fBkG, fBkB, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	GVGLColorFill(&m_rgbRed, 1.0f, 0.0f, 0.0f, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	GVGLColorFill(&m_rgbGreen, 0.0f, 1.0f, 0.0f, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	GVGLColorFill(&m_rgbBlue, 0.0f, 0.0f, 1.0f, 1.0f);  // 색 부여. 1.0 이 최고 값이다.

}

void CMyGL::LoadPcsImg(CString sPath)
{
	if(m_nTexTureBuff >= 0)
	{
		if(TexTureBuff[m_nTexTureBuff])
		{
			wglMakeCurrent(m_hDC->GetSafeHdc(), m_hRC);
			GVGLReleaseMapping(&TexTureBuff[m_nTexTureBuff]);
			wglMakeCurrent(m_hDC->GetSafeHdc(), m_hRC);
		}
	}

	m_nTexTureBuff++;
	if(m_nTexTureBuff>1)
		m_nTexTureBuff = 0;

	GVGLInitmappingOrigin(sPath, &TexTureBuff[m_nTexTureBuff], &m_nTxTrSzX, &m_nTxTrSzY); 
	GVGLGetMappingOrigin(m_hDC, m_hRC, sPath, TexTureBuff[m_nTexTureBuff], m_nTxTrSzX, m_nTxTrSzY);

	m_nWorldW = m_rtDispCtrl.right - m_rtDispCtrl.left;
	m_nWorldH = m_rtDispCtrl.bottom - m_rtDispCtrl.top;

	GVertexFill(&vtScrSt, (GLfloat)0, (GLfloat)0, 0.0f);
	GVertexFill(&vtScrEd, (GLfloat)m_nTxTrSzX, (GLfloat)m_nTxTrSzY, 0.0f);

	GVGLGotoCentermodel(cameraposmap, vtScrSt, vtScrEd);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap, vtScrSt, vtScrEd, 0, m_nWorldW, m_nWorldH, 1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.
}

void CMyGL::GetPixelInfo(int &nSzCtrlX, int &nSzCtrlY, int &nSzImgX, int &nSzImgY)
{
	nSzCtrlX = m_nWorldW;
	nSzCtrlY = m_nWorldH;
	nSzImgX = m_nTxTrSzX;
	nSzImgY = m_nTxTrSzY;
}

void CMyGL::SetPcsDef()
{
	if(!pDoc->m_pReelMap)
		return;

	int i;
	DWORD dwR, dwG, dwB;
	for(i=0; i<MAX_DEF; i++)
	{
		dwR = GetRValue(pDoc->m_pReelMap->m_rgbDef[i]);
		dwG = GetGValue(pDoc->m_pReelMap->m_rgbDef[i]);
		dwB = GetBValue(pDoc->m_pReelMap->m_rgbDef[i]);
		GVGLColorFill(&m_rgbDef[i], (float)dwR/255.0, (float)dwG/255.0, (float)dwB/255.0, 1.0f);  // 색 부여. 1.0 이 최고 값이다.
	}
}

void CMyGL::SetPnlNum()
{
	if(!pDoc->m_pReelMap)
		return;

	if(!m_pPnlNum)
		return;

	int k;
	for (k = 0; k < m_nTotPnl; k++)
	{
		if (pDoc->WorkingInfo.System.bSaveLog)
		{
			CString strData;
			strData.Format(_T("SetPnlNum: m_pPnlNum[%d] = %d"), k, pDoc->m_pReelMap->m_pPnlNum[k]);
			SaveLog(strData);
		}

		m_pPnlNum[k] = pDoc->m_pReelMap->m_pPnlNum[k];
	}
}

void CMyGL::SetPnlDefNum()
{
	if(!pDoc->m_pReelMap)
		return;

	if(!m_pPnlDefNum)
		return;

	int k;
	for(k=0; k<m_nTotPnl; k++)
		m_pPnlDefNum[k]=pDoc->m_pReelMap->m_pPnlDefNum[k];
}

void CMyGL::ResetRgn()
{
	if(!pDoc->m_pReelMap)
		return;

	int i, k;
	int nTotPnl = pDoc->m_pReelMap->nTotPnl;
	if(m_pPcsPnt)
	{
		for(k=0; k<nTotPnl; k++)
		{
			if(m_pPcsPnt[k])
			{
				for(i=0; i<m_nTotPcs; i++)
					delete[] m_pPcsPnt[k][i];

				delete[] m_pPcsPnt[k];
			}
		}
		delete[] m_pPcsPnt;
		m_pPcsPnt = NULL;
	}
}

void CMyGL::SetRgn()
{
	if(!pDoc->m_pReelMap)
		return;

	int i, k;
	int nTotPnl = pDoc->m_pReelMap->nTotPnl;
	int nTotPcs = pDoc->m_pReelMap->nTotPcs;
	double fData1, fData2, fData3, fData4;


	if(m_pPnlNum)
	{
		delete[] m_pPnlNum;
		m_pPnlNum = NULL;
	}
	m_pPnlNum = new int[nTotPnl];

	if(m_pPnlDefNum)
	{
		delete[] m_pPnlDefNum;
		m_pPnlDefNum = NULL;
	}
	m_pPnlDefNum = new int[nTotPnl];

	if(m_pFrmRgn)
	{
// 		for(k=0; k<nTotPnl; k++)
		for(k=0; k<m_nPrevTotPnl; k++)
		{
			if(m_pFrmRgn[k])
			{
				delete[] m_pFrmRgn[k];
				m_pFrmRgn[k] = NULL;
			}
		}
		delete[] m_pFrmRgn;
		m_pFrmRgn = NULL;
	}
		
	m_pFrmRgn = new GVertex*[nTotPnl];
	for(k=0; k<nTotPnl; k++)
		m_pFrmRgn[k] = new GVertex[2];


	if(m_pPcsPnt)
	{
// 		for(k=0; k<nTotPnl; k++)
		for(k=0; k<m_nPrevTotPnl; k++)
		{
			if(m_pPcsPnt[k])
			{
// 				for(i=0; i<nTotPcs; i++)
				for(i=0; i<m_nPrevTotPcs; i++)
				{
					delete[] m_pPcsPnt[k][i];
					m_pPcsPnt[k][i] = NULL;
				}

				delete[] m_pPcsPnt[k];
				m_pPcsPnt[k] = NULL;
			}
		}
		delete[] m_pPcsPnt;
		m_pPcsPnt = NULL;
	}

	if(!m_pPcsPnt)
	{
		m_pPcsPnt = new GVertex**[nTotPnl];
		for(k=0; k<nTotPnl; k++)
		{
			m_pPcsPnt[k] = new GVertex*[nTotPcs];
			for(i=0; i<nTotPcs; i++)
				m_pPcsPnt[k][i] = new GVertex[2];
		}
	}

	m_nTotPnl = nTotPnl;
	m_nTotPcs = nTotPcs;

	m_nPrevTotPnl = nTotPnl;
	m_nPrevTotPcs = nTotPcs;


	for(k=0; k<nTotPnl; k++)
	{
		fData1 = pDoc->m_pReelMap->pFrmRgn[k].left;		// left
		fData2 = pDoc->m_pReelMap->pFrmRgn[k].top;		// top
		fData3 = pDoc->m_pReelMap->pFrmRgn[k].right;	// right
		fData4 = pDoc->m_pReelMap->pFrmRgn[k].bottom;	// bottom
		GVertexFill(&m_pFrmRgn[k][0], (GLfloat)fData1, (GLfloat)fData2, 0.0f);
		GVertexFill(&m_pFrmRgn[k][1], (GLfloat)fData3, (GLfloat)fData4, 0.0f);

		for(i=0; i<nTotPcs; i++)
		{
			fData1 = pDoc->m_pReelMap->pPcsRgn[k][i].left;		// left
			fData2 = pDoc->m_pReelMap->pPcsRgn[k][i].top;		// top
			fData3 = pDoc->m_pReelMap->pPcsRgn[k][i].right;		// right
			fData4 = pDoc->m_pReelMap->pPcsRgn[k][i].bottom;	// bottom

			GVertexFill(&m_pPcsPnt[k][i][0], (GLfloat)fData1, (GLfloat)fData2, 0.0f);
			GVertexFill(&m_pPcsPnt[k][i][1], (GLfloat)fData3, (GLfloat)fData4, 0.0f);
		}
	}

	m_nWorldW = m_rtDispCtrl.right - m_rtDispCtrl.left;
	m_nWorldH = m_rtDispCtrl.bottom - m_rtDispCtrl.top;
	double dScale = pDoc->m_pReelMap->GetAdjRatio();
	GVertexFill(&vtScrSt, (GLfloat)pDoc->m_pReelMap->pFrmRgn[nTotPnl-1].left, (GLfloat)pDoc->m_pReelMap->pFrmRgn[nTotPnl-1].top, 0.0f);
	GVertexFill(&vtScrEd, (GLfloat)pDoc->m_pReelMap->pFrmRgn[0].right, (GLfloat)pDoc->m_pReelMap->pFrmRgn[0].bottom + (MYGL_GAP_NUM + MYGL_SIZE_CHAR) * dScale, 0.0f);
// 	GVertexFill(&vtBkSt, (GLfloat)m_rtDispCtrl.left, (GLfloat)m_rtDispCtrl.top, 0.0f);
// 	GVertexFill(&vtBkEd, (GLfloat)m_rtDispCtrl.right, (GLfloat)m_rtDispCtrl.bottom, 0.0f);
	GVertexFill(&vtBkSt, (GLfloat)pDoc->m_pReelMap->pFrmRgn[nTotPnl-1].left - MYGL_SIDE_MARGIN, (GLfloat)pDoc->m_pReelMap->pFrmRgn[nTotPnl-1].top - (MYGL_GAP_NUM + MYGL_SIZE_CHAR) * 5.0, 0.0f);
	GVertexFill(&vtBkEd, (GLfloat)pDoc->m_pReelMap->pFrmRgn[0].right + MYGL_SIDE_MARGIN, (GLfloat)pDoc->m_pReelMap->pFrmRgn[0].bottom + (MYGL_GAP_NUM + MYGL_SIZE_CHAR) * 5.0, 0.0f);

	GVGLGotoCentermodel(cameraposmap, vtScrSt, vtScrEd);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap, vtScrSt, vtScrEd, MYGL_SIDE_MARGIN+(MYGL_SIZE_CHAR)/nTotPnl, m_nWorldW, m_nWorldH, 1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.
}

void CMyGL::SetMkPos(CfPoint ptPnt)
{
	GVertexFill(&vtMkPnt, (GLfloat)ptPnt.x, (GLfloat)ptPnt.y, 0.0f);
}

void CMyGL::Draw()
{
	if(m_nCtrlId < 0)
		return;

	if(!pView->m_bDrawGL)
		return;
	
	GVGLMakehDC(m_hDC,m_hRC);
	GVGLSetupLight();
	GVGLResize(m_nWorldW, m_nWorldH);

	glDisable(GL_LIGHTING);
	GVGLCameraInit(cameraposmap, m_nWorldW, m_nWorldH, Angle);	

	switch(m_nCtrlId)
	{
	case IDC_STC_REELMAP_IMG:
		DrawPnlDefNum();
		DrawPnlNum();
		DrawRgn();
		DrawBack();
		break;
// 	case IDC_STC_PCS_IMG:
// 		DrawPcs();
// 		break;
// 	case IDC_STC_PIN_IMG:
// 		DrawPin();
// 		break;
	}

	GVGLFlush();

	GVGLSwapBuffers(m_hDC);	
}

void CMyGL::DrawPcs()
{
	GVGLDrawInit(GV_LINE, 2, m_rgbRed);
	GVGLDrawCross(vtMkPnt, 300); // 33um * 300pixel = 9.900mm
	GVGLDrawShow();

	GVGLTextureMapping(0, TexTureBuff[m_nTexTureBuff], vtScrSt, vtScrEd, m_nTxTrSzX, m_nTxTrSzY, 0);
	GVGLDrawInit(GV_RECTF, 3, m_rgbGreen);
	GVGLDrawRectF(vtScrSt, vtScrEd);
	GVGLDrawShow();
}

void CMyGL::DrawPin()
{
	GVGLTextureMapping(0, TexTureBuff[m_nTexTureBuff], vtScrSt, vtScrEd, m_nTxTrSzX, m_nTxTrSzY, 0);
	GVGLDrawInit(GV_RECTF, 3, m_rgbGreen);
	GVGLDrawRectF(vtScrSt, vtScrEd);
	GVGLDrawShow();
}

void CMyGL::DrawBack()
{	
	GVGLDrawInit(GV_RECTF, 3, m_rgbBk);
	GVGLDrawRectF(vtBkSt, vtBkEd);
	GVGLDrawShow();
}

void CMyGL::DrawRgn()
{	
	int i, k, nDef;
	if(!pDoc->m_pReelMap || !m_pFrmRgn || !m_pPcsPnt)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

// 	for(k=0; k<m_nTotPnl; k++)
	for(k=m_nTotPnl-1; k>=0; k--)
	{
		if(pDoc->WorkingInfo.LastJob.bLotSep && m_pPnlNum[k] > pDoc->m_nLotLastShot && !pDoc->m_bDoneChgLot)
			break;


		if(k==pDoc->m_pReelMap->m_nSelMarkingPnl)
		{
			GVGLDrawInit(GV_LINE, 2, m_rgbRed);
			GVGLDrawRectF(m_pFrmRgn[k][0], m_pFrmRgn[k][1]);
			GVGLDrawShow();
		}
		else if(k==pDoc->m_pReelMap->m_nSelMarkingPnl+1)
		{
			GVGLDrawInit(GV_LINE, 2, m_rgbRed);
			GVGLDrawRectF(m_pFrmRgn[k][0], m_pFrmRgn[k][1]);
			GVGLDrawShow();
		}
		else
		{
			GVGLDrawInit(GV_LINE, 1, m_rgbWhite);
			GVGLDrawRectF(m_pFrmRgn[k][0], m_pFrmRgn[k][1]);
			GVGLDrawShow();
		}

		if(m_pPnlNum[k] > 0)
		{
			int nTestMode=2;	
			if(!bDualTest)
				nTestMode=0;
			
			int nIdx = pDoc->GetPcrIdx1(m_pPnlNum[k]);
			if(pDoc->m_pPcr[nTestMode][nIdx]->m_nErrPnl == -1 || pDoc->m_pPcr[nTestMode][nIdx]->m_nErrPnl == -2)
			{
				// Draw Cross....
				GVertex vtPnt[4];
				vtPnt[0] = m_pFrmRgn[k][0];

				vtPnt[1].x = m_pFrmRgn[k][0].x;
				vtPnt[1].y = m_pFrmRgn[k][1].y;
				vtPnt[1].z = m_pFrmRgn[k][1].z;

				vtPnt[2] = m_pFrmRgn[k][1];

				vtPnt[3].x = m_pFrmRgn[k][1].x;
				vtPnt[3].y = m_pFrmRgn[k][0].y;
				vtPnt[3].z = m_pFrmRgn[k][0].z;

				GVGLDrawInit(GV_LINE, 3, m_rgbRed);
				GVGLDrawVertex(vtPnt[0]);
				GVGLDrawVertex(vtPnt[2]);
				GVGLDrawShow();

				GVGLDrawInit(GV_LINE, 3, m_rgbRed);
				GVGLDrawVertex(vtPnt[1]);
				GVGLDrawVertex(vtPnt[3]);
				GVGLDrawShow();
			}
		}

		for(i=0; i<m_nTotPcs; i++)
		{
			if(pDoc->m_pReelMap)
			{
				if(m_pPnlNum[k] <= 0)
				{
					GVGLDrawInit(GV_RECTF, 3, m_rgbWhite);
				}
				else
				{
					nDef = pDoc->m_pReelMap->pPcsDef[k][i];
					if(nDef)
						int iii = i;

					GVGLDrawInit(GV_RECTF, 3, m_rgbDef[nDef]);
				}
			}
			GVGLDrawRectF(m_pPcsPnt[k][i][0], m_pPcsPnt[k][i][1]);
			GVGLDrawShow();
		}
	}
}

void CMyGL::DrawPnlNum()
{
 	int k;
	//char cPnlNum[MAX_PATH];
	//TCHAR cPnlNum[MAX_PATH];
	CString sPnlNum;
// 	for(k=0; k<m_nTotPnl; k++)
	for(k=m_nTotPnl-1; k>=0; k--)
	{
		if(pDoc->WorkingInfo.LastJob.bLotSep && m_pPnlNum[k] > pDoc->m_nLotLastShot && !pDoc->m_bDoneChgLot)
			break;

		if(m_pPnlNum[k] <= 0)
			sPnlNum.Format(_T(""));
		else
		{
			sPnlNum.Format(_T("%d"), m_pPnlNum[k]);

			if (pDoc->WorkingInfo.System.bSaveLog)
			{
				CString strData;
				strData.Format(_T("DrawPnlNum: m_pPnlNum[%d] = %d"), k, m_pPnlNum[k]);
				SaveLog(strData);
			}
		}
		//strcpy(cPnlNum, sPnlNum);
		//_stprintf(cPnlNum, _T("%s"), sPnlNum);

		double dScale = pDoc->m_pReelMap->GetAdjRatio();
		double fPosX = (pDoc->m_pReelMap->pFrmRgn[k].left+pDoc->m_pReelMap->pFrmRgn[k].right-sPnlNum.GetLength()*(MYGL_GAP_NUM + MYGL_SIZE_CHAR)*dScale)/2.0;	
		double fPosY = pDoc->m_pReelMap->pFrmRgn[k].bottom + (MYGL_GAP_NUM + MYGL_SIZE_CHAR) * dScale;	
		GLfloat fFontSize = MYGL_SIZE_NUM * dScale;
		if(k==pDoc->m_pReelMap->m_nSelMarkingPnl)
		{
			if(fPosX > 0.0 && fPosY > 0.0)
				GVGLFont(sPnlNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
				//GVGLFont(TCHARToString(cPnlNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
		}
		else if(k==pDoc->m_pReelMap->m_nSelMarkingPnl+1)
		{
			if(fPosX > 0.0 && fPosY > 0.0)
				GVGLFont(sPnlNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
				//GVGLFont(TCHARToString(cPnlNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
		}
		else
		{
			if(fPosX > 0.0 && fPosY > 0.0)
				GVGLFont(sPnlNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_ROMAN);
				//GVGLFont(TCHARToString(cPnlNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_ROMAN);
		}
	}
}

void CMyGL::DrawPnlDefNum()
{
	if(!m_pPnlDefNum)
		return;

 	int k;
	//char cPnlDefNum[MAX_PATH];
	//TCHAR cPnlDefNum[MAX_PATH];
	CString sPnlDefNum;
// 	for(k=0; k<m_nTotPnl; k++)
	for(k=m_nTotPnl-1; k>=0; k--)
	{
		if(m_pPnlDefNum[k] <= 0)
			sPnlDefNum.Format(_T(""));
		else
			sPnlDefNum.Format(_T("Total: %d"), m_pPnlDefNum[k]);
		//strcpy(cPnlDefNum, sPnlDefNum);
		//_stprintf(cPnlDefNum, _T("%s"), sPnlDefNum);

		double dScale = pDoc->m_pReelMap->GetAdjRatio();
		double fPosX = (pDoc->m_pReelMap->pFrmRgn[k].left+pDoc->m_pReelMap->pFrmRgn[k].right-sPnlDefNum.GetLength()*(MYGL_GAP_NUM + MYGL_SIZE_CHAR)*dScale)/2.0;	
		double fPosY = pDoc->m_pReelMap->pFrmRgn[k].top - MYGL_SIZE_CHAR*dScale; //(MYGL_GAP_NUM + MYGL_SIZE_CHAR) * dScale;	
		GLfloat fFontSize = MYGL_SIZE_DEFNUM * dScale;
		if(k==pDoc->m_pReelMap->m_nSelMarkingPnl)
		{
			if(fPosX > -100.0 && fPosY > -100.0)
				GVGLFont(sPnlDefNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
				//GVGLFont(TCHARToString(cPnlDefNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
		}
		else if(k==pDoc->m_pReelMap->m_nSelMarkingPnl+1)
		{
			if(fPosX > -100.0 && fPosY > -100.0)
				GVGLFont(sPnlDefNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
				//GVGLFont(TCHARToString(cPnlDefNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_MONOROMAN, 1.0, 0.0, 0.0);
		}
		else
		{
			if(fPosX > -100.0 && fPosY > -100.0)
				GVGLFont(sPnlDefNum, 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_ROMAN);
				//GVGLFont(TCHARToString(cPnlDefNum), 1, fPosX, fPosY, 0.0, fFontSize, GVFONT_ROMAN);
		}
	}
}


