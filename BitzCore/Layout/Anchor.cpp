#include "../Interfaces/IAnchorable.h"
#include "Anchor.h"

namespace Bitz
{
	namespace Layout
	{
		Anchor::Anchor()
		{
		}

		Anchor::~Anchor()
		{
			if (!IsDisposed()) Dispose();
		}

		void Anchor::SetPosition(const Math::Vector2F newPosition)
		{
			Interfaces::IPositionable2DF::SetPosition(newPosition);
			UpdateDependents();
		}

		void Anchor::UpdateDependents()
		{
			for (Interfaces::IAnchorable_Ptr a : _Anchorables)a->TriggerPositonUpdate();
		}

		void Anchor::AddDependent(Interfaces::IAnchorable_Ptr newDependednt)
		{
			_Anchorables.push_back(newDependednt);
		}

		void Anchor::RemoveDependent(Interfaces::IAnchorable_Ptr newDependednt)
		{
			_Anchorables.erase(std::remove(_Anchorables.begin(), _Anchorables.end(), newDependednt), _Anchorables.end());
		}

		void Anchor::Dispose()
		{
			_Anchorables.clear();
			IAnchorable::Dispose();
		}
	}
}