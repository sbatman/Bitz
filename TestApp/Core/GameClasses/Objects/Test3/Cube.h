#pragma once

#include "../../../Common.h"

class Cube : public Bitz::GFX::Drawables::Model
{
public:
	Cube();
	~Cube();
	virtual bool ShouldDraw() override;
private:
	bool _VertArrayDirty;
	void UpdateVertArray();
	void UpdateTextureArray();
	void CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition);
};

