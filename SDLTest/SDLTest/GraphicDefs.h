#pragma once

enum DrawCallChannel
{
	DrawCall_Passive,
	DrawCall_PreSceneColor,
	DrawCall_SceneColor,
	DrawCall_Post,
	DrawCall_AfterPost,
};

enum VBDrawType
{
	VB_Static,
	VB_Dynamic,
};

enum GeometryType
{
	Geom_Normal,
	Geom_Quad,
	Geom_QuadGroup,
};

enum TextureFilterType
{
	TextureFilter_Linear,
	TextureFilter_Nearest,
};

enum MaterialBlendType
{
	Blend_Opaque,
	Blend_Alpha,
};

class GraphicDefs
{
public:
	GraphicDefs();
	~GraphicDefs();
};

