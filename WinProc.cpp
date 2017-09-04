#pragma once

#include <Windows.h>
#include "Game.h"
extern Game myGame;
#define TMR_BEGIN 10000
#define TMR_PLAY 10001
#define TMR_PAUSE 10002
#define TMR_OVER 10003
struct GameMenu
{
	int id;
	TCHAR szName[10];
	int xPos;
	int yPos;
};
HPEN hOldPen;
HBRUSH hOldBrush;
HFONT hOldFont;
HBITMAP hOldb;
extern GameMenu menu[4];
extern HWND G_hwnd;
extern HBITMAP hbTitle;
int DRAWSELECT(HWND hwnd, int ctrl);
int DealMouseMove(HWND hwnd);
int DealMouseClick(HWND hwnd);
void DrawGame(HWND hwnd, HDC);
void drawGreenBox(HWND hwnd, int yPos);
void drawWhiteBox(HWND hwnd, int yPos);
void DrawBigBox(HWND hwnd, HDC hdc);
void DrawDarkBlock(HWND hwnd, int x, int y, HDC);
void DrawGreyBlock(HWND hwnd, int x, int y, HDC);
void GetOpsRGB(int& r, int& g, int& b);
wchar_t* AnsiToUnicode(char* szStr, wchar_t* pResult);
void DrawColorLine(HWND hwnd, int x1, int y1, int x2, int y2, int r, int g, int b, HDC hdc);
void DrawColorBlock(HWND hwnd, int x, int y, int r, int g, int b, HDC hdc);
void DrawColorCircle(HWND hwnd, int x, int y, int r, int g, int b, HDC hdc);
void CALLBACK TimerProc(HWND hwnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{

	bool Timerflag[10] = {0,0,0,0,0,0,0,0,0,0};
	//状态计时器
	switch (myGame.GetStatus())
	{
	case MainState:
	{
		//刷新菜单
		if (Timerflag[0] == 0)
		{
			//清除其他计时器
			KillTimer(hwnd, TMR_PLAY);
			KillTimer(hwnd, TMR_PAUSE);
			KillTimer(hwnd, TMR_OVER);
			SetTimer(hwnd, TMR_BEGIN, 10, NULL);
			for (int i = 0; i <= 9; i++) Timerflag[i] = 0;
			Timerflag[0] = 1;
		}
		else {}
		break;
	}
	case PlayingState:
	{
		//刷新游戏状态
		if (Timerflag[1] == 0)
		{
			//清除其他计时器
			KillTimer(hwnd, TMR_BEGIN);
			KillTimer(hwnd, TMR_PAUSE);
			KillTimer(hwnd, TMR_OVER);
			SetTimer(hwnd, TMR_PLAY, 15, NULL);
			for (int i = 0; i <= 9; i++) Timerflag[i] = 0;
			Timerflag[1] = 1;
		}
		else {}
		break;
	}
	case StopState:
	{
		if (Timerflag[2] == 0)
		{
			//清楚其他计时器
			KillTimer(hwnd, TMR_PLAY);
			KillTimer(hwnd, TMR_BEGIN);
			KillTimer(hwnd, TMR_OVER);
			SetTimer(hwnd, TMR_PAUSE, 10, NULL);
			for (int i = 0; i <= 9; i++) Timerflag[i] = 0;
			Timerflag[2] = 1;

			//暂停游戏时间倒计时
		}
		else {}
		break;
	}
	case GameOver:
	{
		if (Timerflag[3] == 0)
		{
			//清除其他计时器
			KillTimer(hwnd, TMR_PLAY);
			KillTimer(hwnd, TMR_PAUSE);
			KillTimer(hwnd, TMR_PAUSE);
			SetTimer(hwnd, TMR_OVER, 10, NULL);
			for (int i = 0; i <= 9; i++) Timerflag[i] = 0;
			Timerflag[3] = 1;
		}
		else {}
		break;
	}
	}

	switch (uMsg)
	{
	case WM_DESTROY://销毁
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CREATE://初始化
	{
		myGame.InitializeGame(hwnd);
		break;
	}
	case WM_TIMER:
	{
		HDC	hdc = GetDC(hwnd);
		switch (wParam)
		{
		case TMR_BEGIN:
		{		
				//清屏
			{
				HBRUSH hBrush;
				hBrush = CreateSolidBrush(RGB(255, 255, 255));
				hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, 0, 0, 784, 562);

				SelectObject(hdc, hOldBrush);
			}
			HDC hCompatibleDC = CreateCompatibleDC(hdc);
			SelectObject(hCompatibleDC, hbTitle);
			BitBlt(hdc, 250, 100, 300, 150, hCompatibleDC, 0, 0, SRCCOPY/*Very Important*/);
			DeleteDC(hCompatibleDC);
			HFONT hFont;
			HBRUSH hBrush;
			LOGFONT lf = { 0 };
			strcpy((char *)(lf.lfFaceName), "黑体");
			lf.lfWidth = 12;
			lf.lfHeight = 25;
			lf.lfWeight = FW_NORMAL;
			lf.lfCharSet = GB2312_CHARSET;
			hFont = CreateFontIndirect(&lf);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));

			SelectObject(hdc, hFont);
			SelectObject(hdc, hBrush);

			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextCharacterExtra(hdc, 3);
			SetTextColor(hdc, RGB(0, 150, 200));

			for (int i = 0; i < 4; i++)
				int p = TextOut(hdc, menu[i].xPos, menu[i].yPos, menu[i].szName, lstrlen(menu[i].szName));

			DeleteObject(hFont);
			DeleteObject(hBrush);
			ReleaseDC(hwnd, hdc);
			//处理选项绘制
			DealMouseMove(hwnd);

			break;
		}
		case TMR_PLAY:
		{
			//判断是否胜利
			if (myGame.GetFinished()<52)
			{
				//处理鼠标信息（修改实况）
				DealMouseMove(hwnd);
				//显示图像
				DrawGame(hwnd, hdc);

			}
			//胜利
			else
			{
				myGame.ChangeStatus(GameOver);
			}
			break;
		}
		case TMR_PAUSE:
		{
			//显示暂停画面

			break;
		}
		case TMR_OVER:
		{
			HFONT hFont;
			HBRUSH hBrush;
			LOGFONT lf = { 0 };
			strcpy((char *)(lf.lfFaceName), "黑体");
			lf.lfWidth = 12;
			lf.lfHeight = 25;
			lf.lfWeight = FW_NORMAL;
			lf.lfCharSet = GB2312_CHARSET;
			hFont = CreateFontIndirect(&lf);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));

			hOldFont = (HFONT)SelectObject(hdc, hFont);
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			SetBkColor(hdc, RGB(255, 255, 255));
			SetTextCharacterExtra(hdc, 3);
			SetTextColor(hdc, RGB(0, 150, 200));

			TextOut(hdc, 650, 250, L"返回菜单", 4);
			TextOut(hdc, 650, 350, L"退出游戏", 4);
			TextOut(hdc, 380, 20, L"You Win", 7);

			SelectObject(hdc, hOldFont);
			DeleteObject(hFont);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hBrush);
			ReleaseDC(hwnd, hdc);
			//处理选项绘制
			DealMouseMove(hwnd);
		}
		}
		ReleaseDC(hwnd,hdc);
		break;
	}
	case WM_PAINT:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	{
		DealMouseClick(hwnd);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//处理光标点击信息
int DealMouseClick(HWND hwnd)
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	switch (myGame.GetStatus())
	{
	case MainState:
	{
		if ((pt.x < 300) || (pt.x > 500))break;
		if (pt.y > 250 && pt.y < 285)
			//开始游戏
			myGame.ChangeStatus(PlayingState);
		if (pt.y > 300 && pt.y < 335)
		{
		}
			//帮助
		if (pt.y > 350 && pt.y < 385)
		{
		}
				//设置
		if (pt.y > 400 && pt.y < 435)
			DestroyWindow(hwnd);
			//退出
		break;
	}
	case PlayingState:
	{
		if ((pt.x >= 200)
			&& (pt.x <= 600)
			&& (pt.y >= 75)
			&& (pt.y <= 475))
		{
			myGame.Click((pt.y - 75) / 20, (pt.x - 200) / 20);
		}
		break;
	}
	case StopState:
	{
		break;
	}
	case GameOver:
	{
		if ((pt.x < 600) || (pt.x > 700))break;
		//返回菜单
		if (pt.y > 225 && pt.y < 275)
			myGame.ChangeStatus(MainState);
		if (pt.y > 325 && pt.y < 375)
			DestroyWindow(hwnd);
		//退出
		break;
	}
	default:
	{
		//ERROR
	}
	}
	return 1;
}

