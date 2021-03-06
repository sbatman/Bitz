#pragma once
#include "../Common.h"
#include "GLContext.h"
#include "Texture.h"
#include "Window.h"
#include "Drawables\IDrawable.h"
#include "Shaders\Shader.h"

namespace Bitz
{
	namespace GFX
	{
		class RenderEngine
		{
		public:
			RenderEngine(Window_Ptr window);
			~RenderEngine();

			void Init();
			void Update();
			void Begin(Shaders::Shader_Ptr activeShader = nullptr);
			void End();
			void Present();
			void Clear(Vector3F colour);
			Window_Ptr GetWindow();
			void Render(Drawables::IDrawable_Ptr idrawable);
			void SetSize(Vector2I newSize);
			void GLErrorCheck();
			Vector2I GetSize() const;

			const uint32_t BUFFERVERTCOUNT = 1000000;
			const uint16_t MAXTEXTUREUNITS = GL_MAX_TEXTURE_UNITS - GL_TEXTURE0;

		private:
			/// <summary>
			/// This struct outligns the limits of each call to draw eletments without chaning the render state, information on
			/// how the render state is calculated during render calls and populated into a list for the render sequence
			/// </summary>
			struct DrawInterval
			{
				uint32_t VertCountStart = 0;
				uint32_t VertCountEnd = 0;
				std::vector<Content::TextureData_Ptr> Texture;
				Drawables::IDrawable::RenderMode Mode;
				Shaders::Shader_Ptr CustomShader = nullptr;
				glm::mat4 Matrix;

				DrawInterval() = default;

				DrawInterval(
					const uint32_t start,
					const uint32_t end,
					const Content::TextureData_Ptr& texture,
					const Drawables::IDrawable::RenderMode mode,
					const Shaders::Shader_Ptr customShader = nullptr,
					const glm::mat4 matrix = glm::mat4(),
					const Content::TextureData_Ptr specularTexture = nullptr)
				{
					VertCountStart = start;
					VertCountEnd = end;
					Texture.push_back(texture);
					Texture.push_back(specularTexture);
					Mode = mode;
					CustomShader = customShader;
					Matrix = matrix;
				}
			};

			/// <summary>
			/// The array used for storing vertex positional information before its sent to the card
			/// </summary>
			float_t * _VertCache;
			/// <summary>
			/// The position along the vertex positional array that has been populated this draw
			/// </summary>
			int32_t _VertCachePos;
			/// <summary>
			/// The array used for storing vertex colour information before its sent to the card
			/// </summary>
			float_t * _ColCache;
			/// <summary>
			/// The position along the vertex colour array that has been populated this draw
			/// </summary>
			int32_t _ColCachePos;
			/// <summary>
			/// The array used for storing vertex texture coordinates before its sent to the card
			/// </summary>
			float_t * _TexCache;
			/// <summary>
			/// The position along the vertex tex array that has been populated this draw
			/// </summary>
			int32_t _TexCachePos;
			/// <summary>
			/// The array used for storing vertex normal coordinates before its sent to the card
			/// </summary>
			float_t * _NormCache;
			/// <summary>
			/// The position along the vertex normal array that has been populated this draw
			/// </summary>
			int32_t _NormCachePos;
			/// <summary>
			/// The array used for storing additional data before its sent to the card
			/// </summary>
			float_t * _AdditionalCache;
			/// <summary>
			/// The position along the additional data that has been populated this draw
			/// </summary>
			int32_t _AdditionalCachePos;

			std::vector<glm::mat4> _MatCache;

			/// <summary>
			/// The current Context that is being used for rendering
			/// </summary>
			GLContext_Ptr _CurrentRenderingContext;
			/// <summary>
			/// The current window used for rendering
			/// </summary>
			Window_Ptr _CurrentWindow;
			/// <summary>
			/// The currently active shader used for this pass of rendering
			/// </summary>
			Shaders::Shader_Ptr _ActiveShader;
			/// <summary>
			/// The draw intervals that will be used for this draw pass
			/// </summary>
			std::vector<DrawInterval> _DrawIntervals;
			/// <summary>
			/// The current number of verts that will be drawn when this render batch completes
			/// </summary>
			uint32_t _RenderedVertCount;
			/// <summary>
			/// The currently active texture
			/// </summary>
			Content::TextureData_Ptr _ActiveTexture[GL_MAX_TEXTURE_UNITS - GL_TEXTURE0];

			int _VertGLCacheLoc = -1;
			int _ColGLCacheLoc = -1;
			int _NormGLCacheLoc = -1;
			int _TexGLCacheLoc = -1;
			int _AdditionalGLCacheLoc = -1;


			/// <summary>
			/// Sets the active texture used for rendering
			/// </summary>
			void SetActiveTexture(Content::TextureData_Ptr activeTexture, int textureUnitID = 0);
		};
	}
}