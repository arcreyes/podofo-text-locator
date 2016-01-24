#pragma once

#include "VdpDocument.h"

#include "podofo\podofo.h"

using namespace PoDoFo;

class VdpTemplateWriter
{
public:
	VdpTemplateWriter(PdfMemDocument *pSourceDocument);
	~VdpTemplateWriter();

	int Write(const char *pszTemplateFilePath, VdpDocument *pVdpDocument, int templatePageCount);

protected:
	int CopyPage(PdfPage *pSrcPage, PdfPage *pDestPage);	
	PdfObject* MigrateResource(PdfObject * obj);
	int RemoveVariableData(PdfPage *pPdfPage, VdpPage *pVdpPage);

protected:
	PdfMemDocument *m_pSourceDocument;
	PdfMemDocument *m_pTargetDocument;

	typedef std::map<std::string, PdfObject*> MigrateMap;
	MigrateMap m_map;

};

