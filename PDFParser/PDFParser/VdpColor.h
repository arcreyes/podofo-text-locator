#pragma once

#include "VdpDefs.h"

class VdpColor
{
public:
	VdpColor();
	VdpColor(double red, double green, double blue);
	VdpColor(double cyan, double magenta, double yellow, double black);
	VdpColor(double gray);
	~VdpColor();

	inline EVdpColorSpace GetColorNamespace(void)
	{
		return m_eColorNamespace;
	}
	inline bool IsRgb(void)
	{
		return m_eColorNamespace == eColorNamespace_Rgb;
	}
	inline bool IsCmyk(void)
	{
		return m_eColorNamespace == eColorNamespace_Cmyk;
	}
	inline bool IsGray(void)
	{
		return m_eColorNamespace == eColorNamespace_Gray;
	}

protected:
	double m_red;
	double m_green;
	double m_blue;
	double m_cyan;
	double m_magenta;
	double m_yellow;
	double m_black;
	double m_gray;
	EVdpColorSpace m_eColorNamespace;
};

