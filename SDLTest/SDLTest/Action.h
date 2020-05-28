#pragma once

class Action
{
public:
	void SetTime(float time) { time_ = time; }
	virtual void Check(float time);
	virtual void DoAction()=0;
protected:
	float time_;
	float errorAccept_{ 0.1f };//(deprecated)
	bool bDone_{ false };
};