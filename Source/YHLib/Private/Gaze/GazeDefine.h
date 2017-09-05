#pragma once
#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"

#ifndef USE_NEW_INPUT_SYSTEM
#if ENGINE_MAJOR_VERSION ==4 && ENGINE_MINOR_VERSION >=13
#define USE_NEW_INPUT_SYSTEM 1
#else
#define USE_NEW_INPUT_SYSTEM 0
#endif 
#endif //USE_NEW_INPUT_SYSTEM

//ENGINE_VERSION_EG_412
#ifndef ENGINE_VERSION_EG_412
#if ENGINE_MAJOR_VERSION ==4 && ENGINE_MINOR_VERSION >=12
#define ENGINE_VERSION_EG_412 1
#else
#define ENGINE_VERSION_EG_412 0
#endif //ENGINE_VERSION_EG_412
#endif