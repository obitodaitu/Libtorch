// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class LibTorch : ModuleRules
{
    public LibTorch(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        IWYUSupport = IWYUSupport.Full;
        
        bUseRTTI = true;
        bUseUnity = false;
        bUsePrecompiled = false;
        bEnableExceptions = true;

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CUDA"
                
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Projects"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        // 添加系统库依赖
        PublicSystemLibraries.AddRange(new string[] {

            "ntdll.lib"

        });

        
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            PublicDefinitions.AddRange(new[]
            {
                "NO_EXPORT"
            });

            

            var libtorchPath = Path.Combine(ModuleDirectory, "../ThirdParty/libtorch");
            AddIncludeFolders(Path.Combine(libtorchPath, "include"));
            AddIncludeFolders(Path.Combine(libtorchPath, "include/torch/csrc/api/include"));
            LinkLibraryFiles(Path.Combine(libtorchPath, "lib"));


            string CUDAPath = Environment.GetEnvironmentVariable("CUDA_PATH") ?? "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v12.6";

            // CUDA头文件
            AddIncludeFolders(Path.Combine(CUDAPath, "include"));
            LinkLibraryFiles(Path.Combine(CUDAPath, "lib"));
            LinkLibraryFiles(Path.Combine(CUDAPath, "bin"));
            
           

            //PublicAdditionalLibraries.Add("/INCLUDE:\"?warp_size@cuda@at@@YAHXZ\"");

            string torch_cpu_path = Path.Combine(libtorchPath, "lib") + "/torch_cpu.lib";
            PublicAdditionalLibraries.Add($"/WHOLEARCHIVE:{torch_cpu_path}");

            string torch_cuda_path = Path.Combine(libtorchPath, "lib") + "/torch_cuda.lib";
            PublicAdditionalLibraries.Add($"/WHOLEARCHIVE:{torch_cuda_path}");

            string c10_cuda_path = Path.Combine(libtorchPath, "lib") + "/c10_cuda.lib";
            PublicAdditionalLibraries.Add($"/WHOLEARCHIVE:{c10_cuda_path}");

        }
    }


    private void AddIncludeFolders(string includePath)
    {
        PublicIncludePaths.AddRange(new[]
        {
            includePath
        });
    }

    /**
	 * Links included libraries as project dependencies
	 *
	 * @param libraryPath Path to folder with library files
	 */
    private void LinkLibraryFiles(string libraryPath)
    {
        var filePaths = Directory.GetFiles(libraryPath!, "*", SearchOption.AllDirectories);
        foreach (var filePath in filePaths)
        {
            var fileName = Path.GetFileName(filePath);

            if (fileName.EndsWith(".lib"))
            {              
                PublicAdditionalLibraries.Add(filePath);
            }
            else if (fileName.EndsWith(".dll"))
            {
                RuntimeDependencies.Add(filePath);
                PublicDelayLoadDLLs.Add(fileName);
            }
            else
            {
                //EpicGames.Core.Log.TraceInformation("Ignored file {0}", filePath);
            }
        }
    }
}
