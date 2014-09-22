#

class Clock
{
private:
	unsigned long long m_timeCycles;	// 64 bit unsigned int
	float m_timeScale;					// 32 bit floating point
	bool m_isPaused;
	
	static float s_cyclesPerSecond;

	static inline unsigned long long secondsToCycles(float timeSeconds)
	{
		return (unsigned long long)(timeSeconds * s_cyclesPerSecond);
	}
	
	//	WARNING: Dangerous -- only use to convert small
	// durations into seconds
	static inline float cyclesToSeconds(float timeSeconds)
	{
		return (float)(timeSeconds / s_cyclesPerSecond);
	}
	
public:
	static void init()
	{
	}

	explicit Clock(float startTimeSeconds = 0.0f) :
		m_timeCycles(startTimeSeconds),	
		m_timeScale(1.0f),	// default to unscaled
		m_isPaused(false)	// default to running
	{
	}

	unsigned long long getTimeCycles() const
	{
		return m_timeCycles;
	}

	void update(float dtRealSeconds)
	{
		if(!m_isPaused)
		{
			unsigned long long dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);
			m_timeCycles += dtScaledCycles;
		}
	}
	
	void setPaused(bool isPaused)
	{
		m_isPaused = isPaused;
	}

	bool isPaused() const
	{
		return m_isPaused;
	}
		
	void setTimeScale(float scale)
	{
		m_timeScale = scale;
	}

	float getTimeScale() const
	{
		return m_timeScale;
	}

	void singleStep()
	{
		if(m_isPaused)
		{
			// Add one ideal frame interval to scale it by our current time scale!
			unsigned long long dtScaledCycles = secondsToCycles((1.0f/30.0) * m_timeScale);
			m_timeCycles += dtScaledCycles;
		}
	}





};