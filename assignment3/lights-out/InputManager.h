#pragma once
#include <Arduino.h>
#include "InputType.h"

class InputManager {
public:
	InputManager(uint8_t vrX, uint8_t vrY, uint8_t sw);
	InputType getInput();
	virtual ~InputManager();
private:
	virtual bool isClicked();
	uint8_t xPin;
	uint8_t yPin;
	uint8_t buttonPin;
};
