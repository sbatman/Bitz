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
				vertexShader += "out vec3 out_Position;													\n";
				vertexShader += "out vec3 out_Normal;													\n";
				vertexShader += "out vec4 out_Color;													\n";
				vertexShader += "out vec2 out_TexCoordinate;											\n";
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

				fragmentShader += "precision highp float;																								\n";
				fragmentShader += "																														\n";
				fragmentShader += "uniform sampler2D Texture0;		//diffuse																			\n";
				fragmentShader += "uniform sampler2D Texture1;		//specular																			\n";
				fragmentShader += "uniform vec3 LightDirection;																							\n";
				fragmentShader += "uniform mat4 ModelMatrix;																							\n";
				fragmentShader += "																														\n";
				fragmentShader += "in vec3 out_Position;																								\n";
				fragmentShader += "in vec3 out_Normal;																									\n";
				fragmentShader += "in vec4 out_Color;																									\n";
				fragmentShader += "in vec2 out_TexCoordinate;																							\n";
				fragmentShader += "																														\n";
				fragmentShader += "void main()																											\n";
				fragmentShader += "{																													\n";
				fragmentShader += "	vec3 specular = vec3(0.0);																							\n";
				fragmentShader += "	vec4 LightColor = vec4(1.0,1.0,1.0,0.4);																			\n";
				fragmentShader += "	vec4 AmbientColor = vec4(1.0,1.0,1.0,0.1);																			\n";
				fragmentShader += "	mat3 NormalMatrix = transpose(inverse(mat3(ModelMatrix)));															\n";
				fragmentShader += "																														\n";
				fragmentShader += "	vec4 TextureColour = texture2D(Texture0, out_TexCoordinate);														\n";
				fragmentShader += "	vec4 SpecularColour = texture2D(Texture1, out_TexCoordinate);														\n";
				fragmentShader += "	vec3 NormalizedLightDirection = normalize(LightDirection);															\n";
				fragmentShader += "	vec3 NormalizedNormal = normalize(NormalMatrix * out_Normal);														\n";
				fragmentShader += "	vec3 ReflectingVector = reflect(-NormalizedLightDirection , NormalizedNormal);										\n";
				fragmentShader += "																														\n";
				fragmentShader += "	vec3 Diffuse = (LightColor.rgb * LightColor.a) * max(dot(NormalizedNormal, NormalizedLightDirection), 0.0);			\n";
				fragmentShader += "	vec3 Ambient = AmbientColor.rgb * AmbientColor.a;																	\n";
				fragmentShader += "	if(max(dot(NormalizedLightDirection,NormalizedNormal),0.0)>0.0)														\n";
				fragmentShader += "		specular = vec3(4)* SpecularColour.rgb * pow(max(dot(ReflectingVector,normalize(-out_Position)),0.0),200);		\n";
				fragmentShader += "																														\n";
				fragmentShader += "	vec4 gamma = vec4(1.0 / 2.2,1.0 / 2.2,1.0 / 2.2,1.0);																\n";
				fragmentShader += "	vec4 resultColor = pow(TextureColour, gamma)* vec4( Diffuse+Ambient+specular ,1.0)*out_Color;						\n";
				fragmentShader += "																														\n";
				fragmentShader += "	gl_FragColor = resultColor;																							\n";
				fragmentShader += "}																													\n";

				this->SetGLSL(vertexShader, fragmentShader);
			}
		}
	}
}
