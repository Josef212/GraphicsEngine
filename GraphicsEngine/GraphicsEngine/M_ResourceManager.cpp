#include "M_ResourceManager.h"

#include "GeometryDefs.h"

#include "App.h"
#include "EventManager.h"

#include "R_Geometry.h"
#include "R_Shader.h"
#include "R_Material.h"
#include "R_Texture.h"


M_ResourceManager::M_ResourceManager() : Module("M_ResourceManager", true)
{
	LOG_CREATION(m_moduleName.c_str());

	for (int i = 0; i < ResourceType::RES_MAX; ++i)
	{
		m_resources[(ResourceType)i] = std::vector<Resource*>();
	}

	m_configuration = M_START;
}


M_ResourceManager::~M_ResourceManager()
{
	LOG_DESTRUCTION(m_moduleName.c_str());

	for(auto it : m_resources)
	{
		for (auto res : it.second)
		{
			res->Free();
			RELEASE(res);
		}
	}
}

bool M_ResourceManager::Start()
{
	LOG_START(m_moduleName.c_str());

	SetDefaultResources();

	return true;
}

void M_ResourceManager::AddResource(Resource * res)
{
	if (!res) return;

	auto it = m_resources.find(res->GetType());
	if (it != m_resources.end())
	{
		(*it).second.push_back(res);
	}
}

void M_ResourceManager::RemoveResource(Resource * res, bool forceDefaults)
{
	if (!res) return;

	auto list = m_resources.find(res->GetType());
	if (list != m_resources.end())
	{
		auto it = std::find((*list).second.begin(), (*list).second.end(), res);
		if (it != (*list).second.end())
		{
			if (!IsDefaultResource(res) || forceDefaults)
			{
				Event e;
				e.type = EventType::EVENT_RESOURCE_REMOVED;
				e.data._int = res->GetType();

				res->Free();
				RELEASE(res);
				(*list).second.erase(it);

				app->eventManager->FireEvent(e);
			}
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
	for (auto list : m_resources)
	{
		for (auto it : list.second)
		{
			bool def = IsDefaultResource(it);
			if ((def && forceDefaults) || !def)
			{
				RemoveResource(it, forceDefaults);
			}
		}
	}
}

unsigned int M_ResourceManager::GatherResourcesOfType(ResourceType type, std::vector<Resource*>& vec)
{
	vec.clear();

	auto list = m_resources.find(type);
	if (list != m_resources.end())
	{
		for (auto it : (*list).second)
		{
			vec.push_back(it);
		}
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
		res == defaultResources.simpleMat ||
		res == defaultResources.lennaTex
		);
}

bool M_ResourceManager::ValidateResource(Resource* res)
{
	if (!res) return false;

	auto list = m_resources.find(res->GetType());
	if (list != m_resources.end())
	{
		auto it = std::find((*list).second.begin(), (*list).second.end(), res);
		if (it != (*list).second.end()) return true;
	}

	return false;
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

	// Textures
	defaultResources.lennaTex = new R_Texture("Lenna tex");
	defaultResources.lennaTex->LoadTexture("Data/Textures/Lenna.png");
}
