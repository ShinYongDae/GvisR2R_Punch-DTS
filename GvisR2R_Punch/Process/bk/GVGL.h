///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////                               GVGL.h Ver 0.40                                 ////
////                             Developer: jun choi                               ////
////                               Corp   : GIGAVIS                                ////
////                             +mouse distance                                   ////
////                             +circle Draw                                      ////
////                             +circleF Draw                                     ////
////                             +Blend                                            ////
////                             +fontsize                                         ////
////                             +map data release                                 ////
////                             +gray map data                                    ////
////                             +modify Circle                                    ////
////                             +drag mouse                                       ////
////                             +index color                                      ////
////                             +font width                                       ////
////                             +distance rotate                                  ////
////                             +RECTF modify                                     ////
////                             +Image release                                    ////
////                             +Image mirror                                     ////
////                             +Backgrand color                                  ////
////                             +font  color                                      ////
////                             +multy window  color                              ////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
#include <gl/gl.h>
#include <gl/GLTools.h>
#include <gl/glu.h>

#include "FreeImage.h"


#define GVFONT_ROMAN 1
#define GVFONT_MONOROMAN 2

#define GVFALSE  -1
#define GV_POINT  0
#define GV_LINE   1
#define GV_RECTE  2
#define GV_RECTF  3
#define GV_CROSS  4
#define GV_CROSSX 4
#define GV_RECTFG 5
#define GV_CIRCLEE 6
#define GV_CIRCLEF 7


#define GV_GRADATION_PUSEUDO  0
#define GV_GRADATION_AUTUMN 1
#define GV_GRADATION_REDSEA 2
#define GV_GRADATION_GRAY 3
#define GV_GRADATION_GREEN 4

#define GV_NOMIRROR 0
#define GV_HMIRROR 1
#define GV_VMIRROR 2
#define GV_HVMIRROR 3


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

//////////////////////////////////////////System Function
extern "C" __declspec(dllexport) void GVGLinit(CDC *Hdc, HGLRC *hrc);
extern "C" __declspec(dllexport) void GVGLMakehDC(CDC *Hdc, HGLRC hrc);
extern "C" __declspec(dllexport) void GVGLSwapBuffers(CDC *Hdc);
extern "C" __declspec(dllexport) void GVGLSetupLight(GLfloat R=0.0f, GLfloat G=0.0f, GLfloat B=0.0f,GLfloat A=1.0f);
extern "C" __declspec(dllexport) void GVGLResize(int cx, int cy);
extern "C" __declspec(dllexport) void GVGLFlush();
extern "C" __declspec(dllexport) int  GVGLGetPieceNum(CString regionName);  //get Piece number
extern "C" __declspec(dllexport) BOOL GVGLGetPiecePos(CString regionName,GVertex *Masterregion, GVertex **Pieceregion);  //get master region pos and piece pos
extern "C" __declspec(dllexport) void GVGLMirrorApply(GVertex ** OriginPieceregion,GVertex ** ChangePieceregion,int PieceNum,int MirrorMode);
extern "C" __declspec(dllexport) void GVertexCopy(GVertex *Data,GVertex DST);
extern "C" __declspec(dllexport) void GVertexFill(GVertex *Data,GLfloat num1, GLfloat num2, GLfloat num3);
extern "C" __declspec(dllexport) void GVGLColorFill(GVColor *Data,GLfloat R, GLfloat G, GLfloat B,GLfloat A);
extern "C" __declspec(dllexport) void GVGLColorFillInDex(GVColor *Data,int index);
extern "C" __declspec(dllexport) void GVGLDisableDepthforBlend();
extern "C" __declspec(dllexport) void GVGLEnableDepth();
extern "C" __declspec(dllexport) void Recombin2P(GVertex &LowPoint, GVertex &HighPoint);


