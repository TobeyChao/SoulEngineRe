#include "FileUtils.h"

namespace Soul
{
	bool CheckIfDirectory(std::wstring filePath)

	{
		try {
			// Create a Path object from given path string
			filesys::path pathObj(filePath);
			// Check if path exists and is of a directory file
			if (filesys::exists(pathObj) && filesys::is_directory(pathObj))
				return true;
		}
		catch (filesys::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
		}
		return false;
	}

	bool CheckIfFile(std::wstring filePath)
	{
		try {
			// Create a Path object from given path string
			filesys::path pathObj(filePath);
			// Check if path exists and is of a regular file
			if (filesys::exists(pathObj) && filesys::is_regular_file(pathObj))
				return true;
		}
		catch (filesys::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
		}
		return false;
	}
}