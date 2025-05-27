#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <screen.h>


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

#define TEXT_SIZE 3 // magnification of text size

int yellowHeight = 16; // height of the yellow section of the display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int noteX = SCREEN_WIDTH/2 - TEXT_SIZE*2;
int noteY = SCREEN_HEIGHT/2 - TEXT_SIZE - 10;
int accidentalX;

void drawNote(String note, String accidental) {
  display.setTextSize(TEXT_SIZE);
  display.setTextColor(WHITE);
  if (accidental != "") {
    display.setCursor(noteX - TEXT_SIZE*2, noteY);
    accidentalX = noteX - TEXT_SIZE*2;
  } else {
    display.setCursor(noteX, noteY);
    accidentalX = noteX;
  }
  display.println(note);

  display.setTextSize(TEXT_SIZE*0.7);
  display.setCursor(accidentalX + 6*TEXT_SIZE, noteY);
  display.println(accidental);

  display.display();

}

int pitchBarBuffer = 10; // Margin between pitch bar and the side of the screen
int pitchBarWidth = SCREEN_WIDTH - 2*pitchBarBuffer;
int pitchBarHeight = 5; // thickness of the bar
int pitchBarX = pitchBarBuffer;
int pitchBarY = SCREEN_HEIGHT - pitchBarHeight - 5;

int noteLineX = SCREEN_WIDTH/2;
int noteLineY1 = pitchBarY - 2;
int noteLineY2 = pitchBarY + pitchBarHeight + 2;

void drawPitch(float error) {
  // Draws the bar showing how far off the pitch is
  // error is a decimal between [-1 1] where 0 is perfectly in tune
  display.drawRect(pitchBarX, pitchBarY, pitchBarWidth, pitchBarHeight, WHITE); // Draw the empty rectangle that will fill up to display pitch
  
  // draw reference line for perfect pitch
  display.drawLine(noteLineX, noteLineY1, noteLineX, noteLineY2, WHITE);

  int errorWidth = pitchBarWidth/2*error;
  int errorX = noteLineX + errorWidth;
  
  // draw filled rectangle to show pitch error
  display.fillRect(min(errorX, noteLineX), pitchBarY, abs(errorWidth), pitchBarHeight, WHITE);

  display.display();
}

void screenSetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}
// void setup() {
//   Serial.begin(9600);

//   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
//     Serial.println(F("SSD1306 allocation failed"));
//     for(;;); // Don't proceed, loop forever
//   }
//   display.clearDisplay();

//   drawNote("F", "#");
//   drawPitch(-0.5);
// }