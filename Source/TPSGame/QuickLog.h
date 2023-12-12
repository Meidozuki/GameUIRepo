#pragma once

#include "Misc/CoreMiscDefines.h"

#define LOGDEBUG(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define LOG(Format, ...) UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__)