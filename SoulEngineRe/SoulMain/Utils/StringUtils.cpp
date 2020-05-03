#include "StringUtils.h"
#include <locale.h>

std::wstring Soul::StringToWstring(const std::string str)
{
	size_t i;
	size_t len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs_s(&i, p, len, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

std::string Soul::WstringToString(const std::wstring str)
{
	size_t i;
	size_t len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char* p = new char[len];
	wcstombs_s(&i, p, len, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}