#pragma once

#include "podofo\podofo.h"

#include "VdpDocument.h"

using namespace PoDoFo;

class VdpExtractor
{
public:
	VdpExtractor(PdfMemDocument *pDocument);
	~VdpExtractor();

	void ExtractDocument(VdpDocument *pVdpDocument);

protected:
	typedef enum EPdfKeyword
	{
		//special graphics
		ePdfKeyword_Save = 0,
		ePdfKeyword_Restore,
		ePdfKeyword_CurrentTransformMatrix,

		ePdfKeyword_BeginText,
		ePdfKeyword_EndText,

		ePdfKeyword_CharacterSpacing,
		ePdfKeyword_WordSpacing,
		ePdfKeyword_HorizontalScaling,
		ePdfKeyword_Leading,
		ePdfKeyword_TextFont,
		ePdfKeyword_TextRenderingMode,
		ePdfKeyword_TextRise,
		ePdfKeyword_TextKnockout,

		ePdfKeyword_TextMoveTo,
		ePdfKeyword_TextMoveToNextLine,
		ePdfKeyword_TextMatrix,
		ePdfKeyword_TextMoveToNextLineLeading,

		ePdfKeyword_TextShow,
		ePdfKeyword_TextShowArray,
		ePdfKeyword_TextShowSpacing,
		ePdfKeyword_TextShowLeading,

		ePdfKeyword_Move,
		ePdfKeyword_Line,
		ePdfKeyword_CubicBeizer,
		ePdfKeyword_CubicBeizer2,
		ePdfKeyword_CubicBeizer3,
		ePdfKeyword_ClosePath,
		ePdfKeyword_Rectangle,

		ePdfKeyword_StrokePath,
		ePdfKeyword_CloseAndStrokePath,
		ePdfKeyword_FillPathNonZero,
		ePdfKeyword_FillPathNonZero2,
		ePdfKeyword_FillPathEvenOdd,
		ePdfKeyword_FillStrokePathNonZero,
		ePdfKeyword_FillStrokePathEvenOdd,
		ePdfKeyword_CloseFillStrokePathNonZero,
		ePdfKeyword_CloseFillStrokePathEvenOdd,
		ePdfKeyword_EndPath,


		ePdfKeyword_Undefined = 0xFFFF,
	};

	typedef struct
	{
		EPdfKeyword eKeyword;
		const char *pszValue;
	} TPdfKeyValPair;


protected:
	void ExtractObjects(PdfPage* pPdfPage, VdpDocument *pVdpDocument, VdpPage *pVdpPage);
	std::string GetRawData(PdfString rString);
	std::string GetRawData(PdfArray rArray);

protected:
	PdfMemDocument *m_pPdfDocument;
};

