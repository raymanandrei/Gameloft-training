#include "stdafx.h"
#pragma push_macro("memory")
#undef memory

#include <memory>
#include "../Utilities/rapidxml/rapidxml.hpp"
#include "../Utilities/rapidxml/rapidxml_iterators.hpp"
#include "../Utilities/rapidxml/rapidxml_print.hpp"
#include "../Utilities/rapidxml/rapidxml_utils.hpp"

#pragma pop_macro("memory")



#include "ResourceManager.h"
#include <string>
#include <iostream>


#include <fstream>
#include <vector>

using namespace rapidxml;

std::string xmlPath = "..\\resourceManager.xml";

ResourceManager* ResourceManager::spInstance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	spInstance = nullptr;
}

void ResourceManager::Init() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rootNode;
	std::ifstream xmlFile(xmlPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);


	xml_node<>* root = doc.first_node("resourceManager");
	xml_node<>* models = root->first_node("models");
	xml_node<>* shaders = root->first_node("shaders");
	xml_node<>* textures = root->first_node("textures");
	xml_node<>* folder = models->first_node("folder");

		for (xml_node<>* folder = models->first_node("folder");
			folder; folder = folder->next_sibling("folder"))
		{
			const char* folderPath =
				folder->first_attribute("path")->value();

			//std::cout << folderPath << "\n";

			for (xml_node<>* model = folder->first_node("model");
				model; model = model->next_sibling("model"))
			{
				int id = std::stoi(model->first_attribute("id")->value());

				xml_node<>* file = model->first_node("file");
				//std::cout  << id<< file->value() << "\n";
			}
		}


		folder = shaders->first_node("folder");
		const char* path = folder->first_attribute("path")->value();

		//std::cout << path << "\n";

		for (xml_node<>* shader = folder->first_node("shader");
			shader; shader = shader->next_sibling("shader"))
		{
			int id = std::stoi(shader->first_attribute("id")->value());

			const char* vs = shader->first_node("vs")->value();
			const char* fs = shader->first_node("fs")->value();

			//std::cout << id<< vs << fs << "\n";
		}
	
		folder = textures->first_node("folder");
		path = folder->first_attribute("path")->value();

		//std::cout << path << "\n";

		for (xml_node<>* tex = folder->first_node("texture");
			tex; tex = tex->next_sibling("texture"))
		{
			int id = std::stoi(tex->first_attribute("id")->value());
			const char* type = tex->first_attribute("type")->value();

			//std::cout << id << type << "\n";

			//std::cout << tex->first_node("file")->value() << "\n";
			//std::cout << tex->first_node("min_filter")->value() << "\n";
			//std::cout <<tex->first_node("mag_filter")->value() << "\n";
			//std::cout << tex->first_node("wrap_s")->value() << "\n";
			//std::cout <<tex->first_node("wrap_t")->value() << "\n";
		}


	xmlFile.close();
}

ResourceManager* ResourceManager::GetInstance() {
		if (!spInstance) 
			spInstance = new ResourceManager();
		return spInstance;
}




Model::Model() {
}

Model::~Model() {
}

Texture::Texture() {
}

Texture::~Texture() {
}

Shader::Shader() {
}

Shader::~Shader() {
}

