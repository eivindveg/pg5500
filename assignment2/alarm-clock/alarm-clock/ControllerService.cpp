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
	Serial.print("Read: ");
	Serial.println(read == HIGH ? "high" : "low");

	return read == HIGH;
}

ControllerService::TimeSelection ControllerService::readSelection() {
	int read = analogRead(xPin);
	if (read <= 256) {
		decrementCurrentSelection();
	} else if (read >= 768) {
		incrementCurrentSelection();
	}
	return selection;
}

void ControllerService::incrementCurrentSelection() {
	TimeSelection newSelection;
	switch (selection) {
	case Hour: newSelection = Minute;
		break;
	case Minute: newSelection = Second;
		break;
	case Second: newSelection = Hour;
		break;
	default: newSelection = Hour;
	}
	selection = newSelection;
}

void ControllerService::decrementCurrentSelection() {
	TimeSelection newSelection;
	switch (selection) {
	case Hour: newSelection = Second;
		break;
	case Minute: newSelection = Hour;
		break;
	case Second: newSelection = Minute;
		break;
	default: newSelection = Hour;
	}
	selection = newSelection;
}

