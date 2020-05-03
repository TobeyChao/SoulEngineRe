#pragma once
#include "IResourceLoader.h"
#include <map>
#include <vector>
namespace Soul
{
	typedef std::map<std::wstring, std::shared_ptr<Resource>> ResourceMap;
	typedef std::vector<std::shared_ptr<IResourceLoader>> ResourceLoaders;
	class ResourceManager
	{
	public:
		//register a resource loader of particular files
		void RegisterLoader(std::shared_ptr<IResourceLoader> loader);
		//register a resource loader of particular files
		void UnregisterLoader(std::shared_ptr<IResourceLoader> loader);
		//get the resource find or load it
		std::shared_ptr<Resource> GetHandle(std::wstring fileName);
	protected:
		//load a file and push it into mResources
		std::shared_ptr<Resource> Load(std::wstring fileName);
		//find a file in mResources
		std::shared_ptr<Resource> Find(std::wstring fileName);
	private:
		ResourceMap mResources;
		ResourceLoaders mResourcesLoaders;
	};
}