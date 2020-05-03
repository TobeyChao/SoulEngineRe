#include "ITexture.h"

namespace Soul
{
	ITexture::ITexture(const std::wstring& filePath)
		:
		Resource(filePath),
		mTextureSize(0, 0)
	{}
	const Core::SDimension2& ITexture::GetOriginalSize() const
	{
		return mTextureSize;
	}
}