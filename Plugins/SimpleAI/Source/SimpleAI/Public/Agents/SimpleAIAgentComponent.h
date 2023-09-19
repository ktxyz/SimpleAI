// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "Actions/SimpleAIAction.h"
#include "Components/ActorComponent.h"
#include "Goals/SimpleAIGoal.h"
#include "SimpleAIAgentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewGoalSetDelegate, USimpleAIGoal*, NewGoal);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEAI_API USimpleAIAgentComponent : public UActorComponent
{
	GENERATED_BODY()

	// This will remove current goal and path and refresh them
	// using subsystem planner
	void RefreshGoalsAndPath();
public:	
	// Sets default values for this component's properties
	USimpleAIAgentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI|Agent")
	TArray<USimpleAIAction*> GetPossibleActions() const { return PossibleActions; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI|Agent")
	TArray<USimpleAIGoal*> GetPossibleGoals() const { return PossibleGoals; }

	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void Debug_UpdateGoalsAndPath() { RefreshGoalsAndPath(); }
	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void MoveToNextAction();

	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void Debug_SetCurrentGoal(USimpleAIGoal* NewGoal) { CurrentGoal = NewGoal; }
	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void Debug_SetCurrentPath(TArray<USimpleAIAction*> NewPath) { CurrentPath = NewPath; MoveToNextAction(); }

	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void AutoManageUpdate(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "SimpleAI|Agent")
	void ForceFailCurrentGoal();

	TArray<USimpleAIGoal*> GetLastFailedGoals() const { return FailedGoalsHistory; }
private:
	// flag indicating if this agent is registered in the subsystem
	bool bIsAgentRegistered = false;

	UPROPERTY()
	TArray<USimpleAIAction*> PossibleActions;

	UPROPERTY()
	TArray<USimpleAIGoal*> PossibleGoals;

	// TODO: this maybe shouldn't be a pure pointer
	// Shared instead? Altought we are only comparing them - not accessing
	UPROPERTY()
	TArray<USimpleAIGoal*> FailedGoalsHistory;

	float LastGoalUpdateTimePassed = 0.0f;
protected:
	// Actions that can be performed on this agent
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI|Target")
	TArray<TSubclassOf<USimpleAIAction>> PossibleActionClasses;

	// Goals that are advertised by this agent for others to pursue
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI|Target")
	TArray<TSubclassOf<USimpleAIGoal>> PossibleGoalClasses;

	// Currently selected goal
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "SimpleAI|Agent")
	USimpleAIGoal* CurrentGoal = nullptr;

	// Current path taken for this goal
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "SimpleAI|Agent")
	TArray<USimpleAIAction*> CurrentPath;

	// Active action being executed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "SimpleAI|Agent")
	USimpleAIAction* ActiveAction = nullptr;

	// How frequently we should update our goal
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI|Agent")
	float GoalUpdateFrequency = 0.5f;

	UPROPERTY(BlueprintAssignable, Category = "SimpleAI|Agent")
	FOnNewGoalSetDelegate OnNewGoalSet;
};
