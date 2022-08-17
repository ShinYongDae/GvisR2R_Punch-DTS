#include "../stdafx.h"
#include "GVGL.h"
#include <gl/glut.h>

#include <math.h>
#include "GlobalFunc.h"

BOOL Firstshow=FALSE;

#define PI 3.1415


static GLfloat GVCOLOR[31][4]={{1.0f,1.0f,1.0f,1.0f},{1.0f,0.0f,1.0f,1.0f},{0.5f,1.0f,1.0f,1.0f},{0.0f,1.0f,0.5f,1.0f},{1.0f,0.5f,0.25,1.0f},{1.0f,0.0f,0.0f,1.0f},{0.5f,1.0f,1.0f,1.0f},{0.5f,1.0f,0.5f,1.0f},{0.3f,0.9f,0.9f,1.0f},{0.5f,0.8f,0.95f,1.0f},{1.0f,0.5f,1.0f,1.0f},
								{1.0f,0.0f,0.5f,1.0f},{1.0f,0.25f,1.0f,1.0f},{1.0f,1.0f,0.5f,1.0f},{1.0f,0.0f,0.5f,1.0f},{1.0f,0.25f,0.5f,1.0f},{0.5f,0.5f,0.5f,1.0f},{0.5f,0.5f,0.5f,1.0f},{0.0f,1.0f,0.25f,1.0f},{1.0f,0.0f,0.0f,1.0f},{1.0f,0.0f,0.5f,1.0f},
								{0.5f,0.5f,1.0f,1.0f},{0.5f,1.0f,1.0f,1.0f},{1.0f,0.5f,0.5f,1.0f},{0.5f,0.0f,1.0f,1.0f},{0.0f,1.0f,0.0f,1.0f},{1.0f,0.5f,1.0f,1.0f},{1.0f,0.0f,0.5f,1.0f},{0.0f,0.0f,1.0f,1.0f},{0.5f,0.5f,1.0f,1.0f},{1.0f,1.0f,0.0f,1.0f}};

static GLfloat ambient[]={0.1f,0.1f,0.1f,1.0f};             //이것은 자연광 1.0이 제일쎄고 지금은 0.1로했군..
static GLfloat diffuse[]={0.7f,0.7f,0.7f,1.0f};               //이것은 반사광
static GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};            //반짝임효과(재질때문에쓰인다
static GLfloat specreff[]={1.0f,1.0f,1.0f,1.0f};                      //위랑 똑같음
static GLfloat pos[]={200.0f,300.0f,400.0f,1.0f};                       //이것은 빛의 위치  x로500y로500z는0 

__declspec(dllexport) void GVGLinit(CDC *Hdc, HGLRC *hrc)
{
	int nPixelFormat;
//	Hdc =new CClientDC (GetDlgItem(IDC_WORLD));
	
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,0,
			0,0,0,0,0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	
	nPixelFormat =ChoosePixelFormat(Hdc->GetSafeHdc(),&pfd);
	VERIFY(SetPixelFormat(Hdc->GetSafeHdc(),nPixelFormat,&pfd));
	*hrc =wglCreateContext(Hdc->GetSafeHdc());	
	VERIFY(wglMakeCurrent(Hdc->GetSafeHdc(),*hrc));
}

__declspec(dllexport) void GVGLMakehDC(CDC *Hdc, HGLRC hrc)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
}

__declspec(dllexport) void GVGLSwapBuffers(CDC *Hdc)
{
	SwapBuffers(Hdc->GetSafeHdc());   //MAP

}

__declspec(dllexport) void GVGLSetupLight(GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
	glClearColor(R, G, B, A);

	
	glEnable(GL_DEPTH_TEST);          //깊이값표현 하기위해사용
//	glEnable(GL_CULL_FACE);                 //안보이는 부분은 랜더링해주지않음
//	glFrontFace(GL_CCW);                       //이건 앞과뒤를 구분하기위해사용됨 
	
	glEnable(GL_LIGHTING);                             //조명을 킨다는뜻
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);           //light0에  자연광을 넣고
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);                //light0에 반사광넣고
	glLightfv(GL_LIGHT0,GL_POSITION,pos);                  //light0에 자연광 위치넣고
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);               //light0반짝임 (재질넣고)

	glEnable(GL_LIGHT0);                                 //light0를 작동시킴
	
	glEnable(GL_COLOR_MATERIAL);                            //재질 파트
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);       //재질파트
	glMaterialfv(GL_FRONT,GL_SPECULAR,specreff);            //재질파트
	glMateriali(GL_FRONT,GL_SHININESS,128);
	
}

__declspec(dllexport) void GVGLResize(int cx, int cy)
{
 	GLfloat fAspect;
 	if(cy==0)
 		cy=1;
 	
 	glViewport(0,0,cx,cy);
 	
 	fAspect=(GLfloat)cx/(GLfloat)cy;
 	
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	gluPerspective(45.0f,fAspect,1.0f,-1.0f);
  
 	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();		
}

__declspec(dllexport) void GVGLCameraInit(GLfloat *cameraposmap,int W, int H,GLfloat *Angle)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(Angle[2],0.0f,0.0f,1.0f);

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);

	glRotatef(Angle[0],1.0f,0.0f,0.0f);
	glRotatef(Angle[1],0.0f,1.0f,0.0f);
		
}

__declspec(dllexport) void GVGLFlush()
{
	glFlush();
}

