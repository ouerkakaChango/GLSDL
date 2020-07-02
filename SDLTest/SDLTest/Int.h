#pragma once
template <int N>
class Int
{
public:
	//!!! Int<N>ÍêÉÆ
	int operator[](int inx) const{ return -1; }
	bool operator<(const Int<N>& intN) const
	{
		for (int i = 0; i < N; i++)
		{
			if ((*this)[i] >= intN[i])
			{
				return false;
			}
		}
		return true;
	}
};

template<>
class Int<2>
{
public:
	Int(int x, int y):x_(x),y_(y) {}
	int operator[](int inx) const
	{
		if (inx == 0)
		{
			return x_;
		}
		else if (inx == 1)
		{
			return y_;
		}
		return -1;
	}

	bool operator<(const Int<2>& int2) const
	{
		bool re = x_ < int2.x_  && y_ < int2.y_;
		return x_ < int2.x_  && y_ < int2.y_;
	}

	bool operator==(const Int<2>& int2) const
	{
		return x_ == int2.x_  && y_ == int2.y_;
	}
private:
	int x_;
	int y_;
};