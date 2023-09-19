// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SimpleAIState.h"

#include "SimpleAILibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEAI_API USimpleAILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	static FSimpleAIState MergeStates(const FSimpleAIState& Main, const FSimpleAIState& Addition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	static bool IsSatisfiedBy(const FSimpleAIState& State, const FSimpleAIState& OtherState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	static FString StateToString(const FSimpleAIState& State, bool bPrintPretty = false);
};
