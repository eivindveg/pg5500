#include "AlarmDisplay.h"

#define TIME_SIZE 2
#define TIME_POSITION_Y 40

#define WEEKDAY_SIZE 2
#define WEEKDAY_POSITION_Y (DATE_POSITION_Y + (DATE_SIZE * HEIGHT_PER_SIZE) + 2)

#define DATE_SIZE 1
#define DATE_POSITION_Y 10

#define WIDTH_PER_SIZE 6
#define HEIGHT_PER_SIZE 8

#define RECT_FILL ST7735_BLACK

AlarmDisplay::AlarmDisplay(uint8_t CS, uint8_t RS, uint8_t RST) {
	Serial.print("CS: ");
	Serial.println(CS);
	Serial.print("RS: ");
	Serial.println(RS);
	Serial.print("RST: ");
	Serial.println(RST);
	tft = new TFT(CS, RS, RST);
	Serial.println("Starting AlarmDisplay service.");
	tft->initR(INITR_BLACKTAB);
	tft->setRotation(3);
	Serial.println("Started TFT screen.");
	tft->background(0, 0, 0);

	setTimeSeparators();
	Serial.println("TFT screen set up.");
}

void AlarmDisplay::updateTime(DateTime& dateTime) {
	if (previousDateTime == NULL) {
		Serial.println("Time was not drawn yet. Drawing fully.");
		drawFully(dateTime);
	} else {
		if (previousDateTime->year() != dateTime.year()) {
			Serial.println("Updating year");
			drawYear(dateTime.year());
		}
		if (previousDateTime->month() != dateTime.month()) {
			Serial.println("Updating month");
			drawMonth(dateTime.month());
		}
		if (previousDateTime->day() != dateTime.day()) {
			Serial.println("Updating day");
			drawDay(dateTime.day());
			drawDayOfWeek(dateTime.dayOfTheWeek());
		}
		if (previousDateTime->hour() != dateTime.hour()) {
			Serial.println("Updating hour");
			drawHour(dateTime.hour());
		}
		if (previousDateTime->minute() != dateTime.minute()) {
			Serial.println("Updating minute");
			drawMinute(dateTime.minute());
		}
		if (previousDateTime->second() != dateTime.second()) {
			Serial.println("Updating second");
			drawSecond(dateTime.second());
		}
		free(previousDateTime);
	}
	previousDateTime = new DateTime(dateTime.unixtime());

}

void AlarmDisplay::drawAlarmFully(DateTime& alarm) {
	drawAlarmHeader("Upcoming alarm:");
	drawAlarmSeparator(":");
	String value = static_cast<String>(alarm.hour());
	if (value.length() == 1) {
		value = "0" + value;
	}
	drawAlarmHour(value);
	value = static_cast<String>(alarm.minute());
	if (value.length() == 1) {
		value = "0" + value;
	}
	drawAlarmMinute(value);
}

void AlarmDisplay::clearAlarmDisplay() {
	drawAlarmHeader("               ");
	drawAlarmSeparator(" ");
	drawAlarmHour("  ");
	drawAlarmMinute("  ");

}

void AlarmDisplay::updateAlarm(DateTime* alarm) {
	if (previousAlarm == NULL && alarm != NULL) {
		drawAlarmFully(*alarm);
	} else if (alarm == NULL) {
		clearAlarmDisplay();
	} else {
		// update changed fields
	}

	free(previousAlarm);
	previousAlarm = new DateTime(*alarm);
}

void AlarmDisplay::drawFully(DateTime& dateTime) {
	drawYear(dateTime.year());
	drawMonth(dateTime.month());
	drawDay(dateTime.day());
	drawDayOfWeek(dateTime.dayOfTheWeek());
	drawHour(dateTime.hour());
	drawMinute(dateTime.minute());
	drawSecond(dateTime.second());
}

