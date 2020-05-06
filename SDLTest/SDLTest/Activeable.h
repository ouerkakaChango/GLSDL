#pragma once
class Activeable
{
public:
	Activeable();
	~Activeable();
	virtual void SetActive(bool active) { bActive_ = active; }
	virtual bool GetActive();
protected:
	bool bActive_{ false };
};

