#pragma once
#include "SubMesh.h"

namespace Soul
{
	class ParticalList : public SubMesh
	{
	public:
		ParticalList(const std::string& name);
		void InitializeBuffer() override;
		void UpdateBuffer() override;
	};
}