#include "../Common.h"
#include "Camera.h"
#include "GraphicsManager.h"

Bitz::GFX::Camera::Camera()
{
	_ForceMakeActiveOnApply = false;
	_CurrentMode = CameraMode::Perspective;
	_FOV = 75;
	_Zoom = 1;
}

Bitz::GFX::Camera::~Camera()
{
}

void Bitz::GFX::Camera::Apply()
{
	if (_ForceMakeActiveOnApply)
	{
		MakeActive();
		_ForceMakeActiveOnApply = false;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (abs(_Rotation.X > EPSILON)) glRotatef(-_Rotation.X, 1, 0, 0);
	if (abs(_Rotation.Y > EPSILON)) glRotatef(-_Rotation.Y, 0, 1, 0);
	if (abs(_Rotation.Z > EPSILON)) glRotatef(-_Rotation.Z, 0, 0, 1);
	glTranslatef(-_Position.X, -_Position.Y, -_Position.Z);
}

void Bitz::GFX::Camera::MakeActive() const
{
	switch (_CurrentMode)
	{
	case Perspective:
	{
		glMatrixMode(GL_PROJECTION);
		glm::mat4 projection = glm::perspective(_FOV, GraphicsManager::GetScreenSize().X / static_cast<float>(GraphicsManager::GetScreenSize().Y), 0.001f, 1000.0f);
		if (abs(_Zoom - 1) > 0.001f)
		{
			projection = glm::translate(projection, glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
			projection = glm::scale(projection, glm::tvec3<float, glm::precision::defaultp>(_Zoom));
			projection = glm::translate(projection, -glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
		}
		glLoadMatrixf(glm::value_ptr(projection));
	}
	break;
	case Ortho:
	{
		glMatrixMode(GL_PROJECTION);
		glm::mat4 projection = glm::ortho(0.0, (double_t)GraphicsManager::GetScreenSize().X, (double_t)GraphicsManager::GetScreenSize().Y, 0.0, 0.0, 1.0);
		if (abs(_Zoom - 1) > 0.001f)
		{
			projection = glm::translate(projection, glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
			projection = glm::scale(projection, glm::tvec3<float, glm::precision::defaultp>(_Zoom));
			projection = glm::translate(projection, -glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
		}
		glLoadMatrixf(glm::value_ptr(projection));
	}
	break;
	default: break;
	}
}

void Bitz::GFX::Camera::SetFOV(const float_t newValue)
{
	if (_FOV == newValue) return;
	_FOV = newValue;
	if (_FOV < 1)_FOV = 1;
	if (_FOV > 360)_FOV = 360;
	_ForceMakeActiveOnApply = true;
}

float_t Bitz::GFX::Camera::GetZoom() const
{
	return _Zoom;
}

void Bitz::GFX::Camera::SetZoom(const float_t newZoom)
{
	if (_Zoom == newZoom) return;
	_Zoom = newZoom;
	_ForceMakeActiveOnApply = true;
}

void Bitz::GFX::Camera::SetMode(const Camera::CameraMode newMode)
{
	if (_CurrentMode == newMode) return;
	_CurrentMode = newMode;
	_ForceMakeActiveOnApply = true;
}

float_t Bitz::GFX::Camera::GetFOV() const
{
	return _FOV;
}

Bitz::GFX::Camera::CameraMode Bitz::GFX::Camera::GetMode() const
{
	return _CurrentMode;
}