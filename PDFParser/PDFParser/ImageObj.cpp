#include "ImageObj.h"


ImageObj::ImageObj(PdfName imageName, Matrix cm)
	: ObjectBase(eImageObj)
{
	m_imageName = imageName;
	SetTransformMatrix(cm);
}


ImageObj::~ImageObj()
{
}

PdfName ImageObj::GetImageName()
{
	return m_imageName;
}

bool operator== (ImageObj &obj1, ImageObj &obj2)
{
	return obj1.GetImageName() == obj2.GetImageName();
}

bool operator!= (ImageObj &obj1, ImageObj &obj2)
{
	return !(obj1 == obj2);
}