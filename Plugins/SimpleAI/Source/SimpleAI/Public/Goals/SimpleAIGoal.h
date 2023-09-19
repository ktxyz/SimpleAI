// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "SimpleAIState.h"
#include "SimpleAIVisualDataInterface.h"
#include "UObject/Object.h"
#include "SimpleAIGoal.generated.h"

/**
 *  Base class for all goals (more like activities) that can be pursued by agents
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class SIMPLEAI_API USimpleAIGoal : public UObject
{
	GENERATED_BODY()

public:
	USimpleAIGoal();
	
	// Calculates dynamic value of this goal for an agent
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	float GetValue(const AActor* Agent) const;
	virtual float GetValue_Implementation(const AActor* Agent) const;

	// Checks if this goal is satisfied by the given agent
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	bool IsSatisfied(const AActor* Agent) const;
	virtual bool IsSatisfied_Implementation(const AActor* Agent) const;

	// Checks if this goal can be satisfied by the given agent
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	bool CanBeSatisfied(const AActor* Agent) const;
	virtual bool CanBeSatisfied_Implementation(const AActor* Agent) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	FSimpleAIState GetDesiredAgentState(const AActor* Agent) const;
	virtual FSimpleAIState GetDesiredAgentState_Implementation(const AActor* Agent) const { return DesiredAgentState; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	FSimpleAIState GetRequiredWorldState(const AActor* Agent) const;
	virtual FSimpleAIState GetRequiredWorldState_Implementation(const AActor* Agent) const { return RequiredWorldState; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "SimpleAI")
	bool IsSatisfiedByWorldState(const AActor* Agent, const FSimpleAIState& WorldState) const;
	virtual bool IsSatisfiedByWorldState_Implementation(const AActor* Agent, const FSimpleAIState& WorldState) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	AActor* GetTarget() const { return Target; }

protected:
	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	void SetTarget(AActor* NewTarget) { Target = NewTarget; }
	friend class USimpleAIAgentComponent;
private:
	UPROPERTY()
	AActor* Target = nullptr;
	
protected:
	// Required state for this goal to be valid
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FSimpleAIState RequiredWorldState;

	// Desired state for this goal to be satisfied
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FSimpleAIState DesiredAgentState;
	
	// Base value of this goal
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	float BaseValue = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	bool bIsDistanceLocked = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI", meta = (EditCondition = "bIsDistanceLocked"))
	float DistanceLock = 150.0f;

	// After this time of having satisfied, the agent will stop pursuing this goal
	// Could be either real time or not, I'll handle that in the future
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SimpleAI")
	float MaxHoursSpentAtOnce = 2; 
	
	// Visual data interface
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FText GoalName = FText::FromString("Goal");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FText GoalDescription = FText::FromString("Goal description");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	UTexture2D* GoalIcon = nullptr;
};
