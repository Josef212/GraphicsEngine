#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <map>
#include <vector>

#include "Event.h"

class IEventListener;

class EventManager
{
public:
	EventManager();
	virtual ~EventManager();

	void AddEventListener(IEventListener* listener);
	void RemoveEventListener(IEventListener* listener);

	void FireEvent(Event e);

private:
	std::map<EventType, std::vector<IEventListener*>> m_listeners;
};

#endif // !__EVENTMANAGER_H__