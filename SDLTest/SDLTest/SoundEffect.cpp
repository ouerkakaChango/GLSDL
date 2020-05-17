#include "SoundEffect.h"

#include "Debug.h"

SoundEffect::SoundEffect(const Path& path)
{
	if (!path.empty())
	{
		Load(path);
	}
}


SoundEffect::~SoundEffect()
{
}

bool SoundEffect::Load(const Path& path)
{
	sound_ = Mix_LoadWAV(path.c_str());
	if (sound_ == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void SoundEffect::Play(bool bLoop)
{
	if (sound_ != NULL)
	{
		if (bLoop)
		{
			Mix_PlayChannel(-1, sound_, -1);
		}
		else
		{
			Mix_PlayChannel(-1, sound_, 0);
		}
	}
}

void SoundEffect::Play(float duration)
{
	if (sound_ != NULL)
	{
		Mix_PlayChannelTimed(-1, sound_, -1, duration * 1000);
	}
}

void SoundEffect::Stop()
{
	if (sound_ != NULL)
	{
		Mix_FadeOutChannel(-1,0);
	}
}

void SoundEffect::Stop(float duration)
{
	if (sound_ != NULL)
	{
		Mix_FadeOutChannel(-1, duration*1000);
	}
}