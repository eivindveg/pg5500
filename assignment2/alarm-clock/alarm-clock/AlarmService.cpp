#include "AlarmService.h"

DateTime* AlarmService::getAlarm() const {
	return alarm;
}

void AlarmService::setAlarm(DateTime& newAlarm) {
	if (alarm != NULL) {
		Serial.println("Freeing old alarm from memory");
		free(alarm);
	}

	alarm = new DateTime(newAlarm);
}

bool AlarmService::isAlarmSet() const {
	return alarm != NULL;
}

void AlarmService::checkAlarm(DateTime& now) {
	if (digitalRead(buttonPin) == HIGH && isAlarmSet()) {
		free(alarm);
		alarm = NULL;
	}

	if (isAlarmSet()) {
		TimeSpan diff = DateTime(now.year(), now.month(), now.day(), alarm->hour(), alarm->minute(), 0) - DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
		if (diff.hours() == 0 && diff.minutes() > -5 && diff.seconds() <= 0) {
			play();
		}
	}
}

DateTime* AlarmService::createAlarm() {
	if (isAlarmSet()) {
		free(alarm);
	}
	alarm = new DateTime(0U, 0U, 0U);
	return alarm;
}

void AlarmService::play() const {
	tone(speakerPin, 2000, 450);

	delay(600);
}

