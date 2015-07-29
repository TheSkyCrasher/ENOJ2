#pragma once

#include "transform.h"

struct Camera
{
public:
	Camera(const Matrix4f& projection);

	void Update(float delta)
	{
		UpdatePosition(delta);
		UpdateRotation(delta);
	}

	Matrix4f GetViewProjection() const;
	Matrix4f GetView() const;
	const Vector3f& GetPos() { return m_transform.GetPos(); }
private:
	Transform m_transform;
	Matrix4f m_projection;

	void UpdatePosition(float delta);
	void UpdateRotation(float delta);

	void Move(const Vector3f& direction, float amt);

	bool mouseLocked;
};