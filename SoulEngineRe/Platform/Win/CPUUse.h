#pragma once
#include <Pdh.h>
namespace Soul
{
	class CPUUse
	{
	public:
		CPUUse();
		void Initialize();
		void ShutDown();
		void Frame();
		int GetCpuPerentage();
	private:
		bool mCanReadCPU;
		HQUERY mQueryHandle;
		HCOUNTER mCounterHandle;
		unsigned long long mLastSampleTime;	//上次的采样时间
		unsigned long long mCpuUsage;		//CPU使用率
	};
}