#include <Windows.h>
#include "clr.h"

#define APPDOMAIN_NAME L"Example"

ICorRuntimeHost* InitCLR()
{
	ICLRMetaHost* metaHost;
	CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (PVOID*)&metaHost);

	ICLRRuntimeInfo* rtInfo;
	metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (PVOID*)&rtInfo);

	ICorRuntimeHost* corRtHost;
	rtInfo->GetInterface(CLSID_CorRuntimeHost, IID_ICorRuntimeHost, (PVOID*)&corRtHost);
	corRtHost->Start();

	return corRtHost;
}

_MethodInfo* LoadAssembly(ICorRuntimeHost* corRtHost, PBYTE assembly, DWORD assemblyLen)
{
	IUnknownPtr appDomainUnk;
	corRtHost->CreateDomain(APPDOMAIN_NAME, NULL, &appDomainUnk);

	_AppDomain* appDomain;
	appDomainUnk->QueryInterface(IID_PPV_ARGS(&appDomain));

	SAFEARRAYBOUND sb[1] = { 0 };
	sb[0].cElements = assemblyLen;
	sb[0].lLbound = 0;
	SAFEARRAY* safeAssembly = SafeArrayCreate(VT_UI1, 1, sb);

	PVOID accessPtr;
	SafeArrayAccessData(safeAssembly, &accessPtr);
	memcpy(accessPtr, assembly, assemblyLen);
	SafeArrayUnaccessData(safeAssembly);

	_Assembly* loadedAssembly;
	appDomain->Load_3(safeAssembly, &loadedAssembly);

	_MethodInfo* entrypoint;
	loadedAssembly->get_EntryPoint(&entrypoint);

	return entrypoint;
}

VOID ExecuteAssembly(_MethodInfo* entrypoint)
{
	SAFEARRAY* entrypointArgs = SafeArrayCreateVector(VT_EMPTY, 0, 0);
	VARIANT ret = { 0 };
	VARIANT obj = { 0 };
	obj.vt = VT_NULL;

	entrypoint->Invoke_3(obj, entrypointArgs, &ret);
}
