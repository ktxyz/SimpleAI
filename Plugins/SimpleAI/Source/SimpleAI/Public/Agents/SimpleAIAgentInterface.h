// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "SimpleAIState.h"
#include "UObject/Interface.h"
#include "SimpleAIAgentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USimpleAIAgentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEAI_API ISimpleAIAgentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Returns current state of the agent
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpleAI")
	FSimpleAIState GetState() const;

	// Returns the agent component
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpleAI")
	class USimpleAIAgentComponent* GetAgentComponent() const;

	// Returns avatar of the agent
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpleAI")
	AActor* GetAvatar() const;

	// Called by action on its update (when finished or failed)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpleAI")
	void OnActionStopped(class USimpleAIAction* Action, ESimpleAIActionStateChangeReason Reason);
	
	// GetRadiusForDistanceReq
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SimpleAI")
	float GetAgentRadius() const;
};
