#pragma once
#include "Utility.h"

class Object;

typedef std::pair<ParamPack, std::string> taskCondition_;

class Task
{
public:
	Task(const std::string& taskName)
		:taskName_(taskName)
	{}
	bool operator==(const Task& task) const;
	void AddCondition(ParamPack param, const std::string& eventName);

	std::vector<taskCondition_ > conditionParams_;
	std::string taskName_;
	Func taskCompleteFunc_;

	friend class TaskManager;
};

class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	void Check(ParamPack param, const std::string& eventName);

	std::vector<Task> tasks_;
protected:
	void CheckTaskSatisfied(Task& task);

	std::vector<taskCondition_> watchedConditions_;
	std::map<Object*, StringMap<ParamPack>> objBasedConditions_;
};

