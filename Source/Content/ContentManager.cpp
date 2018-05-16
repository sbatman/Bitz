#include "../Common.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "../Graphics/Texture.h"
#include <algorithm>
#include "MagicaVoxelData.h"

namespace Bitz
{
	namespace Content
	{
		std::string ContentManager::_GraphicsRoot;
		std::string ContentManager::_AudioRoot;
		std::string ContentManager::_DataRoot;
		GFX::Texture_Ptr ContentManager::_WhiteTexture;
		GFX::Texture_Ptr ContentManager::_BlackTexture;
		bool ContentManager::_HasInit = false;

		void ContentManager::GenerateWhiteTexture()
		{
			const TextureData_Ptr whiteTextureData = std::make_shared<TextureData>();
			whiteTextureData->_Width = 1;
			whiteTextureData->_Height = 1;
			whiteTextureData->_PixelData = new uint8_t[4]{ 255,255,255,255 };
			_WhiteTexture = std::make_shared<GFX::Texture>();
			_WhiteTexture->_Data = whiteTextureData;
			_WhiteTexture->_UV_TL = Vector2S(0, 0);
			_WhiteTexture->_UV_TR = Vector2S(0, 1);
			_WhiteTexture->_UV_BL = Vector2S(1, 0);
			_WhiteTexture->_UV_BR = Vector2S(1, 1);
		}

		void ContentManager::GenerateBlackTexture()
		{
			const TextureData_Ptr blackTextureData = std::make_shared<TextureData>();
			blackTextureData->_Width = 1;
			blackTextureData->_Height = 1;
			blackTextureData->_PixelData = new uint8_t[4]{ 0,0,0,255 };
			_BlackTexture = std::make_shared<GFX::Texture>();
			_BlackTexture->_Data = blackTextureData;
			_BlackTexture->_UV_TL = Vector2S(0, 0);
			_BlackTexture->_UV_TR = Vector2S(0, 1);
			_BlackTexture->_UV_BL = Vector2S(1, 0);
			_BlackTexture->_UV_BR = Vector2S(1, 1);
		}

		void ContentManager::Init(std::string graphicsRoot, std::string audioRoot, std::string dataRoot)
		{
			assert(!_HasInit);
			assert(graphicsRoot.length() != 0);
			assert(audioRoot.length() != 0);
			assert(dataRoot.length() != 0);

			_GraphicsRoot = graphicsRoot;
			_AudioRoot = audioRoot;
			_DataRoot = dataRoot;
			_HasInit = true;

			GenerateWhiteTexture();
			GenerateBlackTexture();
		}

		void ContentManager::UnloadUnused()
		{
			assert(_HasInit);
			TextureData::_LoadedTextureData.erase(
				std::remove_if(TextureData::_LoadedTextureData.begin(), TextureData::_LoadedTextureData.end(),
					[](TextureData_Ptr element) -> bool
			{
				return element->_UsageCount == 0;
			}
			), TextureData::_LoadedTextureData.end());
			_HasInit = false;
		}

		std::string ContentManager::GetGraphicsRoot()
		{
			return _GraphicsRoot;
		}

		std::string ContentManager::GetAudioRoot()
		{
			return _AudioRoot;
		}
		
		std::string ContentManager::GetDataRoot()
		{
			return _DataRoot;
		}

		GFX::Texture_Ptr ContentManager::GetWhiteTexture()
		{
			return _WhiteTexture;
		}

		GFX::Texture_Ptr ContentManager::GetBlackTexture()
		{
			return _BlackTexture;
		}

		template <class T>
		std::shared_ptr<T> ContentManager::Load(std::string fileName)
		{
			assert(_HasInit && L"The content manager must have its init method called before it attempts to load content");
			auto func = T::Load;
			return func(fileName);
		}

		template GFX::Texture_Ptr ContentManager::Load<GFX::Texture>(std::string fileName);
		template MagicVoxelData_Ptr ContentManager::Load<MagicVoxelData>(std::string fileName);
	}
}
