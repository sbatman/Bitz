#pragma once
#include "../Common.h"

namespace Bitz
{
	namespace Math
	{
		template <class T>
		class Vector4
		{
		public:

			T X;
			T Y;
			T Z;
			T W;

			Vector4()
			{
				X = 0;
				Y = 0;
				Z = 0;
				W = 0;
			}

			Vector4(const T x, const T y, const T z, const T w)
			{
				X = x;
				Y = y;
				Z = z;
				W = w;
			}

			explicit Vector4(const T xyzw)
			{
				X = xyzw;
				Y = xyzw;
				Z = xyzw;
				W = xyzw;
			}

			Vector4(const Vector4<T> &instanceToCopy)
			{
				X = instanceToCopy.X;
				Y = instanceToCopy.Y;
				Z = instanceToCopy.Z;
				W = instanceToCopy.W;
			}

			~Vector4()
			{
			}

			Vector4<T>& __FASTCALL operator = (const Vector4<T>& other)
			{
				this->X = other.X;
				this->Y = other.Y;
				this->Z = other.Z;
				this->W = other.W;
				return *this;
			}

			Vector4<T> __FASTCALL operator + (const Vector4<T> other)
			{
				return Vector4<T>(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
			}

			Vector4<T> __FASTCALL operator - (const Vector4<T> other)
			{
				return Vector4<T>(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
			}

			Vector4<T> __FASTCALL operator / (const Vector4<T> other)
			{
				return Vector4<T>(X / other.X, Y / other.Y, Z / other.Z, W / other.W);
			}

			Vector4<T> __FASTCALL operator*(const Vector4<T> other)
			{
				return Vector4<T>(X * other.X, Y * other.Y, Z * other.Z, W * other.W);
			}

			////

			Vector4<T> __FASTCALL operator / (const T other)
			{
				return Vector4<T>(X / other, Y / other, Z / other, W / other);
			}

			Vector4<T> __FASTCALL operator*(const T other)
			{
				return Vector4<T>(X * other, Y * other, Z * other, W * other);
			}

			bool __FASTCALL operator==(const Vector4<T>& other) const
			{
				return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
			}

			bool __FASTCALL operator!=(const Vector4<T>& other) const
			{
				return !(*this == other);
			}

			////

			T __FASTCALL Length() const
			{
				return static_cast<T>(sqrt((X*X) + (Y*Y) + (Z*Z) + (W*W)));
			}

			T __FASTCALL LengthSquared() const
			{
				return static_cast<T> ((X*X) + (Y*Y) + (Z*Z) + (W*W));
			}
		};
	}
}