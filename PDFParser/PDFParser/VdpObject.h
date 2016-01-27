#pragma once

#include <vector>

#include "VdpDefs.h"

#include "VdpGraphicsState.h"
#include "VdpRect.h"


class VdpObject 
{
public:
	VdpObject(EVdpDataType eDataType);
	VdpObject(VdpGraphicsState graphState, VdpRect boundingBox, EVdpDataType eDataType);
	virtual ~VdpObject();

	inline void SetDataType(EVdpDataType eDataType)
	{
		m_eDataType = eDataType;
	}
	inline EVdpDataType GetDataType(void)
	{
		return m_eDataType;
	}

	inline bool IsText(void)
	{
		return m_eDataType == eDataType_Text;
	}
	inline bool IsImage(void)
	{
		return m_eDataType == eDataType_Image;
	}
	inline bool IsGraphics(void)
	{
		return m_eDataType == eDataType_Graphics;
	}

	inline void SetGraphicsState(VdpGraphicsState graphState)
	{
		m_graphState = graphState;
	}
	inline const VdpGraphicsState & GetGraphicsState(void)
	{
		return m_graphState;
	}

	inline void SetBoundingBox(VdpRect boundingBox)
	{
		m_boundingBox = boundingBox;
	}
	inline const VdpRect & GetBoundingBox(void)
	{
		return m_boundingBox;
	}

	inline std::vector<int> & GetPageReferences(void)
	{
		return m_pageReferences;
	}

	inline void SetGraphicsStateResource(std::string gs)
	{
		m_gsResource = gs;
	}

	inline std::string SetGraphicsStateResource(void)
	{
		return m_gsResource;
	}

protected:
	EVdpDataType m_eDataType;
	VdpGraphicsState m_graphState;
	VdpRect m_boundingBox;
	std::vector<int> m_pageReferences;
	std::string m_gsResource;
	
};

