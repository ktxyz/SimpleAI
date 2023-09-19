// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleAIVisualDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class USimpleAIVisualDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEAI_API ISimpleAIVisualDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	virtual FText GetVisualName() = 0;
	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	virtual FText GetVisualDescription() = 0;
	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	virtual UTexture2D* GetVisualIcon() = 0;
};

// Something not worked, no time to check