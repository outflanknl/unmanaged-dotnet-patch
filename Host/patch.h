#pragma once

#include <Windows.h>
#include "clr.h"

_Assembly* LoadMscorlib(ICorRuntimeHost* corRtHost);
VOID PatchExit(_Assembly* mscorlib);