__declspec(dllexport) int GVGLGetPieceNum(CString regionName)
{
	char ch;
	int flag= 0;
	char buffer[100]={0,};
	int i=0;
	int masteri=0;
	int masterj=0;
	BOOL masterinput=1;
	int PieceNum;
	
	FILE *fp;
	fp=fopen(StringToChar(regionName), "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
	
	while((ch=fgetc(fp)) != EOF)
	{
		if(ch == ',' /*|| ch==0x0a*/)
		{
			flag = 1;
			buffer[i] = '\0';
			if(masterinput==1)  //처음엔 마스터 데이터 인풋!
			{
				masterj++;
				if(masterj>=2)
				{
					masterj=0;
					masteri++;
					if(masteri>=2)
					{
						masterinput=0;
						masteri=0;
					}
				}
			}
			else if(masterinput==0)
			{
				PieceNum= atoi(buffer);
				masterinput=2;
				break;
			}
			
			i=0;
		}	
		else 
		{
			buffer[i] = ch;
			i++;
		} 
	}

	fclose(fp);

	return PieceNum;
}

__declspec(dllexport) BOOL GVGLGetPiecePos(CString regionName,GVertex *Masterregion, GVertex **Pieceregion)  //get master region pos and piece pos
{

	char ch;
	int flag= 0;
	char buffer[100]={0,};
	int i=0;
	int masteri=0;
	int masterj=0;
	BOOL masterinput=1;
	int pieceicnt=0;
	

	FILE *fp;
	fp=fopen(StringToChar(regionName), "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
		
	while((ch=fgetc(fp)) != EOF)
	{
		if(ch == ',' /*|| ch==0x0a*/)
		{
			flag = 1;
			buffer[i] = '\0';
			if(masterinput==1)  //처음엔 마스터 데이터 인풋!
			{
				if(masterj==0)
				{
					Masterregion[masteri].x= atof(buffer);
				}
				else if(masterj==1)
				{
					Masterregion[masteri].y= atof(buffer);
					Masterregion[masteri].z=0.0f;
				}
				masterj++;
				if(masterj>=2)
				{
					masterj=0;
					masteri++;
					if(masteri>=2)
					{
						masterinput=0;
						masteri=0;
					}
				}
			}
			else if(masterinput==0)
			{
				masterinput=2;
			}
			else if(masterinput==2)
			{
				if(masterj==0)
				{
					Pieceregion[pieceicnt][masteri].x=atof(buffer);
				}
				else if(masterj==1)
				{
					Pieceregion[pieceicnt][masteri].y=atof(buffer);
					Pieceregion[pieceicnt][masteri].z=0.0f;
				}
				
				
				masterj++;
				if(masterj>=2)
				{
					masterj=0;
					masteri++;
					if(masteri>=2)
					{
						masteri=0;
						pieceicnt++;
					}
					
				}
			}
			i=0;
		}	
		else 
		{
			buffer[i] = ch;
			i++;
		} 
	}
	fclose(fp);

	if(pieceicnt==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

__declspec(dllexport) void GVGLMirrorApply(GVertex ** OriginPieceregion, GVertex **ChangePieceregion,int PieceNum,int MirrorMode)
{
	int i = 0, j = 0;
	int WidthCnt=0;
	int HighCnt=0;
	GLfloat dum_datay=0;
	GVertex ***DumData;
	GVertex ***DumData2;

	int first=0;

	for(i=0; i<PieceNum; i++)
	{
		if(first==0)
		{
			dum_datay=OriginPieceregion[i][0].y;
			first=1;
			HighCnt++;
		}
		else
		{
			if(dum_datay==OriginPieceregion[i][0].y)
			{
				break;
			}
			else
			{
				HighCnt++;
				dum_datay=OriginPieceregion[i][0].y;

			}
		}
	}

	WidthCnt=PieceNum/HighCnt;

	DumData=new GVertex **[HighCnt];
	DumData2=new GVertex **[HighCnt];
	for(i=0; i<HighCnt; i++)
	{
		DumData[i]=new GVertex *[WidthCnt];
		DumData2[i]=new GVertex *[WidthCnt];
		for(int j=0; j<WidthCnt; j++)
		{
			DumData[i][j]= new GVertex [2];
			DumData2[i][j]= new GVertex [2];
		}
	}

	int ii=0;
	int i3=0;
	int jcnt=0;
	BOOL onoff=0;
	for(j=0; j<WidthCnt; j++)
	{
		for(i=0; i<HighCnt; i++)
		{
		
			if(onoff==0)
			{
				DumData[i][j][0].x=OriginPieceregion[ii][0].x;
				DumData[i][j][0].y=OriginPieceregion[ii][0].y;
				DumData[i][j][0].z=OriginPieceregion[ii][0].z;
				DumData[i][j][1].x=OriginPieceregion[ii][1].x;
				DumData[i][j][1].y=OriginPieceregion[ii][1].y;
				DumData[i][j][1].z=OriginPieceregion[ii][1].z;
				DumData2[i][j][0].x=OriginPieceregion[ii][0].x;
				DumData2[i][j][0].y=OriginPieceregion[ii][0].y;
				DumData2[i][j][0].z=OriginPieceregion[ii][0].z;
				DumData2[i][j][1].x=OriginPieceregion[ii][1].x;
				DumData2[i][j][1].y=OriginPieceregion[ii][1].y;
				DumData2[i][j][1].z=OriginPieceregion[ii][1].z;
				jcnt++;
				if(jcnt>=HighCnt)
				{
					onoff=1;
					 i3=ii+jcnt;
				}
			}
			else
			{
				
				DumData[i][j][0].x=OriginPieceregion[i3][0].x;
				DumData[i][j][0].y=OriginPieceregion[i3][0].y;
				DumData[i][j][0].z=OriginPieceregion[i3][0].z;
				DumData[i][j][1].x=OriginPieceregion[i3][1].x;
				DumData[i][j][1].y=OriginPieceregion[i3][1].y;
				DumData[i][j][1].z=OriginPieceregion[i3][1].z;
				DumData2[i][j][0].x=OriginPieceregion[i3][0].x;
				DumData2[i][j][0].y=OriginPieceregion[i3][0].y;
				DumData2[i][j][0].z=OriginPieceregion[i3][0].z;
				DumData2[i][j][1].x=OriginPieceregion[i3][1].x;
				DumData2[i][j][1].y=OriginPieceregion[i3][1].y;
				DumData2[i][j][1].z=OriginPieceregion[i3][1].z;
				jcnt--;
				i3--;
				if(jcnt<=0)
				{
					onoff=0;
				}
			}
			ii++;
		}
	}


	ii=HighCnt-1;
	for(i=0; i<HighCnt; i++)
	{
		int jj=WidthCnt-1;
		for(j=0; j<WidthCnt; j++)
		{
			for(int k=0; k<2; k++)
			{
				if(MirrorMode==0)
				{
					DumData2[i][j][k].x=DumData[i][j][k].x;
					DumData2[i][j][k].y=DumData[i][j][k].y;
					DumData2[i][j][k].z=DumData[i][j][k].z;
				}
				else if(MirrorMode==1)
				{
					DumData2[i][j][k].x=DumData[i][jj][k].x;
					DumData2[i][j][k].y=DumData[i][jj][k].y;
					DumData2[i][j][k].z=DumData[i][jj][k].z;					
				}
				else if(MirrorMode==2)
				{
					DumData2[i][j][k].x=DumData[ii][j][k].x;
					DumData2[i][j][k].y=DumData[ii][j][k].y;
					DumData2[i][j][k].z=DumData[ii][j][k].z;
				}
				else if(MirrorMode==3) 
				{
					DumData2[i][j][k].x=DumData[ii][jj][k].x;
					DumData2[i][j][k].y=DumData[ii][jj][k].y;
					DumData2[i][j][k].z=DumData[ii][jj][k].z;
				}
			}
			jj--;
		}
		ii--;
	}


	ii=0;
	jcnt=0;
	onoff=0;
	i3=0;
	for(j=0; j<WidthCnt; j++)
	{
		for(int i=0; i<HighCnt; i++)
		{
			
			if(onoff==0)
			{
			
				ChangePieceregion[ii][0].x=DumData2[i][j][0].x;
				ChangePieceregion[ii][0].y=DumData2[i][j][0].y;
				ChangePieceregion[ii][0].z=DumData2[i][j][0].z;
				ChangePieceregion[ii][1].x=DumData2[i][j][1].x;
				ChangePieceregion[ii][1].y=DumData2[i][j][1].y;
				ChangePieceregion[ii][1].z=DumData2[i][j][1].z;
				jcnt++;
				if(jcnt>=HighCnt)
				{
					onoff=1;
					 i3=ii+jcnt;
				}
			}
			else
			{
				ChangePieceregion[i3][0].x=DumData2[i][j][0].x;
				ChangePieceregion[i3][0].y=DumData2[i][j][0].y;
				ChangePieceregion[i3][0].z=DumData2[i][j][0].z;
				ChangePieceregion[i3][1].x=DumData2[i][j][1].x;
				ChangePieceregion[i3][1].y=DumData2[i][j][1].y;
				ChangePieceregion[i3][1].z=DumData2[i][j][1].z;
				jcnt--;
				i3--;
				if(jcnt<=0)
				{
					onoff=0;
				}
			}
			ii++;
		}
	}

	for(i=0; i<HighCnt; i++)
	{
		for(j=0; j<WidthCnt; j++)
		{
			delete [] DumData[i][j];
		}
	}
	delete [] DumData;
}

__declspec(dllexport) void GVertexCopy(GVertex *Data,GVertex DST)
{
	Data->x=DST.x;
	Data->y=DST.y;
	Data->z=DST.z;
}

__declspec(dllexport) void GVertexFill(GVertex *Data,GLfloat num1, GLfloat num2, GLfloat num3)
{
	Data->x=num1;
	Data->y=num2;
	Data->z=num3;
}

__declspec(dllexport) void GVGLColorFill(GVColor *Data,GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
	Data->R=R;
	Data->G=G;
	Data->B=B;
	Data->A=A;
}
__declspec(dllexport) void GVGLColorFillInDex(GVColor *Data,int index)
{
	Data->R=GVCOLOR[index][0];
	Data->G=GVCOLOR[index][1];
	Data->B=GVCOLOR[index][2];
	Data->A=GVCOLOR[index][3];
}

__declspec(dllexport) void GVGLDisableDepthforBlend()
{
	glDisable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

__declspec(dllexport) void GVGLEnableDepth()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

__declspec(dllexport) void GVGLMouseDistance(CDC *Hdc, HGLRC hrc,GVertex MasterregionS,GVertex MasterregionE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle)
{

	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);

	GLdouble model[16];
	GLdouble proj[16];
	GLint viewport[4];
	double MasterWindowPosS[3];
	double MasterWindowPosE[3];
	
	glPushMatrix();
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetDoublev(GL_PROJECTION_MATRIX,proj);
	glGetIntegerv(GL_VIEWPORT,viewport);

//	glRotatef(0.0f,0.0f,0.0f,1.0f);

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);
//		glRotatef(0.0f,0.0f,0.0f,1.0f);

		
	Recombin2P(MasterregionS,MasterregionE);
	
	gluProject(MasterregionS.x, -MasterregionS.y, MasterregionS.z, model, proj, viewport, &MasterWindowPosS[0],&MasterWindowPosS[1],&MasterWindowPosS[2]);
	gluProject(MasterregionE.x,   -MasterregionE.y,   MasterregionE.z,   model, proj, viewport, &MasterWindowPosE[0],&MasterWindowPosE[1],&MasterWindowPosE[2]);	
 	MasterWindowPosS[1]=((float)H-(float)MasterWindowPosS[1]);
 	MasterWindowPosE[1]=((float)H-(float)MasterWindowPosE[1]);

	float worldxpos,worldypos;
	float windowxpos, windowypos;
	float incluseX,incluseY;

	worldxpos=MasterregionS.x-MasterregionE.x;
	worldypos=MasterregionS.y-MasterregionE.y;
	windowxpos=MasterWindowPosS[0]-MasterWindowPosE[0];
	windowypos=MasterWindowPosS[1]-MasterWindowPosE[1];

	incluseX=worldxpos/windowxpos;
	incluseY=worldypos/windowypos;



	if(Angle==0)
	{
		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;
	}
	else if(Angle==90 || Angle==-270)
	{	
		*mousey=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousex=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

	}
	else if(Angle==180)
	{
		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

	}
	else if(Angle==270 || Angle==-90)
	{
		*mousey=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousex=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

	}

	/*
	if(Angle==0)
	{
		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;
	}
	else if(Angle==90 || Angle==-270)
	{	
		*mousey=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousex=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

		*mousey=MasterregionE.y-(*mousey); //
	}
	else if(Angle==180)
	{
		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

		*mousey=MasterregionE.y-(*mousey);
		*mousex=MasterregionE.x-(*mousex);
	}
	else if(Angle==270 || Angle==-90)
	{
		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

		*mousex=MasterregionE.x-(*mousex);
	}
*/


	

	glPopMatrix();
}

__declspec(dllexport) BOOL GVGLMouseClickObject(CDC *Hdc, HGLRC hrc,CPoint mousepoint,GLfloat *cameraposmap, GVertex start, GVertex End,int W, int H,int windowSx,int windowSy,GLfloat Angle) //objectClick
{

	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);

	GLdouble model[16];
	GLdouble proj[16];
	GLint viewport[4];
	double MousePos[2][3];

	glPushMatrix();
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetDoublev(GL_PROJECTION_MATRIX,proj);
	glGetIntegerv(GL_VIEWPORT,viewport);
	
	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);
		
	start.y=start.y;
	End.y=End.y;

	Recombin2P(start,End);

	gluProject(start.x, -start.y, start.z, model, proj, viewport, &MousePos[0][0],&MousePos[0][1],&MousePos[0][2]);
	gluProject(End.x,   -End.y,   End.z,   model, proj, viewport, &MousePos[1][0],&MousePos[1][1],&MousePos[1][2]);	
	MousePos[0][1]=((float)H-(float)MousePos[0][1]);
	MousePos[1][1]=((float)H-(float)MousePos[1][1]);
	glPopMatrix();

	if(Angle==0)
	{
		if(mousepoint.x-windowSx>MousePos[0][0] &&mousepoint.x-windowSx<MousePos[1][0] && mousepoint.y-windowSy>MousePos[0][1] &&mousepoint.y-windowSy<MousePos[1][1] )
		{
			return 1;
		}
	}
	else if(Angle==90 || Angle==-270)
	{
		if(mousepoint.x-windowSx>MousePos[0][0] &&mousepoint.x-windowSx<MousePos[1][0] && mousepoint.y-windowSy<MousePos[0][1] &&mousepoint.y-windowSy>MousePos[1][1] )
		{
			return 1;
		}
	}
	else if(Angle==180)
	{
		if(mousepoint.x-windowSx<MousePos[0][0] &&mousepoint.x-windowSx>MousePos[1][0] && mousepoint.y-windowSy<MousePos[0][1] &&mousepoint.y-windowSy>MousePos[1][1] )
		{
			return 1;
		}
	}
	else if(Angle==270 || Angle==-90)
	{
		if(mousepoint.x-windowSx<MousePos[0][0] &&mousepoint.x-windowSx>MousePos[1][0] && mousepoint.y-windowSy>MousePos[0][1] &&mousepoint.y-windowSy<MousePos[1][1] )
		{
			return 1;
		}
	}
	return 0;
}

__declspec(dllexport) void GVGLMoveCamera(GLfloat *cameraposmap,GVertex RegionS, GVertex RegionE,GLfloat XPosVal, GLfloat YPosVal, GLfloat ZPosVal,GLfloat Angle)
{
	GLfloat a,b,c;

	float valx, valy;
	float xx=(RegionS.x-RegionE.x);
	float yy=(RegionS.y-RegionE.y);
	if(xx<0)xx=-xx;
	if(yy<0)yy=-yy;

	valx=xx/yy;
	valy=yy/xx;

	if(Angle==0)
	{
		cameraposmap[0]=XPosVal;
		cameraposmap[1]=YPosVal;
		cameraposmap[2]=ZPosVal;
	}
	else if(Angle==90 || Angle==-270)
	{	


		cameraposmap[0]=XPosVal*valx;
		cameraposmap[1]=YPosVal*valy;
		cameraposmap[2]=ZPosVal;
	}	
	else if(Angle==180)
	{
		cameraposmap[0]=XPosVal;
		cameraposmap[1]=YPosVal;
		cameraposmap[2]=ZPosVal;
	}
	else if(Angle==270 || Angle==-90)
	{
		cameraposmap[0]=XPosVal*valx;
		cameraposmap[1]=YPosVal*valy;
		cameraposmap[2]=ZPosVal;
	}
}
__declspec(dllexport) void GVGLRotateCamera(GLfloat *Angle)
{
	glRotatef(Angle[0],1.0f,0.0f,0.0f);
	glRotatef(Angle[1],0.0f,1.0f,0.0f);
	glRotatef(Angle[2],0.0f,0.0f,1.0f);
}
__declspec(dllexport) void GVGLDragMouse(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex RegionS, GVertex RegionE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle)
{

	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	if(nFlags==0)
	{
		referencepoint[0]=mouseWoldx;
		referencepoint[1]=mouseWoldy;
	}
	if(nFlags==1)
	{
		float disX;
		float disY;
		CString text;
		movingpoint[0]=mouseWoldx;
		movingpoint[1]=mouseWoldy;
		
		disX=(float)(referencepoint[0]-movingpoint[0]);
		disY=(float)(referencepoint[1]-movingpoint[1]);

		GVGLMoveCamera(cameraposmap,RegionS,RegionE,cameraposmap[0]-disX,cameraposmap[1]+disY,cameraposmap[2],Angle[2]);
		
	/*	if(Angle==0 || Angle==360)
		{
			GVGLMoveCamera(cameraposmap,cameraposmap[0]-disX,cameraposmap[1]+disY,cameraposmap[2],Angle);
		}
		else if(Angle==90 || Angle==-270)
		{
			GVGLMoveCamera(cameraposmap,cameraposmap[0]-disY,cameraposmap[1]+disX,cameraposmap[2],Angle);
		}
		else if(Angle==180)
		{
			GVGLMoveCamera(cameraposmap,cameraposmap[0]-disX,cameraposmap[1]+disY,cameraposmap[2],Angle);
		}
		else if(Angle==270 || Angle==-90)
		{
			GVGLMoveCamera(cameraposmap,cameraposmap[0]-disY,cameraposmap[1]+disX,cameraposmap[2],Angle);
		}
	*/	
		GVGLCameraInit(cameraposmap,MAPW,MAPH,Angle);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////DRAW Function
__declspec(dllexport) void GVGLDrawInit(int mode,int size,GVColor color)
{
	if(mode==GV_POINT)//point
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glPointSize(size);
		glBegin(GL_POINTS);
	}
	else if(mode==GV_LINE || mode==GV_RECTE ||mode==GV_CROSS || mode==GV_CROSSX ) //line, RectE
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_LINES);
	}
	else if(mode==GV_RECTF)
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_QUADS);
	}
	else if(mode==GV_RECTFG)
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_QUADS);
	}
	else if(mode==GV_CIRCLEE)
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_LINE_LOOP);
	}
	else if(mode==GV_CIRCLEF)
	{
		glPushMatrix();
		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_POLYGON);
	}
}

