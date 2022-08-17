#include "stdafx.h"
#include "GVGL.h"
#include "FreeImage.h"
#include <math.h>
#include "../Global/GlobalFunc.h"

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



 void GVGLReleaseDC(CDC *Hdc, HGLRC *hrc,HWND m_hWnd)
{
	wglDeleteContext(*hrc);
	::ReleaseDC(m_hWnd,Hdc->GetSafeHdc());
	
	delete  [] Hdc;
	
}

 void GVGLinit(CDC *Hdc, HGLRC *hrc)
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
			1,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	
	nPixelFormat =ChoosePixelFormat(Hdc->GetSafeHdc(),&pfd);
	VERIFY(SetPixelFormat(Hdc->GetSafeHdc(),nPixelFormat,&pfd));
	*hrc =wglCreateContext(Hdc->GetSafeHdc());	
	VERIFY(wglMakeCurrent(Hdc->GetSafeHdc(),*hrc));

	int argc = 1;
	char *argv[1] = { (char*)"Something" };
	glutInit(&argc, argv);
}

 void GVGLShareRC(HGLRC m_hRC,HGLRC m_hRC2)
{
	wglShareLists(m_hRC,m_hRC2);
}

 void GVGLMakehDC(CDC *Hdc, HGLRC hrc)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
}

 void GVGLSwapBuffers(CDC *Hdc)
{
	SwapBuffers(Hdc->GetSafeHdc());   //MAP

}


 void GVGLSetupLightStencil(GLfloat R, GLfloat G, GLfloat B,GLfloat A,BOOL CirclePoint)
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);

	glEnable(GL_DEPTH_TEST);

//	glFrontFace(GL_CW);
	// Clear blue window
	glClearColor(R, G, B, A);

	if(CirclePoint)glEnable(GL_POINT_SMOOTH);

	// Use 0 for clear stencil, enable stencil test
	glClearStencil(0.0f);
