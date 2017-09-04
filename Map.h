#pragma once
#define LEFT 1
#define UP 2
#define RIGHT 3
#define DOWN 4
struct Postion
{
	short x;
	short y;
};
struct RGB
{
	short r;
	short g;
	short b;
};
struct DotInf
{
	short Road[12];
	Postion Destination;
	RGB color;
	int Number;
};

class Map
{
private:
	DotInf MapInf[20][20];
	Postion Focus;
	void SetFocus(int i,int j);
	void LoseFocus();
public:
	Postion Lord;
	Map();
	~Map();
	void Initial();
	int DealChangeRoad(int i,int j);
	void ChangeDes(int i,int j);
	int GetDotNum(int i,int j);
	void ResetDotNum(int i, int j);
	void GetDotRGB(int i,int j,int& r, int& g, int& b);
	void DealClick(int i,int j);
	void* GetRoad(int i, int j);
};