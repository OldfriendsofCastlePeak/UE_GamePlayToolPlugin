// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonGamePlayToolPlugin.h"

DEFINE_LOG_CATEGORY(FCommonGamePlayLog);

#define LOCTEXT_NAMESPACE "FCommonGamePlayToolPluginModule"

void FCommonGamePlayToolPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCommonGamePlayToolPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonGamePlayToolPluginModule, CommonGamePlayToolPlugin)