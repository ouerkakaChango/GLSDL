#include "GameConfig.h"



GameConfig::GameConfig()
{
}


GameConfig::~GameConfig()
{
}

bool GameConfig::Load(const string& filePath)
{
	if (filePath.empty())
	{
		return LoadDefaultConfig();
	}
	else
	{
		//??? ����gameconfig�ļ�
	}
	return false;
}

string GameConfig::operator[](const string& key)
{
	return content_[key];
}

bool GameConfig::LoadDefaultConfig()
{
	//??? Ĭ��gameConfig
	content_["windowTitle"] = "HumanTree";
	content_["windowWidth"] = "1600";
	content_["windowHeight"] = "900";
	return true;
}