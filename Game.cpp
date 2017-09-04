#include "Game.h"

Game::Game()
{

	return;
}

Game::~Game()
{
}

void Game::ChangeStatus(int next)//状态改变
{
	Status = next; 
	if (Status == MainState)
	{
		Gametime.End();
	}
	if (Status == PlayingState)
	{
		Gametime.Start();
	}
	if (Status == StopState)
	{
		Gametime.Pause();
	}
	if (Status == GameOver)
	{
		Gametime.Pause();
	}
}

int Game::InitializeGame(HWND thwnd)
{
	Status = MainState;
	hwnd = thwnd;
	//MyScreen;
	return 1;
}

void Game::ReturnTime(wchar_t * chartime)
{
	double t = Gametime.Gettotaltimesecond();
	int i = int(t*100);
	int mod = 10000;
	for (int f = 0; f <= 5; f++)
	{
		if (f == 3) continue;
		else {
			switch (i / mod)
			{
			case 0:
			{
				chartime[f] = L'0';
				break;
			}
			case 1:
			{
				chartime[f] = L'1';
				break;
			}
			case 2:
			{
				chartime[f] = L'2';
				break;
			}
			case 3:
			{
				chartime[f] = L'3';
				break;
			}
			case 4:
			{
				chartime[f] = L'4';
				break;
			}
			case 5:
			{
				chartime[f] = L'5';
				break;
			}
			case 6:
			{
				chartime[f] = L'6';
				break;
			}
			case 7:
			{
				chartime[f] = L'7';
				break;
			}
			case 8:
			{
				chartime[f] = L'8';
				break;
			}
			case 9:
			{
				chartime[f] = L'9';
				break;
			}
			}
			i = i - (i / mod)*mod;
			mod /= 10;
			
		}
	}
	return ;
}

int Game::GetStatus()
{
	return Status;
}

int Game::GetMapNum(int i, int j) 
{
	int t;
	t = GameMap.GetDotNum(i, j);
	return t;
}

void Game::GetMapRGB(int i, int j, int & r, int & g, int & b)
{
	GameMap.GetDotRGB(i, j, r, g, b);
	return;
}

void Game::Click(int i, int j)
{
	GameMap.DealClick(i,j);
	return;
}

void Game::PlayMouseMove(int i, int j)
{
	//如果连接成功
	if (GameMap.DealChangeRoad(i, j) == 1)
	{
		Finished++;
	}
	return;
}

void* Game::GetRoad(int i, int j)
{
	return GameMap.GetRoad(i, j);
}

int Game::GetFinished()
{
	return Finished;
}

bool Game::IsTiming()
{
	if (Gametime.GetStartTime() == 0.0)
	{
		return false;
	}
	else return true;
}

