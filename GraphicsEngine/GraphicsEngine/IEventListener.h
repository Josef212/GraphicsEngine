#ifndef __I_EVENTLISTENER_H__
#define __I_EVENTLISTENER_H__

#include "Event.h"

class IEventListener
{
public:
	IEventListener() {}
	~IEventListener() = default;

	virtual EventType GetSupportedEvents() = 0;
	virtual void OnEventRecieved(Event e) = 0;
};

#endif // !__I_EVENTLISTENER_H__
