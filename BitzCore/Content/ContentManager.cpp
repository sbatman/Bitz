#include "../Common.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "../Graphics/Texture.h"
#include <algorithm>

namespace Bitz
{
	namespace Content
	{
		std::string ContentManager::_GraphicsRoot;
		std::string ContentManager::_AudioRoot;
		std::string ContentManager::_DataRoot;
		bool ContentManager::_HasInit = false;

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
		}

		void ContentManager::UnloadUnused()
		{
			assert(_HasInit);
			TextureData::_LoadedTextureData.erase(
				std::remove_if(TextureData::_LoadedTextureData.begin(), TextureData::_LoadedTextureData.end(),
					[](TextureData_Ptr element) -> bool
			{
				if (element->_UsageCount == 0)
				{
					return true;
				}
				return false;
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

		template <class T>
		std::shared_ptr<T> ContentManager::Load(std::string fileName)
		{
			assert(_HasInit && L"The content manager must have its init method called before it attempts to load content");
			auto func = T::Load;
			return func(fileName);
		}

		template GFX::Texture_Ptr ContentManager::Load<GFX::Texture>(std::string fileName);
	}
}