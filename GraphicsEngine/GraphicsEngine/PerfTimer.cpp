#include "PerfTimer.h"
#include <SDL_timer.h>

GE::PerfTimer::PerfTimer(bool use) : running(false)
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	if (use) Start();
}

GE::PerfTimer::~PerfTimer()
{
}

void GE::PerfTimer::Start()
{
	running = true;
	startedAt = SDL_GetPerformanceCounter();
}

void GE::PerfTimer::Stop()
{
	running = false;
	stopedAt = SDL_GetPerformanceCounter();
}

double GE::PerfTimer::ReadMs() const
{
	if (running)
		return 1000.0 * (double(SDL_GetPerformanceCounter() - startedAt) / double(frequency));
	else
		return 1000.0 * (stopedAt - startedAt);
}

uint64 GE::PerfTimer::ReadSec() const
{
	if (running)
		return uint64(double(SDL_GetPerformanceCounter() - startedAt) / double(frequency));
	else
		return (stopedAt - startedAt);
}

uint64 GE::PerfTimer::ReadTicks() const
{
	if (running)
		return SDL_GetPerformanceCounter() - startedAt;
	else
		return stopedAt - startedAt;
}