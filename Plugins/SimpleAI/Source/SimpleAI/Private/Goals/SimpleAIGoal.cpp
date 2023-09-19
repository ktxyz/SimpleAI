// Copyright @ 2023 Kamil Tokarski


#include "Goals/SimpleAIGoal.h"

#include "SimpleAISubsystem.h"
#include "Agents/SimpleAIAgentInterface.h"

USimpleAIGoal::USimpleAIGoal()
{
	Target = Cast<AActor>(GetOuter());
}

float USimpleAIGoal::GetValue_Implementation(const AActor* Agent) const
{
	return BaseValue;
}

bool USimpleAIGoal::IsSatisfied_Implementation(const AActor* Agent) const
{
	// UE_LOG(LogTemp, Warning, TEXT("IsSatisfied_Implementation: %s"), *this->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("IsSatisfied_Implementation: %s"), *GetDesiredAgentState(Agent).ToString(true));
	// UE_LOG(LogTemp, Warning, TEXT("IsSatisfied_Implementation: %s"), *ISimpleAIAgentInterface::Execute_GetState(Agent).ToString(true));	
	return ISimpleAIAgentInterface::Execute_GetState(Agent) == GetDesiredAgentState(Agent);
}

bool USimpleAIGoal::CanBeSatisfied_Implementation(const AActor* Agent) const
{
	const FSimpleAIState WorldState = GetWorld()->GetSubsystem<USimpleAISubsystem>()->GetWorldState();
	const FSimpleAIState RequiredState = GetRequiredWorldState(Agent);
	// UE_LOG(LogTemp, Warning, TEXT("Goal %s can be satisfied compared:\n%s\n%s"), *GetName(), *WorldState.ToString(true), *RequiredState.ToString(true));
	return GetWorld()->GetSubsystem<USimpleAISubsystem>()->GetWorldState() == GetRequiredWorldState(Agent);
}

bool USimpleAIGoal::IsSatisfiedByWorldState_Implementation(const AActor* Agent, const FSimpleAIState& WorldState) const
{
	const bool bIsValidAgentReq = WorldState == GetDesiredAgentState(Agent);
	return bIsValidAgentReq;
}
