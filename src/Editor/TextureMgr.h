#ifndef _Texture_Mgr_
#define _Texture_Mgr_

#include "Obj.h"
#include <map>

namespace lyd
{
	class TextureMgr : public Obj
	{
	private:
		std::map<std::string, unsigned int> _map;

	private:
		TextureMgr();
		TextureMgr(const TextureMgr&) = delete;
		TextureMgr& operator= (const TextureMgr&) = delete;

	public:
		static TextureMgr& Instance()
		{
			static TextureMgr _instance;
			return _instance;
		}

		virtual ~TextureMgr();

		unsigned int getTexture(const char* filename);

		unsigned int createTextureFromImage(const char* fileName);

		unsigned int createTexture(int w, int h, const void* data);
	};
}

#endif // !_Texture_Mgr_