//	glEnable(GL_STENCIL_TEST);

	// Clear color and stencil buffer
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


 void GVGLSetupLight(GLfloat R, GLfloat G, GLfloat B,GLfloat A,BOOL CirclePoint)
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

	if(CirclePoint)glEnable(GL_POINT_SMOOTH);
	
	glEnable(GL_COLOR_MATERIAL);                            //재질 파트
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);       //재질파트
	glMaterialfv(GL_FRONT,GL_SPECULAR,specreff);            //재질파트
	glMateriali(GL_FRONT,GL_SHININESS,128);
	
}

 void GVGLResize(int cx, int cy)
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

 void GVGLCameraInit(GLfloat *cameraposmap,int W, int H,GLfloat *Angle)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(Angle[2],0.0f,0.0f,1.0f);

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);

	glRotatef(Angle[0],1.0f,0.0f,0.0f);
	glRotatef(Angle[1],0.0f,1.0f,0.0f);
		
}

 void GVGLCameraInitStencil(GLfloat *cameraposmap,int W, int H,GLfloat *Angle)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(Angle[2],0.0f,0.0f,1.0f);

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);

	glRotatef(Angle[0],1.0f,0.0f,0.0f);
	glRotatef(Angle[1],0.0f,1.0f,0.0f);
		
}


 void GVGLFlush()
{
	glFlush();
}

 int GVGLGetPieceNum(CString regionName)
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
#ifdef UNICODE
	char *pText = StringToChar(regionName);

	fp=fopen(pText, "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
	delete [] pText;
	pText = NULL;
#else
	fp=fopen(regionName, "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
#endif
	
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

 BOOL GVGLGetPiecePos(CString regionName,GVertex *Masterregion, GVertex **Pieceregion)  //get master region pos and piece pos
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
#ifdef UNICODE
	char *pText = StringToChar(regionName);

	fp=fopen(pText, "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
	delete [] pText;
	pText = NULL;
#else
	fp=fopen(regionName, "r"); // result.txt 파일이 쓰기 형태로 열되 없으면 새로 생성, 있으면 덮어씀
#endif
		
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

 void GVGLMirrorPieceApply(GVertex * PieceS, GVertex *PieceE,GVertex MasterS, GVertex MasterE ,int MirrorMode)
{
	if(MirrorMode!=0)
	{
		float Centerdevx,Centerdevy;
//		float posx,posy;
		float P2Pdevx,P2Pdevy;

		Centerdevx=(MasterE.x-MasterS.x)/2;
		Centerdevy=(MasterE.y-MasterS.y)/2;

		P2Pdevx=PieceE->x-PieceS->x;
		P2Pdevy=PieceE->y-PieceS->y;

		
		 if(MirrorMode==1)  //좌우미러링
		 {
			PieceS->x=Centerdevx-PieceS->x;
			PieceE->x=Centerdevx-PieceE->x;
			PieceS->x=Centerdevx+PieceS->x;
			PieceE->x=Centerdevx+PieceE->x;

			PieceS->x=PieceS->x-P2Pdevx;
			PieceE->x=PieceE->x+P2Pdevx;
		 }
		 else if(MirrorMode==2)  //상하미러링
		 {
			PieceS->y=Centerdevy-PieceS->y;
			PieceE->y=Centerdevy-PieceE->y;
			PieceS->y=Centerdevy+PieceS->y;
			PieceE->y=Centerdevy+PieceE->y;

			PieceS->y=PieceS->y-P2Pdevy;
			PieceE->y=PieceE->y+P2Pdevy;
		 }
		  else if(MirrorMode==3)  //좌우 상하미러링
		 {
			PieceS->x=Centerdevx-PieceS->x;
			PieceS->y=Centerdevy-PieceS->y;
			PieceE->x=Centerdevx-PieceE->x;
			PieceE->y=Centerdevy-PieceE->y;
			PieceS->x=Centerdevx+PieceS->x;
			PieceE->x=Centerdevx+PieceE->x;
			PieceS->y=Centerdevy+PieceS->y;
			PieceE->y=Centerdevy+PieceE->y;

			PieceS->x=PieceS->x-P2Pdevx;
			PieceE->x=PieceE->x+P2Pdevx;
			PieceS->y=PieceS->y-P2Pdevy;
			PieceE->y=PieceE->y+P2Pdevy;
		 }	
	}
}
 void GVGLMirrorDefectApply(GVertex *repos,GVertex Gpos,GVertex MasterS, GVertex MasterE,int MirrorMode)
{

	repos->x= Gpos.x;
	repos->y= Gpos.y;
	repos->z= Gpos.z;

	if(MirrorMode!=0)
	{
		float Centerdevx,Centerdevy;

		Centerdevx=(MasterE.x-MasterS.x)/2;
		Centerdevy=(MasterE.y-MasterS.y)/2;

		if(MirrorMode==1)  //좌우미러링
		{
			repos->x=Centerdevx-repos->x;
			repos->x=Centerdevx+repos->x;

		}
		else if(MirrorMode==2)  //상하미러링
		{
			repos->y=Centerdevy-repos->y;
			repos->y=Centerdevy+repos->y;
		}
		else if(MirrorMode==3)  //좌우 상하미러링
		{
			repos->x=Centerdevx-repos->x;
			repos->x=Centerdevx+repos->x;

			repos->y=Centerdevy-repos->y;
			repos->y=Centerdevy+repos->y;
		}	
	}
}
 void GVGLMirrorStripApply(GVertex *StripS,GVertex *StripE,GVertex MasterS, GVertex MasterE,int MirrorMode)
{
	if(MirrorMode!=0)
	{
		float Centerdevx,Centerdevy;
		float P2Pdevx,P2Pdevy;

		Centerdevx=(MasterE.x-MasterS.x)/2;
		Centerdevy=(MasterE.y-MasterS.y)/2;

		P2Pdevx=StripE->x-StripS->x;
		P2Pdevy=StripE->y-StripS->y;

		if(MirrorMode==1)  //좌우미러링
		{
			StripS->x=Centerdevx-StripS->x;
			StripE->x=Centerdevx-StripE->x;
			StripS->x=Centerdevx+StripS->x;
			StripE->x=Centerdevx+StripE->x;

			StripS->x=StripS->x-P2Pdevx;
			StripE->x=StripE->x+P2Pdevx;
		}
		else if(MirrorMode==2)  //상하미러링
		{
			StripS->y=Centerdevy-StripS->y;
			StripE->y=Centerdevy-StripE->y;
			StripS->y=Centerdevy+StripS->y;
			StripE->y=Centerdevy+StripE->y;

			StripS->y=StripS->y-P2Pdevy;
			StripE->y=StripE->y+P2Pdevy;
		}
		else if(MirrorMode==3)  //좌우 상하미러링
		{
			StripS->x=Centerdevx-StripS->x;
			StripS->y=Centerdevy-StripS->y;
			StripE->x=Centerdevx-StripE->x;
			StripE->y=Centerdevy-StripE->y;
			StripS->x=Centerdevx+StripS->x;
			StripE->x=Centerdevx+StripE->x;
			StripS->y=Centerdevy+StripS->y;
			StripE->y=Centerdevy+StripE->y;

			StripS->x=StripS->x-P2Pdevx;
			StripE->x=StripE->x+P2Pdevx;
			StripS->y=StripS->y-P2Pdevy;
			StripE->y=StripE->y+P2Pdevy;
		}	
	}

}

 void GVertexCopy(GVertex *Data,GVertex DST)
{
	Data->x=DST.x;
	Data->y=DST.y;
	Data->z=DST.z;
}

 void GVertexFill(GVertex *Data,GLfloat num1, GLfloat num2, GLfloat num3)
{
	Data->x=num1;
	Data->y=num2;
	Data->z=num3;
}

 void GVGLColorFill(GVColor *Data,GLfloat R, GLfloat G, GLfloat B,GLfloat A)
{
	Data->R=R;
	Data->G=G;
	Data->B=B;
	Data->A=A;
}
 void GVGLColorFillInDex(GVColor *Data,int index)
{
	Data->R=GVCOLOR[index][0];
	Data->G=GVCOLOR[index][1];
	Data->B=GVCOLOR[index][2];
	Data->A=GVCOLOR[index][3];
}

 void GVGLDisableDepthforBlend()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

 void GVGLEnableDepth()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

 void GVGLMouseDistance(CDC *Hdc, HGLRC hrc,GVertex MasterS,GVertex MasterE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle)
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

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);
		
	Recombin2P(MasterS,MasterE);
	
	gluProject(MasterS.x, -MasterS.y, MasterS.z, model, proj, viewport, &MasterWindowPosS[0],&MasterWindowPosS[1],&MasterWindowPosS[2]);
	gluProject(MasterE.x,   -MasterE.y,   MasterE.z,   model, proj, viewport, &MasterWindowPosE[0],&MasterWindowPosE[1],&MasterWindowPosE[2]);	
 	MasterWindowPosS[1]=((float)H-(float)MasterWindowPosS[1]);
 	MasterWindowPosE[1]=((float)H-(float)MasterWindowPosE[1]);

	float worldxpos,worldypos;
	float windowxpos, windowypos;
	float incluseX,incluseY;

	worldxpos=MasterS.x-MasterE.x;
	worldypos=MasterS.y-MasterE.y;
	windowxpos=MasterWindowPosS[0]-MasterWindowPosE[0];
	windowypos=MasterWindowPosS[1]-MasterWindowPosE[1];

	if(Angle==0)
	{
		incluseX=worldxpos/windowxpos;
		incluseY=worldypos/windowypos;

		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;
	}
	else if(Angle==90 || Angle==-270)
	{	
		incluseX=worldypos/windowxpos;
		incluseY=worldxpos/windowypos;

		*mousey=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousex=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;

	}
	else if(Angle==180)
	{

		incluseX=worldxpos/windowxpos;
		incluseY=worldypos/windowypos;

		*mousex=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;
	}
	else if(Angle==270 || Angle==-90)
	{

		incluseX=worldypos/windowxpos;
		incluseY=worldxpos/windowypos;

		*mousey=(((float)point.x-(float)windowSx)-MasterWindowPosS[0])*incluseX;
		*mousex=(((float)point.y-(float)windowSy)-MasterWindowPosS[1])*incluseY;
	}

	glPopMatrix();
}



 void GVGLMouseDistanceCAM(CDC *Hdc, HGLRC hrc,GVertex MasterS,GVertex MasterE,CPoint point,GLfloat *mousex,GLfloat *mousey,GLfloat *cameraposmap,int W, int H,int windowSx,int windowSy,GLfloat Angle)
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

	glTranslatef(cameraposmap[0],cameraposmap[1],cameraposmap[2]);
		
	Recombin2P(MasterS,MasterE);
	
	gluProject(MasterS.x, MasterS.y, MasterS.z, model, proj, viewport, &MasterWindowPosS[0],&MasterWindowPosS[1],&MasterWindowPosS[2]);
	gluProject(MasterE.x,   MasterE.y,   MasterE.z,   model, proj, viewport, &MasterWindowPosE[0],&MasterWindowPosE[1],&MasterWindowPosE[2]);	


	float worldxpos,worldypos;
	float windowxpos, windowypos;
	float incluseX,incluseY;

	worldxpos=MasterS.x-MasterE.x;
	worldypos=MasterS.y-MasterE.y;
	windowxpos=MasterWindowPosS[0]-MasterWindowPosE[0];
	windowypos=MasterWindowPosS[1]-MasterWindowPosE[1];



	if(Angle==0)
	{
		incluseX=worldxpos/windowxpos;
		incluseY=worldypos/windowypos;


		*mousex=(float)(((float)point.x-windowSx)-MasterWindowPosS[0])*incluseX;
		*mousey=(float)((H-((float)point.y-windowSy))-MasterWindowPosS[1])*incluseY;

		*mousex+=MasterS.x;
		*mousey+=MasterS.y;
	}


	glPopMatrix();


}



 BOOL GVGLMouseClickObject(CDC *Hdc, HGLRC hrc,CPoint mousepoint,GLfloat *cameraposmap, GVertex start, GVertex End,int W, int H,int windowSx,int windowSy,GLfloat Angle) //objectClick
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

 void GVGLMoveCamera(GLfloat *cameraposmap,GVertex MasterS, GVertex MasterE,GLfloat XPosVal, GLfloat YPosVal, GLfloat ZPosVal,GLfloat Angle)
{
	float valx, valy;
	float xx=(MasterS.x-MasterE.x);
	float yy=(MasterS.y-MasterE.y);
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
 void GVGLRotateCamera(GLfloat *Angle)
{
	glRotatef(Angle[0],1.0f,0.0f,0.0f);
	glRotatef(Angle[1],0.0f,1.0f,0.0f);
	glRotatef(Angle[2],0.0f,0.0f,1.0f);
}
 void GVGLDragMouse(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex MasterS, GVertex MasterE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle)
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

		GVGLMoveCamera(cameraposmap,MasterS,MasterE,cameraposmap[0]-disX,cameraposmap[1]+disY,cameraposmap[2],Angle[2]);
		
		GVGLCameraInit(cameraposmap,MAPW,MAPH,Angle);
	}
}


 void GVGLDragMouseCAM(CDC *Hdc, HGLRC hrc,UINT nFlags,GVertex MasterS, GVertex MasterE, float mouseWoldx,float mouseWoldy,float *referencepoint,float *movingpoint,int MAPW,int MAPH, GLfloat *cameraposmap,GLfloat *Angle)
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

		GVGLMoveCamera(cameraposmap,MasterS,MasterE,cameraposmap[0]-disX,cameraposmap[1]-disY,cameraposmap[2],Angle[2]);

	//	GVGLCameraInit(cameraposmap,MAPW,MAPH,Angle);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////DRAW Function
 void GVGLDrawInit(int mode,int size,GVColor color)
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

 void GVGLDrawInitStencil(int mode,int size,GVColor color,BOOL stencil)
{
	if(mode==GV_POINT)//point
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glPointSize(size);
		glBegin(GL_POINTS);
	}
	else if(mode==GV_LINE || mode==GV_RECTE ||mode==GV_CROSS || mode==GV_CROSSX ) //line, RectE
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_LINES);
	}
	else if(mode==GV_RECTF)
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_QUADS);
	}
	else if(mode==GV_RECTFG)
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_QUADS);
	}
	else if(mode==GV_CIRCLEE)
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_LINE_LOOP);
	}
	else if(mode==GV_CIRCLEF)
	{
		glPushMatrix();

		if(stencil) glStencilFunc(GL_NOTEQUAL, 0x0, 0x0 );
		else glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		glColor4f(color.R,color.G,color.B,color.A);
		glLineWidth(size);
		glBegin(GL_POLYGON);
	}
}



 void GVGLDrawShow()
{
	glEnd();
	glPopMatrix();
}

 void GVGLDrawShowStencil()
{
	glEnd();
	glPopMatrix();

	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
}


 void GVGLDrawVertex(GVertex V1)
{
	glVertex3f(V1.x,-V1.y,V1.z);
}

 void GVGLDrawVertexCAM(GVertex V1)
{
	glVertex3f(V1.x,V1.y,V1.z);
}

 void GVGLDrawCross(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),-V1.y,V1.z);
	glVertex3f(V1.x+(Length/2),-V1.y,V1.z);

	glVertex3f(V1.x,-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x,-V1.y+(Length/2),V1.z);
}
 void GVGLDrawCrossX(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x+(Length/2),-V1.y+(Length/2),V1.z);

	glVertex3f(V1.x+(Length/2),-V1.y-(Length/2),V1.z);
	glVertex3f(V1.x-(Length/2),-V1.y+(Length/2),V1.z);
}

 void GVGLDrawCrossXCAM(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),V1.y-(Length/2),V1.z);
	glVertex3f(V1.x+(Length/2),V1.y+(Length/2),V1.z);

	glVertex3f(V1.x+(Length/2),V1.y-(Length/2),V1.z);
	glVertex3f(V1.x-(Length/2),V1.y+(Length/2),V1.z);
}

 void GVGLDrawRectE(GVertex V1,GVertex V2)
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

 void GVGLDrawRectF(GVertex V1,GVertex V2)
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

 void GVGLDrawRectECAM(GVertex V1,GVertex V2)
{
	glVertex3f(V1.x,V1.y,V1.z);
	glVertex3f(V2.x,V1.y,V1.z);

	glVertex3f(V2.x,V1.y,V1.z);
	glVertex3f(V2.x,V2.y,V2.z);

	glVertex3f(V2.x,V2.y,V2.z);
	glVertex3f(V1.x,V2.y,V1.z);

	glVertex3f(V1.x,V2.y,V1.z);
	glVertex3f(V1.x,V1.y,V1.z);
}

 void GVGLDrawRectFCAM(GVertex V1,GVertex V2)
{

	glVertex3f(V1.x,V1.y,V1.z);
	glVertex3f(V2.x,V1.y,V1.z);

	glVertex3f(V2.x,V1.y,V1.z);
	glVertex3f(V2.x,V2.y,V2.z);

	glVertex3f(V2.x,V2.y,V2.z);
	glVertex3f(V1.x,V2.y,V1.z);

	glVertex3f(V1.x,V2.y,V1.z);
	glVertex3f(V1.x,V1.y,V1.z);
}


 void GVGLDrawCircleE(GVertex V1,int radius)
{
	/*	double X,Y;
	int i=0; for(i=0; i < 20; i++)
	{
		GLfloat fAngle=2*PI*i/20;
		X=cos(fAngle) * radius;
		Y=sin(fAngle) * radius;
	    glVertex3f(V1.x-X , -V1.y-Y,V1.z);	
 	}
	*/  //너무 느림.
	
	int segments=40;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = V1.x + radius; 
	float y = V1.y; 
	
	int i=0; for(i=0; i<segments; i++)
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
 void GVGLDrawCircleF(GVertex V1,int radius)
{

	int segments=40;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = V1.x + radius; 
	float y = V1.y; 
	
	int i=0; for(i=0; i<segments+1; i++)
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

 void GVGLDrawElipseCAM(GVertex V1,GVertex V2)
{

	float X,Y;
	GLfloat Beta=-90*(PI/180);
	GLfloat sinbeta=sin(Beta);
	GLfloat cosbeta=cos(Beta);
	GLfloat a,b;
	GLfloat x,y;

	a=V1.x-V2.x;
	if(a<0)a=-a;

	b=V1.y-V2.y;
	if(b<0)b=-b;

	x=(V1.x+V2.x)/2;
	y=(V1.y+V2.y)/2;

	int i=0; for(i=0; i<360; i++)
	{
		GLfloat alpha = i*(PI/180);
		GLfloat sinalpha=sin(alpha);
		GLfloat cosalpha=cos(alpha);

		X=x+(a*cosalpha*cosbeta-b*sinalpha*sinbeta);
		Y=y+(a*cosalpha*sinbeta+b*sinalpha*cosbeta);

		glVertex3f(X, Y, V1.z);


	}
}

 void GVGLDrawCavePolygon(GLfloat *Pointx,GLfloat *Pointy,GLfloat *Pointz, int nval,int Includesize,GLfloat R,GLfloat G,GLfloat B,int nOp)
{	
	int i=0;
	if(R==0.0f && G==0.0f && B==0.0f)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_STENCIL_TEST);

		glColor4f(1,G,B,1.0f);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glStencilFunc(GL_NEVER,0,1);
		glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
		glBegin(GL_TRIANGLE_FAN);
		for( i=0; i<nval; i++)
		{
			glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
		}
		glEnd();
		
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc(GL_EQUAL,1,1);
		glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(R,G,B,1.0f);
		for(i=0; i<nval; i++)
		{
			glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
		}
		glEnd();

		glDisable(GL_STENCIL_TEST);
		
	}
	else
	{
		if(nOp)
		{
			glEnableClientState(GL_VERTEX_ARRAY);

			glEnable(GL_STENCIL_TEST);
	//		glEnable(GL_DEPTH_TEST);

			glColor4f(R,G,B,1.0f);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glStencilFunc(GL_NEVER,0,1);
			glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
			glBegin(GL_TRIANGLE_FAN);
			for( i=0; i<nval; i++)
			{
				glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
			}
			glEnd();
			
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glStencilFunc(GL_EQUAL,1,1);
			glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

			glBegin(GL_TRIANGLE_FAN);
			glColor4f(R,G,B,1.0f);
			for(i=0; i<nval; i++)
			{
				glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
			}
			glEnd();

			glDisable(GL_STENCIL_TEST);
		}
		else
		{
			glEnableClientState(GL_VERTEX_ARRAY);

			glEnable(GL_STENCIL_TEST);

			glColor4f(R,G,B,1.0f);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glStencilFunc(GL_NEVER,0,1);
			glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
			glBegin(GL_TRIANGLE_FAN);
			for( i=0; i<nval; i++)
			{
				glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
			}
			glEnd();
			
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glStencilFunc(GL_EQUAL,1,1);
			glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

			glBegin(GL_TRIANGLE_FAN);
			glColor4f(R,G,B,1.0f);
			for(i=0; i<nval; i++)
			{
				glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
			}
			glEnd();

			glDisable(GL_STENCIL_TEST);
		}
	}
	
}

 void GVGLDrawCavePolygonmodeLine(GLfloat *Pointx,GLfloat *Pointy,GLfloat *Pointz, int nval,int Includesize,GLfloat R,GLfloat G,GLfloat B)
{
	int i=0;
	if(R==0.0f && G==0.0f && B==0.0f)
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(R,G,B,1.0f);
		for( i=0; i<nval; i++)
		{
			glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
		}
		glEnd();
			
	}
	else	
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		glEnable(GL_STENCIL_TEST);

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

		glColor4f(R,G,B,1.0f);

		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glStencilFunc(GL_NEVER,0,1);
		glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
		glBegin(GL_TRIANGLE_FAN);
		for( i=0; i<nval; i++)
		{
			glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
		}
		glEnd();

		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc(GL_EQUAL,1,1);
		glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

		glBegin(GL_TRIANGLE_FAN);
		for(i=0; i<nval; i++)
		{
			glVertex3f(Pointx[i]*Includesize,Pointy[i]*Includesize,Pointz[i]);
		}
		glEnd();

		glDisable(GL_STENCIL_TEST);

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}

}

 void GVGLDrawCavePolygonmodePOINT(POINT *Point, int nval,int Includesize,GLfloat R,GLfloat G,GLfloat B)
{
	int i=0;
	if(R==0.0f && G==0.0f && B==0.0f)
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(R,G,B,1.0f);
		for( i=0; i<nval; i++)
		{
			glVertex3f(Point[i].x*Includesize,Point[i].y*Includesize,0.0f);
		}
		glEnd();

	}
	else
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		glEnable(GL_STENCIL_TEST);
//		glEnable(GL_DEPTH_TEST);

		glColor4f(R,0,0,1.0f);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		glStencilFunc(GL_NEVER,0,1);
		glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
		glBegin(GL_TRIANGLE_FAN);
		for( i=0; i<nval; i++)
		{
			glVertex3f(Point[i].x*Includesize,Point[i].y*Includesize,0.0f);
		}
		glEnd();
		

		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		glStencilFunc(GL_EQUAL,1,1);
		glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

		glBegin(GL_TRIANGLE_FAN);
		glColor4f(R,G,B,1.0f);
		for(i=0; i<nval; i++)
		{
			glVertex3f(Point[i].x*Includesize,Point[i].y*Includesize,0.0f);
		}
		glEnd();

		glDisable(GL_STENCIL_TEST);
	}

}

 void GVGLDrawCavePolygonmodeLinePOINT(POINT *Point, int nval,int Includesize,GLfloat R,GLfloat G,GLfloat B,GLfloat width)
{
	if(nval==2)
	{
	/*	if(R==0.0f && G==0.0f && B==0.0f)
		{
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(R,G,B,1.0f);
			int i=0; for(i=0; i<nval; i++)
			{
				glVertex3f(Point[i].x*Includesize,Point[i].y*Includesize,0.0f);
			}
			glEnd();
		}
		else
		{
			int bLinemode;
			GLfloat LineWidth=width/2;
			GLfloat fSx[4],fSy[4];
			GLfloat X1,X2;
			GLfloat Y1,Y2;

			X1=Point[0].x;
			X2=Point[1].x;
			Y1=Point[0].y;
			Y2=Point[1].y;
			
			glLineWidth(1);
			///jun
			if (Point[0].y == Point[1].y) //'수평선
			{
				bLinemode=0;
			}
			else if (Point[0].x == Point[1].x)	// '수직선
			{
				bLinemode=1;
			}
			else    //'사선
			{
				bLinemode=3;
			}

			glColor3f(R,G,B);
			glBegin(GL_POLYGON);
			if(bLinemode==0)
			{
				glVertex3f(X1*Includesize, (Y1-LineWidth)*Includesize,0.0);
				glVertex3f(X2*Includesize, (Y2-LineWidth)*Includesize,0.0);
				glVertex3f(X2*Includesize, (Y2+LineWidth)*Includesize,0.0);
				glVertex3f(X1*Includesize, (Y1+LineWidth)*Includesize,0.0);
			}
			else if(bLinemode==1)	
			{
				glVertex3f((X1-LineWidth)*Includesize, Y1*Includesize,0.0);
				glVertex3f((X1+LineWidth)*Includesize, Y1*Includesize,0.0);
				glVertex3f((X2+LineWidth)*Includesize, Y2*Includesize,0.0);
				glVertex3f((X2-LineWidth)*Includesize, Y2*Includesize,0.0);
			}
			else
			{
				double angle = atan2(Y2 - Y1, X2 - X1);
				double t2sina1 = width / 2 * sin(angle);
				double t2cosa1 = width / 2 * cos(angle);

				fSx[0]=(X2 + t2sina1)*Includesize;
				fSx[1]=(X2 - t2sina1)*Includesize;
				fSx[2]=(X1 - t2sina1)*Includesize;
				fSx[3]=(X1 + t2sina1)*Includesize;
				fSy[0]=(Y2 - t2cosa1)*Includesize;
				fSy[1]=(Y2 + t2cosa1)*Includesize;
				fSy[2]=(Y1 + t2cosa1)*Includesize;
				fSy[3]=(Y1 - t2cosa1)*Includesize;

				glVertex3f(fSx[0] ,fSy[0] ,0.0f);
				glVertex3f(fSx[1] ,fSy[1] ,0.0f);
				glVertex3f(fSx[2] ,fSy[2] ,0.0f);
				glVertex3f(fSx[3] ,fSy[3] ,0.0f);
			}
			glEnd();
		}
	*/
	}
	else
	{
		int i=0;
		if(R==0.0f && G==0.0f && B==0.0f)
		{
			glBegin(GL_TRIANGLE_FAN);
			glColor4f(1.0,0.0,0.0,1.0f);
		//	glColor4f(R,G,B,1.0f);
			for( i=0; i<nval; i++)
			{
				glVertex3f(Point[i].x*Includesize,Point[i].y*Includesize,0.0f);
			}
			glEnd();
		}
		else
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnable(GL_STENCIL_TEST);
			glColor4f(1,0,0,1.0f);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glStencilFunc(GL_NEVER,0,1);
			glStencilOp(GL_INVERT,GL_INVERT,GL_INVERT);
			glBegin(GL_TRIANGLE_FAN);
			for( i=0; i<nval; i++)
			{
				glVertex3f((GLfloat)Point[i].x*Includesize,(GLfloat)Point[i].y*Includesize,0.0f);
			}
			glEnd();

			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glStencilFunc(GL_EQUAL,1,1);
			glStencilOp(GL_ZERO,GL_ZERO,GL_ZERO);

			glBegin(GL_TRIANGLE_FAN);
			for(i=0; i<nval; i++)
			{
				glVertex3f((GLfloat)Point[i].x*Includesize,(GLfloat)Point[i].y*Includesize,0.0f);
			}
			glEnd();

			glDisable(GL_STENCIL_TEST);
		}
	}

}



 void GVGLDrawCrossCAM(GVertex V1,int Length)
{
	glVertex3f(V1.x-(Length/2),V1.y,V1.z);
	glVertex3f(V1.x+(Length/2),V1.y,V1.z);

	glVertex3f(V1.x,V1.y-(Length/2),V1.z);
	glVertex3f(V1.x,V1.y+(Length/2),V1.z);

}

 void GVGLDrawCircleECAM(GLfloat Ox,GLfloat Oy,GLfloat Oz,GLfloat radius,int nSeg)
{
	/*	double X,Y;
	int i=0; for(i=0; i < 20; i++)
	{
		GLfloat fAngle=2*PI*i/20;
		X=cos(fAngle) * radius;
		Y=sin(fAngle) * radius;
	    glVertex3f(V1.x-X , -V1.y-Y,V1.z);	
 	}
	*/  //너무 느림.
	
	int segments=nSeg;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = Ox + radius; 
	float y = Oy; 
	float tx,ty,rx,ry;
	
	int i=0; for(i=0; i<segments; i++)
	{
		glVertex3f(x, y, Oz);
		tx = -(y - Oy); 
		ty = x - Ox; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		rx = Ox- x; 
		ry = Oy- y; 
		x += rx * radial_factor; 
		y += ry * radial_factor; 
	}
}
 void GVGLDrawCircleFCAM(GLfloat Ox,GLfloat Oy,GLfloat Oz,GLfloat radius,int nSeg)
{

	int segments=nSeg;
	float theta = 2 * PI / float(segments); 
	float tangetial_factor = tanf(theta); 
	float radial_factor = 1 - cosf(theta); 
	float x = Ox + radius; 
	float y = Oy; 
	float tx,ty,rx,ry;
	
	int i=0; for(i=0; i<segments+1; i++)
	{
		glVertex3f(Ox,Oy,Oz);
		glVertex3f(x, y, Oz);
		tx = -(y - Oy); 
		ty = x - Ox; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		rx = Ox- x; 
		ry = Oy- y; 
		x += rx * radial_factor; 
		y += ry * radial_factor; 
		glVertex3f(x, y, Oz);		
	}
}


 void GVGLDrawRectFG(GVertex V1,GVertex V2,int Level,int color)
{
	float radian;
	float red;
	float green;
	float blue;
	float glR,glG,glB;

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
}


  void GVGLGotoCentermodel(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2)
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

  void GVGLGotoCentermodelCAM(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2)
 {
	cameraposmap[0]=0;
	cameraposmap[1]=0;
	 
	GLfloat Dragcenter[2];

//	if(Dragpoint1.x>Dragpoint2)
//	{
//		
//	}

//	Recombin2P(Dragpoint1,Dragpoint2);
	Dragcenter[0]=(Dragpoint2.x+Dragpoint1.x)/2;
	Dragcenter[1]=(Dragpoint2.y+Dragpoint1.y)/2;

 	cameraposmap[0]=-Dragcenter[0];
 	cameraposmap[1]=-Dragcenter[1];
 }

 
  void GVGLSetFit(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger)
 {

	 GLfloat modelpoint[4];
	 double MousePosL[3];
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


  void GVGLSetFitCAM(GLfloat *cameraposmap,GVertex Dragpoint1, GVertex Dragpoint2,GLfloat Extra,int W, int H,BOOL Bigger)
 {

	 GLfloat modelpoint[4];
	 double MousePosL[3];
	 GLdouble model[16];	
	 GLdouble proj[16];
	 GLint viewport[4];

	 cameraposmap[2]=-400000.0f;
	 
	 Recombin2P(Dragpoint1,Dragpoint2);

	 modelpoint[0]=Dragpoint1.x;  //minx
	 modelpoint[1]=Dragpoint2.x;   //maxx
	 modelpoint[2]=Dragpoint1.y;   //miny
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

		gluProject(modelpoint[1], modelpoint[3], -Dragpoint1.z, model, proj, viewport, &MousePosL[0],&MousePosL[1],&MousePosL[2]);

		if(Bigger==1)  //점점 커져서 fit한다.
		{
			if(W+Extra<=MousePosL[0] || H-Extra<=MousePosL[1]  )
			{	 
				break;
			}
			else
			{
				cameraposmap[2]+=10.0f;
				if(cameraposmap[2]>-180)
				{
					cameraposmap[2]=-180; 
					break;
				}
			}
		}
		else if(Bigger==2)  //점점 커져서 fit한다.
		{
			if(W+Extra<=MousePosL[0] || H-Extra<=MousePosL[1]  )
			{	 
				break;
			}
			else
			{
				cameraposmap[2]+=100.0f;
				if(cameraposmap[2]>0)
				{
					cameraposmap[2]=-30;
					break;
				}
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
		if(loopcnt>600000)
		{
	//		MessageBoxA(0,"Please cheack your Gvertex!!!","Caution",MB_OK);
			break;
		}
	}
	 
 }




  void Recombin2P(GVertex &LowPoint, GVertex &HighPoint)
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

void GVGLFont(CString str,int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R,GLfloat G,GLfloat B)
{
	//int size = str.GetLength();
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glListBase(m_nlist_base);
	//glCallLists(size, GL_UNSIGNED_BYTE, str);
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);

	  va_list args;
	  char buffer[512];
	  int cnt=0;

//	  sprintf(buffer,"%s",str);
	  StringToChar(str,buffer);

 	
	  glPushMatrix();
	  glEnable(GL_SMOOTH);
	  glTranslatef(Posx, -Posy, Posz);
	  glLineWidth(Linewidth);

	  glColor3f(R,G,B);
	  glScalef(0.05f*size, 0.05f*size, 0.05f);
  
	  int p = NULL;
	  //char p = 'A';
	  //glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, p);

	  if(mode==GVFONT_ROMAN)
	  {
		  int i=0;
		  int nSize=strlen(buffer);
		  for(i=0; i<nSize; i++)		 // for (p = &buffer[0]; *p; p++)
		  {
			  p = buffer[i];
			  cnt++;
			  if (cnt >= 200)
				  break;
			  glutStrokeCharacter(GLUT_STROKE_ROMAN, p);
		  }
	  }
	  else
	  {
		  int i=0;
		  int nSize=strlen(buffer);
		  for(i=0; i<nSize; i++)			  // for (p = &buffer[0]; *p; p++)
		  {
			  p=buffer[i];
			  cnt++;
			  if(cnt>=200)
				  break;
			  glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, p);
			  //glutStrokeCharacter(GLUT_STROKE_ROMAN, p);
		  }		
	  }
 

	  glDisable(GL_SMOOTH);
	  glPopMatrix();

 }


 void GVGLFontCAM(CString str,int Linewidth,GLfloat Posx,GLfloat Posy,GLfloat Posz, GLfloat size,int mode,GLfloat R,GLfloat G,GLfloat B)
{
	  va_list args;
	  char buffer[512], *p;
	  StringToChar(str,buffer);		

	  glPushMatrix();
	  glDisable(GL_BLEND);
	  glDisable(GL_DEPTH_TEST);
	  glEnable(GL_SMOOTH);
	  glTranslatef(Posx, Posy, Posz);
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
			//glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	  }
 

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

 void GVGLRomCurve2D(int nPSize,GVertex *Point,int tval,GVColor Color,int LineSize)
{
	glPushMatrix();
	glColor4f(Color.R,Color.G,Color.B,Color.A);
	glLineWidth(LineSize);
	glBegin(GL_LINE_STRIP);
	GVertex p[4];

	int i=0; for(i = 0; i < (nPSize-1); i++) 
	{ 
        int it=0; for(it = 1; it <= tval; it++)
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

 void GVGLRomCurve3D(int nPSizex,int nPSizey,GVertex **Point,int tval,GVColor Color,int LineSize)
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
			int it=0; for(it = 1; it <= tval; it++)
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
		for(j = 0; j < (nPSizex-1); j++) 
		{ 
			int it=0; for(it = 1; it <= tval; it++)
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

 void GVGLRomCurve3DGrad(int nPSizex,int nPSizey,GVertex **Point, int tval,GVColor **Color,int LineSize)
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
			int it=0; for(it = 1; it <= tval; it++)
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
		for(j = 0; j < (nPSizex-1); j++) 
		{ 
			int it=0; for(it = 1; it <= tval; it++)
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

 void GVGLHistoPoint(GLfloat *cameraposmap,int W, int H,GLfloat *X,GLfloat *y,int cnt,int Psize,GLfloat TSize,GVColor TColor,GVColor HColor)
{
	GVertex a,b,c;
	GLfloat xval,yval;
	GVertex DumPos[4];
	CString numbering;
	GVertex P;
	GLfloat fMaxX=0.0f;
	GLfloat fMaxY=0.0f;
	GLfloat Rxval,Ryval;
	CString txt;
	
	xval=((GLfloat)(W*TSize));
	yval=(GLfloat)H;

	Rxval=xval/cnt;
	Ryval=yval/cnt;
	
	GVertexFill(&a,0.0f,0.0f,0.0f);
	GVertexFill(&b,xval,-yval,0.0f);
	GVGLGotoCentermodel(cameraposmap,a,b);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap,a,b,25,W,H,1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.


	GVertexFill(&DumPos[0],a.x,a.y,0.0f);
	GVertexFill(&DumPos[1],b.x,a.y,0.0f);
	GVertexFill(&DumPos[2],a.x,b.y,0.0f);
	GVGLDrawInit(GV_LINE,2,HColor);
	GVGLDrawVertex(DumPos[0]);
	GVGLDrawVertex(DumPos[1]);
	GVGLDrawVertex(DumPos[0]);
	GVGLDrawVertex(DumPos[2]);
	GVGLDrawShow();


	GVertexFill(&b,xval,0.0f,0.0f);
	GVertexFill(&c,0.0f,-yval,0.0f);

	int i=0;
	for( i=0; i<cnt; i++)
	{	
		if(fMaxX<X[i]) fMaxX=X[i];
		if(fMaxY<y[i]) fMaxY=y[i];
	}

	Rxval=xval/fMaxX;
	Ryval=yval/fMaxY;

	for(i=0; i<cnt; i++)
	{
		P.x=X[i]*Rxval;
		P.y=y[i]*-Ryval;
		P.z=0.0f;
		
		if(fMaxX<X[i]) fMaxX=X[i];
		if(fMaxY<y[i]) fMaxY=y[i];
		
		GVGLDrawInit(GV_POINT,Psize,TColor);
		GVGLDrawVertex(P);
		GVGLDrawShow();
	}

	GVGLFont(_T("0"),1,-30,10.0f,0.0f,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
	txt.Format(_T("%.1f"),(GLfloat)fMaxX);
	GVGLFont(txt,1,b.x,b.y+10,b.z,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
	txt.Format(_T("%.1f"),(GLfloat)fMaxY);
	GVGLFont(txt,1,c.x-30,c.y,c.z,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
}

 void GVGLHistoPointTaget(GLfloat *cameraposmap,int W, int H,GVertex C,GLfloat *X,GLfloat *y,int cnt,GLfloat includ)
{
	GVertex a,b;
	GLfloat xval,yval;
	CString numbering;
	GVertex P;
	GLfloat fMaxX=0.0f;
	GLfloat fMaxY=0.0f;
	GLfloat Rxval,Ryval;
	CString txt;
	GVColor Rcolor;
	GVColor Gcolor;
	GVColor Bcolor;
	GVColor Wcolor;
	
	xval=(GLfloat)W;
	yval=(GLfloat)H;
	

	GVGLColorFill(&Rcolor,1.0f,0.0f,0.0f,1.0f);
	GVGLColorFill(&Gcolor,0.0f,1.0f,0.0f,1.0f);
	GVGLColorFill(&Bcolor,0.0f,0.0f,0.6f,1.0f);
	GVGLColorFill(&Wcolor,0.8f,0.8f,0.8f,1.0f);
	
	GVertexFill(&a,C.x-xval/2,C.y-yval/2,0.0f);
	GVertexFill(&b,C.x+xval/2,(C.y+yval/2),0.0f);
	GVGLGotoCentermodel(cameraposmap,a,b);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap,a,b,100,W,H,1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.

	txt.Format(_T("Dim = %d"),(int)includ);
	GVGLFont(txt,2,a.x-200,-a.y+190,0.0f,6,GVFONT_ROMAN,1.0f,0.0f,0.0f);
	int i=0;
	for( i=0; i<cnt; i++)
	{
		P.x=X[i]-C.x;
		P.y=-(y[i]-C.y);
		P.z=0.0f;

		if(P.x<0)P.x=-P.x;
		if(P.y<0)P.y=-P.y;
		if(fMaxX<P.x) fMaxX=P.x;
		if(fMaxY<P.y) fMaxY=P.y;
	}

	Rxval=xval/fMaxX;
	Ryval=yval/fMaxY;


	for(i=0; i<cnt; i++)
	{
		P.x=(X[i]-C.x)*Rxval;
		P.y=-((y[i]-C.y)*Ryval);
		P.z=0.0f;

		GVGLDrawInit(GV_POINT,4,Gcolor);
		GVGLDrawVertex(P);
		GVGLDrawShow();
	}

	GVGLDrawInit(GV_CIRCLEF,1,Rcolor);
	GVGLDrawCircleF(C,3);
	GVGLDrawShow();

	GVGLDrawInit(GV_CROSS,1,Rcolor);
	GVGLDrawCross(C,W*2);
	GVGLDrawShow();


	BOOL onoff=0;
	GLfloat Line=includ;
	for(;;)
	{
		if(onoff)
		{
			GVGLDrawInit(GV_CIRCLEF,1,Bcolor);
			GVGLDrawCircleF(C,Line*Rxval);
			GVGLDrawShow();
			onoff=0;
		}
		else
		{
			GVGLDrawInit(GV_CIRCLEF,1,Wcolor);
			GVGLDrawCircleF(C,Line*Rxval);
			GVGLDrawShow();
			onoff=1;
		}
		
		Line+=includ;
		if(Line*Rxval>fMaxX*Rxval)
		{
			if(onoff)
			{
				GVGLDrawInit(GV_CIRCLEF,1,Bcolor);
				GVGLDrawCircleF(C,Line*Rxval);
				GVGLDrawShow();
				onoff=0;
			}
			else
			{
				GVGLDrawInit(GV_CIRCLEF,1,Wcolor);
				GVGLDrawCircleF(C,Line*Rxval);
				GVGLDrawShow();
				onoff=1;
			}
			break;
		}
	}
		
}

 void GVGLHistoinit(GLfloat *cameraposmap,int W, int H,GLfloat TSize,GVColor TColor,int Line)
{
	GVertex a,b,c;
	GLfloat xval,yval;
	GVColor color[2];
	CString numbering;

		 
	xval=((GLfloat)(W*TSize))+10;
	yval=(GLfloat)H;
	

	GVGLColorFill(&color[0],TColor.R,TColor.G,TColor.B,TColor.A);
	GVGLColorFill(&color[1],0.3f,0.3f,0.3f,1.0f);
	GVertexFill(&a,0.0f,0.0f,0.0f);
	GVertexFill(&b,xval+100,-yval-40,0.0f);
	GVGLGotoCentermodel(cameraposmap,a,b);//카메라를 사각형의 중심으로 이동
	GVGLSetFit(cameraposmap,a,b,10,W,H,1);  //카메라를 모델쪽으로 밀어 화면에 꽉 채운다.

	GVertexFill(&b,xval+20,0.0f,0.0f);
	GVertexFill(&c,0.0f,-yval-20,0.0f);

//	if(Line)
//	{
//		GVGLFont("0",1,-30,10.0f,0.0f,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);
//		numbering.Format(_T("%.1f"),(GLfloat)x);
//		GVGLFont(numbering,1,b.x,b.y+10,b.z,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);
//	}
//	numbering.Format(_T("%.1f"),(GLfloat)(y));
//	GVGLFont(numbering,1,c.x-30,c.y,c.z,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);

	GVGLDrawInit(GV_LINE,2,color[0]);
	GVGLDrawVertex(a);
	GVGLDrawVertex(b);
	GVGLDrawVertex(a);
	GVGLDrawVertex(c);
	GVGLDrawShow();

}
 void GVGLDrawHisto1(int W, int H,GLfloat *HistoDataX1, GLfloat *HistoDataY1,GVColor LColor,GLfloat TSize,int nCnt,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize)+10;
	yval=(GLfloat)H;
	CString txt;
	int nMAXx=0;
	int nMAXy=0;

	int i=0; for(i=0; i<nCnt; i++)
	{
		if(nMAXx<HistoDataX1[i]) nMAXx=HistoDataX1[i];
		if(nMAXy<HistoDataY1[i]) nMAXy=HistoDataY1[i];
	}	

	Rxval=xval/nMAXx;
	Ryval=yval/nMAXy;

	if(Line)
	{
		GVGLDrawInit(GV_LINE,1,LColor);
		for(i=0; i<nCnt-1; i++)
		{
			GVertexFill(&a,(GLfloat)(HistoDataX1[i]*Rxval),-HistoDataY1[i]*Ryval,0.0f);
			GVertexFill(&b,(GLfloat)((HistoDataX1[i+1])*Rxval),-HistoDataY1[i+1]*Ryval,0.0f);

			GVGLDrawVertex(a);
			GVGLDrawVertex(b);
		}
		GVGLDrawShow();
	}
	/*
	else
	{
		int val=0;
		
		int haf1=x/3;
		int haf2=x/3 + x/3;

		int i=0; for(i=0; i<x; i++)
		{
			GVGLDrawInit(GV_RECTF,1,LColor);
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+10)*Rxval),-(HistoData1[i]*Ryval),0.0f);
			GVGLDrawRectF(a,b);
			GVGLDrawShow();

			if(i==haf1 || i==haf2 || i==x-1 || i==0)
			{
				txt.Format(_T("%.1f"),HistoDataX1[i]);
				GVGLFont(txt,1,a.x+3,a.y+20,0.0f,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
			}
			val+=20;
		}
	}

	GLfloat ywidth;
	GLfloat yval2;
	GVColor color;
	GVGLColorFill(&color,0.3f,0.3f,0.3f,1.0f);
	ywidth=(GLfloat)(yval/5);
	yval2=ywidth;
	int i=0; for(i=0; i<5; i++)
	{
		GVertexFill(&a,0,-yval2,0.0f);
		GVertexFill(&b,xval,-yval2,0.0f);
		GVGLDrawInit(GV_LINE,1,color);
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
		GVGLDrawShow();
		yval2+=ywidth;
	}
*/
}
/*
 void GVGLDrawHisto2(int W, int H,GLfloat *HistoDataX1, GLfloat *HistoDataY1,GLfloat *HistoDataX2, GLfloat *HistoDataY2,GVColor* LColor,GLfloat TSize,int nCnt,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize)+10;
	yval=(GLfloat)H;
	CString txt;
	int nMAXx=0;
	int nMAXy=0;
	CString numbering;

	int i=0; for(i=0; i<nCnt; i++)
	{
		if(nMAXx<HistoDataX1[i]) nMAXx=HistoDataX1[i];
		if(nMAXy<HistoDataY1[i]) nMAXy=HistoDataY1[i];
	}
	for(i=0; i<nCnt; i++)
	{
		if(nMAXx<HistoDataX2[i]) nMAXx=HistoDataX2[i];
		if(nMAXy<HistoDataY2[i]) nMAXy=HistoDataY2[i];
	}
	numbering.Format(_T("%.1f"),(GLfloat)(y));
	GVGLFont(numbering,1,c.x-30,c.y,c.z,2,GVFONT_ROMAN,TColor.R,TColor.G,TColor.B);

	GVGLDrawVertex(a);
	GVGLDrawVertex(b);
		}
		GVGLDrawShow();

		GVGLDrawInit(GV_LINE,1,LColor[1]);
		for(i=0; i<nCnt-1; i++)
		{
			GVertexFill(&a,(GLfloat)(HistoDataX2[i]*Rxval),-HistoDataY2[i]*Ryval,0.0f);
			GVertexFill(&b,(GLfloat)((HistoDataX2[i+1])*Rxval),-HistoDataY2[i+1]*Ryval,0.0f);

	GVGLDrawVertex(a);
			GVGLDrawVertex(b);
		}
		GVGLDrawShow();

	}

}
 void GVGLDrawHisto3(int nDlgWidth, int nDlgHeight,GLfloat *HistoDataX1, GLfloat *HistoDataY1,GLfloat *HistoDataX2, GLfloat *HistoDataY2,GLfloat *HistoDataX3, GLfloat *HistoDataY3,GVColor* LColor,GLfloat TSize,int nCnt,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(nDlgWidth*TSize)+10;
	yval=(GLfloat)nDlgHeight;
	CString txt;
	GLfloat fMaxWidth, fMaxHeight;
	GLfloat fMaxX, fMinX;
	GLfloat fMaxY, fMinY;
	int nBaseLineMaxNum=0;
	int nLeftLineMaxNum=0;
	int nRightLineMaxNum=0;
	int nDumMAX[3]={-INT_MAX,};
	GVColor colorGray;
	GLfloat fFontSize=2.5;
	double nom = 0.0f, dnom = 0.0f;
	GLdouble glCVal = 0.0f;
	GLdouble glLVal = 0.0f;
	GLdouble glRVal = 0.0f;
	double dBar1 = 0.0f, dBar2 = 0.0f;

	GVGLColorFill(&colorGray,0.5,0.5,0.5,1.0);

	fMaxX = -(INT_MAX);
	fMinX = INT_MAX;
	fMaxY = -(INT_MAX);
	fMinY = INT_MAX;

	int i=0; for(i=0; i<nCnt; i++)
	{
		if(fMaxX<HistoDataX1[i])			fMaxX=HistoDataX1[i];
		if(fMinX>HistoDataX1[i])			fMinX=HistoDataX1[i];
		if(fMaxY<HistoDataY1[i])			fMaxY=HistoDataY1[i];
		if(fMinY>HistoDataY1[i])			fMinY=HistoDataY1[i];

		if(fMaxX<HistoDataX2[i])			fMaxX=HistoDataX2[i];
		if(fMinX>HistoDataX2[i])			fMinX=HistoDataX2[i];
		if(fMaxY<HistoDataY2[i])			fMaxY=HistoDataY2[i];
		if(fMinY>HistoDataY2[i])			fMinY=HistoDataY2[i];

		if(fMaxX<HistoDataX3[i])			fMaxX=HistoDataX3[i];
		if(fMinX>HistoDataX3[i])			fMinX=HistoDataX3[i];
		if(fMaxY<HistoDataY3[i])			fMaxY=HistoDataY3[i];
		if(fMinY>HistoDataY3[i])			fMinY=HistoDataY3[i];
	}

	for(i=0; i<nCnt; i++)
	{
		if(nDumMAX[0]<HistoDataY1[i])
		{
			nDumMAX[0]=HistoDataY1[i];
			nBaseLineMaxNum=i;
		}
		if(nDumMAX[1]<HistoDataY2[i])
		{
			nDumMAX[1]=HistoDataY2[i];
			nLeftLineMaxNum=i;
		}
		if(nDumMAX[2]<HistoDataY3[i])
		{
			nDumMAX[2]=HistoDataY3[i];
			nRightLineMaxNum=i;
		}
	}

	////// Gaussian 
	if(nCnt>3)
	{
		if(nBaseLineMaxNum-1>=0 && nBaseLineMaxNum+1<nCnt)
		{
			GLdouble intaval= HistoDataX1[nBaseLineMaxNum]-HistoDataX1[nBaseLineMaxNum-1];

			nom = (log(HistoDataY1[nBaseLineMaxNum])-log(HistoDataY1[nBaseLineMaxNum+1])) * (HistoDataX1[nBaseLineMaxNum]*HistoDataX1[nBaseLineMaxNum] - HistoDataX1[nBaseLineMaxNum-1]*HistoDataX1[nBaseLineMaxNum-1]);
			dnom = 2 * intaval * ((log(HistoDataY1[nBaseLineMaxNum])-log(HistoDataY1[nBaseLineMaxNum-1])) + (log(HistoDataY1[nBaseLineMaxNum])-log(HistoDataY1[nBaseLineMaxNum+1])));

			dBar1 = nom/dnom;

			nom = (log(HistoDataY1[nBaseLineMaxNum])-log(HistoDataY1[nBaseLineMaxNum-1])) * (HistoDataX1[nBaseLineMaxNum]*HistoDataX1[nBaseLineMaxNum] - HistoDataX1[nBaseLineMaxNum+1]*HistoDataX1[nBaseLineMaxNum+1]);
			
			dBar2 = nom/dnom;
			glCVal = dBar1 - dBar2;
		}

		if(nLeftLineMaxNum-1>=0 && nLeftLineMaxNum+1<nCnt)
		{

			GLdouble intaval= HistoDataX2[nLeftLineMaxNum]-HistoDataX2[nLeftLineMaxNum-1];

			nom = (log(HistoDataY2[nLeftLineMaxNum])-log(HistoDataY2[nLeftLineMaxNum+1])) * (HistoDataX2[nLeftLineMaxNum]*HistoDataX2[nLeftLineMaxNum] - HistoDataX2[nLeftLineMaxNum-1]*HistoDataX2[nLeftLineMaxNum-1]);
			dnom = 2 * intaval * ((log(HistoDataY2[nLeftLineMaxNum])-log(HistoDataY2[nLeftLineMaxNum-1])) + (log(HistoDataY2[nLeftLineMaxNum])-log(HistoDataY2[nLeftLineMaxNum+1])));

			dBar1 = nom/dnom;

			nom = (log(HistoDataY2[nLeftLineMaxNum])-log(HistoDataY2[nLeftLineMaxNum-1])) * (HistoDataX2[nLeftLineMaxNum]*HistoDataX2[nLeftLineMaxNum] - HistoDataX2[nLeftLineMaxNum+1]*HistoDataX2[nLeftLineMaxNum+1]);
			
			dBar2 = nom/dnom;
			glLVal = dBar1 - dBar2;
		}

		if(nRightLineMaxNum-1>=0 && nRightLineMaxNum+1<nCnt)
		{

			GLdouble intaval= HistoDataX3[nRightLineMaxNum]-HistoDataX3[nRightLineMaxNum-1];

			nom = (log(HistoDataY3[nRightLineMaxNum])-log(HistoDataY3[nRightLineMaxNum+1])) * (HistoDataX3[nRightLineMaxNum]*HistoDataX3[nRightLineMaxNum] - HistoDataX3[nRightLineMaxNum-1]*HistoDataX3[nRightLineMaxNum-1]);
			dnom = 2 * intaval * ((log(HistoDataY3[nRightLineMaxNum])-log(HistoDataY3[nRightLineMaxNum-1])) + (log(HistoDataY3[nRightLineMaxNum])-log(HistoDataY3[nRightLineMaxNum+1])));

			dBar1 = nom/dnom;

			nom = (log(HistoDataY3[nRightLineMaxNum])-log(HistoDataY3[nRightLineMaxNum-1])) * (HistoDataX3[nRightLineMaxNum]*HistoDataX3[nRightLineMaxNum] - HistoDataX3[nRightLineMaxNum+1]*HistoDataX3[nRightLineMaxNum+1]);
			
			dBar2 = nom/dnom;
			glRVal = dBar1 - dBar2;
		}
	}


//	fMaxX+=100;
//	fMaxY+=100;

	fMaxWidth = fMaxX - fMinX;
	if(fMaxWidth < 0)
		fMaxWidth = -fMaxWidth;

	fMaxHeight = fMaxY - fMinY;
	if(fMaxHeight < 0)
		fMaxHeight = -fMaxHeight;

	Rxval=xval/fMaxWidth;
	Ryval=yval/fMaxHeight;

	if(Line)
	{
		GVGLDrawInit(GV_LINE,1,LColor[0]);
		int i=0; for(i=0; i<nCnt-1; i++)
		{
			GVertexFill(&a,(GLfloat)((HistoDataX1[i]-fMinX)*Rxval),-(HistoDataY1[i]-fMinY)*Ryval,0.0f);
			GVertexFill(&b,(GLfloat)((HistoDataX1[i+1]-fMinX)*Rxval),-(HistoDataY1[i+1]-fMinY)*Ryval,0.0f);
			GVGLDrawVertex(a);
			GVGLDrawVertex(b);
		}
	GVGLDrawShow();

}

	}

}

*/



/*
 void GVGLDrawHisto1(GLfloat *HistoDataX1,GLfloat *HistoData1,GVColor HColor,GVColor LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;
	CString txt;

	if(Line)
	{
		Rxval=xval/x;
		Ryval=yval/y;
	}
	else
	{
		Rxval=xval/(x*20);
		Ryval=yval/y;
	}

	if(Line)
	{
		GVGLDrawInit(GV_LINE,1,LColor);
		int i=0; for(i=0; i<x-1; i++)
		{
			GVertexFill(&a,(GLfloat)(i*Rxval),-(HistoData1[i]*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((i+1)*Rxval),-(HistoData1[i+1]*Ryval),0.0f);

			GVGLDrawVertex(a);
			GVGLDrawVertex(b);
		}
		GVGLDrawShow();
	}
	else
	{
		int val=0;
		
		int haf1=x/3;
		int haf2=x/3 + x/3;

		int i=0; for(i=0; i<x; i++)
		{
			GVGLDrawInit(GV_RECTF,1,LColor);
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+10)*Rxval),-(HistoData1[i]*Ryval),0.0f);
			GVGLDrawRectF(a,b);
			GVGLDrawShow();

			if(i==haf1 || i==haf2 || i==x-1 || i==0)
			{
				txt.Format(_T("%.1f"),HistoDataX1[i]);
				GVGLFont(txt,1,a.x+3,a.y+20,0.0f,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
			}
			val+=20;
		}
		
	}

	GLfloat ywidth;
	GLfloat yval2;
	GVColor color;
	GVGLColorFill(&color,0.3f,0.3f,0.3f,1.0f);
	ywidth=(GLfloat)(yval/5);
	yval2=ywidth;
	int i=0; for(i=0; i<5; i++)
	{
		GVertexFill(&a,0,-yval2,0.0f);
		GVertexFill(&b,xval,-yval2,0.0f);
		GVGLDrawInit(GV_LINE,1,color);
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
		GVGLDrawShow();
		yval2+=ywidth;
	}
}
*/
 void GVGLDrawHisto2(GLfloat *HistoDataX1,GLfloat *HistoData1,GLfloat *HistoData2,GVColor HColor,GVColor *LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	CString txt;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;
	
	if(Line)
	{
		Rxval=xval/x;
		Ryval=yval/y;
	}
	else
	{
		Rxval=xval/(x*20);
		Ryval=yval/y;
	}

	if(Line)
	{
		GVGLDrawInit(GV_LINE,1,LColor[0]);
		int i=0; for(i=0; i<x-1; i++)
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
	else
	{
		int val=0;
		
		int i=0; for(i=0; i<x; i++)
		{
			GVGLDrawInit(GV_RECTF,1,LColor[0]);
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+5)*Rxval),-(HistoData1[i]*Ryval),0.0f);
			GVGLDrawRectF(a,b);
			GVGLDrawShow();

			int haf1=x/3;
			int haf2=x/3 + x/3;

			if(i==haf1 || i==haf2 || i==x-1 || i==0)
			{
				txt.Format(_T("%.1f"),HistoDataX1[i]);
				GVGLFont(txt,1,a.x+3,a.y+20,0.0f,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
			}

			val+=20;
		}
		
		
		val=5;
		GVGLDrawInit(GV_RECTF,1,LColor[1]);
		for(i=0; i<x; i++)
		{
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+5)*Rxval),-(HistoData2[i]*Ryval),0.0f);
			
			GVGLDrawRectF(a,b);
			val+=20;
		}
		GVGLDrawShow();
	}
	
	GLfloat ywidth;
	GLfloat yval2;
	GVColor color;
	GVGLColorFill(&color,0.3f,0.3f,0.3f,1.0f);
	ywidth=(GLfloat)(yval/5);
	yval2=ywidth;
	int i=0; for(i=0; i<5; i++)
	{
		GVertexFill(&a,0,-yval2,0.0f);
		GVertexFill(&b,xval,-yval2,0.0f);
		GVGLDrawInit(GV_LINE,1,color);
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
		GVGLDrawShow();
		yval2+=ywidth;
	}

}
 void GVGLDrawHisto3(GLfloat *HistoDataX1,GLfloat *HistoData1,GLfloat *HistoData2,GLfloat *HistoData3,GVColor HColor,GVColor *LColor,int W, int H,int x,GLfloat y,GLfloat TSize,BOOL Line)
{
	GVertex a,b;
	GLfloat xval,yval;
	GLfloat Rxval,Ryval;
	CString txt;
	xval=(GLfloat)(W*TSize);
	yval=(GLfloat)H;
	int i=0;
	if(Line)
	{
		Rxval=xval/x;
		Ryval=yval/y;
	}
	else
	{
		Rxval=xval/(x*20);
		Ryval=yval/y;
	}
	

	if(Line)
	{
		GVGLDrawInit(GV_LINE,1,LColor[0]);
		for( i=0; i<x-1; i++)
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
	else
	{
		int val=0;

		for(i=0; i<x; i++)
		{
			GVGLDrawInit(GV_RECTF,1,LColor[0]);
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+3)*Rxval),-(HistoData1[i]*Ryval),0.0f);
			GVGLDrawRectF(a,b);
			GVGLDrawShow();

			int haf1=x/3;
			int haf2=x/3 + x/3;

			if(i==haf1 || i==haf2 || i==x-1 || i==0)
			{
				txt.Format(_T("%.1f"),HistoDataX1[i]);
				GVGLFont(txt,1,a.x+3,a.y+20,0.0f,2,GVFONT_ROMAN,HColor.R,HColor.G,HColor.B);
			}

			val+=20;
		}
		
		
		val=3;
		GVGLDrawInit(GV_RECTF,1,LColor[1]);
		for(i=0; i<x; i++)
		{
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+3)*Rxval),-(HistoData2[i]*Ryval),0.0f);
			
			GVGLDrawRectF(a,b);
			val+=20;
		}
		GVGLDrawShow();
		
		val=6;
		GVGLDrawInit(GV_RECTF,1,LColor[2]);
		for(i=0; i<x; i++)
		{
			GVertexFill(&a,(GLfloat)(val*Rxval),-(0*Ryval),0.0f);
			GVertexFill(&b,(GLfloat)((val+3)*Rxval),-(HistoData3[i]*Ryval),0.0f);
			
			GVGLDrawRectF(a,b);
			val+=20;
		}
		GVGLDrawShow();
	}
	
	GLfloat ywidth;
	GLfloat yval2;
	GVColor color;
	GVGLColorFill(&color,0.3f,0.3f,0.3f,1.0f);
	ywidth=(GLfloat)(yval/5);
	yval2=ywidth;

	for(i=0; i<5; i++)
	{
		GVertexFill(&a,0,-yval2,0.0f);
		GVertexFill(&b,xval,-yval2,0.0f);
		GVGLDrawInit(GV_LINE,1,color);
		GVGLDrawVertex(a);
		GVGLDrawVertex(b);
		GVGLDrawShow();
		yval2+=ywidth;
	}

}



 BOOL GVGLInitmappingOriginBIG(CString name,GLubyte **texturera,GLubyte **texturera2,int *SizeX,int *SizeY)
{
#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
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
//////////////////////////////////////////////////////////////////
 BOOL GVGLInitmappingTile2(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage2 *texturera,int *SizeX,int *SizeY)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);


