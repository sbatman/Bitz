#pragma once
#include "../Common.h"
#include "../Physics/Box2D/Common/b2Math.h"

namespace Bitz
{
	namespace Math
	{
		template <class T>
		class Vector3
		{
		public:

			T X;
			T Y;
			T Z;

			Vector3()
			{
				X = 0;
				Y = 0;
				Z = 0;
			}

			Vector3(const T x, const T y, const T z)
			{
				X = x;
				Y = y;
				Z = z;
			}

			explicit Vector3(const T xyz)
			{
				X = xyz;
				Y = xyz;
				Z = xyz;
			}

			Vector3(const Vector3<T> &instanceToCopy)
			{
				X = instanceToCopy.X;
				Y = instanceToCopy.Y;
				Z = instanceToCopy.Z;
			}

			Vector3(const b2Vec3 vec3)
			{
				X = vec3.x;
				Y = vec3.y;
				Z = vec3.z;
			}

			~Vector3()
			{
			}

			Vector3<T>& __FASTCALL operator=(const Vector3<T>& other)
			{
				this->X = other.X;
				this->Y = other.Y;
				this->Z = other.Z;
				return *this;
			}

			Vector3<T> __FASTCALL operator+(const Vector3<T> other)
			{
				return Vector3<T>(X + other.X, Y + other.Y, Z + other.Z);
			}

			Vector3<T> __FASTCALL operator-(const Vector3<T> other)
			{
				return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z);
			}

			Vector3<T> __FASTCALL operator/(const Vector3<T> other)
			{
				return Vector3<T>(X / other.X, Y / other.Y, Z / other.Z);
			}

			Vector3<T> __FASTCALL operator*(const Vector3<T> other)
			{
				return Vector3<T>(X * other.X, Y * other.Y, Z * other.Z);
			}

			////

			Vector3<T> __FASTCALL operator/(const T other)
			{
				return Vector3<T>(X / other.X, Y / other.Y, Z / other.Z);
			}

			Vector3<T> __FASTCALL operator*(const T other)
			{
				return Vector3<T>(X * other, Y * other, Z * other);
			}

			bool __FASTCALL operator==(const Vector3<T>& other) const
			{
				return X == other.X && Y == other.Y && Z == other.Z;
			}

			bool __FASTCALL operator!=(const Vector3<T>& other) const
			{
				return !(*this == other);
			}

			////

			T __FASTCALL Length() const
			{
				return static_cast<T> (sqrt((X*X) + (Y*Y) + (Z*Z)));
			}

			T __FASTCALL LengthSquared() const
			{
				return static_cast<T> ((X*X) + (Y*Y) + (Z*Z));
			}
		};
	}
}
