#include "timer.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

__int64 Timer::m_prevFrame = 0;
__int64 Timer::m_currentFrame = 0;
__int64 Timer::m_freq = 0;
float Timer::m_deltaTime = 0.0f;
float Timer::m_frames = 0.0f;

void Timer::Update()
{
	m_deltaTime = 0.0f;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentFrame);
	int deltaTicks = (int)(m_currentFrame - m_prevFrame);
	m_deltaTime = (float)deltaTicks / (float)m_freq;
	m_prevFrame = m_currentFrame;

	m_frames += deltaTime();
	if (m_frames >= 1.0f)
	{
		std::cout << std::setprecision(3) << 1.0f / deltaTime() << " fps" << std::endl;
		m_frames = 0;
	}
}

float Timer::deltaTime()
{
	float dt = m_deltaTime;
	if (dt <= 0.0f || dt > 1.0f)
	{
		dt = 0.025f;
	}

	return dt;
}

long Timer::currentTime()
{
	std::cout << m_currentFrame << "\n";
	return m_currentFrame;
}