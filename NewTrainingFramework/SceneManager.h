
#include "../Utilities/utilities.h"
#include <string>


class SceneManager {
	public:
		void Init(); //parsing xml and applying settings
		void InitWindow();
		void Draw();
		void Update(float deltaTime);
		SceneManager();
		~SceneManager();
		void LoadScene(const std::string& sceneName);
		void UnloadScene(const std::string& sceneName);
		void SwitchScene(const std::string& sceneName);
};