#pragma once
#include "Effect.h"
#include "Utility.h"
#include "Math2D.h"
#include "TimeCurve.h"

class Object;

class EffectChangeSize : public Effect
{
public:
	EffectChangeSize();
	~EffectChangeSize();

	void Start() override;
	void Update(float deltaTime) override;

	template<class T>
	void Bind(T* obj)
	{
		typeName_ = obj->TypeName();
		obj_ = obj;
	}

	TimeCurve<Vec2> curve_;
protected:
	Object* obj_{ nullptr };
	std::string typeName_;
	float nowTime_{ 0.0f };

};

