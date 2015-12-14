#pragma once

#include "TextObj.h"

class DataManager
{
private:
	static bool bInstanceFlag;
	static DataManager *single;
	DataManager();

public:
	static DataManager* GetInstance();
	void method();
	~DataManager();



	DocObjects *GetDocumentObjects();
	void ClearPageObjects(PageObjects *pageObjects);

protected:
	DocObjects m_docObjects;
};

