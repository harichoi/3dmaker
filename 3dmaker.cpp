#include <math.h>
#include <afxext.h>
#include <afxdisp.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif 

#include <windows.h>
#include <Stdio.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include "resource.h"

#define PI 3.1415926535
#define Ax (30+aax)*PI/180
#define Ay (30+aay)*PI/180
#define Az (0+aaz)*PI/180

struct point
{
	double x;
	double y;
	double z;
};
struct Layer
{
	int np;
	point p[10];
	point center;
	double dist;
	COLORREF rgb;
};

int fx, fy, sx, sy;
double x; double y;
double av;
double rotate;
double v;
double nowx, nowy, nowz;
double aax, aay, aaz;

		point bb;

Layer layer[10000];
int nlayer=1;
int keybd[256];
int aaa;
int order[10000];
int slayer=1;

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
void draw(HWND hWnd);
void stage(int level);
void rot(double ax, double ay, double az, double &x, double &y, double &z);
point makepoint(double xx, double yy, double zz);

int WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE hPrev,
				   LPSTR lpCmd,
				   int nShow)
{
	
	srand(GetTickCount());
	HWND hWnd;
	WNDCLASS wnd;
	MSG msg;

	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wnd.hCursor = 0;//LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
	wnd.hIcon = 0;//LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance = hInst;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = "gre3dmaker";
	wnd.lpszMenuName =0;// MAKEINTRESOURCE(IDR_MENU1);
	wnd.style = 0;

	RegisterClass(&wnd);

	hWnd = CreateWindow("gre3dmaker","GREMCUS-3dmaker",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,700,400,0,0,hInst,0);
	ShowWindow(hWnd,SW_SHOW);

//	selectx++;selecty++;
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
int visit[10000];
int check;
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int ttype;
	int i, j, k, ax, ay, temp;
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(1);
		break;
	case WM_CREATE:
		
		bb.x=500;
		bb.y=-700;
		bb.z=-1002;
	//	rot(Ax,Ay,Az,bb.x,bb.y,bb.z);
		break;
	case WM_TIMER:
		//aay++;
		for(i=1 ; i<=nlayer ; i++)
		{
			order[i]=nlayer-i+1;
			point aa;
			aa.x=aa.y=aa.z=0;
			for(j=1 ; j<=layer[i].np ; j++)
			{
				aa.x+=layer[i].p[j].x;
				aa.y+=layer[i].p[j].y;
				aa.z+=layer[i].p[j].z;
			}
			aa.x/=layer[i].np;
			aa.y/=layer[i].np;
			aa.z/=layer[i].np;
			rot(Ax,Ay,Az,aa.x,aa.y,aa.z);
			layer[i].center=aa;
		}
		for(i=1 ; i<=nlayer ; i++)
			for(j=1 ; j<=nlayer ; j++)
			{
				double ppi, ppj;
				ppi=layer[order[i]].center.z;
				ppj=layer[order[j]].center.z;
				if(ppi>ppj)
				{
					int a;
					a=order[i];
					order[i]=order[j];
					order[j]=a;
				}
			}
				if(keybd['U']){
					keybd['U']=0;
				}
		if(keybd[VK_LEFT])
		{
			nowx-=5;
		}
		if(keybd[VK_RIGHT])
		{
			nowx+=5;
		}
		if(keybd[VK_UP] && keybd[VK_SPACE])
		{
			nowy-=5;
		}
		if(keybd[VK_DOWN] && keybd[VK_SPACE])
		{
			nowy+=5;
		}
		if(keybd[VK_UP] && !keybd[VK_SPACE])
		{
			nowz+=5;
		}
		if(keybd[VK_DOWN] && !keybd[VK_SPACE])
		{
			nowz-=5;
		}
		if(keybd[VK_RETURN])
		{
			keybd[VK_RETURN]=0;
			layer[slayer].p[++layer[slayer].np]=makepoint(nowx,nowy,nowz);
		}
		if(keybd['Z'])
		{
			layer[slayer].np--;
		}
		if(keybd['E'])
		{
			nlayer++;
			slayer=nlayer;
			keybd['E']=0;
		}
		if(keybd['O'])
		{
			FILE *file;
			file=fopen("3dmaker.txt","r");
			{
				fscanf(file,"%d",&nlayer);
				for(i=1 ; i<=nlayer ; i++)
				{
					int a,b,c;
					fscanf(file,";%d;%d;%d;",&a,&b,&c);
					layer[i].rgb=RGB(a,b,c);
					fscanf(file,"\n%d\n",&layer[i].np);
					for(j=1 ; j<=layer[i].np ; j++)
						fscanf(file,"(%lf,%lf,%lf)\n",&layer[i].p[j].x,&layer[i].p[j].y,&layer[i].p[j].z);
				}
			}
			keybd['O']=0;
			fclose(file);
		}
		if(keybd['F'])
		{
			FILE *file;
			file=fopen("3dmaker.txt","w");
			{
				fprintf(file,"%d",nlayer);
				for(i=1 ; i<=nlayer ; i++)
				{
					fprintf(file,";%d;%d;%d;",GetRValue(layer[i].rgb),GetGValue(layer[i].rgb),GetBValue(layer[i].rgb));
					fprintf(file,"\n%d\n",layer[i].np);
					for(j=1 ; j<=layer[i].np ; j++)
						fprintf(file,"(%.0lf,%.0lf,%.0lf)\n",layer[i].p[j].x,layer[i].p[j].y,layer[i].p[j].z);
				}
			}
			keybd['F']=0;
			fclose(file);
		}
		if(keybd['C'])
		{
			keybd['C']=0;
			CColorDialog a;
			a.DoModal();
			layer[slayer].rgb=a.GetColor();
		}
		if(keybd['S'])
		{
			slayer++;
			if(nlayer+1==slayer)
				slayer=1;
			keybd['S']=0;
		}
		if(keybd['R'])
		{
			aay+=3;
		//	aax+=4;
		//	aaz+=5;
		}
		draw(hWnd);
		break;
	case WM_KEYDOWN:
		keybd[wParam]=1;
		break;
	case WM_KEYUP:
		keybd[wParam]=0;
		break;
	case WM_COMMAND:
		
		break;
	case WM_PAINT:
		SetTimer(hWnd,0,100,0);
		draw(hWnd);
		break;
	};
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

