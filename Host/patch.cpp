#include <Windows.h>
#include "clr.h"

VOID patchBytes(PVOID target, LPCBYTE patch, SIZE_T patchLen = 1)
{
	DWORD oldProt = 0;
	VirtualProtect(target, patchLen, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy(target, patch, patchLen);
	VirtualProtect(target, patchLen, oldProt, &oldProt);
}

_Assembly* LoadMscorlib(ICorRuntimeHost* corRtHost)
{

	IUnknownPtr appDomainUnk;
	corRtHost->GetDefaultDomain(&appDomainUnk);

	_AppDomain* appDomain;
	appDomainUnk->QueryInterface(IID_PPV_ARGS(&appDomain));

	_Assembly* mscorlib;
	appDomain->Load_2(SysAllocString(L"mscorlib, Version=4.0.0.0"), &mscorlib);

	return mscorlib;
}

VOID PatchExit(_Assembly* mscorlib)
{
	_Type* exitClass;
	mscorlib->GetType_2(SysAllocString(L"System.Environment"), &exitClass);

	_MethodInfo* exitInfo;
	BindingFlags exitFlags = (BindingFlags)(BindingFlags_Public | BindingFlags_Static);
	exitClass->GetMethod_2(SysAllocString(L"Exit"), exitFlags, &exitInfo);

	_Type* methodInfoClass;
	mscorlib->GetType_2(SysAllocString(L"System.Reflection.MethodInfo"), &methodInfoClass);

	_PropertyInfo* methodHandleProperty;
	BindingFlags methodHandleFlags = (BindingFlags)(BindingFlags_Instance | BindingFlags_Public);
	methodInfoClass->GetProperty(SysAllocString(L"MethodHandle"), methodHandleFlags, &methodHandleProperty);

	VARIANT methodHandlePtr = { 0 };
	methodHandlePtr.vt = VT_UNKNOWN;
	methodHandlePtr.punkVal = exitInfo;

	SAFEARRAY* methodHandleArgs = SafeArrayCreateVector(VT_EMPTY, 0, 0);
	VARIANT methodHandleVal = { 0 };
	methodHandleProperty->GetValue(methodHandlePtr, methodHandleArgs, &methodHandleVal);

	_Type* rtMethodHandleType;
	mscorlib->GetType_2(SysAllocString(L"System.RuntimeMethodHandle"), &rtMethodHandleType);

	_MethodInfo* getFuncPtrMethodInfo;
	BindingFlags getFuncPtrFlags = (BindingFlags)(BindingFlags_Public | BindingFlags_Instance);
	rtMethodHandleType->GetMethod_2(SysAllocString(L"GetFunctionPointer"), getFuncPtrFlags, &getFuncPtrMethodInfo);

	SAFEARRAY* getFuncPtrArgs = SafeArrayCreateVector(VT_EMPTY, 0, 0);
	VARIANT exitPtr = { 0 };
	getFuncPtrMethodInfo->Invoke_3(methodHandleVal, getFuncPtrArgs, &exitPtr);

	printf("[U] Exit function pointer: 0x%p\n", exitPtr.byref);
	BYTE patch = 0xC3;
	patchBytes(exitPtr.byref, &patch);
}
