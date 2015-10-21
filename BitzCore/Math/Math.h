#pragma once
#include "../Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

typedef unsigned char byte;

namespace Bitz
{
	namespace Math
	{
		//Shortcut Typedefs
		//--//Vector2
		typedef Vector2<uint8_t> Vector2B;
		typedef Vector2<float_t> Vector2F;
		typedef Vector2<double_t> Vector2D;
		typedef Vector2<int32_t> Vector2I;
		typedef Vector2<int16_t> Vector2S;
		typedef Vector2<int64_t> Vector2L;

		//--//Vector3
		typedef Vector3<uint8_t> Vector3B;
		typedef Vector3<float_t> Vector3F;
		typedef Vector3<double_t> Vector3D;
		typedef Vector3<int32_t> Vector3I;
		typedef Vector3<int64_t> Vector3L;

		//--//Vector4
		typedef Vector4<uint8_t> Vector4B;
		typedef Vector4<float_t> Vector4F;
		typedef Vector4<double_t> Vector4D;
		typedef Vector4<int32_t> Vector4I;
		typedef Vector4<int64_t> Vector4L;
	}
}