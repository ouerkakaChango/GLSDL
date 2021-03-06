#pragma once
#include <SDL.h>

#include "Utility.h"
#include "GraphicDefs.h"
#include "SceneManager.h"
#include "WatchDog.h"
#include "GameConfig.h"
#include "TaskManager.h"

#define GL_CORE

class Event;
class Object;
class Drawable;
class DrawCall;
class Material;
class ShaderImage;
class Timeline;
class BGMSystem;
class Cursor;
//??? debug
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
	Material* CloneDefaultMaterial(TextureFilterType texFilterType);
	void GetDrawcalls();
	void ElapseGlobalK(float deltaTime);
	float GlobalK() const { return globalK_; }
	//--- Task related
	void TaskNotify(ParamPack param, const std::string& eventName);
	//___ Task related

	SceneManager sceneManager_;
	TaskManager taskManager_;
	GameConfig gameConfig_;
	std::vector<DrawCall*> drawcalls_;

	std::vector<Drawable*> drawcallDrawables_;

	std::vector<DrawCall*> prePassiveDrawcalls_;	//reset sceneRT in FBO
	std::vector<DrawCall*> passiveDrawcalls_;
	std::vector<DrawCall*> preSceneColorDrawcalls_;
	std::vector<DrawCall*> sceneColorDrawcalls_;
	std::vector<DrawCall*> postDrawcalls_;
	std::vector<DrawCall*> afterPostDrawcalls_;		//such as Cursor

	int windowW_;
	int windowH_;
	BGMSystem* bgmSystem_{ nullptr };
	SDL_Window* window_;
	SDL_Renderer* renderer_;//(deprecated)

	Cursor* cursor_{ nullptr };

	//default asset
	ShaderImage* fastBlackCurtain_{ nullptr };
	ShaderImage* fadeBlackCurtain_{ nullptr };
	Material* defaultRTMaterial_{ nullptr };

	//??? debug
	Image* testImg_{ nullptr };
	bool bOldDraw_{ true };

private:
	God();
	void InitGameData();
	void InitSDLGLWindow();
	void InitSDLOpenGL();
	void InitDefaultAsset();

	MapVector<Object*> eventMapVector_;
	std::list<Drawable*> postDrawables_;	//(deprecated,old draw)
	Material* defaultMaterial_{nullptr};
	Timeline* timeline_{nullptr};

	float globalK_;
};

#define GOD God::GetInstance()