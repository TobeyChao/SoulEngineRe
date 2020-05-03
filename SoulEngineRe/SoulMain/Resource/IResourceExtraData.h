#pragma once
#include <string>
namespace Soul
{
	class IResourceExtraData
	{
	public:
		//return some information
		virtual std::wstring ToString() = 0;
	};
}