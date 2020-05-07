#pragma once
#include <string>
namespace Soul
{
	// stringתwstring
	std::wstring StringToWstring(const std::string& str, const std::string& local = "zh_CN.UTF-8");
	// wstringתstring
	std::string WstringToString(const std::wstring& str);
}