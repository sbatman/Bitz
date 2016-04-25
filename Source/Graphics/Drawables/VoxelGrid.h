#pragma once

#include "Model.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			class VoxelGrid : public Model
			{
			public:

				struct Voxel
				{
					int32_t Type = -1;
					Vector4B Colour;
				};

				VoxelGrid(const Vector3I dimentions);
				~VoxelGrid();
				virtual bool ShouldDraw() override;
				virtual Vector3I GetDimentions() const;
				virtual void SetVoxel(const Vector3I position, const Voxel newVoxel);
				virtual Voxel GetVoxelAtPosition(const Vector3I position) const;
				virtual Voxel GetVoxelAtPosition(const int32_t x, const  int32_t y, const int32_t z) const;

			private:
				bool _VertArrayDirty;
				Vector3I _Dimentions;
				Voxel * _VoxelStore = nullptr;
				int32_t _PopulatedVoxelCount = 0;

				void UpdateVertArray();
				static void CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition);
			};

		}
	}
}