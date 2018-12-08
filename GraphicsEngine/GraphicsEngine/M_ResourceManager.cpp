#include "M_ResourceManager.h"

#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Texture.h"
#include "GeometryDefs.h"


M_ResourceManager::M_ResourceManager() : Module("M_ResourceManager", true)
{
	LOG_CREATION(moduleName.c_str());

	configuration = M_START;
}


M_ResourceManager::~M_ResourceManager()
{
	for(auto it : resources)
	{
		it->Free();
		RELEASE(it);
	}
}

bool M_ResourceManager::Start()
{
	LOG_START(moduleName.c_str());

	SetDefaultResources();

	return true;
}

void M_ResourceManager::AddResource(Resource * res)
{
	resources.push_back(res);
}

void M_ResourceManager::RemoveResource(Resource * res, bool forceDefaults)
{
	auto it = std::find(resources.begin(), resources.end(), res);
	if (it != resources.end())
	{
		if (!IsDefaultResource(res) || forceDefaults)
		{
			res->Free();
			RELEASE(res);
			resources.erase(it);
		}
	}
}

void M_ResourceManager::RemoveResources(Resource ** res, int count, bool forceDefaults)
{
	if (res)
	{
		for (int i = 0; i < count; ++i)
		{
			RemoveResource(res[i], forceDefaults);
		}
	}
}

void M_ResourceManager::RemoveResources(std::vector<Resource*>& res, bool forceDefaults)
{
	for (auto it = res.begin(); it != res.end();)
	{
		RemoveResource((*it), forceDefaults);
		it = res.erase(it);
	}
}

void M_ResourceManager::RemoveAllResources(bool forceDefaults)
{
	for (auto it = resources.begin(); it != resources.end();)
	{
		bool def = IsDefaultResource((*it));
		if ((def && forceDefaults) || !def)
		{
			(*it)->Free();
			RELEASE((*it));
			it = resources.erase(it);
		}
		else
		{
			++it;
		}
	}
}

unsigned int M_ResourceManager::GatherResourcesOfType(ResourceType type, std::vector<Resource*>& vec)
{
	vec.clear();

	for (auto it : resources)
	{
		if (it->GetType() == type)
			vec.push_back(it);
	}

	return vec.size();
}

bool M_ResourceManager::IsDefaultResource(Resource * res)
{
	// TODO: Keep adding resources 
	return (res == defaultResources.triangleGeo || 
		res == defaultResources.quadGeo || 
		res == defaultResources.cubeGeo || 
		res == defaultResources.simpleSh || 
		res == defaultResources.simpleMat);
}

void M_ResourceManager::SetDefaultResources()
{// Geometries
	defaultResources.triangleGeo = new R_Geometry("Def triangle geo", triangleVerticesCount, triangleIndicesCount, triangleIndices, triangleVertices, triangleNormals, triangleTexCoords, triangleColors);
	defaultResources.quadGeo = new R_Geometry("Def quad geo", quadVerticesCount, quadIndicesCount, quadIndices, quadVertices, quadNormals, quadTexCoords, quadColors);
	defaultResources.cubeGeo = new R_Geometry("Def cube geo", cubeVerticesCount, cubeIndicesCount, cubeIndices, cubeVertices, cubeNormals, cubeTexCoords, cubeColors);
	
	// Shaders
	defaultResources.simpleSh = new R_Shader("Def simple shader", "./Data/Shaders/simple.vert", "./Data/Shaders/simple.frag");

	// Materials
	//defaultResources.simpleMat = new R_Material("Def simple material", defaultResources.simpleSh);

	// Models
}
