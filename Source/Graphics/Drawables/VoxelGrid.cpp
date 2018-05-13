#include "VoxelGrid.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			VoxelGrid::VoxelGrid(const Vector3I dimentions)
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
							_VoxelStore[x + y*_Dimentions.X + z*_Dimentions.X*_Dimentions.Y].Type = -1;
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
							if (_VoxelStore[x + y*_Dimentions.X + z*_Dimentions.X*_Dimentions.Y].Type != -1)_PopulatedVoxelCount++;
						}
					}
				}

				if (_PopulatedVoxelCount == 0)return;
				int verpos = 0;
				float_t * vertArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];
				float_t * normalArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];
				float_t * texArray = new float_t[6 * 2 * 3 * 2 * _PopulatedVoxelCount];
				float_t * colArray = new float_t[6 * 2 * 3 * 4 * _PopulatedVoxelCount];

				float_t tex[12] = { 0,0, 0,1, 1,0, 0,1, 1,1, 1,0 };

				float centerOffsetX = _Dimentions.X*0.5f;
				float centerOffsetY = _Dimentions.Y*0.5f;
				float centerOffsetZ = _Dimentions.Z*0.5f;

				int32_t facesDrawn = 0;

				for (int x = 0;x < _Dimentions.X;x++)
				{
					for (int y = 0;y < _Dimentions.Y;y++)
					{
						for (int z = 0;z < _Dimentions.Z;z++)
						{
							Voxel targetVoxel = GetVoxelAtPosition(x, y, z);
							if (targetVoxel.Type != -1)
							{
								float_t _tlf[3] = { x - centerOffsetX - 0.5f, y - centerOffsetY - 0.5f, z - centerOffsetZ - 0.5f };
								float_t _trf[3] = { x - centerOffsetX + 0.5f, y - centerOffsetY - 0.5f, z - centerOffsetZ - 0.5f };
								float_t _blf[3] = { x - centerOffsetX - 0.5f, y - centerOffsetY + 0.5f, z - centerOffsetZ - 0.5f };
								float_t _brf[3] = { x - centerOffsetX + 0.5f, y - centerOffsetY + 0.5f, z - centerOffsetZ - 0.5f };
								float_t _tlb[3] = { x - centerOffsetX - 0.5f, y - centerOffsetY - 0.5f, z - centerOffsetZ + 0.5f };
								float_t _trb[3] = { x - centerOffsetX + 0.5f, y - centerOffsetY - 0.5f, z - centerOffsetZ + 0.5f };
								float_t _blb[3] = { x - centerOffsetX - 0.5f, y - centerOffsetY + 0.5f, z - centerOffsetZ + 0.5f };
								float_t _brb[3] = { x - centerOffsetX + 0.5f, y - centerOffsetY + 0.5f, z - centerOffsetZ + 0.5f };

								float_t colors[12] =
								{
									targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f,
									targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f,
									targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f
								};

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

								if (GetVoxelAtPosition(x, y, z - 1).Type == -1)
								{
									//FRONT
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), normals, 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[facesDrawn * 24], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _tlf, _blf, _trf, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _blf, _brf, _trf, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

									facesDrawn++;
								}

								if (GetVoxelAtPosition(x - 1, y, z).Type == -1)
								{
									//LEFT	
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[2 * 9], 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[facesDrawn * 24], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _tlb, _blb, _tlf, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _blb, _blf, _tlf, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);
									facesDrawn++;
								}

								if (GetVoxelAtPosition(x + 1, y, z).Type == -1)
								{
									//RIGHT	
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[4 * 9], 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _trf, _brf, _trb, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _brf, _brb, _trb, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);
									facesDrawn++;
								}

								if (GetVoxelAtPosition(x, y, z + 1).Type == -1)
								{
									//Back
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[6 * 9], 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _trb, _brb, _tlb, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _brb, _blb, _tlb, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

									facesDrawn++;
								}

								if (GetVoxelAtPosition(x, y - 1, z).Type == -1)
								{
									//Top	
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[8 * 9], 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _tlb, _tlf, _trb, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _tlf, _trf, _trb, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

									facesDrawn++;
								}

								if (GetVoxelAtPosition(x, y + 1, z).Type == -1)
								{
									//Bottom	
									Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[10 * 9], 9 * 2 * sizeof(float_t));
									Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _blf, _blb, _brf, &verpos);
									Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
									CopyTri(vertArray, _blb, _brb, _brf, &verpos);
									Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

									facesDrawn++;
								}

							}
						}
					}
				}

				SetVertArray(vertArray, facesDrawn * 6);
				SetNormalArray(normalArray, facesDrawn * 6);
				SetTexCordArray(texArray, facesDrawn * 6);
				SetColourArray(colArray, facesDrawn * 6);
				//SetColour

				delete vertArray;
				delete normalArray;
				delete texArray;
				delete colArray;
			}

			void VoxelGrid::CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition)
			{
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p1, sizeof(float_t) * 3);	*arrayPosition += 3;
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p2, sizeof(float_t) * 3);	*arrayPosition += 3;
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p3, sizeof(float_t) * 3);	*arrayPosition += 3;
			}

			bool VoxelGrid::ShouldDraw()
			{
				if (_VertArrayDirty)UpdateVertArray();
				return Model::ShouldDraw();
			}

			Vector3I VoxelGrid::GetDimentions() const
			{
				return _Dimentions;
			}

			void VoxelGrid::SetVoxel(const Vector3I position,const Voxel newVoxel)
			{
				if (position.X < 0 || position.Y < 0 || position.Z < 0) throw std::invalid_argument("Voxel position not valid");
				if (position.X >= _Dimentions.X || position.Y >= _Dimentions.Y || position.Z >= _Dimentions.Z) throw std::invalid_argument("Voxel position not valid");

				_VoxelStore[position.X + position.Y*_Dimentions.X + position.Z*_Dimentions.X*_Dimentions.Y] = newVoxel;

				_VertArrayDirty = true;
			}

			VoxelGrid::Voxel VoxelGrid::GetVoxelAtPosition(const Vector3I position) const
			{
				return GetVoxelAtPosition(position.X, position.Y, position.Z);
			}

			VoxelGrid::Voxel VoxelGrid::GetVoxelAtPosition(const int32_t x, const int32_t y, const int32_t z) const
			{
				static Voxel nullVoxel = Voxel();

				if (x < 0 || x >= _Dimentions.X || y < 0 || y >= _Dimentions.Y || z < 0 || z >= _Dimentions.Z) return nullVoxel;

				return _VoxelStore[x + y*_Dimentions.X + z*_Dimentions.X*_Dimentions.Y];
			}
		}
	}
}