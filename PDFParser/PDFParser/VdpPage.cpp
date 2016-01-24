#include "VdpPage.h"


VdpPage::VdpPage(int pageIndex)
	: m_pageIndex(pageIndex)
{
}


VdpPage::~VdpPage()
{
	while (!m_objects.empty())
	{
		VdpObject *o = m_objects.back();
		m_objects.pop_back();
		delete o;
		o = NULL;
	}
}

void VdpPage::AddObject(VdpObject *pObject)
{
	m_objects.push_back(pObject);
}

void VdpPage::DeleteObject(int objectIndex)
{
	VdpObject *o = m_objects.at(objectIndex);
	delete o;
	o = NULL;

	m_objects.erase(m_objects.begin() + objectIndex);
}

VdpObject *VdpPage::GetObject(int objectIndex)
{
	return m_objects.at(objectIndex);
}

int VdpPage::GetObjectCount(void)
{
	return (int)m_objects.size();
}

int VdpPage::FindObject(VdpObject *pObject)
{
	int objectCount = this->GetObjectCount();

	int i = 0;
	for (i = 0; i < objectCount; i++)
	{
		VdpObject *o = this->GetObject(i);
		if (o->IsText() == pObject->IsText())
		{
			if (*static_cast<VdpText*>(o) == *static_cast<VdpText*>(pObject))
			{
				break;
			}
		}
	}

	if (i >= objectCount)
	{
		i = -1;
	}

	return i;
}