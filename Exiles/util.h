#pragma once

#include <vector>
#include <sstream>

class ReferenceCounter
{
public:
	ReferenceCounter()
	{
		m_refCount = 1;
	}

	inline int GetReferenceCount() { return m_refCount; }

	inline void AddReference() { m_refCount++; }
	inline bool RemoveReference() { m_refCount--; return m_refCount == 0; }
protected:
private:
	int m_refCount;
};

static float random(float low, float high)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}