void draw(HWND hWnd)
{
	int possible;
	int max;
	int i, j, k;
	char str[256];
	HBITMAP bit, *old, *old2;
	HBRUSH *old3, brush;
	HDC hDC;
	HDC MemDC;
	HDC bitDC;

	hDC=GetDC(hWnd);
	bit=CreateCompatibleBitmap(hDC,1200,1200);
	MemDC=CreateCompatibleDC(hDC);
	bitDC=CreateCompatibleDC(hDC);
	//hDC=BeginPaint(hWnd,&ps);
	old = (HBITMAP *)SelectObject(MemDC,bit);

	Rectangle(MemDC,0,0,1024,768);

	SetBkMode(MemDC,0);

	//배경  출력
	//	old2=(HBITMAP *)SelectObject(bitDC,bgbit);
	//	BitBlt(MemDC,0,0,1024,768,bitDC,0,0,SRCCOPY);
	//	SelectObject(bitDC,old2);
	double ax, ay, az;

	double xx, yy, zz;
	/* 
	layer point
	*/
	for(i=1 ; i<=nlayer ; i++)
	{
		Layer o;
		o=layer[order[i]];
		POINT a[100];
		for(j=1 ; j<=o.np ; j++)
		{
			xx=o.p[j].x;
			yy=o.p[j].y;
			zz=o.p[j].z;
			rot(Ax,Ay,Az,xx,yy,zz);
			a[j-1].x=xx+200;
			a[j-1].y=yy+200;
		}
		if(o.np>=3)
		{
			HPEN pen, *op;
	HBRUSH brs, *ob;
	if(slayer==order[i])
	pen=CreatePen(0,1,RGB(0,200,200));
	else
		pen=CreatePen(0,1,RGB(0,0,0));
	brs=CreateSolidBrush(layer[order[i]].rgb);
	
	op=(HPEN *)SelectObject(MemDC,pen);
	ob=(HBRUSH *)SelectObject(MemDC,brs);
	
			Polygon(MemDC,a,o.np);
	SelectObject(MemDC,ob);
	SelectObject(MemDC,op);
	DeleteObject(op);
	DeleteObject(ob);
	DeleteObject(pen);
	DeleteObject(brs);
		//	Sleep(1000);
		}
	}
	ax=-100, ay=az=0;
	rot(Ax,Ay,Az,ax,ay,az);
	MoveToEx(MemDC,ax+200,ay+200,0);

	ax=100, ay=az=0;
	rot(Ax,Ay,Az,ax,ay,az);
	LineTo(MemDC,ax+200,ay+200);
	
	ay=100, ax=az=0;
	rot(Ax,Ay,Az,ax,ay,az);
	MoveToEx(MemDC,ax+200,ay+200,0);

	ay=-100, ax=az=0;
	rot(Ax,Ay,Az,ax,ay,az);
	LineTo(MemDC,ax+200,ay+200);
	
	az=-100, ax=ay=0;
	rot(Ax,Ay,Az,ax,ay,az);
	MoveToEx(MemDC,ax+200,ay+200,0);

	az=100, ax=ay=0;
	rot(Ax,Ay,Az,ax,ay,az);
	LineTo(MemDC,ax+200,ay+200);
	
	az=-100, ax=ay=0;
	rot(Ax,Ay,Az,ax,ay,az);
	MoveToEx(MemDC,ax+202,ay+200,0);

	az=100, ax=ay=0;
	rot(Ax,Ay,Az,ax,ay,az);
	LineTo(MemDC,ax+202,ay+200);
	
	xx=nowx;
	yy=nowy;
	zz=nowz;
	rot(Ax,Ay,Az,xx,yy,zz);
	Ellipse(MemDC,xx-5+200,yy-5+200,xx+5+200,yy+5+200);

	/*xx=50;
	yy=50;
	zz=++aaa;
	rot(Ax,Ay,Az,xx,yy,zz);
	Ellipse(MemDC,xx-5+200,yy-5+200,xx+5+200,yy+5+200);
*/
	xx=nowx;
	yy=0;
	zz=0;
	rot(Ax,Ay,Az,xx,yy,zz);
	MoveToEx(MemDC,xx+200,yy+200,0);
	xx=nowx;
	yy=nowy;
	zz=nowz;
	rot(Ax,Ay,Az,xx,yy,zz);
	LineTo(MemDC,xx+200,yy+200);
	xx=0;
	yy=nowy;
	zz=0;
	rot(Ax,Ay,Az,xx,yy,zz);
	MoveToEx(MemDC,xx+200,yy+200,0);
	xx=nowx;
	yy=nowy;
	zz=nowz;
	rot(Ax,Ay,Az,xx,yy,zz);
	LineTo(MemDC,xx+200,yy+200);
	xx=0;
	yy=0;
	zz=nowz;
	rot(Ax,Ay,Az,xx,yy,zz);
	MoveToEx(MemDC,xx+200,yy+200,0);
	xx=nowx;
	yy=nowy;
	zz=nowz;
	rot(Ax,Ay,Az,xx,yy,zz);
	LineTo(MemDC,xx+200,yy+200);
	
	//rot(Ax,Ay,Az,nowx,nowy,nowz);
	sprintf(str,"%.0lf,%.0lf,%.0lf",nowx,nowy,nowz);
	TextOut(MemDC,00,00,str,strlen(str));
BitBlt(hDC,0,0,1024,768,MemDC,00,00,SRCCOPY);
	
	SelectObject(MemDC,old);
	ReleaseDC(hWnd,MemDC);
	ReleaseDC(hWnd,bitDC);
	ReleaseDC(hWnd,hDC);
	DeleteObject(bit);
	DeleteObject(old);
	DeleteObject(old2);
	DeleteObject(old3);
	DeleteObject(MemDC);
	DeleteObject(bitDC);
	DeleteObject(hDC);


}

void rot(double ax, double ay, double az, double &x, double &y, double &z)
{
	double x1, x2, y1, y2, z1, z2, x3, y3, z3;
/*
	x1=z*sin(ay)+x*cos(ay);
	y1=y;
	z1=z*cos(ay)-x*sin(ay);
	x2=x1;
	y2=z1*sin(ax)+y1*cos(ax);
	z2=z1*cos(ax)-y1*sin(ax);
	x3=x2*cos(az)-y2*sin(az);
	y3=x2*sin(az)+y2*cos(az);
	z3=z3;
	x=x3;
	y=y3;
	z=z3;
*/	/*x 축의 중심으로 */
	y1=y*cos(ax)-z*sin(ax);
	z1=y*sin(ax)+z*cos(ax);
/*y 축의 중심으로 */
	z2=z1*cos(ay)-x*sin(ay);
	x1=z1*sin(ay)+x*cos(ay);
/*z 축의 중심으로 */
	x2=x1*cos(az)-y1*sin(az);
	y2=x1*sin(az)+y1*cos(az);
	x=x2;
	y=y2;
	z=z2;
}

point makepoint(double xx, double yy, double zz)
{
	point a;
	a.x=xx;
	a.y=yy;
	a.z=zz;
	return a;
}