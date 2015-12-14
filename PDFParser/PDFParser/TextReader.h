#pragma once

#include <vector>
#include <map>

#include "TextObj.h"
#include "ImageObj.h"


class TextReader
{
public:
	TextReader( const char *pszFileName );
	~TextReader();

	void CopyPagesToFile(const char *pszFilename, int iStart, int iCount);
	int StartExtract(DocObjects *pDocObjects, int count = 1);
	void CreateTemplate(const char *pszFileName, int count = 1);
	void PrintVariableData(DocObjects *pDocObjects);
	void MarkText(const char *pszFileName, DocObjects *pDocObjects);

protected:
	void ExtractObjects(PdfPage* pPage, PageObjects *pPageObjects);
	int AnalyzeVariableData(DocObjects *pDocObjects, int count = 1);
	void AddTextElement(double dCurPosX, double dCurPosY,
		PdfFont* pCurFont, const PdfString & rString);


	void WriteArgumentsAndKeyword(std::vector<PdfVariant> & rArgs, const char* pszKeyword, PdfOutputDevice & rDevice);
	PdfObject* MigrateResource(PdfObject * obj, PdfMemDocument *pTargetDoc, PdfMemDocument *pSourceDoc);

	PdfMemDocument *m_pDoc;

	std::map<std::string, PdfObject*> m_mapMigrate;
};

