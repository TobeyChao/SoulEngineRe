#pragma once
#include "SubMesh.h"
namespace Soul
{
	class Point3D : public SubMesh
	{
	public:
		Point3D(std::string name);
		void SetShader(Shader* shader);
	};
}