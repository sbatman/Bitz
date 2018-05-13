#pragma once
#include "TextureData.h"
#include "../Graphics/Texture.h"

namespace Bitz
{
	namespace Content
	{
		class ContentManager
		{
		public:			
			static void Init(std::string graphicsRoot, std::string audioRoot, std::string dataRoot);
			static void UnloadUnused();
			static std::string GetGraphicsRoot();
			static std::string GetAudioRoot();
			static std::string GetDataRoot();
			static GFX::Texture_Ptr GetWhiteTexture();
			static GFX::Texture_Ptr GetBlackTexture();

			template <class T>
			static std::shared_ptr<T> Load(std::string fileName);

		private:
			static void GenerateWhiteTexture();
			static void GenerateBlackTexture();
			static std::string _GraphicsRoot;
			static std::string _AudioRoot;
			static std::string _DataRoot;
			static bool _HasInit;
			static GFX::Texture_Ptr _WhiteTexture;
			static GFX::Texture_Ptr _BlackTexture;
		};
	}
}
