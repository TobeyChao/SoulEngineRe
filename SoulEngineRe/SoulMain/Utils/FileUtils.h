#pragma once
#include <filesystem>
#include <iostream>
namespace filesys = std::filesystem;

namespace Soul
{
	/*
	Check if given string path is of a Directory
	*/
	bool CheckIfDirectory(std::wstring filePath);

	/*
		Check if given string path is of a file
	*/
	bool CheckIfFile(std::wstring filePath);
}