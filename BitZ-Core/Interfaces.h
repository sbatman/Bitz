#pragma once
#include <string>
#include "Common.h"
#include "Math\Math.h"

namespace Bitz
{
	using namespace Math;

	namespace Interfaces
	{
		template <class T>
		class ISizeable
		{
		public:
			virtual ~ISizeable() {}
			virtual T GetSize() const
			{
				return _Size;
			}
			virtual void SetSize(const T newSize)
			{
				_Size = newSize;
			}
		protected:
			T _Size;
		};

		template <class T>
		class IPositionable
		{
		public:
			virtual ~IPositionable() {}
			virtual T GetPosition() const
			{
				return _Position;
			}
			virtual void SetPosition(const T newPosition)
			{
				_Position = newPosition;
			}
		protected:
			T _Position;
		};

		template <class T>
		class IRotateable
		{
		public:
			virtual ~IRotateable() {}
			virtual T GetRotation() const
			{
				return _Rotation;
			}
			virtual void SetRotation(const T newRotation)
			{
				_Rotation = newRotation;
			}
		protected:
			T _Rotation;
		};

		/// <summary>
		/// This interface describes a class that has a Vector2F size
		/// </summary>
		typedef ISizeable<Vector2F> ISizeable2DF;
		/// <summary>
		/// This interface describes a class that has a Vector3F size
		/// </summary>
		typedef ISizeable<Vector3F> ISizeable3DF;

		/// <summary>
		/// This interface describes a class that has a float_t rotation
		/// </summary>
		typedef IRotateable<float_t> IRotateable2DF;
		/// <summary>
		/// This interface describes a class that has a Vector3F rotation
		/// </summary>
		typedef IRotateable<Vector3F> IRotateable3DF;

		/// <summary>
		/// This interface describes a class that has a Vector2F Position
		/// </summary>
		typedef IPositionable<Vector2F> IPositionable2DF;
		/// <summary>
		/// This interface describes a class that has a Vector3F Position
		/// </summary>
		typedef IPositionable<Vector3F> IPositionable3DF;

		/// <summary>
		/// This interface describes a class that has a string name
		/// </summary>
		class INameable
		{
		public:
			virtual ~INameable() {}
			/// <summary>
			/// Gets the current name of this class
			/// </summary>
			/// <returns>string containing the class name</returns>
			virtual std::string GetName()
			{
				return _Name;
			}
			/// <summary>
			/// Sets the current name of this class
			/// </summary>
			/// <param name='newName'>A std::string containing the classes new name</param>
			virtual void SetName(const std::string newName)
			{
				_Name = newName;
			}
		protected:
			std::string _Name;
		};

		/// <summary>
		/// This interface describes a class that has an Update function that can be paused
		/// </summary>
		class IUpdateable
		{
		public:
			virtual ~IUpdateable() {}

			/// <summary>
			/// Calls the class Internale Update function if the update isn't paused
			/// </summary>
			virtual void Update()
			{
				if (!_UpdatePaused)InternalUpdate();
			}
			/// <summary>
			/// Returns whether the class update function is paused
			/// </summary>
			/// <returns>True if it is paused else false</returns>
			virtual bool IsUpdatePaused()
			{
				return _UpdatePaused;
			}
			/// <summary>
			/// Pauses or resumes the class update function
			/// </summary>
			/// <param name='paused'>True to pause the update else false</param>
			virtual void PauseUpdate(bool paused)
			{
				_UpdatePaused = paused;
			}
		protected:
			bool _UpdatePaused;
			virtual void InternalUpdate() = 0;
		};

		class IColourable
		{
		public:
			virtual ~IColourable() {}
			virtual Vector4F GetColour() const
			{
				return _Colour;
			}
			virtual void SetColour(const Vector4F newColour)
			{
				_Colour = newColour;
			}
		protected:
			Vector4F _Colour = Vector4F(1);
		};
	}
}
