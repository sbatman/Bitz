#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <cstdio>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <string>
#include <stdint.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

//OpenGL Includes
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#elif WIN32
#define GLEW_STATIC true
#include "Libs/Glew/glew.h"
#endif

//GLM Include
#include "../Support Libraries/glm/glm/glm.hpp"
#include "../Support Libraries/glm/glm/gtc/matrix_transform.hpp"
#include "../Support Libraries/glm/glm/gtc/type_ptr.hpp"

//Cpp Format Includes
#include "Libs/CppFormat/format.h"