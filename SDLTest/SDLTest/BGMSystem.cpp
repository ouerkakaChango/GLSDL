#include "BGMSystem.h"
#include "SoundEffect.h"


BGMSystem::BGMSystem()
{
}


BGMSystem::~BGMSystem()
{
}

void BGMSystem::SetBGM(const Path& filePath)
{
	nowMusic_ = GetMixMusic(filePath);
	sure(nowMusic_ != nullptr);
	Mix_PlayMusic(nowMusic_, -1);
}

void BGMSystem::StopBGM()
{
	Mix_HaltMusic();
}

void BGMSystem::ChangeBGM(const Path& filePath)
{
	StopBGM();
	SetBGM(filePath);
}

Mix_Music* BGMSystem::GetMixMusic(const Path& filePath)
{
	Mix_Music* re = nullptr;
	if (musics_[filePath] == nullptr)
	{
		re = Mix_LoadMUS(filePath.c_str());
		if (re == NULL)
		{
			abort();
		}
		musics_[filePath] = re;
	}
	else
	{
		re = musics_[filePath];
	}
	return re;
}

void BGMSystem::PlayChunk(const Path& filePath)
{
	SoundEffect temp(filePath);
	temp.Play();
}

void BGMSystem::PlayChunk(const Path& filePath, float fadeStart, float fadeEnd)
{
	SoundEffect temp(filePath);
	temp.PlayWithFadeOut(fadeStart,fadeEnd);
}