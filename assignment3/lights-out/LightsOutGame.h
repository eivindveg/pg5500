#pragma once
#include <Adafruit_ST7735.h>
#include "pitches.h"
#include "InputType.h"

#define COLUMNS 6
#define ROWS 5
#define STARTS_ON 6
#define MOVE_SOUND_FREQ 196
#define CLICK_SOUND_FREQ 262
#define NOTES_IN_VICTORY_TUNE 9

class LightsOutGame {
public:
	LightsOutGame(Adafruit_ST7735& tft, uint16_t highScore);

	virtual ~LightsOutGame();

	virtual void handleMove(InputType moveDirection);
	virtual bool handleClick();
	virtual uint16_t getMoves();
private:
	// notes in the melody:
	int victoryTune[NOTES_IN_VICTORY_TUNE] = {
		NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_GS4, NOTE_AS4, NOTE_C4, NOTE_AS4, NOTE_C4
	};

	// note durations: 4 = quarter note, 8 = eighth note, etc.:
	int victoryTuneDurations[NOTES_IN_VICTORY_TUNE] = {
		8, 8, 8, 4, 4, 4, 4, 8, 4
	};

	virtual void incrementMoves();
	virtual void toggleLight(uint8_t x, uint8_t y);
	virtual void drawLights();
	virtual void drawLight(uint8_t x, uint8_t y);
	virtual void playVictoryTune();
	int16_t getXPositionForLight(uint8_t x);
	int16_t getYPositionForLight(uint8_t y);
	uint16_t moves = 0;
	Adafruit_ST7735 *tft;
	bool lights[COLUMNS][ROWS];
	uint8_t currentX = 2;
	uint8_t currentY = 2;
};

