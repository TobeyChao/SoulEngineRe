#include "ResourceManager.h"
namespace Soul
{
	void ResourceManager::RegisterLoader(std::shared_ptr<IResourceLoader> loader)
	{
		mResourcesLoaders.push_back(loader);
	}

	void ResourceManager::UnregisterLoader(std::shared_ptr<IResourceLoader> loader)
	{
		auto it = std::find(mResourcesLoaders.begin(), mResourcesLoaders.end(), loader);
		if (it != mResourcesLoaders.end())
		{
			mResourcesLoaders.erase(it);
		}
	}

	std::shared_ptr<Resource> ResourceManager::GetHandle(std::wstring fileName)
	{
		std::shared_ptr<Resource> handle(Find(fileName));
		if (handle == nullptr)
		{
			handle = Load(fileName);
		}
		return handle;
	}

	std::shared_ptr<Resource> ResourceManager::Load(std::wstring fileName)
	{
		std::shared_ptr<IResourceLoader> loader;
		auto handle = std::make_shared<Resource>(fileName);
		std::wstring pattern = fileName.substr(fileName.find_last_of(L".") + 1);
		for (auto it = mResourcesLoaders.begin(); it != mResourcesLoaders.end(); it++)
		{
			std::shared_ptr<IResourceLoader> testLoader = *it;

			if (std::find(testLoader->GetPatterns().begin(), testLoader->GetPatterns().end(), pattern) !=
				testLoader->GetPatterns().end())
			{
				loader = testLoader;
				break;
			}
		}
		// Start Load
		if (!loader)
		{
			return handle;
		}

		loader->LoadResource(handle);

		if (handle)
		{
			mResources[fileName] = handle;
		}
		return handle;
	}

	std::shared_ptr<Resource> ResourceManager::Find(std::wstring fileName)
	{
		const auto it = mResources.find(fileName);
		if (it == mResources.end())
		{
			return std::shared_ptr<Resource>();
		}
		return it->second;
	}
}