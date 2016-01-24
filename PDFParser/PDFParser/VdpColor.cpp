#include "VdpColor.h"


VdpColor::VdpColor()
	: 
	m_red(0.0),
	m_green(0.0),
	m_blue(0.0),
	m_cyan(0.0),
	m_magenta(0.0),
	m_yellow(0.0),
	m_black(0.0),
	m_gray(0.0),
	m_eColorNamespace(eColorNamespace_Rgb)
{
}

VdpColor::VdpColor(double red, double green, double blue)
	:
	m_red(red),
	m_green(green),
	m_blue(blue),
	m_cyan(0.0),
	m_magenta(0.0),
	m_yellow(0.0),
	m_black(0.0),
	m_gray(0.0),
	m_eColorNamespace(eColorNamespace_Rgb)
{
}

VdpColor::VdpColor(double cyan, double magenta, double yellow, double black)
	:
	m_red(0.0),
	m_green(0.0),
	m_blue(0.0),
	m_cyan(cyan),
	m_magenta(magenta),
	m_yellow(yellow),
	m_black(black),
	m_gray(0.0),
	m_eColorNamespace(eColorNamespace_Cmyk)
{
}

VdpColor::VdpColor(double gray)
	:
	m_red(0.0),
	m_green(0.0),
	m_blue(0.0),
	m_cyan(0.0),
	m_magenta(0.0),
	m_yellow(0.0),
	m_black(0.0),
	m_gray(gray),
	m_eColorNamespace(eColorNamespace_Gray)
{
}

VdpColor::~VdpColor()
{
}