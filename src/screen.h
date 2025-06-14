#ifndef SCREEN_H
#define SCREEN_H
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

void drawNote(const String &note, const String &accidental);
void drawPitch(float error);
void screenSetup();
void drawMode(const String &modeName, const String &noteString);

void clearNote();
void clearPitch();
void clearMode();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

extern Adafruit_SSD1306 display;
#endif