#include "TaskManager.h"

bool Task::operator==(const Task& task) const
{
	if (conditionParams_.size() != task.conditionParams_.size())
	{
		return false;
	}
	for (unsigned i = 0; i < conditionParams_.size(); i++)
	{
		auto& c1 = conditionParams_[i];
		auto& c2 = task.conditionParams_[i];
		if (c1.first == c2.first && c1.second == c2.second)
		{
			//ok
		}
		else
		{
			return false;
		}
	}
	return true;
}

void Task::AddCondition(ParamPack param, const std::string& eventName)
{
	conditionParams_.push_back(std::make_pair(param, eventName));
}

TaskManager::TaskManager()
{
}


TaskManager::~TaskManager()
{
}

void TaskManager::Check(ParamPack param, const std::string& eventName)
{
	if (param.objPtr_ != nullptr)
	{//obj based condition
		objBasedConditions_[param.objPtr_].map_[eventName] = param;
	}
	for (auto& task : tasks_)
	{
		CheckTaskSatisfied(task);
	}
}

void TaskManager::CheckTaskSatisfied(Task& task)
{
	bool bComplete = true;
	for (auto& condition : task.conditionParams_)
	{
		if (condition.first.objPtr_ != nullptr)
		{//obj based condition
			if (objBasedConditions_[condition.first.objPtr_].map_[condition.second] != condition.first)
			{
				bComplete = false;
				break;
			}
		}
	}
	if (bComplete)
	{
		task.taskCompleteFunc_();
		sure(STL_Remove(tasks_,task));
	}
}