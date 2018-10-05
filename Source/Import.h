#pragma once
//Common includes
#include "Common.h"

//Content
#include "Content/ContentManager.h"
#include "Content/IO.h"
#include "Content/TextureData.h"
#include "Content/MagicaVoxelData.h"

//GameLogic
#include "GameLogic/GameCore.h"
#include "GameLogic/GameState.h"
#include "GameLogic/GameStateService.h"

//Graphics
#include "Graphics/Window.h"
#include "Graphics/GLContext.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "Graphics/GraphicsStates/GS2D.h"
#include "Graphics/GraphicsStates/GS3D.h"
#include "Graphics/Effects/Light.h"
#include "Graphics/Drawables/Sprite.h"
#include "Graphics/Drawables/Model.h"
#include "Graphics/Drawables/VoxelGrid.h"
#include "Graphics/Drawables/VoxelCollection.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Window.h"

//Physics
#include "Physics/I2DPhysicsEnabled.h"
#include "Physics/Box2D/Box2D.h"

//Debug
#include "Debug/Logging.h"
#include "Debug/Console.h"

//Math
#include "Math/Math.h"

//Input
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

//Time
#include "Time/Consts.h"
#include "Time/Timer.h"