#include "Resource.h"

namespace Soul
{
	Resource::Resource(const std::wstring fileName)
		:
		mFileName(fileName),
		mBuffer(nullptr),
		mSize(0),
		mExtra(nullptr)
	{
	}

	Resource::~Resource()
	{
		if (mBuffer)
			delete[] mBuffer;
		mBuffer = nullptr;
	}

	bool Resource::InitializeBuffer(unsigned int size)
	{
		mSize = size;
		mBuffer = new unsigned char[size];
		if (!mBuffer)
		{
			return false;
		}
		return true;
	}
	void Resource::ReleaseBuffer()
	{
		if (mBuffer)
			delete[] mBuffer;
		mBuffer = nullptr;
	}
}