#include "VdpRect.h"


VdpRect::VdpRect()
	:
	m_x(0.0),
	m_y(0.0),
	m_width(0.0),
	m_height(0.0)
{
}

VdpRect::VdpRect(double x, double y, double width, double height)
	:
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{
}

VdpRect::~VdpRect()
{
}
