// Copyright @ 2023 Kamil Tokarski


#include "Actions/SimpleAIAction.h"

#include "SimpleAILibrary.h"
#include "SimpleAISubsystem.h"
#include "Agents/SimpleAIAgentComponent.h"
#include "Agents/SimpleAIAgentInterface.h"

USimpleAIAction::USimpleAIAction()
{
	Target = Cast<AActor>(GetOuter());
}

bool USimpleAIAction::IsValidForAgent_Implementation(const AActor* Agent) const
{
	// HOLY .... WHAT HAVE I DONE HERE
	const FSimpleAIState ActorWorldState = GetWorld()->GetSubsystem<USimpleAISubsystem>()->GetWorldState() + ISimpleAIAgentInterface::Execute_GetState(Agent);
	const bool bIsValid = IsAvailableForAgent(Agent) && IsValidForState(ActorWorldState, Agent);

	// UE_LOG(LogTemp, Warning, TEXT("Action %s IsValid %d(DIST: %f REQ: %f)(StateWorld: %s STATE REQ: %s)"), *GetName(), bIsValid, ActorDistanceFromTarget, ActorActionDistanceRequirement, *ActorWorldState.ToString(true), *Conditions.ToString(true));

	return bIsValid;
}

bool USimpleAIAction::IsValidForState_Implementation(const FSimpleAIState& State, const AActor* Agent) const
{
	// UE_LOG(LogTemp, Warning, TEXT("IsValidForState_Implementation: %s"), *State.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("IsValidForState_Implementation: %s"), *Conditions.ToString());
	// if (bIsDistanceLocked)
		// UE_LOG(LogTemp, Warning, TEXT("IsValidForState_Implementation: %f vs %f"), FVector::Dist(State.Location, Target->GetActorLocation()), DistanceThreshold);
	return State == Conditions && (bIsDistanceLocked ? FVector::Dist(State.Location, Target->GetActorLocation()) <= (DistanceThreshold + ISimpleAIAgentInterface::Execute_GetAgentRadius(Agent)) : true);
}

FSimpleAIState USimpleAIAction::SimulateForState_Implementation(const FSimpleAIState& State) const
{
	return State + Effects;
}

bool USimpleAIAction::IsAvailableForAgent_Implementation(const AActor* Agent) const
{
	// I'm not sure how to cleanly solve this without repeating States
	// So this will be per-case implemented, but I'm sure it shouldn't be
	return true;
}

float USimpleAIAction::CalculateCost_Implementation(const FSimpleAIState& State) const
{
	return BaseCost;
}

void USimpleAIAction::Start_Implementation(AActor* Agent)
{
}

void USimpleAIAction::Execute_Implementation(AActor* Agent, float DeltaSeconds)
{
	Finish(Agent);
}

void USimpleAIAction::Finish_Implementation(AActor* Agent)
{
	Cleanup(Agent);
	if (Agent->Implements<USimpleAIAgentInterface>())
	{
		if (const auto Component = ISimpleAIAgentInterface::Execute_GetAgentComponent(Agent))
		{
			Component->MoveToNextAction();
		}
	}
}

void USimpleAIAction::FinishedSuccessfully_Implementation(AActor* Agent)
{
	
}

void USimpleAIAction::OnStart(AActor* Agent)
{
	if (IsValidForAgent(Agent))
		Start(Agent);
}

void USimpleAIAction::Fail_Implementation(AActor* Agent)
{
	Cleanup(Agent);
	if (Agent->Implements<USimpleAIAgentInterface>())
	{
		if (const auto Component = ISimpleAIAgentInterface::Execute_GetAgentComponent(Agent))
		{
			Component->MoveToNextAction();
		}
	}
}

void USimpleAIAction::Cleanup_Implementation(AActor* Agent)
{
}
