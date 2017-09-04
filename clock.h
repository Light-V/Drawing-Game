#pragma once
#include <time.h>

class Clock
{
private:
	double lastStartTime;
	double acTime;
public:
	Clock();
	~Clock();
	void Start();
	void Pause();
	void End();
	double Gettotaltimesecond();
	double GetStartTime();
};