__declspec(dllexport) void GVGLDrawShow()
{
	glEnd();
	glPopMatrix();
}

__declspec(dllexport) void GVGLDrawVertex(GVertex V1)
{
	glVertex3f(V1.x,-V1.y,V1.z);
}

__declspec(dllexport) void GVGLDrawCross(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),-V1.y,V1.z);
	glVertex3f(V1.x+(Length/2),-V1.y,V1.z);

	glVertex3f(V1.x,-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x,-V1.y+(Length/2),V1.z);
}
__declspec(dllexport) void GVGLDrawCrossX(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x+(Length/2),-V1.y+(Length/2),V1.z);

	glVertex3f(V1.x+(Length/2),-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x-(Length/2),-V1.y+(Length/2),V1.z);
}


__declspec(dllexport) void GVGLDrawRectE(GVertex V1,GVertex V2)
{
	glVertex3f(V1.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V1.y,V1.z);

	glVertex3f(V2.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V2.y,V2.z);

	glVertex3f(V2.x,-V2.y,V2.z);
	glVertex3f(V1.x,-V2.y,V1.z);

	glVertex3f(V1.x,-V2.y,V1.z);
	glVertex3f(V1.x,-V1.y,V1.z);

}

__declspec(dllexport) void GVGLDrawRectF(GVertex V1,GVertex V2)
{

	glVertex3f(V1.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V1.y,V1.z);

	glVertex3f(V2.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V2.y,V2.z);

	glVertex3f(V2.x,-V2.y,V2.z);
	glVertex3f(V1.x,-V2.y,V1.z);

	glVertex3f(V1.x,-V2.y,V1.z);
	glVertex3f(V1.x,-V1.y,V1.z);
}

