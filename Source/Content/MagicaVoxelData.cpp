#include "MagicaVoxelData.h"
#include "IO.h"
#include "../Graphics/Drawables/VoxelGrid.h"
#include "ContentManager.h"
#include <valarray>

Bitz::Content::MagicVoxelData::MagicVoxelData()
{

}

uint32_t Bitz::Content::MagicVoxelData::GetWidth() const
{
	for(const std::shared_ptr<Chunk> c : _RootChunk->Children)
	{
		if(std::string(c->ChunkID, c->ChunkID + 4)=="SIZE")
		{
			return std::static_pointer_cast<SizeChunk>(c)->SizeX;
		}
	}

	return 0;
}

uint32_t Bitz::Content::MagicVoxelData::GetHeight() const
{
	for (const std::shared_ptr<Chunk> c : _RootChunk->Children)
	{
		if (std::string(c->ChunkID, c->ChunkID + 4) == "SIZE")
		{
			return std::static_pointer_cast<SizeChunk>(c)->SizeZ;
		}
	}
	return 0;
}

uint32_t Bitz::Content::MagicVoxelData::GetDepth() const
{
	for (const std::shared_ptr<Chunk> c : _RootChunk->Children)
	{
		if (std::string(c->ChunkID, c->ChunkID + 4) == "SIZE")
		{
			return std::static_pointer_cast<SizeChunk>(c)->SizeY;
		}
	}
	return 0;
}

std::vector<Bitz::Vector4B> Bitz::Content::MagicVoxelData::GetColourPallet() const
{
	for (const std::shared_ptr<Chunk> c : _RootChunk->Children)
	{
		if (std::string(c->ChunkID, c->ChunkID + 4) == "RGBA")
		{
			std::shared_ptr<RGBAChunk> Chunk = std::static_pointer_cast<RGBAChunk>(c);
			std::vector<Vector4B> returnVector = std::vector<Vector4B>();
			for (int i = 0; i<256; i++)
			{
				Vector4B vec = Vector4B{ Chunk->Colours[i].R ,Chunk->Colours[i].G ,Chunk->Colours[i].B ,Chunk->Colours[i].A };
				returnVector.push_back(vec);
			}
			return returnVector;
		}
	}
	return std::vector<Vector4B>();
}

std::vector<Bitz::Vector4B> Bitz::Content::MagicVoxelData::GetVoxelData() const
{
	for (const std::shared_ptr<Chunk> c : _RootChunk->Children)
	{
		if (std::string(c->ChunkID, c->ChunkID + 4) == "XYZI")
		{
			std::shared_ptr<XYZIChunk> Chunk = std::static_pointer_cast<XYZIChunk>(c);
			std::vector<Vector4B> returnVector = std::vector<Vector4B>();
			for (int i = 0; i<Chunk->NumberOfVoxels; i++)
			{
				Vector4B vec = Vector4B{ Chunk->Voxels[i].X ,Chunk->Voxels[i].Y ,Chunk->Voxels[i].Z ,Chunk->Voxels[i].CI };
				returnVector.push_back(vec);
			}
			return returnVector;
		}
	}
	return std::vector<Vector4B>();
}


std::shared_ptr<Bitz::Content::MagicVoxelData::Chunk> Bitz::Content::MagicVoxelData::ReadChunk(const std::vector<byte> data, int& currentPos)
{
	int startPos = currentPos;
	//chunk ID
	char chunkID[4];
	std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(chunkID), chunkID);
	currentPos += 4;
	std::shared_ptr<Chunk> currentChunk;

	if (std::string(chunkID, chunkID + 4) == "MAIN")currentChunk = std::make_shared<MainChunk>();
	if (std::string(chunkID, chunkID + 4) == "SIZE")currentChunk = std::make_shared<SizeChunk>();
	if (std::string(chunkID, chunkID + 4) == "XYZI")currentChunk = std::make_shared<XYZIChunk>();
	if (std::string(chunkID, chunkID + 4) == "RGBA")currentChunk = std::make_shared<RGBAChunk>();

	std::copy(std::begin(chunkID), std::begin(chunkID) + sizeof(chunkID), (currentChunk->ChunkID));

	//ChunkContentsSize
	byte temp[4];
	std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
	currentPos += 4;
	currentChunk->ChunkContentsSize = Bitz::Content::MagicVoxelData::ByteToInt(temp);

	//ChunkContentsSize
	std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(temp), temp);
	currentPos += 4;
	currentChunk->ChunkChildrenSize = Bitz::Content::MagicVoxelData::ByteToInt(temp);

	currentChunk->GetData(data, currentPos);
	
	while (currentPos < (startPos+4+4+4+ currentChunk->ChunkContentsSize+ currentChunk->ChunkChildrenSize))
	{
		currentChunk->Children.push_back(ReadChunk(data, currentPos));
	}
	return currentChunk;
}

Bitz::Content::MagicVoxelData_Ptr Bitz::Content::MagicVoxelData::Load(const std::string& fileName)
{
	const std::vector<byte> data = IO::ReadAllBytes(Bitz::Content::ContentManager::GetGraphicsRoot() + fileName);

	int currentPos = 0;
	const int endPos = data.size();

	MagicVoxelData_Ptr returnPtr = std::make_shared<MagicVoxelData>();

	//Header
	byte magicNumber[4];
	byte versionNumber[4];
	std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(magicNumber), magicNumber);
	currentPos += 4;
	std::copy(begin(data) + currentPos, begin(data) + currentPos + sizeof(versionNumber), versionNumber);
	currentPos += 4;

	returnPtr->_VersionNumber = *reinterpret_cast<int*>(versionNumber);
	returnPtr->_RootChunk = std::static_pointer_cast<MainChunk>(ReadChunk(data, currentPos));

	return returnPtr;
}
