#pragma once

#include <Windows.h>

#include "Game.h"

//function
int Getwndx(int);
int Getwndy(int);

//Var
HWND G_hwnd = NULL;
MSG msg = {};
Game myGame;

//������Ϣ

#define LEFT 1
#define UP 2
#define RIGHT 3
#define DOWN 4

//Timer
#define TMR_BEGIN 10000
#define TMR_PLAY 10001
#define TMR_PAUSE 10002
//Class
UINT      cstyle = CS_HREDRAW | CS_VREDRAW;							//ˮƽ�ػ��Լ���ֱ�ػ�
int       ccbClsExtra = 0;
int       ccbWndExtra = 0;											//����ռ�
HICON     chIcon = (HICON)::LoadImage(	NULL,						//Not from instance
										L".\\res\\icon1.ico",		//path
										IMAGE_ICON,
										0,
										0, 
										LR_LOADFROMFILE);			//Loadmode
HCURSOR   chCursor = LoadCursor(NULL, IDC_ARROW);
HBRUSH    chbrBackground = CreateSolidBrush(RGB(255, 255, 255));					//���ڱ�����ɫ
LPCTSTR   clpszMenuName = NULL;										//�޲˵�
WCHAR*    className = L"Puzzle Class";								//����

//Window
WCHAR*		wndTittle = L"Puzzle:      ?";
DWORD		wndStyle = WS_OVERLAPPEDWINDOW^WS_THICKFRAME;
int			wndWidth = 800;
int			wndHeight = 600;
int			wndx = Getwndx(wndWidth);
int			wndy = Getwndy(wndHeight);
HWND		wndParent = NULL;
HMENU		wndMenu = NULL;
LPVOID		wndParam = NULL;

HBITMAP     hbTitle = (HBITMAP)::LoadImage(NULL,						
								L".\\res\\title.bmp",		//path
								IMAGE_BITMAP,
								0,
								0,
								LR_LOADFROMFILE);
//Menu
struct GameMenu
{
	int id;
	TCHAR szName[10];
	int xPos;
	int yPos;
}menu[] = {
	{ 1, TEXT("��ʼ��Ϸ"), 340, 250 },
	{ 2, TEXT("�鿴����"), 340, 300 },
	{ 3, TEXT("����ѡ��"), 340, 350 },
	{ 4, TEXT("�˳���Ϸ"), 340, 400 }
};
int Getwndx(int width)
{
	return (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
}

int Getwndy(int height)
{
	return (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
}