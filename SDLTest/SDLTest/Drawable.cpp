#include "Drawable.h"



Drawable::Drawable()
{
}


Drawable::~Drawable()
{
}

void Drawable::SetSceneRT(RenderTexture* sceneRT)
{
	sceneRT_ = sceneRT;
}