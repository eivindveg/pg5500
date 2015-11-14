#pragma once
#include <Arduino.h>
class ControllerService {

public:
	enum TimeSelection {
		Hour,
		Minute,
		Second
	};

	enum TimeAdjustment {
		Increment,
		None,
		Decrement
	};
	ControllerService(uint8_t vrX, uint8_t vrY, uint8_t sw);
	bool isClicked();
	TimeSelection readSelection();

private:
	uint8_t xPin;
	uint8_t yPin;
	uint8_t buttonPin;
	TimeSelection selection;

	void incrementCurrentSelection();
	void decrementCurrentSelection();
};
