#include "TextureMgr.h"
#include "FileSystem.h"
#include "freeimage\FreeImage.h"
#include "glew\glew.h"

#pragma comment(lib, "FreeImage.lib")

namespace lyd
{
	TextureMgr::TextureMgr() { }

	TextureMgr::~TextureMgr() { }

	unsigned int TextureMgr::getTexture(const char* filename)
	{
		auto itr = _map.find(filename);
		if (itr != _map.end())
		{
			return itr->second;
		}
		unsigned	tex = createTextureFromImage(filename);
		_map[filename] = tex;
		return tex;
	}

	unsigned int TextureMgr::createTextureFromImage(const char* fileName)
	{
		char filePath[1024];
		FileSystem::getResourcePath(fileName, filePath);

		//1 获取图片格式
		FREE_IMAGE_FORMAT       fifmt = FreeImage_GetFileType(filePath, 0);
		if (fifmt == FIF_UNKNOWN)
		{
			return  0;
		}
		//2 加载图片
		FIBITMAP*               dib = FreeImage_Load(fifmt, filePath, 0);

		FREE_IMAGE_COLOR_TYPE   type = FreeImage_GetColorType(dib);

		//! 获取数据指针
		FIBITMAP*   temp = dib;
		dib = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(temp);

		BYTE*   pixels = (BYTE*)FreeImage_GetBits(dib);
		int     width = FreeImage_GetWidth(dib);
		int     height = FreeImage_GetHeight(dib);
		//bgr转rgb
		for (int i = 0; i < width * height * 4; i += 4)
		{
			BYTE temp = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = temp;
		}

		unsigned int res = createTexture(width, height, pixels);
		FreeImage_Unload(dib);
		return      res;

	}

	unsigned int TextureMgr::createTexture(int w, int h, const void* data)
	{
		unsigned int texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		return  texId;

	}
}