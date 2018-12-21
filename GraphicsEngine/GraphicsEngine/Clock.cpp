#include "Clock.h"

namespace GE
{

	Clock::Clock()
	{
		m_msTimer		= std::make_unique<PerfTimer>();
		m_msGameTimer = std::make_unique<PerfTimer>();
		m_fpsTimer	= std::make_unique<PerfTimer>();
	}

	Clock::~Clock()
	{
		m_msTimer.release();
		m_msGameTimer.release();
		m_fpsTimer.release();
	}

	/**
	*	- OnPrepareUpdate: Calculate all attributes at the begging of each frame
	*		- Add the elapsed time.
	*		- Recal real dt.
	*		- Add one frame to counter.
	*		- If app state is PLAY do the same with the game timer.
	*/
	void Clock::OnPrepareUpdate(/*APP_STATE appState*/)
	{
		//1. Add time
		m_timeSinceAppStart += m_realDt;
		m_timeSinceLevelLoaded += m_realDt;
		//2. Calc dt
		m_realDt = (float)(m_msTimer->ReadMs() / 1000.0f);
		if (m_realDt > m_maximumDT) m_realDt = 1 / 30.0f;
		m_msTimer->Start();
		//3. Add a frame
		++m_realFrameCount;

		//if (appState == APP_STATE::PLAY)
		//{
		//	gameTimeSinceLevelLoaded += gameDt;
		//
		//	gameDt = ((float)(msGameTimer->ReadMs() / 1000.0f)) * scale;
		//	msGameTimer->Start();
		//
		//	++gameFrameCount;
		//}
	}

	/**
	*	- OnFinishUpdate: Calculate all attributes at the ending of the frame.
	*		- Add one to fps counter.
	*		- Check if one second has passed to calculate fps.
	*		- Calculate last frame ms.
	*/
	void Clock::OnFinishUpdate()
	{
		++m_fpsCounter;

		if (m_fpsTimer->ReadMs() > 1000.0f)
		{
			m_lastFps = m_fpsCounter;
			m_fpsCounter = 0;
			m_fpsTimer->Start();
		}

		m_lastFrameMs = m_msTimer->ReadMs();
	}

	void Clock::OnSceneLoaded()
	{
		m_timeSinceLevelLoaded = 0.0;
	}

	void Clock::Pause()
	{
		//TODO: Check this, make sense to stop the time even if u pause?? (Scene since level started should continue even if u pause)
		//msGameTimer->Stop();
	}

	void Clock::UnPause()
	{
		m_msGameTimer->Start();
	}

	void Clock::Stop()
	{
		m_msGameTimer->Stop();
		m_gameFrameCount = 0;
		m_gameDt = 0.0f;
	}

	void Clock::Play()
	{
		m_msGameTimer->Start();
	}

	/**
	*	- DT: Return real dt.
	*/
	float Clock::Dt() const
	{
		return m_realDt;
	}

	/**
	*	- RealFrameCount: Return the real frames passed.
	*/
	uint Clock::RealFrameCount() const
	{
		return m_realFrameCount;
	}

	/**
	*	- FPS: Return the fps counter.
	*/
	uint Clock::FPS() const
	{
		return m_fpsCounter;
	}

	/**
	*	- LastFPS: Return the last frame fps counter.
	*/
	uint Clock::LastFPS() const
	{
		return m_lastFps;
	}

	/**
	*	- TimeSinceStart: Return the time in seconds since app started.
	*/
	float Clock::TimeSinceStart() const
	{
		return m_timeSinceAppStart;
	}

	/**
	*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded.
	*/
	float Clock::TimeSinceSceneLoaded() const
	{
		return m_timeSinceLevelLoaded;
	}

	/**
	*	- GameDT: Return the game dt.
	*/
	float Clock::GameDt() const
	{
		return m_gameDt;
	}

	/**
	*	- GameFrameCounter: Return the game frames passed.
	*/
	uint Clock::GameFrameCounter() const
	{
		return m_gameFrameCount;
	}

	/**
	*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded in game scale.
	*/
	float Clock::GameTimeSinceLevelLoaded() const
	{
		return m_gameTimeSinceLevelLoaded;
	}

	/**
	*	- GetScale: Return the game time scale.
	*/
	float Clock::GetScale() const
	{
		return m_scale;
	}

	/**
	*	- LastFrameMs: Return the last frame ms.
	*/
	float Clock::LastFrameMs() const
	{
		return m_lastFrameMs;
	}

	/**
	*	- SetScale: Set the game time scale.
	*/
	void Clock::SetScale(float scl)
	{
		if (scl >= 0.0f)
			m_scale = scl;
		else
			m_scale = 0.0f;
	}

}