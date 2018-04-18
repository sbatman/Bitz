#include "Utility.h"

void Memcpy(void *dest, size_t numberOfElements, const void *src, size_t count)
{
#ifdef __ANDROID__
	memcpy(dest, src, count);
#elif WIN32
	memcpy_s(dest, numberOfElements, src, count);
#endif
}