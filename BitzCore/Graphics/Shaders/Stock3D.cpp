#include "Stock3D.h"
#include "../BitzCore/Content/IO.h"

namespace Bitz
{
	namespace GFX
	{
		namespace Shaders
		{
			Stock3D::Stock3D()
			{
				std::string vertexShader = "#version 150												\n";
				vertexShader += "precision highp float;												\n";
				vertexShader += "uniform mat4 ProjectionMatrix;											\n";
				vertexShader += "uniform mat4 ViewMatrix;												\n";
				vertexShader += "uniform mat4 ModelMatrix;												\n";
				vertexShader += "																		\n";
				vertexShader += "attribute vec4 in_Position;											\n";
				vertexShader += "attribute vec3 in_Normal;												\n";
				vertexShader += "attribute vec4 in_Color;												\n";				
				vertexShader += "attribute vec2 in_TexCoordinate;										\n";
				vertexShader += "																		\n";
				vertexShader += "out vec3 out_Position;												\n";
				vertexShader += "out vec3 out_Normal;												\n";
				vertexShader += "out vec4 out_Color;												\n";
				vertexShader += "out vec2 out_TexCoordinate;										\n";
				vertexShader += "																		\n";
				vertexShader += "void main()															\n";
				vertexShader += "{																		\n";
				vertexShader += "	mat4 MVMatrix = ViewMatrix*ModelMatrix;								\n";
				vertexShader += "	out_Position = vec3(MVMatrix*in_Position);							\n";
				vertexShader += "	out_Normal = in_Normal;												\n";
				vertexShader += "	out_Color = in_Color;												\n";
				vertexShader += "	out_TexCoordinate = in_TexCoordinate;								\n";
				vertexShader += "	gl_Position = ProjectionMatrix * MVMatrix * in_Position;			\n";
				vertexShader += "}																";

				std::string fragmentShader = "#version 150															\n";

				fragmentShader += "precision highp float;																				\n";
				fragmentShader += "																										\n";
				fragmentShader += "uniform sampler2D Texture0;																			\n";
				fragmentShader += "uniform sampler2D Texture1;																			\n";
				fragmentShader += "																										\n";
				fragmentShader += "in vec3 out_Position;																			\n";
				fragmentShader += "in vec3 out_Normal;																				\n";
				fragmentShader += "in vec4 out_Color;																				\n";
				fragmentShader += "in vec2 out_TexCoordinate;																		\n";
				fragmentShader += "																										\n";
				fragmentShader += "void main()																							\n";
				fragmentShader += "{																									\n";
				fragmentShader += "	vec3 LightDirection = vec3(1.0, 1.0, 1.0);															\n";
				fragmentShader += "	vec4 LightColor = vec4(1.0,1.0,1.0,0.9);															\n";
				fragmentShader += "	vec4 AmbientColor = vec4(1.0,0.0,0.0,0.05);															\n";
				fragmentShader += "	vec3 Normal = 2.0 * texture2D(Texture1, out_TexCoordinate).xyz - 1.0;								\n";
				fragmentShader += "	Normal = (Normal)* out_Normal;																		\n";
				fragmentShader += "	vec3 NormalizedLightDirection = normalize(LightDirection);											\n";
				fragmentShader += "	vec3 NormalizedNormal = normalize(Normal);															\n";
				fragmentShader += "																										\n";
				fragmentShader += "	vec3 Diffuse = (LightColor.rgb * LightColor.a) * max(dot(NormalizedNormal, NormalizedLightDirection), 0.0);		\n";
				fragmentShader += "	vec3 Ambient = AmbientColor.rgb * AmbientColor.a;													\n";
				fragmentShader += "	vec4 TextureColour = texture2D(Texture0, out_TexCoordinate);										\n";
				fragmentShader += "																										\n";
				fragmentShader += "	gl_FragColor =  TextureColour *vec4(Ambient+Diffuse ,1.0);															\n";
				fragmentShader += "}																									\n";


				this->SetGLSL(vertexShader, fragmentShader);
			}
		}
	}
}
