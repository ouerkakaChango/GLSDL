#pragma once
#include "Drawable.h"

class Image;
class ShaderImage;


class Button : public Drawable
{
public:
	Button(bool bRender=true);
	Button(Image* img);
	~Button();
	void Render() override;
	void SetActive(bool active) override;
	void SetSceneRT(RenderTexture* sceneRT) override;

	void SetImage(Image* image) { image_ = image; } //(deprecated) for oldDraw
protected:
	Image* image_;
	ShaderImage* sImg_{nullptr};		//for glDraw
	bool bRender_{ true };
};

