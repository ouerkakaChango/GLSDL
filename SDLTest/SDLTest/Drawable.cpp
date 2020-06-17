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
	switch (dcChannel_)
	{
	case DrawCall_Passive:
		GOD.passiveDrawcalls_.push_back(dc_);
		break;
	case DrawCall_Post:
		GOD.postDrawcalls_.push_back(dc_);
		break;
	case DrawCall_AfterPost:
		GOD.afterPostDrawcalls_.push_back(dc_);
		break;
	default:
		break;
	}
}