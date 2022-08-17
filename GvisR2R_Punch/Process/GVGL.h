/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////                               GVGL.h Ver 2.4                                  /////
/////                             Developer: jun choi                               /////
/////                               Corp   : GIGAVIS                                /////
/////                             +mouse distance                                   /////
/////                             +circle Draw                                      /////
/////                             +circleF Draw                                     /////
/////                             +Blend                                            /////
/////                             +fontsize                                         /////
/////                             +map data release                                 /////
/////                             +gray map data                                    /////
/////                             +modify Circle                                    /////
/////                             +drag mouse                                       /////
/////                             +index color                                      /////
/////                             +font width                                       /////
/////                             +distance rotate                                  /////
/////                             +RECTF modify                                     /////
/////                             +Image release                                    /////
/////                             +Image mirror                                     /////
/////                             +Backgrand color                                  /////
/////                             +font  color                                      /////
/////                             +Histogram point taget                            /////
/////                             +multy window  color                              /////
/////                             +stencil function                                 /////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GVGL_H__876EE1DF_5B8E_4297_93C8_B8A7FE8D4B72__INCLUDED_)
#define AFX_GVGL_H__876EE1DF_5B8E_4297_93C8_B8A7FE8D4B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <gl\gl.h>
//#include <gl\glu.h>
//#include <gl\glut.h>
//glu32.lib, glut32.lib, opengl32.lib

#include <gl\freeglut_std.h>
//#pragma comment (lib, "glut32.lib")
#pragma comment (lib, "freeglut.lib")
//#pragma comment (lib, "C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\glut32.lib")
//#pragma comment (lib, "C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\freeglut.lib")

#define GVFONT_ROMAN 1
#define GVFONT_MONOROMAN 2

#define GVFALSE  -1
#define GV_POINT  0
#define GV_LINE   1
#define GV_RECTE  2
#define GV_RECTF  3
#define GV_CROSS  4
#define GV_CROSSX 5
#define GV_RECTFG 6
#define GV_CIRCLEE 7
#define GV_CIRCLEF 8


#define GV_GRADATION_PUSEUDO  0
#define GV_GRADATION_AUTUMN 1
#define GV_GRADATION_REDSEA 2
#define GV_GRADATION_GRAY 3
#define GV_GRADATION_GREEN 4

#define GV_NOMIRROR 0
#define GV_HMIRROR 1
#define GV_VMIRROR 2
#define GV_HVMIRROR 3



struct GVTextureImage16
{
	GLubyte *Image1;
	GLubyte *Image2;
	GLubyte *Image3;
	GLubyte *Image4;
	GLubyte *Image5;
	GLubyte *Image6;
	GLubyte *Image7;
	GLubyte *Image8;
	GLubyte *Image9;
	GLubyte *Image10;
	GLubyte *Image11;
	GLubyte *Image12;
	GLubyte *Image13;
	GLubyte *Image14;
	GLubyte *Image15;
	GLubyte *Image16;
};

struct GVTextureImage4
{
	GLubyte *Image1;
	GLubyte *Image2;
	GLubyte *Image3;
	GLubyte *Image4;
};

struct GVTextureImage2
{
	GLubyte *Image1;
	GLubyte *Image2;
};

struct GVertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct GVColor
{
	GLfloat R;
	GLfloat G;
	GLfloat B;
	GLfloat A;
};

////////////    ^
 //////////     |
///////         |
////// /        |
///////normal      ------>




////////////     ------->
 //////////     |
///////         |
////// /        |
///////cam      


//////////////////////////////////////////System Function
 void GVGLinit(CDC *Hdc, HGLRC *hrc);
 void GVGLShareRC(HGLRC m_hRC,HGLRC m_hRC2);
 void GVGLMakehDC(CDC *Hdc, HGLRC hrc);
 void GVGLSwapBuffers(CDC *Hdc);
 void GVGLSetupLightStencil(GLfloat R=0.0f, GLfloat G=0.0f, GLfloat B=0.0f,GLfloat A=1.0f,BOOL CirclePoint=1);
 void GVGLSetupLight(GLfloat R=0.0f, GLfloat G=0.0f, GLfloat B=0.0f,GLfloat A=1.0f,BOOL CirclePoint=1);
 void GVGLResize(int cx, int cy);
 void GVGLFlush();
 int  GVGLGetPieceNum(CString regionName);  //get Piece number
 BOOL GVGLGetPiecePos(CString regionName,GVertex *Masterregion, GVertex **Pieceregion);  //get master region pos and piece pos
 void GVGLMirrorPieceApply(GVertex * PieceS, GVertex *PieceE,GVertex MasterS, GVertex MasterE ,int MirrorMode);
 void GVGLMirrorStripApply(GVertex *StripS,GVertex *StripE,GVertex MasterS, GVertex MasterE,int MirrorMode);
 void GVGLMirrorDefectApply(GVertex *repos,GVertex pos,GVertex MasterS, GVertex MasterE,int MirrorMode);
 void GVertexCopy(GVertex *Data,GVertex DST);
 void GVertexFill(GVertex *Data,GLfloat num1, GLfloat num2, GLfloat num3);
 void GVGLColorFill(GVColor *Data,GLfloat R, GLfloat G, GLfloat B,GLfloat A);
 void GVGLColorFillInDex(GVColor *Data,int index);
 void GVGLDisableDepthforBlend();
 void GVGLEnableDepth();
 void Recombin2P(GVertex &LowPoint, GVertex &HighPoint);
 void GVGLReleaseDC(CDC *Hdc, HGLRC *hrc,HWND m_hWnd);

