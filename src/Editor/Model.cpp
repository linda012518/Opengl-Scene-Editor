#include "Model.h"
#include "FileReader.h"
#include "FileSystem.h"
#include "TextureMgr.h"
#include "glew\glew.h"

namespace lyd
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	bool Model::load(const char* filename)
	{
		char filePath[1024];
		lyd::FileSystem::getResourcePath(filename, filePath);

		FileReader  file(filePath);
		if (file.isBad())
		{
			return  false;
		}
		file.readAll();
		try
		{
			rapidxml::xml_document<>    doc;
			rapidxml::xml_node<>*	    rootNode = 0;
			char*                       xmlData = (char*)file.data();
			doc.parse<0>(xmlData);
			rootNode = doc.first_node("mesh");
			if (rootNode == 0)
			{
				return  false;
			}
			rapidxml::xml_node<>*       node = rootNode->first_node();
			for (; node != 0; node = node->next_sibling())
			{
				if (strcmp(node->name(), "vertex") == 0)
				{
					parseVertex(node);
				}
				else if (strcmp(node->name(), "face") == 0)
				{
					parseFace(node);
				}
			}
			return  true;
		}
		catch (...)
		{
			return  false;
		}

	}

	void Model::render(Transform*, char mode)
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(V3N3U2), &_vertex[0].x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(V3N3U2), &_vertex[0].u);

		switch (mode)
		{
		case 0:
			glDrawElements(GL_POINTS, _face.size() * 3, GL_UNSIGNED_SHORT, &_face[0]);
			break;
		case 1:
			glDrawElements(GL_LINE_LOOP, _face.size() * 3, GL_UNSIGNED_SHORT, &_face[0]);
			break;
		case 2:
			glDrawElements(GL_TRIANGLES, _face.size() * 3, GL_UNSIGNED_SHORT, &_face[0]);
			break;
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Model::updateAabb()
	{
		float3  vMin = float3(FLT_MAX, FLT_MAX, FLT_MAX);
		float3  vMax = float3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (size_t i = 0; i < _vertex.size(); ++i)
		{
			V3N3U2&     pData = _vertex[i];

			vMin.x = tmin(pData.x, vMin.x);
			vMin.y = tmin(pData.y, vMin.y);
			vMin.z = tmin(pData.z, vMin.z);

			vMax.x = tmax(pData.x, vMax.x);
			vMax.y = tmax(pData.y, vMax.y);
			vMax.z = tmax(pData.z, vMax.z);
		}
		_aabb.setExtents(vMin, vMax);
	}

	void Model::parseVertex(rapidxml::xml_node<>* node)
	{
		rapidxml::xml_attribute<>*  attrCount = node->first_attribute("count");
		rapidxml::xml_attribute<>*  attrSizeOf = node->first_attribute("size");
		rapidxml::xml_node<>*       cNode = 0;
		const char*                 pValue = 0;
		unsigned                    dataSize = 0;

		if (attrCount == 0 || attrSizeOf == 0)
		{
			return;
		}

		int dataCount = atoi(attrCount->value());
		if (dataCount == 0)
		{
			return;
		}
		_vertex.resize(dataCount);
		cNode = node->first_node();

		for (int i = 0; cNode && i < dataCount; cNode = cNode->next_sibling(), ++i)
		{
			V3N3U2& vertex = _vertex[i];
			pValue = cNode->value();
			sscanf(
				pValue
				, "%f %f %f %f %f %f %f %f"
				, &vertex.x
				, &vertex.y
				, &vertex.z
				, &vertex.nx
				, &vertex.ny
				, &vertex.nz
				, &vertex.u
				, &vertex.v
			);
		}

		updateAabb();

	}
	void Model::parseFace(rapidxml::xml_node<>* node)
	{
		rapidxml::xml_attribute<>*  attrType = node->first_attribute("type");
		rapidxml::xml_attribute<>*  attrCount = node->first_attribute("count");
		rapidxml::xml_attribute<>*  attrTexture = node->first_attribute("texture");
		rapidxml::xml_node<>*       cNode = node->first_node();

		if (attrType == 0 || attrCount == 0 || cNode == 0)
		{
			return;
		}
		int dataCount = atoi(attrCount->value());

		if (dataCount == 0)
		{
			return;
		}
		if (attrTexture)
		{
			const char* pTextureFile = attrTexture->value();

			_texture = TextureMgr::Instance().getTexture(pTextureFile);
		}
		_face.resize(dataCount);

		for (int i = 0; cNode && i < dataCount; cNode = cNode->next_sibling(), ++i)
		{
			int     r, g, b;
			Face&   face = _face[i];
			sscanf(cNode->value(), "%d %d %d", &r, &g, &b);
			face.x = (unsigned short)(r);
			face.y = (unsigned short)(g);
			face.z = (unsigned short)(b);
		}

	}
}