#pragma once
#include "../../Common.h"
#include "../Texture.h"
#include "../../Interfaces/Core.h"

namespace Bitz
{
	namespace GFX
	{
		class GraphicsManager;
		class RenderEngine;
		
		namespace Drawables
		{
			/// <summary>
			/// This interface defines an object that can be rendered by the graphics manager
			/// </summary>
			class IDrawable : 
				public Bitz::Interfaces::IVisible,
				public Bitz::Interfaces::IColourable
			{
			public:
				friend GraphicsManager;
				friend RenderEngine;
				enum RenderMode
				{
					TwoD, ThreeD
				};
				virtual ~IDrawable()
				{
					delete[] _VertArray;
					_VertArray = nullptr;
					delete[] _ColArray;
					_ColArray = nullptr;
					delete[] _TexArray;
					_TexArray = nullptr;
					delete[] _NormArray;
					_NormArray = nullptr;
				}
				virtual void PopulateVertArray(float_t * vertArray, int32_t * startPosition) = 0;
				virtual void PopulateColArray(float_t * colArray, int32_t * startPosition) = 0;
				virtual void PopulateTexArray(float_t * texArray, int32_t * startPosition) = 0;
				virtual void PopulateNormArray(float_t * normArray, int32_t * startPosition) = 0;
				virtual uint32_t GetVertCount() const = 0;
				virtual void SetTexture(const Texture_Ptr newTexture)
				{
					_Texture = newTexture;
					
				}
				virtual Texture_Ptr GetTexture() const
				{
					return _Texture;
				}
				virtual bool ShouldDraw()
				{
					return _Visible && _VertCount > 0;
				}

			protected:
				float_t * _VertArray = nullptr;
				float_t * _ColArray = nullptr;
				float_t * _TexArray = nullptr;
				float_t * _NormArray = nullptr;
				int32_t _VertCount = 0;
				RenderMode _RenderMode;
				Texture_Ptr _Texture = nullptr;
			};
			typedef std::shared_ptr<IDrawable> IDrawable_Ptr;
		}
	}
}