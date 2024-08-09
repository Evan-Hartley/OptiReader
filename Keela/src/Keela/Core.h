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

