#ifndef _Obj_
#define _Obj_

namespace lyd
{
	class Obj
	{
	public:
		Obj(void* user = 0);
		virtual ~Obj();

		void setUserData(void* data);
		void* getUserData();
		
		char* getName();
		void setName(const char* name);

	protected:
		void*	_user;
		char	_name[64];
	};
}

#endif // !_Obj_

