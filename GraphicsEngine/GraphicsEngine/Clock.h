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
		std::unique_ptr<PerfTimer> msTimer = nullptr;
		std::unique_ptr<PerfTimer> fpsTimer = nullptr;
		float realDt = 0.f;
		uint realFrameCount = 0;
		uint fpsCounter = 0;
		uint lastFps = 0;

		float timeSinceAppStart = 0.f; //Time in seconds since start up
		float timeSinceLevelLoaded = 0.f;

		//Game -------------------------
		std::unique_ptr<PerfTimer> msGameTimer = nullptr;
		float gameDt = 0.f;
		uint gameFrameCount = 0;
		float gameTimeSinceLevelLoaded = 0.f;

		//-----------------------------------

		float scale = 1.0f;

		float lastFrameMs = 0;
		float maximumDT = 1.0f;
	};

}

#endif