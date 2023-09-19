// Copyright @ 2023 Kamil Tokarski


#include "SimpleAIState.h"

FSimpleAIState FSimpleAIState::operator+(const FSimpleAIState& Other) const
{
	FSimpleAIState NewState = *this;
	for (auto &Elem : Other.Atoms)
	{
		NewState.Atoms.Add(Elem.Key, Elem.Value);
	}

	if (Other.bIsLocationSet)
	{
		NewState.Location = Other.Location;
		NewState.bIsLocationSet = true;
	}
	
	return NewState;
}

void FSimpleAIState::operator+=(const FSimpleAIState& Other)
{
	const FSimpleAIState NewState = *this + Other;
	this->Atoms = NewState.Atoms;

	if (Other.bIsLocationSet)
	{
		this->Location = Other.Location;
		this->bIsLocationSet = true;
	}
}

FString FSimpleAIState::ToString(bool bPrintPretty) const
{
	FString Result = "";
	for (auto &Elem : Atoms)
	{
		Result += (Elem.Key.ToString() + ": " + (Elem.Value ? "true" : "false")) + (bPrintPretty ? "\n" : "; ");
	}
	if (bIsLocationSet)
	{
		Result += (bPrintPretty ? "\n" : "; ") + ("Location: " + Location.ToString());
	}
	return Result;
}

bool FSimpleAIState::operator==(const FSimpleAIState& Other) const
{
	for (auto &Elem : Other.Atoms)
	{
		if (this->Atoms.Contains(Elem.Key))
		{
			if (this->Atoms[Elem.Key] != Elem.Value)
				return false;
		} else if (Elem.Value)
		{
			return false;
		}
	}
	return true;
}

void FSimpleAIState::UpdateLocation(const FVector& NewLocation)
{
	this->Location = NewLocation;
	this->bIsLocationSet = true;
}

int FSimpleAIState::HeuristicDistance(const FSimpleAIState& Other) const
{
	int Distance = 0;

	for (auto &Elem : Other.Atoms)
	{
		if (this->Atoms.Contains(Elem.Key))
		{
			if (this->Atoms[Elem.Key] != Elem.Value)
				Distance++;
		}
		else if (Elem.Value)
		{
			Distance++;
		}
	}

	if (Other.bIsLocationSet && this->Location != Other.Location)
		Distance++;
	
	return Distance;
}
