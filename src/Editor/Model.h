#ifndef _Model_
#define _Model_

#include "Renderable.h"
#include "xml\rapidxml.hpp"

namespace lyd
{
	class Model : public Renderable
	{
	public:
		struct V3N3U2
		{
			float   x, y, z;
			float   nx, ny, nz;
			float   u, v;
		};

		struct Face
		{
			short x, y, z;
		};

		typedef std::vector<V3N3U2>		ArrayVertex;
		typedef std::vector<Face>		ArrayFace;

	private:
		Model(const Model&);
		Model& operator = (const Model&);
	public:
		Model();
		virtual ~Model();

		virtual bool load(const char* filename);

		virtual void render(Transform*, char mode);

	protected:
		void updateAabb();

		virtual void parseVertex(rapidxml::xml_node<>* node);
		virtual void parseFace(rapidxml::xml_node<>* node);

	public:
		ArrayVertex		_vertex;
		ArrayFace		_face;
		unsigned int	_texture;
	};
}

#endif // !_Model_


