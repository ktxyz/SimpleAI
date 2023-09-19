// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleClockSystem.h"

#include "ISettingsModule.h"
#include "SimpleClockConfig.h"

#define LOCTEXT_NAMESPACE "FSimpleClockSystemModule"

void FSimpleClockSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register the settings
		SettingsModule->RegisterSettings("Project", "Plugins", "SimpleClockSystem",
			LOCTEXT("RuntimeSettingsName", "SimpleClockSystem"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the SimpleClockSystem plugin"),
			GetMutableDefault<USimpleClockConfig>()
		);
	}
}

void FSimpleClockSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleClockSystemModule, SimpleClockSystem)