#include "WatchDog.h"

shared_ptr<ProfilerNode> AutoProfiler::dataRoot_ = make_shared<ProfilerNode>("[root]",0.0f, true);
ProfilerNode* AutoProfiler::nowFather_{nullptr};

ProfilerNode::ProfilerNode(const std::string& name, float duration, bool bRoot)
	:name_(name),
	duration_(duration),
	bRoot_(bRoot) 
{

}

AutoProfiler::AutoProfiler(const std::string& tagName)
	:tagName_(tagName)
{
	data_ = make_shared<ProfilerNode>(tagName, 0.0f);
	if (nowFather_ == nullptr)
	{
		dataRoot_->AddChild(data_);
		nowFather_ =  data_.get();
	}
	else
	{
		nowFather_->AddChild(data_);
		nowFather_ = data_.get();
	}
	startTime_ = SDL_GetTicks();
}

AutoProfiler::~AutoProfiler()
{
	if (nowFather_->father_ != nullptr)
	{
		nowFather_ = nowFather_->father_;
	}
	endTime_ = SDL_GetTicks();
	data_->duration_ = (endTime_ - startTime_) / 1000.0f;
}