#include "Renderable.h"

namespace lyd
{
	Renderable::Renderable() { }

	Renderable::~Renderable() { }

	void Renderable::render(Transform*, char mode) { }

	aabb3d& Renderable::getAabb()
	{
		return _aabb;
	}

	void Renderable::setFileName(const char* filename)
	{
		strncpy(_modelFileName, filename, sizeof(_modelFileName));
	}
	char* Renderable::getFileName()
	{
		return _modelFileName;
	}
}