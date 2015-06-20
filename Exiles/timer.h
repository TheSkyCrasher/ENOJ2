#pragma once

class Timer
{
public:
	static void Update();
	static float deltaTime();

private:
	static __int64 m_prevFrame;
	static __int64 m_currentFrame;
	static __int64 m_freq;
	static float m_deltaTime;
	static float m_frames;
};