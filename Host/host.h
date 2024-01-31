#pragma once

#include <Windows.h>
#include "clr.h"

ICorRuntimeHost* InitCLR();
_MethodInfo* LoadAssembly(ICorRuntimeHost* corRtHost, PBYTE assembly, DWORD assemblyLen);
VOID ExecuteAssembly(_MethodInfo* entrypoint);
