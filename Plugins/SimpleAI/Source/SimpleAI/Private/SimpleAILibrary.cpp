// Copyright @ 2023 Kamil Tokarski


#include "SimpleAILibrary.h"

FSimpleAIState USimpleAILibrary::MergeStates(const FSimpleAIState& Main, const FSimpleAIState& Addition)
{
	return Main + Addition;
}

bool USimpleAILibrary::IsSatisfiedBy(const FSimpleAIState& State, const FSimpleAIState& OtherState)
{
	for (auto &Elem : State.Atoms)
	{
		if (OtherState.Atoms.Contains(Elem.Key))
		{
			if (OtherState.Atoms[Elem.Key] != Elem.Value)
				return false;
		} else if (Elem.Value)
		{
			return false;
		}
	}
	
	return true;
}

FString USimpleAILibrary::StateToString(const FSimpleAIState& State, bool bPrintPretty)
{
	return State.ToString(bPrintPretty);
}
