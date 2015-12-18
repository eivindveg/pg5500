#include "LightsOutManager.h"
#include "InputType.h"

LightsOutManager::LightsOutManager(uint8_t CS, uint8_t RS, uint8_t RST) {
	Serial.print("CS: ");
	Serial.println(CS);
	Serial.print("RS: ");
	Serial.println(RS);
	Serial.print("RST: ");
	Serial.println(RST);

	inputManager = new InputManager(A1, A2, A0);

	tft = new Adafruit_ST7735(CS, RS, RST);
	tft->initR(INITR_BLACKTAB);
	tft->setRotation(3);
	tft->fillScreen(ST7735_BLACK);

	if (!SD.begin(4)) {
		tft->println("No SD.");
	}

	Serial.println("LightsOutManager set up.");
}

void LightsOutManager::handle() {
	uint16_t highScore = getHighScore();
	if (game == NULL) {
		InputType input;
		while ((input = inputManager->getInput()) != InputType::CLICK) {
			delay(10);
		}
		Serial.println("Creating game");
		uint16_t highScore = getHighScore();
		game = new LightsOutGame(*tft, highScore);
	}

	InputType input = inputManager->getInput();
	if (lastInput == input) {
		return;
	} else {
		if (input == InputType::CLICK) {
			if (game->handleClick()) {
				uint16_t moves = game->getMoves();
				delete game;
				game = NULL;
				if (moves < highScore || highScore == 0) {
					saveHighScore(moves);
				}
				tft->fillScreen(ST7735_BLACK);
				tft->println("WIN");
			}
		} else if (input != InputType::NONE) {
			game->handleMove(input);
		}
	}
	lastInput = input;
}


uint16_t LightsOutManager::getHighScore() {
	File file = SD.open("scores.txt", FILE_READ);
	if (file) {
		String contents = "";
		while (file.available()) {
			char c = file.read();
			contents += c;
		}

		Serial.print("Read high score: ");
		Serial.println(contents);
		file.close();

		return (uint16_t) atoi(contents.c_str());
	}
	return 0;
}

void LightsOutManager::saveHighScore(uint16_t newHighScore) {
	File file = SD.open("scores.txt", FILE_WRITE);
	if (file) {
		file.println(newHighScore);
		file.close();
	}
}
