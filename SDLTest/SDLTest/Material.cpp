#include "Material.h"

#include <SDL_image.h>

#include "File.h"
#include "RenderTexture.h"
#include "God.h"
#include "Debug.h"

bool MaterialParam::Check(Material* material)
{
	//uniform
	if (typeName_ == "texture2d" || typeName_ == "uniform1f")
	{
		paramLocation_ = glGetUniformLocation(material->programID_, name_.c_str());
		if (paramLocation_ == -1)
		{
			printf(" %s is not a valid glsl program variable!\n", name_.c_str());
			return false;
		}
	}
	else
	{
		abort();
	}
		
	return true;
}

void VSAttributeParam::SetFormat(GLint num, GLenum type, GLboolean normalized)
{
	num_ = num;
	type_ = type;
	normalized_ = normalized;
}

void VSAttributeParam::SetFormatByType(const std::string& typeName)
{
	if (typeName == "vec2")
	{
		SetFormat(2, GL_FLOAT, GL_FALSE);
	}
	else
	{
		abort();
	}
}

void Uniform1fParam::UpdateValue()
{
	//???
	if (name_ == "alpha")
	{
		int a = 1;
	}
	value_ = toUpdateValue_;
	glUniform1f(paramLocation_, value_);
	bNeedUpdate_ = false;
}

std::string Uniform1fParam::TypeName()
{
	return "uniform1f";
}

void Uniform1fParam::InjectValue(float newValue)
{
	bNeedUpdate_ = true;
	toUpdateValue_ = newValue;
}

std::string Texture2DParam::TypeName()
{
	return "texture2d";
}

Texture2DParam::Texture2DParam(SDL_Surface* textureSurface, unsigned textureUnit, TextureFilterType texFilterType)
	:nowSurface_(textureSurface),
	textureUnit_(textureUnit),
	texFilterType_(texFilterType)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glGenTextures(1, &textureID_);
	glBindTexture(GL_TEXTURE_2D, textureID_);

	int Mode = GL_RGB;

	if (textureSurface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}
	else
	{
		abort();
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, textureSurface->w, textureSurface->h, 0, Mode, GL_UNSIGNED_BYTE, textureSurface->pixels);

	//???
	if (texFilterType_ == TextureFilter_Linear)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (texFilterType_ == TextureFilter_Nearest)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

}

void Texture2DParam::ChangeFilter(TextureFilterType texFilterType)
{
	texFilterType_ = texFilterType;

	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	glBindTexture(GL_TEXTURE_2D, textureID_);

	if (texFilterType_ == TextureFilter_Linear)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (texFilterType_ == TextureFilter_Nearest)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void Texture2DParam::UpdateValue()
{
	glActiveTexture(GL_TEXTURE0 + textureUnit_);
	if (updateType_ == TextureUpdate_Surface)
	{
		glBindTexture(GL_TEXTURE_2D, textureID_);
		if (toUpdateSurface_ && toUpdateSurface_ != nowSurface_)
		{
			int Mode = GL_RGB;

			if (toUpdateSurface_->format->BytesPerPixel == 4) {
				Mode = GL_RGBA;
			}
			else
			{
				//???
				LOG("Warning:RGB texutre,may be having unhandled situation");
				//if (!GOD.bOldDraw_)
				//{
				//	abort();
				//}
			}
			glTexImage2D(GL_TEXTURE_2D, 0, Mode, toUpdateSurface_->w, toUpdateSurface_->h, 0, Mode, GL_UNSIGNED_BYTE, toUpdateSurface_->pixels);
		}
	}
	else if (updateType_ == TextureUpdate_ID)
	{
		if (textureID_ != toUpdateID_)
		{
			textureID_ = toUpdateID_;
		}
		glBindTexture(GL_TEXTURE_2D, textureID_);
	}
	else if (updateType_ == TextureUpdate_RenderTexture)
	{
		if (textureID_ != toUpdateRT_->renderTextureID_)
		{
			textureID_ = toUpdateRT_->renderTextureID_;
		}
		glBindTexture(GL_TEXTURE_2D, toUpdateRT_->renderTextureID_);
	}
	glUniform1i(paramLocation_, textureUnit_);
	//保持bNeedUpdate_为true
}

void Texture2DParam::InjectValue(RenderTexture* newValue)
{
	bNeedUpdate_ = true;
	updateType_ = TextureUpdate_RenderTexture;
	toUpdateRT_ = newValue;
}

bool paramInfo::operator==(const std::string& name)
{
	return name_ == name;
}

bool operator==(const paramInfo& info1, const paramInfo& info2)
{
	return info1.typeName_ == info2.typeName_ && info1.name_ == info2.name_;
}

Material::Material()
{
	//Generate program
	programID_ = glCreateProgram();
}


Material::~Material()
{
}

void Material::InitAttributes(const std::vector<std::string>& vsWords)
{
	for (unsigned i = 0; i < vsWords.size(); i++)
	{
		std::string temp = vsWords[i];
		if (temp == "in" && i+2 < vsWords.size())
		{
			attributeInfos_.push_back(paramInfo(vsWords[i + 1],vsWords[i+2]));
		}
	}
}

void Material::InitParams(const std::vector<std::string>& shaderWords)
{
	for (unsigned i = 0; i < shaderWords.size(); i++)
	{
		std::string temp = shaderWords[i];
		if (temp == "uniform" && i + 2 < shaderWords.size())
		{
			if (std::find(paramInfos_.begin(), paramInfos_.end(), shaderWords[i + 2]) == paramInfos_.end())
			{//vs,fs中可能检测到同一个变量，去重复
				paramInfos_.push_back(paramInfo(shaderWords[i + 1], shaderWords[i + 2]));
			}
		}
	}
}

bool Material::LoadVS(const Path& vsFilePath)
{
	//Create vertex shader
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
	ReadFile vsFile(vsFilePath);
	const GLchar* vsSource = vsFile.GetGLCharBuffer();
	if (vsSource == nullptr)
	{
		return false;
	}
	glShaderSource(vertexShader_, 1, &vsSource, NULL);
	glCompileShader(vertexShader_);
	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled == GL_TRUE)
	{
		auto& words = vsFile.GetWords();
		InitAttributes(words);
		InitParams(words);
		glAttachShader(programID_, vertexShader_);
		return true;
	}
	else
	{
		return false;
	}
}

