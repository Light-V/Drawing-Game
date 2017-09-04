#include "clock.h"

Clock::Clock()
{
	lastStartTime = -1;
	acTime = 0;
	return;
}

Clock::~Clock()
{
	return;
}

void Clock::Start()
{
	lastStartTime = clock()/CLOCKS_PER_SEC;
	return;
}

void Clock::Pause()
{
	acTime += (clock() / CLOCKS_PER_SEC - lastStartTime);
}

void Clock::End()
{
	lastStartTime = -1;
	acTime = 0;
	return;
}

double Clock::Gettotaltimesecond()
{
	if (lastStartTime == -1) 
	{ 
		return 0; //ERROR
	}
	else return (acTime + clock() / CLOCKS_PER_SEC - lastStartTime);
}

double Clock::GetStartTime()
{
	return lastStartTime;
}
