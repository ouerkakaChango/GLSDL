#pragma once
//https://open.gl/textures
#include "Utility.h"
#include "GLUtility.h"
#include "GraphicDefs.h"
#include "TypeNameable.h"
#include "Nameable.h"
class DrawCall;
class Material;
class RenderTexture;

#define MaterialParamClass \
public: \
static std::string TypeName(); \
private: \
friend class Material;\
friend class DrawCall;

//面向drawcall的材质参数类
class MaterialParam : public TypeNameable,public Nameable
{
	friend class Material;
	friend class DrawCall;
public:

	GLint paramLocation_ = -1;
protected:
	bool bNeedUpdate_{ false };
	virtual void UpdateValue() {};
	bool Check(Material*);
};

class VSAttributeParam : public MaterialParam
{
	friend class Material;
	friend class DrawCall;
public:
	void SetFormatByType(const std::string& typeName);
protected:
	void SetFormat(GLint num, GLenum type, GLboolean normalized);

	GLint num_ = -1;
	GLenum type_;
	GLboolean normalized_ = GL_FALSE;
};

class Uniform1fParam : public MaterialParam
{
	MaterialParamClass
public:
	Uniform1fParam(float defaultValue) :defaultvalue_(defaultValue) {}
	void InjectValue(float newValue);
private:
	void UpdateValue() override;

	float value_;
	float toUpdateValue_;
	float defaultvalue_;
};

enum TextureUpdateType
{
	TextureUpdate_Surface,
	TextureUpdate_ID,
	TextureUpdate_RenderTexture,
};

class Texture2DParam : public MaterialParam
{
	MaterialParamClass
public:
	Texture2DParam(SDL_Surface* textureSurface, unsigned textureUnit, TextureFilterType texFilterType = TextureFilter_Linear);
	void InjectValue(RenderTexture* newValue);
	void ChangeFilter(TextureFilterType texFilterType);
private:
	void UpdateValue() override;

	TextureUpdateType updateType_{ TextureUpdate_Surface };
	SDL_Surface* nowSurface_;
	SDL_Surface* toUpdateSurface_{nullptr};
	RenderTexture* toUpdateRT_{ nullptr };
	GLuint toUpdateID_;
	GLuint textureID_;
	TextureFilterType texFilterType_;
	unsigned textureUnit_{ 0 };
};

struct paramInfo
{
	paramInfo() = default;
	paramInfo(const std::string& typeName, const std::string& name)
		:typeName_(typeName),
		name_(name)
	{}
	friend bool operator==(const paramInfo& info1, const paramInfo& info2);
	bool operator==(const std::string& name);
	std::string typeName_{""};
	std::string name_{ "" };
};

bool operator==(const paramInfo& info1, const paramInfo& info2);

//导入类（用户材质->dc可用的数据） 面向用户的抽象材质类
class Material : public Nameable
{
public:
	Material();
	~Material();

	bool CompileShader(const Path& vsPath, const Path& fsPath);
	void SetBlendType(MaterialBlendType blend) { blendType_ = blend; }
	MaterialBlendType GetBlendType() const{ return blendType_; }

	template <class T>
	T* GetParam(const std::string& paramName)
	{
		for (auto& param : params_)
		{
			if (param->typeName_ == T::TypeName() && param->name_ == paramName)
			{
				return static_cast<T*>(param);
			}
		}
		return nullptr;
	}
	template<>
	MaterialParam* GetParam<MaterialParam>(const std::string& paramName)
	{
		for (auto& param : params_)
		{
			if (param->name_ == paramName)
			{
				return param;
			}
		}
		return nullptr;
	}

	void UpdateParam(const std::string& paramName, float newValue);					//for uniform1f
	void UpdateParam(const std::string& paramName, SDL_Surface* newTextureSurface);	//for texture2d
	void UpdateParam(const std::string& paramName, RenderTexture* rt);				//for texture2d
	void UpdateParam(const std::string& paramName, GLuint n)=delete;				//prevent unexpected implicit call
	Material* Clone();
	Material* Clone(TextureFilterType texFilterType);	//??? clone,but change the "tex" parameter's filter
	void CloneType(Material* ori, TextureFilterType texFilterType=TextureFilter_Linear); //???
	bool IsStaticParameterInjected() { return bStaticParameterInjected_; }

	GLuint programID_ = 0;
	std::vector<VSAttributeParam*> vsAttributeParams_;
	std::vector<MaterialParam*> params_;
protected:
	friend class DrawCall;
	GLsizei CalculateStride();
	const GLvoid* CalculateOffset(int index);

	bool LoadVS(const Path& vsFilePath);
	bool LoadFS(const Path& fsFilePath);
	bool LinkProgram();
	bool CheckAttributes();
	bool CheckParams();
	void InitAttributes(const std::vector<std::string>& vsWords);
	void InitParams(const std::vector<std::string>& shaderWords);
	void OutputVSError();
	void OutputFSError();
	void OutputLinkError();

	MaterialParam* AddParamByInfo(const paramInfo& info);
	MaterialParam* AddParam(const std::string& paramName, float defaultValue);
	MaterialParam* AddParam(const std::string& paramName, SDL_Surface* textureSurface);
	//???
	MaterialParam* AddTextureParam(const std::string& paramName, TextureFilterType texFilterType);

	std::vector<paramInfo> attributeInfos_;	
	std::vector<paramInfo> paramInfos_;
	MaterialBlendType blendType_{ Blend_Opaque };
	GLuint vertexShader_;
	GLuint fragmentShader_;

	Path vsPath_, fsPath_;
	unsigned nowTexturePos_{ 0 };

	bool bStaticParameterInjected_{ false };

};