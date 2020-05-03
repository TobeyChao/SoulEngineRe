#pragma once
#include "../Core/Mathematics/SDimension2.h"
#include "../Resource/Resource.h"
namespace Soul
{
	class ITexture : public Resource
	{
	public:
		explicit ITexture(const std::wstring& filePath);
		[[nodiscard]] const Core::SDimension2& GetOriginalSize() const;
	protected:
		Core::SDimension2 mTextureSize;
	};
}