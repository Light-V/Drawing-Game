#include <Windows.h>

#include "config.h"

//��Ӧ����ԭ��
LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

//���
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
		MessageBox(NULL, TEXT("�޷�ע�ᴰ����"), TEXT("����"), MB_ICONERROR);
		return 0;
	}

	G_hwnd = CreateWindow(className,wndTittle,wndStyle,wndx,wndy,wndWidth,wndHeight,wndParent,wndMenu,hInstance,wndParam);
	if (G_hwnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), TEXT("����"), MB_ICONERROR);
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
