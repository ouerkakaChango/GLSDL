#pragma once
#include "Effect.h"

#include "Utility.h"
#include "TimeCurve.h"

class Material;

class EffectShaderParam : public Effect
{
public:
	EffectShaderParam(InterpType interpType=Interp_Linear);
	void Start() override;
	void Update(float deltaTime) override;


	void Bind(Material* material, std::string paramName);
	void AddPoint(float time, float value);
protected:
	InterpType interpType_;
	Material* material_;
	float nowTime_{ 0.0f };
	std::string paramName_;
	std::string paramTypeName_;
	TimeCurve<float> floatCurve_;
};

