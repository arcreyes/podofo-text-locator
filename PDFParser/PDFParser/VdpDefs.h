#ifndef _VDPNODEDEFS_H_
#define _VDPNODEDEFS_H_

#include <string>

#define MAX_FONT					32

#define DEFAULT_TEMPLATE_COUNT		1

typedef enum EVdpDataType
{
	eDataType_Text = 0,
	eDataType_Image,
	eDataType_Graphics
};

typedef enum EVdpUnit
{
	eUnit_PostScript = 0,
	eUnit_Inches,
	eUnit_Millimeters
};

typedef enum EVdpInFileFormat
{
	eInFileFormat_Pdf = 0,
	eInFileFormat_Excel,
	eInFileFormat_Csv,
};

typedef enum EVdpOutFileFormat
{
	eOutFileFormat_Pdf = 0,
	eOutFileFormat_PdfVt1,
	eOutFileFormat_PdfVt2,
	eOutFileFormat_Ppml
};

typedef enum EVdpAlignment
{
	eAlignment_Left = 0,
	eAlignment_Center,
	eAlignment_Right,
};

typedef enum EVdpColorSpace
{
	eColorNamespace_Rgb = 0,
	eColorNamespace_Cmyk,
	eColorNamespace_Gray
};

typedef struct 
{
	double x;
	double y;
	double width;
	double height;
	EVdpUnit eUnit;
} TFieldPosition;

typedef struct 
{
	EVdpColorSpace eFormat;
	double red;
	double green;
	double blue;
	double cyan;
	double magenta;
	double yellow;
	double black;
	double gray;
} TTextColor;

typedef struct 
{
	char fontName[MAX_FONT];
	bool leftToRight;
	double fontSize;
	TTextColor textColor;
	bool bold;
	bool italic;
	bool underline;
} TFieldText;

typedef struct 
{
	bool autoFit;
} TFieldImage;

typedef struct 
{
	EVdpDataType eDataType;
	TFieldPosition position;
	EVdpAlignment eAlignment;
} TFieldInformation;

#endif //_VDPNODEDEFS_H_