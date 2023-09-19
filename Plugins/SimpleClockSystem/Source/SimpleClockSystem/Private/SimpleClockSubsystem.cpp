// Copyright @ 2023 Kamil Tokarski


#include "SimpleClockSubsystem.h"
#include "SimpleClockConfig.h"

void USimpleClockSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Config = GetMutableDefault<USimpleClockConfig>();
	const float CurrentTimeOfDayMinuteLength = Config->GetCurrentTimeOfDayRealLength(WorldTime.TimeOfDay);
	TimeSinceLastTick += DeltaTime;

	if (TimeSinceLastTick >= CurrentTimeOfDayMinuteLength)
	{
		TimeSinceLastTick -= CurrentTimeOfDayMinuteLength;

		const ETimeOfDay PreviousTimeOfDay = WorldTime.TimeOfDay;
		WorldTime.ProgressMinutes(Config->HoursInDay, Config->MinutesInHour, Config->DayStartHour, Config->NightStartHour);
		if (PreviousTimeOfDay != WorldTime.TimeOfDay)
			OnTimeOfDayChanged.Broadcast(WorldTime.TimeOfDay);
		OnWorldTimeChanged.Broadcast(WorldTime);
	}
}
