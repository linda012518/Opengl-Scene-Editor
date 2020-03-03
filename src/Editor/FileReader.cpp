#include "FileReader.h"

namespace lyd
{
	FileReader::FileReader(const char* fileName)
	{
		_buffer = 0;
		_length = 0;
		if (fileName)
		{
			_file = fopen(fileName, "rb");
		}
	}

	FileReader::~FileReader()
	{
		close();
	}

	bool    FileReader::open(const char* fileName)
	{
		close();
		_file = fopen(fileName, "rb");
		return  _file != 0;
	}

	void    FileReader::close()
	{
		if (_file)
		{
			fclose(_file);
			_file = 0;
		}
		if (_buffer)
		{
			delete[]_buffer;
			_buffer = 0;
			_length = 0;
		}
	}
	bool    FileReader::isGood()
	{
		return  _file != 0;
	}
	bool    FileReader::isBad()
	{
		return  _file == 0;
	}

	void    FileReader::seek(unsigned pos)
	{
		fseek(_file, pos, SEEK_SET);
	}
	void    FileReader::seekEnd()
	{
		fseek(_file, 0, SEEK_END);
	}
	void    FileReader::seekBegin()
	{
		fseek(_file, 0, SEEK_SET);
	}
	size_t  FileReader::tell()
	{
		return  ftell(_file);
	}
	void    FileReader::readAll()
	{
		_buffer = readFile(_length);
	}

	unsigned    FileReader::read(void* buffer, unsigned size)
	{
		return  fread(buffer, 1, size, _file);
	}
	void*       FileReader::data()
	{
		return  _buffer;
	}
	unsigned    FileReader::size()
	{
		return  _length;
	}
	unsigned    FileReader::length()
	{
		return  _length;
	}
	bool        FileReader::end()
	{
		return  feof(_file) != 0;
	}

	char*		FileReader::readFile(unsigned& length)
	{
		if (_file)
		{
			fseek(_file, 0, SEEK_END);
			length = ftell(_file);
			fseek(_file, 0, SEEK_SET);
			char* buffer = new char[length + 1];
			fread(buffer, 1, length, _file);
			buffer[length] = 0;
			return  buffer;
		}
		return  0;
	}
}