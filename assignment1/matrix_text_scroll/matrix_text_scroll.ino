#include <LedControl.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define MATRIX_SIZE 8

/**
 * pin 12: DataIn
 * pin 11: CLK
 * pin 10: LOAD
 */
LedControl leds = LedControl(10, 9, 8, 1);

/**
 * Use an LCD display for displaying status(READY/DRAWING); application only accepts input while READY
 * Also display speed
 */
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/**
 * Related to lettering
 */
#define A {B00000000, B00111100, B01000010, B01000010, B01111110, B01000010, B01000010, B01000010}
#define B {B00000000, B01111100, B01000010, B01111100, B01000010, B01000010, B01000010, B01111100}
#define C {B00000000, B00111100, B01000010, B01000000, B01000000, B01000010, B01000010, B00111100}
#define D {B00000000, B01111100, B01000010, B01000010, B01000010, B01000010, B01000010, B01111100}
#define E {B00000000, B01111110, B01000000, B01000000, B01111110, B01000000, B01000000, B01111110}
#define F {B00000000, B01111110, B01000000, B01000000, B01111110, B01000000, B01000000, B01000000}
#define G {B00000000, B00111100, B01000010, B01000000, B01000000, B01001110, B01000010, B00111100}
#define H {B00000000, B01000010, B01000010, B01000010, B01111110, B01000010, B01000010, B01000010}
#define I {B00000000, B00010000, B00000000, B00010000, B00010000, B00010000, B00010000, B00010000}
#define J {B00000000, B00000100, B00000100, B00000100, B00000100, B01000100, B01000100, B00111000}
#define K {B00000000, B01000010, B01000100, B01001000, B01110000, B01001000, B01000100, B01000010}
#define L {B00000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01111110}
#define M {B00000000, B01000010, B01100110, B01011010, B01000010, B01000010, B01000010, B01000010}
#define N {B00000000, B00000000, B01000010, B01100010, B01010010, B01001010, B01000110, B01000010}
#define O {B00000000, B00111100, B01000010, B01000010, B01000010, B01000010, B01000010, B00111100}
#define P {B00000000, B01111100, B01000010, B01000010, B01111100, B01000000, B01000000, B01000000}
#define Q {B00000000, B00111100, B01000010, B01000010, B01000010, B01001010, B01000100, B00111010}
#define R {B00000000, B01111100, B01000010, B01000010, B01111100, B01001000, B01000100, B01000010}
#define S {B00000000, B00111100, B01000010, B01000000, B00111100, B00000010, B01000010, B00111100}
#define T {B00000000, B01111100, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000}
#define U {B00000000, B01000010, B01000010, B01000010, B01000010, B01000010, B01000010, B00111100}
#define V {B00000000, B01000010, B01000010, B01000010, B01000010, B01000010, B00100100, B00011000}
#define W {B00000000, B01000010, B01000010, B01000010, B01000010, B01011010, B01100110, B01000010}
#define X {B00000000, B00000000, B01000010, B00100100, B00011000, B00011000, B00100100, B01000010}
#define Y {B00000000, B00000000, B01000010, B00100100, B00011000, B00010000, B00100000, B01000000}
#define Z {B00000000, B01111110, B00000010, B00000100, B00001000, B00010000, B00100000, B01111110}
static const byte AELIG[] = {B00000000, B00111110, B01001000, B01001000, B01111110, B01001000, B01001000, B01001110};
static const byte OSLASH[] = {B00000000, B00011001, B00100110, B01000110, B01001010, B01010010, B00100100, B01011000};
static const byte ARING[] = {B00011000, B00000000, B00111100, B01000010, B01000010, B01111110, B01000010, B01000010};
static const byte SPACE[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
// Index the characters we can return by simple calculation: this really takes up a lot of memory. =(
static const byte ALPHABET[][8] = {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
static const byte SYMBOLS = sizeof(ALPHABET) / 8;

/**
 * Related to the speed button
 */
#define MAX_SPEED 8
#define SPEED_BUTTON 13
int speed;

void setup() {
  leds.shutdown(0, false);
  leds.setIntensity(0, 1);
  leds.clearDisplay(0);
  Serial.begin(9600);

  pinMode(SPEED_BUTTON, INPUT);

  lcd.begin(16, 2);
  setLcdStatus("READY");
  updateSpeed();
}

void loop() {
  if (Serial.available() > 0) {
    setLcdStatus("DRAWING");
    char c = Serial.read();
    drawFigure(getChar(c));
  } else {
    setLcdStatus("READY");
    int input = digitalRead(SPEED_BUTTON);
    if(input == HIGH) {
      updateSpeed();
    }
    leds.clearDisplay(0);
  }
  delay(100);
}

void setLcdStatus(String status) {
  String text = "Status: " + status;
  int end = text.length();
  lcd.setCursor(0,0);
  lcd.print(text);
  for(int i = end; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print(" ");
  }
}

void updateSpeed() {
  if(speed == MAX_SPEED) {
    speed = 1;
  } else {
    speed++;
  }

  lcd.setCursor(0, 1);
  String text = "Speed : ";
  lcd.print(text);
  for(int i = 0; i < MAX_SPEED; i++) {
    lcd.setCursor(i + text.length(), 1);
    if(i < speed) {
      lcd.print((char) 219);
    } else {
      lcd.print(" ");
    }
  }
}

const byte *getChar(char character) {
  byte value = (byte) character;
  if (value == 0x20) {
    return SPACE;
  }

  // normal ASCII characters
  if ((value >= 0x61 && value <= 0x7F) || (value >= 0xE4)) {
    // XOR against space uppercases a lowercase character
    value = value ^ ' ';
  }
  // extended ascii characters
  if(value > 0x7F) {
    return getSpecialChar(value);
  }
  byte index = value - 0x41;

  // check bounds to avoid getting garbage.
  
  return ALPHABET[index];
}

const byte *getSpecialChar(byte value) {
  if(value == 198) {
    return AELIG;
  }
  if(value == 216) {
    return OSLASH;
  }
  if(value == 197) {
    return ARING;
  }
  
  // Handle potential junk
  return SPACE; 
}

void drawFigure(const byte figure[]) {
  for (int offset = 8; offset > -8; offset--) {
    if (offset >= 0) {
      drawFigureLeft(figure, offset);
    } else {
      drawFigureRight(figure, -offset);
    }
    delay(250 / speed);
  }
}

void drawFigureLeft(const byte figure[], byte offset) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    leds.setRow(0, i, figure[i] >> offset);
  }
}

void drawFigureRight(const byte figure[], byte offset) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    leds.setRow(0, i, figure[i] << offset);
  }
}

void draw(byte row, byte value) {
  leds.setRow(0, row, value);
}

