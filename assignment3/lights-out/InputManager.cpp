#pragma once
#include "InputManager.h"

InputManager::InputManager(uint8_t vrX, uint8_t vrY, uint8_t sw) {
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

InputType InputManager::getInput() {
	const bool clicked = isClicked();
	if (clicked) {
		return InputType::CLICK;
	}

	const int x = analogRead(xPin);
	const int y = analogRead(yPin);
	if (x <= 256) {
		return InputType::RIGHT;
	} else if (x >= 768) {
		return InputType::LEFT;
	} else if (y <= 256) {
		return InputType::UP;
	} else if (y >= 768) {
		return InputType::DOWN;
	}

	return InputType::NONE;
}


InputManager::~InputManager() {
	digitalWrite(buttonPin, LOW);
}

bool InputManager::isClicked() {
	int read = digitalRead(buttonPin);
	return read == LOW;
}
