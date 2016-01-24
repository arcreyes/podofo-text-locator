#pragma once

#include <string>

using namespace std;

class VdpTextState
{
public:
	VdpTextState();
	VdpTextState(double charSpace, double wordSpace, int scale, string fontResource, double fontSize, int render, int rise);
	~VdpTextState();

	inline void SetCharacterSpacing(double charSpace)
	{
		m_charSpace = charSpace;
	}
	inline const double & GetCharacterSpacing(void)
	{
		return m_charSpace;
	}

	inline void SetWordSpacing(double wordSpace)
	{
		m_wordSpace = wordSpace;
	}
	inline const double & GetWordSpacing(void)
	{
		return m_wordSpace;
	}

	inline void SetHorizontalScale(int scale)
	{
		m_scale = scale;
	}
	inline const int & GetHorizontalScale(void)
	{
		return m_scale;
	}

	inline void SetFontResource(std::string fontResouce)
	{
		m_fontResource = fontResouce;
	}
	inline const string & GetFontResource(void)
	{
		return m_fontResource;
	}

	inline void SetFontSize(double fontSize)
	{
		m_fontSize = fontSize;
	}
	inline double & GetFontSize(void)
	{
		return m_fontSize;
	}

	inline void SetTextRenderingMode(int render)
	{
		m_render = render;
	}
	inline const int & GetTextRenderingMode(void)
	{
		return m_render;
	}

	inline void SetTextRise(int rise)
	{
		m_rise = rise;
	}
	inline const int & GetTextRise(void)
	{
		return m_rise;
	}

protected:
	double	m_charSpace;
	double	m_wordSpace;
	int		m_scale;
	string	m_fontResource;
	double	m_fontSize;
	int		m_render;
	int		m_rise;
};

