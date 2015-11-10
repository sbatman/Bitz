#pragma once
#include <string>
#include "Common.h"
#include "Math\Math.h"
#include "../Interfaces/Core.h"
#include "IDisposable.h"

namespace Bitz
{
	namespace Layout
	{
		class Anchor;

		typedef std::shared_ptr<Anchor> Anchor_Ptr;
	}

	namespace Interfaces
	{
		class IAnchorable : 
			public IPositionable2DF,
			public IDisposable
		{
		public:
			IAnchorable();
			virtual ~IAnchorable() = 0;
			virtual Layout::Anchor_Ptr GetParentAnchor() const;
			virtual void SetParentAnchor(const Layout::Anchor_Ptr newParentAnchor);
			virtual Vector2F GetOffset() const;
			virtual void SetOffset(const Math::Vector2F newOffset);
			virtual void TriggerPositonUpdate();
			virtual void Dispose() override;
		private:
			Layout::Anchor_Ptr _ParentAnchor;
			Math::Vector2F _Offset;
		};

		typedef std::shared_ptr<IAnchorable> IAnchorable_Ptr;
	}
}