#pragma once
#include "../Common.h"
#include "../Physics/Box2D/Common/b2Math.h"

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

			Vector2(const b2Vec2 vec2)
			{
				X = vec2.x;
				Y = vec2.y;
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

			////

			Vector2<T> __FASTCALL operator/(const T other)
			{
				return Vector2<T>(X / other, Y / other);
			}

			Vector2<T> __FASTCALL operator*(const T other)
			{
				return Vector2<T>(X * other, Y * other);
			}

			bool __FASTCALL operator==(const Vector2<T>& other) const
			{
				return X == other.X && Y == other.Y;
			}

			bool __FASTCALL operator!=(const Vector2<T>& other) const
			{
				return !(*this == other);
			}

			////

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