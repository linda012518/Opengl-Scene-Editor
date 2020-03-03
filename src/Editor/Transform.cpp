#include "Transform.h"
#include "Renderable.h"
#include "glew\glew.h"

namespace lyd
{
	Transform::Transform()
	{
		_flag	= State_Normal | State_Visible;
		_scale	= float3(1, 1, 1);
		_quat	= angleAxis(real(0), real3(0, 1, 0));
		_local	= makeTransform(_position, _scale, _quat);
		sprintf(_name, "CELLNode-%p", this);
	}

	Transform::~Transform() { }

	void Transform::setFlag(unsigned int flag)
	{
		_flag |= flag;
	}
	void Transform::removeFlag(unsigned int flag)
	{
		_flag &= ~flag;
	}
	bool Transform::hasFlag(unsigned int flag)
	{
		return _flag & flag ? true : false;
	}

	Renderable* Transform::getAttack()
	{
		return _renderable;
	}
	void Transform::attach(Renderable* renderable)
	{
		_renderable = renderable;
	}

	void Transform::setAabb(const aabb3d& aabb)
	{
		_aabb = aabb;
	}
	aabb3d Transform::getAabb() const
	{
		return _aabb;
	}

	void Transform::setScale(const float3& scale)
	{
		_scale = scale;
	}
	float3 Transform::getScale() const
	{
		return _scale;
	}

	void Transform::setPosition(const float3& pos)
	{
		_position = pos;
	}
	float3 Transform::getPosition() const
	{
		return _position;
	}

	void Transform::setQuat(const quatr& quat)
	{
		_quat = quat;
	}
	quatr Transform::getQuat() const
	{
		return _quat;
	}

	void Transform::setAngle(float angle, float3 axis)
	{
		_quat = lyd::angleAxis(angle, axis);
	}

	matrix4 Transform::local()
	{
		return _local;
	}

	void Transform::update()
	{
		_local = makeTransform(_position, _scale, _quat);
	}

	void Transform::render(const matrix4& vp, char mode)
	{
		if (_renderable)
		{
			matrix4	mvp = vp * _local;
			glLoadMatrixf(mvp.data());
			_renderable->render(this, mode);
			if (_flag & State_Select)
			{
				renderAabb(vp);
			}
		}
	}

	void Transform::renderAabb(const matrix4& vp)
	{
		float3			vertex[8];
		aabb3d			aabb = _renderable->getAabb();
		aabb.transform(_local);//相当于变换到模型矩阵了
		aabb.getAllCorners(vertex);//变换完再得到包围

		unsigned char	index[] =
		{
			0,1,	1,2,	2,3,	3,0,
			4,5,	5,6,	6,7,	7,4,
			1,5,	2,4,	0,6,	3,7
		};

		//这行不要是obb包围
		glLoadMatrixf(vp.data());

		glColor3f(1, 0, 0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(float3), vertex);
		glDrawElements(GL_LINES, sizeof(index), GL_UNSIGNED_BYTE, index);
		glDisableClientState(GL_VERTEX_ARRAY);
		glColor3f(1, 1, 1);
	}
}