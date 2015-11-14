#pragma once
#include <Wire.h>
#include <RTClib.h>

class TimeService {
public:
	TimeService();
	DateTime now() const;
private:
	RTC_DS1307 rtc;
	DateTime* alarm = NULL;
};

