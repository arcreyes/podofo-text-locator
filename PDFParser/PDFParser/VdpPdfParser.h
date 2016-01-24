#pragma once

#include <string>

class VdpPdfParser
{
public:
	VdpPdfParser(const char *pszInputFolder, const char *pszOutputFolder);
	~VdpPdfParser();

	int ParsePdfFile(const char *pszPdfPath, const char *pszTemplateFilePath, int templateCount);

protected:
	std::string m_inputFolder;
	std::string m_outputFolder;
};

