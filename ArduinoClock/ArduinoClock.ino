
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>

// If using software SPI (the default case):
#define OLED_MOSI   A4
#define OLED_CLK   A5
#define OLED_DC    8
#define OLED_CS    12
#define OLED_RESET 9
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int date = 0;
int currentDay = 0;
String month = "0";
String year = "0";
String days[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
};

int hour = 0;
int minute = 0;
int second = 0;

String meridiem = "AM";

void setup() {
	Serial.begin(9600);
	display.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {

	if (Serial.available()) {
		String varName = Serial.readStringUntil(':');
		if (varName.equals("hour"))
		{
			hour = (Serial.readStringUntil(';')).toInt();
		}
		else if (varName.equals("minute"))
		{
			minute = (Serial.readStringUntil(';')).toInt();
		}
		else if (varName.equals("second"))
		{
			second = (Serial.readStringUntil(';')).toInt();
		}
		else if (varName.equals("date"))
		{
			date = (Serial.readStringUntil(';')).toInt();
		}
		else if (varName.equals("month"))
		{
			month = Serial.readStringUntil(';');
		}
		else if (varName.equals("year"))
		{
			year = Serial.readStringUntil(';');
		}
		else if (varName.equals("day"))
		{
			currentDay = (Serial.readStringUntil(';')).toInt();
		}
		else if (varName.equals("meridiem"))
		{
			meridiem = Serial.readStringUntil(';');
		}
	}

	second++;

	if (second > 59)
	{
		second = 0;
		minute++;
	}

	if (minute > 59)
	{
		minute = 0;
		hour++;
	}

	if (hour > 12)
	{
		hour = 1;

		if (meridiem == "AM")
		{
			meridiem = "PM";
		}
		else {
			meridiem = "AM";
			date++;
			currentDay++;
		}
	}

	clock();
	delay(970);
}


void clock(void) {

	display.clearDisplay();
	display.setTextColor(WHITE);

	String dateString = "";
	if (date < 10)
	{
		dateString = "0" + String(date);
	}
	else {
		dateString = String(date);
	}

	display.setFont();
	display.setTextSize(1);
	display.setCursor(25, 0);
	display.print(dateString + " / " + month + " / " + year);

	display.setFont();
	display.setTextSize(1);
	display.setCursor(40, 8);
	display.println(days[currentDay]);

	display.setFont(&FreeSerifBold12pt7b);
	display.setTextSize(1);
	display.setCursor(0, 31);

	String hourString = "";
	String minuteString = "";
	String secondString = "";

	if (hour < 10)
	{
		hourString = "0" + String(hour);
	}
	else {
		hourString = String(hour);
	}

	if (minute < 10)
	{
		minuteString = "0" + String(minute);
	}
	else {
		minuteString = String(minute);
	}

	if (second < 10)
	{
		secondString = "0" + String(second);
	}
	else {
		secondString = String(second);
	}
	display.print(hourString + " : " + minuteString);

	display.setFont(&FreeSerifBold9pt7b);
	display.setTextSize(1);
	display.setCursor(75, 31);
	display.println(secondString);

	display.setFont(&FreeMonoBold9pt7b);
	display.setTextSize(1);
	display.setCursor(100, 31);
	display.println(meridiem);

	display.display();
}