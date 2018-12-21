#ifndef __MODULE_H__
#define __MODULE_H__

#include "Defs.h"
#include <string>

typedef enum
{
	M_NONE = 0,
	M_INIT = 1 << 0,
	M_START = 1 << 1,
	M_PRE_UPDATE = 1 << 2,
	M_UPDATE = 1 << 3,
	M_POST_UPDATE = 1 << 4,
	M_CLEAN_UP = 1 << 5,
	M_SAVE_CONFIG = 1 << 6,
	M_RESIZE_EVENT = 1 << 7,
	M_DRAW_DEBUG = 1 << 8

} ModuleConfig;

#define MODULE_FULL_CONFIG\
	M_INIT | M_START |\
	M_PRE_UPDATE | M_UPDATE | M_POST_UPDATE |\
	M_CLEAN_UP |\
	M_SAVE_CONFIG |\
	M_RESIZE_EVENT |\
	M_DRAW_DEBUG


class Module
{
	friend class App;
public:
	Module(const char* name, bool startEnabled, uint32 config = 0) : m_moduleName(name), m_enabled(startEnabled), 
	m_configuration(config)
	{}
	virtual ~Module()
	{}

	// ==========================================

	virtual bool Init() { return true; }
	virtual bool Start() { return true; }
	virtual UpdateReturn PreUpdate(float dt) { return UpdateReturn::UPDT_CONTINUE; }
	virtual UpdateReturn Update(float dt) { return UpdateReturn::UPDT_CONTINUE; }
	virtual UpdateReturn PostUpdate(float dt) { return UpdateReturn::UPDT_CONTINUE; }
	virtual bool CleanUp() { return true; }

	virtual bool Save() { return true; }
	virtual bool Load() { return true; }

	virtual void DrawDebug() {}

	virtual void OnResize(uint w, uint h) {}

	// ==========================================

	bool IsEnabled() const { return m_enabled; }

	bool Enable()
	{
		if (!m_enabled)
		{
			m_enabled = Start();
			return m_enabled;
		}

		return false;
	}

	bool Disable()
	{
		if (m_enabled)
		{
			m_enabled = !CleanUp();
			return m_enabled;
		}

		return false;
	}

	bool SetActive(const bool set)
	{
		if (set != m_enabled)
		{
			return set ? Enable() : Disable();
		}

		return false;
	}

public:
	std::string m_moduleName;

private:
	bool m_enabled;

protected:
	uint32 m_configuration = 0;
};

#endif