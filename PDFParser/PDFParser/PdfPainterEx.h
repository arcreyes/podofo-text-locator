#pragma once
//#include "C:\Program Files (x86)\PoDoFo\include\podofo\doc\PdfPainter.h"

#include <podofo\podofo.h>

using namespace PoDoFo;

class PdfPainterEx :
	public PdfPainter
{
public:
	PdfPainterEx();
	~PdfPainterEx();

	void SetTextTransformationMatrix(double a, double b, double c, double d, double e, double f);

	void AddTextEx(const PdfString & sText);
};