// void GVGLSaveFile(int WindowSx,int WindowSy,);


////////////////////////////////////Draw Function
 void GVGLDrawInit(int mode,int size,GVColor color);
 void GVGLDrawInitStencil(int mode,int size,GVColor color,BOOL stencil=0);
 void GVGLDrawShow();
 void GVGLDrawShowStencil();
 void GVGLDrawVertex(GVertex V1);
 void GVGLDrawVertexCAM(GVertex V1);
 void GVGLDrawCross(GVertex V1,int Length);
 void GVGLDrawCrossX(GVertex V1,int Length);
 void GVGLDrawRectE(GVertex V1,GVertex V2);
 void GVGLDrawRectF(GVertex V1,GVertex V2);
 void GVGLDrawCircleE(GVertex V1,int radius);
 void GVGLDrawCircleF(GVertex V1,int radius);
 void GVGLDrawCrossXCAM(GVertex V1,int Length); //130922 LGH

 void GVGLDrawCavePolygon(GLfloat *Pointx,GLfloat *Pointy,GLfloat *Pointz, int nval,int size,GLfloat R,GLfloat G,GLfloat B,int nOp=1);
 void GVGLDrawCavePolygonmodeLine(GLfloat *Pointx,GLfloat *Pointy,GLfloat *Pointz, int nval,int size,GLfloat R,GLfloat G,GLfloat B);
 void GVGLDrawCavePolygonmodePOINT(POINT *Point, int nval,int size,GLfloat R,GLfloat G,GLfloat B);
 void GVGLDrawCavePolygonmodeLinePOINT(POINT *Point, int nval,int size,GLfloat R,GLfloat G,GLfloat B,GLfloat width);

 void GVGLDrawCrossCAM(GVertex V1,int Length);
 void GVGLDrawCircleECAM(GLfloat Ox,GLfloat Oy,GLfloat Oz,GLfloat radius,int nSeg);
 void GVGLDrawCircleFCAM(GLfloat Ox,GLfloat Oy,GLfloat Oz,GLfloat radius,int nSeg);
 void GVGLDrawRectECAM(GVertex V1,GVertex V2);
 void GVGLDrawRectFCAM(GVertex V1,GVertex V2);
 void GVGLDrawElipseCAM(GVertex V1,GVertex V2);

 void GVGLDrawRectFG(GVertex V1,GVertex V2,int Level,int color=1);
 void GVGLFont(CString str,int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R=1.0f,GLfloat G=1.0f,GLfloat B=1.0f);
 void GVGLFontCAM(CString str,int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R=1.0f,GLfloat G=1.0f,GLfloat B=1.0f);
 void GVGLRomCurve2D(int nPSize,GVertex *Point,int tval,GVColor Color,int LineSize);
 void GVGLRomCurve3D(int nPSizex,int nPSizey,GVertex **Point,int tval,GVColor Color,int LineSize);
 void GVGLRomCurve3DGrad(int nPSizex,int nPSizey,GVertex **Point, int tval,GVColor **Color,int LineSize);


 void GVGLHistoPoint(GLfloat *cameraposmap,int W, int H,GLfloat *X,GLfloat *y,int cnt,int Psize,GLfloat TSize,GVColor TColor,GVColor HColor);
 void GVGLHistoPointTaget(GLfloat *cameraposmap,int W, int H,GVertex C,GLfloat *X,GLfloat *y,int cnt,GLfloat includ);

 void GVGLHistoinit(GLfloat *cameraposmap,int W, int H,GLfloat TSize,GVColor TColor,BOOL Line=1);
 void GVGLDrawHisto1(int W, int H,GLfloat *HistoDataX1, GLfloat *HistoDataY1,GVColor LColor,GLfloat TSize,int nCnt,BOOL Line=1);
 void GVGLDrawHisto2(int W, int H,GLfloat *HistoDataX1, GLfloat *HistoDataY1,GLfloat *HistoDataX2, GLfloat *HistoDataY2,GVColor* LColor,GLfloat TSize,int nCnt,BOOL Line=1);
 void GVGLDrawHisto3(int nDlgWidth, int nDlgHeight, GLfloat *HistoDataX1, GLfloat *HistoDataY1,GLfloat *HistoDataX2, GLfloat *HistoDataY2,GLfloat *HistoDataX3, GLfloat *HistoDataY3,GVColor* LColor,GLfloat TSize,int nCnt,BOOL Line=1);
