// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "SimpleAIState.h"
#include "SimpleAIVisualDataInterface.h"
#include "UObject/Object.h"
#include "SimpleAIAction.generated.h"

// Enum used to track the reason why the action state changed
UENUM(BlueprintType, Blueprintable)
enum class ESimpleAIActionStateChangeReason : uint8
{
	Finished,
	Failed
};

/**
 *  Base class for actions that can be executed by agents
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class SIMPLEAI_API USimpleAIAction : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	AActor* GetTarget() const;
	virtual AActor* GetTarget_Implementation() const { return Target; }
	
public:
	USimpleAIAction();

	UFUNCTION(BlueprintImplementableEvent, Category="SimpleAI")
	void OnInitAction();
	
	// Checks if this action can be executed by the given agent
	// in the current real world state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	bool IsValidForAgent(const AActor* Agent) const;
	virtual bool IsValidForAgent_Implementation(const AActor* Agent) const;
	// Checks if this action can be executed in the given state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	bool IsValidForState(const FSimpleAIState& State, const AActor* Agent) const;
	virtual bool IsValidForState_Implementation(const FSimpleAIState& State, const AActor* Agent) const;

	// Simulates the execution of this action by the given agent
	// and returns the resulting state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	FSimpleAIState SimulateForState(const FSimpleAIState& State) const;
	virtual FSimpleAIState SimulateForState_Implementation(const FSimpleAIState& State) const;

	// Checks if this action can be executed by the given agent
	// in some state he would likely achieve - used to track if the agent
	// can still pursue the goal using this action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	bool IsAvailableForAgent(const AActor* Agent) const;
	virtual bool IsAvailableForAgent_Implementation(const AActor* Agent) const;

	// Calculates the cost of executing this action by the given agent
	// in the given state
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	float CalculateCost(const FSimpleAIState& State) const;
	virtual float CalculateCost_Implementation(const FSimpleAIState& State) const;

	// Starting the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void Start(AActor* Agent);
	virtual void Start_Implementation(AActor* Agent);
	
	// Running the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void Execute(AActor* Agent, float DeltaTime);
	virtual void Execute_Implementation(AActor* Agent, float DeltaTime);

	// Finishing the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void Finish(AActor* Agent);
	virtual void Finish_Implementation(AActor* Agent);

	// This shouldn't exist, its fixing issues that are fixable in other ways
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void FinishedSuccessfully(AActor* Agent);
	virtual void FinishedSuccessfully_Implementation(AActor* Agent);

	UFUNCTION()
	void OnStart(AActor* Agent);
	
	// Failing the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void Fail(AActor* Agent);
	virtual void Fail_Implementation(AActor* Agent);

	// Cleaning up the action
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleAI")
	void Cleanup(AActor* Agent);
	virtual void Cleanup_Implementation(AActor* Agent);
private:
	UPROPERTY()
	AActor* Target = nullptr;

protected:
	// Visual data interface
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FText ActionName = FText::FromString("Action");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FText ActionDescription = FText::FromString("Action description");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	UTexture2D* ActionIcon = nullptr;

	// Condition that must be satisfied for this action to be valid
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FSimpleAIState Conditions;

	// Can this action be executed only if the agent is close enough to the target
	// This is the aspect of this system that I struggled with the most
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	bool bIsDistanceLocked = false;

	// Distance from the target that the agent must be in order to execute this action
	// (NPC radius will be automatically added to this value when checking the distance)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI", meta = (EditCondition = "bIsDistanceLocked"))
	float DistanceThreshold = 150.0f;

	// Base cost of this action
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	float BaseCost = 1.0f;
	
	// Effects that will be applied after this action is executed
	// I had the idea of using dynamic requirement objects
	// but couldn't get it to work without too much hassle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FSimpleAIState Effects;

protected:
	void SetTarget(AActor* NewTarget) { Target = NewTarget; }
	friend class USimpleAIAgentComponent;
};
