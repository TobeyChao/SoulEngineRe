#pragma once
namespace Soul
{
	class Module
	{
	public:
		virtual void Initialize() = 0;
		virtual void Uninitialize() = 0;
	};
}