#pragma once

#include "TextObj.h"
#include "ImageObj.h"

class TextMarker
{
public:
	TextMarker();
	~TextMarker();

	void MarkText(const char* pszFileName, DocObjects *pDocObjects);
};

