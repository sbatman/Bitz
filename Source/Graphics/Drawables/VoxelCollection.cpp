#include "VoxelCollection.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			VoxelCollection::VoxelCollection(const Vector3I& dimentions)
			{
				_VertArrayDirty = true;
				_Dimentions = dimentions;
				_VoxelStore = new Voxel[_Dimentions.X*_Dimentions.Y*_Dimentions.Z];
				for (int x = 0; x < _Dimentions.X; x++)
				{
					for (int y = 0; y < _Dimentions.Y; y++)
					{
						for (int z = 0; z < _Dimentions.Z; z++)
						{
							_VoxelStore[x + y * _Dimentions.X + z * _Dimentions.X*_Dimentions.Y].Type = -1;
						}
					}
				}
			}

			VoxelCollection::~VoxelCollection()
			{
			}

			void VoxelCollection::UpdateVertArray()
			{
				_VertArrayDirty = false;

				_PopulatedVoxelCount = 0;

				for (int x = 0; x < _Dimentions.X; x++)
				{
					for (int y = 0; y < _Dimentions.Y; y++)
					{
						for (int z = 0; z < _Dimentions.Z; z++)
						{
							if (_VoxelStore[x + y * _Dimentions.X + z * _Dimentions.X*_Dimentions.Y].Type != -1)_PopulatedVoxelCount++;
						}
					}
				}

				if (_PopulatedVoxelCount == 0)return;
				int verpos = 0;
				float_t * vertArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];
				float_t * normalArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];
				float_t * texArray = new float_t[6 * 2 * 3 * 2 * _PopulatedVoxelCount];
				float_t * colArray = new float_t[6 * 2 * 3 * 4 * _PopulatedVoxelCount];
				float_t * additionalArray = new float_t[6 * 2 * 3 * 3 * _PopulatedVoxelCount];

				float_t tex[12] = { 0,0, 0,1, 1,0, 0,1, 1,1, 1,0 };

				const float centerOffsetX = _Dimentions.X*0.5f;
				const float centerOffsetY = _Dimentions.Y*0.5f;
				const float centerOffsetZ = _Dimentions.Z*0.5f;

				int32_t facesDrawn = 0;

				for (Voxel targetVoxel : _VoxelStore)
				{
					auto position = targetVoxel.GetPosition();
					if (targetVoxel.Type != -1)
					{
						float_t tlf[3] = { position.X - centerOffsetX - 0.5f, position.Y - centerOffsetY - 0.5f, position.Z - centerOffsetZ - 0.5f };
						float_t trf[3] = { position.X - centerOffsetX + 0.5f, position.Y - centerOffsetY - 0.5f, position.Z - centerOffsetZ - 0.5f };
						float_t blf[3] = { position.X - centerOffsetX - 0.5f, position.Y - centerOffsetY + 0.5f, position.Z - centerOffsetZ - 0.5f };
						float_t brf[3] = { position.X - centerOffsetX + 0.5f, position.Y - centerOffsetY + 0.5f, position.Z - centerOffsetZ - 0.5f };
						float_t tlb[3] = { position.X - centerOffsetX - 0.5f, position.Y - centerOffsetY - 0.5f, position.Z - centerOffsetZ + 0.5f };
						float_t trb[3] = { position.X - centerOffsetX + 0.5f, position.Y - centerOffsetY - 0.5f, position.Z - centerOffsetZ + 0.5f };
						float_t blb[3] = { position.X - centerOffsetX - 0.5f, position.Y - centerOffsetY + 0.5f, position.Z - centerOffsetZ + 0.5f };
						float_t brb[3] = { position.X - centerOffsetX + 0.5f, position.Y - centerOffsetY + 0.5f, position.Z - centerOffsetZ + 0.5f };

						float_t colors[12] =
						{
							targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f,
							targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f,
							targetVoxel.Colour.X / 255.0f ,targetVoxel.Colour.Y / 255.0f,targetVoxel.Colour.Z / 255.0f,targetVoxel.Colour.W / 255.0f
						};

						float_t additional[9] =
						{
							targetVoxel.Emissive,targetVoxel.Specular,1.0f,
							targetVoxel.Emissive,targetVoxel.Specular,1.0f,
							targetVoxel.Emissive,targetVoxel.Specular,1.0f,
						};

						float normals[9 * 12] = {
							0, 0, -1, 0, 0, -1, 0, 0, -1, //bottom
							0, 0,-1, 0, 0, -1, 0, 0, -1, //bottom

							-1, 0, 0, -1, 0, 0, -1, 0, 0, //right
							-1, 0, 0, -1, 0, 0, -1, 0, 0, //right
							1, 0, 0, 1, 0, 0, 1, 0, 0, //left
							1, 0, 0, 1, 0, 0, 1, 0, 0, //left
							0, 0, 1, 0, 0, 1, 0, 0, 1, //Top
							0, 0, 1, 0, 0, 1, 0, 0, 1,//Top

							0, -1, 0, 0, -1, 0, 0, -1, 0, //Back
							0, -1, 0, 0, -1, 0, 0, -1, 0, //Back
							0, 1, 0, 0, 1, 0, 0, 1, 0, //Front
							0, 1, 0, 0, 1, 0, 0, 1, 0, //Front
						};

						//bottom
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), normals, 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[facesDrawn * 24], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, tlf, blf, trf, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, blf, brf, trf, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

						facesDrawn++;

						//LEFT	
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[2 * 9], 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[facesDrawn * 24], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, tlb, blb, tlf, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, blb, blf, tlf, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);
						facesDrawn++;

						//RIGHT	
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[4 * 9], 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, trf, brf, trb, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, brf, brb, trb, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);
						facesDrawn++;

						//Back
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[6 * 9], 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, trb, brb, tlb, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, brb, blb, tlb, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

						facesDrawn++;

						//Top	
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[8 * 9], 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, tlb, tlf, trb, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, tlf, trf, trb, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

						facesDrawn++;

						//Bottom	
						Memcpy(normalArray + verpos, 9 * 2 * sizeof(float_t), &normals[10 * 9], 9 * 2 * sizeof(float_t));
						Memcpy(&colArray[(facesDrawn * 24)], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, blf, blb, brf, &verpos);
						Memcpy(&colArray[facesDrawn * 24 + 12], sizeof(float_t) * 12, colors, sizeof(float_t) * 12);
						Memcpy(additionalArray + verpos, sizeof(float_t) * 9, additional, sizeof(float_t) * 9);
						CopyTri(vertArray, blb, brb, brf, &verpos);
						Memcpy(&texArray[facesDrawn * 12], sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

						facesDrawn++;


					}

				}

				SetVertArray(vertArray, facesDrawn * 6);
				SetNormalArray(normalArray, facesDrawn * 6);
				SetTexCordArray(texArray, facesDrawn * 6);
				SetColourArray(colArray, facesDrawn * 6);
				SetAdditionalArray(additionalArray, facesDrawn * 6);
				//SetColour

				delete[] vertArray;
				delete[] normalArray;
				delete[] texArray;
				delete[] colArray;
			}

			void VoxelCollection::CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition)
			{
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p1, sizeof(float_t) * 3);	*arrayPosition += 3;
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p2, sizeof(float_t) * 3);	*arrayPosition += 3;
				Memcpy(vertList + *arrayPosition, sizeof(float_t) * 3, p3, sizeof(float_t) * 3);	*arrayPosition += 3;
			}

			bool VoxelCollection::ShouldDraw()
			{
				if (_VertArrayDirty)UpdateVertArray();
				return Model::ShouldDraw();
			}

			Vector3I VoxelCollection::GetDimentions() const
			{
				return _Dimentions;
			}

			void VoxelCollection::AddVoxel(const Voxel newVoxel)
			{
				_VoxelStore.push_back(newVoxel);
			}

			void VoxelCollection::RemoveVoxel(const Voxel voxel)
			{
				std::remove(std::begin(_VoxelStore), std::end(_VoxelStore), voxel);
			}
		}
	}
}