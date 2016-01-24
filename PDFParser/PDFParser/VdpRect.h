#pragma once

#include "VdpDefs.h"

class VdpRect
{
public:
	VdpRect();
	VdpRect(double x, double y, double width, double height);
	~VdpRect();

	inline void SetLeft(double x)
	{
		m_x = x;
	}
	inline double GetLeft(void)
	{
		return m_x;
	}

	inline void SetBottom(double x)
	{
		m_y = x;
	}
	inline double GetBottom(void)
	{
		return m_y;
	}

	inline void SetWidth(double x)
	{
		m_width = x;
	}
	inline double GetWidth(void)
	{
		return m_width;
	}

	inline void SetHeight(double x)
	{
		m_height = x;
	}
	inline double GetHeight(void)
	{
		return m_height;
	}

protected:
	double m_x;
	double m_y;
	double m_width;
	double m_height;

	EVdpUnit m_eUnit;
};

