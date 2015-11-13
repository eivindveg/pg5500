#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <TFT.h>
#include <SoftwareSerial.h>
#include <doxygen.h>

#include "TimeService.h"
#include "AlarmDisplay.h"


// RTC clock
TimeService* timeServer;

AlarmDisplay* alarmDisplay;

void setup() {
	Serial.begin(19200);
	timeServer = new TimeService();
	alarmDisplay = new AlarmDisplay(10, 9, 8);

	Serial.println("Setup complete.");
}

void loop() {
	if (millis() % 50 == 0) {
		DateTime now = timeServer->now();
		alarmDisplay->updateTime(now);
	}

}

