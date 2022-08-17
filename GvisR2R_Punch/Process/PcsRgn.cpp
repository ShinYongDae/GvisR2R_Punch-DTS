// PcsRgn.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "PcsRgn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\\GvisR2R_PunchDoc.h"
#include "..\\GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CPcsRgn

CPcsRgn::CPcsRgn(int nPcs)
{
	pPcs = NULL;
	pCenter = NULL;
	pMkPnt[0] = NULL;
	pMkPnt[1] = NULL;
	nTotPcs = nPcs;
	if(nPcs>0)
	{
		pPcs = new CRect[nPcs];
		pCenter = new CfPoint[nPcs];
		pMkPnt[0] = new CfPoint[nPcs];
		pMkPnt[1] = new CfPoint[nPcs];
	}

	nCol = 0;
	nRow = 0;

	m_ptPinPos[0].x = 0.0;
	m_ptPinPos[0].y = 0.0;
	m_ptPinPos[1].x = 0.0;
	m_ptPinPos[1].y = 0.0;
	
	m_ptOffset.x = 0.0;
	m_ptOffset.y = 0.0;
}

CPcsRgn::~CPcsRgn()
{
	if(pPcs)
	{
		delete[] pPcs;
		pPcs = NULL;
	}
	if(pCenter)
	{
		delete[] pCenter;
		pCenter = NULL;
	}
	if(pMkPnt[0])
	{
		delete[] pMkPnt[0];
		pMkPnt[0] = NULL;
	}
	if(pMkPnt[1])
	{
		delete[] pMkPnt[1];
		pMkPnt[1] = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPcsRgn, CWnd)
	//{{AFX_MSG_MAP(CPcsRgn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPcsRgn message handlers

void CPcsRgn::SetMkPnt(int nCam)
{
	CfPoint ptRef, ptTgt,  ptRefPin[2], ptAdjCM[2];
	int nC, nR, idx=0;
	//double dResCam[2];

// 	dResCam[0] = atof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;
// 	ptRefPin[0].x = (double)pDoc->m_pCellRgn->nCADPinPosPixX * dResCam[0];
// 	ptRefPin[0].y = (double)pDoc->m_pCellRgn->nCADPinPosPixY * dResCam[0];
// 	ptAdjCM[0].x = m_ptPinPos[0].x - ptRefPin[0].x;
// 	ptAdjCM[0].y = m_ptPinPos[0].y - ptRefPin[0].y;
// 
// 	dResCam[1] = atof(pDoc->WorkingInfo.Vision[1].sCamPxlRes) / 10000.0;
// 	ptRefPin[1].x = (double)pDoc->m_pCellRgn->nCADPinPosPixX * dResCam[1];
// 	ptRefPin[1].y = (double)pDoc->m_pCellRgn->nCADPinPosPixY * dResCam[1];
// 	ptAdjCM[1].x = m_ptPinPos[1].x - ptRefPin[1].x;
// 	ptAdjCM[1].y = m_ptPinPos[1].y - ptRefPin[1].y;


	if(nCam == CAM_BOTH)
	{
		for(nC=0; nC<nCol; nC++)
		{
			for(nR=0; nR<nRow; nR++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X;
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y;

// 				pMkPnt[0][idx].x = ptRef.x + ptAdjCM[0].x;
// 				pMkPnt[0][idx].y = ptRef.y + ptAdjCM[0].y;
// 				pMkPnt[1][idx].x = ptRef.x + ptAdjCM[1].x;
// 				pMkPnt[1][idx].y = ptRef.y + ptAdjCM[1].y;
				pMkPnt[0][idx].x = ptRef.x + m_ptPinPos[0].x;
				pMkPnt[0][idx].y = ptRef.y + m_ptPinPos[0].y;
				pMkPnt[1][idx].x = ptRef.x + m_ptPinPos[1].x;
				pMkPnt[1][idx].y = ptRef.y + m_ptPinPos[1].y;

				idx++;
			}
		}
	}
	else
 	{
		for(nC=0; nC<nCol; nC++)
		{
			for(nR=0; nR<nRow; nR++)
			{
				ptRef.x = pDoc->m_Master[0].m_stPcsMk[idx].X;
				ptRef.y = pDoc->m_Master[0].m_stPcsMk[idx].Y;

// 				pMkPnt[nCam][idx].x = ptRef.x + ptAdjCM[nCam].x;
// 				pMkPnt[nCam][idx].y = ptRef.y + ptAdjCM[nCam].y;
				pMkPnt[nCam][idx].x = ptRef.x + m_ptPinPos[nCam].x;
				pMkPnt[nCam][idx].y = ptRef.y + m_ptPinPos[nCam].y;

				idx++;
			}
		}
	}
}
 
// void CPcsRgn::SetMkPnt(int nCam)	// PinÀ§Ä¡¿¡ ÀÇÇÑ Á¤·Ä.
// {
// // 	m_ptOffset.x = ptOffset.x;
// // 	m_ptOffset.y = ptOffset.y;
// 	CfPoint ptRefPin, ptAdjCM;
// 	double dResCam = atof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;
// 	if(nCam == CAM_BOTH)
// 	{
// 		for(int i=0; i<CAM_BOTH; i++)
// 		{
// 			nCam = i;
// 			ptRefPin.x = (double)pDoc->m_pCellRgn->nCADPinPosPixX * dResCam;
// 			ptRefPin.y = (double)pDoc->m_pCellRgn->nCADPinPosPixY * dResCam;
// 			ptAdjCM.x = m_ptPinPos[nCam].x - ptRefPin.x;
// 			ptAdjCM.y = m_ptPinPos[nCam].y - ptRefPin.y;	
// 			
// 			for(int i=0; i<nTotPcs; i++)
// 			{
// 				if(m_ptPinPos[nCam].x > 0.0 || m_ptPinPos[nCam].y > 0.0)
// 				{
// 					if(ptAdjCM.x < 0.0 || ptAdjCM.y < 0.0)
// 					{
// 						pMkPnt[nCam][i].x = pCenter[i].x;// + ptOffset.x;
// 						pMkPnt[nCam][i].y = pCenter[i].y;// + ptOffset.y;
// 					}
// 					else
// 					{
// 						pMkPnt[nCam][i].x = pCenter[i].x + ptAdjCM.x; // + ptOffset.x
// 						pMkPnt[nCam][i].y = pCenter[i].y + ptAdjCM.y; // + ptOffset.y
// 					}
// 				}
// 				else
// 				{
// 					pMkPnt[nCam][i].x = pCenter[i].x; // + ptOffset.x;
// 					pMkPnt[nCam][i].y = pCenter[i].y; // + ptOffset.y;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		ptRefPin.x = (double)pDoc->m_pCellRgn->nCADPinPosPixX * dResCam;
// 		ptRefPin.y = (double)pDoc->m_pCellRgn->nCADPinPosPixY * dResCam;
// 		ptAdjCM.x = m_ptPinPos[nCam].x - ptRefPin.x;
// 		ptAdjCM.y = m_ptPinPos[nCam].y - ptRefPin.y;	
// 		
// 		for(int i=0; i<nTotPcs; i++)
// 		{
// 			if(m_ptPinPos[nCam].x > 0.0 || m_ptPinPos[nCam].y > 0.0)
// 			{
// 				if(ptAdjCM.x < 0.0 || ptAdjCM.y < 0.0)
// 				{
// 					pMkPnt[nCam][i].x = pCenter[i].x;// + ptOffset.x;
// 					pMkPnt[nCam][i].y = pCenter[i].y;// + ptOffset.y;
// 				}
// 				else
// 				{
// 					pMkPnt[nCam][i].x = pCenter[i].x + ptAdjCM.x; // + ptOffset.x
// 					pMkPnt[nCam][i].y = pCenter[i].y + ptAdjCM.y; // + ptOffset.y
// 				}
// 			}
// 			else
// 			{
// 				pMkPnt[nCam][i].x = pCenter[i].x; // + ptOffset.x;
// 				pMkPnt[nCam][i].y = pCenter[i].y; // + ptOffset.y;
// 			}
// 		}
// 	}
// }

// void CPcsRgn::SetMkPnt()	// One Point Align¿¡ ÀÇÇÑ Á¤·Ä.
// {
// 	double dDataX, dDataY, dAdjX, dAdjY;
// 	CfPoint ptRefPin, ptAdjCM;
// 	double dResCam = atof(pDoc->WorkingInfo.Vision[0].sCamPxlRes) / 10000.0;
// 	ptRefPin.x = (double)pDoc->m_pCellRgn->nCADPinPosPixX * dResCam;
// 	ptRefPin.y = (double)pDoc->m_pCellRgn->nCADPinPosPixY * dResCam;
// 	ptAdjCM.x = m_ptPinPos.x - ptRefPin.x;
// 	ptAdjCM.y = m_ptPinPos.y - ptRefPin.y;	
// 
// 	for(int i=0; i<nTotPcs; i++)
// 	{
// 		if(m_ptPinPos.x > 0.0 || m_ptPinPos.y > 0.0)
// 		{			
// 			if(ptAdjCM.x < 0.0 || ptAdjCM.y < 0.0)
// 			{
// 				pMkPnt[i].x = pCenter[i].x + m_ptOffset.x;
// 				pMkPnt[i].y = pCenter[i].y + m_ptOffset.y;
// 			}
// 			else
// 			{
// 				dDataX = pCenter[i].x + m_ptOffset.x;
// 				dDataY = pCenter[i].y + m_ptOffset.y;
// 				pView->m_Align.PointAlignment(dDataX, dDataY, dAdjX, dAdjY);
// 				pMkPnt[i].x = dAdjX;
// 				pMkPnt[i].y = dAdjY;
// 			}
// 		}
// 		else
// 		{
// 			pMkPnt[i].x = pCenter[i].x + m_ptOffset.x;
// 			pMkPnt[i].y = pCenter[i].y + m_ptOffset.y;
// 		}
// 	}
// }

CfPoint CPcsRgn::GetMkPnt(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[0][nPcsId].x;
	ptPnt.y = pMkPnt[0][nPcsId].y;
	return ptPnt;
}

CfPoint CPcsRgn::GetMkPnt0(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[0][nPcsId].x;
	ptPnt.y = pMkPnt[0][nPcsId].y;

	return ptPnt;
}

CfPoint CPcsRgn::GetMkPnt1(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[1][nPcsId].x;
	ptPnt.y = pMkPnt[1][nPcsId].y;
	return ptPnt;
}

void CPcsRgn::GetMkPnt(int nC, int nR, int &nPcsId, CfPoint &ptPnt)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	if(nC<nNodeX && nR<nNodeY)
	{
		if(nC%2)	// È¦¼ö.
			nPcsId = nNodeY * (nC+1) - 1 - nR;
		else		// Â¦¼ö.
			nPcsId = nNodeY * nC + nR;

		ptPnt.x = pMkPnt[0][nPcsId].x;
		ptPnt.y = pMkPnt[0][nPcsId].y;
	}
	else
		nPcsId = -1;
}