//处理光标移动信息
int DealMouseMove(HWND hwnd)
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	switch (myGame.GetStatus())
	{
		case MainState:
		{
			if ((pt.x < 300) | (pt.x > 500))break;
			if (pt.y > 250 && pt.y < 285)
				DRAWSELECT(hwnd, 1);

			if (pt.y > 300 && pt.y < 335)
				DRAWSELECT(hwnd, 2);
			//DRAW TWO

			if (pt.y > 350 && pt.y < 385)
				DRAWSELECT(hwnd, 3);
			//DRAW THREE

			if (pt.y > 400 && pt.y < 435)
				DRAWSELECT(hwnd, 4);
			//DRAW FOUR
			break;
		}
		case PlayingState:
		{
			if (	(pt.x >= 200)
				&&	(pt.x <= 600)
				&&	(pt.y >= 75)
				&&	(pt.y <= 475))
			{
				myGame.PlayMouseMove((pt.y - 75) / 20, (pt.x - 200) / 20);
			}
			break;
		}
		case StopState:
		{
			break;
		}
		case GameOver:
		{

		}
		default:
		{
			//ERROR
		}
	}
	


	return 0;
}

//DRAW 框框
int DRAWSELECT(HWND hwnd,int ctrl)
{
	int yPos=190+ctrl*50;
	int i = 0;
	for (i; i < 4; i++)
	{
		drawWhiteBox(hwnd, 240 + i * 50);
	}
	drawGreenBox(hwnd, yPos);
	return 0;
}

