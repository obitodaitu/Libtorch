// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#if PLATFORM_WINDOWS
/**
 * Structure which deals with releasing the DLL handle pointers
 */
struct FDllDeleter
{
	/**
	 * Unlinks a DLL handle cleanly as soon as the pointer is deleted
	 *
	 * @param DllHandle Pointer to DLL handle
	 */
	void operator()(void* DllHandle) const noexcept;
};
#endif

class FLibTorchModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	bool GetLibraryPath(FString& OutPath);

#if PLATFORM_WINDOWS
	/**
	 * Collection of all DLL handles as smart pointers
	 */
	static TArray<TUniquePtr<void, FDllDeleter>> DllHandles;
#endif
};
