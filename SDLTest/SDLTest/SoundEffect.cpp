#include "SoundEffect.h"

#include "God.h"
#include "FuncAction.h"
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

void SoundEffect::PlayWithFadeOut(float fadeStart, float fadeEnd)
{
	Mix_PlayChannel(-1, sound_, 0);
	static float dur = fadeEnd - fadeStart;
	//由于是延时调用，必须用生命周期还存在的static变量
	Func fade = [&]()
	{
		Mix_FadeOutChannel(-1, dur * 1000);
	};
	GOD.SetTimer(fadeStart, fade);
}