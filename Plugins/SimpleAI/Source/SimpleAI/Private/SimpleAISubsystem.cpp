// Copyright @ 2023 Kamil Tokarski


#include "SimpleAISubsystem.h"

#include "Agents/SimpleAIAgentComponent.h"
#include "Agents/SimpleAIAgentInterface.h"
#include "Algo/RandomShuffle.h"

void USimpleAISubsystem::RegisterAgent(AActor* Agent)
{
	if (!Agent->Implements<USimpleAIAgentInterface>())
		return;
	
	const auto Component = ISimpleAIAgentInterface::Execute_GetAgentComponent(Agent); 
	if (!Component)
		return;

	// // UE_LOG(LogTemp, Warning, TEXT("RegisterAgent %s: %d actions %d goals"), *Agent->GetName(),
										// Component->GetPossibleActions().Num(), Component->GetPossibleGoals().Num());
	RegisteredAgents.Add(Agent);
	for (auto &Action : Component->GetPossibleActions())
	{
		if (!Action || !Action->GetTarget())
			continue;
		// // UE_LOG(LogTemp, Warning, TEXT("\t%s"), *Action->GetName());
		RegisteredActions.AddUnique(Action);
	}
	for (auto &Goal : Component->GetPossibleGoals())
	{
		if (!Goal || !Goal->GetTarget())
			continue;
		// // UE_LOG(LogTemp, Warning, TEXT("\t%s"), *Goal->GetName());
		RegisteredGoals.AddUnique(Goal);
	}
}

void USimpleAISubsystem::UnregisterAgent(AActor* Agent)
{
	if (!Agent->Implements<USimpleAIAgentInterface>())
		return;
	
	const auto Component = ISimpleAIAgentInterface::Execute_GetAgentComponent(Agent); 
	if (!Component)
		return;
	
	RegisteredAgents.Remove(Agent);
	RegisteredActions.RemoveAll([&](USimpleAIAction* Action) { return Component->GetPossibleActions().Contains(Action); });
	RegisteredGoals.RemoveAll([&](USimpleAIGoal* Goal) { return Component->GetPossibleGoals().Contains(Goal); });
}

void USimpleAISubsystem::UpdateWorldState(const FSimpleAIState& NewState)
{
	WorldState = WorldState + NewState;
}

FSimpleAIState USimpleAISubsystem::GetWorldState() const
{
	return WorldState;
}

// float USimpleAISubsystem::FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal, TArray<FSimpleAIActionPathNode>& Nodes,
// 	float Cost, float Bound) const
// {
// 	FSimpleAIActionPathNode CurrentNode = Nodes.Last();
// 	// // UE_LOG(LogTemp, Warning, TEXT("FindPathToGoal for %s: Goal(%s), Cost(%f), Bound(%f)"), *Agent->GetName(), *Goal->GetName(), Cost, Bound);
// 	// // UE_LOG(LogTemp, Warning, TEXT("\tCurrentNode:\n%s"), *CurrentNode.WorldState.ToString());
// 	
// 	if (CurrentNode.WorldState == Goal->GetDesiredAgentState(Agent))
// 	{
// 		// // UE_LOG(LogTemp, Warning, TEXT("\t\tFound goal"));
// 		// // UE_LOG(LogTemp, Warning, TEXT("\t\tState %s"), *CurrentNode.WorldState.ToString());
// 		// // UE_LOG(LogTemp, Warning, TEXT("\t\tSatisfied %s"), *Goal->GetDesiredAgentState(Agent).ToString());
// 		return 0.0f;
// 	}
//
// 	CurrentNode.Cost_H = CurrentNode.WorldState.HeuristicDistance(Goal->GetDesiredAgentState(Agent));
// 	CurrentNode.Cost_G = Cost;
// 	CurrentNode.Cost_F = Cost + CurrentNode.Cost_H;
//
// 	if (CurrentNode.Cost_F > Bound)
// 		return CurrentNode.Cost_F;
//
// 	float Min = FLT_MAX;
// 	TArray<USimpleAIAction*> ValidActions = GetValidActionsForState(CurrentNode.WorldState, Agent);
// 	// // UE_LOG(LogTemp, Warning, TEXT("\tValid actions: %d"), ValidActions.Num());
// 	for (auto &Action : ValidActions)
// 	{
// 		// // UE_LOG(LogTemp, Warning, TEXT("\t\t%s"), *Action->GetName());
// 	}
// 	for (auto Action : ValidActions)
// 	{
// 			FSimpleAIState NewState = CurrentNode.WorldState + Action->SimulateForState(CurrentNode.WorldState);
// 			if (FSimpleAIActionPathNode NewNode = FSimpleAIActionPathNode(NewState, Action, CurrentNode.Hash); !Nodes.Contains(NewNode))
// 			{
// 				Nodes.Add(NewNode);
// 				float NewCost = FindPathToGoal(Agent, Goal, Nodes, Cost + CurrentNode.Cost_G, Bound);
// 				if (NewCost == 0.0f)
// 					return 0.0f;
// 				if (NewCost < Min)
// 					Min = NewCost;
// 				Nodes.Pop();
// 			}
// 	}
// 	
// 	return Min;
// }

