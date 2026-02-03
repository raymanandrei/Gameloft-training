#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "../Utilities/rapidxml/rapidxml.hpp"
#include "SceneManager.h"
#include "Camera.h"
#include "SceneObject.h"
#include "ResourceManager.h"

using namespace rapidxml;

SceneManager* SceneManager::spInstance = nullptr;	

SceneManager* SceneManager::GetInstance() {
	if (spInstance == nullptr) 
		spInstance = new SceneManager();
	return spInstance;
}

SceneManager::SceneManager() {
	
}

SceneManager::~SceneManager() {
	
}

void SceneManager::InitWindow(ESContext* esContext) {
	std::string xmlPath = "..\\sceneManager.xml";

	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rootNode;
	std::ifstream xmlFile(xmlPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	xml_node<>* root = doc.first_node("sceneManager");
	xml_node<>* gameName = root->first_node("gameName");
	xml_node<>* defaultScreenSize = root->first_node("defaultScreenSize");


	esCreateWindow(esContext, gameName->value(), std::stoi(defaultScreenSize->first_node("width")->value()), std::stoi(defaultScreenSize->first_node("height")->value()), ES_WINDOW_RGB | ES_WINDOW_DEPTH);

}

void SceneManager::Init() {
	
	std::string xmlPath = "..\\sceneManager.xml";

	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rootNode;
	std::ifstream xmlFile(xmlPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());

	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);

	xml_node<>* root = doc.first_node("sceneManager");
	xml_node<>* backgroundColor = root->first_node("backgroundColor");
	xml_node<>* controls = root->first_node("controls");
	xml_node<>* objects = root->first_node("objects");

	for (xml_node<>* object = objects->first_node("object"); object; object = object->next_sibling("object")) {

		SceneObject* newObject = new SceneObject();
		newObject->id = std::stoi(object->first_attribute("id")->value());
		newObject->position[0] = std::stof(object->first_node("position")->first_node("x")->value());
		newObject->position[1] = std::stof(object->first_node("position")->first_node("y")->value());
		newObject->position[2] = std::stof(object->first_node("position")->first_node("z")->value());

		newObject->rotation.x = std::stof(object->first_node("rotation")->first_node("x")->value());
		newObject->rotation.y = std::stof(object->first_node("rotation")->first_node("y")->value());
		newObject->rotation.z = std::stof(object->first_node("rotation")->first_node("z")->value());

		newObject->scale.x = std::stof(object->first_node("scale")->first_node("x")->value());
		newObject->scale.y = std::stof(object->first_node("scale")->first_node("y")->value());
		newObject->scale.z = std::stof(object->first_node("scale")->first_node("z")->value());

		if (object->first_node("color")) {
			std::cout << "Loading color for object ID: " << newObject->id << std::endl;
			newObject->color.x = std::stof(object->first_node("color")->first_node("r")->value());
			newObject->color.y = std::stof(object->first_node("color")->first_node("g")->value());
			newObject->color.z = std::stof(object->first_node("color")->first_node("b")->value());
		}

		ResourceManager* resourceManager = ResourceManager::GetInstance();

		std::string modelId = object->first_node("model")->value();
		std::string shaderId = object->first_node("shader")->value();

		xml_node<>* textureRoot = object->first_node("textures");
		std::string textureId;
		if (textureRoot)
		{
			xml_node<>* textureNode = textureRoot->first_node("texture");
			xml_attribute<>* idAttr = textureNode->first_attribute("id");
			textureId = idAttr->value();
		}

		std::cout << "Loading object ID: " << newObject->id << " Model ID: " << modelId << " Shader ID: " << shaderId << " Texture ID: " << std::endl;

		if (modelId != "generated")
			newObject->model = resourceManager->modelResources[std::stoi(modelId)]; 
		if (shaderId != "generated")
			newObject->shader = resourceManager->shaderResources[std::stoi(shaderId)];
		if (textureId != "generated" && textureId != "")
			newObject->texture = resourceManager->textureResources[std::stoi(textureId)];

		if (modelId != "generated") {
			std::cout << "Model ID: " << object->first_node("model")->value() << std::endl;
			if (textureId != "generated" && textureId != "")
			std::cout << resourceManager->textureResources[std::stoi(textureId)]->file << std::endl;
			//std::cout << resourceManager->shaderResources[std::stoi(shaderId)]->file << std::endl;
			std::cout << resourceManager->modelResources[std::stoi(modelId)]->file << std::endl;
		}

		SceneManager::spInstance->currentSceneObjects.push_back(newObject);
	}
}