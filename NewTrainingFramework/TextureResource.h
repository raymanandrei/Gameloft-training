#pragma once
#include <string>
struct TextureResource
{
	std::string id;
	std::string file;
	std::string min_filter;
	std::string mag_filter;
	std::string wrap_s;
	std::string wrap_t;
};