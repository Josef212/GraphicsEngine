#include "EventManager.h"

#include "IEventListener.h"


// TODO: Keep adding event types to the vector
std::vector<EventType> GetEventTypes()
{
	return { EVENT_RESOURCE_REMOVED, EVENT_WINDOW_RESIZE };
}



EventManager::EventManager()
{
	for (auto it : GetEventTypes())
	{
		m_listeners[it] = std::vector<IEventListener*>();
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
			m_listeners[it].push_back(listener);
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
			auto list = m_listeners[it];
			auto it = std::find(list.begin(), list.end(), listener);

			if (it != list.end()) 
				list.erase(it);
		}
	}
}

void EventManager::FireEvent(Event e)
{
	auto list = m_listeners[e.type];
	for (auto it : list)
	{
		it->OnEventRecieved(e);
	}
}