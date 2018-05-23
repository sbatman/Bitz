#include "Stock3D.h"
#include "../../Content/IO.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			Stock3D::Stock3D()
			{
				std::string vertexShader = "#version 150																			\n";
				vertexShader += "precision highp float;																				\n";
				vertexShader += "uniform mat4 ProjectionMatrix;																		\n";
				vertexShader += "uniform mat4 ViewMatrix;																			\n";
				vertexShader += "uniform mat4 ModelMatrix;																			\n";
				vertexShader += "																									\n";
				vertexShader += "attribute vec4 in_Position;																		\n";
				vertexShader += "attribute vec3 in_Normal;																			\n";
				vertexShader += "attribute vec4 in_Color;																			\n";
				vertexShader += "attribute vec2 in_TexCoordinate;																	\n";
				vertexShader += "attribute vec3 in_Additional;																		\n";
				vertexShader += "																									\n";
				vertexShader += "out vec3 out_Position;																				\n";
				vertexShader += "out vec3 out_Normal;																				\n";
				vertexShader += "out vec4 out_Color;																				\n";
				vertexShader += "out vec2 out_TexCoordinate;																		\n";
				vertexShader += "out vec3 out_Additional;																			\n";
				vertexShader += "																									\n";
				vertexShader += "void main()																						\n";
				vertexShader += "{																									\n";
				vertexShader += "	mat4 MVMatrix = ViewMatrix*ModelMatrix;															\n";
				vertexShader += "	out_Position = vec3(ModelMatrix*vec4(in_Position.rgb, 1.0));									\n";
				vertexShader += "	out_Normal = mat3(transpose(inverse(ModelMatrix))) *in_Normal;									\n";
				vertexShader += "	out_Color = in_Color;																			\n";
				vertexShader += "	out_TexCoordinate = in_TexCoordinate;															\n";
				vertexShader += "	out_Additional = in_Additional;																	\n";
				vertexShader += "	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position.rgb, 1.0);			\n";
				vertexShader += "}																									  ";

				std::string fragmentShader = "#version 150															\n";

				fragmentShader += "precision highp float;																													\n";
				fragmentShader += "																																			\n";
				fragmentShader += "	struct PointLight {																														\n";
				fragmentShader += "		vec3 Position;																														\n";
				fragmentShader += "		float Linear;																														\n";
				fragmentShader += "		float Quadratic;																													\n";
				fragmentShader += "		vec3 Colour;																															\n";
				fragmentShader += "	};																																		\n";
				fragmentShader += "																																			\n";
				fragmentShader += "uniform PointLight PointLights[10];																										\n";
				fragmentShader += "uniform sampler2D Texture0;		//diffuse																								\n";
				fragmentShader += "uniform sampler2D Texture1;		//specular																								\n";
				fragmentShader += "uniform mat4 ModelMatrix;																												\n";
				fragmentShader += "uniform vec3 CameraPosition;																												\n";
				fragmentShader += "uniform float LightCount;																												\n";
				fragmentShader += "																																			\n";
				fragmentShader += "in vec3 out_Position;																													\n";
				fragmentShader += "in vec3 out_Normal;																														\n";
				fragmentShader += "in vec4 out_Color;																														\n";
				fragmentShader += "in vec2 out_TexCoordinate;																												\n";
				fragmentShader += "in vec3 out_Additional;																													\n";
				fragmentShader += "																																			\n";
				fragmentShader += "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection, vec3 additional, vec3 specularMapColour)			\n";
				fragmentShader += "{																																		\n";
				fragmentShader += "		float distance = length(light.Position - fragPos);																					\n";
				fragmentShader += "		float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * (distance * distance));								\n";
				fragmentShader += "		vec3 NlightDir = normalize(light.Position - fragPos);																				\n";
				fragmentShader += "		float diff = max(dot(normal, NlightDir), 0.0);																						\n";
				fragmentShader += "		float specularStrength = pow(max(dot(viewDirection, reflect(-NlightDir, normal)), 0.0), 16);										\n";
				fragmentShader += "																																			\n";
				fragmentShader += "		vec3 specularComponent = additional.g * specularStrength * specularMapColour * attenuation * light.Colour;							\n";
				fragmentShader += "		vec3 diffuseComponent = diff * light.Colour * attenuation;																			\n";
				fragmentShader += "		return specularComponent + diffuseComponent;																						\n";
				fragmentShader += "}																																		\n";
				fragmentShader += "																																			\n";
				fragmentShader += "void main()																																\n";
				fragmentShader += "{																																		\n";
				fragmentShader += "	vec3 LightColor = vec3(1.0,1.0,1.0);																									\n";
				fragmentShader += "	vec3 AmbientColor = LightColor;																											\n";
				fragmentShader += "	vec3 NNormal = normalize(out_Normal);																									\n";
				fragmentShader += "																																			\n";
				fragmentShader += "	vec4 TextureColour = texture2D(Texture0, out_TexCoordinate);																			\n";
				fragmentShader += "	vec4 SpecularColour = texture2D(Texture1, out_TexCoordinate);																			\n";
				fragmentShader += "																																			\n";
				fragmentShader += "	vec3 ViewDirection = normalize(CameraPosition - out_Position);																			\n";
				fragmentShader += "																																			\n";
				fragmentShader += "	vec3 lightColour = vec3(0);																																			\n";
				fragmentShader += "	for(int i=0;i<LightCount;i++){				\n";
				fragmentShader += "	lightColour+=CalcPointLight(PointLights[i],NNormal,out_Position,ViewDirection,out_Additional,SpecularColour.rgb);															\n";
				fragmentShader += "	}																																		\n";
				fragmentShader += "																																			\n";
				fragmentShader += "	vec3 AmbientComponent = 0.2 * AmbientColor;																								\n";
				fragmentShader += "	vec3 LightColour = 	((AmbientComponent+lightColour)*max(1-out_Additional.r,0));															\n";
				fragmentShader += "	vec3 result = (LightColour + out_Additional.r) * out_Color.rgb * TextureColour.rgb;														\n";
				fragmentShader += "	gl_FragColor = vec4(result, 1.0);																										\n";
				fragmentShader += "																																			\n";
				fragmentShader += "}																																		\n";

				this->SetGLSL(vertexShader, fragmentShader);
			}
		}
	}
}
