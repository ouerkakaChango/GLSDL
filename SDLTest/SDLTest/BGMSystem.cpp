#include "BGMSystem.h"



BGMSystem::BGMSystem()
{
}


BGMSystem::~BGMSystem()
{
}

void BGMSystem::SetBGM(const Path& filePath)
{
	gMusic_ = Mix_LoadMUS(filePath.c_str());
	if (gMusic_ == NULL)
	{
		abort();
	}
	Mix_PlayMusic(gMusic_, -1);
}

void BGMSystem::StopBGM()
{
	Mix_HaltMusic();
}