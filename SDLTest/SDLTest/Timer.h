#pragma once

#include<cstdlib>
#include<ctime>

class Timer
{
public:
	Timer();
	~Timer();
	void Start();
	double GetSec(bool reset = false);
protected:
	clock_t start_;
};

