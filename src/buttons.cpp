#include <Arduino.h>
#include <note.h>
#include <screen.h>
// Control power and mode buttons

#define MODE_BUTTON 1
#define POWER_BUTTON 0

void button_loop() {
  bool modePressed = digitalRead(MODE_BUTTON) == HIGH;
  bool powerPressed = digitalRead(POWER_BUTTON) == HIGH;

  if (modePressed) {
    Serial.println("Incrementing Mode...");
    incrementCurrentMode();

    // reset the display
    const Mode &currentMode = getCurrentMode();
    clearMode();
    drawMode(currentMode.modeName, currentMode.noteString);
    // drawNote("-", "");
    // drawPitch(0.0);

    delay(200);
  }

  if (powerPressed) {
    Serial.println("Power pressed");

    delay(200);
  }
}

void button_setup() {
  pinMode(MODE_BUTTON, INPUT);
  pinMode(POWER_BUTTON, INPUT);
}
