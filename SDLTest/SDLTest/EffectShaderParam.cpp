#include "EffectShaderParam.h"

#include "Material.h"

EffectShaderParam::EffectShaderParam(InterpType interpType):interpType_(interpType)
{

}

void EffectShaderParam::Bind(Material* material, std::string paramName)
{
	material_ = material;
	paramName_ = paramName;
	auto param = material->GetParam<MaterialParam>(paramName_);
	paramTypeName_ = param->typeName_;
	if (paramTypeName_ == "uniform1f")
	{
		floatCurve_.SetInterp(interpType_);
	}
}

void EffectShaderParam::AddPoint(float time, float value)
{
	if (paramTypeName_ == "uniform1f")
	{
		floatCurve_.Push(time, value);
	}
}

void EffectShaderParam::Start()
{
	bActive_ = true;
	nowTime_ = 0.0f;
}

void EffectShaderParam::Update(float deltaTime)
{
	nowTime_ += deltaTime;
	if (paramTypeName_ == "uniform1f")
	{
		material_->UpdateParam(paramName_, floatCurve_.Get(nowTime_));
	}
}