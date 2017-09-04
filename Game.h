#pragma once

#include <windows.h>
#include "clock.h"
#include "Map.h"
#define MainState 0
#define PlayingState 1
#define StopState 2
#define GameOver 3

class Game
{

private:

	int Status;
	HWND hwnd;
	Clock Gametime;
	Map GameMap;
	int Finished;
public:


	Game();
	~Game();
	void ChangeStatus(int);//改变状态
	int GetStatus();
	int InitializeGame(HWND);//游戏初始化
	void ReturnTime(wchar_t*);
	int GetMapNum(int i, int j);
	void GetMapRGB(int i, int j,int& r, int& g, int& b);
	void Click(int i, int j);
	void PlayMouseMove(int i,int j);
	void* GetRoad(int i, int j);
	int GetFinished();
	bool IsTiming();
};