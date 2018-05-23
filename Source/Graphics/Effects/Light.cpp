#include "Light.h"
#include "../GraphicsStates/GS3D.h"
#include "../GraphicsManager.h"

Bitz::GFX::Effects::Light::Light()
{
	SetRadius(100);
}

void Bitz::GFX::Effects::Light::SetRadius(float_t radius)
{
	if (radius < 7) radius = 8;
	if (radius >= 3250) radius = 3249;

	int16_t range[]{ 7,13,20,32,50,65,100,160,200,325,600,3250 };
	double_t linear[]{ 0.7,0.35,0.22,0.14,0.09,0.07,0.045,0.027,0.022,0.014,0.007,0.0014 };
	double_t quadratic[]{ 1.8,0.44,0.20,0.07,0.032,0.017,0.0075,0.0028,0.0019,0.0007,0.0002,0.000007 };

	int indexOfFirstMatching = 0;
	for (int i = 0; i < 12; i++)
	{
		if (radius <= range[i])
		{
			indexOfFirstMatching = i;
			break;
		}
	}

	const double_t offset = (radius - range[indexOfFirstMatching]) / (range[indexOfFirstMatching + 1] - range[indexOfFirstMatching]);

	_Linear = static_cast<float_t>((linear[indexOfFirstMatching] * (1 - offset)) + (linear[indexOfFirstMatching + 1] * (offset)));
	_Quadratic = static_cast<float_t>((quadratic[indexOfFirstMatching] * (1 - offset)) + (quadratic[indexOfFirstMatching + 1] * (offset)));

}

void Bitz::GFX::Effects::Light::SetColour(const Bitz::Vector3F& newColour)
{
	_Colour = newColour;
}

Bitz::Vector3F Bitz::GFX::Effects::Light::GetColour() const
{
	return _Colour;
}

void Bitz::GFX::Effects::Light::SetupShader(const Bitz::GFX::Shaders::Shader_Ptr& shader, const int  lightID) const
{
	shader->SetVariable(fmt::format("PointLights[{0}].Position", lightID), glm::vec3(GetPosition().X, GetPosition().Y, GetPosition().Z));
	shader->SetVariable(fmt::format("PointLights[{0}].Linear", lightID), _Linear);
	shader->SetVariable(fmt::format("PointLights[{0}].Quadratic", lightID), _Quadratic);
	shader->SetVariable(fmt::format("PointLights[{0}].Colour", lightID), _Colour);
}
