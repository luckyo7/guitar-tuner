#include <Arduino.h>
#include <i2s.h>
#include <note.h>
#include <sampling.h>
#include <screen.h>

Mode mode = getCurrentMode();
void loop() {
  sampling_loop();
  // currentMode = getCurrentMode();
  // mode = getCurrentMode();
  // drawMode(mode);
  // Serial.println(getCurrentMode().modeName);
}

void setup() {
  Serial.begin(115200);

  screenSetup();
  display.clearDisplay();

  drawNote("A", "b");
  drawMode(mode);
  drawPitch(-0.5);

  // i2s setup
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  delay(500);
}