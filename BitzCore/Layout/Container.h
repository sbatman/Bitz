#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "../Interfaces/Core.h"

namespace Bitz
{
	namespace Layout
	{
		class Container : 
			public Bitz::Interfaces::IPositionable2DF,
			public Bitz::Interfaces::ISizeable2DF,
			public Bitz::Interfaces::INameable
		{
		public:
			Container();
			~Container();
		};
	}
}
