#include "LightsOutGame.h"

LightsOutGame::LightsOutGame(Adafruit_ST7735& tft, uint16_t highScore) {
	this->tft = &tft;
	pinMode(A5, OUTPUT);

	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLUMNS; x++) {
			lights[x][y] = false;
		}
	}

	int selected = 0;
	Serial.println("Seeding lights.");
	randomSeed(analogRead(A4));
	while (selected < 4) {
		uint8_t randomColumn = random(0, 4);
		uint8_t randomRow = random(0, 4);

		Serial.print("Activating light number ");
		Serial.println(selected + 1);

		if (!lights[randomColumn][randomRow]) {
			Serial.print("Setting light at column ");
			Serial.print(randomColumn);
			Serial.print(", row ");
			Serial.print(randomRow);
			Serial.println(" to active.");
			lights[randomColumn][randomRow] = true;
			selected += 1;
		} else {
			Serial.print("Skipping light at column ");
			Serial.print(randomColumn);
			Serial.print(", row ");
			Serial.print(randomRow);
			Serial.println(", already active.");
		}
	}
	Serial.println("Lights seeded.");

	this->tft->setCursor(10, (this->tft->height() / 2) - 10);
	this->tft->setTextColor(ST7735_WHITE);
	this->tft->print("Moves:");
	this->tft->setCursor(10, (this->tft->height() / 2));
	this->tft->print(moves);
	this->tft->setCursor(10, (this->tft->height() / 2) + 20);
	this->tft->print("High score:");
	this->tft->setCursor(10, (this->tft->height() / 2) + 30);
	this->tft->print(highScore);


	drawLights();
}

LightsOutGame::~LightsOutGame() {
}

void LightsOutGame::handleMove(InputType moveDirection) {
	uint8_t oldX = currentX;
	uint8_t oldY = currentY;
	if (moveDirection == InputType::RIGHT && currentX < COLUMNS - 1) {
		currentX++;
	} else if (moveDirection == InputType::LEFT && currentX > 0) {
		currentX--;
	} else if (moveDirection == InputType::DOWN && currentY < ROWS - 1) {
		currentY++;
	} else if (moveDirection == InputType::UP && currentY > 0) {
		currentY--;
	}

	tone(A5, MOVE_SOUND_FREQ, 100);

	drawLight(oldX, oldY);
	drawLight(currentX, currentY);
}

bool LightsOutGame::handleClick() {
	toggleLight(currentX, currentY);

	tone(A5, CLICK_SOUND_FREQ, 100);

	if (currentX >= 1) {
		toggleLight(currentX - 1, currentY);
	}
	if (currentX < COLUMNS - 1) {
		toggleLight(currentX + 1, currentY);
	}
	if (currentY >= 1) {
		toggleLight(currentX, currentY - 1);
	}
	if (currentY < ROWS - 1) {
		toggleLight(currentX, currentY + 1);
	}

	incrementMoves();

	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			if (lights[x][y]) {
				return false;
			}
		}
	}

	playVictoryTune();

	return true;
}

uint16_t LightsOutGame::getMoves() {
	return moves;
}

void LightsOutGame::incrementMoves() {
	// Do not allow overflows; you've pretty much failed the game if you get to this point(though you could argue your patience should be rewarded):
	if (moves == 65535) {
		return;
	}

	tft->setCursor(10, tft->height() / 2);
	tft->setTextColor(ST7735_BLACK);
	tft->print(moves);
	tft->setCursor(10, tft->height() / 2);
	moves++;
	tft->setTextColor(ST7735_WHITE);
	tft->print(moves);
}

void LightsOutGame::toggleLight(uint8_t x, uint8_t y) {
	lights[x][y] = !lights[x][y];
	drawLight(x, y);
}

void LightsOutGame::drawLights() {
	Serial.println("Drawing all lights.");
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLUMNS; x++) {
			drawLight(x, y);
		}
	}
}

void LightsOutGame::drawLight(uint8_t x, uint8_t y) {
	int16_t xPos = getXPositionForLight(x);
	int16_t yPos = getYPositionForLight(y);
	uint16_t color = lights[x][y] ? ST7735_YELLOW : 0x0333;

	tft->fillCircle(xPos, yPos, 7, color);
	if (currentX == x && currentY == y) {
		tft->drawCircle(xPos, yPos, 7, ST7735_CYAN);
		tft->drawCircle(xPos, yPos, 6, ST7735_CYAN);
	}
}

void LightsOutGame::playVictoryTune() {
	for (int thisNote = 0; thisNote < NOTES_IN_VICTORY_TUNE; thisNote++) {

		int noteDuration = 1000 / victoryTuneDurations[thisNote];
		tone(A5, victoryTune[thisNote], noteDuration);
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);
		noTone(A5);
	}
}

int16_t LightsOutGame::getXPositionForLight(uint8_t x) {
	return (tft->width() / 2) + x * 17;
}

int16_t LightsOutGame::getYPositionForLight(uint8_t y) {
	return y * 17 + 30;
}
