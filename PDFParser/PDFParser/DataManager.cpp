#include "DataManager.h"


bool DataManager::bInstanceFlag = false;
DataManager* DataManager::single = NULL;

DataManager* DataManager::GetInstance()
{
	if (!bInstanceFlag)
	{
		single = new DataManager();
		bInstanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}

DataManager::DataManager()
{
	while (!m_docObjects.empty())
	{
		PageObjects *pPageObj = m_docObjects.back();
		m_docObjects.pop_back();

		ClearPageObjects(pPageObj);

		delete pPageObj;
		pPageObj = NULL;
	}
}

DataManager::~DataManager()
{
	bInstanceFlag = false;
}

DocObjects *DataManager::GetDocumentObjects()
{
	return &m_docObjects;
}

void DataManager::ClearPageObjects(PageObjects *pageObjects)
{
	while (!pageObjects->empty())
	{
		ObjectBase *pObj = pageObjects->back();
		pageObjects->pop_back();

		delete pObj;
		pObj = NULL;
	}
}