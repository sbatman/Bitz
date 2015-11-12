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
	_ViewMatrix = glm::mat4();
	if (abs(_Rotation.X > EPSILON)) glm::rotate(_ViewMatrix, -_Rotation.X, glm::vec3(1.0f, 0.0f, 0.0f));
	if (abs(_Rotation.Y > EPSILON)) glm::rotate(_ViewMatrix, -_Rotation.Y, glm::vec3(0.0f, 1.0f, 0.0f));
	if (abs(_Rotation.Z > EPSILON)) glm::rotate(_ViewMatrix, -_Rotation.Z, glm::vec3(0.0f, 0.0f, 1.0f));
	_ViewMatrix = glm::translate(_ViewMatrix, glm::vec3(_Position.X, _Position.Y, _Position.Z));
	glLoadMatrixf(glm::value_ptr(_ViewMatrix));
}

void Bitz::GFX::Camera::MakeActive()
{
	switch (_CurrentMode)
	{
	case Perspective:
	{
		glMatrixMode(GL_PROJECTION);
		_ProjectionMatrix = glm::perspective(_FOV, GraphicsManager::GetScreenSize().X / static_cast<float>(GraphicsManager::GetScreenSize().Y), 0.001f, 1000.0f);
		if (abs(_Zoom - 1) > EPSILON)
		{
			_ProjectionMatrix = glm::translate(_ProjectionMatrix, glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
			_ProjectionMatrix = glm::scale(_ProjectionMatrix, glm::tvec3<float, glm::precision::defaultp>(_Zoom));
			_ProjectionMatrix = glm::translate(_ProjectionMatrix, -glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
		}
		glLoadMatrixf(glm::value_ptr(_ProjectionMatrix));
	}
	break;
	case Ortho:
	{
		glMatrixMode(GL_PROJECTION);
		_ProjectionMatrix = glm::ortho(0.0, (double_t)GraphicsManager::GetScreenSize().X, (double_t)GraphicsManager::GetScreenSize().Y, 0.0, 0.0, 1.0);
		if (abs(_Zoom - 1) > EPSILON)
		{
			_ProjectionMatrix = glm::translate(_ProjectionMatrix, glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
			_ProjectionMatrix = glm::scale(_ProjectionMatrix, glm::tvec3<float, glm::precision::defaultp>(_Zoom));
			_ProjectionMatrix = glm::translate(_ProjectionMatrix, -glm::tvec3<float, glm::precision::defaultp>(GraphicsManager::GetScreenSize().X *0.5f, GraphicsManager::GetScreenSize().Y *0.5f, 0));
		}
		glLoadMatrixf(glm::value_ptr(_ProjectionMatrix));
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

glm::mat4 Bitz::GFX::Camera::GetProjectionMatrix() const
{
	return _ProjectionMatrix;
}

glm::mat4 Bitz::GFX::Camera::GetViewMatrix() const
{
	return _ViewMatrix;
}