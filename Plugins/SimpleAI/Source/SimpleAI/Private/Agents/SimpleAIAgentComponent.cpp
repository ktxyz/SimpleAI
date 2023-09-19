// Copyright @ 2023 Kamil Tokarski


#include "Agents/SimpleAIAgentComponent.h"

#include "SimpleAISubsystem.h"

void USimpleAIAgentComponent::RefreshGoalsAndPath()
{
	if (!IsActive() || !bIsAgentRegistered)
		return;
	
	// Cleanup
	if (ActiveAction)
	{
		if ((!CurrentGoal || !CurrentGoal->IsSatisfied(GetOwner())))
		{
			ActiveAction->Fail(GetOwner());
			// // UE_LOG(LogTemp, Warning, TEXT("Action FAILED BY REFRESH"));
		} else
		{
			ActiveAction->Finish(GetOwner());
			// // UE_LOG(LogTemp, Warning, TEXT("Action Finisehd BY REFRESH"));
		}
	}
	ActiveAction = nullptr;
	CurrentGoal = nullptr;
	CurrentPath.Empty();

	// To save the tick if there are no goals
	if (LastGoalUpdateTimePassed < GoalUpdateFrequency)
		return;
	LastGoalUpdateTimePassed = 0.0f;

	if (const FSimpleAIGoalPathQueryResult QueryResult =
									GetWorld()->GetSubsystem<USimpleAISubsystem>()->FindRandomGoalPath(GetOwner());
		QueryResult.bIsSuccessful)
	{
		CurrentGoal = QueryResult.Goal;
		CurrentPath = QueryResult.Path;
		MoveToNextAction();
		OnNewGoalSet.Broadcast(CurrentGoal); // I should have used more delegates :(
	} else
	{
		FailedGoalsHistory.Empty();
	}
}

// Sets default values for this component's properties
USimpleAIAgentComponent::USimpleAIAgentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USimpleAIAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	// Spawn all possible actions
	for (auto &ActionClass : PossibleActionClasses)
	{
		if (!ActionClass)
			continue;
		if (USimpleAIAction* Action = NewObject<USimpleAIAction>(GetOwner(), ActionClass))
		{
			Action->SetTarget(GetOwner());
			PossibleActions.Add(Action);

			Action->OnInitAction();
		}
	}

	for (auto &GoalClass : PossibleGoalClasses)
	{
		if (!GoalClass)
			continue;
		if (USimpleAIGoal* Goal = NewObject<USimpleAIGoal>(GetOwner(), GoalClass))
		{
			Goal->SetTarget(GetOwner());
			PossibleGoals.Add(Goal);
		}
	}
	
	if (!bIsAgentRegistered)
	{
		GetWorld()->GetSubsystem<USimpleAISubsystem>()->RegisterAgent(GetOwner());
		bIsAgentRegistered = true;
	}
}

void USimpleAIAgentComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
}

void USimpleAIAgentComponent::Deactivate()
{	
	Super::Deactivate();
}

void USimpleAIAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (bIsAgentRegistered)
	{
		GetWorld()->GetSubsystem<USimpleAISubsystem>()->UnregisterAgent(GetOwner());
		bIsAgentRegistered = false;
	}
}


// Called every frame
void USimpleAIAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive() || !bIsAgentRegistered)
		return;
}


void USimpleAIAgentComponent::MoveToNextAction()
{
	if (!IsActive() || !bIsAgentRegistered)
		return;

	if (CurrentPath.IsEmpty())
		return;

	ActiveAction = CurrentPath.Pop();
	if (ActiveAction && ActiveAction->IsValidForAgent(GetOwner()))
		ActiveAction->OnStart(GetOwner()); // There's some timing problem i think
												// between BPs and Native Code
	else if (ActiveAction)
	{
		// // UE_LOG(LogTemp, Warning, TEXT("Action FAILED On Start"));
		ActiveAction->Fail(GetOwner());
	}
}

void USimpleAIAgentComponent::AutoManageUpdate(float DeltaTime)
{
	// // UE_LOG(LogTemp, Warning, TEXT("AutoManageUpdate for %s"), *GetOwner()->GetName());
	if (!IsActive() || !bIsAgentRegistered)
	{
		// // // UE_LOG(LogTemp, Warning, TEXT("Agent(%s) is not active or registered"), *GetOwner()->GetName());
		return;
	}

	LastGoalUpdateTimePassed += DeltaTime;

	
	if (!CurrentGoal || !CurrentGoal->CanBeSatisfied(GetOwner()))
	{
			// // UE_LOG(LogTemp, Warning, TEXT("Current goal is %s"), *CurrentGoal->GetName());
		// // UE_LOG(LogTemp, Warning, TEXT("Current goal is not satisfied and cant be satisfied, refreshing goals and path"));
		if (CurrentGoal)
			FailedGoalsHistory.Add(CurrentGoal);
		RefreshGoalsAndPath();
		return;
	}

	if (CurrentGoal->IsSatisfied(GetOwner()))
	{
		// // UE_LOG(LogTemp, Warning, TEXT("State satisfied, no need to do stuff"));
		return;
	}
	
	// // UE_LOG(LogTemp, Warning, TEXT("State not satisfied, need to do stuff"));

	// DAMN, I'm blind
	if (!ActiveAction && CurrentPath.IsEmpty())
	{
		FailedGoalsHistory.Add(CurrentGoal);
		RefreshGoalsAndPath();
		return;

		if (const FSimpleAIGoalPathQueryResult QueryResult =
										GetWorld()->GetSubsystem<USimpleAISubsystem>()->FindPathToGoal(GetOwner(), CurrentGoal);
			QueryResult.bIsSuccessful)
		{
			CurrentGoal = QueryResult.Goal;
			CurrentPath = QueryResult.Path;
			MoveToNextAction();
		}
	}

	if (!ActiveAction)
	{
		if (!CurrentGoal->IsSatisfied(GetOwner()))
		{
			// // UE_LOG(LogTemp, Warning, TEXT("State is refreshed because goal is not satisfied"));
			// // // UE_LOG(LogTemp, Warning, TEXT("No active action, no current goal or current goal is not satisfied"));
			RefreshGoalsAndPath();
			FailedGoalsHistory.Add(CurrentGoal);
		} 
		return;
	}

	if (ActiveAction->IsValidForAgent(GetOwner()))
	{
		// // // UE_LOG(LogTemp, Warning, TEXT("Active action is valid for agent(%s %s)"), *ActiveAction->GetName(), *GetOwner()->GetName());
		ActiveAction->Execute(GetOwner(), DeltaTime);
	}
	else
	{
		// // // UE_LOG(LogTemp, Warning, TEXT("Active action is not valid for agent(%s %s)"), *ActiveAction->GetName(), *GetOwner()->GetName());
		// // UE_LOG(LogTemp, Warning, TEXT("State is refreshed because action(%s) is not valid"), *ActiveAction->GetName());
		RefreshGoalsAndPath();
	}
}

void USimpleAIAgentComponent::ForceFailCurrentGoal()
{
	FailedGoalsHistory.Add(CurrentGoal);
	CurrentGoal = nullptr;
	RefreshGoalsAndPath();
}

