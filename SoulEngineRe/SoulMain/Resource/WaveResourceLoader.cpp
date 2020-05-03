#include "WaveResourceLoader.h"
#include <fstream>
#include <cassert>
#include <iostream>

#include "../Audio/SoundResourceExtraData.h"

namespace Soul
{
	WaveResourceLoader::WaveResourceLoader()
	{
		mPatterns.push_back(L"wav");
	}
	bool WaveResourceLoader::LoadResource(std::shared_ptr<Resource> handle)
	{
		auto extra = std::make_shared<SoundResourceExtraData>();
		extra->mSoundType = SOUND_TYPE_WAVE;
		handle->SetExtra(extra);
		if (!ParseWave(handle))
		{
			return false;
		}
		return true;
	}

	std::vector<std::wstring>& WaveResourceLoader::GetPatterns()
	{
		return mPatterns;
	}

	bool WaveResourceLoader::ParseWave(std::shared_ptr<Resource> handle)
	{
		std::shared_ptr<SoundResourceExtraData> extra =
			std::static_pointer_cast<SoundResourceExtraData>(handle->GetExtra());

		const auto IsFourCC = [](const BYTE *pData, const char *pFourcc)
		{
			assert(strlen(pFourcc) == 4);
			for (int i = 0; i < 4; i++)
			{
				if (char(pData[i]) != pFourcc[i])
				{
					return false;
				}
			}
			return true;
		};

		//读取文件
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(handle->GetName(), std::ios::binary);

		//读取Format
		BYTE fourcc[4];
		file.read(reinterpret_cast<char*>(fourcc), 4u);
		std::cout << "Chunk ID: " << fourcc[0] << fourcc[1] << fourcc[2] << fourcc[3] << std::endl;
		if (!IsFourCC(fourcc, "RIFF"))
		{
			std::cout << "RIFF ERROR!" << std::endl;
		}

		//读取文件大小
		unsigned int fileSize = 0;
		file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
		fileSize += 8u; // entry doesn't include the fourcc or itself
		std::cout << "Chunk Size: " << fileSize / 1024u / 1024u << "MB" << std::endl;
		if (fileSize <= 44u)
		{
			std::cout << "FILE TOO SMALL!" << std::endl;
		}

		//读取整个文件
		std::unique_ptr<BYTE[]> pFileIn;
		file.seekg(0, std::ios::beg);
		pFileIn = std::make_unique<BYTE[]>(fileSize);
		file.read(reinterpret_cast<char*>(pFileIn.get()), fileSize);

		//读取文件格式
		std::cout << "Format: " << pFileIn[8] << pFileIn[9] << pFileIn[10] << pFileIn[11] << std::endl;;
		if (!IsFourCC(&pFileIn[8], "WAVE"))
		{
			std::cout << "WAVE ERROR!" << std::endl;
		}

		//look for 'fmt ' chunk id
		memset(&extra->mWavFormatEx, 0, sizeof(WAVEFORMATEX));
		bool bFilledFormat = false;
		for (size_t i = 12u; i < fileSize; )
		{
			if (IsFourCC(&pFileIn[i], "fmt "))
			{
				memcpy(&extra->mWavFormatEx, &pFileIn[i + 8u], sizeof(WAVEFORMATEX));
				bFilledFormat = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size + word padding
			unsigned int chunkSize;
			memcpy(&chunkSize, &pFileIn[i + 4u], sizeof(chunkSize));
			i += (chunkSize + 9u) & 0xFFFFFFFEu;
		}
		if (!bFilledFormat)
		{
			std::cout << "FORMAT ERROR!" << std::endl;;
		}

		//look for 'data' chunk id
		UINT32 nBytes = 0u;//data区大小
		bool bFilledData = false;
		for (size_t i = 12u; i < fileSize; )
		{
			unsigned int chunkSize;
			memcpy(&chunkSize, &pFileIn[i + 4u], sizeof(chunkSize));

			if (IsFourCC(&pFileIn[i], "data"))
			{
				extra->mLengthMilli = (int)(((float)chunkSize / extra->mWavFormatEx.nAvgBytesPerSec) * 1000.0f);
				UINT num = chunkSize / extra->mWavFormatEx.nAvgBytesPerSec;
				UINT hour = num / 3600;
				UINT min = num % 3600 / 60;
				UINT sec = num % 60;
				std::cout << "Duration: " << hour << "H" << min << "M" << sec << "S" << std::endl;

				nBytes = chunkSize;
				handle->InitializeBuffer(nBytes);
				memcpy(handle->WritableBuffer(), &pFileIn[i + 8u], nBytes);

				bFilledData = true;
				break;
			}
			// chunk size + size entry size + chunk id entry size
			i += chunkSize + 8u;
		}
		extra->mIsInitialized = true;

		//int ** intArray = new int*[2];
		//intArray[0] = new int[nBytes / 4];
		//intArray[1] = new int[nBytes / 4];
		//const short * data = (const short *)(handle->Buffer());
		//for (size_t i = 0, j = 0; i < nBytes; i += 4, j++)
		//{
		//	intArray[0][j] = int((data[i] & 0x000000FF) | ((data[i + 1]) << 8));
		//	intArray[1][j] = int((data[i + 2] & 0x000000FF) | ((data[i + 3]) << 8));
		//	std::cout << intArray[0][j] << " " << intArray[1][j] << std::endl;
		//}
		//int count = 0;
		//for (size_t i = 0; i < nBytes / 2; i += extra->mWavFormatEx.nAvgBytesPerSec / 2)
		//{
		//	std::cout << data[i] << " " << data[i + 1] << std::endl;
		//	count++;
		//}
		return true;
	}
}
