#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

enum ResourceType
{
	RES_NONE = -1,  // IMPORTANT: If change begin number take into account implications on the manager
	RES_GEOMETRY,
	RES_SHADER,
	RES_MATERIAL,
	RES_MODEL,
	RES_TEXTURE,
	RES_SCENE,
	RES_RENDERER,

	RES_MAX
};

class Resource
{
public:
	Resource(const char* _name, ResourceType type);
	Resource(std::string _name, ResourceType type);

	virtual ~Resource() = default;

	virtual void Load() {}
	virtual void Free() {}

	ResourceType GetType() const { return m_type; }
	std::string GetName() const { return m_name; }
	const char* GetNameCStr() const { return m_name.c_str(); }

private:

public:

private:
	std::string m_name = "Non named resource";
	ResourceType m_type = RES_NONE;
};

#endif // !__RESOURCE_H__