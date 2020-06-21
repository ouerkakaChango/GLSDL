#include "Drawable.h"

#include "God.h"
#include "DrawCall.h"

Drawable::Drawable()
{
}


Drawable::~Drawable()
{
}

void Drawable::SetSceneRT(RenderTexture* sceneRT)
{
	sceneRT_ = sceneRT;
	dc_->SetRenderTexture(sceneRT_);
}

void Drawable::SetDrawCallChannel(DrawCallChannel newChannel)
{
	dcChannel_ = newChannel;
}

void Drawable::CommitDrawCall()
{
	if (dcChannel_ == DrawCall_Passive)
	{
		GOD.passiveDrawcalls_.push_back(dc_);
	}
	else if (dcChannel_ == DrawCall_PreSceneColor)
	{
		GOD.preSceneColorDrawcalls_.push_back(dc_);
	}
	else if (dcChannel_ == DrawCall_SceneColor)
	{
		GOD.sceneColorDrawcalls_.push_back(dc_);
	}
	else if (dcChannel_ == DrawCall_Post)
	{
		GOD.postDrawcalls_.push_back(dc_);
	}
	else if (dcChannel_ == DrawCall_AfterPost)
	{
		GOD.afterPostDrawcalls_.push_back(dc_);
	}
	else
	{
		abort();
	}
}