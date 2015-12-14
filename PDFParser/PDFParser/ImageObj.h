#pragma once
#include "ObjectBase.h"

class ImageObj :
	public ObjectBase
{
public:
	ImageObj(PdfName imageName, Matrix cm);
	~ImageObj();

protected:
	PdfName m_imageName;

	PdfName GetImageName();

public:
	friend bool operator== (ImageObj &obj1, ImageObj &obj2);
	friend bool operator!= (ImageObj &obj1, ImageObj &obj2);
};

