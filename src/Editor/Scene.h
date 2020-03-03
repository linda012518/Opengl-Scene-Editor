#ifndef _Scene_
#define _Scene_

#include <vector>

namespace lyd
{
	class Transform;
	class Camera;

	class Scene
	{
	public:
		struct  V3U2
		{
			float   x, y, z;
			float   u, v;
		};

	protected:
		std::vector<Transform*> _vector;

	public:
		Camera*			_camera = nullptr;
		unsigned int	_texGround;
	public:
		Scene();
		virtual ~Scene();

		virtual void initScene();
		virtual void destoryScene();

		virtual void clear();

		void renderScene(char mode);

		virtual Transform* createNode(const char* name, int x, int y);
	};
}

#endif // !_Scene_
