#include "TerrainCreator.h"
#include "GeometryGenerator.h"
#include "SubMesh.h"
#include <fstream>

namespace Soul
{
	struct HeightMapInfo
	{
		HeightMapInfo()
			:
			terrainWidth(0),
			terrainHeight(0),
			color(nullptr)
		{

		}
		unsigned terrainWidth;
		unsigned terrainHeight;
		std::vector<float> height;
		Core::SVector3* color;
	};
	void Soul::TerrainCreator::CreateTerrainWithHeightMap(const json& createParameters, SubMesh* subMesh)
	{
		GeometryGenerator geoGen;
		float width = 10.f;
		float depth = 10.f;
		unsigned m = 5;
		unsigned n = 5;
		float scale = 0.003f;
		std::string heightMap;
		std::string colorMap;

		if (createParameters.contains("width"))
		{
			width = createParameters["width"];
		}
		if (createParameters.contains("depth"))
		{
			depth = createParameters["depth"];
		}
		if (createParameters.contains("m"))
		{
			m = createParameters["m"];
		}
		if (createParameters.contains("n"))
		{
			n = createParameters["n"];
		}
		if (createParameters.contains("scale"))
		{
			scale = createParameters["scale"];
		}

		std::function<float(int, int)> heightFunc = [](int, int)
		{
			return 0.0f;
		};
		std::function<Core::SVector4(int, int)> colorFunc = [](int, int)
		{
			return Core::SVector4(1.0f, 1.0f, 1.0f, 1.0f);
		};

		HeightMapInfo heightMapInfo;

		if (createParameters.contains("heightMap"))
		{
			heightMap = createParameters["heightMap"];

			// 读取高度图
			std::ifstream heightMapFile(heightMap, std::ios::binary);
			if (!heightMapFile)
			{
				return;
			}
			// 读取图片字节数
			heightMapFile.seekg(0, heightMapFile.end);
			unsigned length = (unsigned)heightMapFile.tellg();
			heightMapFile.seekg(0, heightMapFile.beg);
			// 图片字节数与需要的是否匹配
			if (length != m * n * sizeof(unsigned short))
			{
				return;
			}
			// 16位数组
			unsigned short* rawImage = new unsigned short[m * n];
			if (!rawImage)
			{
				return;
			}
			// 读取图片
			heightMapFile.read(reinterpret_cast<char*>(rawImage), length);
			if (heightMapFile)
				std::cout << "all characters read successfully." << std::endl;
			else
				std::cout << "error: only " << heightMapFile.gcount() << " could be read" << std::endl;
			heightMapFile.close();
			heightMapInfo.height.resize(m * n, 0.f);
			for (size_t i = 0; i < m * n; i++)
			{
				heightMapInfo.height[i] = (float)rawImage[i] * scale;
			}
			heightFunc = [&](int x, int z)
			{
				return heightMapInfo.height[x * n + z];
			};
		}
		if (createParameters.contains("colorMap"))
		{
			colorMap = createParameters["colorMap"];

			// 读取颜色图

			colorFunc = [&](int x, int z)
			{
				return Core::SVector4();
			};
		}

		geoGen.CreateTerrain(width, depth, m, n, *subMesh->GetOriginalMeshDataPtr(), heightFunc, colorFunc);
	}
}