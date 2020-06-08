#include "God.h"

#include "Event.h"
#include "Object.h"
#include "SceneManager.h"
#include "Drawable.h"
#include "Material.h"
#include "Image.h"
#include "ShaderImage.h"
#include "Timeline.h"
#include "BGMSystem.h"

#include "Debug.h"

God& God::GetInstance()
{
	static God god;
	return god;
}

God::God()
{
	timeline_ = new Timeline;
	timeline_->Reset();
}

void God::InitDefaultAsset()
{
	defaultMaterial_ = new Material;
	defaultMaterial_->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quad.fs");

	Image* blackBGImg = new Image(windowW_, windowH_);
	blackBGImg->ReadFile("D:/HumanTree/ResEngine/black.png");
	blackBGImg->SetPosition(windowW_ / 2, windowH_ / 2);
	blackBackground_ = new ShaderImage(blackBGImg);
	blackBackground_->name_ = "blackBackground_";
}

void God::BroadCast(Event* event)
{
	std::vector<Object*>* results = nullptr;
	if (eventMapVector_.Find(event->name_, results))
	{
		for (auto& object : *results)
		{
			object->HandleEvent(event);
		}
	}
}

void God::BindEvent(std::string eventName, Object* object)
{
	eventMapVector_.map_[eventName].push_back(object);
}

void God::AddPostDrawable(Drawable* drawable)
{
	if (drawable != nullptr)
	{
		drawable->SetActive(true);
		postDrawables_.push_back(drawable);
	}
}

void God::ChangePostDrawable(Drawable* oriDrawable, Drawable* newDrawable)
{
	oriDrawable->SetActive(false);
	newDrawable->SetActive(true);
	postDrawables_.remove(oriDrawable);
	postDrawables_.push_back(newDrawable);
}

void God::Update(float deltaTime)
{
	timeline_->Update(deltaTime, watchDog_);
	sceneManager_.Update(deltaTime, watchDog_);
	for (auto& drawable : postDrawables_)
	{
		drawable->Render();
	}
	watchDog_.Watch("postDrawables");
}

Material* God::CloneDefaultMaterial()
{
	return defaultMaterial_->Clone();
}

void God::SetTimer(float delay, Func function)
{

	timeline_->AddAction(timeline_->Now()+delay, function);
}

void God::GetDrawcalls()
{
	passiveDrawcalls_.clear();
	postDrawcalls_.clear();
	
	for (auto& drawable : drawcallDrawables_)
	{
		drawable->GetDrawcall();
	}
}

void God::TurnOn()
{
	InitGameData();
	InitSDLGLWindow();
	InitDefaultAsset();
}

void God::InitGameData()
{
	gameConfig_.Load();
	windowW_ = gameConfig_.Get<int>("windowWidth");
	windowH_ = gameConfig_.Get<int>("windowHeight");
}

void God::InitSDLGLWindow()
{
	//--- init SDLGL window
	SDL_Init(SDL_INIT_EVERYTHING);
	// Create an application window with the following settings:
	window_ = SDL_CreateWindow(
		gameConfig_["windowTitle"].c_str(),                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		windowW_,                               // width, in pixels
		windowH_,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);
	// Check that the window was successfully created
	if (window_ == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		abort();
	}
	//___ init SDLGL window

	//--- (deprecated) SDLrenderer init
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
	if (renderer_ == nullptr) {
		SDL_DestroyWindow(window_);
		std::cout << "SDL_CreateRender Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		abort();
	}
	GOD.SetRenderer(renderer_);

	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);
	//___ (deprecated) SDLrenderer init

	//--- SDL mixer init
	bgmSystem_ = new BGMSystem;
	//___ SDL mixer init

	//--- init OpenGL for SDL
	InitSDLOpenGL();
	//___ init OpenGL for SDL
}

void God::InitSDLOpenGL()
{
	//??? gl版本
	// core version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef GL_CORE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
	//由于代码中还有“过时”，或者不在PROFILE_CORE中的写法，所以使用兼容性版本
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#endif

	//Create context
	auto gContext = SDL_GL_CreateContext(window_);
	if (gContext == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		abort();
	}
	else
	{
		//Initialize GLEW
		//glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}
}