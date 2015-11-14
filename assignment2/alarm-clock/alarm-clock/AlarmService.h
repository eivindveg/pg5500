#pragma once
#include <Arduino.h>
#include <RTClib.h>

class AlarmService {
public:
	inline AlarmService(uint8_t speakerPin, uint8_t buttonPin) {
		this->speakerPin = speakerPin;
		this->buttonPin = buttonPin;
		pinMode(speakerPin, OUTPUT);
		pinMode(buttonPin, INPUT);
	}

	DateTime* getAlarm() const;
	void setAlarm(DateTime& newAlarm);
	bool isAlarmSet() const;
	
	void checkAlarm(DateTime& now);
	DateTime* createAlarm();
private:
	int8_t speakerPin;
	uint8_t buttonPin;
	DateTime* alarm = NULL;
	void play() const;
};