__declspec(dllexport) void GVGLDrawCircleE(GVertex V1,int radius)
{
	/*	double X,Y;
	for(int i=0; i < 20; i++)
	{
		GLfloat fAngle=2*PI*i/20;
		X=cos(fAngle) * radius;
		Y=sin(fAngle) * radius;
	    glVertex3f(V1.x-X , -V1.y-Y,V1.z);	
 	}
	*/  //너무 느림.
	
	int segments=20;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = V1.x + radius; 
	float y = V1.y; 
	
	for(int i=0; i<segments; i++)
	{
		glVertex3f(x, -y, V1.z);
		float tx = -(y - V1.y); 
		float ty = x - V1.x; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		float rx = V1.x- x; 
		float ry = V1.y- y; 
		x += rx * radial_factor; 
		y += ry * radial_factor; 
	}
}
__declspec(dllexport) void GVGLDrawCircleF(GVertex V1,int radius)
{
	/*	double X,Y,X2,Y2;
	for(int i=0; i < 20; i++)
	{
		GLfloat fAngle=2*PI*i/20;
		X=cos(fAngle) * radius;
		Y=sin(fAngle) * radius;

		GLfloat fAngle2=2*PI*(i+1)/20;
		X2=cos(fAngle2) * radius;
		Y2=sin(fAngle2) * radius;
			
		glVertex3f(V1.x , -V1.y,V1.z);
		glVertex3f(V1.x-X , -V1.y-Y,V1.z);
		glVertex3f(V1.x-X2 , -V1.y-Y2,V1.z);	
	 }*/

	int segments=20;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = V1.x + radius; 
	float y = V1.y; 
	
	for(int i=0; i<segments+1; i++)
	{
		glVertex3f(V1.x,-V1.y,V1.z);
		glVertex3f(x, -y, V1.z);
		float tx = -(y - V1.y); 
		float ty = x - V1.x; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		float rx = V1.x- x; 
		float ry = V1.y- y; 
		x += rx * radial_factor; 
		y += ry * radial_factor; 
		glVertex3f(x, -y, V1.z);		
	}
}

__declspec(dllexport) void GVGLDrawRectFG(GVertex V1,GVertex V2,int Level,int color)
{
	float radian;
	float red;
	float green;
	float blue;
	float glR,glG,glB;
	GLfloat pos[2][3];

	if(color==GV_GRADATION_PUSEUDO)  //pusedo
	{
		radian=(float)(((Level+30)*360)/256)*(3.1415/180);
		
		red=-sin(radian);
		blue=sin(radian);
		green=-cos(radian);
		
		glR=(red+1)*127.5;
		glG=(green+1)*127.5;
		glB=(blue+1)*127.5;
		
		glR=glR/255;
		glG=glG/255;
		glB=glB/255;
		
		glColor4f(glR,glG,glB,1.0f);	
	}
	else if(color==GV_GRADATION_GRAY)  //gray
	{
		glR=1.0f-(GLfloat)Level/255;
		glG=1.0f-(GLfloat)Level/255;
		glB=1.0f-(GLfloat)Level/255;
		
		glColor4f(glR,glG,glB,1.0f);	
	}

	else if(color==GV_GRADATION_REDSEA)  //blue <-> red
	{
		glR=(GLfloat)Level/255;
		glG=0;//(GLfloat)Level/255;
		glB=1.0f-(GLfloat)Level/255;
		
		glColor4f(glR,glG,glB,1.0f);	
	}

	else if(color==GV_GRADATION_AUTUMN)  //green <-> red
	{
		glR=(GLfloat)Level/255;
		glG=1.0f-(GLfloat)Level/255;
		glB=0;//1.0f-(GLfloat)Level/255;
		
		glColor4f(glR,glG,glB,1.0f);	
	}
	else if(color==GV_GRADATION_GREEN)  //green grad
	{
		glR=0;//(GLfloat)Level/255;
		glG=1.0f-(GLfloat)Level/255;
		glB=0;//1.0f-(GLfloat)Level/255;
		
		glColor4f(glR,glG,glB,1.0f);	
	}



	glVertex3f(V1.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V1.y,V1.z);

	glVertex3f(V2.x,-V1.y,V1.z);
	glVertex3f(V2.x,-V2.y,V2.z);

	glVertex3f(V2.x,-V2.y,V2.z);
	glVertex3f(V1.x,-V2.y,V1.z);

	glVertex3f(V1.x,-V2.y,V1.z);
	glVertex3f(V1.x,-V1.y,V1.z);
	
/*
	pos[0][0]=V1.x;
	pos[0][1]=-V1.y;
	pos[0][2]=V1.z;
	
	pos[1][0]=V2.x;
	pos[1][1]=-V2.y;
	pos[1][2]=V2.z;
	
	glRectfv(pos[0],pos[1]);
	*/
}




