#ifndef RELEASE
#ifdef _WIN64
#include <intrin.h>
#define DebugTrap() __debugbreak()
#else
#define DebugTrap() __builtin_trap()
#endif
#endif
