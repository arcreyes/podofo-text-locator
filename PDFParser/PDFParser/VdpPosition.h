#pragma once

#include "VdpMatrix.h"

class VdpPosition
{
public:
	VdpPosition();
	VdpPosition(VdpMatrix ctm);
	~VdpPosition();

	inline void SetCurrentTransformMatrix(VdpMatrix ctm)
	{
		m_currentTransformMatrix = ctm;
	}
	inline VdpMatrix SetCurrentTransformMatrix(void)
	{
		return m_currentTransformMatrix;
	}

protected:
	VdpMatrix m_currentTransformMatrix;

};

