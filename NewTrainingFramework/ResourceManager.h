#pragma once
#include "../Utilities/utilities.h"
#include <string>
#include <unordered_map>
#include <memory>

struct ModelResource
{
	std::string id;       
	std::string file;    
	bool wire = false;
};

struct ShaderResource
{
	std::string id;       
	std::string vs;    //Vertex shader file
	std::string fs;  //Fragement shader file
};

struct TextureResource
{
	std::string id;       
	std::string file;    
	std::string min_filter;
	std::string mag_filter;
	std::string wrap_s;
	std::string wrap_t;
};

class Model
{
	public:
		Model();
		~Model();

		bool Load();

		ModelResource* mr;
		GLuint iboId;
		GLuint wiredIboId;
		GLuint vboId;
		int nrIndici;
		int nrIndiciWired;
};

class Texture
{
	public:
		Texture();
		~Texture();

		bool Load();

		TextureResource* tr;
		GLuint textureId;
};

class Shader
{
	public:
		Shader();
		~Shader();

		bool Load();

		ShaderResource* sr;
		GLuint programId;
		GLuint vsId;
		GLuint fsId;
};


class ResourceManager
{
	public:
		ResourceManager();
		~ResourceManager();

		bool LoadConfig(const std::string& xmlFilePath);

		Model* LoadModel(const std::string& id);
		Texture* LoadTexture(const std::string& id);
		Shader* LoadShader(const std::string& id);

		const ModelResource* GetModelResource(const std::string& id) const;
		const TextureResource* GetTextureResource(const std::string& id) const;
		const ShaderResource* GetShaderResource(const std::string& id) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<ModelResource>>   modelResources;
		std::unordered_map<std::string, std::unique_ptr<TextureResource>> textureResources;
		std::unordered_map<std::string, std::unique_ptr<ShaderResource>>  shaderResources;

		std::unordered_map<std::string, std::unique_ptr<Model>>   loadedModels;
		std::unordered_map<std::string, std::unique_ptr<Texture>> loadedTextures;
		std::unordered_map<std::string, std::unique_ptr<Shader>>  loadedShaders;

		bool ParseModelNode(void* node);
		bool ParseTextureNode(void* node);
		bool ParseShaderNode(void* node);
};

