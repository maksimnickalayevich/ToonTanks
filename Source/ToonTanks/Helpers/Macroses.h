#pragma once
#include "Engine/GameEngine.h"
#define ASSET_NOT_FOUND(msg) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT(msg));} 