#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <TFT.h>
#include <SoftwareSerial.h>
#include <doxygen.h>

#include "TimeService.h"
#include "AlarmDisplay.h"
#include "ControllerService.h"
#include "AlarmService.h"

#define INTERVAL 100


// RTC clock
TimeService* timeServer;

AlarmService* alarmService;

AlarmDisplay* alarmDisplay;

ControllerService* controllerService;

void setup() {
	Serial.begin(19200);
	timeServer = new TimeService();
	alarmService = new AlarmService(6, 5);
	alarmDisplay = new AlarmDisplay(10, 9, 8);
	controllerService = new ControllerService(A2, A1, A0);

	Serial.println("Setup complete.");
}

void endCycle(unsigned long start, unsigned long end) {
	uint16_t elapsed = end - start;

	Serial.print("Cycle completed in ");
	Serial.print(elapsed);
	Serial.println(" millis.");

	if (elapsed < INTERVAL) {
		delay(INTERVAL - elapsed);
	}
}

void updateTimeDisplay() {
	DateTime now = timeServer->now();
	alarmDisplay->updateTime(now);
}

void updateAlarmDisplay() {
	DateTime* alarm = alarmService->getAlarm();
	alarmDisplay->updateAlarm(alarm);
}

void handleControllerInput() {
	bool clicked = controllerService->isClicked();

	if (clicked) {
		Serial.println("Setting edit mode");
		if (alarmDisplay->isEditMode()) {
			alarmDisplay->setEditMode(false);
		} else {
			alarmDisplay->setEditMode(true);
		}
		delay(500);
	} else {
		ControllerService::TimeAdjustment readAdjustment = controllerService->readAdjustment();

		bool isAlarmSet = alarmService->isAlarmSet();
		bool isEditMode = alarmDisplay->isEditMode();

		Serial.print("Alarm set: ");
		Serial.println(isAlarmSet);
		Serial.print("Edit mode: ");
		Serial.println(isEditMode);
		Serial.print("No adjustment: ");
		Serial.println(readAdjustment == ControllerService::None);
		ControllerService::TimeSelection readSelection = controllerService->readSelection();
		alarmDisplay->setSelection(readSelection);
		if (isEditMode && !isAlarmSet) {
			Serial.println("Creating alarm");
			alarmService->createAlarm();

		} else if (isEditMode && isAlarmSet && readAdjustment != ControllerService::None) {
			Serial.println("Updating alarm");
			DateTime* alarm = alarmService->getAlarm();
			DateTime adjusted = *alarm;
			TimeSpan adjustment;
			if (readSelection == ControllerService::Hour) {
				adjustment = TimeSpan(0, 1, 0, 0);
			} else if (readSelection == ControllerService::Minute) {
				adjustment = TimeSpan(0, 0, 1, 0);
			} else {
				adjustment = TimeSpan(0, 0, 0, 0);
			}

			if (readAdjustment == ControllerService::Increment) {
				adjusted = adjusted + adjustment;
			} else if (readAdjustment == ControllerService::Decrement) {
				adjusted = adjusted - adjustment;
			}

			alarmService->setAlarm(adjusted);

			delay(250);
		}
	}

}

void loop() {
	unsigned long start = millis();

	DateTime now = timeServer->now();
	if (!alarmDisplay->isEditMode()) {
		alarmService->checkAlarm(now);
	}

	handleControllerInput();
	updateTimeDisplay();
	updateAlarmDisplay();


	unsigned long end = millis();
	endCycle(start, end);
}

