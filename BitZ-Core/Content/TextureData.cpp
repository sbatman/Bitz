#include "../Common.h"
#include "TextureData.h"
#include "ContentManager.h"
//#include "libs/lodepng/lodepng.h"

namespace Bitz
{
	namespace Content
	{
		uint32_t TextureData::_NextID = 0;

		std::vector<TextureData *> TextureData::_LoadedTextureData;

		TextureData::TextureData()
		{
			_ID = _NextID++;
			_LoadedTextureData.push_back(this);
		}

		TextureData::~TextureData()
		{
			delete[] _PixelData;
		}

		uint32_t TextureData::GetWidth() const
		{
			return _Width;
		}

		uint32_t TextureData::GetHeight() const
		{
			return _Height;
		}

		uint32_t TextureData::GetID() const
		{
			return _ID;
		}

		TextureData * TextureData::Load(const std::string fileName)
		{
			std::vector<uint8_t> png;
			std::vector<uint8_t> image; //the raw pixels
			unsigned width = 0, height = 0;

			//load and decode
			//lodepng::load_file(png, ContentManager::GetGraphicsRoot() + L"\\" + fileName);
		//	unsigned error = lodepng::decode(image, width, height, png);
			//if (error != 0) throw std::exception("Error loading PNG");

			TextureData * returnTextureData = new TextureData();

			returnTextureData->_PixelData = new uint8_t[width*height * 4];

#ifdef __ANDROID__
			memcpy(returnTextureData->_PixelData, &image[0], width*height * 4);
#elif WIN32
			memcpy_s(returnTextureData->_PixelData, width*height * 4, &image[0], width*height * 4);
#endif
			returnTextureData->_Width = width;
			returnTextureData->_Height = height;

			return returnTextureData;
		}

		uint32_t Content::TextureData::GetOpenglTextureID() const
		{
			return _OpenglTextureID;
		}

		void TextureData::SetOpenglTextureID(uint32_t newID)
		{
			_OpenglTextureID = newID;
		}

		void TextureData::IncrementUsageCount()
		{
			_UsageCount++;
		}

		void TextureData::DecrementUsageCount()
		{
			_UsageCount--;
			assert(_UsageCount >= 0);
		}
	}
}