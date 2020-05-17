#include "TimeCurve.h"

template <class DataType>
void TimeCurve<DataType>::SetInterp(InterpType interpType)
{
	if (interp_ != nullptr)
	{
		delete interp_;
		interp_ = nullptr;
	}
	if (interpType == Interp_Linear)
	{
		interp_ = new CurveLinearInterp;
	}
}

template <class DataType>
void TimeCurve<DataType>::Push(float inVal, DataType outVal)
{
	curve_.push_back(make_pair(inVal, outVal));
}

template <class DataType>
DataType TimeCurve<DataType>::Get(float inVal)
{
	DataType emptyData=0;
	if (curve_.empty())
	{
		return emptyData;
	}

	if (inVal <= curve_.front().first)
	{
		return curve_.front().second;
	}

	if (inVal >= curve_.back().first)
	{
		return curve_.back().second;
	}

	int foundInx = -1;
	for (unsigned i = 0; i < curve_.size(); i++)
	{
		float t1 = curve_[i].first;
		float t2 = t1;
		if (i + 1 < curve_.size())
		{
			t2 = curve_[i+1].first;
		}
		if (inVal >= t1 && inVal < t2)
		{
			foundInx = i;
			break;
		}
	}

	if (foundInx == -1)
	{
		return emptyData;
	}

	return interp_->GetInterped<DataType>(inVal, foundInx, curve_);


}

template<class DataType>
DataType CurveInterp::GetInterped(float inVal, unsigned inx, vector<pair<float, DataType>>& curve)
{
	DataType emptyData=0;
	if (type_ == Interp_Linear)
	{
		auto real = static_cast<CurveLinearInterp*>(this);
		return real->GetInterped_Imp<DataType>(inVal, inx, curve);
	}
	return emptyData;
}

template<class DataType>
DataType CurveLinearInterp::GetInterped_Imp(float inVal, unsigned inx, vector<pair<float, DataType>>& curve)
{
	float t1 = curve[inx].first;
	DataType f1 = curve[inx].second;
	float t2 = curve[inx+1].first;
	DataType f2 = curve[inx+1].second;
	return f1 + (f2 - f1)*(inVal - t1) / (t2 - t1);
}