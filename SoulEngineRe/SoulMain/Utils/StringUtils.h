#pragma once
#include <string>
namespace Soul
{
	// stringתwstring
	std::wstring StringToWstring(const std::string str);
	// wstringתstring
	std::string WstringToString(const std::wstring str);
}