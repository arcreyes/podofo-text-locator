// PDFParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "DataManager.h"
#include "TextReader.h"
#include "ObjectExtractor.h"

#include "VdpPdfParser.h"

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

int doTest5(const char *pszInputFolder, const char *pszOutputFolder, const char *pszPdfPath, const char *pszTemplateFilePath, int templateCount)
{
	VdpPdfParser parser(pszInputFolder, pszOutputFolder);

	int ret = parser.ParsePdfFile(pszPdfPath, pszTemplateFilePath, templateCount);
	if (ret < 0)
	{
		printf("ret=[%d]\n", ret);
	}
	return ret;
}

#define INPUT_FOLDER	"F:\\VS2012\\podofo-text-locator\\PDFParser\\Test Data"
#define OUTPUT_FOLDER	"F:\\VS2012\\podofo-text-locator\\PDFParser\\Test Data\\output"

typedef struct
{
	const char *pszInputFolder;
	const char *pszOutputFolder;
	const char *pszPdfName;
	int			templateCount;
} TParamaters;

static TParamaters params[] =
{
	{ INPUT_FOLDER, OUTPUT_FOLDER, "1_Weseraue500page.pdf", 1 }, //0
	{ INPUT_FOLDER, OUTPUT_FOLDER, "1_Weseraue500page_from_CheckPPML.pdf", 1 }, //1

	// terminator
	{ NULL, NULL, 0 }
};

std::string PathCombine(string path, string filename)
{
	int len = (int)path.length();

	if (path.at(len - 1) != '\\')
	{
		path.append("\\");
	}

	return path + filename;
}

//std::string PathRemoveExtension(string filePath)
//{
//	int index = filePath.find_last_of(".");
//}

int doTest(int testCase)
{
	char szTemplateFilename[MAX_PATH] = "";
	sprintf(szTemplateFilename, "template%d.pdf", testCase);

	return doTest5(params[testCase].pszInputFolder, 
		params[testCase].pszOutputFolder, 
		params[testCase].pszPdfName,
		szTemplateFilename,
		params[testCase].templateCount);
}

int main(int argc, char* argv[])
{



	doTest(0);

	system( "pause" );

	return 0;
}

