#include "ModelLoader.h"

#include "R_Geometry.h"
#include "R_Scene.h"
#include "R_Material.h"
#include "R_Model.h"

#include <iostream>
#include "Defs.h"

#include <assimp/postprocess.h>


ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

R_Model* ModelLoader::LoadModel(std::string path, R_Scene* scene)
{
	Assimp::Importer importer;
	const aiScene* aiScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// TODO: Read from memory with FS

	if(!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
	{
		LOG(LOG_ERROR, "ERROR::ASSIMP:: %s", importer.GetErrorString());
		return nullptr;
	}

	LOG(LOG_INFO, "Loading model: %s", path.c_str());

	int lastSeparator = path.find_last_of('/');
	int lastPoint = path.find_last_of('.');
	std::string name = path.substr(lastSeparator, lastPoint - lastSeparator);

	R_Model* model = new R_Model(name.c_str());
	model->m_directory = path.substr(0, lastSeparator);

	ProcessNode(aiScene->mRootNode, aiScene, model, scene);

	scene->AddModel(model);

	return model;
}

void ModelLoader::ProcessNode(aiNode * node, const aiScene * aiScene, R_Model * model, R_Scene * scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = aiScene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, model, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], aiScene, model, scene);
	}
}

void ModelLoader::ProcessMesh(aiMesh * mesh, R_Model * model, R_Scene * scene)
{
	static int meshIndex = 0;
	R_Geometry* geometry = new R_Geometry(mesh->mName.length > 0 ? mesh->mName.C_Str() : ("mesh" + std::to_string(meshIndex)).c_str());

	geometry->m_numVertices = mesh->mNumVertices;
	geometry->m_vertices = new float[geometry->m_numVertices * 3];
	memcpy(geometry->m_vertices, mesh->mVertices, sizeof(float)*geometry->m_numVertices * 3);

	if(mesh->HasFaces())
	{
		geometry->m_numIndices = mesh->mNumFaces * 3;
		geometry->m_indices = new int[geometry->m_numIndices];
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
				std::cout << "WARNING: geometry face with != 3 indices" << std::endl;
			memcpy(&geometry->m_indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(int));
		}
	}

	if (mesh->HasNormals())
	{
		geometry->m_normals = new float[geometry->m_numVertices * 3];
		memcpy(geometry->m_normals, mesh->mNormals, sizeof(float) * geometry->m_numVertices * 3);
	}

	if (mesh->HasTextureCoords(0))
	{
		geometry->m_texCoords = new float[geometry->m_numVertices * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (unsigned int i = 0; i < geometry->m_numVertices * 2; i += 2)
		{
			geometry->m_texCoords[i] = tmp->x;
			geometry->m_texCoords[i + 1] = tmp->y;
			++tmp;
		}
	}

	if (mesh->HasVertexColors(0))
	{
		geometry->m_colors = new float[geometry->m_numVertices * 3];
		memcpy(geometry->m_colors, mesh->mColors, sizeof(float) * geometry->m_numVertices * 3);
	}

	// TODO: Tangents

	geometry->SendInfoToVRAM();

	// TODO: Should process material here

	model->AddMesh(geometry, scene->GetDefaultMaterial());
}
