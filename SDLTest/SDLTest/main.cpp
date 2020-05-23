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

	if (bOldDraw)
	{
		//??? 把d3d画的背景作为参数传进来
		SDL_Surface* Surface = IMG_Load("D:/22.bmp");


		//???
		gDC.material_->UpdateParam("tex", Surface);
		gDC.Do();
		SDL_FreeSurface(Surface);
	}

	auto& god = GOD;
	for (auto& dc : GOD.drawcalls_)
	{
		dc->Do();
	}
}

bool initGL()
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

void InitSDL_OpenGL(SDL_Window *window)
{
	// core version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef GL_CORE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
	//由于代码中还有“过时”，或者不在PROFILE_CORE中的写法，所以使用兼容性版本
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#endif

	//Create context
	auto gContext = SDL_GL_CreateContext(window);
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

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize OpenGL
		if (!initGL())
		{
			printf("Unable to initialize OpenGL!\n");
			//abort();
		}
	}
}

int main(int argc, char* argv[]) {

	SDL_Window *window;                    // Declare a pointer

	GameConfig& gameConfig = GOD.gameConfig_;
	gameConfig.Load();

	int windowW = gameConfig.Get<int>("windowWidth");
	int windowH = gameConfig.Get<int>("windowHeight");

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
										   // Create an application window with the following settings:
	window = SDL_CreateWindow(
		gameConfig["windowTitle"].c_str(),                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		windowW,                               // width, in pixels
		windowH,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		abort();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		std::cout << "SDL_CreateRender Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	GOD.SetRenderer(renderer);

	//??? 各种初始化的封装
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		abort();
	}

	//???
	InitSDL_OpenGL(window);
	GOD.Init(); //必须在SDL Init了GL之后
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	auto& sceneMgr = GOD.sceneManager_;
	SoundEffect* sound1 = new SoundEffect;
	if (!sound1->Load("D:/HumanTree/bird.wav"))
	{
		abort();
	}

	//???
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
	BGMSystem bgm;
	bgm.SetBGM("D:/HumanTree/1.wav");
	///////////////////////////////////////////////////////////////////////
	//scene5
	auto scene5 = sceneMgr.InsertScene("D:/HumanTree/16.bmp");

	SoundEffect w1("D:/HumanTree/warning11.wav");
	SoundEffect w2("D:/HumanTree/warning12.wav");
	SoundEffect w3("D:/HumanTree/warning13.wav");

	Image* red = new Image(1600, 900);
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

	//关音效，关红闪特效
	scene7->AddCustomAction(0.1f, stopWarningFunc);

	Material* vortexMat = new Material;
	if (!vortexMat->CompileShader("D:/HumanTree/code/vortex.vs", "D:/HumanTree/code/vortex2.fs"))
	{
		abort();
	}
	vortexMat->UpdateParam("maxScale", 0.1f);
	vortexMat->UpdateParam("tex", IMG_Load("D:/HumanTree/vortex2.png"));
	vortexMat->SetBlendType(Blend_Alpha);
	Image* vortexImage = new Image(1600, 900);
	vortexImage->SetPosition(800, 450);

	EffectShaderParam* paramEffect = new EffectShaderParam;
	paramEffect->Bind(vortexMat, "maxScale");
	paramEffect->AddPoint(0.1f, 0.1f);
	paramEffect->AddPoint(7.f, 6.f);
	scene7->Show(new ShaderImage(vortexImage, vortexMat), 0.1f, paramEffect);
	scene7->SetAutoEnd(8.0f);

	//////////////////////////////////////////////////////////////
	auto scene8 = sceneMgr.AddScene("D:/HumanTree/test.png");
	//???
	//直接黑屏3s，然后2s缓出,并带后期模糊参数
	//SceneTransition* transition8 = new FastBlackWithBlurInTransition(3.0f,2.0f);
	SceneTransition* transition8 = new SceneTransition("fadeOutIn", 2);
	sceneMgr.AddTransition(Int<2>(6, 7), transition8);

	Func closeOldDraw = [&]()
	{;
		bOldDraw = false;
	};

	scene8->AddCustomAction(0.1f, closeOldDraw);

	//scene8->SetAutoEnd(7);

	/////////////////////////////////////////////
	sceneMgr.JumpToScene(4);


	bool bLoop = true;
	long last = 0;
	float deltaTime = 0.0;

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
		long now = SDL_GetTicks();
		if (now > last)
		{
			deltaTime = ((float)(now - last)) / 1000;
			if (deltaTime > 0.017f)//封顶60
			{
				//std::cout << deltaTime << std::endl;
				last = now;

				//主循环
				SDL_RenderClear(renderer);
				GOD.Update(deltaTime);
				if (bOldDraw)
				{
					//??? 把d3d画的保存为图
					saveScreenshot("D:/22.bmp", window, renderer);
				}
				GLRender();
				SDL_GL_SwapWindow(window);
			}
		}
	}

	//SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

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


//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

//Graphics program
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLint gTexCoord2DLocation = -1;
GLint texParamLocation = -1;
GLint maxThetaParamLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

