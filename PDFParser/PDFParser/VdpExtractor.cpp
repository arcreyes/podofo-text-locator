#include "VdpExtractor.h"

#include "VdpText.h"

#include <stack>


VdpExtractor::VdpExtractor(PdfMemDocument *pDocument)
	: m_pPdfDocument(pDocument)
{
	
}


VdpExtractor::~VdpExtractor()
{
}

void VdpExtractor::ExtractDocument(VdpDocument *pVdpDocument)
{
	int pageCount = m_pPdfDocument->GetPageCount();

	pageCount = 10;

	for (int i = 0; i < pageCount; i++)
	{
		PdfPage *pPdfPage = m_pPdfDocument->GetPage(i);

		VdpPage *pVdpPage = new VdpPage(i);

		this->ExtractObjects(pPdfPage, pVdpDocument, pVdpPage);

		pVdpDocument->AddPage(pVdpPage);
	}

	pVdpDocument->Finalize();
}

void VdpExtractor::ExtractObjects(PdfPage* pPdfPage, VdpDocument *pVdpDocument, VdpPage *pVdpPage)
{
	const char*      pszToken = NULL;
	PdfVariant       var;
	EPdfContentsType eType;

	PdfContentsTokenizer tokenizer(pPdfPage);

	bool   bTextBlock = false;

	PdfFont* pCurFont = NULL;

	double dTL = 0.0;

	std::stack<PdfVariant> stack;

	VdpGraphicsState gs;
	std::stack<VdpGraphicsState> gsStack;

	VdpMatrix tm;
	double xpos = 0.0;
	double ypos = 0.0;
	VdpTextState ts;

	while (tokenizer.ReadNext(eType, pszToken, var))
	{
		if (eType == ePdfContentsType_Keyword)
		{
			// support 'l' and 'm' tokens
			if (strcmp(pszToken, "l") == 0 ||
				strcmp(pszToken, "m") == 0)
			{
				stack.pop();
				stack.pop();
			}
			else if (strcmp(pszToken, "BT") == 0)
			{
				bTextBlock = true;
			}
			else if (strcmp(pszToken, "ET") == 0)
			{
				if (!bTextBlock)
					fprintf(stderr, "WARNING: Found ET without BT!\n");

				bTextBlock = false;
			}
			else if (strcmp(pszToken, "q") == 0)
			{
				gsStack.push(gs);
			}
			else if (strcmp(pszToken, "Q") == 0)
			{
				gs = gsStack.top();
				gsStack.pop();
			}
			else if (strcmp(pszToken, "cm") == 0)
			{
				double cm[MATRIX_SIZE];
				cm[5] = stack.top().GetReal();
				stack.pop();
				cm[4] = stack.top().GetReal();
				stack.pop();
				cm[3] = stack.top().GetReal();
				stack.pop();
				cm[2] = stack.top().GetReal();
				stack.pop();
				cm[1] = stack.top().GetReal();
				stack.pop();
				cm[0] = stack.top().GetReal();
				stack.pop();

				gs.SetCurrentTransformMatrix(VdpMatrix(cm));
			}

			else if (strcmp(pszToken, "Do") == 0)
			{
				PdfName imageName = stack.top().GetName();
				stack.pop();

				PdfObject *pObj = pPdfPage->GetFromResources(PdfName("XObject"), imageName);
				if (!pObj)
				{
					PODOFO_RAISE_ERROR_INFO(ePdfError_InvalidHandle, "Cannot create font!");
				}

				PdfObject *pObjSubType = pObj->GetDictionary().GetKey(PdfName::KeySubtype);
				if (pObjSubType && pObjSubType->IsName() &&
					(pObjSubType->GetName().GetName() == "Image"))
				{
					//ImageObj *pObj = new ImageObj(imageName, cm);

					//pPageObjects->push_back(pObj);
				}

			}

			if (bTextBlock)
			{
				if (strcmp(pszToken, "Tf") == 0)
				{
					ts.GetFontSize();

					ts.GetFontSize() = stack.top().GetReal();
					stack.pop();

					PdfName fontName = stack.top().GetName();
					PdfObject* pFont = pPdfPage->GetFromResources(PdfName("Font"), fontName);
					if (!pFont)
					{
						PODOFO_RAISE_ERROR_INFO(ePdfError_InvalidHandle, "Cannot create font!");
					}
					ts.SetFontResource(fontName.GetName());

					pCurFont = m_pPdfDocument->GetFont(pFont);
					if (!pCurFont)
					{
						fprintf(stderr, "WARNING: Unable to create font for object %i %i R\n",
							pFont->Reference().ObjectNumber(),
							pFont->Reference().GenerationNumber());
					}

					//printf("\n/%s %g Tf\n", fontName.GetName().c_str(), fontSize);
					//printf("Font Size       : %g\n", pCurFont->GetFontSize());
					//printf("Font Scale      : %g\n", pCurFont->GetFontScale());
					//printf("Font Char Space : %g\n", pCurFont->GetFontCharSpace());
					//printf("Font Word Space : %g\n", pCurFont->GetWordSpace());
					//printf("Bold            : %s\n", pCurFont->IsBold() ? "TRUE" : "FALSE" );
					//printf("Italic          : %s\n", pCurFont->IsItalic() ? "TRUE" : "FALSE");
					//printf("Underlined      : %s\n", pCurFont->IsUnderlined() ? "TRUE" : "FALSE");
					//printf("Ascent          : %g\n", pCurFont->GetFontMetrics()->GetAscent());
					//printf("Descent         : %g\n", pCurFont->GetFontMetrics()->GetDescent());
				}
				else if (strcmp(pszToken, "'") == 0)
				{
					ypos += dTL;
					//Tm.Translate(0, dTL);

					//TextObj *pObj = new TextObj(stack.top().GetString(), pCurFont, fontSize, fontName, Tm, Tw, Tc);
					stack.pop();

					//pPageObjects->push_back(pObj);
				}
				else if (strcmp(pszToken, "\"") == 0)
				{
					PdfString rString = stack.top().GetString();
					stack.pop();

					ts.SetCharacterSpacing(stack.top().GetReal());
					stack.pop(); // remove char spacing from stack
					ts.SetWordSpacing(stack.top().GetReal());
					stack.pop(); // remove word spacing from stack

					ypos += dTL;

					//pPageObjects->push_back(pObj);
				}
				else if (strcmp(pszToken, "Tj") == 0)
				{
					float fontSize  = static_cast<float>(ts.GetFontSize() * tm.GetXScale());
					float charSpace = static_cast<float>(ts.GetCharacterSpacing() * tm.GetXScale());
					float wordSpace = static_cast<float>(ts.GetWordSpacing() * tm.GetXScale());

					pCurFont->SetFontSize(fontSize);
					pCurFont->SetFontCharSpace(charSpace);
					pCurFont->SetWordSpace(wordSpace);

					PdfString rString = stack.top().GetString();
					stack.pop();

					PdfString unicodeString = pCurFont->GetEncoding()->ConvertToUnicode(rString, pCurFont);
					double xactual = tm.GetXTranslate() + (xpos * tm.GetXScale());
					double yactual = tm.GetYTranslate() + (ypos * tm.GetYScale()) + pCurFont->GetFontMetrics()->GetDescent();
					double width = pCurFont->GetFontMetrics()->StringWidth(unicodeString.GetUnicode(), unicodeString.GetUnicodeLength());
					double height = pCurFont->GetFontMetrics()->GetAscent() - pCurFont->GetFontMetrics()->GetDescent();


					VdpText *pText = new VdpText(this->GetRawData(rString), false, xpos, ypos, ts, gs, VdpRect(xactual, yactual, width, height));


					if (pVdpDocument->FindObject(pText, pVdpPage->GetPageIndex()) == -1)
					{
						pVdpPage->AddObject(pText);
					}
					else
					{
						delete pText;
					}
				}
				else if (strcmp(pszToken, "TJ") == 0)
				{
					float fontSize = static_cast<float>(ts.GetFontSize() * tm.GetXScale());
					float charSpace = static_cast<float>(ts.GetCharacterSpacing() * tm.GetXScale());
					float wordSpace = static_cast<float>(ts.GetWordSpacing() * tm.GetXScale());

					pCurFont->SetFontSize(fontSize);
					pCurFont->SetFontCharSpace(charSpace);
					pCurFont->SetWordSpace(wordSpace);

					PdfArray pdfArray = stack.top().GetArray();
					stack.pop();

					double dTJOffset = tm.GetXTranslate() + (xpos * tm.GetXScale());

					for (int i = 0; i < pdfArray.size(); i++)
					{
						if (pdfArray[i].IsNumber())
						{
							dTJOffset = (dTJOffset - ((pdfArray[i].GetNumber() / 1000.0) * tm.GetXScale()));
						}
						else if (pdfArray[i].IsString() || pdfArray[i].IsHexString())
						{

							dTJOffset += pCurFont->GetFontMetrics()->StringWidth(pdfArray[i].GetString());
						}
					}

					double xactual = tm.GetXTranslate() + (xpos * tm.GetXScale());
					double yactual = tm.GetYTranslate() + (ypos * tm.GetYScale()) + pCurFont->GetFontMetrics()->GetDescent();
					double width = dTJOffset - xactual;
					double height = pCurFont->GetFontMetrics()->GetAscent() - pCurFont->GetFontMetrics()->GetDescent();

					VdpText *pText = new VdpText(this->GetRawData(pdfArray), false, xpos, ypos, ts, gs, VdpRect(xactual, yactual, width, height));


					if (pVdpDocument->FindObject(pText, pVdpPage->GetPageIndex()) == -1)
					{
						pVdpPage->AddObject(pText);
					}
					else
					{
						delete pText;
					}

				}
				else if (strcmp(pszToken, "Tm") == 0)
				{
					double mat[MATRIX_SIZE];
					mat[5] = stack.top().GetReal();
					stack.pop();
					mat[4] = stack.top().GetReal();
					stack.pop();
					mat[3] = stack.top().GetReal();
					stack.pop();
					mat[2] = stack.top().GetReal();
					stack.pop();
					mat[1] = stack.top().GetReal();
					stack.pop();
					mat[0] = stack.top().GetReal();
					stack.pop();

					tm.SetMatrix(mat);

					xpos = 0.0;
					ypos = 0.0;
				}
				else if (strcmp(pszToken, "Td") == 0)
				{
					double dTy = stack.top().GetReal();
					stack.pop();
					double dTx = stack.top().GetReal();
					stack.pop();

					xpos += dTx;
					ypos += dTy;
				}
				else if (strcmp(pszToken, "TD") == 0)
				{
					double dTy = stack.top().GetReal();
					stack.pop();
					double dTx = stack.top().GetReal();
					stack.pop();

					dTL = dTy;

					xpos += dTx;
					ypos += dTy;
				}
				else if (strcmp(pszToken, "T*") == 0)
				{
					ypos += dTL;
				}
				else if (strcmp(pszToken, "Tw") == 0)
				{
					ts.SetWordSpacing(stack.top().GetReal());
					stack.pop(); // remove word spacing from stack
				}
				else if (strcmp(pszToken, "Tc") == 0)
				{
					ts.SetCharacterSpacing(stack.top().GetReal());
					stack.pop(); // remove char spacing from stack
				}
			}
		}
		else if (eType == ePdfContentsType_Variant)
		{
			stack.push(var);
		}
		else
		{
			// Impossible; type must be keyword or variant
			PODOFO_RAISE_ERROR(ePdfError_InternalLogic);
		}
	}

}


std::string VdpExtractor::GetRawData(PdfString rString)
{

	PdfRefCountedBuffer buffer;
	PdfOutputDevice device(&buffer);

	rString.Write(&device, ePdfWriteMode_Compact);

	return std::string(buffer.GetBuffer(), buffer.GetBuffer() + buffer.GetSize());
}

std::string VdpExtractor::GetRawData(PdfArray rArray)
{
	int count = (int)rArray.size();

	PdfRefCountedBuffer buffer;
	PdfOutputDevice device(&buffer);

	rArray.Write(&device, ePdfWriteMode_Compact);

	return std::string(buffer.GetBuffer(), buffer.GetBuffer() + buffer.GetSize());
}
