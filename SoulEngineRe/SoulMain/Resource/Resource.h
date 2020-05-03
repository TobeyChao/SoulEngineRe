#pragma once
#include "../SoulPCH.h"
#include "IResourceExtraData.h"
namespace Soul
{
	class Resource
	{
	public:
		Resource(const std::wstring fileName);
		virtual ~Resource();
		//create a Buffer of size bytes
		bool InitializeBuffer(unsigned int size);
		//fileName
		const std::wstring GetName() { return mFileName; }
		//buffer
		unsigned int Size() const { return mSize; }
		unsigned char * Buffer() const { return mBuffer; }
		unsigned char *WritableBuffer() { return mBuffer; }
		void ReleaseBuffer();
		//extra setter getter
		std::shared_ptr<IResourceExtraData> GetExtra() { return mExtra; }
		void SetExtra(std::shared_ptr<IResourceExtraData> extra) { mExtra = extra; }
	protected:
		std::wstring mFileName;						//filename
		unsigned char *mBuffer;						//bytes
		unsigned int mSize;							//file size
		std::shared_ptr<IResourceExtraData> mExtra;	//extra data
	};
}