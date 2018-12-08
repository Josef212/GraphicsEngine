#ifndef __MODELLOADER_H__
#define __MODELLOADER_H__

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class R_Scene;
class R_Model;

class ModelLoader
{
public:
	ModelLoader();
	virtual ~ModelLoader();

	static R_Model* LoadModel(std::string path, R_Scene* scene);

private:
	static void ProcessNode(aiNode* node, const aiScene* aiScene, R_Model* model, R_Scene* scene);
	static void ProcessMesh(aiMesh* mesh, R_Model* model, R_Scene* scene);
};

#endif // !__MODELLOADER_H__