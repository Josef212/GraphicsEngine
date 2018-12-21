#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>

// TODO: Keep adding events & add them on EventManager.cpp GetTypes function
enum EventType
{
	EVENT_NONE = 0,
	EVENT_RESOURCE_REMOVED = 1 << 0
};

// TODO: Keep adding data supported in event
struct Event
{
	union
	{
		int _int;
		float _float;
		void* _ptr; // This is extremly dangerous!
		bool _bool;
	}data;

	EventType type = EventType::EVENT_NONE;
};

#endif // !__EVENT_H__