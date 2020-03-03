#include "Camera.h"

namespace lyd
{
	Camera::Camera(const float3& target, const float3& eye, const float3& right)
	{
		_matView	=	matrix4(1);
		_matProj	=	matrix4(1);
		_matWorld	=	matrix4(1);

		_viewSize	=	float2(512, 512);

		_target		=	target;
		_eye		=	eye;
		_dir		=	normalize(_target - _eye);
		_right		=	right;
		_up			=	normalize(cross(_right, _dir));
	}

	Camera::~Camera() { }

	float3	Camera::getEye() const
	{
		return _eye;
	}
	void	Camera::setEye(float3& val)
	{
		_eye = val;
	}

	float3	Camera::getTarget() const
	{
		return _target;
	}
	void	Camera::setTarget(float3& val)
	{
		_target = val;
	}

	float3	Camera::getRight() const
	{
		return _right;
	}
	void	Camera::setRight(float3& val)
	{
		_right = val;
	}

	float3	Camera::getUp() const
	{
		return _up;
	}
	void	Camera::setUp(float3& val)
	{
		_up = val;
	}

	float2	Camera::getViewSize() const
	{
		return _viewSize;
	}
	void	Camera::setViewSize(float2& val)
	{
		_viewSize = val;
	}
	void	Camera::setViewSize(float x, float y)
	{
		_viewSize = float2(x, y);
	}

	const	matrix4& Camera::getProject() const
	{
		return _matProj;
	}
	void	Camera::setProject(const matrix4& proj)
	{
		_matProj = proj;
	}
	const	matrix4& Camera::getView() const
	{
		return _matView;
	}

	float3	Camera::getDir() const
	{
		return _dir;
	}

	void	Camera::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		_matProj = lyd::ortho(left, right, bottom, top, zNear, zFar);
	}
	void	Camera::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		_matProj = lyd::perspective<float>(fovy, aspect, zNear, zFar);
	}

	bool	Camera::project(const float4& world, float4& screen)
	{
		screen = _matProj * _matView * _matWorld * world;
		if (screen.w == 0.0f)
		{
			return false;
		}

		screen.x /= screen.w;
		screen.y /= screen.w;
		screen.z /= screen.w;

		screen.x = screen.x *0.5f + 0.5f;
		screen.y = screen.y *0.5f + 0.5f;
		screen.z = screen.z *0.5f + 0.5f;

		screen.x = screen.x * _viewSize.x;
		screen.y = _viewSize.y - (screen.y * _viewSize.y);
		return true;
	}
	bool	Camera::unProject(const float4& screen, float4& world)
	{
		float4 v;
		v.x = screen.x;
		v.y = screen.y;
		v.z = screen.z;
		v.w = 1.0f;

		v.x = v.x / _viewSize.x;
		v.y = (_viewSize.y - v.y) / _viewSize.y;

		v.x = v.x * 2.0f - 1.0f;
		v.y = v.y * 2.0f - 1.0f;
		v.z = v.z * 2.0f - 1.0f;

		matrix4 inverse = (_matProj * _matView * _matWorld).inverse();
		v = v * inverse;
		if (v.w == 0.0f)
		{
			return false;
		}

		world = v / v.w;
		return true;
	}

	float2	Camera::worldToScreen(const float3& world)
	{
		float4  worlds(world.x, world.y, world.z, 1);
		float4  screens;
		project(worlds, screens);
		return  float2(screens.x, screens.y);
	}
	float3	Camera::screenToWorld(const float2& screen)
	{
		float4  screens(screen.x, screen.y, 0, 1);
		float4  world;
		unProject(screens, world);
		return  float3(world.x, world.y, world.z);
	}
	float3	Camera::screenToWorld(float x, float y)
	{
		return screenToWorld(float2(x, y));
	}

	void	Camera::rotateViewY(float angle)
	{
		_dir		=	rotateY<float>(_dir, angle);
		_up			=	rotateY<float>(_up, angle);
		_right		=	normalize(cross(_dir, _up));
		float	len =	length(_eye - _target);
		_eye		=	_target - _dir * len;
		_matView	=	lyd::lookAt(_eye, _target, _up);
	}
	void	Camera::rotateViewX(float angle)
	{
		matrix4 mat(1);
		mat.rotate(angle, _right);
		_dir		=	_dir * mat;
		_up			=	_up * mat;
		_right		=	normalize(cross(_dir, _up));
		float   len =	length(_eye - _target);
		_eye		=	_target - _dir * len;
		_matView	=	lyd::lookAt(_eye, _target, _up);
	}

	void	Camera::update()
	{
		_dir = normalize(_target - _eye);
		_matView = lyd::lookAt(_eye, _target, _up);
	}

	Ray		Camera::createRayFromScreen(int x, int y)
	{
		float4  minWorld;
		float4  maxWorld;

		float4  screen(float(x), float(y), 0, 1);
		float4  screen1(float(x), float(y), 1, 1);

		unProject(screen, minWorld);
		unProject(screen1, maxWorld);
		Ray     ray;
		ray.setOrigin(float3(minWorld.x, minWorld.y, minWorld.z));

		float3  dir(maxWorld.x - minWorld.x, maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
		ray.setDirection(normalize(dir));
		return  ray;
	}

	float3	Camera::calcIntersectPoint(Ray& ray)
	{
		float3	pos		=	ray.getOrigin();
		float	tm		=	abs(pos.y / ray.getDirection().y);
		float3	target	=	ray.getPoint(tm);
		return	float3(target.x, 0, target.z);
	}
}