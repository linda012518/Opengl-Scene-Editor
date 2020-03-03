#include "Obj.h"
#include "string.h"

namespace lyd
{
	Obj::Obj(void* user)
	{
		_user = user;
	}

	Obj::~Obj() { }

	void	Obj::setUserData(void* data)
	{
		_user = data;
	}
	void*	Obj::getUserData()
	{
		return _user;
	}

	char*	Obj::getName()
	{
		return _name;
	}
	void	Obj::setName(const char* name)
	{
		strncpy(_name, name, sizeof(_name));
	}
}