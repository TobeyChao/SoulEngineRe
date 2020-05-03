#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Timer.h"
double Timer::m_secondsPerCount = 0.0;
double Timer::m_deltatime = -1.0;
__int64 Timer::m_baseTime = 0;
__int64 Timer::m_pausedTime = 0;
__int64 Timer::m_stopTime = 0;
__int64 Timer::m_prevTime = 0;
__int64 Timer::m_currTime = 0;
bool Timer::m_isStopped = true;

void Timer::InitTimer()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);//counts in one second
	m_secondsPerCount = 1.0 / countsPerSec;
}

float Timer::TotalTime()
{
	if (m_isStopped)
		return static_cast<float>((m_stopTime - m_baseTime - m_pausedTime) * m_secondsPerCount);
	else
		return static_cast<float>((m_currTime - m_baseTime - m_pausedTime) * m_secondsPerCount);
}

float Timer::DeltaTime()
{
	return static_cast<float>(m_deltatime);
}

void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_isStopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		m_prevTime = startTime;

		m_stopTime = 0;
		m_isStopped = false;
	}
}

void Timer::Reset()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)(&currentTime));

	m_baseTime = currentTime;
	m_prevTime = currentTime;
	m_stopTime = 0;
	m_isStopped = false;
}

void Timer::Stop()
{
	if (!m_isStopped)
	{
		__int64 currentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

		m_stopTime = currentTime;
		m_isStopped = true;
	}
}

void Timer::Tick()
{
	if (m_isStopped)
	{
		m_deltatime = 0.0f;
		return;
	}

	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	m_currTime = currentTime;

	m_deltatime = (m_currTime - m_prevTime) * m_secondsPerCount;

	m_prevTime = m_currTime;

	if (m_deltatime < 0.0)
		m_deltatime = 0.0;
}