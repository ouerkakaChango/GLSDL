#include "SoundAction.h"

#include "SoundEffect.h"

SoundAction::~SoundAction()
{
}

void SoundAction::DoAction()
{
	sound_->Play();
}