FSimpleAIGoalPathQueryResult USimpleAISubsystem::FindRandomGoalPath(const AActor* Agent) const
{
	TArray<USimpleAIGoal*> RandomizedGoals = RegisteredGoals;
	if (!Agent->Implements<USimpleAIAgentInterface>())
		return FSimpleAIGoalPathQueryResult();
	const auto Component = ISimpleAIAgentInterface::Execute_GetAgentComponent(Agent);
	if (!Component)
		return FSimpleAIGoalPathQueryResult();
	
	Algo::RandomShuffle(RandomizedGoals);
	auto BlockedGoals = Component->GetLastFailedGoals();
	for (const auto Goal : RandomizedGoals)
	{
		if (Goal == nullptr) // Why are there null goals?
			continue;
		// if (Goal == Component->GetLastFailedGoal())
			// continue; // We skip this goal if it was the last failed one
		if (BlockedGoals.Contains(Goal))
			continue;
		if (!Goal->CanBeSatisfied(Agent))
			continue;
		
		if (auto Result = FindPathToGoal(Agent, Goal); Result.bIsSuccessful)
			return Result;
	}
	
	return FSimpleAIGoalPathQueryResult();
}

TArray<USimpleAIAction*> USimpleAISubsystem::GetValidActionsForState(const FSimpleAIState& State, const AActor* Agent) const
{
	// UE_LOG(LogTemp, Warning, TEXT("GetValidActionsForState %s"), *State.ToString(true));
	TArray<USimpleAIAction*> ValidActions;

	for (auto &Action : RegisteredActions)
	{
		// UE_LOG(LogTemp, Warning, TEXT("\t%s"), *Action->GetName());
		if (Action->IsValidForState(State, Agent) && Action->IsAvailableForAgent(Agent))
		{
			// UE_LOG(LogTemp, Warning, TEXT("\t\tValid"));
			ValidActions.Add(Action);
		} else
		{
			// UE_LOG(LogTemp, Warning, TEXT("\t\tinvalid"));
		}
	}

	Algo::RandomShuffle(ValidActions);
	return ValidActions;
}

USimpleAIGoal* USimpleAISubsystem::Debug_FindRandomGoal(const AActor* Agent) const
{
	// // UE_LOG(LogTemp, Warning, TEXT("Debug_FindRandomGoal for %s"), *Agent->GetName());
	
	if (RegisteredGoals.Num() > 0)
	{
		// // UE_LOG(LogTemp, Warning, TEXT("\tFound %d possible goals"), RegisteredGoals.Num());
		auto Goal = RegisteredGoals[FMath::RandRange(0, RegisteredGoals.Num() - 1)];
		// // UE_LOG(LogTemp, Warning, TEXT("\t\t%s"), *Goal->GetName());
		return Goal;
	}
	
	return nullptr;
}

