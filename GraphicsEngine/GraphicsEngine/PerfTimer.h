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
		bool running;
		uint64 startedAt;
		uint64 stopedAt;
		uint64 frequency = 0;
	};
}

#endif