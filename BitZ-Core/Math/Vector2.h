#pragma once
#include "../Common.h"

namespace Bitz
{
	namespace Math
	{
		template <class T>
		class Vector2
		{
		public:

			T X;
			T Y;

			Vector2()
			{
				X = 0;
				Y = 0;
			}

			Vector2(const T x, const T y)
			{
				X = x;
				Y = y;
			}

			explicit Vector2(const T xy)
			{
				X = xy;
				Y = xy;
			}

			Vector2(const Vector2<T> &instanceToCopy)
			{
				X = instanceToCopy.X;
				Y = instanceToCopy.Y;
			}

			~Vector2()
			{
			}

			Vector2<T>& __FASTCALL operator=(const Vector2<T>& other)
			{
				this->X = other.X;
				this->Y = other.Y;
				return *this;
			}

			Vector2<T> __FASTCALL operator+(const Vector2<T> other)
			{
				return Vector2<T>(X + other.X, Y + other.Y);
			}

			Vector2<T> __FASTCALL operator-(const Vector2<T> other)
			{
				return Vector2<T>(X - other.X, Y - other.Y);
			}

			Vector2<T> __FASTCALL operator/(const Vector2<T> other)
			{
				return Vector2<T>(X / other.X, Y / other.Y);
			}

			Vector2<T> __FASTCALL operator*(const Vector2<T> other)
			{
				return Vector2<T>(X * other.X, Y * other.Y);
			}

			T __FASTCALL Length() const
			{
				return static_cast<T>(sqrt((X*X) + (Y*Y)));
			}

			T __FASTCALL LengthSquared() const
			{
				return static_cast<T> ((X*X) + (Y*Y));
			}
		};
	}
}