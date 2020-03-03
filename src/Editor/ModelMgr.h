#ifndef _Model_Mgr_
#define _Model_Mgr_

#include "Obj.h"
#include <map>

namespace lyd
{
	class Renderable;

	class ModelMgr
	{
	private:
		std::map<std::string, Renderable*> _map;

	private:
		ModelMgr();
		ModelMgr(const ModelMgr&) = delete;
		ModelMgr& operator= (const ModelMgr&) = delete;

	public:
		static ModelMgr& Instance()
		{
			static ModelMgr _instance;
			return _instance;
		}

		Renderable* getRenderable(const char* filename);

		virtual ~ModelMgr();
	};
}

#endif // !_Model_Mgr_



