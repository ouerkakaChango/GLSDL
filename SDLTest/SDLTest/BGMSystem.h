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
	void ChangeBGM(const Path& filePath);

protected:
	Mix_Music* GetMixMusic(const Path& filePath);

	std::map<Path, Mix_Music*> musics_;
	Mix_Music* nowMusic_;
};

