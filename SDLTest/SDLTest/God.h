#pragma once
#include <SDL.h>

#include "Utility.h"
#include "SceneManager.h"
#include "GameConfig.h"

class Event;
class Object;
class Drawable;
class DrawCall;

class God
{
public:
	static God& GetInstance();

	//??? 以后renderer自建
	void SetRenderer(SDL_Renderer* renderer) { renderer_ = renderer; };
	SDL_Renderer* GetRenderer() { return renderer_; }
	void BroadCast(Event* event);
	void BindEvent(std::string eventName,Object* object);
	void Update(float deltaTime);
	void AddPostDrawable(Drawable* drawable);
	void ChangePostDrawable(Drawable* oriDrawable, Drawable* newDrawable);

	SceneManager sceneManager_;
	GameConfig gameConfig_;
	std::vector<DrawCall*> drawcalls_;

private:
	God();

	SDL_Renderer* renderer_;
	MapVector<Object*> eventMapVector_;
	std::list<Drawable*> postDrawables_;
};

#define GOD God::GetInstance()