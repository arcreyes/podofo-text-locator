// PDFParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "DataManager.h"
#include "TextReader.h"
#include "ObjectExtractor.h"

#include <string>

using namespace std;

int doTest1(string strPath, string strFileName)
{
	TextReader reader((strPath + strFileName).c_str());

	DataManager *pDM = DataManager::GetInstance();

	reader.StartExtract(pDM->GetDocumentObjects());

	
	//size_t lastindex = strFileName.find_last_of(".");
	//string strTemplate = strFileName.substr(0, lastindex) + "_template.pdf";
	//reader.CreateTemplate((strPath + strTemplate).c_str(), 2);

	size_t lastindex = strFileName.find_last_of(".");
	string strTemplate = strFileName.substr(0, lastindex) + "_marked.pdf";
	reader.MarkText((strPath + strTemplate).c_str(), pDM->GetDocumentObjects());

	return 0;
}

int doTest2(string strPath, string strFileName)
{
	DocObjects vDocObjects;
	ObjectExtractor objExtractor((strPath + strFileName).c_str(), &vDocObjects);

	objExtractor.DivideAndConquer();
	


	objExtractor.PrintData();

	ObjectExtractor::ClearDocObjects(&vDocObjects);

	return 0;
}

int doTest3(string strPath, string strFileName)
{
	DocObjects vDocObjects;
	ObjectExtractor objExtractor((strPath + strFileName).c_str(), &vDocObjects);

	objExtractor.SingleProcess();


	ObjectExtractor::ClearDocObjects(&vDocObjects);
	return 0;
}

int doTest4(string strPath, string strFileName)
{
	TextReader reader((strPath + strFileName).c_str());

	size_t lastindex = strFileName.find_last_of(".");
	string strTemplate = strFileName.substr(0, lastindex) + "_2pages.pdf";

	reader.CopyPagesToFile((strPath + strTemplate).c_str(), 0, 2);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	string strPath = "F:\\VS2012\\PDFParser\\Test Data\\";
	string strFileName = "60 K Pages_template.pdf";
	//string strFileName = "1_Weseraue500page.pdf";
	
	doTest1(strPath, strFileName);

	system( "pause" );

	return 0;
}

