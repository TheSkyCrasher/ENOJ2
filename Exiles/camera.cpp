#include "camera.h"
#include "input.h"
#include "window.h"

Camera::Camera(const Matrix4f& projection) :
m_projection(projection), mouseLocked(false)
{
	m_transform.SetPos(Vector3f(0.0f,2.0f,0.0f));
}

Matrix4f Camera::GetViewProjection() const
{
	Matrix4f cameraRotation = m_transform.GetTransformedRot().Conjugate().ToRotationMatrix();
	Matrix4f cameraTranslation;

	cameraTranslation.InitTranslation(m_transform.GetTransformedPos() * -1);

	return m_projection * cameraRotation * cameraTranslation;
}

void Camera::UpdatePosition(float delta)
{
	float movAmt = 10.0f * delta;

	if (Input::GetKey(Input::KEY_W))
		Move(m_transform.GetRot().GetForward(), movAmt);
	else if(Input::GetKey(Input::KEY_S))
		Move(m_transform.GetRot().GetBack(), movAmt);

	if (Input::GetKey(Input::KEY_A))
		Move(m_transform.GetRot().GetLeft(), movAmt);
	else if (Input::GetKey(Input::KEY_D))
		Move(m_transform.GetRot().GetRight(), movAmt);

	if (Input::GetKey(Input::KEY_SPACE))
		Move(m_transform.GetRot().GetUp(), movAmt);
	else if (Input::GetKey(Input::KEY_LCTRL))
		Move(m_transform.GetRot().GetDown(), movAmt);
}

void Camera::UpdateRotation(float delta)
{
	float m_sensitivity = 0.5f;

	if (Input::GetMouseDown(Input::RIGHT_MOUSE))
	{
		Input::SetCursor(true);
		mouseLocked = false;
	}

	if (mouseLocked)
	{
		Vector2f centerPosition = Vector2f((float)Window::GetWidth() / 2.0f, (float)Window::GetHeight() / 2.0f);
		Vector2f deltaPos = Input::GetMousePosition() - centerPosition;

		bool rotY = deltaPos.GetX() != 0;
		bool rotX = deltaPos.GetY() != 0;

		if (rotY)
			m_transform.Rotate(Vector3f(0, 1, 0), ToRadians(deltaPos.GetX() * m_sensitivity));
		if (rotX)
			m_transform.Rotate(m_transform.GetRot().GetRight(), ToRadians(deltaPos.GetY() * m_sensitivity));

		if (rotY || rotX)
			Input::SetMousePosition(centerPosition);
	}

	if (Input::GetMouseDown(Input::LEFT_MOUSE))
	{
		Vector2f centerPosition = Vector2f((float)Window::GetWidth() / 2.0f, (float)Window::GetHeight() / 2.0f);
		Input::SetCursor(false);
		Input::SetMousePosition(centerPosition);
		mouseLocked = true;
	}
}

void Camera::Move(const Vector3f& direction, float amt)
{
	m_transform.SetPos(m_transform.GetPos() + (direction * amt));
}