bool Material::LoadFS(const Path& fsFilePath)
{
	//Create fragment shader
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	ReadFile fsFile(fsFilePath);
	const GLchar* fsSource = fsFile.GetGLCharBuffer();
	if (fsSource == nullptr)
	{
		return false;
	}
	glShaderSource(fragmentShader_, 1, &fsSource, NULL);
	glCompileShader(fragmentShader_);
	//Check fragment shader for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled == GL_TRUE)
	{
		auto& words = fsFile.GetWords();
		InitParams(words);
		glAttachShader(programID_, fragmentShader_);
		return true;
	}
	else
	{
		return false;
	}
}

bool Material::LinkProgram()
{
	//Link program
	glLinkProgram(programID_);
	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programID_, GL_LINK_STATUS, &programSuccess);
	return programSuccess == GL_TRUE;
}

bool Material::CheckAttributes()
{
	for (auto& info : attributeInfos_)
	{
		VSAttributeParam* param = new VSAttributeParam;
		param->SetFormatByType(info.typeName_);
		param->paramLocation_ = glGetAttribLocation(programID_, info.name_.c_str());
		if (param->paramLocation_ == -1)
		{
			printf("%s is not a valid glsl program variable!\n",info.name_.c_str());
			delete param;
			return false;
		}
		else
		{
			vsAttributeParams_.push_back(param);
		}
	}
	return true;
}

MaterialParam* Material::AddParamByInfo(const paramInfo& info)
{
	if (info.typeName_ == "sampler2D")
	{
		return AddParam(info.name_, IMG_Load("D:/HumanTree/dante.png"));
	}
	else if (info.typeName_ == "float")
	{
		return AddParam(info.name_, 0.0f);
	}
	return nullptr;
}

MaterialParam* Material::AddTextureParam(const std::string& paramName, TextureFilterType texFilterType)
{
	auto* param = new Texture2DParam(IMG_Load("D:/HumanTree/dante.png"), nowTexturePos_, texFilterType);
	param->name_ = paramName;
	param->typeName_ = "texture2d";
	params_.push_back(param);
	nowTexturePos_ += 1;
	return param;
}

bool Material::CheckParams()
{
	for (auto& info : paramInfos_)
	{
		auto param = AddParamByInfo(info);
		if (!param->Check(this))
		{
			return false;
		}
	}

	return true;
}

void Material::OutputVSError()
{
	printf("Unable to compile vertex shader %d!\n", vertexShader_);
	printShaderLog(vertexShader_);
}

void Material::OutputFSError()
{
	printf("Unable to compile fragment shader %d!\n", fragmentShader_);
	printShaderLog(fragmentShader_);
}

void Material::OutputLinkError()
{
	printf("Error linking program %d!\n", programID_);
	printProgramLog(programID_);
}

bool Material::CompileShader(const Path& vsPath, const Path& fsPath)
{
	vsPath_ = vsPath;
	fsPath_ = fsPath;
	if (!LoadVS(vsPath))
	{
		OutputVSError();
		return false;
	}

	if (!LoadFS(fsPath))
	{
		OutputFSError();
		return false;
	}

	if (!LinkProgram())
	{
		OutputLinkError();
		return false;
	}

	if (!CheckAttributes())
	{
		return false;
	}

	return CheckParams();
}

