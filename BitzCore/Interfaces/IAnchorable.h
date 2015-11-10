#pragma once
#include <string>
#include "Common.h"
#include "Math\Math.h"
#include "../Interfaces/Core.h"
#include "../Layout/Anchor.h"

namespace Bitz
{
	using namespace Math;

	namespace Interfaces
	{

		class IAnchorable : public IPositionable2DF
		{
		public:
			virtual ~IAnchorable() {}
			virtual Bitz::Layout::Anchor_Ptr GetParentAnchor()
			{
				return _ParentAnchor;
			}
			virtual void SetParentAnchor(Bitz::Layout::Anchor_Ptr newParentAnchor)
			{
				_ParentAnchor = newParentAnchor;
			}
			virtual Vector2F GetOffset()
			{
				return _Offset;
			}
			virtual void SetOffset(Vector2F newOffset)
			{
				_Position = _ParentAnchor->GetPosition() + _Offset;
				_Offset = newOffset;
			}
			virtual void TriggerPositonUpdate()
			{
				_Position = _ParentAnchor->GetPosition() + _Offset;
			}
		private:
			Bitz::Layout::Anchor_Ptr _ParentAnchor = nullptr;
			Vector2F _Offset = Vector2F(0);
		};

		typedef std::shared_ptr<IAnchorable> IAnchorable_Ptr;
	}
}