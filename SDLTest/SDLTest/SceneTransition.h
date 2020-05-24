#pragma once
#include "Activeable.h"

#include <SDL.h>

#include "Timeline.h"
#include "Utility.h"
using std::string;

class Image;

class SceneTransition:public Activeable
{
public:
	SceneTransition(const string& effectName,float transitionTime);
	SceneTransition(const string& effectName, Params<float> params) ;
	void Update(float deltaTime);

	int frontInx_{ -1 }, nextInx_{-1};
	float transitionTime_{2.0f };
protected:
	string effectName_;
	Image* blackImg_;//(deprecated)
	Timeline timeline_;
};