void Material::UpdateParam(const std::string& paramName, float newValue)
{
	for (auto& param : params_)
	{
		if (param->typeName_ == "uniform1f" && param->name_ == paramName)
		{
			Uniform1fParam* realParam = static_cast<Uniform1fParam*>(param);
			if (realParam)
			{
				realParam->InjectValue(newValue);
				break;
			}
		}
	}
}

void Material::UpdateParam(const std::string& paramName, SDL_Surface* newTextureSurface)
{
	for (auto& param : params_)
	{
		if (param->typeName_ == "texture2d" && param->name_ == paramName)
		{
			Texture2DParam* realParam = static_cast<Texture2DParam*>(param);
			if (realParam)
			{
				realParam->bNeedUpdate_ = true;
				realParam->toUpdateSurface_ = newTextureSurface;
				break;
			}
		}
	}
}

void Material::UpdateParam(const std::string& paramName, RenderTexture* rt)
{
	for (auto& param : params_)
	{
		if (param->typeName_ == "texture2d" && param->name_ == paramName)
		{
			Texture2DParam* realParam = static_cast<Texture2DParam*>(param);
			if (realParam)
			{
				realParam->InjectValue(rt);
				return;
			}
		}
	}
}

//???
void Material::UpdateTextureParam(const std::string& paramName, GLuint n)
{
	for (auto& param : params_)
	{
		if (param->typeName_ == "texture2d" && param->name_ == paramName)
		{
			Texture2DParam* realParam = static_cast<Texture2DParam*>(param);
			if (realParam)
			{
				realParam->toUpdateID_ = n;
				realParam->bNeedUpdate_ = true;
				realParam->updateType_ = TextureUpdate_ID;
				return;
			}
		}
	}
}

MaterialParam* Material::AddParam(const std::string& paramName, float defaultValue)
{
	auto* param = new Uniform1fParam(defaultValue);
	param->name_ = paramName;
	param->typeName_ = "uniform1f";
	params_.push_back(param);
	return param;
}

MaterialParam* Material::AddParam(const std::string& paramName, SDL_Surface* textureSurface)
{
	auto* param = new Texture2DParam(textureSurface, nowTexturePos_);
	param->name_ = paramName;
	param->typeName_ = "texture2d";
	params_.push_back(param);
	nowTexturePos_ += 1;
	return param;
}


GLsizei Material::CalculateStride()
{
	GLsizei re = 0;
	for (auto& param : vsAttributeParams_)
	{
		re += param->num_ * sizeof(param->type_);
	}
	return re;
}

const GLvoid* Material::CalculateOffset(int index)
{
	GLsizei offset = 0;
	for (unsigned i = 0; i < (unsigned)index; i++)
	{
		offset += vsAttributeParams_[i]->num_ * sizeof(vsAttributeParams_[i]->type_);
	}
	return (const GLvoid*)(offset);
}

Material* Material::Clone()
{
	Material* re = new Material;
	re->name_ = name_ + "_Clone";
	re->vsPath_ = vsPath_;
	re->fsPath_ = fsPath_;

	re->programID_ = programID_;
	re->vsAttributeParams_ = vsAttributeParams_;
	//深拷参数，不然出问题
	for (auto& info : paramInfos_)
	{
		auto param = re->AddParamByInfo(info);
		if (!param->Check(this))
		{
			abort();
		}
	}
	re->blendType_ = blendType_;
	re->paramInfos_ = paramInfos_;
	re->vertexShader_ = vertexShader_;
	re->fragmentShader_ = fragmentShader_;
	return re;
}

Material* Material::Clone(TextureFilterType texFilterType)
{
	Material* re = new Material;
	re->name_ = name_ + "_Clone";
	re->vsPath_ = vsPath_;
	re->fsPath_ = fsPath_;

	re->programID_ = programID_;
	re->vsAttributeParams_ = vsAttributeParams_;
	//深拷参数，不然出问题
	for (auto& info : paramInfos_)
	{
		MaterialParam* param = nullptr;
		//???
		if (info.name_ == "tex")
		{
			param = re->AddTextureParam("tex",texFilterType);
		}
		else
		{
			param = re->AddParamByInfo(info);
		}
		if (!param->Check(this))
		{
			abort();
		}
	}
	re->blendType_ = blendType_;
	re->paramInfos_ = paramInfos_;
	re->vertexShader_ = vertexShader_;
	re->fragmentShader_ = fragmentShader_;
	return re;
}

void Material::CloneType(Material* ori, TextureFilterType texFilterType)
{
	sure(ori != nullptr);
	blendType_ = ori->blendType_;
	//???
	GetParam<Texture2DParam>("tex")->ChangeFilter(texFilterType);
}