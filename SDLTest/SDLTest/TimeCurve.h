#pragma once

#include <iostream>
#include <vector>
using namespace std;

enum InterpType
{
	Interp_Undefined,
	Interp_Linear,
};

class CurveInterp
{
public:
	
	template<class DataType>
	DataType GetInterped(float inVal, unsigned inx, vector<pair<float, DataType>>& curve);

	InterpType type_{ Interp_Undefined };
};

class CurveLinearInterp : public CurveInterp
{
public:
	CurveLinearInterp();
	template<class DataType>
	DataType GetInterped_Imp(float inVal, unsigned inx, vector<pair<float, DataType>>& curve);
};

template <class DataType>
class TimeCurve
{
public:
	void SetInterp(InterpType interpType);
	void Push(float inVal, DataType outVal);
	DataType Get(float inVal);
	vector<pair<float, DataType>> curve_;

private:
	CurveInterp* interp_{nullptr};
};

#include "TimeCurve.inl"