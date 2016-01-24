#include <iostream>
#include <fstream>

#include "VdpDocument.h"


VdpDocument::VdpDocument()
	:
	m_templateCount(DEFAULT_TEMPLATE_COUNT)
{
}


VdpDocument::~VdpDocument()
{
	while (!m_pages.empty())
	{
		VdpPage *p = m_pages.back();
		m_pages.pop_back();
		delete p;
		p = NULL;
	}
}

void VdpDocument::AddPage(VdpPage *pPage)
{
	m_pages.push_back(pPage);
}

VdpPage *VdpDocument::GetPage(int pageIndex)
{
	return m_pages.at(pageIndex);
}

int VdpDocument::GetPageCount(void)
{
	return (int)m_pages.size();
}

int VdpDocument::FindObject(VdpObject *pObject, int pageIndex)
{
	int pageCount = this->GetPageCount();

	if (pageCount < m_templateCount)
	{
		return -1;
	}

	int templateIndex = pageIndex % m_templateCount;

	VdpPage *p = this->GetPage(templateIndex);

	int ret = p->FindObject(pObject);
	if (ret != -1)
	{
		p->GetObject(ret)->GetPageReferences().push_back(pageIndex);
	}

	return ret;
}

int VdpDocument::Finalize(void)
{
	int pageCount = this->GetPageCount();

	if (pageCount < m_templateCount)
	{
		return -1;
	}

	int ret = 0;

	for (int i = 0; i < m_templateCount; i++)
	{
		VdpPage *p = this->GetPage(i);

		int objectCount = p->GetObjectCount();

		for (int j = objectCount -1; j >= 0; j--)
		{
			VdpObject *o = p->GetObject(j);

			if (o->GetPageReferences().size() == pageCount-1)
			{
				p->DeleteObject(j);
			}
		}
	}
}

void VdpDocument::Dump(const char *pszFilePath)
{

	std::ofstream myfile;
	myfile.open(pszFilePath);
	


	int pageCount = this->GetPageCount();

	for (int i = 0; i < pageCount; i++)
	{
		VdpPage *p = this->GetPage(i);

		myfile << "Page: " << i << "\n";

		int objectCount = p->GetObjectCount();

		for (int j = 0; j < objectCount; j++)
		{
			VdpObject *o = p->GetObject(j);

			if (o->IsText())
			{
				VdpText *t = static_cast<VdpText*>(o);
				myfile << "\t" << t->GetRawData() << "\n";
			}
		}
	}

	myfile.close();

}