bool initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();
	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	//###vs
	const GLchar* vertexShaderSource[] =
	{
		"\n \
		in vec2 LVertexPos2D; \
		in vec2 inTexCoord2D; \
		varying vec2 texCoord2D; \
		void main() \
		{ \
			texCoord2D = inTexCoord2D; \
			gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 );\
		 }"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Get fragment source
		//###fs
		const GLchar* fragmentShaderSource[] =
		{
			"\n \
			uniform sampler2D tex; \
			uniform float maxTheta; \
			varying vec2 texCoord2D;\
			void main() \
			{ \
				float pi = 3.1415926f; \
				float maxRho2 = 0.5f; \
				float thetaMin = 0.0f; \
				float thetaMax = maxTheta; \
				float u = texCoord2D.x-0.5f; \
				float v = texCoord2D.y-0.5f; \
				float rho2 = u*u+v*v; \
				float theta = mix(thetaMin,thetaMax,rho2/maxRho2) ;\
				float s = sin(theta); \
				float c = cos(theta); \
				float u0 = (c*u+s*v)/(c*c+s*s); \
				float v0 = 0.0f; \
				if(abs(s)<0.00001) \
				{ \
					v0 = (v-s*u0)/c; \
				} \
				else \
				{ \
					v0 = (c*u0-u)/s ; \
				} \
				vec2 uv0 = vec2(u0+0.5,v0+0.5); \
				gl_FragColor = texture(tex, uv0); \
			}"
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			printShaderLog(fragmentShader);
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);


			//Link program
			glLinkProgram(gProgramID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				printProgramLog(gProgramID);
				success = false;
			}
			else
			{
				//Get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				}
				else
				{
					gTexCoord2DLocation = glGetAttribLocation(gProgramID, "inTexCoord2D");
					if (gTexCoord2DLocation == -1)
					{
						printf("inTexCoord2D is not a valid glsl program variable!\n");
						success = false;
					}
					else
					{
						//Initialize clear color
						glClearColor(0.f, 0.f, 1.f, 1.f);

						//VBO data
						GLfloat vertexData[] =
						{
							-0.5f, -0.5f, 0.0f,1.0f,
							 0.5f, -0.5f, 1.0f,1.0f,
							 0.5f,  0.5f, 1.0f,0.0f,
							-0.5f,  0.5f, 0.0f,0.0f
						};

						//IBO data
						GLuint indexData[] = { 0, 1, 3, 2 };

						//Create VBO
						glGenBuffers(1, &gVBO);
						glBindBuffer(GL_ARRAY_BUFFER, gVBO);
						glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

						//Create IBO
						glGenBuffers(1, &gIBO);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

						/////////////////////
						//贴图
						GLuint TextureID = 0;

						// You should probably use CSurface::OnLoad ... ;)
						//-- and make sure the Surface pointer is good!
						SDL_Surface* Surface = IMG_Load("D:/HumanTree/1.jpg");

						SoundEffect* sound1 = new SoundEffect;
						if (!sound1->Load("D:/HumanTree/bird.wav"))
						{
							abort();
						}
						glBindTexture(GL_TEXTURE_2D, TextureID);

						int Mode = GL_RGB;

						if (Surface->format->BytesPerPixel == 4) {
							Mode = GL_RGBA;
						}

						glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

						//Any other glTex* stuff here

						texParamLocation = glGetUniformLocation(gProgramID, "tex");
						if (texParamLocation == -1)
						{
							printf("%s is not a valid glsl program variable!\n", "tex");
							success = false;
						}

						maxThetaParamLocation = glGetUniformLocation(gProgramID, "maxTheta");
						if (maxThetaParamLocation == -1)
						{
							printf("%s is not a valid glsl program variable!\n", "maxTheta");
							success = false;
						}
					}	
				}
			}
		}
	}

	return success;
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

//???
//1s内从0到1，过了之后再为0
//因为很多程序都用到，不如全局
//现在应该会有问题，舍弃了少数时间
float globalK = 0.0f;

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Bind program
	glUseProgram(gProgramID);
	
	//Enable vertex position
	glEnableVertexAttribArray(gVertexPos2DLocation);
	glEnableVertexAttribArray(gTexCoord2DLocation);
	
	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	//posx posy texx texy |...
	//参数位置，参数数量，参数类型，是否归一化，步长，起始偏移
	glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glVertexAttribPointer(gTexCoord2DLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));

	////////////////////////////////
	//传uniform参数
	//tex使用的是寄存器Texuture0
	glUniform1i(texParamLocation, 0);
	//???
	float inParam = globalK * PI*6.0f;
	inParam = PI*6;
	glUniform1f(maxThetaParamLocation, inParam);
	
	//Set index data and render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	
	//???
	//glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	
	//Disable vertex position
	glDisableVertexAttribArray(gVertexPos2DLocation);
	glDisableVertexAttribArray(gTexCoord2DLocation);
	
	//Unbind program
	glUseProgram(NULL);
}

int main(int argc, char* argv[]) 
{
	SDL_Window *gWindow;
	SDL_Init(SDL_INIT_EVERYTHING);

	// core version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//由于代码中还有“过时”，或者不在PROFILE_CORE中的写法，所以使用兼容性版本
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	gWindow = SDL_CreateWindow(
		"xctest",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		1600,                               // width, in pixels
		900,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);


	//Create context
	auto gContext = SDL_GL_CreateContext(gWindow);
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

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize OpenGL
		if (!initGL())
		{
			printf("Unable to initialize OpenGL!\n");
			//abort();
		}
	}

	bool bLoop = true;
	long last = 0;
	float deltaTime = 0.0;

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
		}
		long now = SDL_GetTicks();
		if (now > last)
		{
			deltaTime = ((float)(now - last)) / 1000;
			if (deltaTime > 0.017f)//封顶60
			{
				last = now;
				//逻辑更新
				globalK += deltaTime;
				globalK = globalK > 4.0f ? 0.0f : globalK;
				//渲染循环
				render();
				SDL_GL_SwapWindow(gWindow);
			}
		}
	}



	// Close and destroy the window
	SDL_DestroyWindow(gWindow);

	// Clean up
	SDL_Quit();
	return 0;
}
#endif // XCTEST
