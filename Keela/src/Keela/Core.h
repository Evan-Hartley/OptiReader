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

// Define Bit(x) as 1 shifted by x places
#define BIT(x) (1 << x)
