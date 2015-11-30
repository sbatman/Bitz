#include "Cube.h"

Cube::Cube()
{
	_VertArrayDirty = true;
}

Cube::~Cube()
{
}

void Cube::UpdateVertArray()
{
	_VertArrayDirty = false;
	float_t vertArray[6 * 2 * 3 * 3] = { 0 };

	float_t _tlf[3] = { -0.5f,  -0.5f,  -0.5f };
	float_t _trf[3] = { 0.5f, -0.5f,  -0.5f };
	float_t _blf[3] = { -0.5f,   0.5f,  -0.5f };
	float_t _brf[3] = { 0.5f,   0.5f, -0.5f };
	float_t _tlb[3] = { -0.5f,  -0.5f,   0.5f };
	float_t _trb[3] = { 0.5f,  -0.5f,   0.5f };
	float_t _blb[3] = { -0.5f,   0.5f,   0.5f };
	float_t _brb[3] = { 0.5f,   0.5f,   0.5f };

	int verpos = 0;

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

	SetVerts(vertArray, 36);

	UpdateTextureArray();
}

void Cube::UpdateTextureArray()
{
	float_t texArray[6 * 2 * 3 * 2] = { 0 };

	float_t tex[12] = { 0,0, 0,1, 1,0, 0,1, 1,1, 1,0 };

	for (int i = 0;i < 6;i++)Memcpy(texArray + (i * 12), sizeof(float_t) * 12, tex, sizeof(float_t) * 12);

	SetTexCords(texArray, 36);
}

void Cube::CopyTri(float_t * vertList, const float_t* p1, const float_t* p2, const float_t* p3, int* arrayPosition)
{
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p1, sizeof(float_t) * 3);	(*arrayPosition) += 3;
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p2, sizeof(float_t) * 3);	(*arrayPosition) += 3;
	Memcpy(vertList + (*arrayPosition), sizeof(float_t) * 3, p3, sizeof(float_t) * 3);	(*arrayPosition) += 3;
}

bool Cube::ShouldDraw()
{
	if (_VertArrayDirty)UpdateVertArray();
	return Model::ShouldDraw();
}