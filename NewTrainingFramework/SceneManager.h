#include "sceneObject.h"

class SceneManager {
	public:
		static SceneManager* GetInstance();
		void InitWindow(ESContext* esContext);
		void static Init();
		void Draw();
		void Update(float deltaTime);
		
		std::vector<SceneObject*> currentSceneObjects;

		~SceneManager();
		void LoadScene(const std::string& sceneName);
		void UnloadScene(const std::string& sceneName);
		void SwitchScene(const std::string& sceneName);
	private:
		SceneManager();
		static SceneManager* spInstance;
};