void drawGreenBox(HWND hwnd, int yPos)
{
	HDC hdc;
	HPEN hPen;
	POINT apn[5] = { { 325, yPos },{ 460, yPos },{ 460, yPos + 45 },{ 325, yPos + 45 },{ 325, yPos } };        //坐标组

	hdc = GetDC(hwnd);
	hPen = CreatePen(PS_SOLID, 3, RGB(0, 128, 0));
	SelectObject(hdc, hPen);
	Polyline(hdc, apn, 5);

	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
}

//绘制白色边框
void drawWhiteBox(HWND hwnd, int yPos)
{
	HDC hdc;
	HPEN hPen;
	POINT apn[5] = { { 325, yPos },{ 460, yPos },{ 460, yPos + 45 },{ 325, yPos + 45 },{ 325, yPos } };        //坐标组

	hdc = GetDC(hwnd);
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Polyline(hdc, apn, 5);

	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
}

void DrawGame(HWND hwnd, HDC hdc)
{
	//HDC hdc;
	//HDC prehdc = GetDC(hwnd);
	//hdc = CreateCompatibleDC(prehdc);
	HDC newhdc;
	newhdc = CreateCompatibleDC(hdc);
	HBITMAP hb = CreateCompatibleBitmap(hdc, 800, 600);

	hOldb = (HBITMAP)SelectObject(newhdc,hb);

	HFONT hFont;
	HBRUSH hBrush;
	HPEN hPen;
	LOGFONT lf = { 0 };
	//画背景以及边框
	//hPen = CreatePen(PS_SOLID, 10, RGB(20, 20, 20));
	hBrush = CreateSolidBrush(RGB(150, 220, 235));
	hOldBrush = (HBRUSH)SelectObject(newhdc, hBrush);

	Rectangle(newhdc, 0, 0, 784, 562);

	SelectObject(newhdc, hOldBrush);
	//DeleteObject(hPen);
	int x =DeleteObject(hBrush);

	//TEXTOUT TIME & FINISH
	{
		const char st[] = "黑体";
		strcpy((char *)(lf.lfFaceName), st);
		lf.lfWidth = 12;
		lf.lfHeight = 25;
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = GB2312_CHARSET;
		hFont = CreateFontIndirect(&lf);
		hBrush = CreateSolidBrush(RGB(0, 0, 0));

		hOldFont = (HFONT)SelectObject(newhdc, hFont);
		hOldBrush = (HBRUSH)SelectObject(newhdc, hBrush);

		//主体
		{
			SetBkColor(newhdc, RGB(255, 255, 255));
			SetTextCharacterExtra(hdc, 3);
			SetTextColor(newhdc, RGB(0, 150, 200));

			//draw time
			{
				TextOut(newhdc, 50, 50, L"TIME:", 5);
				wchar_t ctime[8] = { L'0',L'0', L'0', L'.', L'0', L'0',L's',L'\0' };
				myGame.ReturnTime(ctime);
				TextOut(newhdc, 50, 75, ctime, 7);
			}
			//draw finish
			{
				TextOut(newhdc, 50, 300, L"FINISH:", 7);
				{
					int num = myGame.GetFinished();
					wchar_t t;
					if (num >= 10)
					{
						//十位
						int num_ten = num / 10;
						t = num_ten + 48;
						TextOut(newhdc, 36, 340, &t, 1);
						//个位
						num -= 10 * num_ten;
						t = num + 48;
						TextOut(newhdc, 48, 340, &t, 1);
					}
					else
					{
						t = num + 48;
						TextOut(newhdc, 50, 340, &t, 1);
					}
					TextOut(newhdc, 62, 340, L"/52", 3);
				}
			}
		}

		SelectObject(newhdc, hOldFont);
		DeleteObject(hFont);
		SelectObject(newhdc, hOldBrush);
		DeleteObject(hBrush);
	}
	int inix = 200;
	int iniy = 75;

	//Draw chess board
	{
		for (int i = 0; i <= 19; i++)
		{
			for (int j = 0; j <= 19; j++)
			{
				if (((i + j) % 2) == 0)
				{
					DrawGreyBlock(hwnd, inix + i * 20, iniy + j * 20, newhdc);
				}
				else DrawDarkBlock(hwnd, inix + i * 20, iniy + j * 20, newhdc);
			}
		}
	}

	//Draw connected Road
	{
		for (int i = 0; i <= 19; i++)
		{
			for (int j = 0; j <= 19; j++)
			{
				int newi = i;
				int newj = j;
				int r;
				int g;
				int b;
				myGame.GetMapRGB(i, j, r, g, b);
				static short* road;
				road = (short *)myGame.GetRoad(i, j);
				for (int t = 0; t <= 11; t++)
				{
					if (road[t] == 0) break;
					else
					{
						switch(road[t])
						{
							case LEFT:
							{
								DrawColorLine(hwnd, (newj * 20 + 200) + 10, (newi * 20 + 75) + 10, ((newj - 1) * 20 + 200) + 10, (newi * 20 + 75) + 10, r, g, b, newhdc);
								newj--;
								break;
							}
							case UP:
							{
								DrawColorLine(hwnd, (newj * 20 + 200) + 10, (newi * 20 + 75) + 10, (newj * 20 + 200) + 10, ((newi - 1) * 20 + 75) + 10, r, g, b, newhdc);
								newi--;
								break;
							}
							case RIGHT:
							{
								DrawColorLine(hwnd, (newj * 20 + 200) + 10, (newi * 20 + 75) + 10, ((newj + 1) * 20 + 200) + 10, (newi * 20 + 75) + 10, r, g, b, newhdc);
								newj++;
								break;
							}
							case DOWN:
							{
								DrawColorLine(hwnd, (newj * 20 + 200) + 10, (newi * 20 + 75) + 10, (newj * 20 + 200) + 10, ((newi + 1) * 20 + 75) + 10, r, g, b, newhdc);
								newi++;
								break;
							}
						}
					}
				}
			}
		}
	}

	//Draw chess
	{
		//Draw chess color
		for (int i = 0; i <= 19; i++)
		{
			for (int j = 0; j <= 19; j++)
			{
				switch (myGame.GetMapNum(i, j))
				{
					case 1:
					{
						int r, g, b;
						myGame.GetMapRGB(i, j, r, g, b);
						DrawColorBlock(hwnd, 200 + j * 20, 75 + i * 20, r, g, b,newhdc);
						break;
					}
					case 0:
					{
						break;
					}
					default:
					{
						if (myGame.GetMapNum(i, j) >= 12)
							break;//ERROR
						if (myGame.GetMapNum(i, j) <= -1)
							break;

						int r, g, b;
						myGame.GetMapRGB(i, j, r, g, b);
						DrawColorCircle(hwnd, 200 + j * 20, 75 + i * 20, r, g, b, newhdc);
						break;
					}
				}
			}
		}
	}

	//Draw Number
	{
		for (int i = 0; i <= 19; i++)
		{
			for (int j = 0; j <= 19; j++)
			{
				if (myGame.GetMapNum(i, j) > 1)
				{
					int r, g, b;
					myGame.GetMapRGB(i, j, r, g, b);
					//反色
					GetOpsRGB(r, g, b);

					SetTextColor(newhdc, RGB(r, g, b));
					int tempmode = GetBkMode(newhdc);
					SetBkMode(newhdc, NULL);
					LOGFONT lf = { 0 };
					HFONT hFont;
					const char st[] = "黑体";
					strcpy((char *)(lf.lfFaceName), st);
					lf.lfWidth = 12;
					lf.lfHeight = 18;
					lf.lfWeight = FW_NORMAL;
					lf.lfCharSet = GB2312_CHARSET;
					lf.lfPitchAndFamily = 35;
					hFont = CreateFontIndirect(&lf);
					hOldFont = (HFONT)SelectObject(newhdc, hFont);
					//选择数字
					{
						int tnum = myGame.GetMapNum(i, j);
						if (tnum <= 9)
						{
							static char t[3];
							t[0]= char(tnum + 48);
							static wchar_t tw[3];
							AnsiToUnicode(t, tw);
							TextOutW(newhdc, 200 + j * 20 + 4, 75 + i * 20 + 2, tw, 1);

						}
						else // >=10
						{
							static char t[4];
							t[0] = '1';
							t[1] = char(tnum + 38);
							static wchar_t tw[4];
							AnsiToUnicode(t, tw);
							lf.lfWidth = 8;
							HFONT hFont = CreateFontIndirect(&lf);
							hOldFont = (HFONT)SelectObject(newhdc, hFont); 

							TextOutW(newhdc, 200 + j * 20 +2, 75 + i * 20 + 2, tw, 2);

							SelectObject(newhdc,hOldFont);
							DeleteObject(hFont);

						}
					}
					SetBkMode(newhdc, tempmode);

					SelectObject(newhdc, hOldFont);
					DeleteObject(hFont);
				}
			}
		}
	}

	//Draw chessboard Boarder
	DrawBigBox(hwnd, newhdc);

	BitBlt(GetDC(hwnd), 0, 0, 800, 600, newhdc, 0, 0, SRCCOPY);

	SelectObject(newhdc, hOldb);

	DeleteObject(hb);
	ReleaseDC(hwnd,hdc);
	DeleteDC(newhdc);

	return;
}

