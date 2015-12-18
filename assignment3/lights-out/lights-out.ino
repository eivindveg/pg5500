#pragma once
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <SD.h>

#include "LightsOutManager.h"

LightsOutManager* manager;

void setup() {
	Serial.begin(19200);
	manager = new LightsOutManager(10, 9, 8);

	Serial.println("Setup complete.");
}

void loop() {
	manager->handle();
}