// void GVGLDrawHisto1(GLfloat *HistoDataX1, GLfloat *HistoData1,GVColor HColor,GVColor LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line=1);
// void GVGLDrawHisto2(GLfloat *HistoDataX1,GLfloat *HistoData1,GLfloat *HistoData2,GVColor HColor,GVColor *LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line=1);
// void GVGLDrawHisto3(GLfloat *HistoDataX1,GLfloat *HistoData1,GLfloat *HistoData2,GLfloat *HistoData3,GVColor HColor,GVColor *LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line=1);

////////////////////////////////////////////////////////////////////mouse & Camera function
 void GVGLCameraInit(GLfloat *cameraposmap,int W, int H,GLfloat *Angle);
 void GVGLCameraInitStencil(GLfloat *cameraposmap,int W, int H,GLfloat *Angle);
 void GVGLGotoCentermodel(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2);
 void GVGLGotoCentermodelCAM(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2);
 void GVGLSetFit(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger=1);
 void GVGLSetFitCAM(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger=1);
 void GVGLMouseDistance(CDC *Hdc, HGLRC hrc,GVertex MasterS,GVertex MasterE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle);
 void GVGLMouseDistanceCAM(CDC *Hdc, HGLRC hrc,GVertex MasterS,GVertex MasterE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle);
 BOOL GVGLMouseClickObject(CDC *Hdc, HGLRC hrc,CPoint mousepoint,GLfloat *cameraposmap, GVertex start, GVertex End,int W, int H,int windowSx,int windowSy,GLfloat Angle); //objectClick
 void GVGLMoveCamera(GLfloat *cameraposmap,GVertex MasterS, GVertex MasterE,GLfloat XPosVal, GLfloat YPosVal, GLfloat ZPosVal,GLfloat Angle);
 void GVGLRotateCamera(GLfloat *Angle);
 //void GVGLRotateObject(GLfloat *Angle,GLfloat Rotatevalue);  //new
 void GVGLDragMouse(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex MasterS, GVertex MasterE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle);
 void GVGLDragMouseCAM(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex MasterS, GVertex MasterE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle);

////////////////////////////////////////////////Texture mapping


 BOOL GVGLInitmappingTile2(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage2 *texturera,int *SizeX,int *SizeY);
 void GVGLTextureMappingTile2(GVTextureImage2 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLTextureMappingTile2CAM(GVTextureImage2 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLReleaseMappingTile2(GVTextureImage2 *texturera);

 BOOL GVGLInitmappingTile4(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage4 *texturera,int *SizeX,int *SizeY);
 void GVGLTextureMappingTile4(GVTextureImage4 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLTextureMappingTile4CAM(GVTextureImage4 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLReleaseMappingTile4(GVTextureImage4 *texturera);

 BOOL GVGLInitmappingTile16(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage16 *texturera,int *SizeX,int *SizeY);
 void GVGLTextureMappingTile16(GVTextureImage16 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLTextureMappingTile16CAM(GVTextureImage16 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit=1);
 void GVGLReleaseMappingTile16(GVTextureImage16 *texturera);




 BOOL GVGLInitmappingOrigin(CString name,GLubyte **texturera,int *SizeX,int *SizeY);
 BOOL GVGLInitmappingOriginBIG(CString name,GLubyte **texturera,GLubyte **texturera2,int *SizeX,int *SizeY);
 BOOL GVGLInitmappingResize(CString name,GLubyte **texturera,int *SizeX,int *SizeY,int Multival);

 BOOL GVGLGetMappingOrigin(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror=0);
 BOOL GVGLGetMappingOriginBIG(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,GLubyte *texturera2,int SizeX, int SizeY);
 BOOL GVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror=0);
 BOOL CustomGVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror=0);

 //extern "C" __declspec(dllexport) void GVGLTextureMapping(int Mappingmodel,GLubyte * texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL Full);

 void GVGLTextureMapping(GLubyte * texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL Full=0);
 void GVGLTextureMappingBIG(GLubyte * texturera,GLubyte * texturera2,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY);

 void GVGLReleaseMapping(GLubyte ** texturera);

 void GVGLTextureMapping(int Mappingmodel, GLubyte * texturera, GVertex startpoint, GVertex Endpoint, int SizeX, int SizeY, BOOL Full);

#endif // !defined(AFX_OPENGLDRAW_H__876EE1DF_5B8E_4297_93C8_B8A7FE8D4B72__INCLUDED_)
