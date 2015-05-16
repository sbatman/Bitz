#pragma once
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

			~Vector3()
			{
			}

			Vector3<T>& __fastcall operator=(const Vector3<T>& other)
			{
				this->X = other.X;
				this->Y = other.Y;
				this->Z = other.Z;
				return *this;
			}

			Vector3<T> __fastcall operator+(const Vector3<T> other)
			{
				return Vector3<T>(X + other.X, Y + other.Y, Z + other.Z);
			}

			Vector3<T> __fastcall operator-(const Vector3<T> other)
			{
				return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z);
			}

			Vector3<T> __fastcall operator/(const Vector3<T> other)
			{
				return Vector3<T>(X / other.X, Y / other.Y, Z / other.Z);
			}

			Vector3<T> __fastcall operator*(const Vector3<T> other)
			{
				return Vector3<T>(X * other.X, Y * other.Y, Z * other.Z);
			}

			T __fastcall Length() const
			{
				return static_cast<T> (sqrt((X*X) + (Y*Y) + (Z*Z)));
			}

			T __fastcall LengthSquared() const
			{
				return static_cast<T> ((X*X) + (Y*Y) + (Z*Z));
			}
		};
	}
}
