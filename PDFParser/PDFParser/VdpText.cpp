#include "VdpText.h"


VdpText::VdpText()
	: VdpObject(eDataType_Text),
	m_rawData(""),
	m_bArray(false),
	m_xpos(0.0),
	m_ypos(0.0)
{
}

VdpText::VdpText(std::string rawData, bool bArray, double xpos, double ypos, VdpTextState textState, VdpGraphicsState graphState, VdpRect boundingBox)
	: VdpObject(graphState, boundingBox, eDataType_Text),
	m_rawData(rawData),
	m_bArray(bArray),
	m_xpos(xpos),
	m_ypos(ypos),
	m_textState(textState)
{
}

VdpText::~VdpText()
{
}
