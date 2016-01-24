#pragma once

#include "VdpPage.h"

class VdpDocument
{
public:
	VdpDocument();
	~VdpDocument();

	void AddPage(VdpPage *pPage);
	VdpPage *GetPage(int pageIndex);
	int GetPageCount(void);

	int FindObject(VdpObject *pObject, int pageIndex);

	int Finalize(void);

	void Dump(const char *pszFilePath);

	inline void SetTemplateCount(int templateCount)
	{
		m_templateCount = templateCount;
	}
	inline const int & GetTemplateCount(void)
	{
		return m_templateCount;
	}

protected:
	std::vector<VdpPage*> m_pages;

	int m_templateCount;
};

