#pragma once
#include <Adafruit_ST7735.h>
#include "InputType.h"

#define COLUMNS 5
#define ROWS 5

class LightsOutGame {
public:
	LightsOutGame(Adafruit_ST7735& tft, uint16_t highScore);

	virtual ~LightsOutGame();

	virtual void handleMove(InputType moveDirection);
	virtual bool handleClick();
	virtual uint16_t getMoves();
private:
	virtual void incrementMoves();
	virtual void toggleLight(uint8_t x, uint8_t y);
	virtual void drawLights();
	virtual void drawLight(uint8_t x, uint8_t y);
	int16_t getXPositionForLight(uint8_t x);
	int16_t getYPositionForLight(uint8_t y);
	uint16_t moves = 0;
	Adafruit_ST7735 *tft;
	bool lights[COLUMNS][ROWS];
	uint8_t currentX = 2;
	uint8_t currentY = 2;
};

