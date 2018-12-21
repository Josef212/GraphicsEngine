#include "PerfTimer.h"
#include <SDL_timer.h>

GE::PerfTimer::PerfTimer(bool use) : m_running(false)
{
	if (m_frequency == 0)
		m_frequency = SDL_GetPerformanceFrequency();

	if (use) Start();
}

GE::PerfTimer::~PerfTimer()
{
}

void GE::PerfTimer::Start()
{
	m_running = true;
	m_startedAt = SDL_GetPerformanceCounter();
}

void GE::PerfTimer::Stop()
{
	m_running = false;
	m_stopedAt = SDL_GetPerformanceCounter();
}

double GE::PerfTimer::ReadMs() const
{
	if (m_running)
		return 1000.0 * (double(SDL_GetPerformanceCounter() - m_startedAt) / double(m_frequency));
	else
		return 1000.0 * (m_stopedAt - m_startedAt);
}

uint64 GE::PerfTimer::ReadSec() const
{
	if (m_running)
		return uint64(double(SDL_GetPerformanceCounter() - m_startedAt) / double(m_frequency));
	else
		return (m_stopedAt - m_startedAt);
}

uint64 GE::PerfTimer::ReadTicks() const
{
	if (m_running)
		return SDL_GetPerformanceCounter() - m_startedAt;
	else
		return m_stopedAt - m_startedAt;
}