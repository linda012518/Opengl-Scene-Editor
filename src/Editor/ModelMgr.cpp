#include "ModelMgr.h"
#include "Model.h"

namespace lyd
{
	ModelMgr::ModelMgr() { }

	ModelMgr::~ModelMgr() { }

	Renderable* ModelMgr::getRenderable(const char* filename)
	{
		auto itr = _map.find(filename);
		if (itr != _map.end())
		{
			return itr->second;
		}

		Model* model = new Model();

		if (model->load(filename))
		{
			_map[filename] = model;
			return model;
		}
		else
		{
			delete model;
			return 0;
		}
	}
}