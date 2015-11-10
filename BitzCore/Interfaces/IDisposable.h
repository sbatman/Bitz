#pragma once

namespace Bitz
{
	namespace Interfaces
	{
		class IDisposable
		{
		public:
			IDisposable();
			virtual ~IDisposable() = 0;

			virtual void AttemptDispose();
			virtual bool IsDisposed()const;
		protected:
			virtual void Dispose() = 0;
		private:
			bool _IsDisposed;
		};
	}
}
