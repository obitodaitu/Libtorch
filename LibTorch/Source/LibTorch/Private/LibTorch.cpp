// Copyright Epic Games, Inc. All Rights Reserved.

#include "LibTorch.h"
#include <Misc/Paths.h>
#include <HAL/PlatformProcess.h>

#include "LibTorchHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FLibTorchModule"


#if PLATFORM_WINDOWS
TArray<TUniquePtr<void, FDllDeleter>> FLibTorchModule::DllHandles;
#endif

#if PLATFORM_WINDOWS
void FDllDeleter::operator()(void* DllHandle) const noexcept
{
	if (DllHandle)
	{
		FPlatformProcess::FreeDllHandle(DllHandle);
	}
}
#endif


void FLibTorchModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString LibraryPath;
	if (!GetLibraryPath(LibraryPath))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Cannot load LibTorch on platform: %s"), *UGameplayStatics::GetPlatformName());
		return;
	}

	const TCHAR* const LibraryPathPointer = *LibraryPath;
	UE_LOG(LogTemp, Warning, TEXT("Loading LibTorch libraries from path: %s"), LibraryPathPointer)

	//FPlatformProcess::PushDllDirectory(LibraryPathPointer);

#if PLATFORM_WINDOWS
	FPlatformProcess::AddDllDirectory(LibraryPathPointer);

	TArray<FString> DllNames;
	IFileManager::Get().FindFilesRecursive(
		DllNames,
		LibraryPathPointer,
		TEXT("*.dll"),
		true,
		false,
		false
	);

	DllHandles.Reserve(DllNames.Num());
	for (const FString& DllName : DllNames)
	{
		FString FullDllName = FPaths::ConvertRelativePathToFull(DllName);
		void* DllHandle = FPlatformProcess::GetDllHandle(*FullDllName);
		if (DllHandle)
		{
			DllHandles.Emplace(DllHandle);  
		}
	}
#endif

	torch::init();
	torch::set_num_threads(10);
	

	UE_LOG(LogTemp, Warning, TEXT("Loaded plugin with LibTorch version %ls!"), TEXT(TORCH_VERSION))

}

void FLibTorchModule::ShutdownModule()
{
	
}

bool FLibTorchModule::GetLibraryPath(FString& OutPath)
{
	const IPlugin* const LibtorchPlugin = IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME).Get();
	if (LibtorchPlugin == nullptr)
		return false;

	OutPath = LibtorchPlugin->GetBaseDir() / TEXT("Source/ThirdParty/libtorch/lib");
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLibTorchModule, LibTorch)