void DrawDarkBlock(HWND hwnd, int x, int y,HDC hdc)
{
	//HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	hPen = CreatePen(PS_NULL,0,0);
	//hdc = GetDC(hwnd);
	hBrush = CreateSolidBrush(RGB(190, 190, 190));

	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	Rectangle(hdc, x, y, x+21, y+21);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void DrawGreyBlock(HWND hwnd, int x, int y,HDC hdc)
{
	//HDC hdc;
	HBRUSH hBrush;
	HPEN hPen;
	hPen = CreatePen(PS_NULL, 0, 0);
	//hdc = GetDC(hwnd);
	hBrush = CreateSolidBrush(RGB(220, 220, 220));

	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	Rectangle(hdc, x, y, x + 21, y + 21);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void DrawColorBlock(HWND hwnd, int x, int y, int r, int g, int b, HDC hdc)
{
	HBRUSH hBrush;
	HPEN hPen;
	hPen = CreatePen(PS_NULL, 0, 0);
	hBrush = CreateSolidBrush(RGB(r, g, b));

	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	Rectangle(hdc, x, y, x + 21, y + 21);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void DrawColorCircle(HWND hwnd, int x, int y, int r, int g, int b, HDC hdc)
{
	HBRUSH hBrush;
	HPEN hPen;
	hPen = CreatePen(PS_NULL, 0, 0);
	hBrush = CreateSolidBrush(RGB(r, g, b));

	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	Ellipse(hdc, x, y, x + 21, y + 21);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void DrawColorLine(HWND hwnd, int x1, int y1, int x2, int y2, int r, int g, int b, HDC hdc)
{

	HPEN hPen;
	hPen = CreatePen(PS_SOLID, 5, RGB(r, g, b));
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
}

void DrawBigBox(HWND hwnd, HDC hdc)
{
	HPEN hPen;
	hPen = CreatePen(PS_SOLID, 5, RGB(0,0,0));
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	POINT apt[5] = { {198,73},{198,477},{602,477},{602,73},{198,73} };
	Polyline(hdc,apt,5);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
}

void GetOpsRGB(int& r, int& g, int& b)
{
	if ((r >= 125) && (r <= 150))
	{
		r = 0;
	}
	else if ((r < 125) && (r >= 100))
	{
		r = 255;
	}
	else r = 255 - r;

	if ((g >= 125) && (g <= 150))
	{
		g = 0;
	}
	else if ((g < 125) && (g >= 100))
	{
		g = 255;
	}
	else g = 255 - g; 

	if ((b >= 125) && (b <= 150))
	{
		b = 0;
	}
	else if ((b < 125) && (b >= 100))
	{
		b = 255;
	}
	else b = 255 - b;

	return;
}

wchar_t* AnsiToUnicode(char* szStr,wchar_t* pResult)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}