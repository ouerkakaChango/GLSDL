#pragma once
#include "Drawable.h"

class Image;
class Event;

class Button : public Drawable
{
public:
	Button(bool bRender=true);
	~Button();
	void Render() override;

	void SetImage(Image* image) { image_ = image; }
protected:
	Image* image_;
	bool bRender_{ true };
};

