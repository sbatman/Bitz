#include "IAnchorable.h"
#include "../Math/Math.h"
#include "../Layout/Anchor.h"

namespace Bitz
{
	namespace Interfaces
	{
		IAnchorable::IAnchorable()
		{
			_ParentAnchor = nullptr;
			_Offset = Vector2F(0);
		}

		Layout::Anchor_Ptr IAnchorable::GetParentAnchor() const
		{
			return _ParentAnchor;
		}

		void IAnchorable::SetParentAnchor(const Layout::Anchor_Ptr newParentAnchor)
		{
			_ParentAnchor = newParentAnchor;
		}

		Math::Vector2F IAnchorable::GetOffset() const
		{
			return _Offset;
		}

		void IAnchorable::SetOffset(const Math::Vector2F newOffset)
		{
			SetPosition(_ParentAnchor->GetPosition() + _Offset);
			_Offset = newOffset;
		}

		void IAnchorable::TriggerPositonUpdate()
		{
			SetPosition(_ParentAnchor->GetPosition() + _Offset);
		}

		void IAnchorable::Dispose()
		{
			_ParentAnchor = nullptr;
		}
	}
}