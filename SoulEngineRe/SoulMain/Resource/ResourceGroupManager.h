#pragma once
#include "../Utils/Singleton.h"
#include "ResourceManager.h"
namespace Soul
{
	class ResourceGroupManager : public Singleton<ResourceGroupManager>
	{
	public:
		ResourceGroupManager();
		//register a new resource manager
		void RegisterResourceManager(const std::wstring& resourceType, ResourceManager* rm);
		//unregister a resource manager whose name is resourceType
		void UnregisterResourceManager(const std::wstring& resourceType);
	protected:
		typedef std::map<std::wstring, ResourceManager*> ResourceManagerMap;
		ResourceManagerMap mResourceManagerMap;
	};
}