__declspec(dllexport)  void GVGLGotoCentermodel(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2)
 {
	cameraposmap[0]=0;
	cameraposmap[1]=0;
	 
	GLfloat Dragcenter[2];
	Recombin2P(Dragpoint1,Dragpoint2);
	Dragcenter[0]=(Dragpoint2.x+Dragpoint1.x)/2;
	Dragcenter[1]=(Dragpoint2.y+Dragpoint1.y)/2;

 	cameraposmap[0]=-Dragcenter[0];
 	cameraposmap[1]=Dragcenter[1];
 }
 
__declspec(dllexport)  void GVGLSetFit(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger)
 {
	 GLfloat minrepoint[3];
	 GLfloat maxrepoint[3];
	 GLfloat modelpoint[4];

	 double MousePosL[3];
	 double MousePosH[3];
	 GLdouble model[16];	
	 GLdouble proj[16];
	 GLint viewport[4];

	 cameraposmap[2]=-100000.0f;

	 Recombin2P(Dragpoint1,Dragpoint2);

	 modelpoint[0]=Dragpoint1.x;  //minx
	 modelpoint[1]=Dragpoint2.x;   //maxx
	 modelpoint[2]=-Dragpoint1.y;   //miny
	 modelpoint[3]=Dragpoint2.y;   //maxy
	 
	 int loopcnt=0;
	 for(;;)
	 {
		GLfloat fAspect;
		if(H==0)
			H=1;
		glViewport(0,0,W,H);
		fAspect=(GLfloat)W/(GLfloat)H;
		 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,fAspect,1.0f,-500.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);

		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glGetDoublev(GL_PROJECTION_MATRIX,proj);
		glGetIntegerv(GL_VIEWPORT,viewport);

		gluProject(modelpoint[0], modelpoint[2], -Dragpoint1.z, model, proj, viewport, &MousePosL[0],&MousePosL[1],&MousePosL[2]);

		if(Bigger==1)  //점점 커져서 fit한다.
		{
			if(0.0f+Extra>=MousePosL[0] || H-Extra<=MousePosL[1] )
			{	 
				break;
			}
			else
			{
				cameraposmap[2]+=10.0f;
			}
		}
		else  //점점 작아져서 fit함.
		{
			if(0.0f+Extra<=MousePosL[0] && H-Extra>=MousePosL[1] )
			{	 
				break;
			}
			else
			{
				cameraposmap[2]-=10.0f;
			}

		}
		loopcnt++;
		if(loopcnt>60000)
		{
	//		MessageBoxA(0,"Please cheack your Gvertex!!!","Caution",MB_OK);
			break;
		}
	}
	 
 }


__declspec(dllexport)  void Recombin2P(GVertex &LowPoint, GVertex &HighPoint)
 {
 	GLfloat dumpoint;
 	if(LowPoint.x>HighPoint.x)
 	{
 		dumpoint=LowPoint.x;
 		LowPoint.x=HighPoint.x;
 		HighPoint.x=dumpoint;
 	}
 
 	if(LowPoint.y>HighPoint.y)
 	{
 		dumpoint=LowPoint.y;
 		LowPoint.y=HighPoint.y;
 		HighPoint.y=dumpoint;
 	}
 }


