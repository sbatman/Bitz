#include "../Common.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "../Graphics/Texture.h"
#include <algorithm>

namespace Bitz
{
	namespace Content
	{
		std::wstring ContentManager::_GraphicsRoot;
		std::wstring ContentManager::_AudioRoot;
		std::wstring ContentManager::_DataRoot;
		bool ContentManager::_HasInit = false;

		void ContentManager::Init(std::wstring graphicsRoot, std::wstring audioRoot, std::wstring dataRoot)
		{
			_ASSERT(!_HasInit);
			_ASSERT(graphicsRoot.length() != 0);
			_ASSERT(audioRoot.length() != 0);
			_ASSERT(dataRoot.length() != 0);

			_GraphicsRoot = graphicsRoot;
			_AudioRoot = audioRoot;
			_DataRoot = dataRoot;
			_HasInit = true;
		}

		void ContentManager::UnloadUnused()
		{
			_ASSERT(_HasInit);
			TextureData::_LoadedTextureData.erase(
				std::remove_if(TextureData::_LoadedTextureData.begin(), TextureData::_LoadedTextureData.end(),
					[](TextureData* element) -> bool
			{
				if (element->_UsageCount == 0)
				{
					delete element;
					return true;
				}
				return false;
			}
			), TextureData::_LoadedTextureData.end());
			_HasInit = false;
		}

		std::wstring ContentManager::GetGraphicsRoot()
		{
			return _GraphicsRoot;
		}

		std::wstring ContentManager::GetAudioRoot()
		{
			return _AudioRoot;
		}

		std::wstring ContentManager::GetDataRoot()
		{
			return _DataRoot;
		}

		template <class T>
		T* ContentManager::Load(std::wstring fileName)
		{
			_ASSERT_EXPR(_HasInit, L"The content manager must have its init method called before it attempts to load content");
			auto func = T::Load;
			return func(fileName);
		}

		template GFX::Texture* ContentManager::Load<GFX::Texture>(std::wstring fileName);

	}
}