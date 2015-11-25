#pragma once

#include "../../../Common.h"

class Cube : public Bitz::GFX::Drawables::Model
{
public:
	Cube();
	~Cube();
	void SetSize(const Bitz::Math::Vector3F newSize) override;
	void SetPosition(const Bitz::Math::Vector3F newPosition) override;
	virtual bool ShouldDraw() override;
private:
	bool _VertArrayDirty;
	void UpdateVertArray();
	void UpdateTextureArray();
	void CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition);
};

