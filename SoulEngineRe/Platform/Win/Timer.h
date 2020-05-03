#pragma once
namespace Soul
{
	class Timer
	{
	public:
		static void InitTimer();
		static float TotalTime();
		static float DeltaTime();
		static void Start();
		static void Reset();
		static void Stop();
		static void Tick();

	private:
		static double m_secondsPerCount;
		static double m_deltatime;
		static __int64 m_baseTime;
		static __int64 m_pausedTime;
		static __int64 m_stopTime;
		static __int64 m_prevTime;
		static __int64 m_currTime;
		static bool m_isStopped;
	};
}