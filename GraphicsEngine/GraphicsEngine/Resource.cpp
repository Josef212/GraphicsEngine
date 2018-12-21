#include "Resource.h"

#include "App.h"
#include "M_ResourceManager.h"

Resource::Resource(const char* _name, ResourceType type) : m_type(type)
{
	m_name = _name;
	app->resourceManager->AddResource(this);
}

Resource::Resource(std::string _name, ResourceType type) : m_type(type)
{
	m_name = _name;
	app->resourceManager->AddResource(this);
}