__declspec(dllexport) void GVGLFont(const char str[],int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R,GLfloat G,GLfloat B)
{
	va_list args;
	char buffer[200], *p;
	int l;

	va_start(args, str);
	vsprintf(buffer, str, args);
	va_end(args);




	glPushMatrix();
	glEnable(GL_SMOOTH);
	glTranslatef(Posx, -Posy, Posz);
	glLineWidth(Linewidth);

	glColor3f(R,G,B);
	glScalef(0.05f*size, 0.05f*size, 0.05f);

	if(mode==GVFONT_ROMAN)
	{
	  for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
	else
	{
	  for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
	}

	glScalef(1.0f/(0.05f*size), 1.0f/(0.05f*size), 1.0f/0.05f);

	glDisable(GL_SMOOTH);
	glPopMatrix();
}



GVertex CRSpline(double t, GVertex p1, GVertex p2, GVertex p3, GVertex p4) 
{
    double tt = t*t ;
    double ttt = tt * t ;
    double x = 0.5 * ((-p1.x + 3*p2.x -3*p3.x + p4.x)*ttt
        + (2*p1.x -5*p2.x + 4*p3.x - p4.x)*tt
        + (-p1.x+p3.x)*t
        + 2*p2.x) ;
    double y = 0.5 * ((-p1.y + 3*p2.y -3*p3.y + p4.y)*ttt
        + (2*p1.y -5*p2.y + 4*p3.y - p4.y)*tt
        + (-p1.y+p3.y)*t
        + 2*p2.y) ;
	double z = 0.5 * ((-p1.z + 3*p2.z -3*p3.z + p4.z)*ttt
        + (2*p1.z -5*p2.z + 4*p3.z - p4.z)*tt
        + (-p1.z+p3.z)*t
        + 2*p2.z) ;
	
	GVertex a;
	
	GVertexFill(&a,x,y,z);
    return a ;
}

__declspec(dllexport) void GVGLRomCurve2D(int nPSize,GVertex *Point,int tval,GVColor Color,int LineSize)
{
	glPushMatrix();
	glColor4f(Color.R,Color.G,Color.B,Color.A);
	glLineWidth(LineSize);
	glBegin(GL_LINE_STRIP);
	GVertex p[4];

	for(int i = 0; i < (nPSize-1); i++) 
	{ 
        for(int it = 1; it <= tval; it++)
		{
			if(i-1<0)		p[0]=Point[0];
			else  p[0]=Point[i-1];

			p[1]=Point[i];
			p[2]=Point[i+1];

			if(i+2>nPSize-1)		p[3]=Point[nPSize-1];
			else  p[3]=Point[i+2];

            GVertex P = CRSpline(double(it)/tval,p[0],p[1],p[2],p[3]);
			
			GVGLDrawVertex(P);
        }
    }
	GVGLDrawShow();
}

__declspec(dllexport) void GVGLRomCurve3D(int nPSizex,int nPSizey,GVertex **Point,int tval,GVColor Color,int LineSize)
{
	CString aa;
	GVertex Dump[4];
	GVertex **P;
	GVertex **P2;
	int i,j,k;

	P=new GVertex *[nPSizey*tval];
	P2=new GVertex *[nPSizey*tval];

	for(i=0; i<nPSizey*tval; i++)
	{
		P[i]=new GVertex [nPSizex];
		P2[i]=new GVertex [nPSizex*tval];
	}

	for(i=0; i<nPSizex; i++)
	{
		k=0;
		for(j = 0; j < (nPSizey-1); j++) 
		{ 
			for(int it = 1; it <= tval; it++)
			{
				if(j-1<0)		Dump[0]=Point[0][i];
				else  Dump[0]=Point[j-1][i];
				Dump[1]=Point[j][i];
				Dump[2]=Point[j+1][i];
				if(j+2>nPSizey-1)	Dump[3]=Point[nPSizey-1][i];
				else  Dump[3]=Point[j+2][i];

				P[k][i] = CRSpline(double(it)/tval,Dump[0],Dump[1],Dump[2],Dump[3]);
				k++;
			}
		};
	}
	
	for(i=0; i<nPSizey*tval; i++)
	{
		k=0;
		for(int j = 0; j < (nPSizex-1); j++) 
		{ 
			for(int it = 1; it <= tval; it++)
			{
				if(j-1<0)		Dump[0]=P[i][0];
				else  Dump[0]=P[i][j-1];
				Dump[1]=P[i][j];
				Dump[2]=P[i][j+1];
				if(j+2>nPSizex-1)	Dump[3]=P[i][nPSizex-1];
				else  Dump[3]=P[i][j+2];
				
				P2[i][k] = CRSpline(double(it)/tval,Dump[0],Dump[1],Dump[2],Dump[3]);
				k++;
			}
		}
	}

	glPushMatrix();
	glColor4f(Color.R,Color.G,Color.B,Color.A);
	glPointSize(3);
	glBegin(GL_QUADS);

	for(i=0; i<((nPSizey-1)*tval)-1; i++)
	{
		for(j=0; j<((nPSizex-1)*tval)-1; j++)
		{
			GVGLDrawVertex(P2[i][j]);
			GVGLDrawVertex(P2[i][j+1]);
			GVGLDrawVertex(P2[i+1][j+1]);
			GVGLDrawVertex(P2[i+1][j]);
		}		
	}
	GVGLDrawShow();
	

	for(i=0; i<nPSizey*tval; i++)
	{
		delete [] P[i];
		delete [] P2[i];
	}

	delete [] P;
	delete [] P2;
	
}

__declspec(dllexport) void GVGLRomCurve3DGrad(int nPSizex,int nPSizey,GVertex **Point, int tval,GVColor **Color,int LineSize)
{
	CString aa;
	GVertex Dump[4];
	GVertex **P;
	GVertex **P2;
	GVColor **PColor;
	int i,j,k;
	
	P=new GVertex *[nPSizey*tval];
	P2=new GVertex *[nPSizey*tval];
	PColor=new GVColor *[nPSizey*tval];
	for(i=0; i<nPSizey*tval; i++)
	{
		P[i]=new GVertex [nPSizex];
		P2[i]=new GVertex [nPSizex*tval];
		PColor[i]=new GVColor [nPSizex*tval];
	}
	
	for(i=0; i<nPSizex; i++)
	{
		k=0;
		for(j = 0; j < (nPSizey-1); j++) 
		{ 
			for(int it = 1; it <= tval; it++)
			{
				if(j-1<0)		Dump[0]=Point[0][i];
				else  Dump[0]=Point[j-1][i];
				Dump[1]=Point[j][i];
				Dump[2]=Point[j+1][i];
				if(j+2>nPSizey-1)	Dump[3]=Point[nPSizey-1][i];
				else  Dump[3]=Point[j+2][i];
				
				P[k][i] = CRSpline(double(it)/tval,Dump[0],Dump[1],Dump[2],Dump[3]);
				k++;
			}
		};
	}
	
	int numx,numy;
	for(i=0; i<nPSizey*tval; i++)
	{
		k=0;
		for(int j = 0; j < (nPSizex-1); j++) 
		{ 
			for(int it = 1; it <= tval; it++)
			{
				if(j-1<0) Dump[0]=P[i][0];
				else  Dump[0]=P[i][j-1];
				Dump[1]=P[i][j];
				Dump[2]=P[i][j+1];
				if(j+2>nPSizex-1)	Dump[3]=P[i][nPSizex-1];
				else  Dump[3]=P[i][j+2];
				
				P2[i][k] = CRSpline(double(it)/tval,Dump[0],Dump[1],Dump[2],Dump[3]);
				numx=i/tval;
				numy=k/tval;
				GVGLColorFill(&PColor[i][k],Color[numx][numy].R,Color[numx][numy].G,Color[numx][numy].B,Color[numx][numy].A);
				k++;
			}
		}
	}
	

	glShadeModel(GL_SMOOTH);
	glPushMatrix();
	glLineWidth(1);
	glBegin(GL_QUADS);
	for(i=0; i<((nPSizey-1)*tval)-1; i++)
	{
		for(j=0; j<((nPSizex-1)*tval)-1; j++)
		{
			glColor4f(PColor[i][j].R,PColor[i][j].G,PColor[i][j].B,PColor[i][j].A);
			GVGLDrawVertex(P2[i][j]);
			glColor4f(PColor[i][j+1].R,PColor[i][j+1].G,PColor[i][j+1].B,PColor[i][j+1].A);
			GVGLDrawVertex(P2[i][j+1]);
			glColor4f(PColor[i+1][j+1].R,PColor[i+1][j+1].G,PColor[i+1][j+1].B,PColor[i+1][j+1].A);
			GVGLDrawVertex(P2[i+1][j+1]);
			glColor4f(PColor[i+1][j].R,PColor[i+1][j].G,PColor[i+1][j].B,PColor[i+1][j].A);
			GVGLDrawVertex(P2[i+1][j]);
		}		
	}
	GVGLDrawShow();
	
	for(i=0; i<nPSizey*tval; i++)
	{
		delete [] P[i];
		delete [] P2[i];
		delete [] PColor[i];
	}
	delete [] P;
	delete [] P2;
	delete [] PColor;
}
__declspec(dllexport) void GVGLHistoinit(GLfloat *cameraposmap,int W, int H,int x,int y,GLfloat TSize,GVColor TColor)
{
	GVertex a,b,c;
	GLfloat xval,yval;
	GVColor color[2];
	CString numbering;
	GLfloat ywidth;
		 

	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;

	GVGLColorFill(&color[0],TColor.R,TColor.G,TColor.B,TColor.A);
	GVGLColorFill(&color[1],0.3f,0.3f,0.3f,1.0f);
	GVertexFill(&a,0.0f,0.0f,0.0f);
	GVertexFill(&b,xval,-yval,0.0f);
	GVGLGotoCentermodel(cameraposmap,a,b);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap,a,b,25,W,H,1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.

	GVertexFill(&b,xval,0.0f,0.0f);
	GVertexFill(&c,0.0f,-yval,0.0f);

	GVGLFont("0",1,-30,10.0f,0.0f,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);
	numbering.Format(_T("%.1f"),(GLfloat)x);
	GVGLFont(StringToChar(numbering),1,b.x,b.y+10,b.z,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);
	numbering.Format(_T("%.1f"),(GLfloat)(y-1));
	GVGLFont(StringToChar(numbering),1,c.x-30,c.y,c.z,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);

	GVGLDrawInit(GV_LINE,2,color[0]);
	GVGLDrawVertex(a);
	GVGLDrawVertex(b);
	GVGLDrawVertex(a);
	GVGLDrawVertex(c);
	GVGLDrawShow();


	GLfloat yval2;
	ywidth=(GLfloat)(yval/5);
	yval2=ywidth;
	for(int i=0; i<5; i++)
	{
		GVertexFill(&a,0,-yval2,0.0f);
		GVertexFill(&b,xval,-yval2,0.0f);
		GVGLDrawInit(GV_LINE,1,color[1]);
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
		GVGLDrawShow();
		yval2+=ywidth;
	}

}

__declspec(dllexport) void GVGLDrawHisto1(GLfloat *HistoData1,GVColor LColor,int W, int H,int x,int y,GLfloat TSize)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;

	Rxval=xval/x;
	Ryval=yval/y;

	GVGLDrawInit(GV_LINE,1,LColor);
	for(int i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData1[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData1[i+1]*Ryval),0.0f);

		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();
}
__declspec(dllexport) void GVGLDrawHisto2(GLfloat *HistoData1,GLfloat *HistoData2,GVColor *LColor,int W, int H,int x,int y,GLfloat TSize)
{
	int i = 0;
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;
	
	Rxval=xval/x;
	Ryval=yval/y;
	
	GVGLDrawInit(GV_LINE,1,LColor[0]);
	for(i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData1[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData1[i+1]*Ryval),0.0f);
		
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();


	GVGLDrawInit(GV_LINE,1,LColor[1]);
	for(i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData2[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData2[i+1]*Ryval),0.0f);
		
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();

}
__declspec(dllexport) void GVGLDrawHisto3(GLfloat *HistoData1,GLfloat *HistoData2,GLfloat *HistoData3,GVColor *LColor,int W, int H,int x,int y,GLfloat TSize)
{
	int i;
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;
	
	Rxval=xval/x;
	Ryval=yval/y;
	
	GVGLDrawInit(GV_LINE,1,LColor[0]);
	for(i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData1[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData1[i+1]*Ryval),0.0f);
		
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();

	GVGLDrawInit(GV_LINE,1,LColor[1]);
	for(i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData2[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData2[i+1]*Ryval),0.0f);
		
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();

	GVGLDrawInit(GV_LINE,1,LColor[2]);
	for(i=0; i<x-1; i++)
	{
		GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData3[i]*Ryval),0.0f);
		GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData3[i+1]*Ryval),0.0f);
		
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
	}
	GVGLDrawShow();

}



