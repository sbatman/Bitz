#include "VoxelGrid.h"

VoxelGrid::VoxelGrid(Bitz::Math::Vector3I dimentions)
{
	_VertArrayDirty = true;
	_Dimentions = dimentions;
	_VoxelStore = new Voxel[_Dimentions.X*_Dimentions.Y*_Dimentions.Z];
	for (int x = 0;x < _Dimentions.X;x++)
	{
		for (int y = 0;y < _Dimentions.Y;y++)
		{
			for (int z = 0;z < _Dimentions.Z;z++)
			{
				_VoxelStore[x + (y*_Dimentions.X) + (z*_Dimentions.X*_Dimentions.Y)].Type = -1;
			}
		}
	}
}

VoxelGrid::~VoxelGrid()
{
}

void VoxelGrid::UpdateVertArray()
{
	_VertArrayDirty = false;

	_PopulatedVoxelCount = 0;

	for (int x = 0;x < _Dimentions.X;x++)
	{
		for (int y = 0;y < _Dimentions.Y;y++)
		{
			for (int z = 0;z < _Dimentions.Z;z++)
			{
				if (_VoxelStore[x + (y*_Dimentions.X) + (z*_Dimentions.X*_Dimentions.Y)].Type != -1)_PopulatedVoxelCount++;
			}
		}
	}

	if (_PopulatedVoxelCount == 0)return;
	int verpos = 0;
	float_t * vertArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];
	float_t * normalArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];

	float centerOffsetX = _Dimentions.X*0.5f;
	float centerOffsetY = _Dimentions.Y*0.5f;
	float centerOffsetZ = _Dimentions.Z*0.5f;

	for (int x = 0;x < _Dimentions.X;x++)
	{
		for (int y = 0;y < _Dimentions.Y;y++)
		{
			for (int z = 0;z < _Dimentions.Z;z++)
			{
				if (_VoxelStore[x + (y*_Dimentions.X) + (z*_Dimentions.X*_Dimentions.Y)].Type != -1)
				{

					float_t _tlf[3] = { (x - centerOffsetX) - 0.5f, (y - centerOffsetY) - 0.5f, (z - centerOffsetZ) - 0.5f };
					float_t _trf[3] = { (x - centerOffsetX) + 0.5f, (y - centerOffsetY) - 0.5f, (z - centerOffsetZ) - 0.5f };
					float_t _blf[3] = { (x - centerOffsetX) - 0.5f, (y - centerOffsetY) + 0.5f, (z - centerOffsetZ) - 0.5f };
					float_t _brf[3] = { (x - centerOffsetX) + 0.5f, (y - centerOffsetY) + 0.5f, (z - centerOffsetZ) - 0.5f };
					float_t _tlb[3] = { (x - centerOffsetX) - 0.5f, (y - centerOffsetY) - 0.5f, (z - centerOffsetZ) + 0.5f };
					float_t _trb[3] = { (x - centerOffsetX) + 0.5f, (y - centerOffsetY) - 0.5f, (z - centerOffsetZ) + 0.5f };
					float_t _blb[3] = { (x - centerOffsetX) - 0.5f, (y - centerOffsetY) + 0.5f, (z - centerOffsetZ) + 0.5f };
					float_t _brb[3] = { (x - centerOffsetX) + 0.5f, (y - centerOffsetY) + 0.5f, (z - centerOffsetZ) + 0.5f };

					float normals[9 * 12] = {
						0, 0, -1, 0, 0, -1, 0, 0, -1, //front
						0, 0, -1, 0, 0, -1, 0, 0, -1, //front
						-1, 0, 0, -1, 0, 0, -1, 0, 0, //left
						-1, 0, 0, -1, 0, 0, -1, 0, 0, //left
						1, 0, 0, 1, 0, 0, 1, 0, 0, //right
						1, 0, 0, 1, 0, 0, 1, 0, 0, //right
						0, 0, 1, 0, 0, 1, 0, 0, 1, //back
						0, 0, 1, 0, 0, 1, 0, 0, 1,//back
						0, -1, 0, 0, -1, 0, 0, -1, 0, //top
						0, -1, 0, 0, -1, 0, 0, -1, 0, //top
						0, 1, 0, 0, 1, 0, 0, 1, 0, //bottom
						0, 1, 0, 0, 1, 0, 0, 1, 0 //bottom
					};

					Memcpy(normalArray + verpos, 9 * 12 * sizeof(float_t), normals, 9 * 12 * sizeof(float_t));

					//FRONT
					CopyTri(vertArray, _tlf, _blf, _trf, &verpos);
					CopyTri(vertArray, _blf, _brf, _trf, &verpos);

					//LEFT	
					CopyTri(vertArray, _tlb, _blb, _tlf, &verpos);
					CopyTri(vertArray, _blb, _blf, _tlf, &verpos);

					//RIGHT	
					CopyTri(vertArray, _trf, _brf, _trb, &verpos);
					CopyTri(vertArray, _brf, _brb, _trb, &verpos);

					//Back

					CopyTri(vertArray, _trb, _brb, _tlb, &verpos);
					CopyTri(vertArray, _brb, _blb, _tlb, &verpos);

					//Top	
					CopyTri(vertArray, _tlb, _tlf, _trb, &verpos);
					CopyTri(vertArray, _tlf, _trf, _trb, &verpos);

					//Bottom	
					CopyTri(vertArray, _blf, _blb, _brf, &verpos);
					CopyTri(vertArray, _blb, _brb, _brf, &verpos);

				}
			}
		}
	}

	SetVerts(vertArray, 36 * _PopulatedVoxelCount);
	SetNormals(normalArray, 36 * _PopulatedVoxelCount);
	delete vertArray;
	delete normalArray;

	UpdateTextureArray();
}

void VoxelGrid::UpdateTextureArray()
{
	float_t * texArray = new float_t[6 * 2 * 3 * 2 * _PopulatedVoxelCount];


	float_t tex[12] = { 0,0, 0,1, 1,0, 0,1, 1,1, 1,0 };

	for (int i = 0;i < 6 * _PopulatedVoxelCount;i++)Memcpy(texArray + (i * 12), sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

	SetTexCords(texArray, 36 * _PopulatedVoxelCount);
}

void VoxelGrid::CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition)
{
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p1, sizeof(float_t) * 3);	(*arrayPosition) += 3;
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p2, sizeof(float_t) * 3);	(*arrayPosition) += 3;
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p3, sizeof(float_t) * 3);	(*arrayPosition) += 3;
}

bool VoxelGrid::ShouldDraw()
{
	if (_VertArrayDirty)UpdateVertArray();
	return Model::ShouldDraw();
}

Bitz::Math::Vector3I VoxelGrid::GetDimentions()
{
	return _Dimentions;
}

void VoxelGrid::SetVoxel(Bitz::Math::Vector3I position, Voxel newVoxel)
{
	if (position.X < 0 || position.Y < 0 || position.Z < 0) throw std::exception("Voxel position not valid");
	if (position.X > _Dimentions.X || position.Y > _Dimentions.Y || position.Z > _Dimentions.Z) throw std::exception("Voxel position not valid");

	_VoxelStore[position.X + (position.Y*_Dimentions.X) + (position.Z*_Dimentions.X*_Dimentions.Y)] = newVoxel;

	_VertArrayDirty = true;
}
