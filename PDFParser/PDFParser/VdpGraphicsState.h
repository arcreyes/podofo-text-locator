#pragma once

#include "VdpMatrix.h"
#include "VdpColor.h"

class VdpGraphicsState
{
public:
	VdpGraphicsState();
	~VdpGraphicsState();

	inline void SetCurrentTransformMatrix(VdpMatrix currentTransformMatrix)
	{
		m_currentTransformMatrix = currentTransformMatrix;
	}
	inline const VdpMatrix & GetCurrentTransformMatrix()
	{
		return m_currentTransformMatrix;
	}

	inline void SetNonStrokingColor(VdpColor nonStrokingColor)
	{
		m_nonStrokingColor = nonStrokingColor;
	}
	inline const VdpColor & GetNonStrokingColor()
	{
		return m_nonStrokingColor;
	}

	inline void SetStrokingColor(VdpColor strokingColor)
	{
		m_strokingColor = strokingColor;
	}
	inline const VdpColor & GetStrokingColor()
	{
		return m_strokingColor;
	}

protected:
	VdpMatrix m_currentTransformMatrix;
	VdpColor m_nonStrokingColor;
	VdpColor m_strokingColor;
};

