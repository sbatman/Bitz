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
	float_t vertArray[108] = { 0 };

	float_t halfSizeX = _Size.X*0.5f;
	float_t halfSizeY = _Size.Y*0.5f;
	float_t halfSizeZ = _Size.Z*0.5f;

	float x = GetPosition().X;
	float y = GetPosition().Y;
	float z = GetPosition().Z;

	float_t _tlf[3] = { x - halfSizeX, y - halfSizeY, z - halfSizeZ };
	float_t _trf[3] = { x + halfSizeX, y - halfSizeY, z - halfSizeZ };
	float_t _blf[3] = { x - halfSizeX, y + halfSizeY, z - halfSizeZ };
	float_t _brf[3] = { x + halfSizeX, y + halfSizeY, z - halfSizeZ };
	float_t _tlb[3] = { x - halfSizeX, y - halfSizeY, z + halfSizeZ };
	float_t _trb[3] = { x + halfSizeX, y - halfSizeY, z + halfSizeZ };
	float_t _blb[3] = { x - halfSizeX, y + halfSizeY, z + halfSizeZ };
	float_t _brb[3] = { x + halfSizeX, y + halfSizeY, z + halfSizeZ };

	int verpos = 0;

	//FRONT
	CopyTri(vertArray, _tlf, _blf, _trf, &verpos);
	CopyTri(vertArray, _blf, _brf, _trf, &verpos);

	//LEFT	
	CopyTri(vertArray, _tlb, _blb, _tlf, &verpos);
	CopyTri(vertArray, _blb, _blf, _tlf, &verpos);

	//RIGHT	
	CopyTri(vertArray, _trb, _trf, _brb, &verpos);
	CopyTri(vertArray, _brf, _brb, _trf, &verpos);

	//Back	
	CopyTri(vertArray, _blb, _tlb, _trb, &verpos);
	CopyTri(vertArray, _blb, _trb, _brb, &verpos);

	//Top	
	CopyTri(vertArray, _tlb, _tlf, _trb, &verpos);
	CopyTri(vertArray, _tlf, _trf, _trb, &verpos);

	//Bottom	
	CopyTri(vertArray, _blb, _brb, _blf, &verpos);
	CopyTri(vertArray, _blf, _brb, _brf, &verpos);
}

void Cube::CopyTri(float_t * vertList, float_t* p1, float_t* p2, float_t* p3, int* arrayPosition)
{
	memcpy_s(vertList + (*arrayPosition), sizeof(double) * 3, p1, sizeof(double) * 3);	(*arrayPosition) += 3;
	memcpy_s(vertList + (*arrayPosition), sizeof(double) * 3, p2, sizeof(double) * 3);	(*arrayPosition) += 3;
	memcpy_s(vertList + (*arrayPosition), sizeof(double) * 3, p3, sizeof(double) * 3);	(*arrayPosition) += 3;
}

void Cube::SetSize(const Bitz::Math::Vector3F newSize)
{
	if (_Size != newSize) _VertArrayDirty = true;
	_Size = newSize;
}

