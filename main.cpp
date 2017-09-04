#include <Windows.h>

#include "config.h"

//响应函数原型
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

//入口
int WINAPI WinMain(
	  HINSTANCE hInstance,
	  HINSTANCE hPrevInstance,
	  LPSTR lpCmdLine,
	  int nCmdShow
)
{
	WNDCLASS wnc = {};
	wnc.style =				cstyle;
	wnc.lpfnWndProc =		WindowProc;
	wnc.cbClsExtra =		ccbClsExtra;
	wnc.cbWndExtra =		ccbWndExtra;
	wnc.hInstance =			hInstance;
	wnc.hIcon =				chIcon;
	wnc.hCursor =			chCursor;
	wnc.hbrBackground =		chbrBackground;
	wnc.lpszMenuName =		clpszMenuName;
	wnc.lpszClassName =		className;


	if (!RegisterClass(&wnc))
	{
		MessageBox(NULL, TEXT("无法注册窗口类"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}

	G_hwnd = CreateWindow(className,wndTittle,wndStyle,wndx,wndy,wndWidth,wndHeight,wndParent,wndMenu,hInstance,wndParam);
	if (G_hwnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}

	ShowWindow(G_hwnd, SW_SHOW);
	UpdateWindow(G_hwnd);

	myGame.InitializeGame(G_hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
