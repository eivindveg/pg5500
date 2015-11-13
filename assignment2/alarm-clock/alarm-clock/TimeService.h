#pragma once
#include <Wire.h>
#include <RTClib.h>

class TimeService {
public:
	TimeService();
	DateTime now();
private:
	RTC_DS1307 rtc;
};

