#define XCMAIN
//#define XCTEST
#define GL_CORE

#ifdef XCMAIN
#include <SDL_image.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
//??? 头文件整合
#include "GameConfig.h"
#include "Image.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneTransition.h"
#include "God.h"
#include "Button.h"
#include "Events.h"
#include "UsualEffects.h"
#include "SoundEffect.h"
#include "Cursor.h"
#include "Touchable.h"
#include "BGMSystem.h"
//???
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DrawCall.h"
#include "ShaderImage.h"
#include "Timeline.h"
#include "Pass.h"
//???
#include "WatchDog.h"

#include "GameUtility.h"
#include "Debug.h"

DrawCall gDC;
VertexBuffer gVB;
IndexBuffer gIB;
bool bOldDraw = true;

void GLRender()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	auto& gWatchDog = GOD.watchDog_;
	if (bOldDraw)
	{
		//??? 把d3d画的背景作为参数传进来
		SDL_Surface* Surface = IMG_Load("D:/22.bmp");

		gDC.material_->UpdateParam("tex", Surface);
		gDC.Do();
		SDL_FreeSurface(Surface);
	}
	gWatchDog.Watch("OldBackground");
	auto& god = GOD;
	GOD.GetDrawcalls();
	for (auto& dc : GOD.drawcalls_) //(deprecated)
	{
		dc->Do();
	}
	gWatchDog.Watch("dc");

	for (unsigned i = 0; i < GOD.passiveDrawcalls_.size(); i++)
	{
		GOD.passiveDrawcalls_[i]->Do();
	}
	gWatchDog.Watch("passiveDC");
	DrawCall *nextDC;
	bool bInSimple = false;
	for (unsigned i=0;i< GOD.postDrawcalls_.size();i++)
	{
		DrawCall* dc = GOD.postDrawcalls_[i];
		if (i < GOD.postDrawcalls_.size() - 1)
		{
			nextDC = GOD.postDrawcalls_[i + 1];
		}
		else
		{
			nextDC = nullptr;
		}
		if (dc == nextDC &&!bInSimple)
		{
			dc->SimpleDoStart();
			bInSimple = true;
		}
		else if (bInSimple && dc != nextDC)
		{
			dc->SimpleDoEnd();
			bInSimple = false;
		}
		else if (bInSimple)
		{
			dc->SimpleDo();
		}
		else
		{
			dc->Do();
		}
	}
	gWatchDog.Watch("postDC");
}

