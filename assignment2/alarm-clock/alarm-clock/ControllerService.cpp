#include "ControllerService.h"

ControllerService::ControllerService(uint8_t vrX, uint8_t vrY, uint8_t sw) {
	xPin = vrX;
	yPin = vrY;
	buttonPin = sw;
	Serial.println("Settings pin modes for joystick.");
	pinMode(vrX, INPUT);
	pinMode(vrY, INPUT);
	pinMode(sw, OUTPUT);

	Serial.println("Powering up button");
	digitalWrite(sw, HIGH);
}

bool ControllerService::isClicked() {
	int read = digitalRead(buttonPin);
	return read == LOW;
}

ControllerService::TimeSelection ControllerService::readSelection() {
	int read = analogRead(yPin);
	if (read <= 256 || read >= 768) {
		selection = selection == Minute ? Hour : Minute;
	}
	return selection;
}

ControllerService::TimeAdjustment ControllerService::readAdjustment() const {
	int read = analogRead(xPin);
	if(read >= 768) {
		return Decrement;
	}
	if(read <= 256) {
		return Increment;
	}
	return None;
}
