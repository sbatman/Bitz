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
				std::string vertexShader = "#version 150									\n";
				vertexShader += "uniform mat4 ProjectionMatrix;								\n";
				vertexShader += "uniform mat4 ViewMatrix;									\n";
				vertexShader += "															\n";
				vertexShader += "attribute vec4 in_Position;								\n";
				vertexShader += "attribute vec3 in_Normal;									\n";
				vertexShader += "attribute vec4 in_Color;									\n";				
				vertexShader += "attribute vec2 in_TexCoordinate;							\n";
				vertexShader += "															\n";
				vertexShader += "varying vec3 out_Position;									\n";
				vertexShader += "varying vec3 out_Normal;									\n";
				vertexShader += "varying vec4 out_Color;									\n";
				vertexShader += "varying vec2 out_TexCoordinate;							\n";
				vertexShader += "															\n";
				vertexShader += "void main()												\n";
				vertexShader += "{															\n";
				vertexShader += "	out_Position = vec3(ViewMatrix*in_Position);			\n";
				vertexShader += "	out_Normal = in_Normal;									\n";
				vertexShader += "	out_Color = in_Color;									\n";
				vertexShader += "	out_TexCoordinate = in_TexCoordinate;					\n";
				vertexShader += "	gl_Position = ProjectionMatrix* ViewMatrix*in_Position;	\n";
				vertexShader += "}																";

				std::string fragmentShader = "#version 150															\n";
				fragmentShader += "precision mediump float;															\n";
				fragmentShader += "																					\n";
				fragmentShader += "uniform sampler2D Texture;														\n";
				fragmentShader += "																					\n";
				fragmentShader += "varying vec3 out_Position;														\n";
				fragmentShader += "varying vec3 out_Normal;															\n";
				fragmentShader += "varying vec4 out_Color;															\n";
				fragmentShader += "varying vec2 out_TexCoordinate;													\n";
				fragmentShader += "																					\n";
				fragmentShader += "void main()																		\n";
				fragmentShader += "{																				\n";
				fragmentShader += " gl_FragColor = texture2D( Texture,out_TexCoordinate) * out_Color;				\n";
				fragmentShader += "}																				";

				this->SetGLSL(vertexShader, fragmentShader);
			}
		}
	}
}
