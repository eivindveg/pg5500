#pragma once
#include <TFT.h>
#include <RTClib.h>
#include "ControllerService.h"

class AlarmDisplay {
public:
	AlarmDisplay(uint8_t CS, uint8_t RS, uint8_t RST);
	virtual ~AlarmDisplay();

	void updateTime(DateTime& dateTime);

	void updateAlarm(DateTime* alarm);
	bool isEditMode() const;
	void setEditMode(bool mode);
	void setSelection(ControllerService::TimeSelection selection);
private:
	TFT* tft;
	DateTime* previousDateTime = NULL;
	DateTime* previousAlarm = NULL;
	bool editMode = false;
	String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	ControllerService::TimeSelection selection;
	static String getSuffixForInteger(uint8_t day);
	int16_t getMiddleLeftX(String value, int16_t widthPerSize) const;
	int16_t getMiddleRightX(String value, int16_t widthPerSize) const;
	int16_t getMiddleX(String value, uint8_t widthPerSymbol) const;
	int16_t getOffSet(String value, uint8_t widthPerSymbol) const;
	void setTimeSeparators();

	void drawYear(uint16_t year);
	void drawMonth(uint8_t month);
	void drawDay(uint8_t day);
	void drawDayOfWeek(uint8_t day_of_the_week);
	void drawHour(uint8_t hour);
	void drawMinute(uint8_t minute);
	void drawFully(DateTime& dateTime);
	void drawSecond(uint8_t second);

	void drawAlarmHeader(String value);
	void drawAlarmHour(String value);
	void drawAlarmMinute(String value);
	void drawAlarmSeparator(String separator);
	void drawAlarmFully(DateTime& alarm);
	void clearAlarmDisplay();
};

