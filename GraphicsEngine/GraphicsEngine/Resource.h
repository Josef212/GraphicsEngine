#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

enum ResourceType
{
	RES_NONE = 0,
	RES_GEOMETRY,
	RES_SHADER,
	RES_MATERIAL,
	RES_MODEL,
	RES_TEXTURE,
	RES_SCENE
};

class Resource
{
public:
	Resource(const char* _name, ResourceType type);
	Resource(std::string _name, ResourceType type);

	virtual ~Resource() = default;

	virtual void Load() {}
	virtual void Free() {}

	ResourceType GetType() const { return type; }
	std::string GetName() const { return name; }
	const char* GetNameCStr() const { return name.c_str(); }

private:

public:

private:
	std::string name = "Non named resource";
	ResourceType type = RES_NONE;
};

#endif // !__RESOURCE_H__