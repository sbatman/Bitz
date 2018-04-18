#include "IDisposable.h"

namespace Bitz
{
	namespace Interfaces
	{
		IDisposable::IDisposable()
		{
			_IsDisposed = false;
		}

		IDisposable::~IDisposable()
		{
		}

		bool IDisposable::IsDisposed()const
		{
			return _IsDisposed;
		}

		void IDisposable::AttemptDispose()
		{
			if (!_IsDisposed)
			{
				_IsDisposed = true;
				Dispose();
			}
		}
	}
}
