// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "SimpleClockSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleClockLib.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Category = "SimpleClock")
class SIMPLECLOCKSYSTEM_API USimpleClockLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleClock")
	static int HoursPassed(const FWorldTimeStruct& TimeStart, const FWorldTimeStruct& TimeEnd);
};