#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
	FIBITMAP* temp;
	char* pixeles;
	char** data;
	int w,h;
	int fw,fh;

	temp = FreeImage_ConvertTo8Bits(imagen);

	w = FreeImage_GetWidth(imagen);
	*SizeX=w;
	h = FreeImage_GetHeight(imagen);
	*SizeY=h;
	fw=w;
	fh=h;

	if(w==0 && h==0)
	{
		FreeImage_Unload(imagen);
		FreeImage_Unload(temp);
		return 0;
	}

	h=h/2;

	pixeles=0;
	pixeles= (char*)FreeImage_GetBits(temp);

	data= new char *[fh];
	int i=0; for(i=0; i<fh; i++)
	{
		data[i]= new char [fw];
	}
		
	int ii=0;
	int jj=0;
	int j=0; for(j= 0; j<fw*fh; j++)
	{
		data[ii][jj]=pixeles[j];
		jj++;
		if(jj>=fw)
		{
			jj=0;
			ii++;
		}
	}

	if(texturera->Image1!=NULL)
	{
		delete [] texturera->Image1;
		delete [] texturera->Image2;

		texturera->Image1=NULL;
		texturera->Image2=NULL;
	}
	texturera->Image1 = new GLubyte[w*h];
	texturera->Image2 = new GLubyte[w*h];


	////data in
	ii=0;
	for(i=fh-1; i>=fh/2; i--) //1
	{
		for(j=0; j<fw; j++)
		{
		//	texturera->Image1[ii*4+0]=data[i][j+2];
		//	texturera->Image1[ii*4+1]=data[i][j+1];
			texturera->Image1[ii]=data[i][j];
		//	texturera->Image1[ii*4+3]=data[i][j+3];
			ii++;
		}
	}

	ii=0;
	for(i=fh/2-1; i>=0; i--) //2
	{
		for(j=0; j<fw; j++)
		{
		//	texturera->Image2[ii*4+0]=data[i][j+2];
		//	texturera->Image2[ii*4+1]=data[i][j+1];
			texturera->Image2[ii]=data[i][j];
		//	texturera->Image2[ii*4+3]=data[i][j+3];
			ii++;
		}
	}



	glGenTextures(3, (GLuint*)texturera->Image1);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, fw, fh/2, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image1 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(3, (GLuint*)texturera->Image2);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image2);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, fw, fh/2, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image2 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//////////////////////////////////////end
	for(i=0; i<fh; i++)
	{
		delete [] data[i];
	}
	delete [] data;

	FreeImage_Unload(imagen);
	FreeImage_Unload(temp);
	imagen=NULL;
	temp=NULL;
	return 1;

	//1 
	//2 
}

 void GVGLTextureMappingTile2CAM(GVTextureImage2 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{
		if(fit)
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(Endpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(Endpoint.x, (Endpoint.y/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (Endpoint.y/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (Endpoint.y/2),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, (Endpoint.y/2),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, (Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (Endpoint.y),  startpoint.z); 
		glEnd();
		glPopMatrix();


		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX,       startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX,       (SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (SizeY/2),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX,       (SizeY/2),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX,       (SizeY),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (SizeY),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
}

 void GVGLTextureMappingTile2(GVTextureImage2 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{

	if(fit)
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(Endpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(Endpoint.x, (-Endpoint.y/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-Endpoint.y/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (-Endpoint.y/2),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, (-Endpoint.y/2),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, (-Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-Endpoint.y),  startpoint.z); 
		glEnd();
		glPopMatrix();


		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX,       -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX,       (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (-SizeY/2),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX,       (-SizeY/2),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX,       (-SizeY),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-SizeY),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	
}

 void GVGLReleaseMappingTile2(GVTextureImage2 *texturera)
{
	if(texturera->Image1!=NULL)
	{
		delete [] texturera->Image1;
		delete [] texturera->Image2;
	
		texturera->Image1=NULL;
		texturera->Image2=NULL;
	}
}


////////////////////////////////////////////////////////////////////

 BOOL GVGLInitmappingTile4(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage4 *texturera,int *SizeX,int *SizeY)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);

#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
	FIBITMAP* temp;
	char* pixeles;
	char** data;
	int w,h;
	int fw,fh;

	temp = FreeImage_ConvertTo8Bits(imagen);

	w = FreeImage_GetWidth(imagen);
	*SizeX=w;
	h = FreeImage_GetHeight(imagen);
	*SizeY=h;

	if(w%2!=0)
	{
		w-=-1;
	}
	if(h%2!=0)
	{
		h-=-1;
	}
	fw=w;
	fh=h;

	if(w==0 && h==0)
	{
		FreeImage_Unload(imagen);
		FreeImage_Unload(temp);
		return 0;
	}

	w=w/2;
	h=h/2;

	pixeles=0;
	pixeles= (char*)FreeImage_GetBits(temp);

	data= new char *[fh];
	int i=0; for(i=0; i<fh; i++)
	{
		data[i]= new char [fw];
	}
		
	int ii=0;
	int jj=0;
	int j=0; for(j= 0; j<fw*fh; j++)
	{
		data[ii][jj]=pixeles[j];
		jj++;
		if(jj>=fw)
		{
			jj=0;
			ii++;
		}
	}

	if(texturera->Image1!=NULL)
	{
		delete [] texturera->Image1;
		delete [] texturera->Image2;
		delete [] texturera->Image3;
		delete [] texturera->Image4;
	
		texturera->Image1=NULL;
		texturera->Image2=NULL;
		texturera->Image3=NULL;
		texturera->Image4=NULL;
	}

	texturera->Image1 = new GLubyte[w*h];
	texturera->Image2 = new GLubyte[w*h];
	texturera->Image3 = new GLubyte[w*h];
	texturera->Image4 = new GLubyte[w*h];

	
	////data in
	ii=0;
	for(i=fh-1; i>=fh/2; i--) //1
	{
for(j=0; j<fw/2; j++)
		{
		//	texturera->Image1[ii*4+0]=data[i][j+2];
		//	texturera->Image1[ii*4+1]=data[i][j+1];
			texturera->Image1[ii]=data[i][j];
		//	texturera->Image1[ii*4+3]=data[i][j+3];
			ii++;
		}
	}

	ii=0;
	for(i=fh-1; i>=fh/2; i--) //2
	{
for(j=fw/2; j<fw; j++)
		{
		//	texturera->Image2[ii*4+0]=data[i][j+2];
		//	texturera->Image2[ii*4+1]=data[i][j+1];
			texturera->Image2[ii]=data[i][j];
		//	texturera->Image2[ii*4+3]=data[i][j+3];
			ii++;
		}
	}

	ii=0;
	for(i=(fh/2)-1; i>=0; i--) //3
	{
for(j=0; j<fw/2; j++)
		{
		//	texturera->Image3[ii*4+0]=data[i][j+2];
		//	texturera->Image3[ii*4+1]=data[i][j+1];
			texturera->Image3[ii]=data[i][j];
		//	texturera->Image3[ii*4+3]=data[i][j+3];
			ii++;
		}
	}

	ii=0;
	for(i=(fh/2)-1; i>=0; i--) //4
	{
for(j=fw/2; j<fw; j++)
		{
		//	texturera->Image4[ii*4+0]=data[i][j+2];
		//	texturera->Image4[ii*4+1]=data[i][j+1];
			texturera->Image4[ii]=data[i][j];
		//	texturera->Image4[ii*4+3]=data[i][j+3];
			ii++;
		}
	}

	glGenTextures(3, (GLuint*)texturera->Image1);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, w, h, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image1 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(3, (GLuint*)texturera->Image2);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image2);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, w, h, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image2 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(3, (GLuint*)texturera->Image3);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image3);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, w, h, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image3 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(3, (GLuint*)texturera->Image4);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image4);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, w, h, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image4 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	//////////////////////////////////////end
	for(i=0; i<fh; i++)
	{
		delete [] data[i];
	}
	delete [] data;


	FreeImage_Unload(imagen);
	FreeImage_Unload(temp);
	imagen=NULL;
	temp=NULL;
	return 1;

	//1 2
	//3 4
}

 void GVGLTextureMappingTile4(GVTextureImage4 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{
	if(fit)
	{
	
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -(startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,-(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -Endpoint.y,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -Endpoint.y,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -Endpoint.y,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), (-SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -SizeY,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), -SizeY,  startpoint.z); 
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}	
}

 void GVGLTextureMappingTile4CAM(GVTextureImage4 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{
	if(fit)
	{
		glPushMatrix();
		glTranslatef(0.0f,startpoint.y+Endpoint.y,0.0f);

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image1);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -(startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,-(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image2);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, -Endpoint.y,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image3);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, -(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, -Endpoint.y,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2, -Endpoint.y,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image4);
		glEnd();
		glPopMatrix();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	else
	{

		glPushMatrix();
		glTranslatef(0.0f,startpoint.y+Endpoint.y,0.0f);

		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-SizeY/2),  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image1);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), (-SizeY/2),  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image2);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -SizeY,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image3);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), -SizeY,  startpoint.z); 
		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image4);
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
}

 void GVGLReleaseMappingTile4(GVTextureImage4 *texturera)
{
	if(texturera->Image1!=NULL)
	{
//		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image1);
//		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image2);
//		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image3);
//		glDeleteTextures (SizeX/2+SizeY/2,(GLuint*)texturera->Image4);

		delete [] texturera->Image1;
		delete [] texturera->Image2;
		delete [] texturera->Image3;
		delete [] texturera->Image4;
	
		texturera->Image1=NULL;
		texturera->Image2=NULL;
		texturera->Image3=NULL;
		texturera->Image4=NULL;
	}

}



 BOOL GVGLInitmappingTile16(CDC *Hdc, HGLRC hrc,CString name,GVTextureImage16 *texturera,int *SizeX,int *SizeY)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);

