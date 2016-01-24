#pragma once

#include <string>

class VdpUtility
{
public:
	VdpUtility();
	~VdpUtility();

	static std::string PathCombine(std::string path, std::string filename);
};