////////////////////////////////////Draw Function
extern "C" __declspec(dllexport) void GVGLDrawInit(int mode,int size,GVColor color);
extern "C" __declspec(dllexport) void GVGLDrawShow();
extern "C" __declspec(dllexport) void GVGLDrawVertex(GVertex V1);
extern "C" __declspec(dllexport) void GVGLDrawCross(GVertex V1,int Length);
extern "C" __declspec(dllexport) void GVGLDrawCrossX(GVertex V1,int Length);
extern "C" __declspec(dllexport) void GVGLDrawRectE(GVertex V1,GVertex V2);
extern "C" __declspec(dllexport) void GVGLDrawRectF(GVertex V1,GVertex V2);
extern "C" __declspec(dllexport) void GVGLDrawCircleE(GVertex V1,int radius);
extern "C" __declspec(dllexport) void GVGLDrawCircleF(GVertex V1,int radius);
extern "C" __declspec(dllexport) void GVGLDrawRectFG(GVertex V1,GVertex V2,int Level,int color=1);
extern "C" __declspec(dllexport) void GVGLFont(const char str[],int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R=1.0f,GLfloat G=1.0f,GLfloat B=1.0f);
extern "C" __declspec(dllexport) void GVGLRomCurve2D(int nPSize,GVertex *Point,int tval,GVColor Color,int LineSize);
extern "C" __declspec(dllexport) void GVGLRomCurve3D(int nPSizex,int nPSizey,GVertex **Point,int tval,GVColor Color,int LineSize);
extern "C" __declspec(dllexport) void GVGLRomCurve3DGrad(int nPSizex,int nPSizey,GVertex **Point, int tval,GVColor **Color,int LineSize);

extern "C" __declspec(dllexport) void GVGLHistoinit(GLfloat *cameraposmap,int W, int H,int x,int y,GLfloat TSize,GVColor TColor);
extern "C" __declspec(dllexport) void GVGLDrawHisto1(GLfloat *HistoData1,GVColor LColor,int W, int H,int x,int y,GLfloat TSize);
extern "C" __declspec(dllexport) void GVGLDrawHisto2(GLfloat *HistoData1,GLfloat *HistoData2,GVColor *LColor,int W, int H,int x,int y,GLfloat TSize);
extern "C" __declspec(dllexport) void GVGLDrawHisto3(GLfloat *HistoData1,GLfloat *HistoData2,GLfloat *HistoData3,GVColor *LColor,int W, int H,int x,int y,GLfloat TSize);

////////////////////////////////////////////////////////////////////mouse & Camera function
extern "C" __declspec(dllexport) void GVGLCameraInit(GLfloat *cameraposmap,int W, int H,GLfloat *Angle);
extern "C" __declspec(dllexport) void GVGLGotoCentermodel(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2);
extern "C" __declspec(dllexport) void GVGLSetFit(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger);
extern "C" __declspec(dllexport) void GVGLMouseDistance(CDC *Hdc, HGLRC hrc,GVertex MasterregionS,GVertex MasterregionE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle);
extern "C" __declspec(dllexport) BOOL GVGLMouseClickObject(CDC *Hdc, HGLRC hrc,CPoint mousepoint,GLfloat *cameraposmap, GVertex start, GVertex End,int W, int H,int windowSx,int windowSy,GLfloat Angle); //objectClick
extern "C" __declspec(dllexport) void GVGLMoveCamera(GLfloat *cameraposmap,GVertex RegionS, GVertex RegionE,GLfloat XPosVal, GLfloat YPosVal, GLfloat ZPosVal,GLfloat Angle);
extern "C" __declspec(dllexport) void GVGLRotateCamera(GLfloat *Angle);
extern "C" __declspec(dllexport) void GVGLRotateObject(GLfloat *Angle,GLfloat Rotatevalue);  //new
extern "C" __declspec(dllexport) void GVGLDragMouse(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex RegionS, GVertex RegionE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle);

////////////////////////////////////////////////Texture mapping
extern "C" __declspec(dllexport) BOOL GVGLInitmappingOriginBIG(CString name,GLubyte **texturera,GLubyte **texturera2,int *SizeX,int *SizeY);
extern "C" __declspec(dllexport) BOOL GVGLInitmappingOrigin(CString name,GLubyte **texturera,int *SizeX,int *SizeY);
extern "C" __declspec(dllexport) BOOL GVGLInitmappingResize(CString name,GLubyte **texturera,int *SizeX,int *SizeY,int Multival);

extern "C" __declspec(dllexport) BOOL GVGLGetMappingOrigin(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror=0);
extern "C" __declspec(dllexport) BOOL GVGLGetMappingOriginBIG(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,GLubyte *texturera2,int SizeX, int SizeY);
extern "C" __declspec(dllexport) BOOL GVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror=0);

extern "C" __declspec(dllexport) void GVGLTextureMapping(int Mappingmodel,GLubyte * texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL Full);
extern "C" __declspec(dllexport) void GVGLTextureMappingBIG(int Mappingmodel,GLubyte * texturera,GLubyte * texturera2,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY);
extern "C" __declspec(dllexport) void GVGLReleaseMapping(GLubyte ** texturera);