#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
	FIBITMAP* temp=0;
	BYTE* pixeles;
	char** data;
	INT OriginW,OriginH;
	int w,h;
	int fw,fh;
	int ii=0;	
	int jj=0;
	int MaxWidth;

	w = FreeImage_GetWidth(imagen);
	h = FreeImage_GetHeight(imagen);

	temp = FreeImage_ConvertToGreyscale(imagen);

	w = FreeImage_GetWidth(temp);
	h = FreeImage_GetHeight(temp);
	*SizeX=w;
	*SizeY=h;
	OriginW=w;
	OriginH=h;

	int decx=0;
	int decy=0;

	for(;;)
	{
		if(w%4!=0 || w%16!=0)
		{
			w-=-1;
			decx++;
		}
		else
		{
			break;
		}
	}

	for(;;)
	{
		if(h%4!=0 || h%16!=0)
		{
			h-=-1;
		}
		else
		{
			break;
		}
	}

	fw=w;
	fh=h;

	if(w==0 && h==0)
	{
		FreeImage_Unload(imagen);
		FreeImage_Unload(temp);
		return 0;
	}

	w=w/4;
	h=h/4;

	pixeles=0;
	pixeles= FreeImage_GetBits(temp);
	
	data= new char *[fh];
	int i=0; 
	for(i=0; i<fh; i++)
	{
		data[i]= new char [fw];
	}

	int val=decx%4;
	MaxWidth=OriginW+val;

	int j=0; 
	for(j= 0; j<(OriginW*OriginH); j++)
	{
		data[ii][jj]=pixeles[j];
		jj++;
		if(jj==(MaxWidth))
		{
			jj=0;
			ii++;
			if(ii==OriginH)
				break;
		}	
	}

	if(texturera->Image1!=NULL)
	{
		glDeleteTextures(3,(GLuint*)texturera->Image1);
		glDeleteTextures(3,(GLuint*)texturera->Image2);
		glDeleteTextures(3,(GLuint*)texturera->Image3);
		glDeleteTextures(3,(GLuint*)texturera->Image4);
		glDeleteTextures(3,(GLuint*)texturera->Image5);
		glDeleteTextures(3,(GLuint*)texturera->Image6);
		glDeleteTextures(3,(GLuint*)texturera->Image7);
		glDeleteTextures(3,(GLuint*)texturera->Image8);
		glDeleteTextures(3,(GLuint*)texturera->Image9);
		glDeleteTextures(3,(GLuint*)texturera->Image10);
		glDeleteTextures(3,(GLuint*)texturera->Image11);
		glDeleteTextures(3,(GLuint*)texturera->Image12);
		glDeleteTextures(3,(GLuint*)texturera->Image13);
		glDeleteTextures(3,(GLuint*)texturera->Image14);
		glDeleteTextures(3,(GLuint*)texturera->Image15);
		glDeleteTextures(3,(GLuint*)texturera->Image16);

		delete [] texturera->Image1;
		delete [] texturera->Image2;
		delete [] texturera->Image3;
		delete [] texturera->Image4;
		delete [] texturera->Image5;
		delete [] texturera->Image6;
		delete [] texturera->Image7;
		delete [] texturera->Image8;
		delete [] texturera->Image9;
		delete [] texturera->Image10;
		delete [] texturera->Image11;
		delete [] texturera->Image12;
		delete [] texturera->Image13;
		delete [] texturera->Image14;
		delete [] texturera->Image15;
		delete [] texturera->Image16;
	
		texturera->Image1=NULL;
		texturera->Image2=NULL;
		texturera->Image3=NULL;
		texturera->Image4=NULL;
		texturera->Image5=NULL;
		texturera->Image6=NULL;
		texturera->Image7=NULL;
		texturera->Image8=NULL;
		texturera->Image9=NULL;
		texturera->Image10=NULL;
		texturera->Image11=NULL;
		texturera->Image12=NULL;
		texturera->Image13=NULL;
		texturera->Image14=NULL;
		texturera->Image15=NULL;
		texturera->Image16=NULL;
	}

	texturera->Image1 = new GLubyte[w*h];
	texturera->Image2 = new GLubyte[w*h];
	texturera->Image3 = new GLubyte[w*h];
	texturera->Image4 = new GLubyte[w*h];
	texturera->Image5 = new GLubyte[w*h];
	texturera->Image6 = new GLubyte[w*h];
	texturera->Image7 = new GLubyte[w*h];
	texturera->Image8 = new GLubyte[w*h];
	texturera->Image9 = new GLubyte[w*h];
	texturera->Image10 = new GLubyte[w*h];
	texturera->Image11 = new GLubyte[w*h];
	texturera->Image12 = new GLubyte[w*h];
	texturera->Image13 = new GLubyte[w*h];
	texturera->Image14 = new GLubyte[w*h];
	texturera->Image15 = new GLubyte[w*h];
	texturera->Image16 = new GLubyte[w*h];

	
	////data in
	ii=0;
	int x[16];
	int y[16];

	x[0]=0;
	x[1]=fw/4;
	x[2]=fw/2;
	x[3]=fw-(fw/4);
	x[4]=0;
	x[5]=fw/4;
	x[6]=fw/2;
	x[7]=fw-(fw/4);
	x[8]=0;
	x[9]=fw/4;
	x[10]=fw/2;
	x[11]=fw-(fw/4);
	x[12]=0;
	x[13]=fw/4;
	x[14]=fw/2;
	x[15]=fw-(fw/4);

	y[0]=fh-1;
	y[1]=fh-1;
	y[2]=fh-1;
	y[3]=fh-1;
	y[4]=(fh-(fh/4))-1;
	y[5]=(fh-(fh/4))-1;
	y[6]=(fh-(fh/4))-1;
	y[7]=(fh-(fh/4))-1;
	y[8]=(fh/2)-1;
	y[9]=(fh/2)-1;
	y[10]=(fh/2)-1;
	y[11]=(fh/2)-1;
	y[12]=(fh/4)-1;
	y[13]=(fh/4)-1;
	y[14]=(fh/4)-1;
	y[15]=(fh/4)-1;

	for(i=0; i<fh/4; i++) //1
	{
 for(j=0; j<fw/4; j++)
		{
			texturera->Image1[ii]=data[y[0]][x[0]];
			texturera->Image2[ii]=data[y[1]][x[1]];
			texturera->Image3[ii]=data[y[2]][x[2]];
			texturera->Image4[ii]=data[y[3]][x[3]];
			texturera->Image5[ii]=data[y[4]][x[4]];
			texturera->Image6[ii]=data[y[5]][x[5]];
			texturera->Image7[ii]=data[y[6]][x[6]];
			texturera->Image8[ii]=data[y[7]][x[7]];
			texturera->Image9[ii]=data[y[8]][x[8]];
			texturera->Image10[ii]=data[y[9]][x[9]];
			texturera->Image11[ii]=data[y[10]][x[10]];
			texturera->Image12[ii]=data[y[11]][x[11]];
			texturera->Image13[ii]=data[y[12]][x[12]];
			texturera->Image14[ii]=data[y[13]][x[13]];
			texturera->Image15[ii]=data[y[14]][x[14]];
			texturera->Image16[ii]=data[y[15]][x[15]];

			if(texturera->Image1[ii]>=180)	texturera->Image1[ii]=250;
			if(texturera->Image2[ii]>=180)	texturera->Image2[ii]=250;
			if(texturera->Image3[ii]>=180)	texturera->Image3[ii]=250;
			if(texturera->Image4[ii]>=180)	texturera->Image4[ii]=250;
			if(texturera->Image5[ii]>=180)	texturera->Image5[ii]=250;
			if(texturera->Image6[ii]>=180)	texturera->Image6[ii]=250;
			if(texturera->Image7[ii]>=180)	texturera->Image7[ii]=250;
			if(texturera->Image8[ii]>=180)	texturera->Image8[ii]=250;
			if(texturera->Image9[ii]>=180)	texturera->Image9[ii]=250;
			if(texturera->Image10[ii]>=180)	texturera->Image10[ii]=250;
			if(texturera->Image11[ii]>=180)	texturera->Image11[ii]=250;
			if(texturera->Image12[ii]>=180)	texturera->Image12[ii]=250;
			if(texturera->Image13[ii]>=180)	texturera->Image13[ii]=250;
			if(texturera->Image14[ii]>=180)	texturera->Image14[ii]=250;
			if(texturera->Image15[ii]>=180)	texturera->Image15[ii]=250;
			if(texturera->Image16[ii]>=180)	texturera->Image16[ii]=250;
			ii++;

			for(int k=0; k<16; k++)
			{
				x[k]++;
			}
		}
		for(int k2=0; k2<16; k2++)
		{
			y[k2]--;
		}
		x[0]=0;
		x[1]=fw/4;
		x[2]=fw/2;
		x[3]=fw-(fw/4);
		x[4]=0;
		x[5]=fw/4;
		x[6]=fw/2;
		x[7]=fw-(fw/4);
		x[8]=0;
		x[9]=fw/4;
		x[10]=fw/2;
		x[11]=fw-(fw/4);
		x[12]=0;
		x[13]=fw/4;
		x[14]=fw/2;
		x[15]=fw-(fw/4);
	}

	int valx=fw/4;
	int valy=fh/4;

	glGenTextures(3, (GLuint*)texturera->Image1);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image1 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	
	glGenTextures(3, (GLuint*)texturera->Image2);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image2);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image2 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image2);
	
	glGenTextures(3, (GLuint*)texturera->Image3);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image3);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx,valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image3 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image3);
	
	glGenTextures(3, (GLuint*)texturera->Image4);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image4);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image4 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image4);
	
	glGenTextures(3, (GLuint*)texturera->Image5);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image5);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image5 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image5);

	glGenTextures(3, (GLuint*)texturera->Image6);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image6);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image6 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image6);

	glGenTextures(3, (GLuint*)texturera->Image7);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image7);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image7 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image7);

	glGenTextures(3, (GLuint*)texturera->Image8);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image8);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx,valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image8 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image8);

	//
	glGenTextures(3, (GLuint*)texturera->Image9);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image9);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image9 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image9);
	
	glGenTextures(3, (GLuint*)texturera->Image10);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image10);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image10 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image10);

	glGenTextures(3, (GLuint*)texturera->Image11);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image11);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image11 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image11);

	glGenTextures(3, (GLuint*)texturera->Image12);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image12);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image12 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image12);

	glGenTextures(3, (GLuint*)texturera->Image13);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image13);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image13 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image13);

	glGenTextures(3, (GLuint*)texturera->Image14);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image14);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image14 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image14);

	glGenTextures(3, (GLuint*)texturera->Image15);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image15);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image15 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image15);
	
	glGenTextures(3, (GLuint*)texturera->Image16);
	glBindTexture(GL_TEXTURE_2D, *texturera->Image16);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE, valx, valy, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE,(GLvoid*)texturera->Image16 );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glDeleteTextures(valx+valy,(GLuint*)texturera->Image16);
	

	//////////////////////////////////////end
	for(i=0; i<fh; i++)
	{
		delete [] data[i];
	}
	delete [] data;
	data=NULL;

	FreeImage_Unload(imagen);
	FreeImage_Unload(temp);
	imagen=NULL;
	temp=NULL;
	return 1;

	//1  2  3  4
	//5  6  7  8
	//9  10 11 12
	//13 14 15 16
}

 void GVGLTextureMappingTile16(GVTextureImage16 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{

	if(fit)
	{
		glPushMatrix();
	//	if(startpoint.y<0)
			glTranslatef(0.0f,startpoint.y+Endpoint.y,0.0f);

		GLfloat HalfX,HalfY;

		HalfX= (startpoint.x+Endpoint.x)/2;
		HalfY= -(startpoint.y+Endpoint.y)/2;

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
 		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x,            -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x,            -(startpoint.y-HalfY)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image1);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( HalfX                 , -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( HalfX                 , -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -(startpoint.y-HalfY)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image2);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( HalfX                 , -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( HalfX                 , -(startpoint.y-HalfY)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image3);
		glEnd();
		glPopMatrix();
		

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x			, -startpoint.y,            startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x			, -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(startpoint.y-HalfY)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image4);
		glEnd();
		glPopMatrix();
		//
		
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image5);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x,            -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -(startpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2,                  HalfY,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x,                             HalfY,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image5);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image6);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( HalfX                 , -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( HalfX                 , HalfY,				   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, HalfY,				   startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image6);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image7);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( HalfX                 , -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , HalfY,				   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( HalfX                 , HalfY,				   startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image7);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image8);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x			, -(startpoint.y-HalfY)/2, startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x			, HalfY,				   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , HalfY,				   startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image8);
		glEnd();
		glPopMatrix();
		//

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image9);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x,            HalfY,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, HalfY,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -(Endpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x,            -(Endpoint.y-HalfY)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image9);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image10);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( HalfX                 , HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( HalfX                 , -(Endpoint.y-HalfY)/2,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -(Endpoint.y-HalfY)/2,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image10);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image11);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( HalfX                 , HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(Endpoint.y-HalfY)/2,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( HalfX                 , -(Endpoint.y-HalfY)/2,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image11);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image12);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x			, HalfY,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x			, -(Endpoint.y-HalfY)/2,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(Endpoint.y-HalfY)/2,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image12);
		glEnd();
		glPopMatrix();

		//
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image13);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x,            -(Endpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -(Endpoint.y-HalfY)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -Endpoint.y,  startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x,            -Endpoint.y,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image13);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image14);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (startpoint.x+HalfX)/2, -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( HalfX                 , -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( HalfX                 , -Endpoint.y,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (startpoint.x+HalfX)/2, -Endpoint.y,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image14);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image15);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( HalfX                 , -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -Endpoint.y,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( HalfX                 , -Endpoint.y,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image15);
		glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image16);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x			, -(Endpoint.y-HalfY)/2,                startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x			, -Endpoint.y,startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( (HalfX+Endpoint.x)/2  , -Endpoint.y,startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image16);
		glEnd();
		glPopMatrix();

		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (-SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, (-SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), (-SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -SizeY,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, -SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, -SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), -SizeY,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	
}

 void GVGLTextureMappingTile16CAM(GVTextureImage16 *texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL fit)
{

	if(fit)
	{

		glPushMatrix();
		glTranslatef(0.0f,startpoint.y-Endpoint.y,0.0f);

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/4,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (startpoint.y+Endpoint.y)/4,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image1);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/4, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, (startpoint.y+Endpoint.y)/4,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/4,(startpoint.y+Endpoint.y)/4,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image2);
		glEnd();
		glPopMatrix();
		

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/4,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,(startpoint.y+Endpoint.y)/4,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image3);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4), startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, (startpoint.y+Endpoint.y)/4,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4),(startpoint.y+Endpoint.y)/4,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image4);
		glEnd();
		glPopMatrix();
		//

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image5);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image5);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image6);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2,(startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, (startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/4,(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image6);
		glEnd();
		glPopMatrix();
		

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image7);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image7);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image8);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, (startpoint.y+Endpoint.y)/4,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x, (startpoint.y+Endpoint.y)/2,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4),(startpoint.y+Endpoint.y)/2,  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image8);
		glEnd();
		glPopMatrix();
		//

		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image9);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image9);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image10);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/4, (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2,(startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/4,((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image10);
		glEnd();
		glPopMatrix();
		

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image11);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image11);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image12);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4), (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, (startpoint.y+Endpoint.y)/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x,((Endpoint.y)-(startpoint.y+Endpoint.y)/4),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4),((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image12);
		glEnd();
		glPopMatrix();

		//
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image13);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/4, (Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (Endpoint.y),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image13);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image14);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/4, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (startpoint.x+Endpoint.x)/2,((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (startpoint.x+Endpoint.x)/2, (Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/4,(Endpoint.y),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image14);
		glEnd();
		glPopMatrix();
		

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image15);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((startpoint.x+Endpoint.x)/2, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x-((startpoint.x+Endpoint.x)/4), (Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((startpoint.x+Endpoint.x)/2,(Endpoint.y),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image15);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image16);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4), ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Endpoint.x, ((Endpoint.y)-(startpoint.y+Endpoint.y)/4),  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Endpoint.x,(Endpoint.y),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(Endpoint.x-((startpoint.x+Endpoint.x)/4),(Endpoint.y),  startpoint.z); 
		glDeleteTextures(SizeX/4+SizeY/4,(GLuint*)texturera->Image16);
		glEnd();
		glPopMatrix();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image1);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), (SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, (SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image2);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, startpoint.y,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, (SizeY/2),   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), (SizeY/2),  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image3);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (SizeX/2), SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (SizeX/2), SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, SizeY,  startpoint.z); 
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glBindTexture ( GL_TEXTURE_2D, *texturera->Image4);
		glBegin ( GL_QUADS );
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((SizeX/2), SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( SizeX, SizeY/2,  startpoint.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( SizeX, SizeY,   startpoint.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((SizeX/2), SizeY,  startpoint.z); 
		glEnd();
		glPopMatrix();


		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}
	
}



 void GVGLReleaseMappingTile16(GVTextureImage16 *texturera)
{
	if(texturera->Image1!=NULL)
	{
		delete [] texturera->Image1;
		delete [] texturera->Image2;
		delete [] texturera->Image3;
		delete [] texturera->Image4;
		delete [] texturera->Image5;
		delete [] texturera->Image6;
		delete [] texturera->Image7;
		delete [] texturera->Image8;
		delete [] texturera->Image9;
		delete [] texturera->Image10;
		delete [] texturera->Image11;
		delete [] texturera->Image12;
		delete [] texturera->Image13;
		delete [] texturera->Image14;
		delete [] texturera->Image15;
		delete [] texturera->Image16;
	
		texturera->Image1=NULL;
		texturera->Image2=NULL;
		texturera->Image3=NULL;
		texturera->Image4=NULL;
		texturera->Image5=NULL;
		texturera->Image6=NULL;
		texturera->Image7=NULL;
		texturera->Image8=NULL;
		texturera->Image9=NULL;
		texturera->Image10=NULL;
		texturera->Image11=NULL;
		texturera->Image12=NULL;
		texturera->Image13=NULL;
		texturera->Image14=NULL;
		texturera->Image15=NULL;
		texturera->Image16=NULL;
	}

}


 BOOL GVGLInitmappingOrigin(CString name,GLubyte **texturera,int *SizeX,int *SizeY)
{
#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
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

 BOOL GVGLInitmappingResize(CString name,GLubyte **texturera,int *SizeX,int *SizeY,int Multival)
{
#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
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

 BOOL GVGLGetMappingOriginBIG(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,GLubyte *texturera2,int SizeX, int SizeY)
{

	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
#ifdef UNICODE
	char *pText = StringToChar(name);

	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, pText);

	delete [] pText;
	pText = NULL;
#else
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
	FIBITMAP* temp;
	temp = FreeImage_ConvertTo8Bits(imagen);
		
	char* pixeles = (char*)FreeImage_GetBits(temp);
	char** data;
		
	data= new char *[SizeY];
	int i=0; for(i=0; i<SizeY; i++)
	{
		data[i]= new char [SizeX];
	}
		
	int ii=0;
	int ii2=0;
	int jj=0;
	int j=0; for(j= 0; j<SizeX*SizeY; j++)
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
		for(j=0; j<SizeX; j++)
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

 BOOL GVGLGetMappingOrigin(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera,int SizeX, int SizeY,int Mirror)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	int gray=0;
	if(gray==0)
	{
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* temp;
		temp = FreeImage_ConvertTo32Bits(imagen);
		
		char* pixeles = (char*)FreeImage_GetBits(temp);
		char** data;
		
		data= new char *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new char [SizeX*4];
		}
		
		int ii=0;
		int jj=0;
		int j=0; for(j= 0; j<SizeX*SizeY*4; j++)
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
				for(j=(SizeX*4)-4; j>=0; j-=4)
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
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(j=(SizeX*4)-4; j>=0; j-=4)
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
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* temp;
		temp = FreeImage_ConvertTo8Bits(imagen);
		
		char* pixeles = (char*)FreeImage_GetBits(temp);
		char** data;
		
		data= new char *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new char [SizeX];
		}
		
		int ii=0;
		int jj=0;
		int j=0; for(j= 0; j<SizeX*SizeY; j++)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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


 BOOL GVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	int gray=0;
	if(gray==0)
	{
		int ii=0;
		int jj=0;
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo32Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX*4];
		}
		
		int j=0; for(j= 0; j<SizeX*SizeY*4; j++)
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
				for(j=(SizeX*4)-4; j>=0; j-=4)
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
		else if(Mirror==3)
		{
			for(i=0; i<SizeY; i++)
			{
				for(j=(SizeX*4)-4; j>=0; j-=4)
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
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo8Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX];
		}
		
		int j=0; for(j= 0; j<SizeX*SizeY; j++)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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

 BOOL CustomGVGLGetMappingResize(CDC *Hdc, HGLRC hrc,CString name,GLubyte *texturera, int SizeX, int SizeY,int Mirror)
{
	wglMakeCurrent(Hdc->GetSafeHdc(), hrc);
	int gray=0;
	if(gray==0)
	{
		int ii=0;
		int jj=0;
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo32Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX*4];
		}
		
		int j=0; for(j= 0; j<SizeX*SizeY*4; j++)
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

					if(data[i][j+2]>180 &&data[i][j+1]>180 &&data[i][j+0]>180)
					{
						data[i][j+2]=222;
						data[i][j+1]=172;
						data[i][j+0]=147;
					}

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
				for(j=(SizeX*4)-4; j>=0; j-=4)
				{

					if(data[i][j+2]>180 &&data[i][j+1]>180 &&data[i][j+0]>180)
					{
						data[i][j+2]=222;
						data[i][j+1]=172;
						data[i][j+0]=147;
					}

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
				for(j=0; j<SizeX*4; j+=4)
				{

					if(data[i][j+2]>180 &&data[i][j+1]>180 &&data[i][j+0]>180)
					{
						data[i][j+2]=222;
						data[i][j+1]=172;
						data[i][j+0]=147;
					}

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
				for(j=(SizeX*4)-4; j>=0; j-=4)
				{

					if(data[i][j+2]>180 &&data[i][j+1]>180 &&data[i][j+0]>180)
					{
						data[i][j+2]=222;
						data[i][j+1]=172;
						data[i][j+0]=147;
					}

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
#ifdef UNICODE
		char *pText = StringToChar(name);

		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(pText,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, pText);

		delete [] pText;
		pText = NULL;
#else
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name,0);//Automatocally detects the format(from over 20 formats!)
		FIBITMAP* imagen = FreeImage_Load(formato, name);
#endif
		FIBITMAP* aa;
		FIBITMAP* bb;
		aa = FreeImage_Rescale(imagen,SizeX,SizeY,FILTER_BOX);
		bb = FreeImage_ConvertTo8Bits(aa);

		char* pixeles = (char*)FreeImage_GetBits(bb);
		BYTE** data;
		
		data= new BYTE *[SizeY];
		int i=0; for(i=0; i<SizeY; i++)
		{
			data[i]= new BYTE [SizeX];
		}
		
		int j=0; for(j= 0; j<SizeX*SizeY; j++)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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
				for(j=0; j<SizeX; j++)
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
				for(j=SizeX-1; j>=0; j--)
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


 void GVGLTextureMapping(GLubyte * texturera,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY,BOOL Full)
{
	if(texturera)
	{
		if(Full==0)
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
			glDeleteTextures(SizeX+SizeY,(GLuint*)texturera);
			glEnd();
			glPopMatrix();
			
			glDisable(GL_TEXTURE_2D);
				
		}
		else
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
			glDeleteTextures(SizeX+SizeY,(GLuint*)texturera);
			glEnd();
			glPopMatrix();
			
			glDisable(GL_TEXTURE_2D);
		
		}
	}
	
}

 void GVGLTextureMappingBIG(GLubyte * texturera,GLubyte * texturera2,GVertex startpoint,GVertex Endpoint,int SizeX, int SizeY)
{
	if(texturera)
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

 void GVGLReleaseMapping(GLubyte ** texturera)
{
	if(*texturera)
	{
		delete [] *texturera;
		*texturera=NULL;
	}
	
}

 void GVGLTextureMapping(int Mappingmodel, GLubyte * texturera, GVertex startpoint, GVertex Endpoint, int SizeX, int SizeY, BOOL Full)
 {
	 if (texturera)
	 {
		 if (Full == 0)
		 {
			 if (Mappingmodel == 0)
			 {
				 glEnable(GL_TEXTURE_2D);

				 glPushMatrix();
				 glBindTexture(GL_TEXTURE_2D, *texturera);
				 glBegin(GL_QUADS);
				 glColor3f(1.0f, 1.0f, 1.0f);
				 glNormal3f(0.0f, 0.0f, 1.0f);
				 glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y, startpoint.z);
				 glTexCoord2f(1.0f, 0.0f); glVertex3f(Endpoint.x, -startpoint.y, startpoint.z);
				 glTexCoord2f(1.0f, 1.0f); glVertex3f(Endpoint.x, -Endpoint.y, startpoint.z);
				 glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -Endpoint.y, startpoint.z);
				 glEnd();
				 glPopMatrix();

				 glDisable(GL_TEXTURE_2D);
			 }
		 }
		 else
		 {
			 if (Mappingmodel == 0)
			 {
				 glEnable(GL_TEXTURE_2D);

				 glPushMatrix();
				 glBindTexture(GL_TEXTURE_2D, *texturera);
				 glBegin(GL_QUADS);
				 glColor3f(1.0f, 1.0f, 1.0f);
				 glNormal3f(0.0f, 0.0f, 1.0f);
				 glTexCoord2f(0.0f, 0.0f); glVertex3f(startpoint.x, -startpoint.y, startpoint.z);
				 glTexCoord2f(1.0f, 0.0f); glVertex3f(SizeX, -startpoint.y, startpoint.z);
				 glTexCoord2f(1.0f, 1.0f); glVertex3f(SizeX, -SizeY, startpoint.z);
				 glTexCoord2f(0.0f, 1.0f); glVertex3f(startpoint.x, -SizeY, startpoint.z);
				 glEnd();
				 glPopMatrix();

				 glDisable(GL_TEXTURE_2D);
			 }
		 }
	 }

 }
