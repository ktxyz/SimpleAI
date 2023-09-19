// Copyright @ 2023 Kamil Tokarski


#include "SimpleClockConfig.h"
#include "SimpleClockSubsystem.h"


float USimpleClockConfig::GetCurrentTimeOfDayRealLength(ETimeOfDay TimeOfDay) const
{
	// Naming :(
	const int HoursInMorning = abs(DayStartHour - NightStartHour);
	const int HoursInNight = HoursInDay - HoursInMorning; 
	const int NumberOfHours = (TimeOfDay == ETimeOfDay::Day) ? HoursInMorning : HoursInNight;

	const float NumberOfMinutes = NumberOfHours * MinutesInHour;
	const float DurationOfTimeOfDay = (TimeOfDay == ETimeOfDay::Day) ? DayLengthInRealSeconds : NightLengthInRealSeconds;

	return DurationOfTimeOfDay / NumberOfMinutes * 60.f;
}
