#pragma once
#pragma once
#include "../Common.h"
#include "../Interfaces/Core.h"

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
		class MagicVoxelData;

		typedef std::shared_ptr<MagicVoxelData> MagicVoxelData_Ptr;
		
		class MagicVoxelData
		{
			struct Chunk
			{
				char ChunkID[4];
				int32_t ChunkContentsSize;
				int32_t ChunkChildrenSize;
				std::vector<std::shared_ptr<Chunk>> Children;

				virtual void GetData(const std::vector<byte> data, int& currentPos) = 0;
			};

			struct MainChunk : Chunk
			{
				void GetData(const std::vector<byte> data, int& currentPos) override
				{
				}
			};

			struct SizeChunk : Chunk
			{
				int32_t SizeX;
				int32_t SizeY;
				int32_t SizeZ;

				void GetData(const std::vector<byte> data, int& currentPos) override
				{
					//SizeX
					byte temp[4];
					std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
					currentPos += 4;
					SizeX = Bitz::Content::MagicVoxelData::ByteToInt(temp);
					//SizeY
					std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
					currentPos += 4;
					SizeY = Bitz::Content::MagicVoxelData::ByteToInt(temp);
					//SizeZ
					std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
					currentPos += 4;
					SizeZ = Bitz::Content::MagicVoxelData::ByteToInt(temp);
				}
			};

			struct XYZIChunk : Chunk
			{
				struct Voxel
				{
					byte X;
					byte Y;
					byte Z;
					byte CI;
				};
				int32_t NumberOfVoxels;
				std::vector<Voxel> Voxels;

				void GetData(const std::vector<byte> data, int& currentPos) override
				{
					//SizeX
					byte temp[4];
					std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
					currentPos += 4;
					NumberOfVoxels = Bitz::Content::MagicVoxelData::ByteToInt(temp);

					for (int i = 0; i<NumberOfVoxels; i++)
					{
						Voxel vox = Voxel();
						vox.X = data[currentPos++];
						vox.Y = data[currentPos++];
						vox.Z = data[currentPos++];
						vox.CI = data[currentPos++];
						Voxels.push_back(vox);
					}
				}

			};

			struct RGBAChunk : Chunk
			{
				struct Colour
				{
					byte R;
					byte G;
					byte B;
					byte A;
				};
				std::vector<Colour> Colours;

				void GetData(const std::vector<byte> data, int& currentPos) override
				{
					for (int i = 0; i < 256; i++)
					{
						Colour colour = Colour();
						colour.R = data[currentPos++];
						colour.G = data[currentPos++];
						colour.B = data[currentPos++];
						colour.A = data[currentPos++];
						Colours.push_back(colour);
					}
				}
			};

			friend GFX::GraphicsManager;
			friend GFX::RenderEngine;
			friend ContentManager;
			friend GFX::Texture;

		public:
			MagicVoxelData();

			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			uint32_t GetDepth() const;
			std::vector<Vector4B> GetColourPallet() const;
			std::vector<Vector4B> GetVoxelData() const;

		protected:
			static std::shared_ptr<Chunk> ReadChunk(std::vector<byte> data, int& currentPos);
			static MagicVoxelData_Ptr Load(const std::string& fileName);

		private:
			int _VersionNumber;
			std::shared_ptr<MainChunk> _RootChunk;

			static int32_t ByteToInt(const byte buffer[])
			{
				return int(static_cast<unsigned char>(buffer[3]) << 24 |
					static_cast<unsigned char>(buffer[2]) << 16 |
					static_cast<unsigned char>(buffer[1]) << 8 |
					static_cast<unsigned char>(buffer[0]));
			}
		};
		
	}
}
