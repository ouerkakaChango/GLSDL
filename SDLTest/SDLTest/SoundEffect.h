#pragma once

#include <SDL_mixer.h>

#include "Utility.h"

class SoundEffect
{
public:
	SoundEffect(const Path& path="");
	~SoundEffect();
	bool Load(const Path& path);
	void Play(bool bLoop=false);
	void Play(float duration);
	void Stop();
	void Stop(float duration);
protected:
	Mix_Chunk* sound_ = NULL;
};

