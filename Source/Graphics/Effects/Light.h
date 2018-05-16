#pragma once
#pragma once
#include "../../Common.h"
#include "../../Math/Math.h"
#include "../../Interfaces/Core.h"
#include "../../Interfaces/IAnchorable.h"

class Light : public Bitz::Interfaces::IPositionable3DF
{
public:
	Light();

	typedef std::shared_ptr<Light> Light_Ptr;
};

