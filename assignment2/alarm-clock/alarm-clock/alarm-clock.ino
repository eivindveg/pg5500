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

	
	DateTime alarm = timeServer->now() + TimeSpan(0, 0, 0, 3);
	alarmService->setAlarm(alarm);
	Serial.println("Setup complete.");
}

void endCycle(unsigned long start, unsigned long end) {
	uint16_t elapsed = end - start;

	Serial.print("Cycle completed in ");
	Serial.print(elapsed);
	Serial.println(" millis.");

	if(elapsed < INTERVAL) {
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

void loop() {
	unsigned long start = millis();

	DateTime now = timeServer->now();
	alarmService->checkAlarm(now);

	updateTimeDisplay();
	updateAlarmDisplay();

	unsigned long end = millis();
	endCycle(start, end);
}

