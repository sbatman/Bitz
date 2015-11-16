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
			RenderEngine(Window * window);
			~RenderEngine();

			void Init();
			void Update();
			void Begin(Shaders::Shader_Ptr activeShader = nullptr);
			void End();
			void Present();
			void Clear(Vector3F colour);
			Window * GetWindow();
			void Render(Drawables::IDrawable* idrawable);
			void SetSize(Vector2I newSize);
			Vector2I GetSize() const;
			void EnableNormals(bool enabled);
			bool IsEnabledNormals() const;
			bool IsEnabledTexturing() const;

			const uint32_t BUFFERVERTCOUNT = 100000;

		private:
			/// <summary>
			/// This struct outligns the limits of each call to draw eletments without chaning the render state, information on
			/// how the render state is calculated during render calls and populated into a list for the render sequence
			/// </summary>
			struct DrawInterval
			{
				uint32_t VertCountStart = 0;
				uint32_t VertCountEnd = 0;
				Content::TextureData_Ptr Texture = nullptr;
				Drawables::IDrawable::RenderMode Mode;
				Drawables::IDrawable * InitialDrawable;

				DrawInterval(uint32_t start, uint32_t end, Content::TextureData_Ptr texture, Drawables::IDrawable::RenderMode mode, Drawables::IDrawable * drawable)
				{
					VertCountStart = start;
					VertCountEnd = end;
					Texture = texture;
					Mode = mode;
					InitialDrawable = drawable;
				}

				~DrawInterval()
				{
					Texture = nullptr;
					InitialDrawable = nullptr;
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
			/// The current Context that is being used for rendering
			/// </summary>
			GLContext * _CurrentRenderingContext;
			/// <summary>
			/// The current window used for rendering
			/// </summary>
			Window * _CurrentWindow;
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
			Content::TextureData_Ptr _ActiveTexture;
			/// <summary>
			/// Whether rendering with a texture is currently enabled
			/// </summary>
			bool _TexturingEnabled;
			/// <summary>
			/// Whether rendering with normal corrdinates is currently enabled
			/// </summary>
			bool _NormalsEnabled;

			/// <summary>
			/// Sets the active texture used for rendering
			/// </summary>
			void SetActiveTexture(Content::TextureData_Ptr activeTexture);
		};
	}
}