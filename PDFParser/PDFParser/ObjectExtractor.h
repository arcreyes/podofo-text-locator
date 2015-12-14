#pragma once

#include <podofo\podofo.h>
#include <vector>
#include <memory>

#include "ImageObj.h"
#include "TextObj.h"

using namespace PoDoFo;

class ObjectExtractor
{
public:
	ObjectExtractor(const char *pszFilename, DocObjects *pvDocObjects);
	~ObjectExtractor();

	int SingleProcess(void);
	int DivideAndConquer(void);
	int ExtractDocumentObject(PdfMemDocument *pDocument, int iStartPage, int iEndPage);
	int ExtractDocumentObject(PdfMemDocument *pDocument, const char *pszFileName, int iStartPage, int iEndPage);
	int ExtractPageObjects(PdfMemDocument *pDocument, PdfPage *pPage, int iPageIndex, PageObjects *pPageObjects);

	void PrintData();

	static void ClearDocObjects(DocObjects *pvDocObjects);

protected:
	int AddPageObject(int iPageIndex, PageObjects *pPageObjects);
	int GetCurPage();

	HANDLE m_hMutex;

	int m_iCurPageIndex;
	int m_iMaxPage;
	
	std::string m_strFilename;
	PdfMemDocument m_document;
	DocObjects *m_pvDocObjects;
};

