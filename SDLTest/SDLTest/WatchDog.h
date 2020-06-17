#pragma once
//based on SDL
//watch informations for the game
//such as performance data...
//--Chango
#include "Utility.h"
#include "Tree.h"

class ProfilerNode :public AutoTree<ProfilerNode>
{
public:
	ProfilerNode() = default;
	explicit ProfilerNode(const std::string& name, float duration, bool bRoot = false);
protected:
	bool bRoot_{ false };
	float duration_{ 0.0f };
	std::string name_{ "[empty]" };
	friend class AutoProfiler;
};

class AutoProfiler
{
public:
	AutoProfiler(const std::string& tagName);
	~AutoProfiler();
	static shared_ptr<ProfilerNode> dataRoot_;
protected:
	static ProfilerNode* nowFather_;
	shared_ptr <ProfilerNode> data_{nullptr};
	std::string tagName_;
	Uint32 startTime_{ 0 }, endTime_{0};
};

#define Profile(tagName) AutoProfiler autoProfiler(tagName);