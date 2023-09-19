// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "SimpleAIState.h"
#include "Actions/SimpleAIAction.h"
#include "Subsystems/WorldSubsystem.h"
#include "SimpleAISubsystem.generated.h"

USTRUCT()
struct FSimpleAIActionPathNode
{
	GENERATED_BODY()

	UPROPERTY()
	FSimpleAIState WorldState;

	UPROPERTY()
	USimpleAIAction* Action;

	uint32 Hash;
	uint32 ParentHash;

	// Heuristic values
	float Cost_H = FLT_MIN;
	float Cost_G = 0;
	float Cost_F = 0;

	// Deterministic
	int Distance;
	int Cost;
	int Depth;

	FSimpleAIActionPathNode() : WorldState(FSimpleAIState()), Action(nullptr), Hash(-1), ParentHash(-1), Cost_H(FLT_MIN), Cost_G(0), Cost_F(0), Distance(INT_MAX), Cost(INT_MAX), Depth(INT_MAX) {}

	FSimpleAIActionPathNode(const FSimpleAIState& WorldState, USimpleAIAction* Action, uint32 ParentHash) : WorldState(WorldState), Action(Action), ParentHash(ParentHash)
	{
		Hash = WorldState.GetHash();
		Depth = 0; // HOLY F... I FOUGHT AGAINST THIS FOR COUPLE HOURS
	}

	void RecalcHash(int D, int C, int De)
	{
		Distance = D;
		Cost = C;
		this->Depth = De;
		Hash = WorldState.GetHash() ^ GetTypeHash(Distance) ^ GetTypeHash(Cost);
	}
	
	bool operator==(const FSimpleAIActionPathNode& Other) const
	{
		return Hash == Other.Hash;
	}

	bool operator!=(const FSimpleAIActionPathNode& Other) const
	{
		return Hash != Other.Hash;
	}

	bool operator<(const FSimpleAIActionPathNode& Other) const
	{
		if (Distance == Other.Distance && Cost == Other.Cost)
			return Depth < Other.Depth;
		if (Distance == Other.Distance)
			return Cost < Other.Cost;
		return Distance < Other.Distance;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FSimpleAIGoalPathQueryResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SimpleAI")
	bool bIsSuccessful = false;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleAI")
	USimpleAIGoal* Goal;
	
	UPROPERTY(BlueprintReadWrite, Category = "SimpleAI")
	TArray<USimpleAIAction*> Path;

	FSimpleAIGoalPathQueryResult() : bIsSuccessful(false), Goal(nullptr), Path(TArray<USimpleAIAction*>()) {}
};

/**
 * 
 */
UCLASS()
class SIMPLEAI_API USimpleAISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	void RegisterAgent(AActor* Agent);

	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	void UnregisterAgent(AActor* Agent);

	UFUNCTION(BlueprintCallable, Category = "SimpleAI")
	void UpdateWorldState(const FSimpleAIState& NewState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	FSimpleAIState GetWorldState() const;

	FSimpleAIGoalPathQueryResult FindRandomGoalPath(const AActor* Agent) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	TArray<USimpleAIAction*> GetValidActionsForState(const FSimpleAIState& State, const AActor* Agent) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	USimpleAIGoal* Debug_FindRandomGoal(const AActor* Agent) const;

	// Actual A* implementation
	float FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal, TArray<FSimpleAIActionPathNode>& Nodes) const;
	// float FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal, TArray<FSimpleAIActionPathNode>& Nodes, float Cost, float Bound) const;

	// Helper function for finding path to goal
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleAI")
	FSimpleAIGoalPathQueryResult FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal) const;
private:
	// Current world state
	FSimpleAIState WorldState;

	// List of all registered agents
	UPROPERTY()
	TArray<AActor*> RegisteredAgents;

	UPROPERTY()
	TArray<USimpleAIAction*> RegisteredActions;

	UPROPERTY()
	TArray<USimpleAIGoal*> RegisteredGoals;
};
