#include "Game.h"

void ANativeActivity_onCreate(ANativeActivity* activity,
	void* savedState, size_t savedStateSize)
{
	Game* gameInstance = new Game();
	ANativeActivity_Create(activity, savedState, savedStateSize, gameInstance);
	
}