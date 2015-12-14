#include "TextObj.h"


TextObj::TextObj(PdfString pdfString, PdfFont *pFont, double dFontSize, PdfName fontName, Matrix Tm, double Tw, double Tc)
	: ObjectBase(eTextObj)
{
	m_arrayString.push_back(pdfString);
	m_pFont = pFont;
	m_fontSize = dFontSize;
	m_fontName = fontName;
	m_Tm = Tm;
	m_Tw = Tw;
	m_Tc = Tc;

	//ComputePosition();
}

TextObj::TextObj(PdfArray arrayString, PdfFont *pFont, double dFontSize, PdfName fontName, Matrix Tm, double Tw, double Tc)
	: ObjectBase(eTextObj)
{
	m_arrayString = arrayString;
	m_pFont = pFont;
	m_fontSize = dFontSize;
	m_fontName = fontName;
	m_Tm = Tm;
	m_Tw = Tw;
	m_Tc = Tc;

	//ComputePosition();
}

TextObj::TextObj(std::string strValue, PdfName fontName, double dFontSize)
	: ObjectBase(eTextObj)
{
	m_arrayString.push_back(PdfString(strValue));
	m_pFont = NULL;
	m_fontName = fontName;
	m_fontSize = dFontSize;	
}

TextObj::~TextObj()
{

}

void TextObj::SetTextMatrix(Matrix Tm)
{
	m_Tm = Tm;

	ComputePosition();
}

Matrix TextObj::GetTextMatrix()
{
	return m_Tm;
}

PdfArray TextObj::GetPdfStringArray()
{
	return m_arrayString;
}

std::string TextObj::GetValue()
{
	int size = m_arrayString.size();
	std::string value;

	for (int i = 0; i < size; i++)
	{
		if (m_arrayString[i].IsHexString() || m_arrayString[i].IsString())
		{ 
			value += m_arrayString[i].GetString().GetString();
		}
	}

	return value;
}

PdfFont *TextObj::GetFont()
{
	return m_pFont;
}

PdfName TextObj::GetFontReference()
{
	return m_fontName;
}

std::string TextObj::GetFontName()
{
	return m_fontName.GetName();
}

double TextObj::GetFontSize()
{
	return m_fontSize;
}

void TextObj::SetWordSpacing(double Tw)
{
	m_Tw = Tw;
}

double TextObj::GetWordSpacing()
{
	return m_Tw;
}

void TextObj::SetCharacterSpacing(double Tc)
{
	m_Tc = Tc;
}

double TextObj::GetCharacterSpacing()
{
	return m_Tc;
}

void TextObj::SetBoundingBox(double dX, double dY, double dWeight, double dHeight)
{
	m_dX = dX;
	m_dY = dY;
	m_dWidth = dWeight;
	m_dHeight = dHeight;
}

double TextObj::GetPosX()
{
	return m_dX;
}

double TextObj::GetPosY()
{
	
	return m_dY;
}

double TextObj::GetWidth()
{
	return m_dWidth;
}

double TextObj::GetHeight()
{
	return m_dHeight;
}

void TextObj::ComputePosition()
{
	m_dX = m_Tm.GetE();
	m_dY = m_Tm.GetF() + m_pFont->GetFontMetrics()->GetDescent();
	m_dWidth = m_pFont->GetFontMetrics()->StringWidth(this->GetValue());
	m_dHeight = m_pFont->GetFontMetrics()->GetAscent() - m_pFont->GetFontMetrics()->GetDescent();
}

bool operator== (TextObj &obj1, TextObj &obj2)
{
	return obj1.GetValue() == obj2.GetValue() &&
		obj1.m_fontName == obj2.m_fontName &&
		obj1.m_fontSize == obj2.m_fontSize &&
		obj1.m_Tm == obj2.m_Tm;

}

bool operator!= (TextObj &obj1, TextObj &obj2)
{
	return !(obj1 == obj2);
}

