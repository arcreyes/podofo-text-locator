#pragma once

#include "ObjectBase.h"

class TextObj : public ObjectBase
{
public:
	TextObj(PdfString pdfString, PdfFont *pFont, double dFontSize, PdfName fontName, Matrix Tm, double Tw, double Tc);
	TextObj(PdfArray arrayString, PdfFont *pFont, double dFontSize, PdfName fontName, Matrix Tm, double Tw, double Tc);
	TextObj(std::string strValue, PdfName fontName, double dFontSize);
	~TextObj();

	void SetTextMatrix(Matrix Tm);
	Matrix GetTextMatrix();

	PdfArray GetPdfStringArray();
	std::string GetValue();
	PdfFont *GetFont();
	PdfName GetFontReference();
	std::string GetFontName();
	double GetFontSize();

	void SetWordSpacing(double Tw);
	double GetWordSpacing();
	void SetCharacterSpacing(double Tc);
	double GetCharacterSpacing();

	void SetBoundingBox(double dX, double dY, double dWeight, double dHeight);

	double GetPosX();
	double GetPosY();
	double GetWidth();
	double GetHeight();

protected:
	Matrix m_Tm;
	double m_fontSize;
	PdfArray m_arrayString;

	PdfFont *m_pFont;
	PdfName m_fontName;

	double m_Tw;
	double m_Tc;

	void ComputePosition();

	double m_dX;
	double m_dY;
	double m_dWidth;
	double m_dHeight;

public:
	friend bool operator== (TextObj &obj1, TextObj &obj2);
	friend bool operator!= (TextObj &obj1, TextObj &obj2);

};