bool initOldDraw()
{
	auto oldBackgroundMaterial = new Material;

	if (!oldBackgroundMaterial->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quadBMP.fs"))
	{
		return false;
	}

	gDC.SetMaterial(oldBackgroundMaterial);
	gVB.name_ = "backgroundVB";
	gDC.SetVB(&gVB);
	gDC.SetIB(&gIB);

	//Initialize clear color
	glClearColor(0.f, 1.f, 0.f, 1.f);

	gVB.InitQuad(1.0f);
	gIB.InitQuad(gVB.vao_);
	/////////////////////

	return true;
}

int main(int argc, char* argv[]) {
	GOD.TurnOn();     

	if (!initOldDraw())
	{
		abort();
	}
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	auto windowW = GOD.windowW_;
	auto windowH = GOD.windowH_;

	auto& sceneMgr = GOD.sceneManager_;
	SoundEffect* sound1 = new SoundEffect;
	if (!sound1->Load("D:/HumanTree/bird.wav"))
	{
		abort();
	}

	auto scene1 = sceneMgr.InsertScene();
	Image bgImg(500, 500);
	if (bgImg.Load("D:/HumanTree/1.bmp"))
	{
		bgImg.SetPosition(windowW / 2, windowH / 2 - 100);
		scene1->AddDrawable(&bgImg);
	}

	//### button
	Image buttonImg(93 / 2, 83 / 2);
	if (buttonImg.Load("D:/HumanTree/2.bmp"))
	{
		buttonImg.SetPosition(windowW / 2 - 15, windowH / 2 + 150);
	}

	Button button;
	button.SetImage(&buttonImg);
	EventHandler func = [&](Event* event)
	{
		LOG("next!");
		sceneMgr.ToNext();
	};
	button.BindEventHandler("LMB_Down", func);
	scene1->AddDrawable(&button);

	SceneTransition* transition = new SceneTransition("fadeIn", 10);
	//???
	transition->SetActive(true);
	sceneMgr.AddTransition(Int<2>(-1, 0), transition);
	//////////////////////////////////////////////////////////////////////
	// scene2
	auto scene2 = sceneMgr.InsertScene("D:/HumanTree/6.bmp");

	Image* leg = new Image(700, 500);
	if (leg->Load("D:/HumanTree/10.png"))
	{
		leg->SetPosition(windowW / 2, windowH - leg->GetHeight() / 2);
		EffectFadeIn* eff = new EffectFadeIn(leg, 5);
		scene2->Show(leg, 8, eff);
	}
	//scene2的自动转幕
	scene2->SetAutoEnd(16);

	SceneTransition* transition2 = new SceneTransition("fadeOutIn", 10);
	sceneMgr.AddTransition(Int<2>(0, 1), transition2);

	///////////////////////////////////////////////////////////////////////
	//scene3 
	auto scene3 = sceneMgr.InsertScene("D:/HumanTree/10.bmp");

	SceneTransition* transition3 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(1, 2), transition3);

	SoundEffect* catSound = new SoundEffect;
	if (!catSound->Load("D:/HumanTree/cat.wav"))
	{
		abort();
	}
	scene3->AddSound(catSound, 4);
	scene3->SetAutoEnd(7);
	/////////////
	//鼠标
	Cursor cursor("D:/HumanTree/c7.png", 50, 50);
	///////////////////////////////////////////////////////////////////////
	//scene4
	auto scene4 = sceneMgr.InsertScene("D:/HumanTree/15.bmp");

	Image* cat = new Image(350, 450);
	if (cat->Load("D:/HumanTree/cat_Simple2.png"))
	{
		cat->SetPosition(windowW / 2 + 650, windowH / 2 + 200);
		scene4->AddDrawable(cat);
	}

	Touchable touch;
	touch.SetImage(cat);
	EventHandler func2 = [&](Event* event)
	{
		if (!cursor.IsDefaultImg())
		{
			cursor.SetDefaultImage();
		}
		catSound->Play();
		sceneMgr.ToNext();
	};
	touch.BindEventHandler("LMB_Down", func2);
	touch.BindEventHandler("Mouse_Move");
	touch.beginOverlapFunc_ = [&]()
	{
		if (cursor.IsDefaultImg())
		{
			cursor.ChangeImage("D:/HumanTree/c2.png");
		}
	};
	touch.endOverlapFunc_ = [&]()
	{
		if (!cursor.IsDefaultImg())
		{
			cursor.SetDefaultImage();
		}
	};
	scene4->AddDrawable(&touch);

	SceneTransition* transition4 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(2, 3), transition4);
	//////////////////////////////////////////////////
	//BGM
	BGMSystem& bgm=* GOD.bgmSystem_;
	bgm.SetBGM("D:/HumanTree/1.wav");
	///////////////////////////////////////////////////////////////////////
	//scene5
	auto scene5 = sceneMgr.InsertScene("D:/HumanTree/16.bmp");

	SoundEffect w1("D:/HumanTree/warning11.wav");
	SoundEffect w2("D:/HumanTree/warning12.wav");
	SoundEffect w3("D:/HumanTree/warning13.wav");

	Image* red = new Image(GOD.windowW_, GOD.windowH_);
	if (red->Load("D:/HumanTree/red.png"))
	{
		red->SetPosition(windowW / 2, windowH / 2);
	}
	EffectFlash redFlash(red, 1);

	Func changeBGMFunc = [&]()
	{
		bgm.ChangeBGM("D:/HumanTree/DeeperThanSpace.wav");
		w1.Play(true);
		w2.Play(true);
		w3.Play(true);
		sceneMgr.AddCrossEffect(&redFlash);
	};
	//红色警报事件
	scene5->AddCustomAction(4, changeBGMFunc);
	SoundEffect cat2("D:/HumanTree/cat2.wav");
	scene5->AddSound(&cat2, 6);
	scene5->SetAutoEnd(7);

	SceneTransition* transition5 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(3, 4), transition5);

	///////////////////////////////////////////////////////////////////////
	//scene6
	auto scene6 = sceneMgr.InsertScene("D:/HumanTree/18.bmp");
	scene6->SetAutoEnd(1.5);
	SceneTransition* transition6 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(4, 5), transition6);

	///////////////////////////////////////////////////////////////////////
	//scene7 

	auto scene7 = sceneMgr.InsertScene("D:/HumanTree/19.bmp");
	
	SceneTransition* transition7 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(5, 6), transition7);

	Func stopWarningFunc = [&]()
	{;
		w1.Stop(3);
		w2.Stop(3);
		w3.Stop(3);
		redFlash.SetActive(false);
	};

	Func heavyDropFunc = [&]()
	{
		bgm.StopBGM();
		bgm.PlayChunk("D:/HumanTree/HeaveFall.wav",3,4);
		bOldDraw = false;
	};

	//关音效，关红闪特效
	scene7->AddCustomAction(0.1f, stopWarningFunc);

	Material* vortexMat = new Material;
	if (!vortexMat->CompileShader("D:/HumanTree/code/vortex.vs", "D:/HumanTree/code/vortex2.fs"))
	{
		abort();
	}
	vortexMat->SetBlendType(Blend_Alpha);
	Image* vortexImage = new Image(GOD.windowW_, GOD.windowH_);
	vortexImage->SetPosition(800, 450);
	vortexImage->ReadFile("D:/HumanTree/vortex2.png");

	EffectShaderParam* paramEffect = new EffectShaderParam;
	paramEffect->Bind(vortexMat, "maxScale");
	paramEffect->AddPoint(0.1f, 0.1f);
	paramEffect->AddPoint(2.f, 0.1f);
	paramEffect->AddPoint(9.f, 6.f);
	auto vortexShaderImg = new ShaderImage(vortexImage, vortexMat);
	vortexShaderImg->name_ = "vortexShaderImg";
	scene7->Show(vortexShaderImg, 0.1f, paramEffect);
	scene7->AddCustomAction(9.f, heavyDropFunc);

	scene7->SetAutoEnd(9.1f);

	//////////////////////////////////////////////////////////////
	//??? 图片大小不应手动输入
	Image* musicImg = new Image(150, 150);
	musicImg->SetPosition(800, 450);
	musicImg->ReadFile("D:/HumanTree/17.png");
	//???
	Image* testImg = new Image(256, 256);
	testImg->SetPosition(800, 450);
	testImg->ReadFile("D:/HumanTree/dante.png");
	GOD.testImg_ = testImg;
	//___
	ShaderImage* musicSImg = new ShaderImage(musicImg);
	//___
	auto scene8 = sceneMgr.AddScene("D:/HumanTree/15.png");
	//直接黑屏4s，然后12s带模糊缓出
	SceneTransition* transition8 = new SceneTransition("fastBlackWithBlurIn", MakeParam<float>(4.0f,12.0f));
	sceneMgr.AddTransition(Int<2>(6, 7), transition8);

	Func wakeEvent = [&]()
	{
		bgm.PlayChunk("D:/HumanTree/Env_HiTech.wav");
		bgm.ChangeBGM("D:/HumanTree/Env_Room.wav");
	};

	Func textSoundEvent = [&]()
	{
		bgm.PlayChunk("D:/HumanTree/sound/HiTech_short.wav");
	};

	Func music2Event = [&]()
	{
		bgm.PlayChunk("D:/HumanTree/sound/music2.wav");
	};

	musicSImg->name_ = "musicSImg";
	musicSImg->material_->SetBlendType(Blend_Alpha);
	Pass* GBlurOncePass = new Pass;
	//??? 尽管是用于png的pass，但其画RT的dc的mat的blendtype还得是opaque，不知道为啥，之后查查
	GBlurOncePass->SetShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/gaussianBlurForGlow.fs");
	Pass* blur = new Pass;
	blur->AddChild(GBlurOncePass, 40);
	Pass* endPass = new Pass;
	endPass->SetShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/quadWithGlow.fs",Blend_Alpha);
	musicSImg->UsePass(blur, endPass);
	//___
	scene8->Show(musicSImg, 2.0f);


	scene8->AddCustomAction(0.0f, wakeEvent);
	scene8->AddCustomAction(2.0f, textSoundEvent);
	scene8->AddCustomAction(8.0f, music2Event);

	/////////////////////////////////////////////
	sceneMgr.JumpToScene(6);

	//???
	auto vendor = glGetString(GL_VENDOR);
	glDepthMask(false);
	glDisable(GL_DEPTH_TEST);

	bool bLoop = true;

	SDL_Event e;
	while (bLoop)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				bLoop = false;
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_EventType ee = SDL_EventType(e.type);
				bLoop = false;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				//??? 鼠标的点击sound，以后整合进鼠标和sceneMgr
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (sceneMgr.GetNowSceneIndex() == 0)
					{
						sound1->Play();
					}
					int x = e.button.x;
					int y = e.button.y;
					GOD.BroadCast(&LMB_Down(x,y));
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int x = e.motion.x;
				int y = e.motion.y;
				GOD.BroadCast(&Mouse_Move(x, y));
			}
		}
		WatchDog& gWatch = GOD.watchDog_;
		float deltaTime =  gWatch.Lapse();

		auto lastWatch = gWatch.GetWatchList();
		if (deltaTime > 0.03 && !bOldDraw)
		{
			std::cout << "Slow"<<deltaTime<<"\n";
			gWatch.Record();
		}
		if (deltaTime > 0.017f)//封顶60
		{
			//std::cout << deltaTime << std::endl;
			gWatch.Tick();
			gWatch.StartWatch();
			if (bOldDraw)
			{
				SDL_RenderClear(GOD.renderer_);
			}
			GOD.Update(deltaTime);
			if (bOldDraw)
			{
				//??? 把d3d画的保存为图
				saveScreenshot("D:/22.bmp", GOD.window_, GOD.renderer_);
			}
			GLRender();
			gWatch.Watch("Render");
			SDL_GL_SwapWindow(GOD.window_);
			gWatch.Watch("Swap");
		}
	}

	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(GOD.window_);

	// Clean up
	SDL_Quit();
	return 0;
}
#endif

#ifdef XCTEST
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <stdio.h>
#include <string>

#include "SDL_image.h"

#include "Numeric.h"
#endif // XCTEST
