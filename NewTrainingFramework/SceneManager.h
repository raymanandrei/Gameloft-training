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
	private:
		SceneManager();
		static SceneManager* spInstance;
};