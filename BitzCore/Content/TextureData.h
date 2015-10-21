#pragma once

namespace Bitz
{
	namespace GFX
	{
		class Texture;
		class GraphicsManager;
		class RenderEngine;
	}

	namespace Content
	{
		class TextureData;
		class ContentManager;

		typedef std::shared_ptr<TextureData> TextureData_Ptr;

		class TextureData
		{
			friend GFX::GraphicsManager;
			friend GFX::RenderEngine;
			friend ContentManager;
			friend GFX::Texture;

		public:
			TextureData();
			~TextureData();

			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			uint32_t GetID() const;

		protected:
			static std::vector<TextureData_Ptr> _LoadedTextureData;

			/// <summary>
			/// Loads the PNG at the provided fileName returning a TextureData
			/// </summary>
			/// <param name='fileName'>The filename to load relative to ContentManager::GetGraphicsRoot()</param>
			static TextureData_Ptr Load(const std::string fileName);
			/// <summary>
			/// Resets the OpenGL Id's of all loaded assets, this is usualy triggered when the OpenGL context has gone down
			/// </summary>
			static void ClearAllOpenGLIDs();

			/// <summary>
			/// Gets the texture identifier assigned by opengl
			/// </summary>
			/// <returns>The int returned by GenTextures or -1 if this texture is not registered</returns>
			uint32_t GetOpenglTextureID() const;
			void SetOpenglTextureID(uint32_t newID);

			void IncrementUsageCount();
			void DecrementUsageCount();

			uint32_t _OpenglTextureID = -1;

		private:
			uint8_t * _PixelData;
			uint32_t _Width = 0;
			uint32_t _Height = 0;
			uint32_t _ID = -1;
			uint32_t _UsageCount = 0;

			static std::atomic<uint32_t> _NextID;
		};
	}
}
