#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "PerfTimer.h"
#include "Defs.h"
#include <memory>

namespace GE
{
	class Clock
	{
	public:
		Clock();
		virtual ~Clock();

		void OnPrepareUpdate(/*APP_STATE appState*/);
		void OnFinishUpdate();

		void OnSceneLoaded();

		void Pause();
		void UnPause();
		void Stop();
		void Play();

		//Some getters---------------
		float Dt()const;
		uint RealFrameCount()const;
		uint FPS()const;
		uint LastFPS()const;

		float TimeSinceStart()const;
		float TimeSinceSceneLoaded()const;

		float GameDt()const;
		uint GameFrameCounter()const;
		float GameTimeSinceLevelLoaded()const;

		float GetScale()const;

		float LastFrameMs()const;

		void SetScale(float scl);


	private:
		//Real -------------------------
		std::unique_ptr<PerfTimer> m_msTimer = nullptr;
		std::unique_ptr<PerfTimer> m_fpsTimer = nullptr;
		float m_realDt = 0.f;
		uint m_realFrameCount = 0;
		uint m_fpsCounter = 0;
		uint m_lastFps = 0;

		float m_timeSinceAppStart = 0.f; //Time in seconds since start up
		float m_timeSinceLevelLoaded = 0.f;

		//Game -------------------------
		std::unique_ptr<PerfTimer> m_msGameTimer = nullptr;
		float m_gameDt = 0.f;
		uint m_gameFrameCount = 0;
		float m_gameTimeSinceLevelLoaded = 0.f;

		//-----------------------------------

		float m_scale = 1.0f;

		float m_lastFrameMs = 0;
		float m_maximumDT = 1.0f;
	};

}

#endif