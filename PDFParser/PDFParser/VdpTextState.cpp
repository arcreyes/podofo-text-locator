#include "VdpTextState.h"


VdpTextState::VdpTextState()
	:
	m_charSpace(0.0),
	m_wordSpace(0.0),
	m_scale(100),
	m_fontResource(""),
	m_fontSize(0.0),
	m_render(0),
	m_rise(0)
{
}

VdpTextState::VdpTextState(double charSpace, double wordSpace, int scale, string fontResource, double fontSize, int render, int rise)
	:
	m_charSpace(charSpace),
	m_wordSpace(wordSpace),
	m_scale(scale),
	m_fontResource(fontResource),
	m_fontSize(fontSize),
	m_render(render),
	m_rise(rise)
{
}

VdpTextState::~VdpTextState()
{
}
