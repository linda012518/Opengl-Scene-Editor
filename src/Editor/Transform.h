#ifndef _Transform_
#define _Transform_

#include "Obj.h"
#include "CELLMath.hpp"

namespace lyd
{
	class Renderable;
}

namespace lyd
{
	class Transform : public Obj
	{
	public:
		enum {
			State_Normal	=	1 << 0,
			State_Select	=	1 << 1,
			State_Visible	=	1 << 2,
			State_Update	=	1 << 3,
		};

	public:
		quatr			_quat;
		float3			_scale;
		float3			_position;
		aabb3d			_aabb;
		matrix4			_local;
		Renderable*		_renderable;
		unsigned int	_flag;

	public:
		Transform();
		virtual ~Transform();

		void setFlag(unsigned int flag);
		void removeFlag(unsigned int flag);
		bool hasFlag(unsigned int flag);

		Renderable* getAttack();
		void attach(Renderable* renderable);

		void setAabb(const aabb3d& aabb);
		aabb3d getAabb() const;

		void setScale(const float3& scale);
		float3 getScale() const;

		void setPosition(const float3& pos);
		float3 getPosition() const;

		void setQuat(const quatr& quat);
		quatr getQuat() const;

		void setAngle(float angle, float3 axis = float3(0, 1, 0));

		matrix4 local();

		void update();

		void render(const matrix4& vp, char mode);

		void renderAabb(const matrix4& vp);

	};
}

#endif // !_Transform_



