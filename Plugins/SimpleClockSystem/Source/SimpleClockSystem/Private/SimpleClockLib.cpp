// Copyright @ 2023 Kamil Tokarski


#include "SimpleClockLib.h"

#include "SimpleClockConfig.h"

int USimpleClockLib::HoursPassed(const FWorldTimeStruct& TimeStart, const FWorldTimeStruct& TimeEnd)
{
	int HoursPassed = 0;
	auto Config = GetMutableDefault<USimpleClockConfig>();
	if (TimeStart.CurrentDay == TimeEnd.CurrentDay)
	{
		HoursPassed = TimeEnd.CurrentHour - TimeStart.CurrentHour;
	}
	else
	{
		HoursPassed = TimeEnd.CurrentHour + (TimeEnd.CurrentDay - TimeStart.CurrentDay) * (Config->HoursInDay) - TimeStart.CurrentHour;
	}
	return HoursPassed;
}
