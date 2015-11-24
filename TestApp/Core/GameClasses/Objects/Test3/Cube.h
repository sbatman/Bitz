#pragma once

#include "../../../Common.h"

class Cube : public Bitz::GFX::Drawables::Model
{
public:
	Cube();
	~Cube();
	void SetSize(const Bitz::Math::Vector3F newSize) override;

private:
	bool _VertArrayDirty;

	void UpdateVertArray();
	void CopyTri(float_t * vertList, float_t* p1, float_t* p2, float_t* p3, int* arrayPosition);
};

