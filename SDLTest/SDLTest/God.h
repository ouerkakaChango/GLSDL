#pragma once
#include <SDL.h>

#include "Utility.h"
#include "SceneManager.h"
#include "GameConfig.h"

class Event;
class Object;
class Drawable;
class DrawCall;
class Material;
class ShaderImage;

class God
{
public:
	static God& GetInstance();

	void Init();
	void SetRenderer(SDL_Renderer* renderer) { renderer_ = renderer; }; //(deprecated)
	SDL_Renderer* GetRenderer() { return renderer_; }
	void BroadCast(Event* event);
	void BindEvent(std::string eventName,Object* object);
	void Update(float deltaTime);
	void AddPostDrawable(Drawable* drawable); //(deprecated)
	void ChangePostDrawable(Drawable* oriDrawable, Drawable* newDrawable); //(deprecated)
	Material* CloneDefaultMaterial();

	SceneManager sceneManager_;
	GameConfig gameConfig_;
	ShaderImage* blackBackground_{ nullptr };
	std::vector<DrawCall*> drawcalls_;

private:
	God();

	MapVector<Object*> eventMapVector_;
	std::list<Drawable*> postDrawables_;
	SDL_Renderer* renderer_;
	Material* defaultMaterial_{nullptr};
};

#define GOD God::GetInstance()