#pragma once 

#if PLATFORM_WINDOWS
#pragma push_macro("CONSTEXPR")
#undef CONSTEXPR
#pragma push_macro("check")
#undef check
#pragma push_macro("dynamic_cast")
#undef dynamic_cast
#pragma push_macro("PI")
#undef PI
#pragma push_macro("ensure")
#undef ensure

#pragma warning(push)       
#pragma warning(disable: 4686)  

THIRD_PARTY_INCLUDES_START
//PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
#include <torch/script.h>
#include <torch/torch.h>
//PRAGMA_POP_PLATFORM_DEFAULT_PACKING
THIRD_PARTY_INCLUDES_END

#pragma warning(pop)  

#pragma pop_macro("ensure")
#pragma pop_macro("PI")
#pragma pop_macro("check")
#pragma pop_macro("dynamic_cast")
#pragma pop_macro("CONSTEXPR")
#endif

