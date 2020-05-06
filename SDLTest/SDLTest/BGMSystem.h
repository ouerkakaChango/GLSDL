#pragma once

#include <SDL_mixer.h>

#include "Utility.h"

class BGMSystem
{
public:
	BGMSystem();
	~BGMSystem();
	void SetBGM(const Path& filePath);
	void StopBGM();

protected:
	Mix_Music* gMusic_;
};

