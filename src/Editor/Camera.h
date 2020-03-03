#ifndef _Camera_
#define _Camera_

#include "CELLMath.hpp"

namespace lyd
{
	class Camera
	{
	public:
		float3	_eye;
		float3	_up;
		float3	_right;
		float3	_target;
		float3	_dir;

		matrix4 _matView;
		matrix4 _matProj;
		matrix4 _matWorld;

		float2	_viewSize;

	public:
		Camera(const float3& target = float3(0, 0, 0), const float3& eye = float3(100, 100, 100), const float3& right = float3(1, 0, 0));
		virtual ~Camera();

		float3	getEye() const;
		void	setEye(float3& val);

		float3	getTarget() const;
		void	setTarget(float3& val);

		float3	getRight() const;
		void	setRight(float3& val);

		float3	getUp() const;
		void	setUp(float3& val);

		float2	getViewSize() const;
		void	setViewSize(float2& val);
		void	setViewSize(float x, float y);

		const	matrix4& getProject() const;
		void	setProject(const matrix4& proj);
		const	matrix4& getView() const;

		float3	getDir() const;

		void	ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		void	perspective(float fovy, float aspect, float zNear, float zFar);

		bool	project(const float4& world, float4& screen);
		bool	unProject(const float4& screen, float4& world);

		float2	worldToScreen(const float3& world);
		float3	screenToWorld(const float2& screen);
		float3	screenToWorld(float x, float y);

		void	rotateViewY(float angle);
		void	rotateViewX(float angle);

		void	update();

		Ray		createRayFromScreen(int x, int y);

		float3	calcIntersectPoint(Ray& ray);
	};
}

#endif // !_Camera_



