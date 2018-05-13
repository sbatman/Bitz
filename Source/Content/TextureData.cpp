#include "TextureData.h"
#include "ContentManager.h"
#include "../Serialize/Packet.h"
#include "IO.h"

namespace Bitz
{
	namespace Content
	{
		std::atomic<uint32_t> TextureData::_NextID(0);

		std::vector<TextureData_Ptr> TextureData::_LoadedTextureData;

		TextureData::TextureData()
		{
			_ID = _NextID++;			
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

		TextureData_Ptr TextureData::Load(const std::string& fileName)
		{
			int32_t width = 0, height = 0;

			const std::vector<byte> imageData = IO::ReadAllBytes(fileName);
			Serialize::Packet * packet = Serialize::Packet::FromByteArray(imageData);
			std::vector<Bitz::Serialize::Packet::PakObject> objects = packet->GetObjects();
			width = *static_cast<int32_t *>(objects[0].Ptr);
			height = *static_cast<int32_t *>(objects[1].Ptr);
			uint8_t * byteArray = static_cast<uint8_t *>(objects[2].Ptr);

			TextureData_Ptr returnTextureData = std::make_shared<TextureData>();
			_LoadedTextureData.push_back(returnTextureData);
			returnTextureData->_PixelData = new uint8_t[width*height * 4];

			Memcpy(returnTextureData->_PixelData, width*height * 4, byteArray, width*height * 4);

			returnTextureData->_Width = width;
			returnTextureData->_Height = height;

			return returnTextureData;
		}

		uint32_t Content::TextureData::GetOpenglTextureID() const
		{
			return _OpenglTextureID;
		}

		void TextureData::SetOpenglTextureID(const uint32_t newID)
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

		void TextureData::ClearAllOpenGLIDs()
		{
			for (const TextureData_Ptr& tex : _LoadedTextureData)tex->SetOpenglTextureID(-1);
		}
	}
}