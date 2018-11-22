#include "Clock.h"

namespace GE
{

	Clock::Clock()
	{
		msTimer		= std::make_unique<PerfTimer>();
		msGameTimer = std::make_unique<PerfTimer>();
		fpsTimer	= std::make_unique<PerfTimer>();
	}

	Clock::~Clock()
	{
		msTimer.release();
		msGameTimer.release();
		fpsTimer.release();
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
		timeSinceAppStart += realDt;
		timeSinceLevelLoaded += realDt;
		//2. Calc dt
		realDt = (float)(msTimer->ReadMs() / 1000.0f);
		if (realDt > maximumDT) realDt = 1 / 30.0f;
		msTimer->Start();
		//3. Add a frame
		++realFrameCount;

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
		++fpsCounter;

		if (fpsTimer->ReadMs() > 1000.0f)
		{
			lastFps = fpsCounter;
			fpsCounter = 0;
			fpsTimer->Start();
		}

		lastFrameMs = msTimer->ReadMs();
	}

	void Clock::OnSceneLoaded()
	{
		timeSinceLevelLoaded = 0.0;
	}

	void Clock::Pause()
	{
		//TODO: Check this, make sense to stop the time even if u pause?? (Scene since level started should continue even if u pause)
		//msGameTimer->Stop();
	}

	void Clock::UnPause()
	{
		msGameTimer->Start();
	}

	void Clock::Stop()
	{
		msGameTimer->Stop();
		gameFrameCount = 0;
		gameDt = 0.0f;
	}

	void Clock::Play()
	{
		msGameTimer->Start();
	}

	/**
	*	- DT: Return real dt.
	*/
	float Clock::Dt() const
	{
		return realDt;
	}

	/**
	*	- RealFrameCount: Return the real frames passed.
	*/
	uint Clock::RealFrameCount() const
	{
		return realFrameCount;
	}

	/**
	*	- FPS: Return the fps counter.
	*/
	uint Clock::FPS() const
	{
		return fpsCounter;
	}

	/**
	*	- LastFPS: Return the last frame fps counter.
	*/
	uint Clock::LastFPS() const
	{
		return lastFps;
	}

	/**
	*	- TimeSinceStart: Return the time in seconds since app started.
	*/
	float Clock::TimeSinceStart() const
	{
		return timeSinceAppStart;
	}

	/**
	*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded.
	*/
	float Clock::TimeSinceSceneLoaded() const
	{
		return timeSinceLevelLoaded;
	}

	/**
	*	- GameDT: Return the game dt.
	*/
	float Clock::GameDt() const
	{
		return gameDt;
	}

	/**
	*	- GameFrameCounter: Return the game frames passed.
	*/
	uint Clock::GameFrameCounter() const
	{
		return gameFrameCount;
	}

	/**
	*	- TimeSinceSceneLoaded: Return the time in seconds since scene was loaded in game scale.
	*/
	float Clock::GameTimeSinceLevelLoaded() const
	{
		return gameTimeSinceLevelLoaded;
	}

	/**
	*	- GetScale: Return the game time scale.
	*/
	float Clock::GetScale() const
	{
		return scale;
	}

	/**
	*	- LastFrameMs: Return the last frame ms.
	*/
	float Clock::LastFrameMs() const
	{
		return lastFrameMs;
	}

	/**
	*	- SetScale: Set the game time scale.
	*/
	void Clock::SetScale(float scl)
	{
		if (scl >= 0.0f)
			scale = scl;
		else
			scale = 0.0f;
	}

}