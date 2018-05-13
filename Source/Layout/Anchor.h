#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "../Interfaces/Core.h"
#include "../Interfaces/IAnchorable.h"

namespace Bitz
{
	namespace Layout
	{
		/// <summary>
		/// The Anchor class is used for constucting 2D relative positioning with cascading updates
		/// This is most useful in 2d games and UI work
		/// </summary>
		class Anchor :
			public Interfaces::INameable,
			public Interfaces::IAnchorable
		{
		public:
			/// <summary>
			/// Creates a new instance of the Anchor Class
			/// </summary>
			Anchor();
			/// <summary>
			/// The destuctor of the Anchor Class
			/// </summary>
			~Anchor();
			virtual void SetPosition(const Math::Vector2F newPosition);
			virtual void UpdateDependents();
			virtual void AddDependent(const Interfaces::IAnchorable_Ptr newDependednt);
			virtual void RemoveDependent(const Interfaces::IAnchorable_Ptr newDependednt);

		private:
			std::vector<Interfaces::IAnchorable_Ptr> _Anchorables = std::vector<Interfaces::IAnchorable_Ptr>();
		};

		typedef std::shared_ptr<Anchor> Anchor_Ptr;
	}
}