void AlarmDisplay::drawSecond(uint8_t second) {
	String value = static_cast<String>(second);
	if (value.length() == 1) {
		value = "0" + value;
	}

	int x = getMiddleRightX(value, WIDTH_PER_SIZE * TIME_SIZE);

	tft->fillRect(x, TIME_POSITION_Y, (value.length() * TIME_SIZE * WIDTH_PER_SIZE) - TIME_SIZE, TIME_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(TIME_SIZE);
	tft->setCursor(x, TIME_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

void AlarmDisplay::drawAlarmHeader(String value) {

	int x = getMiddleX(value, WIDTH_PER_SIZE * 1);

	tft->fillRect(x, 70, (value.length() * 1 * WIDTH_PER_SIZE) - 1, 1 * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(1);
	tft->setCursor(x, 70);
	tft->setTextColor(ST7735_CYAN);
	tft->print(value);
}

void AlarmDisplay::drawAlarmHour(String value) {

	int firstX = getMiddleLeftX(value, 2 * WIDTH_PER_SIZE);
	int secondX = getMiddleX(value, 2 * WIDTH_PER_SIZE);

	int x = (firstX + secondX) / 2;

	tft->fillRect(x, 80, (value.length() * 2 * WIDTH_PER_SIZE) - 2, 2 * HEIGHT_PER_SIZE, RECT_FILL);
	tft->setTextColor(ST7735_RED);
	tft->setCursor(x, 80);
	tft->setTextSize(2);
	tft->print(value);
}

void AlarmDisplay::drawAlarmMinute(String value) {
	int firstX = getMiddleRightX(value, TIME_SIZE * WIDTH_PER_SIZE);
	int secondX = getMiddleX(value, TIME_SIZE * WIDTH_PER_SIZE);

	int x = (firstX + secondX) / 2;

	tft->fillRect(x, 80, (value.length() * TIME_SIZE * WIDTH_PER_SIZE) - TIME_SIZE, TIME_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->setTextColor(ST7735_RED);
	tft->setCursor(x, 80);
	tft->setTextSize(2);
	tft->print(value);
}

void AlarmDisplay::drawAlarmSeparator(String separator) {
	int x = getMiddleX(separator, WIDTH_PER_SIZE * 2);

	tft->fillRect(x, 80, (separator.length() * TIME_SIZE * WIDTH_PER_SIZE) - TIME_SIZE, TIME_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->setTextColor(ST7735_RED);
	tft->setCursor(x, 80);
	tft->setTextSize(2);
	tft->print(separator);
}

AlarmDisplay::~AlarmDisplay() {
}

void AlarmDisplay::drawYear(uint16_t year) {
	String value = static_cast<String>(year);

	int16_t x = getMiddleRightX(value, DATE_SIZE * WIDTH_PER_SIZE);

	tft->fillRect(x, DATE_POSITION_Y, (value.length() * DATE_SIZE * WIDTH_PER_SIZE) - DATE_SIZE, DATE_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(DATE_SIZE);
	tft->setCursor(x, DATE_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

void AlarmDisplay::drawMonth(uint8_t month) {
	String value = months[month - 1];
	if (value.length() == 1) {
		value = "0" + value;
	}

	int16_t x = getMiddleX(value, DATE_SIZE * WIDTH_PER_SIZE);

	tft->fillRect(x, DATE_POSITION_Y, (value.length() * DATE_SIZE * WIDTH_PER_SIZE) - DATE_SIZE, DATE_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(DATE_SIZE);
	tft->setCursor(x, DATE_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

String AlarmDisplay::getSuffixForInteger(uint8_t day) {
	if (day >= 10 && day <= 20) {
		return "th";
	}

	uint8_t lastDigit = day % 10;
	switch (lastDigit) {
	case 1:
		return "st";
		break;
	case 2:
		return "nd";
		break;
	case 3:
		return "rd";
		break;
	default:
		return "th";
		break;
	}
}

int16_t AlarmDisplay::getMiddleLeftX(String value, int16_t widthPerSize) const {
	int16_t screenLeftCenter = tft->width() / 4;
	uint8_t leftOffset = getOffSet(value, widthPerSize);

	return screenLeftCenter - leftOffset;
}

int16_t AlarmDisplay::getMiddleRightX(String value, int16_t widthPerSize) const {
	int16_t screenCenterRight = tft->width() - (tft->width() / 4);
	uint8_t leftOffset = getOffSet(value, widthPerSize);

	return screenCenterRight - leftOffset;
}

void AlarmDisplay::drawDay(uint8_t day) {
	String suffix = getSuffixForInteger(day);
	String value = static_cast<String>(day);
	if (value.length() == 1) {
		value = "0" + value;
	}
	value += suffix;

	int16_t x = getMiddleLeftX(value, DATE_SIZE * WIDTH_PER_SIZE);
	tft->fillRect(x, DATE_POSITION_Y, (value.length() * DATE_SIZE * WIDTH_PER_SIZE) - DATE_SIZE, DATE_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(DATE_SIZE);
	tft->setCursor(x, DATE_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

int16_t AlarmDisplay::getOffSet(String value, uint8_t widthPerSymbol) const {
	uint16_t leftOffset = (value.length() / 2) * widthPerSymbol;
	if (value.length() % 2 == 1) {
		leftOffset += widthPerSymbol / 2;
	}
	return leftOffset;
}

void AlarmDisplay::setTimeSeparators() {
	String separator = ":";
	int middleCenterLeft = getMiddleLeftX(separator, TIME_SIZE * WIDTH_PER_SIZE);
	int middleCenter = getMiddleX(separator, TIME_SIZE * WIDTH_PER_SIZE);
	int middleCenterRight = getMiddleRightX(separator, TIME_SIZE * WIDTH_PER_SIZE);

	int firstX = (middleCenterLeft + middleCenter) / 2;
	tft->textSize(TIME_SIZE);
	tft->setTextColor(ST7735_WHITE);
	tft->setCursor(firstX, TIME_POSITION_Y);
	tft->print(separator);

	int secondX = (middleCenterRight + middleCenter) / 2;
	tft->setCursor(secondX, TIME_POSITION_Y);
	tft->print(separator);
}

int16_t AlarmDisplay::getMiddleX(String value, uint8_t widthPerSymbol) const {
	int16_t screenMiddle = tft->width() / 2;
	uint16_t leftOffset = getOffSet(value, widthPerSymbol);

	return screenMiddle - leftOffset;
}

void AlarmDisplay::drawDayOfWeek(uint8_t dayOfTheWeek) {
	String value = daysOfTheWeek[dayOfTheWeek];

	int16_t x = getMiddleX(value, WEEKDAY_SIZE * WIDTH_PER_SIZE);

	tft->fillRect(x, WEEKDAY_POSITION_Y, (value.length() * WEEKDAY_SIZE * WIDTH_PER_SIZE) - WEEKDAY_SIZE, WEEKDAY_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(WEEKDAY_SIZE);
	tft->setCursor(x, WEEKDAY_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

void AlarmDisplay::drawHour(uint8_t hour) {
	String value = static_cast<String>(hour);
	if (value.length() == 1) {
		value = "0" + value;
	}

	int16_t x = getMiddleLeftX(value, TIME_SIZE * WIDTH_PER_SIZE);

	tft->fillRect(x, TIME_POSITION_Y, (value.length() * TIME_SIZE * WIDTH_PER_SIZE) - TIME_SIZE, TIME_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(TIME_SIZE);
	tft->setCursor(x, TIME_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

void AlarmDisplay::drawMinute(uint8_t minute) {
	String value = static_cast<String>(minute);
	if (value.length() == 1) {
		value = "0" + value;
	}

	int16_t x = getMiddleX(value, TIME_SIZE * WIDTH_PER_SIZE);

	tft->fillRect(x, TIME_POSITION_Y, (value.length() * TIME_SIZE * WIDTH_PER_SIZE) - TIME_SIZE, TIME_SIZE * HEIGHT_PER_SIZE, RECT_FILL);
	tft->textSize(TIME_SIZE);
	tft->setCursor(x, TIME_POSITION_Y);
	tft->setTextColor(ST7735_WHITE);
	tft->print(value);
}

