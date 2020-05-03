#pragma once
#include "SubMesh.h"
#include "../Core/Mathematics/SVector3.h"

namespace Soul
{
	class Line3D : public SubMesh
	{
	public:
		Line3D(std::string name, const Core::SVector3& start, const Core::SVector3& end);
		void SetShader(Shader* shader);
	private:
		Core::SVector3 mStart;
		Core::SVector3 mEnd;
	};
}