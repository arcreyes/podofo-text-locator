#include "VdpUtility.h"




VdpUtility::VdpUtility()
{
}


VdpUtility::~VdpUtility()
{
}

std::string VdpUtility::PathCombine(std::string path, std::string filename)
{
	int len = path.length();

	if (path.at(len - 1) != '\\')
	{
		path.append("\\");
	}

	return path + filename;
}