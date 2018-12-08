#ifndef __M_RESOURCEMANAGER_H__
#define __M_RESOURCEMANAGER_H__

#include "Module.h"
#include <vector>
#include "Resource.h"

class Resource;
class R_Geometry;
class R_Shader;
class R_Material;
class R_Texture;


class M_ResourceManager : public Module
{
public:
	M_ResourceManager();
	virtual ~M_ResourceManager();

	bool Start() override;

	void AddResource(Resource* res);

	void RemoveResource(Resource* res, bool forceDefaults = false);
	void RemoveResources(Resource** res, int count, bool forceDefaults = false);
	void RemoveResources(std::vector<Resource*>& res, bool forceDefaults = false);
	void RemoveAllResources(bool forceDefaults = false);

	unsigned int GatherResourcesOfType(ResourceType type, std::vector<Resource*>& vec);

	bool IsDefaultResource(Resource* res);

private:
	void SetDefaultResources();

public:
	struct DefaultResources
	{
		R_Geometry* triangleGeo = nullptr;
		R_Geometry* quadGeo = nullptr;
		R_Geometry* cubeGeo = nullptr;

		R_Shader* simpleSh = nullptr;

		R_Material* simpleMat = nullptr;

	}defaultResources;

private:
	std::vector<Resource*> resources;
};

#endif // !__M_RESOURCEMANAGER_H__