#ifndef _GL_Context_
#define _GL_Context_

#include <Windows.h>

namespace lyd
{
	class GLContext
	{
	public:
		GLContext();
		virtual ~GLContext();

		bool setup(HWND hwnd, HDC hdc);

		void shutdown();

		void swapBuffer();

		bool makeCurrent();

		HWND getHWND();

	protected:
		HWND	_hwnd;	//´°¿Ú¾ä±ú
		HDC		_hdc;	//dc
		HGLRC	_hrc;	//rc
		int		_format;
	};
}

#endif // !_GL_Context_

