#ifndef __PERF_TIMER_H__
#define __PERF_TIMER_H__

#include "Defs.h"

namespace GE
{
	class PerfTimer
	{
	public:
		PerfTimer(bool use = true);
		virtual ~PerfTimer();

		void Start();
		void Stop();

		double ReadMs()const;
		uint64 ReadSec()const;
		unsigned long long ReadTicks()const;

	private:
		bool m_running;
		uint64 m_startedAt;
		uint64 m_stopedAt;
		uint64 m_frequency = 0;
	};
}

#endif