#pragma once
#include "../Utilities/utilities.h"
#include <string>
#include <unordered_map>
#include <memory>

#include "ModelResource.h"
#include "ShaderResource.h"
#include "TextureResource.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"


class ResourceManager
{
	public:
		void static Init();
		static ResourceManager* GetInstance();
		
		~ResourceManager();

		Model* loadModel(int);
		Texture* loadTexture(int);
		Shader* loadShader(int);

	private:
		
		static ResourceManager* spInstance;
		ResourceManager();
		std::unordered_map<int, ModelResource*>   modelResources;
		std::unordered_map<int, TextureResource*> textureResources;
		std::unordered_map<int,ShaderResource*>  shaderResources;

		std::unordered_map<int, Model*>   loadedModels;
		std::unordered_map<int, Texture*> loadedTextures;
		std::unordered_map<int, Shader*>  loadedShaders;
};

