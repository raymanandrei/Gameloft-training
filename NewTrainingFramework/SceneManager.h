#include <vector>
#include "Camera.h"
#include "sceneObject.h"

class SceneManager {
	public:
		static SceneManager* GetInstance();
		void InitWindow(ESContext* esContext);
		void static Init();
		void Draw(ESContext* esContext);
		void Update(float deltaTime);
		
		float totalTime;
		Camera camera;

		std::vector<SceneObject*> currentSceneObjects;

		~SceneManager();
	private:
		SceneManager();
		static SceneManager* spInstance;
};