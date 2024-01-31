#include <Windows.h>
#include "clr.h"
#include "host.h"
#include "patch.h"

#define ASSEMBLY_PATH L"..\\bin\\ExampleAssembly.exe"

PBYTE readAssembly(PDWORD assemblyLen)
{
	HANDLE hFile = CreateFile(ASSEMBLY_PATH, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	*assemblyLen = GetFileSize(hFile, NULL);

	PBYTE assembly = (PBYTE)malloc(*assemblyLen);
	DWORD read = 0;
	ReadFile(hFile, assembly, *assemblyLen, &read, NULL);

	return assembly;
}

int main()
{
	DWORD assemblyLen = 0;
	PBYTE assembly = readAssembly(&assemblyLen);

	ICorRuntimeHost* corRtHost = InitCLR();
	_MethodInfo* entrypoint = LoadAssembly(corRtHost, assembly, assemblyLen);

	_Assembly* mscorlib = LoadMscorlib(corRtHost);
	PatchExit(mscorlib);
	ExecuteAssembly(entrypoint);

	return 0;
}
