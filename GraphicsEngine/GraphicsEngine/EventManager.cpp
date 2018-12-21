#include "EventManager.h"

#include "IEventListener.h"


// TODO: Keep adding event types to the vector
std::vector<EventType> GetEventTypes()
{
	return { EVENT_RESOURCE_REMOVED };
}



EventManager::EventManager()
{
	for (auto it : GetEventTypes())
	{
		listeners[it] = std::vector<IEventListener*>();
	}
}

EventManager::~EventManager()
{
}

void EventManager::AddEventListener(IEventListener * listener)
{
	if (!listener) return;

	EventType types = listener->GetSupportedEvents();

	for (auto it : GetEventTypes())
	{
		if ((it & types) == it)
		{
			listeners[it].push_back(listener);
		}
	}
}

void EventManager::RemoveEventListener(IEventListener * listener)
{
	if (!listener) return;

	EventType types = listener->GetSupportedEvents();

	for (auto it : GetEventTypes())
	{
		if ((it & types) == it)
		{
			auto list = listeners[it];
			auto it = std::find(list.begin(), list.end(), listener);

			if (it != list.end()) 
				list.erase(it);
		}
	}
}

void EventManager::FireEvent(Event e)
{
	auto list = listeners[e.type];
	for (auto it : list)
	{
		it->OnEventRecieved(e);
	}
}