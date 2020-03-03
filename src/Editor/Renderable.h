#ifndef _Renderable_
#define _Renderable_

#include "Obj.h"
#include "CELLMath.hpp"

namespace lyd
{
	class Transform;
}

namespace lyd
{
	class Renderable : public Obj
	{
	protected:
		aabb3d	_aabb;
		char	_modelFileName[128];

	public:
		Renderable();
		virtual ~Renderable();

		virtual void render(Transform*, char mode);

		aabb3d& getAabb();

		void setFileName(const char* filename);
		char* getFileName();
	};
}

#endif // !_Renderable_



