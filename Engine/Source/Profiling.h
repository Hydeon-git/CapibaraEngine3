#pragma once

#include "Optick/include/optick.h"

#if CB_PROFILING
	#define CB_PROFILING_FUNCTION(...)			OPTICK_EVENT(__VA_ARGS__)
	#define CB_PROFILING_FRAME(...)				OPTICK_FRAME(__VA_ARGS__)
	#include "mmgr/mmgr.h"
#else
	#define CB_PROFILING_FUNCTION(...)			
	#define CB_PROFILING_FRAME(...)	
	#include "mmgr/mmgr.h"
#endif // !CB_PROFILING