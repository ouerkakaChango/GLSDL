#include "Timer.h"



Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Start()
{
	start_ = clock();
}

double Timer::GetSec(bool reset)
{
	clock_t now = clock();
	double re = (now - start_) / CLOCKS_PER_SEC;
	if (reset)
	{
		start_ = now;
	}
	return re;
}