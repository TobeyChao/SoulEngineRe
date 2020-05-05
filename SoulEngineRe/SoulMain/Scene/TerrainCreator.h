#pragma once
#include <string>
#include "../SoulPCH.h"

namespace Soul
{
	class TerrainCreator
	{
	public:
		static void CreateTerrainWithHeightMap(const json& createParameters, SubMesh* subMesh);
	private:

	};
}