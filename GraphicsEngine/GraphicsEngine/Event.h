#ifndef __EVENT_H__
#define __EVENT_H__

#include <vector>

// TODO: Keep adding events & add them on EventManager.cpp GetTypes function
enum EventType
{
	EVENT_NONE = 0,
	EVENT_RESOURCE_REMOVED = 1 << 0,
	EVENT_WINDOW_RESIZE = 1 << 1
};

struct Vec2 { int x; int y; };
struct Vec3 { int x; int y; int z; };
struct Vec4 { int x; int y; int z; int w; };

// TODO: Keep adding data supported in event
struct Event
{
	union
	{
		int _int;
		float _float;
		void* _ptr; // This is extremly dangerous!
		bool _bool;
		Vec2 _v2;
		Vec3 _v3;
		Vec4 _v4;
	}data;

	EventType type = EventType::EVENT_NONE;
};

#endif // !__EVENT_H__