#pragma once
#include "SubMesh.h"
#include "../Core/Mathematics/SDimension2.h"

namespace Soul
{
	class Rectangle2D : public SubMesh
	{
	public:
		Rectangle2D(std::string name, const Core::SDimension2& size);
		void SetShader(Shader* shader);
	private:
		Core::SDimension2 mSize;
	};
}
