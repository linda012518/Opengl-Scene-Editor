#include "Scene.h"
#include "Camera.h"
#include "Renderable.h"
#include "Transform.h"
#include "TextureMgr.h"
#include "ModelMgr.h"
#include "FileSystem.h"
#include "glew\glew.h"

namespace lyd
{
	Scene::Scene()
	{
		_camera = new Camera();
	}

	Scene::~Scene()
	{
		destoryScene();
	}

	void Scene::renderScene(char mode)
	{
		V3U2    vertexs[] =
		{
			{ -1, 0,   1,  0,  0 },
			{ 1, 0,   1,  1,  0 },
			{ 1, 0,  -1,  1,  1 },

			{ -1, 0,   1,  0,  0 },
			{ 1, 0,  -1,  1,  1 },
			{ -1, 0,  -1,  0,  1 }
		};

		for (int i = 0; i < 6; ++i)
		{
			vertexs[i].x *= 100;
			vertexs[i].z *= 100;

			vertexs[i].u *= 10;
			vertexs[i].v *= 10;
		}

		glBindTexture(GL_TEXTURE_2D, _texGround);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(V3U2), &vertexs[0].x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3U2), &vertexs[0].u);

		switch (mode)
		{
		case 0:
			glDrawArrays(GL_POINTS, 0, 6);
			break;
		case 1:
			glDrawArrays(GL_LINE_LOOP, 0, 6);
			break;
		case 2:
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		}
		

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);


		for (auto itr = _vector.begin(); itr != _vector.end(); ++itr)
		{
			Transform*	node = *itr;
			node->update();
			node->render(_camera->getProject() * _camera->getView(), mode);
		}

	}

	void Scene::initScene()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);

		_texGround = TextureMgr::Instance().getTexture("data/image/1.jpg");
	}
	void Scene::destoryScene()
	{
		if (_camera)
		{
			delete _camera;
			_camera = nullptr;
		}

		clear();
	}

	void Scene::clear()
	{
		for (auto itr = _vector.begin(); itr != _vector.end(); ++itr)
		{
			Transform*	node = *itr;
			delete node;
			node = nullptr;
		}
		_vector.clear();
	}

	Transform* Scene::createNode(const char* name, int x, int y)
	{
		Ray		ray = _camera->createRayFromScreen(x, y);
		float3	pos = _camera->calcIntersectPoint(ray);

		Transform*	node = new Transform();
		Renderable*	model = ModelMgr::Instance().getRenderable(name);
		if (model == 0)
		{
			return 0;
		}
		model->setFileName(name);
		node->attach(model);
		node->setAabb(model->getAabb());
		node->setScale(float3(5, 5, 5));
		node->setPosition(pos);

		_vector.push_back(node);
		return node;
	}
}