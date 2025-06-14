#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <note.h>
#include <screen.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

#define TEXT_SIZE 3 // magnification of text size

int yellowHeight = 16; // height of the yellow section of the display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int noteX = SCREEN_WIDTH / 2 - TEXT_SIZE * 2;
int noteY = SCREEN_HEIGHT / 2 - TEXT_SIZE - 10;
int accidentalX;

void drawNote(const String &note, const String &accidental) {
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);

  Serial.println(note);
  if (accidental != "") {
    display.setCursor(noteX - TEXT_SIZE * 2, noteY);
    accidentalX = noteX - TEXT_SIZE * 2;
  } else {
    display.setCursor(noteX, noteY);
    accidentalX = noteX;
  }
  display.print(note);

  display.setTextSize(TEXT_SIZE * 0.7);
  display.setCursor(accidentalX + 6 * TEXT_SIZE, noteY);
  display.println(accidental);

  display.display();
}

int pitchBarBuffer = 10; // Margin between pitch bar and the side of the screen
int pitchBarWidth = SCREEN_WIDTH - 2 * pitchBarBuffer;
int pitchBarHeight = 5; // thickness of the bar
int pitchBarX = pitchBarBuffer;
int pitchBarY = SCREEN_HEIGHT - pitchBarHeight - 5;

int noteLineX = SCREEN_WIDTH / 2;
int noteLineY1 = pitchBarY - 2;
int noteLineY2 = pitchBarY + pitchBarHeight + 2;

void drawPitch(float error) {
  // Draws the bar showing how far off the pitch is
  // error is a decimal between [-1 1] where 0 is perfectly in tune
  display.drawRect(
      pitchBarX, pitchBarY, pitchBarWidth, pitchBarHeight,
      WHITE); // Draw the empty rectangle that will fill up to display pitch

  // draw reference line for perfect pitch
  display.drawLine(noteLineX, noteLineY1, noteLineX, noteLineY2, WHITE);

  // clamp error between [-1, 1]
  if (error > 1.0) {
    error = 1.0;
  } else if (error < -1.0) {
    error = -1.0;
  }
  int errorWidth = pitchBarWidth / 2 * error;
  int errorX = noteLineX + errorWidth;

  // draw filled rectangle to show pitch error
  display.fillRect(min(errorX, noteLineX), pitchBarY, abs(errorWidth),
                   pitchBarHeight, WHITE);

  display.display();
}

void drawMode(const String &modeName, const String &noteString) {
  // Serial.println(modeName);
  display.setTextSize(1);
  display.setCursor(1, 1);
  display.println(modeName);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(noteString, 1, 1, &x1, &y1, &w, &h);

  int notesX = SCREEN_WIDTH - w;
  display.setCursor(notesX, 1);
  display.println(noteString);

  display.display();
}

void clearNote() {
  // clear the note section of the oled screen
  display.fillRect(0, yellowHeight, SCREEN_WIDTH, SCREEN_HEIGHT * 0.5, BLACK);

  display.display();
}

void clearMode() {
  // clear top 16 rows
  display.fillRect(0, 0, SCREEN_WIDTH, yellowHeight, BLACK);
  // display.clearDisplay();
  display.display();
}

void clearPitch() {
  // clear pitch error box
  display.fillRect(pitchBarX, pitchBarY, pitchBarWidth, pitchBarHeight,
                   BLACK); // remove entire section

  // fill the empty box back in
  display.drawRect(
      pitchBarX, pitchBarY, pitchBarWidth, pitchBarHeight,
      WHITE); // Draw the empty rectangle that will fill up to display pitch

  // draw reference line for perfect pitch
  display.drawLine(noteLineX, noteLineY1, noteLineX, noteLineY2, WHITE);

  display.display();
}

void screenSetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}