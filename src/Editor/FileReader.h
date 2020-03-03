#ifndef _File_Reader_
#define _File_Reader_

#include <stdio.h>

namespace lyd
{
	class FileReader
	{
	public:
		FileReader(const char* fileName = 0);
		virtual ~FileReader();
		bool    open(const char* fileName);

		void    close();
		bool    isGood();
		bool    isBad();

		void    seek(unsigned pos);
		void    seekEnd();
		void    seekBegin();
		size_t  tell();
		void    readAll();

		unsigned    read(void* buffer, unsigned size);
		void*       data();
		unsigned    size();
		unsigned    length();
		bool        end();

	protected:
		char*	readFile(unsigned& length);

	private:
		char*       _buffer;
		unsigned    _length;
		FILE*       _file;
	};
}

#endif // !_File_Reader_