// Dijkstra instead of A*
// cause i coukldnt get A* to work with my awful of actions and states
float USimpleAISubsystem::FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal,
	TArray<FSimpleAIActionPathNode>& Nodes) const
{
	TArray<FSimpleAIActionPathNode> OpenList;
	TMap<uint32, FSimpleAIActionPathNode> ClosedListReverse;
	TMap<uint32, bool> ClosedList;

	// Add start node to open list
	FSimpleAIState StartState = GetWorldState() + ISimpleAIAgentInterface::Execute_GetState(Agent);
	FSimpleAIState GoalState = Goal->GetDesiredAgentState(Agent);

	FSimpleAIActionPathNode StartNode = FSimpleAIActionPathNode(StartState, nullptr, -1);
	StartNode.Distance = StartState.HeuristicDistance(GoalState);
	StartNode.Depth = 0;
	
	OpenList.HeapPush(StartNode);
	while (OpenList.Num() > 0) {
		FSimpleAIActionPathNode CurrentNode; OpenList.HeapPop(CurrentNode);
		// UE_LOG(LogTemp, Warning, TEXT("\t\tCurrentNode: %s"), *CurrentNode.WorldState.ToString(true));
		// UE_LOG(LogTemp, Warning, TEXT("\t\tCurrentNode: %d %d"), CurrentNode.Distance, CurrentNode.Cost);

		if (CurrentNode.WorldState == Goal->GetDesiredAgentState(Agent))
		{
			// UE_LOG(LogTemp, Warning, TEXT("\t\t\tFound goal"));
			if (true || Goal->IsSatisfiedByWorldState(Agent, CurrentNode.WorldState)) {
				// Found goal, gather path
				Nodes.Push(CurrentNode);
				while (CurrentNode.ParentHash != -1)
				{
					CurrentNode = ClosedListReverse[CurrentNode.ParentHash];
					Nodes.Push(CurrentNode);
				}
				return CurrentNode.Cost;
			}
		} 
		
		if (ClosedList.Contains(CurrentNode.Hash))
			continue;

		ClosedList.Add(CurrentNode.Hash, true);
		ClosedListReverse.Add(CurrentNode.Hash, CurrentNode);

		auto ValidActions = GetValidActionsForState(CurrentNode.WorldState, Agent);
		// UE_LOG(LogTemp, Warning, TEXT("\t\t\tValid actions: %d"), ValidActions.Num());
		for (auto &Action : ValidActions)
		{
			// UE_LOG(LogTemp, Warning, TEXT("\t\t\tChecking neighbour\n %s"), *Action->GetName());
			FSimpleAIState NewState = CurrentNode.WorldState + Action->SimulateForState(CurrentNode.WorldState);
			FSimpleAIActionPathNode NewNode = FSimpleAIActionPathNode(NewState, Action, CurrentNode.Hash);
			NewNode.Distance = NewState.HeuristicDistance(GoalState);
			NewNode.Cost = CurrentNode.Cost + Action->CalculateCost(CurrentNode.WorldState);
			NewNode.Depth = CurrentNode.Depth + 1;
			// UE_LOG(LogTemp, Warning, TEXT("\t\t\t\tNeighbour: \n%s"), *NewNode.WorldState.ToString(true));

			
			if (NewState == Goal->GetDesiredAgentState(Agent))
			{
				// UE_LOG(LogTemp, Warning, TEXT("\t\t\t\tNeighbour is solution (WHY IS THIS HAPPENING)?"));
				OpenList.HeapPush(NewNode);
			} else if (ClosedList.Contains(NewNode.Hash))
			{
				// UE_LOG(LogTemp, Warning, TEXT("\t\t\t\tNeighbour Contained"));
				continue;
			}

			// TODO: Give this to options
			if (NewNode.Depth <= 10)
			{
				// UE_LOG(LogTemp, Warning, TEXT("\t\t\t\tNeighbour added"));
				OpenList.HeapPush(NewNode);
			} else
			{
				// UE_LOG(LogTemp, Warning, TEXT("\t\t\t\tNeighbour not added because depth is too high: %d"), NewNode.Depth);
			}
		}
	}

	return FLT_MAX;
}

FSimpleAIGoalPathQueryResult USimpleAISubsystem::FindPathToGoal(const AActor* Agent, USimpleAIGoal* Goal) const
{
	if (!Agent || !Goal || !Agent->Implements<USimpleAIAgentInterface>())
	{
		// // UE_LOG(LogTemp, Warning, TEXT("Debug_FindPathToGoal invalid call"));
		return FSimpleAIGoalPathQueryResult();	
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("Debug_FindPathToGoal for %s(%s)"), *Agent->GetName(), *Goal->GetName());

	FSimpleAIGoalPathQueryResult Result;
	
	TArray<USimpleAIAction*> Path;
	const FSimpleAIState GoalState = Goal->GetDesiredAgentState(Agent);
	const FSimpleAIState CurrentState = GetWorldState() + ISimpleAIAgentInterface::Execute_GetState(Agent);

	TArray<FSimpleAIActionPathNode> Nodes;

	if (FindPathToGoal(Agent, Goal, Nodes) < FLT_MAX)
	{
		// // UE_LOG(LogTemp, Warning, TEXT("\tFound path with %d nodes"), Nodes.Num());
		for (auto &Node : Nodes)
		{
			if (Node.Action)
			{
				// // UE_LOG(LogTemp, Warning, TEXT("\t\t%s"), *Node.Action->GetName());
				Path.Add(Node.Action);
			} else
			{
				// // UE_LOG(LogTemp, Warning, TEXT("\t\tNode with no action"));
			}
		}

		Result.bIsSuccessful = true;
		Result.Goal = Goal;

		Result.Path = Path;
	} else {			
		Path.Empty();
	}

	// UE_LOG(LogTemp, Warning, TEXT("\tFound path with %d nodes"), Nodes.Num());
	return Result;
}
