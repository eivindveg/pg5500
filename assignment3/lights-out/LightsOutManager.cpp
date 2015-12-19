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

	Serial.println("LightsOutManager set up.");

	if (!SD.begin(4)) {
		tft->println("No SD.");
	}
}

void LightsOutManager::handle() {
	uint16_t highScore = getHighScore();
	if (game == NULL) {
		drawStart();
		InputType input;
		while ((input = inputManager->getInput()) != InputType::CLICK) {
			delay(10);
		}
		Serial.println("Creating game");
		tft->fillScreen(ST7735_BLACK);
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
			}
		} else if (input != InputType::NONE) {
			game->handleMove(input);
		}
	}
	lastInput = input;
}


uint16_t LightsOutManager::getHighScore() {
	if (this->highScore == 0) {
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

			this->highScore = (uint16_t)atoi(contents.c_str());
		}
	}
	return this->highScore;
}

void LightsOutManager::saveHighScore(uint16_t newHighScore) {
	this->highScore = newHighScore;
	File file = SD.open("scores.txt", FILE_WRITE);
	if (file) {
		file.println(newHighScore);
		file.close();
	}
}

void LightsOutManager::drawStart() {
	String title = "Lights Out";
	String text = "Press button to start.";
	int16_t screenCenter = tft->width() / 2;
	uint8_t titleOffset = (title.length() / 2) * 6;

	tft->setCursor(screenCenter - titleOffset, 40);
	tft->println(title);

	uint8_t textOffset = (text.length() / 2) * 6;
	tft->setCursor(screenCenter - textOffset, 50);
	tft->println(text);
}
