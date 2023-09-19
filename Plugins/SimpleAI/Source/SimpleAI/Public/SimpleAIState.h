// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleAIState.generated.h"

/*
 * State is a set of boolean values assigned to names. Also a location since I dont know how to handle it differently
 */
USTRUCT(BlueprintType, Blueprintable)
struct SIMPLEAI_API FSimpleAIState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	TMap<FName, bool> Atoms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	bool bIsLocationSet = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
	FVector Location;

	FSimpleAIState() : Atoms(TMap<FName, bool>()), Location(FVector::ZeroVector) {}
	FSimpleAIState(const TMap<FName, bool>& Atoms, const FVector& Location) : Atoms(Atoms), Location(Location) {}

	// Merging two states into new one
	FSimpleAIState operator+(const FSimpleAIState& Other) const;
	void operator+=(const FSimpleAIState& Other);

	FString ToString(bool bPrintPretty = false) const;

	// Satisfaction check
	// Returns true if all atoms in the other state are satisfied in this state
	bool operator==(const FSimpleAIState& Other) const;

	// Updating the state location
	void UpdateLocation(const FVector& NewLocation);

	uint32 GetHash() const
	{
		uint32 HashValue = 0;

		for (auto &Elem : Atoms)
		{
			HashValue ^= GetTypeHash(Elem.Key) ^ GetTypeHash(Elem.Value);
		}
		
		if (bIsLocationSet)
		{
			return HashValue ^ GetTypeHash(Location);
		}
	
		return HashValue;
	}

	int HeuristicDistance(const FSimpleAIState& Other) const;
};

// Hash function for the state
FORCEINLINE uint32 GetTypeHash(const FSimpleAIState& State)
{
	return State.GetHash();
}


// /*
//  * Enum allows specifying the type of state we want to access as preconditions or effects
//  */
// UENUM(BlueprintType, Blueprintable)
// enum class ESimpleAIStateType : uint8
// {
// 	World, // World state is a state of the world that is not affected by the agents actions
// 	Agent, // Agent state is a state of the agent that is affected by the agents actions
// 	Target // Target state is a state of the target that is affected by the agents actions
// };
//
//
// /* Condition is a state that must be satisfied for the action to be valid */
// UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
// class USimpleAIStateCondition : public UObject
// {
// 	GENERATED_BODY()
//
// public:
// 	bool IsSatisfied(const FSimpleAIState& CheckedState) const;
// 	FSimpleAIState GetState() const { return State; }
//
// protected:
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
// 	FSimpleAIState State;
//
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SimpleAI")
// 	ESimpleAIStateType StateType;
// };