void CPcsRgn::GetPcsRgn(int nC, int nR, int &nPcsId, CRect &ptRect)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	if (nC < nNodeX && nR < nNodeY)
	{
		if (nC % 2)	// È¦¼ö.
			nPcsId = nNodeY * (nC + 1) - 1 - nR;
		else		// Â¦¼ö.
			nPcsId = nNodeY * nC + nR;

		ptRect.left = pPcs[nPcsId].left;
		ptRect.top = pPcs[nPcsId].top;
		ptRect.right = pPcs[nPcsId].right;
		ptRect.bottom = pPcs[nPcsId].bottom;
	}
	else
		nPcsId = -1;
}

BOOL CPcsRgn::GetMkMatrix(int nPcsId, int &nC, int &nR)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	if(-1 < nPcsId && nPcsId < (nNodeX*nNodeY))
	{
		nC = int(nPcsId/nNodeY);
		if(nC%2)	// È¦¼ö.
			nR = nNodeY*(nC+1)-1-nPcsId;
		else		// Â¦¼ö.
			nR = nPcsId-nC*nNodeY;
	}
	else
	{
		nC = -1;
		nR = -1;
		return FALSE;
	}

	return TRUE;
}

BOOL CPcsRgn::GetMkMatrix(int nPcsId, int &nStrip, int &nC, int &nR) // nStrip:0~3 , nC:0~ , nR:0~
{
	int nNodeX = nCol;
	int nNodeY = nRow;
	int nStPcsY = nNodeY / 4;
	int nRow;// , nCol;

	if(-1 < nPcsId && nPcsId < (nNodeX*nNodeY))
	{
		nC = int(nPcsId/nNodeY);
		if(nC%2)	// È¦¼ö.
			nRow = nNodeY*(nC+1)-1-nPcsId;
		else		// Â¦¼ö.
			nRow = nPcsId-nC*nNodeY;
	}
	else
	{
		nC = -1;
		nR = -1;
		return FALSE;
	}

	nStrip = int(nRow / nStPcsY);
	nR = nRow % nStPcsY;

	return TRUE;
}

// void CPcsRgn::GetMkPnt(int nC, int nR, int &nPcsId, CfPoint &ptPnt)
// {
// 	int nNodeY = nRow;
// 
// 	if(nC%2)	// È¦¼ö.
// 		nPcsId = nNodeY * (nC+1) - (nR+1);
// 	else		// Â¦¼ö.
// 		nPcsId = nNodeY * nC + nR;
// 
// 	ptPnt.x = pMkPnt[nPcsId].x;
// 	ptPnt.y = pMkPnt[nPcsId].y;
// }

void CPcsRgn::SetPinPos(int nCam, CfPoint ptPnt)
{
	m_ptPinPos[nCam].x = ptPnt.x;
	m_ptPinPos[nCam].y = ptPnt.y;
}

