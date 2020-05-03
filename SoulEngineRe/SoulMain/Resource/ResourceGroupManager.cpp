#include "ResourceGroupManager.h"

namespace Soul
{
	template<> ResourceGroupManager* Singleton<ResourceGroupManager>::mSingleton = nullptr;

	ResourceGroupManager::ResourceGroupManager()
	{
	}

	void ResourceGroupManager::RegisterResourceManager(const std::wstring& resourceType, ResourceManager* rm)
	{
		mResourceManagerMap[resourceType] = rm;
	}

	void ResourceGroupManager::UnregisterResourceManager(const std::wstring& resourceType)
	{
		ResourceManagerMap::iterator i = mResourceManagerMap.find(resourceType);
		if (i != mResourceManagerMap.end())
		{
			mResourceManagerMap.erase(i);
		}
	}
}