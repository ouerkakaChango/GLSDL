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
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DrawCall.h"
#include "ShaderImage.h"
#include "Timeline.h"
#include "Pass.h"
#include "SceneShaderImage.h"
#include "GameUtility.h"
#include "ShaderQuadGroup.h"
#include "ShaderPPT.h"
#include "ShaderDragImage.h"
#include "Conditions.h"
#include "Debug.h"

DrawCall gDC;
VertexBuffer gVB;
IndexBuffer gIB;

void GLRender()
{
	Profile("GLRender")
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	if (GOD.bOldDraw_)
	{
		//??? 把d3d画的背景作为参数传进来
		SDL_Surface* Surface = IMG_Load("D:/22.bmp");

		gDC.material_->UpdateParam("tex", Surface);
		gDC.Do();
		SDL_FreeSurface(Surface);
	}
	auto& god = GOD;
	GOD.GetDrawcalls();
	for (auto& dc : GOD.drawcalls_) //(deprecated) oldDraw
	{
		dc->Do();
	}

	for (unsigned i = 0; i < GOD.prePassiveDrawcalls_.size(); i++)
	{
		GOD.prePassiveDrawcalls_[i]->Do();
	}

	for (unsigned i = 0; i < GOD.passiveDrawcalls_.size(); i++)
	{
		GOD.passiveDrawcalls_[i]->Do();
	}

	for (unsigned i = 0; i < GOD.preSceneColorDrawcalls_.size(); i++)
	{
		GOD.preSceneColorDrawcalls_[i]->Do();
	}

	for (unsigned i = 0; i < GOD.sceneColorDrawcalls_.size(); i++)
	{
		GOD.sceneColorDrawcalls_[i]->Do();
	}

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

	for (unsigned i = 0; i < GOD.afterPostDrawcalls_.size(); i++)
	{
		GOD.afterPostDrawcalls_[i]->Do();
	}
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
	///////////////////////////////////////////////////////////
	{
		//??? testImg
		Image* testImg = new Image(25, 25);
		testImg->SetPosition(800, 450);
		testImg->ReadFile("D:/HumanTree/dante.png");
		GOD.testImg_ = testImg;
	}
	////////////////////////////////////////////////////////////
	//???
	//Cursor cursor("D:/HumanTree/c7.png", 50, 50);
	Cursor* cursor = new Cursor("D:/HumanTree/c7.png", 50, 50);
	GOD.cursor_ = cursor;
	/////////////////////////////////////////////////////////////////
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
		if (!cursor->IsDefaultImg())
		{
			cursor->SetDefaultImage();
		}
		catSound->Play();
		sceneMgr.ToNext();
	};
	touch.BindEventHandler("LMB_Down", func2);
	touch.BindEventHandler("Mouse_Move");
	touch.beginOverlapFunc_ = [&]()
	{
		if (cursor->IsDefaultImg())
		{
			cursor->ChangeImage("D:/HumanTree/c2.png");
		}
	};
	touch.endOverlapFunc_ = [&]()
	{
		if (!cursor->IsDefaultImg())
		{
			cursor->SetDefaultImage();
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
		GOD.bOldDraw_ = false;
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
	//--- Scene 8
	Image* musicImg = new Image(150, 150);
	musicImg->SetPosition(800, 450);
	musicImg->ReadFile("D:/HumanTree/17.png");
	ShaderImage* musicSImg = new ShaderImage(musicImg);

	auto scene8 = sceneMgr.AddScene("D:/HumanTree/21.png");
	//???
	scene8->GetSceneImg()->name_ = "scene8SceneImg";
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

	//???
	Material* getUpMouseMaterial = new Material;
	sure( getUpMouseMaterial->CompileShader("D:/HumanTree/code/quad.vs", "D:/HumanTree/code/quadWithOffset.fs") );
	Image* getUpMouseImg = new Image(50, 50);
	getUpMouseImg->ReadFile("D:/HumanTree/c8.png");

	Button* getUpButton=nullptr;
	Func music2Event = [&]()
	{
		bgm.PlayChunk("D:/HumanTree/sound/music2.wav");
		//???
		//use glDraw's mouse,change mouse material here
		cursor->SetActive(true);
		cursor->sImg_->ChangeMaterial(getUpMouseMaterial);
		cursor->sImg_->SetImage(getUpMouseImg);
		//???
		getUpButton = new Button(false);
		getUpButton->SetImage(GOD.fastBlackCurtain_->GetImage());
		EventHandler func = [&](Event* event)
		{
			LOG("next 9!");
			sceneMgr.ToNext();
		};
		getUpButton->BindEventHandler("LMB_Down", func);
		getUpButton->SetActive(true);
	};

	musicSImg->name_ = "musicSImg";
	musicSImg->material_->SetBlendType(Blend_Alpha);
	Pass* GBlurOncePass = new Pass;
	GBlurOncePass->SetShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/gaussianBlurForGlow.fs"s);
	Pass* blur = new Pass;
	blur->AddChild(GBlurOncePass, 30);
	musicSImg->UsePass(blur);

	Material* drawMat = new Material;
	drawMat->CompileShader("D:/HumanTree/code/quadRT.vs", "D:/HumanTree/code/quadWithGlow.fs");
	drawMat->SetBlendType(Blend_Alpha);
	musicSImg->SetCustomDrawMaterial(drawMat,"glowedTex");

	scene8->Show(musicSImg, 2.0f);


	scene8->AddCustomAction(0.0f, wakeEvent);
	scene8->AddCustomAction(2.0f, textSoundEvent);
	scene8->AddCustomAction(8.0f, music2Event);
	//___ Scene 8
	/////////////////////////////////////////////
	//--- Scene 9
	auto scene9 = sceneMgr.AddScene("D:/HumanTree/PlayDesk.png");
	SceneTransition* transition9 = new SceneTransition("glFadeOutIn", 6);
	sceneMgr.AddTransition(Int<2>(7, 8), transition9);

	//--- cabinet rightButton
	Image* rightButtonImg = new Image(50, 50);
	rightButtonImg->ReadFile("D:/HumanTree/rightButton.png");
	rightButtonImg->SetPosition(1300, 800);

	Button* rightButton = new Button(rightButtonImg);
	EventHandler rightButtonFunc = [&](Event* event)
	{
		LOG("right");
	};
	rightButton->BindEventHandler("LMB_Down", rightButtonFunc);
	scene9->Show(rightButton);
	//___ cabinet rightButton

	//--- cabinet leftButton
	Image* leftButtonImg = new Image(50, 50);
	leftButtonImg->ReadFile("D:/HumanTree/leftButton.png");
	leftButtonImg->SetPosition(300, 800);

	Button* leftButton = new Button(leftButtonImg);
	EventHandler leftButtonFunc = [&](Event* event)
	{
		LOG("left");
	};
	leftButton->BindEventHandler("LMB_Down", leftButtonFunc);
	scene9->Show(leftButton);
	//___ cabinet leftButton

	//--- scene change button
	Image* sceneRBImg = new Image(25, 25);
	sceneRBImg->ReadFile("D:/HumanTree/sceneRB.png");
	sceneRBImg->SetPosition(1580, 480);

	Button* sceneRB = new Button(sceneRBImg);
	EventHandler sceneRBFunc = [&](Event* event)
	{
		LOG("right scene");
		sceneMgr.TransToScene(9);
	};
	sceneRB->BindEventHandler("LMB_Down", sceneRBFunc);
	scene9->Show(sceneRB);

	//Image* sceneLBImg = new Image(25, 25);
	//sceneLBImg->ReadFile("D:/HumanTree/sceneLB.png");
	//sceneLBImg->SetPosition(20, 480);
	//
	//Button* sceneLB = new Button(sceneLBImg);
	//EventHandler sceneLBFunc = [&](Event* event)
	//{
	//	LOG("left scene");
	//};
	//sceneLB->BindEventHandler("LMB_Down", sceneLBFunc);
	//scene9->Show(sceneLB);
	//___ scene change button

	//--- cabinet
	Image* cabinetImg = new Image(530, 190);
	cabinetImg->ReadFile("D:/HumanTree/cabinet.png");
	cabinetImg->SetPosition(800, 780);

	ShaderImage* cabnet = new ShaderImage(cabinetImg);
	cabnet->material_->SetBlendType(Blend_Alpha);
	scene9->Show(cabnet);
	//___ cabinet

	//--- playGrid
	Image* gridLineImg = new Image(50, 50);
	gridLineImg->ReadFile("D:/HumanTree/gridLine.png");

	Vec2 p1(0, 600);
	Vec2 p2(1600, 600);
	Vec2 p3(1300, 473);
	Vec2 p4(300, 473);
	MorphGrid grid(p1,p2,p3,p4,10,4);
	//grid细节:首先不需要边缘线，其次宽度根据grid边长调整（假透视）
	ShaderQuadGroup* shaderQuads = new ShaderQuadGroup(gridLineImg, grid, 10,1.0f,2.0f,false);
	shaderQuads->material_->SetBlendType(Blend_Alpha);
	scene9->Show(shaderQuads);
	//___ playGrid

	//--- PPT pre declare
	ShaderPPT* ppt = nullptr;
	Image* pptButtonImg = new Image(25, 25);
	pptButtonImg->ReadFile("D:/HumanTree/rightButton.png");
	pptButtonImg->SetPosition(1250, 250);
	Button* pptButton = new Button(pptButtonImg);
	//___ PPT pre declare

	//--- wordPPT
	{
		Image* ppt1 = new Image(1000, 250);
		ppt1->ReadFile("D:/HumanTree/PPT1.png");
		ppt1->SetPosition(800, 175);

		ppt = new ShaderPPT(ppt1);
		ppt->material_->SetBlendType(Blend_Alpha);

		ppt->SetFlipSound("D:/HumanTree/Sound/click.wav");
		ppt->SetChangeGroupSound("D:/HumanTree/Sound/click.wav");
		ppt->BindButton(pptButton);
		ppt->InsertPPT("D:/HumanTree/PPT2.png");
		ppt->InsertPPT("askPosition","D:/HumanTree/PPTQ1.png");
		ppt->InsertPPT("askReason", "D:/HumanTree/PPTQ2.png");
		scene9->Show(ppt);
	}
	//___ wordPPT

	//--- PPT button
	{
		EventHandler pptButtonFunc = [&](Event* event)
		{
			LOG("pptButton");
			ppt->Flip();
		};
		pptButton->BindEventHandler("LMB_Down", pptButtonFunc);
		scene9->Show(pptButton);
	}
	//___ PPT button

	//--- dragConditions
	PPTHasReadCondition *hasReadMessage1 = new PPTHasReadCondition;
	hasReadMessage1->Bind(ppt, "default");
	hasReadMessage1->SetActive(true);
	//___ dragConditions

	//--- drags
	Image* drag1Img = new Image(100, 100);
	drag1Img->ReadFile("D:/HumanTree/IconPos.png");
	drag1Img->SetPosition(800, 780);

	ShaderDragImage* drag1 = new ShaderDragImage(drag1Img,nullptr,VB_Static,TextureFilter_Nearest);
	drag1->material_->SetBlendType(Blend_Alpha);
	drag1->AddDragTarget(&grid);
	drag1->UsePass(blur);
	drag1->SetCustomDrawMaterial(drawMat->Clone(), "glowedTex");
	drag1->SetDragReleaseSound("D:/HumanTree/Sound/placement2.wav");
	scene9->ConditionShow(drag1,hasReadMessage1);

	Image* drag2Img = new Image(100, 100);
	drag2Img->ReadFile("D:/HumanTree/IconQues.png");
	drag2Img->SetPosition(975, 780);

	ShaderDragImage* drag2 = new ShaderDragImage(drag2Img, nullptr, VB_Static, TextureFilter_Nearest);
	drag2->material_->SetBlendType(Blend_Alpha);
	drag2->AddDragTarget(&grid);
	drag2->UsePass(blur);
	drag2->SetCustomDrawMaterial(drawMat->Clone(), "glowedTex");
	drag2->SetDragReleaseSound("D:/HumanTree/Sound/placement2.wav");
	scene9->ConditionShow(drag2, hasReadMessage1);

	Image* drag3Img = new Image(100, 100);
	drag3Img->ReadFile("D:/HumanTree/IconReason.png");
	drag3Img->SetPosition(625, 780);

	ShaderDragImage* drag3 = new ShaderDragImage(drag3Img, nullptr, VB_Static, TextureFilter_Nearest);
	drag3->material_->SetBlendType(Blend_Alpha);
	drag3->AddDragTarget(&grid);
	drag3->UsePass(blur);
	drag3->SetCustomDrawMaterial(drawMat->Clone(), "glowedTex");
	drag3->SetDragReleaseSound("D:/HumanTree/Sound/placement2.wav");
	scene9->ConditionShow(drag3, hasReadMessage1);
	//___drags
	//event: reset mouse
	Func func9_1 = [&]()
	{
		cursor->SetDefaultImage();
	};
	scene9->AddCustomAction(0.0f, func9_1);
	//___ Scene 9
	/////////////////////////////////////////////
	//Scene 10 (cat scene)
	auto scene10 = sceneMgr.AddScene("D:/HumanTree/catScene.png");

	//--- sceneLB
	Image* sceneLBImg10 = new Image(25, 25);
	sceneLBImg10->ReadFile("D:/HumanTree/sceneLB.png");
	sceneLBImg10->SetPosition(20, 480);
	
	Button* sceneLB10 = new Button(sceneLBImg10);
	EventHandler sceneLBFunc10 = [&](Event* event)
	{
		LOG("left scene");
		sceneMgr.TransToScene(8);
	};
	sceneLB10->BindEventHandler("LMB_Down", sceneLBFunc10);
	scene10->Show(sceneLB10);
	//___ sceneLB

	//--- wordBox
	Image* wordBoxImg = new Image(300, 150);
	wordBoxImg->ReadFile("D:/HumanTree/wordBox1.png");
	wordBoxImg->SetPosition(900, 350);
	wordBoxImg->SetAchorType(Anchor_LeftDown);

	ShaderImage* wordBox = new ShaderImage(wordBoxImg);
	wordBox->material_->SetBlendType(Blend_Alpha);
	EffectChangeSize* sizeEff = new EffectChangeSize;
	sizeEff->Bind(wordBox);
	sizeEff->curve_.Push(0.0, Vec2(3, 1.5));
	sizeEff->curve_.Push(0.5, Vec2(300, 150));
	scene10->Show(wordBox,0.0f, sizeEff);
	//___ wordBox
	/////////////////////////////////////////////
	//--- task
	TaskManager& taskMgr = GOD.taskManager_;
	Task t1("askPosition");
	t1.AddCondition(MakePack(drag1, Vec2(800, 536.5)), "DragRelease");
	t1.AddCondition(MakePack(drag2, Vec2(930, 536.5)), "DragRelease");
	t1.taskCompleteFunc_ = [&]()
	{
		ppt->ChangeGroup("askPosition");
	};
	taskMgr.tasks_.push_back(t1);

	Task t2("askReason");
	t2.AddCondition(MakePack(drag3, Vec2(800, 536.5)), "DragRelease");
	t2.taskCompleteFunc_ = [&]()
	{
		ppt->ChangeGroup("askReason");
	};
	taskMgr.tasks_.push_back(t2);
	//___ task
	////////////////////////////////////////////
	auto DebugJumpToScene = [&](unsigned inx)
	{
		sceneMgr.JumpToScene(inx);
		if (inx >= 7)
		{
			GOD.bOldDraw_ = false;
		}
		if (inx >= 8)
		{
			cursor->SetActive(true);
			bgm.ChangeBGM("D:/HumanTree/sound/music2.wav");
		}
	};
	DebugJumpToScene(8);

	//??? debug
	{
		auto vendor = glGetString(GL_VENDOR);
	}

	bool bLoop = true;
	//???
	bool bTicked = false;
	Uint32 last_, now_;

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
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					int x = e.button.x;
					int y = e.button.y;
					GOD.BroadCast(&LMB_Up(x, y));
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int x = e.motion.x;
				int y = e.motion.y;
				GOD.BroadCast(&Mouse_Move(x, y));
			}
		}

		//???
		float deltaTime;
		now_ = SDL_GetTicks();
		if (!bTicked)
		{
			last_ = now_;
			deltaTime = 0.0f;
			bTicked = true;
		}
		else
		{
			deltaTime = (now_ - last_) / 1000.0f;
		}

		if (deltaTime > 0.03 && !GOD.bOldDraw_) //detect too slow frame
		{
			std::cout << "Slow"<<deltaTime<<"\n";
			//???
			//gWatchDog.Record();
		}
		AutoProfiler::dataRoot_->children_.clear();

		if (deltaTime > 0.017f)//封顶60
		{
			//???
			last_ = now_;
			//std::cout << deltaTime << std::endl;
			GOD.ElapseGlobalK(deltaTime);
			{
				Profile("PreRender")
				if (GOD.bOldDraw_)
				{
					Profile("OldClear")
					SDL_RenderClear(GOD.renderer_);
				}
				GOD.Update(deltaTime);
				if (GOD.bOldDraw_)
				{
					Profile("OldSaveScreenShot")
					//??? 把d3d画的保存为图
					saveScreenshot("D:/22.bmp", GOD.window_, GOD.renderer_);
				}
			}
			GLRender();
			{
				Profile("Swap")
				SDL_GL_SwapWindow(GOD.window_);
			}
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
