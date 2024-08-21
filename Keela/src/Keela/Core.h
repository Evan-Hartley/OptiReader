#pragma once

//Set the Platform to run on

#ifdef KEE_PLATFORM_WINDOWS
	#ifdef KEE_BUILD_DLL
		#define KEELA_API __declspec(dllexport)
	#else
		#define KEELA_API __declspec(dllimport)
	#endif
#else
	#error Keela only supports Windows!
#endif

#ifdef KEE_DEBUG
	#define KEE_ENABLE_ASSERTS
#endif

#ifdef KEE_ENABLE_ASSERTS
	#define KEE_ASSERT(x, ...) { if(!(x)) { KEE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define KEE_CORE_ASSERT(x, ...) { if(!(x)) { KEE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define KEE_ASSERT(x, ...)
	#define KEE_CORE_ASSERT(x, ...)
#endif

// Define Bit(x) as 1 shifted by x places
#define BIT(x) (1 << x)

#define KEE_BIND_EVENTS_FN(fn) std::bind(&fn, this, std::placeholders::_1)