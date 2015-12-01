#pragma once

#include "../../../Common.h"

class VoxelGrid : public Bitz::GFX::Drawables::Model
{
public:

	struct Voxel
	{
		int32_t Type;
		Bitz::Math::Vector4F Colour;
	};

	VoxelGrid(Bitz::Math::Vector3I dimentions);
	~VoxelGrid();
	virtual bool ShouldDraw() override;
	virtual Bitz::Math::Vector3I GetDimentions();
	virtual void SetVoxel(Bitz::Math::Vector3I position, Voxel newVoxel);

private:
	bool _VertArrayDirty;
	Bitz::Math::Vector3I _Dimentions;
	Voxel * _VoxelStore = nullptr;
	int32_t _PopulatedVoxelCount = 0;

	void UpdateVertArray();
	void UpdateTextureArray();
	void CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition);
};

