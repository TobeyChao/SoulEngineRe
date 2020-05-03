#pragma once
#include "Resource.h"
namespace Soul
{
	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() = default;
		//file extensions
		virtual std::vector<std::wstring>& GetPatterns() = 0;
		//load the resource with mPatterns(file extensions)
		virtual bool LoadResource(std::shared_ptr<Resource> handle) = 0;
	protected:
		std::vector<std::wstring> mPatterns;
	};
}