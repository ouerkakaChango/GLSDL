#pragma once
#include <SDL.h>

#include "Utility.h"
#include "SceneManager.h"
#include "WatchDog.h"
#include "GameConfig.h"

#define GL_CORE

class Event;
class Object;
class Drawable;
class DrawCall;
class Material;
class ShaderImage;
class Timeline;
class BGMSystem;
//???
class Image;

class God
{
public:
	static God& GetInstance();

	void TurnOn();
	void SetRenderer(SDL_Renderer* renderer) { renderer_ = renderer; }; //(deprecated)
	SDL_Renderer* GetRenderer() { return renderer_; }
	void BroadCast(Event* event);
	void BindEvent(std::string eventName,Object* object);
	void Update(float deltaTime);
	void SetTimer(float delay, Func function);
	void AddPostDrawable(Drawable* drawable); //(deprecated)
	void ChangePostDrawable(Drawable* oriDrawable, Drawable* newDrawable); //(deprecated)
	Material* CloneDefaultMaterial();
	void GetDrawcalls();

	SceneManager sceneManager_;
	GameConfig gameConfig_;
	ShaderImage* blackBackground_{ nullptr };
	std::vector<DrawCall*> drawcalls_;

	std::vector<Drawable*> drawcallDrawables_;
	std::vector<DrawCall*> passiveDrawcalls_;
	std::vector<DrawCall*> postDrawcalls_;

	int windowW_;
	int windowH_;
	BGMSystem* bgmSystem_{ nullptr };

	//???
	WatchDog watchDog_;
	Image* testImg_{ nullptr };

	//???
	SDL_Window* window_;
	SDL_Renderer* renderer_;//(deprecated)

private:
	God();
	void InitGameData();
	void InitSDLGLWindow();
	void InitSDLOpenGL();
	void InitDefaultAsset();

	MapVector<Object*> eventMapVector_;
	std::list<Drawable*> postDrawables_;
	Material* defaultMaterial_{nullptr};
	Timeline* timeline_{nullptr};
};

#define GOD God::GetInstance()