__declspec(dllexport) BOOL GVGLInitmappingOriginBIG(CString name,GLubyte **texturera,GLubyte **texturera2,int *SizeX,int *SizeY)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
	int w,h;
	w = FreeImage_GetWidth(imagen);
	*SizeX=w;
	h = FreeImage_GetHeight(imagen);
	*SizeY=h;

	if(*texturera!=NULL)
	{
		delete [] *texturera;
		*texturera=NULL;
	}
	if(*texturera2!=NULL)
	{
		delete [] *texturera2;
		*texturera2=NULL;
	}

	*texturera = new GLubyte[w*h/2];
	*texturera2 = new GLubyte[w*h/2];
	
	FreeImage_Unload(imagen);
	imagen=NULL;
	return 1;
}


__declspec(dllexport) BOOL GVGLInitmappingOrigin(CString name,GLubyte **texturera,int *SizeX,int *SizeY)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
	int w,h;
	int gray=0;

	w = FreeImage_GetWidth(imagen);
	*SizeX=w;
	h = FreeImage_GetHeight(imagen);
	*SizeY=h;

	if(w==0 && h==0)
	{
		FreeImage_Unload(imagen);
		return 0;
	}

	if(*texturera!=NULL)
	{
		delete [] *texturera;
		*texturera=NULL;
	}

	if(gray==0) *texturera = new GLubyte[4*w*h];
	else *texturera = new GLubyte[w*h];

	FreeImage_Unload(imagen);
	imagen=NULL;
	return 1;
}

__declspec(dllexport) BOOL GVGLInitmappingResize(CString name,GLubyte **texturera,int *SizeX,int *SizeY,int Multival)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
	int w,h;
	int gray=0;
	w = FreeImage_GetWidth(imagen);
	*SizeX=(GLfloat)w/Multival+0.5;
	h = FreeImage_GetHeight(imagen);
	*SizeY=(GLfloat)h/Multival+0.5;

	if(w==0 && h==0)
	{
		FreeImage_Unload(imagen);
		return 0;
	}

	int resizeX,resizeY;
	resizeX=*SizeX;
	resizeY=*SizeY;

	if(*texturera!=NULL)
	{
		delete [] *texturera;
		*texturera=NULL;
	}

	
	if(gray==0) *texturera = new GLubyte[4*resizeX*resizeY];
	else *texturera = new GLubyte[resizeX*resizeY];
	
	FreeImage_Unload(imagen);
	imagen=NULL;
	return 1;
}

__declspec(dllexport) BOOL GVGLGetMappingOriginBIG(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,GLubyte *texturera2,int SizeX, int SizeY)
{
	int i = 0, j = 0;
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
	FIBITMAP* temp;
	temp = FreeImage_ConvertTo8Bits(imagen);
		
	char* pixeles = (char*)FreeImage_GetBits(temp);
	char** data;
		
	data= new char *[SizeY];
	for(i=0; i<SizeY; i++)
	{
		data[i]= new char [SizeX];
	}
		
	int ii=0;
	int ii2=0;
	int jj=0;
	for(j= 0; j<SizeX*SizeY; j++)
	{
		data[ii][jj]=pixeles[j];
		jj++;
		if(jj>=SizeX)
		{
			jj=0;
			ii++;
		}
	}
		
	ii=0;
	for(i=SizeY-1; i>=0; i--)
	{
		for(int j=0; j<SizeX; j++)
		{
			if(ii<SizeX*SizeY/2)
			{
				texturera[ii]= data[i][j];
				ii++;
			}
			else
			{
				texturera2[ii2]= data[i][j];
				ii2++;
			}
		
		}
	}
		
	for(i=0; i<SizeY; i++)
	{
		delete [] data[i];
	}
	delete [] data;		
	FreeImage_Unload(imagen);
	imagen=NULL;
	FreeImage_Unload(temp);
	temp=NULL;


	glGenTextures(3, (GLuint*)texturera);
	glBindTexture(GL_TEXTURE_2D, *texturera);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, SizeX, SizeY/2, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(3, (GLuint*)texturera2);
	glBindTexture(GL_TEXTURE_2D, *texturera2);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, SizeX, SizeY/2, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera2 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	return 1;
}

