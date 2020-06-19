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

class GraphicDefs
{
public:
	GraphicDefs();
	~GraphicDefs();
};

