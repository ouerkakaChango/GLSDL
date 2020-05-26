#pragma once
#include "Utility.h"
class GameUtility
{
public:
	GameUtility();
	~GameUtility();
};

bool saveScreenshot(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);