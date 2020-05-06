#pragma once
#include "Drawable.h"

class Image;
class Event;

class Button : public Drawable
{
public:
	Button();
	~Button();
	void Render() override;

	void SetImage(Image* image) { image_ = image; }
protected:
	Image* image_;
};

