#pragma once

#include "Model.h"
#include "../../Physics/I2DPhysicsEnabled.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			class VoxelCollection : public Model
			{
			public:

				struct Voxel : public Interfaces::IPositionable3DF
				{
					int32_t Type = -1;
					Vector4B Colour;
					float_t Specular = 0;
					float_t Emissive = 0;
				};

				VoxelCollection(const Vector3I& dimentions);
				~VoxelCollection();
				virtual bool ShouldDraw() override;
				virtual Vector3I GetDimentions() const;
				virtual void AddVoxel(const Voxel newVoxel);
				virtual void RemoveVoxel(const Voxel newVoxel);

			private:
				bool _VertArrayDirty;
				Vector3I _Dimentions;
				std::vector<Voxel> _VoxelStore ;
				int32_t _PopulatedVoxelCount = 0;

				void UpdateVertArray();
				static void CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition);
			};

		}
	}
}
