// LightsOutManager.h
#pragma once

#include <SD.h>
#include "LightsOutGame.h"
#include "InputManager.h"


class LightsOutManager {
public:
	LightsOutManager(uint8_t CS, uint8_t RS, uint8_t RST);
	void handle();
private:
	Adafruit_ST7735* tft;
	LightsOutGame* game = NULL;
	InputManager* inputManager;
	InputType lastInput;
	virtual uint16_t getHighScore();
	virtual void saveHighScore(uint16_t newHighScore);
};
