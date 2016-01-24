#include "VdpObject.h"


VdpObject::VdpObject(EVdpDataType eDataType)
	:
	m_eDataType(eDataType)
{
}

VdpObject::VdpObject(VdpGraphicsState graphState, VdpRect boundingBox, EVdpDataType eDataType)
	:
	m_eDataType(eDataType),
	m_graphState(graphState),
	m_boundingBox(boundingBox)
{
}


VdpObject::~VdpObject()
{
}