__declspec(dllexport) BOOL GVGLGetMappingOrigin(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,int SizeX, int SizeY,int Mirror)
{
	int i = 0, j = 0;

	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	int gray=0;
	if(gray==0)
	{
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
		FIBITMAP* temp;
		temp = FreeImage_ConvertTo32Bits(imagen);
		
		char* pixeles = (char*)FreeImage_GetBits(temp);
		char** data;
		
		data= new char *[SizeY];
		for(i=0; i<SizeY; i++)
		{
			data[i]= new char [SizeX*4];
		}
		
		int ii=0;
		int jj=0;
		for(j= 0; j<SizeX*SizeY*4; j++)
		{
			data[ii][jj]=pixeles[j];
			jj++;
			if(jj>=SizeX*4)
			{
				jj=0;
				ii++;
			}
		}
		
		ii=0;

		if(Mirror==0)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(j=0; j<SizeX*4; j+=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		else if(Mirror==1)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=(SizeX*4)-4; j>=0; j-=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
				}
			}
		}
		else if(Mirror==2)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=0; j<SizeX*4; j+=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=(SizeX*4)-4; j>=0; j-=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		
		
		for(i=0; i<SizeY; i++)
		{
			delete [] data[i];
		}
		delete [] data;

		pixeles=NULL;
		data=NULL;
		FreeImage_Unload(imagen);
		imagen=NULL;
		FreeImage_Unload(temp);
		temp=NULL;

		glGenTextures(3, (GLuint*)texturera);
		glBindTexture(GL_TEXTURE_2D, *texturera);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, SizeX, SizeY, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)texturera );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
		FIBITMAP* temp;
		temp = FreeImage_ConvertTo8Bits(imagen);
		
		char* pixeles = (char*)FreeImage_GetBits(temp);
		char** data;
		
		data= new char *[SizeY];
		for(int i=0; i<SizeY; i++)
		{
			data[i]= new char [SizeX];
		}
		
		int ii=0;
		int jj=0;
		for(int j= 0; j<SizeX*SizeY; j++)
		{
			data[ii][jj]=pixeles[j];
			jj++;
			if(jj>=SizeX)
			{
				jj=0;
				ii++;
			}
		}
		
		ii=0;

		if(Mirror==0)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=0; j<SizeX; j++)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}
		else if(Mirror==1)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=SizeX-1; j>=0; j--)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
				int ttt=0;
			}	
		}
		else if(Mirror==2)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=0; j<SizeX; j++)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=SizeX-1; j>=0; j--)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}
		
		
		for(i=0; i<SizeY; i++)
		{
			delete [] data[i];
		}
		delete [] data;		

		pixeles=NULL;
		data=NULL;
		FreeImage_Unload(imagen);
		imagen=NULL;
		FreeImage_Unload(temp);
		temp=NULL;


		glGenTextures(3, (GLuint*)texturera);
		glBindTexture(GL_TEXTURE_2D, *texturera);
		glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, SizeX, SizeY, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	return 1;
}


__declspec(dllexport) BOOL GVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	int gray=0, i=0, j=0;
	if(gray==0)
	{
		int ii=0;
		int jj=0;
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo32Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		for(i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX*4];
		}
		
		for(j= 0; j<SizeX*SizeY*4; j++)
		{
			data[ii][jj]=pixeles[j];
			jj++;
			if(jj>=SizeX*4)
			{
				jj=0;
				ii++;
			}
		}
		
		ii=0;
		if(Mirror==0)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(j=0; j<SizeX*4; j+=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		else if(Mirror==1)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=(SizeX*4)-4; j>=0; j-=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
				}
			}
		}
		else if(Mirror==2)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=0; j<SizeX*4; j+=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=(SizeX*4)-4; j>=0; j-=4)
				{
					texturera[ii*4+0]= data[i][j+2];
					texturera[ii*4+1]= data[i][j+1];
					texturera[ii*4+2]= data[i][j+0];
					texturera[ii*4+3]= data[i][j+3];
					ii++;
					
				}
			}
		}
		
		for(i=0; i<SizeY; i++)
		{
			delete [] data[i];
		}
		delete [] data;

		pixeles=NULL;
		data=NULL;

		FreeImage_Unload(imagen);
		imagen=NULL;
		FreeImage_Unload(aa);
		aa=NULL;
		FreeImage_Unload(bb);
		bb=NULL;

		glGenTextures(3, (GLuint*)texturera);

		glBindTexture(GL_TEXTURE_2D, *texturera);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, SizeX, SizeY, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)texturera );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	}
	else
	{
		int ii=0;
		int jj=0;
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(StringToChar(name),0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, StringToChar(name));
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo8Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		for(int i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX];
		}
		
		for(int j= 0; j<SizeX*SizeY; j++)
		{
			data[ii][jj]=pixeles[j];
			jj++;
			if(jj>=SizeX)
			{
				jj=0;
				ii++;
			}
		}
		
		ii=0;
		if(Mirror==0)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=0; j<SizeX; j++)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}
		else if(Mirror==1)
		{
			for(i=SizeY-1; i>=0; i--)
			{
				for(int j=SizeX-1; j>=0; j--)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
				int ttt=0;
			}	
		}
		else if(Mirror==2)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=0; j<SizeX; j++)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(int j=SizeX-1; j>=0; j--)
				{
					texturera[ii]= data[i][j];
					ii++;
				}
			}
		}

		for(i=0; i<SizeY; i++)
		{
			delete [] data[i];
		}
		delete [] data;

		pixeles=NULL;
		data=NULL;
		
		FreeImage_Unload(imagen);
		imagen=NULL;
		FreeImage_Unload(aa);
		aa=NULL;
		FreeImage_Unload(bb);
		bb=NULL;

		glGenTextures(3, (GLuint*)texturera);
		glBindTexture(GL_TEXTURE_2D, *texturera);
		glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, SizeX, SizeY, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	return 1;
}

__declspec(dllexport) void GVGLTextureMapping(int Mappingmodel,GLubyte * texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL Full)
{
	if(texturera)
	{
		if(Full==0)
		{
			if(Mappingmodel==0)
			{
				glEnable(GL_TEXTURE_2D);
				
				glPushMatrix();
				glBindTexture ( GL_TEXTURE_2D, *texturera );
				glBegin ( GL_QUADS );
				glColor3f(1.0f,1.0f,1.0f);
				glNormal3f(0.0f,0.0f,1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -startpoint.y,  startpoint.z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -Endpoint.y,   startpoint.z);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y,  startpoint.z); 
				glEnd();
				glPopMatrix();
				
				glDisable(GL_TEXTURE_2D);
			}	
		}
		else
		{
			if(Mappingmodel==0)
			{
				glEnable(GL_TEXTURE_2D);
				
				glPushMatrix();
				glBindTexture ( GL_TEXTURE_2D, *texturera );
				glBegin ( GL_QUADS );
				glColor3f(1.0f,1.0f,1.0f);
				glNormal3f(0.0f,0.0f,1.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -startpoint.y,  startpoint.z);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, -SizeY,   startpoint.z);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -SizeY,  startpoint.z); 
				glEnd();
				glPopMatrix();
				
				glDisable(GL_TEXTURE_2D);
			}	
		}
	}
	
}

__declspec(dllexport) void GVGLTextureMappingBIG(int Mappingmodel,GLubyte * texturera,GLubyte * texturera2,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY)
{
	if(texturera)
	{
		if(Mappingmodel==0)
		{
			glEnable(GL_TEXTURE_2D);

			glPushMatrix();
			glBindTexture ( GL_TEXTURE_2D, *texturera );
			glBegin ( GL_QUADS );
			glColor3f(1.0f,1.0f,1.0f);
			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -startpoint.y,  startpoint.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -Endpoint.y/2,   startpoint.z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y/2,  startpoint.z); 
			glEnd();
			glPopMatrix();
			
			glDisable(GL_TEXTURE_2D);
			
			
			glEnable(GL_TEXTURE_2D);
			
			glPushMatrix();
			glBindTexture ( GL_TEXTURE_2D, *texturera2 );
			glBegin ( GL_QUADS );
			glColor3f(1.0f,1.0f,1.0f);
			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -Endpoint.y/2,  startpoint.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -Endpoint.y/2,  startpoint.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -Endpoint.y,   startpoint.z);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y,  startpoint.z); 
			glEnd();
			glPopMatrix();
			
			glDisable(GL_TEXTURE_2D);
		}	
	}
}

__declspec(dllexport) void GVGLReleaseMapping(GLubyte ** texturera)
{
	if(*texturera)
	{
		delete [] *texturera;
		*texturera=NULL;
	}
	
}