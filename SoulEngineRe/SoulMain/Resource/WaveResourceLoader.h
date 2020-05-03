#pragma once
#include "IResourceLoader.h"
namespace Soul
{
	class WaveResourceLoader : public IResourceLoader
	{
	public:
		WaveResourceLoader();
		bool LoadResource(std::shared_ptr<Resource> handle) override;
		std::vector<std::wstring>& GetPatterns() override;

	private:
		bool ParseWave(std::shared_ptr<Resource> handle);
	};
}