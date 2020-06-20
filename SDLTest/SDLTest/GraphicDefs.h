#pragma once

enum DrawCallChannel
{
	DrawCall_Passive,
	DrawCall_Post,
	DrawCall_AfterPost,
};

enum VBDrawType
{
	VB_Static,
	VB_Dynamic,
};

enum TextureFilterType
{
	TextureFilter_Linear,
	TextureFilter_Nearest,
};

class GraphicDefs
{
public:
	GraphicDefs();
	~GraphicDefs();
};

