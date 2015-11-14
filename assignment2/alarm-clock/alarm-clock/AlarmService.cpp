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

	if(isAlarmSet()) {
		TimeSpan diff = *alarm - now;
		if(diff.seconds() <= 0) {
			play();
		}
	}
}

void AlarmService::play() const {
	tone(speakerPin, 2000, 450);

	delay(600);
}
