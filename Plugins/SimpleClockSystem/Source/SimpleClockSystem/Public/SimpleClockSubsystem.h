// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SimpleClockSubsystem.generated.h"

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Day,
	Night
};

USTRUCT(BlueprintType, Category = "SimpleClock|Time")
struct FWorldTimeStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SimpleClock|Time")
	int CurrentHour;
	UPROPERTY(BlueprintReadWrite, Category = "SimpleClock|Time")
	int CurrentMinute;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleClock|Time")
	int CurrentDay;

	UPROPERTY(BlueprintReadWrite, Category = "SimpleClock|Time")
	ETimeOfDay TimeOfDay;

	FWorldTimeStruct()
	{
		CurrentHour = 0;
		CurrentMinute = 0;
		CurrentDay = 0;
		TimeOfDay = ETimeOfDay::Day;
	}
	
	void ProgressMinutes(const int NumberOfHours, const int NumberOfMinutes, int DayBeginHour, int NightBeginHour)
	{
		CurrentMinute += 1;
		if (CurrentMinute >= NumberOfMinutes)
		{
			CurrentMinute = 0;
			CurrentHour += 1;
			if (CurrentHour >= NumberOfHours)
			{
				CurrentHour = 0;
				CurrentDay += 1;
			}
		}
		
		if ((CurrentHour >= NightBeginHour && CurrentHour <= NumberOfHours) || (CurrentHour >= 0 && CurrentHour <= DayBeginHour))
			TimeOfDay = ETimeOfDay::Night;
		else
			TimeOfDay = ETimeOfDay::Day;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldTimeChangedDelegate, FWorldTimeStruct, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeOfDayChangedDelegate, ETimeOfDay, NewTimeOfDay);

/**
 * 
 */
UCLASS()
class SIMPLECLOCKSYSTEM_API USimpleClockSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USimpleClockSubsystem, STATGROUP_Tickables);}
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "SimpleClock|Time")
	void SetWorldTime(FWorldTimeStruct NewTime) { WorldTime = NewTime; OnWorldTimeChanged.Broadcast(WorldTime); };	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleClock|Time")
	FWorldTimeStruct GetWorldTime() const { return WorldTime; }
private:
	float TimeSinceLastTick = 0.0f;
	FWorldTimeStruct WorldTime;
public:
	UPROPERTY(BlueprintAssignable, Category = "SimpleClock|Time")
	FOnWorldTimeChangedDelegate OnWorldTimeChanged;
	UPROPERTY(BlueprintAssignable, Category = "SimpleClock|Time")
	FOnTimeOfDayChangedDelegate OnTimeOfDayChanged;
};
