#pragma once

#include <vector>

#include "VdpText.h"

class VdpPage
{
public:
	VdpPage(int pageIndex);
	~VdpPage();

	void AddObject(VdpObject *pObject);
	void DeleteObject(int objectIndex);
	VdpObject *GetObject(int objectIndex);
	int GetObjectCount(void);

	int FindObject(VdpObject *pObject);


	inline void SetPageIndex(int pageIndex)
	{
		m_pageIndex = pageIndex;
	}
	inline const int & GetPageIndex(void)
	{
		return m_pageIndex;
	}

protected:
	int m_pageIndex;

	std::vector<VdpObject*> m_objects;


};

