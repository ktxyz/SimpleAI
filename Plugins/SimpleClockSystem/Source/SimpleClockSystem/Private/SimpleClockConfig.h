// Copyright @ 2023 Kamil Tokarski

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleClockSubsystem.h"
#include "SimpleClockConfig.generated.h"

/**
 * 
 */
UCLASS(Config=SimpleClockSystem, DefaultConfig)
class SIMPLECLOCKSYSTEM_API USimpleClockConfig : public UObject
{
	GENERATED_BODY()

public:
	USimpleClockConfig() = default;
	float GetCurrentTimeOfDayRealLength(ETimeOfDay TimeOfDay) const;

	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	int HoursInDay = 24;
	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	int MinutesInHour = 60;

	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	int DayStartHour = 6;
	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	int NightStartHour = 22;
	
	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	float DayLengthInRealSeconds = 4.0f;
	UPROPERTY(Config, EditAnywhere, Category = "SimpleClock|Config")
	float NightLengthInRealSeconds = 2.0f;
};
