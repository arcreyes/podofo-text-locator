#include "VdpPdfParser.h"

#include "podofo\podofo.h"

#include "VdpTemplateWriter.h"
#include "VdpExtractor.h"
#include "VdpUtility.h"

using namespace PoDoFo;

VdpPdfParser::VdpPdfParser(const char *pszInputFolder, const char *pszOutputFolder)
	:
	m_inputFolder(pszInputFolder),
	m_outputFolder(pszOutputFolder)
{
}


VdpPdfParser::~VdpPdfParser()
{
}

int VdpPdfParser::ParsePdfFile(const char *pszPdfName, const char *pszTemplateFilePath, int templateCount)
{

	int iRet = 0;
	try
	{
		PdfMemDocument document(VdpUtility::PathCombine(m_inputFolder, pszPdfName).c_str());


		VdpDocument vdpDoc;
		vdpDoc.SetTemplateCount(templateCount);

		VdpExtractor extractor(&document);
		extractor.ExtractDocument(&vdpDoc);

#ifdef _DEBUG
		vdpDoc.Dump(VdpUtility::PathCombine(m_outputFolder, "dump.txt").c_str());
#endif
			
		VdpTemplateWriter vdpTemplate(&document);
		iRet = vdpTemplate.Write(VdpUtility::PathCombine(m_outputFolder, pszTemplateFilePath).c_str(), &vdpDoc, templateCount);
	}
	catch (const PdfError & e)
	{
		e.PrintErrorMsg();
		iRet = -3;
	}

	return iRet;
}
