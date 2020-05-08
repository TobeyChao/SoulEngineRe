#include "RenderSystem.h"
#include <fstream>

namespace Soul
{
	RenderSystem::RenderSystem()
		:
		mActiveRenderTarget(nullptr),
		mActiveViewport(nullptr),
		mShaderManager(nullptr),
		mTextureManager(nullptr),
		mGpuBufferManager(nullptr)
	{
	}

	RenderSystem::~RenderSystem()
	{
		// 销毁所有RenderTarget
		for (auto it = mRenderTargets.begin(); it != mRenderTargets.end();)
		{
			RenderTarget* current = it->second;
			if (mActiveRenderTarget && current == mActiveRenderTarget)
			{
				++it;
			}
			else
			{
				it = mRenderTargets.erase(it);
				delete current;
			}
		}
		if (mActiveRenderTarget)
		{
			delete mActiveRenderTarget;
			mActiveRenderTarget = nullptr;
		}
		mRenderTargets.clear();

		// 销毁ShaderManager
		if (mShaderManager)
		{
			delete mShaderManager;
			mShaderManager = nullptr;
		}

		// 销毁mTextureManager
		if (mTextureManager)
		{
			delete mTextureManager;
			mTextureManager = nullptr;
		}

		// 销毁mGpuBufferManager
		if (mGpuBufferManager)
		{
			delete mGpuBufferManager;
			mGpuBufferManager = nullptr;
		}
	}
	void RenderSystem::Initialize(const std::string& initConfig)
	{
		// 存储设置
		std::ifstream i(initConfig);
		i >> mConfig;
		i.close();
	}
	json& RenderSystem::GetRenderWindowDescRef()
	{
		return mConfig;
	}
	void RenderSystem::SetRenderTarget(RenderTarget* renderTarget)
	{
		AddRenderTarget(renderTarget);
		mActiveRenderTarget = renderTarget;
	}
	void RenderSystem::AddRenderTarget(RenderTarget* renderTarget)
	{
		mRenderTargets.emplace(renderTarget->GetName(), renderTarget);
	}
	void RenderSystem::RemoveRenderTarget(const std::string& name)
	{
		const auto it = mRenderTargets.find(name);
		if (it != mRenderTargets.end())
		{
			mRenderTargets.erase(it);
			if (it->second == mActiveRenderTarget)
			{
				mActiveRenderTarget = nullptr;
			}
		}
	}
	RenderTarget* RenderSystem::GetRenderTargetByName(const std::string& name) const
	{
		const auto it = mRenderTargets.find(name);
		if (it != mRenderTargets.end())
		{
			return it->second;
		}
		return nullptr;
	}
	void RenderSystem::UpdateRenderTarget()
	{
		for (auto it = mRenderTargets.begin(); it != mRenderTargets.end(); it++)
		{
			it->second->Update();
		}
	}
	void RenderSystem::SwapRenderTargetBuffer()
	{
		for (auto it = mRenderTargets.begin(); it != mRenderTargets.end(); it++)
		{
			it->second->SwapBuffers();
		}
	}
	void RenderSystem::Render(const RenderParameter& rp)
	{
	}
}