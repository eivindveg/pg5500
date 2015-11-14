#include "TimeService.h"

TimeService::TimeService() {
	while (!rtc.begin()) {
		Serial.println("RTC IS NOT RUNNING!");
	}

	if (!rtc.isrunning()) {
		Serial.println("Starting RTC");
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
	else {
		Serial.println("RTC was already running.");
	}
}

DateTime TimeService::now() const {
	return rtc.now();
}
